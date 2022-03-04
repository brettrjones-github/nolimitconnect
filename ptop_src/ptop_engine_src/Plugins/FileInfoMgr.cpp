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

#include <CoreLib/Sha1GeneratorMgr.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxSha1Hash.h>
#include <CoreLib/VxFileShredder.h>

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
void FileInfoMgr::onAfterUserLogOnThreaded( void )
{
	// user specific directory should be set
	std::string dbName = VxGetUserSpecificDataDirectory() + "settings/";
	dbName += m_FileInfoDb.getFileInfoDbName();
	lockFileList();
	m_FileInfoDb.dbShutdown();
	m_FileInfoDb.dbStartup( 1, dbName );
	unlockFileList();

	updateFilesListFromDb();
}

//============================================================================
void FileInfoMgr::fileLibraryShutdown( void )
{
	lockFileList();
	clearLibraryFileList();
	m_FileInfoDb.dbShutdown();
	unlockFileList();
}

//============================================================================
void FileInfoMgr::addFileToGenHashQue( VxGUID& fileId, std::string fileName )
{
	GetSha1GeneratorMgr().generateSha1( fileId, fileName, this );
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
		addFileToGenHashQue( shareInfo->getAssetId(), shareInfo->getFileName() );
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
bool FileInfoMgr::addFileToLibrary( std::string& fileName, VxGUID assetId, uint8_t* fileHashIdIn )
{
	if( fileName.empty() )
	{
		LogMsg( LOG_ERROR, "FileInfoMgr::addFileToLibrary invalid param empty fileName" );
		return false;
	}

	VxSha1Hash fileHashId( fileHashIdIn );
	bool isHashValid = fileHashId.isHashValid();
	if( !assetId.isVxGUIDValid() )
	{
		assetId.initializeWithNewVxGUID();
	}

	// file is not currently in library and should be
	uint64_t fileLen = VxFileUtil::fileExists( fileName.c_str() );
	uint8_t fileType = VxFileExtensionToFileTypeFlag( fileName.c_str() );
	if( ( false == isAllowedFileOrDir( fileName ) )
		|| ( 0 == fileLen ) )
	{
		return false;
	}

	FileInfo* fileInfo = new FileInfo( fileName, fileLen, fileType, assetId, fileHashId );
	if( !addFileToLibrary( fileInfo ) )
	{
		delete fileInfo;
		return false;
	}

	return true;
}

//============================================================================
bool FileInfoMgr::addFileToLibrary( FileInfo * fileInfoIn )
{
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileInfoIn->getFileName() == (*iter)->getFileName() )
		{
			m_FileInfoDb.removeFile( fileInfoIn->getFileName() );

			delete *iter;
			m_FileInfoList.erase( iter );
			unlockFileList();
			updateFileTypes();
			break;
		}
	}

	unlockFileList();
	bool result{ false };

	if( fileInfoIn->isValid( true ) )
	{
		lockFileList();
		m_FileInfoList.push_back( fileInfoIn );
		m_FileInfoDb.addFile( fileInfoIn );
		unlockFileList();
		result = true;
	}
	else if( fileInfoIn->isValid( false ) )
	{
		// needs file hash then save
		m_FileInfoNeedHashAndSaveList.push_back( fileInfoIn );
		addFileToGenHashQue( fileInfoIn->getAssetId(), fileInfoIn->getFileName() );
		result = true;
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoMgr::fromGuiAddFileToLibrary invalid %s", fileInfoIn->getFileName().c_str() );
	}

	return result;
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
			m_FileInfoDb.removeFile( fileName );
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

//============================================================================
void FileInfoMgr::callbackSha1GenerateResult( ESha1GenResult sha1GenResult, VxGUID& assetId, Sha1Info& sha1Info )
{
	lockFileList();
	std::vector<FileInfo*>::iterator iter;
	for( iter = m_FileInfoNeedHashAndSaveList.begin(); iter != m_FileInfoNeedHashAndSaveList.end(); ++iter )
	{
		if( assetId == ( *iter )->getAssetId() )
		{
			( *iter )->setFileHashId( sha1Info.getSha1Hash() );
			m_FileInfoList.push_back( *iter );
			m_FileInfoDb.addFile( *iter );
			m_FileInfoNeedHashAndSaveList.erase( iter );
			break;
		}
	}

	unlockFileList();

	updateFileTypes();
}

//============================================================================
bool FileInfoMgr::fromGuiAddFileToLibrary( const char* fileNameIn, bool addFile, uint8_t* fileHashId )
{
	std::string fileName = fileNameIn;
	VxGUID assetId;
	return addFileToLibrary( fileName, assetId, fileHashId );
}