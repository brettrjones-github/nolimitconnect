//============================================================================
// Copyright (C) 2022 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include "FileInfoMgr.h"
#include "FileInfo.h"
#include "SharedFilesMgr.h"

#include "PluginBase.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxFileShredder.h>

namespace
{
	//============================================================================
    static void * UpdateFileLibraryThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		FileInfoMgr * poMgr = (FileInfoMgr *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->updateFilesListFromDb( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}

	//============================================================================
    static void * GenHashIdsThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		FileInfoMgr * poMgr = (FileInfoMgr *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->generateHashIds( poThread );
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
FileInfoMgr::FileInfoMgr( P2PEngine& engine, PluginBase& plugin, std::string fileLibraryDbName )
: m_Engine( engine )
, m_Plugin( plugin )
, m_FileShredder( GetVxFileShredder() )
, m_FileInfoDb( fileLibraryDbName )
, m_FileInfoXferMgr( engine, plugin, *this )
{
}

//============================================================================
FileInfoMgr::~FileInfoMgr()
{
	fileLibraryShutdown();
}

//============================================================================
bool FileInfoMgr::isFileShared( VxSha1Hash& fileHashId )
{
	return true;
}

//============================================================================
bool FileInfoMgr::isFileShared( std::string& fileName )
{
	return true;
}

//============================================================================
void FileInfoMgr::fromGuiUserLoggedOn( void )
{
	// user specific directory should be set
	std::string dbName = VxGetUserSpecificDataDirectory() + "settings/";
	dbName += m_FileInfoDb.getFileInfoDbName();
	lockFileList();
	m_FileInfoDb.dbShutdown();
	m_FileInfoDb.dbStartup( 1, dbName );
	unlockFileList();
	if( false == isThreadCreated() )
	{
		std::string fileInfoThread( "FileInfo" );
		fileInfoThread += m_FileInfoDb.getFileInfoDbName();
		startThread( (VX_THREAD_FUNCTION_T)UpdateFileLibraryThreadFunc, this, fileInfoThread.c_str() );
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoMgr::updateFilesList: Thread Still Running" );
	}
}

//============================================================================
void FileInfoMgr::fileLibraryShutdown( void )
{
	m_GenHashThread.abortThreadRun( true );
	killThread();
	lockFileList();
	clearLibraryFileList();
	m_FileInfoDb.dbShutdown();
	unlockFileList();
}

//============================================================================
void FileInfoMgr::addFileToGenHashQue( std::string fileName )
{
	m_GenHashMutex.lock();
	m_GenHashList.push_back( fileName );
	m_GenHashMutex.unlock();
	if( false == m_GenHashThread.isThreadRunning() )
	{
		m_GenHashThread.startThread( (VX_THREAD_FUNCTION_T)GenHashIdsThreadFunc, this, "GenHashIds" );
	}
}

//============================================================================
void FileInfoMgr::generateHashIds( VxThread * thread )
{
	bool atLeastOneFileUpdated = false;
	int updatedCnt = 0;
	while( m_GenHashList.size() )
	{
		if( thread->isAborted() )
		{
			return;
		}

		VxSha1Hash fileHash;
		m_GenHashMutex.lock();
		std::string thisFile = m_GenHashList[0];
		m_GenHashList.erase( m_GenHashList.begin() );
		m_GenHashMutex.unlock();
		if( fileHash.generateHashFromFile( thisFile.c_str(), thread ) )
		{
			if( thread->isAborted() )
			{
				return;
			}

			lockFileList();
			std::vector<FileInfo*>::iterator iter;
			for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
			{
				if( thisFile == (*iter)->getFileName() )
				{
					(*iter)->m_FileHash = fileHash;
					atLeastOneFileUpdated = true;
					updatedCnt++;
					m_FileInfoDb.addFile( *iter );
					break;
				}
			}

			unlockFileList();

			if( updatedCnt >= 5 )
			{
				// hashing may take a long time so every 5 files update share info so user
				// will see shared files right away
				if( thread->isAborted() )
				{
					return;
				}

				updateFileTypes();
			}
		}
	}

	if( atLeastOneFileUpdated 
		&& ( false == thread->isAborted() ) )
	{
		updateFileTypes();
	}
}

//============================================================================
void FileInfoMgr::clearLibraryFileList( void )
{
	m_u16FileTypes = 0;
	m_s64TotalByteCnt = 0;

	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		delete (*iter);
	}

	m_FileInfoList.clear();
}

//============================================================================
bool FileInfoMgr::isAllowedFileOrDir( std::string strFileName )
{
	if( VxIsExecutableFile( strFileName ) 
		|| VxIsShortcutFile( strFileName ) )
	{
		return false;
	}

	return true;
}

//============================================================================
void FileInfoMgr::updateFilesListFromDb( VxThread * thread )
{
	lockFileList();

	clearLibraryFileList();
	m_FileInfoDb.getAllFiles( m_FileInfoList );
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( thread && thread->isAborted() )
		{
			return;
		}

		FileInfo* shareInfo = (*iter);
		addFileToGenHashQue( shareInfo->getFileName() );
		if( (*iter)->getIsDirty() )
		{
			(*iter)->updateFileInfo( thread );
		}
	}

	unlockFileList();
	updateFileTypes();
}

//============================================================================
void FileInfoMgr::updateFileTypes( void )
{
}

//============================================================================
// returns -1 if unknown else percent downloaded
int FileInfoMgr::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	int result = -1;
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		FileInfo* fileInfo = (*iter);
		if( fileInfo->getFileHashId().isEqualTo( fileHashId ) )
		{
			result = 100;
			break;
		}
	}

	unlockFileList();
	return result;
}

//============================================================================
bool FileInfoMgr::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashIdIn )
{
	VxSha1Hash fileHashId( fileHashIdIn );
	bool isHashValid = fileHashId.isHashValid();
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == (*iter)->getFileName() )
		{
			if( false == addFile )
			{
				m_FileInfoDb.removeFile( fileName );

				delete *iter;
				m_FileInfoList.erase( iter );
				unlockFileList();
				updateFileTypes();
				return true;
			}
			else
			{
				if( isHashValid )
				{
					(*iter)->setFileHashId( fileHashId );
				}

				unlockFileList();
				return true;
			}
		}
	}

	unlockFileList();

	if( addFile )
	{
		// file is not currently in library and should be
		uint64_t fileLen = VxFileUtil::fileExists( fileName );
		uint8_t fileType = VxFileExtensionToFileTypeFlag( fileName );
		if( ( false == isAllowedFileOrDir( fileName ) )
			|| ( 0 == fileLen ) )
		{
			return false;
		}

		FileInfo * sharedInfo = new FileInfo( fileName, fileLen, fileType );
		lockFileList();
		m_FileInfoList.push_back( sharedInfo );
		if( isHashValid )
		{
			sharedInfo->setFileHashId( fileHashId );
			m_FileInfoDb.addFile( sharedInfo );
		}
		else
		{
			addFileToGenHashQue( fileName );
		}

		unlockFileList();
	}

	return true;
}

//============================================================================
void FileInfoMgr::addFileToLibrary(	std::string		fileName,
										uint64_t		fileLen, 
										uint8_t			fileType,
										VxSha1Hash&		fileHashId )
{
	removeFromLibrary( fileName );
	FileInfo * sharedInfo = new FileInfo( fileName, fileLen, fileType );
	sharedInfo->setFileHashId( fileHashId );
	lockFileList();
	m_FileInfoList.push_back( sharedInfo );
	m_FileInfoDb.addFile(  sharedInfo );
	unlockFileList();
}

//============================================================================
bool FileInfoMgr::isFileInLibrary( std::string& fileName )
{
	bool isInLib = false;
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == (*iter)->getFileName() )
		{
			isInLib = true;
			break;
		}
	}

	unlockFileList();
	if( isInLib )
	{
		if( 0 == VxFileUtil::fileExists( fileName.c_str() ) )
		{
			removeFromLibrary( fileName );
			isInLib = false;
		}
	}

	return isInLib;
}

//============================================================================
bool FileInfoMgr::isFileInLibrary( VxSha1Hash& fileHashId )
{
	bool isInLib = false;
	std::string fileName("");
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileHashId == (*iter)->getFileHashId() )
		{
			isInLib = true;
			fileName = (*iter)->getFileName();
			break;
		}
	}

	unlockFileList();
	if( isInLib && fileName.size() )
	{
		if( 0 == VxFileUtil::fileExists( fileName.c_str() ) )
		{
			removeFromLibrary( fileName );
			isInLib = false;
		}
	}

	return isInLib;
}

//============================================================================
bool FileInfoMgr::isFileInLibrary( VxGUID& assetId )
{
	bool isInLib = false;
	std::string fileName( "" );
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( assetId == (*iter)->getAssetId() )
		{
			isInLib = true;
			fileName = (*iter)->getFileName();
			break;
		}
	}

	unlockFileList();
	if( isInLib && fileName.size() )
	{
		if( 0 == VxFileUtil::fileExists( fileName.c_str() ) )
		{
			removeFromLibrary( fileName );
			isInLib = false;
		}
	}

	return isInLib;
}

//============================================================================
void FileInfoMgr::removeFromLibrary( std::string& fileName )
{
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == (*iter)->getFileName() )
		{
			m_FileInfoDb.removeFile( fileName.c_str() );
			m_FileInfoList.erase( iter );
			break;
		}
	}

	unlockFileList();
}

//============================================================================
bool FileInfoMgr::getFileFullName( VxSha1Hash& fileHashId, std::string& retFileFullName )
{
	bool isShared = false;
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileHashId == ( *iter )->getFileHashId() )
		{
			isShared = true;
			retFileFullName = ( *iter )->getLocalFileName();
			break;
		}
	}

	lockFileList();
	return isShared;
}

//============================================================================
bool FileInfoMgr::getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId )
{
	bool foundHash = false;
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileFullName == ( *iter )->getLocalFileName() )
		{
			retFileHashId = ( *iter )->getFileHashId();
			foundHash = retFileHashId.isHashValid();
			break;
		}
	}

	lockFileList();
	return foundHash;
}

//============================================================================
bool FileInfoMgr::onFileDownloadComplete( std::string& fileName, bool addFile, uint8_t* fileHashId )
{
	LogMsg( LOG_VERBOSE, "FileInfoMgr::onFileDownloadComplete %s", fileName.c_str() );
	return true;
}
