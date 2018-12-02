//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "FileLibraryMgr.h"
#include "LibraryFileInfo.h"
#include "SharedFilesMgr.h"

#include "PluginBase.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxSha1Hash.h>

namespace
{
	const char * FILE_LIBRARY_DB_NAME = "FileLibraryDb.db3";

	//============================================================================
	static uint32_t UpdateFileLibraryThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		FileLibraryMgr * poMgr = (FileLibraryMgr *)poThread->getThreadUserParam();
		poMgr->updateFilesListFromDb( poThread );
		poThread->threadAboutToExit();
		return 0;
	}

	//============================================================================
	static uint32_t GenHashIdsThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		FileLibraryMgr * poMgr = (FileLibraryMgr *)poThread->getThreadUserParam();
		poMgr->generateHashIds( poThread );
		poThread->threadAboutToExit();
		return 0;
	}
}

//============================================================================
FileLibraryMgr::FileLibraryMgr( PluginBase& plugin, IToGui& toGui, SharedFilesMgr& sharedFilesMgr )
: m_Plugin( plugin )
, m_Engine( plugin.getEngine() )
, m_ToGui( toGui )
, m_SharedFilesMgr( sharedFilesMgr )
, m_s64TotalByteCnt(0)
, m_u16FileTypes(0)
{
}

//============================================================================
FileLibraryMgr::~FileLibraryMgr()
{
	fileLibraryShutdown();
}

//============================================================================
void FileLibraryMgr::fromGuiUserLoggedOn( void )
{
	// user specific directory should be set
	std::string dbName = VxGetUserSpecificDataDirectory() + "settings/";
	dbName += FILE_LIBRARY_DB_NAME; 
	lockFileLibrary();
	m_FileLibraryDb.dbShutdown();
	m_FileLibraryDb.dbStartup( 1, dbName );
	unlockFileLibrary();
	if( false == isThreadCreated() )
	{
		startThread( (VX_THREAD_FUNCTION_T)UpdateFileLibraryThreadFunc, this, "UpdateFileLibrary" );
	}
	else
	{
		LogMsg( LOG_ERROR, "FileLibraryMgr::updateFilesList: Thread Still Running\n" );
	}
}

//============================================================================
void FileLibraryMgr::fileLibraryShutdown( void )
{
	m_GenHashThread.abortThreadRun( true );
	killThread();
	lockFileLibrary();
	clearLibraryFileList();
	m_FileLibraryDb.dbShutdown();
	unlockFileLibrary();
}

//============================================================================
void FileLibraryMgr::addFileToGenHashQue( std::string fileName )
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
void FileLibraryMgr::generateHashIds( VxThread * thread )
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

			lockFileLibrary();
			std::vector<LibraryFileInfo*>::iterator iter;
			for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
			{
				if( thisFile == (*iter)->getFileName() )
				{
					(*iter)->m_FileHash = fileHash;
					atLeastOneFileUpdated = true;
					updatedCnt++;
					m_FileLibraryDb.addFile( *iter );
					break;
				}
			}

			unlockFileLibrary();

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
void FileLibraryMgr::clearLibraryFileList( void )
{
	m_u16FileTypes = 0;
	m_s64TotalByteCnt = 0;

	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		delete (*iter);
	}

	m_LibraryFileList.clear();
}

//============================================================================
bool FileLibraryMgr::isAllowedFileOrDir( std::string strFileName )
{
	if( VxIsExecutableFile( strFileName ) 
		|| VxIsShortcutFile( strFileName ) )
	{
		return false;
	}

	return true;
}

//============================================================================
void FileLibraryMgr::updateFilesListFromDb( VxThread * thread )
{
	lockFileLibrary();

	clearLibraryFileList();
	m_FileLibraryDb.getAllFiles( m_LibraryFileList );
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		if( thread && thread->isAborted() )
		{
			return;
		}

		LibraryFileInfo* shareInfo = (*iter);
		addFileToGenHashQue( shareInfo->getFileName() );
		if( (*iter)->getIsDirty() )
		{
			(*iter)->updateFileInfo( thread );
		}
	}

	unlockFileLibrary();
	updateFileTypes();
}

//============================================================================
void FileLibraryMgr::updateFileTypes( void )
{
}

//============================================================================
// returns -1 if unknown else percent downloaded
int FileLibraryMgr::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	int result = -1;
	lockFileLibrary();
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		LibraryFileInfo* fileInfo = (*iter);
		if( fileInfo->getFileHashId().isEqualTo( fileHashId ) )
		{
			result = 100;
			break;
		}
	}

	unlockFileLibrary();
	return result;
}

//============================================================================
bool FileLibraryMgr::fromGuiAddFileToLibrary( const char * fileName, bool addFile, uint8_t * fileHashIdIn )
{
	VxSha1Hash fileHashId( fileHashIdIn );
	bool isHashValid = fileHashId.isHashValid();
	lockFileLibrary();
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		if( fileName == (*iter)->getFileName() )
		{
			if( false == addFile )
			{
				m_FileLibraryDb.removeFile( fileName );

				delete *iter;
				m_LibraryFileList.erase( iter );
				unlockFileLibrary();
				updateFileTypes();
				return true;
			}
			else
			{
				if( isHashValid )
				{
					(*iter)->setFileHashId( fileHashId );
				}

				unlockFileLibrary();
				return true;
			}
		}
	}

	unlockFileLibrary();

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

		LibraryFileInfo * sharedInfo = new LibraryFileInfo( fileName, fileLen, fileType );
		lockFileLibrary();
		m_LibraryFileList.push_back( sharedInfo );
		if( isHashValid )
		{
			sharedInfo->setFileHashId( fileHashId );
			m_FileLibraryDb.addFile( sharedInfo );
		}
		else
		{
			addFileToGenHashQue( fileName );
		}

		unlockFileLibrary();
	}

	return true;
}

//============================================================================
void FileLibraryMgr::addFileToLibrary(	std::string		fileName,
										uint64_t				fileLen, 
										uint8_t				fileType,
										VxSha1Hash&		fileHashId )
{
	removeFromLibrary( fileName );
	LibraryFileInfo * sharedInfo = new LibraryFileInfo( fileName, fileLen, fileType );
	sharedInfo->setFileHashId( fileHashId );
	lockFileLibrary();
	m_LibraryFileList.push_back( sharedInfo );
	m_FileLibraryDb.addFile(  sharedInfo );
	unlockFileLibrary();
}

//============================================================================
bool FileLibraryMgr::isFileInLibrary( std::string& fileName )
{
	bool isInLib = false;
	lockFileLibrary();
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		if( fileName == (*iter)->getFileName() )
		{
			isInLib = true;
			break;
		}
	}

	unlockFileLibrary();
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
bool FileLibraryMgr::isFileInLibrary( VxSha1Hash& fileHashId )
{
	bool isInLib = false;
	std::string fileName("");
	lockFileLibrary();
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		if( fileHashId == (*iter)->getFileHashId() )
		{
			isInLib = true;
			fileName = (*iter)->getFileName();
			break;
		}
	}

	unlockFileLibrary();
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
void FileLibraryMgr::removeFromLibrary( std::string& fileName )
{
	lockFileLibrary();
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		if( fileName == (*iter)->getFileName() )
		{
			m_FileLibraryDb.removeFile( fileName.c_str() );
			m_LibraryFileList.erase( iter );
			break;
		}
	}

	unlockFileLibrary();
}

//============================================================================
void FileLibraryMgr::fromGuiGetFileLibraryList( uint8_t fileTypeFilter )
{
	lockFileLibrary();
	std::vector<LibraryFileInfo*>::iterator iter;
	for( iter = m_LibraryFileList.begin(); iter != m_LibraryFileList.end(); ++iter )
	{
		LibraryFileInfo * fileInfo = (*iter);
		if( 0 != ( fileTypeFilter & fileInfo->getFileType() ) )
		{
			m_ToGui.toGuiFileList(	fileInfo->getFileName().c_str(), 
									fileInfo->getFileLength(), 
									fileInfo->getFileType(), 
									m_SharedFilesMgr.isFileShared( fileInfo->getLocalFileName() ),
									true,
									fileInfo->getFileHashId().getHashData() );
		}
	}

	unlockFileLibrary();
	m_ToGui.toGuiFileList( "", 0, 0, false, false );
}

/*
//============================================================================
RCODE FileLibraryMgr::SendMatchList(	uint32_t			u32SktNum,		// Tronacom socket number
									CString &	csMatchString,	// Search Match name
									uint16_t			u16FileTypes,	// types of files to match
									uint16_t			u16LimitType,	// file size limit type
									uint64_t			u64SizeLimit,	// file size limit
									bool		bCloseAfterSend,// if true close the socket after send
									bool		bEncrypt )		// if true encrypt
{	
	int i;
	RCODE rc = 0;

	PktShareFileList gPktList;
	char as8FileName[ VX_MAX_PATH ];
	char as8TmpFileName[ VX_MAX_PATH ];
	long s32PktIdx = 0;
	unsigned long u32FileLen = 0;
	bool bFileOpen = false;
	FILE * pgFile = 0;
	CStringArray acsMatchTokens;
	bool bSendAll = false;
	ShareFileInfo * poInfo;

	if( 1 == csMatchString.GetLength() && 
		'*' == csMatchString[ 0 ] )
	{
		bSendAll = true; //send complete list
	}
	else
	{

		rc = ParseSearchString(	csMatchString,		//String of search tokens
							acsMatchTokens );	//return seperate token in each String
	}
	if( rc )
	{
		LogMsg( LOG_INFO, "FileLibraryMgr::SendMatchList Error 0x%x occured parsing string\n", rc );
		ASSERT( false );
	}
	else
	{
		int	iCnt = m_List.GetSize();
		for( i = 0; i < iCnt; i++ )
		{
			poInfo = &m_List[ i ];
			if( 0 == ( u16FileTypes & poInfo->m_u32Flags ) )
			{
				//is not the file type we are searching for
				continue;
			}
			if( bSendAll ||
				Match( acsMatchTokens, poInfo->m_csDisplayName ) )
			{
				if( 0 == poInfo->m_s64FileLen )
				{
					LogMsg( LOG_INFO, "FileLibraryMgr::Zero Length File %s\n", (const char *) poInfo->m_csDisplayName );
					continue;
				}
				//add file to PktFileList
				unsigned char * pu8Temp = poInfo->m_pu8ChoppedDisplayName;
				unsigned long u32ItemLen = * pu8Temp + sizeof( __int64 );
				if( gPktList.m_u16PktLen + u32ItemLen > sizeof( PktShareFileList ) )
				{
					//wont fit in this Pkt so store and start another
					gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
					if( false == bFileOpen )
					{	
						tmpnam( as8TmpFileName );
						strcpy( as8FileName, g_oGlobals.m_csExeDir );
						strcat( as8FileName, "TcFileShareRes" );
						strcat( as8FileName, as8TmpFileName );
						strcat( as8FileName, "tmp" );
						pgFile = fopen( as8FileName, "wb+" );
						if( 0 == pgFile )
						{
							rc = FILE_OPEN_ERR;
							break;
						}
						bFileOpen = true;
					}

					if( (unsigned long)gPktList.m_u16PktLen != fwrite( &gPktList, 
																		1, 
																		(unsigned long)gPktList.m_u16PktLen, 
																		pgFile ) )
					{
						rc = FILE_WRITE_ERR;
					}
					u32FileLen += gPktList.m_u16PktLen;
					gPktList.m_u16PktLen = gPktList.emptyLen();
					s32PktIdx = 0;
					gPktList.m_u16ListCnt = 0;
				}
				//copy file length first
				*( ( __int64 *)&gPktList.m_au8List[ s32PktIdx ] ) = poInfo->m_s64FileLen;
				//copy chopped string that has display name
				memcpy( &gPktList.m_au8List[ s32PktIdx + sizeof( __int64 ) ], poInfo->m_pu8ChoppedDisplayName, u32ItemLen - sizeof( __int64 ) );

				//show trace of file we added to list
				char as8Buf[ 256 * 2 ];
				VxUnchopStr( poInfo->m_pu8ChoppedDisplayName, as8Buf );
				LogMsg( LOG_INFO, "Added to PktFileList file size %d file %s\n", 
						*( ( unsigned long *)&gPktList.m_au8List[ s32PktIdx ] ),
						as8Buf );


				//increment vars for next item
				s32PktIdx += u32ItemLen;
				gPktList.m_u16ListCnt++;
				gPktList.m_u16PktLen += (unsigned short)u32ItemLen;
			}	
		}
	}
	if( pgFile )
	{
		if( u32FileLen )
		{
			//we allready have at least one pkt in file
			//so add this one if has data
			if( s32PktIdx )
			{
				//we have a pkt with data
				gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
			
				//flush Pkt to file
				if( (unsigned long)gPktList.m_u16PktLen != fwrite( &gPktList, 
																	1, 
																	(unsigned long)gPktList.m_u16PktLen, 
																	pgFile ) )
				{
					rc = FILE_WRITE_ERR;
				}
				u32FileLen += gPktList.m_u16PktLen;
				if( u32FileLen & 0x0f )
				{
					//file length is not on 16 byte boundery so fill in a little
					if( 16 - (u32FileLen & 0x0f) != fwrite( &gPktList, 
																		16 - (u32FileLen & 0x0f),
																		(unsigned long)gPktList.m_u16PktLen, 
																		pgFile ) )
					{
						rc = FILE_WRITE_ERR;
					}
				}
				fclose( pgFile );
			}
			//use threaded file send to send list
			//this uses a separate thread to send file containing packets
			rc = g_poApi->SktSendFile(	u32SktNum,// Socket to send on
							as8FileName,	// packet to be sent via tcp connection
							0,	//offset into file to start at
							4096, //size of pkt chunks
							-2,	// PktType to put in header 	
								// positive value..add header of u16PktType to each chunk sent
								//  -1 = use no headers..send file in chunks of u32PktSize in length
								//	-2 = file is file of packets add value in size of pkt chunks to pkt types
							0, //instance value if has header
							true,		// if true delete file after it is sent
							true,		// if true disconnect after send
							0, //user data for callback
							NULL );		// callback from thread with File Send status
		}
	}
	else
	{
		//we havent even written one pkt to file so
		//just send this one
		gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
 
		g_poApi->SktSend( u32SktNum, &gPktList, bCloseAfterSend );
	}
	return rc;
}

//============================================================================
RCODE FileLibraryMgr::AddDir( CString & csDirPath )
{
	HRESULT hr;
	char as8FullPath[ VX_MAX_PATH ];

	struct _stat gStat;
	int i;


    CArray<CString, CString&> acsWildNameList;//Extensions ( file extentions )
	acsWildNameList.Add( CString( "*.*" ) );
	CArray<LibraryFileInfo, LibraryFileInfo&> agFileLibraryList;//return FileInfo in array

	VxFindFilesByName(	csDirPath,				//start path to search in
						acsWildNameList,//Extensions ( file extentions )
						agFileLibraryList,//return FileInfo in array
						true,						//recurse subdirectories if TRUE
						false		//if TRUE dont return files matching filter else return files that do
 						);
	int iCnt = agFileLibraryList.GetSize();
	//expand list to include the new files
	int iCurListCnt = m_List.GetSize();
	m_List.SetSize( iCnt + iCurListCnt );
	int iResolvedListIdx = iCurListCnt;

	for( i = 0; i < iCnt; i++ )
	{
		CString cs = agFileLibraryList[ i ];
		__int64 s64FileLen = agFileLibraryList[ i ].m_s64Len;
		makeForwardSlashPath( cs );

		//get file extension
		CString csExt = getFileExtension( cs );
		char * pExt = (char *)((const char *)csExt);
		bool bIsLink = false;

		if( 0 == strcmp( pExt, "lnk" ) )
		{ 
			bIsLink = true; 
			as8FullPath[ 0 ] = 0;
			hr = VxResolveShortcut(	0,				//handle to window of caller
									cs,	//.lnk file
									as8FullPath );		//return path to target file
			if( FAILED( hr ) )
			{
				TRACE("Unable to resolve path in %s\n", (const char *)cs );
				continue;
			}
			else
			{
				cs = as8FullPath;	
				int iResult = ::_stat( (const char *)cs, &gStat );
				if( iResult != 0 )
				{
					TRACE( "Problem getting file information\n" );
					continue;
				}
				else
				{
					s64FileLen = gStat.st_size;
					csExt = getFileExtension( cs );
					pExt = (char *)((const char *)csExt);
				}
			}
		}
		//weve got a live one so

		unsigned long u32FileFlags = 0;

		if( VxIsPhotoFileExtention( pExt ) )
		{
			m_bHasImage = true;
			u32FileFlags |= FILE_TYPE_PHOTO;
			m_u32FileFlags |= FILE_TYPE_PHOTO;
		}
		else if( VxIsAudioFileExtention( pExt ) )
		{
			m_bHasAudio = true;
			u32FileFlags |= FILE_TYPE_AUDIO;
			m_u32FileFlags |= FILE_TYPE_AUDIO; 
		}
		else if( VxIsVideoFileExtention( pExt ) )
		{
			m_bHasVideo = true;
			u32FileFlags |= FILE_TYPE_VIDEO;
			m_u32FileFlags |= FILE_TYPE_VIDEO; 
		}
		else if( VxIsExecutableFileExtention( pExt ) )
		{
			m_bHasExecutable = true;
			u32FileFlags |= FILE_TYPE_EXECUTABLE;
			m_u32FileFlags |= FILE_TYPE_EXECUTABLE;
		}
		else if( VxIsArchiveOrDocFileExtention( pExt ) )
		{
			m_bHasArchiveOrDoc = true;
			u32FileFlags |= FILE_TYPE_ARCHIVE_OR_DOC;
			m_u32FileFlags |= FILE_TYPE_ARCHIVE_OR_DOC;
		}
		else if( VxIsArchiveOrDocFileExtention( pExt ) )
		{
			m_bHasCdImage = true;
			u32FileFlags |= FILE_TYPE_CD_IMAGE;
			m_u32FileFlags |= FILE_TYPE_CD_IMAGE;
		}
		else
		{
			m_bHasOther = true;
			u32FileFlags |= FILE_TYPE_OTHER;
			m_u32FileFlags |= FILE_TYPE_OTHER;
		}
		//fill in the data
		m_s64TotalByteCnt += s64FileLen;
		ShareFileInfo * poInfo = &m_List[ iResolvedListIdx ];
		poInfo->m_csPathAndName = cs;
		poInfo->m_s64FileLen = s64FileLen;
		poInfo->m_u32Flags = u32FileFlags;
		if( bIsLink )
		{
			//display name is the file name
			poInfo->m_csDisplayName = VxGetFileNameFromFullPath( cs );
		}
		else
		{
			//display name is full path - root path
			char * pTemp = (char *)(const char *)cs;
			int iRootLen =  csDirPath.GetLength();
			ASSERT( iRootLen < cs.GetLength() );
			if( iRootLen )
			{
				if( '\\' == pTemp[ iRootLen - 1 ] )
				{
					//root path has the slash
					poInfo->m_csDisplayName = &pTemp[ iRootLen ];
				}
				else
				{
					//root path doesnt has the slash so add 1 to length
					poInfo->m_csDisplayName = &pTemp[ iRootLen + 1 ];
				}

			}
		}

		unsigned char * pu8ChoppedStr = new unsigned char[ poInfo->m_csDisplayName.GetLength() + 10 ];
		VxChopStr( (char *)(const char *) poInfo->m_csDisplayName, pu8ChoppedStr );
		if( poInfo->m_pu8ChoppedDisplayName )
		{
			delete poInfo->m_pu8ChoppedDisplayName;
		}
		poInfo->m_pu8ChoppedDisplayName = pu8ChoppedStr;
		LogChoppedString( pu8ChoppedStr );
		poInfo = &m_List[0];
		const char * pTemp = (const char *)poInfo->m_csDisplayName;
		
		iResolvedListIdx++;

	}
	m_List.SetSize( iResolvedListIdx );//remove any unused entries
	return 0;
}
//============================================================================
//! make array of packets with matching files
RCODE FileLibraryMgr::MakeMatchList(	CPtrArray * paoRetListPkts, // return list of packets of file lists
									CString &	csMatchString,	// Search Match name
									uint16_t			u16FileTypes,	// types of files to match
									uint16_t			u16LimitType,	// file size limit type
									uint64_t			u64SizeLimit )	// file size limit
{	
	int i;
	RCODE rc = 0;

	PktShareFileList gPktList;
	long s32PktIdx = 0;
	unsigned long u32FileLen = 0;
	bool bFileOpen = false;
	FILE * pgFile = 0;
	CStringArray acsMatchTokens;
	bool bSendAll = false;
	ShareFileInfo * poInfo;

	if( 1 == csMatchString.GetLength() && 
		'*' == csMatchString[ 0 ] )
	{
		bSendAll = true; //send complete list
	}
	else
	{

		rc = ParseSearchString(	csMatchString,		//String of search tokens
							acsMatchTokens );	//return seperate token in each String
	}
	if( rc )
	{
		LogMsg( LOG_INFO, "FileLibraryMgr::SendMatchList Error 0x%x occured parsing string\n", rc );
		ASSERT( false );
	}
	else
	{
		int	iCnt = m_List.GetSize();
		for( i = 0; i < iCnt; i++ )
		{
			poInfo = &m_List[ i ];
			if( 0 == ( u16FileTypes & poInfo->m_u32Flags ) )
			{
				//is not the file type we are searching for
				continue;
			}
			if( bSendAll ||
				Match( acsMatchTokens, poInfo->m_csDisplayName ) )
			{
				if( 0 == poInfo->m_s64FileLen )
				{
					LogMsg( LOG_INFO, "FileLibraryMgr::Zero Length File %s\n", (const char *) poInfo->m_csDisplayName );
					continue;
				}
				//add file to PktFileList
				unsigned char * pu8Temp = poInfo->m_pu8ChoppedDisplayName;
				unsigned long u32ItemLen = * pu8Temp + sizeof( __int64 );
				if( gPktList.m_u16PktLen + u32ItemLen > sizeof( PktShareFileList ) )
				{
					//wont fit in this Pkt so store and start another
					gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
					u32FileLen += gPktList.m_u16PktLen;
					
					paoRetListPkts->Add( gPktList.makeCopy() );

					gPktList.m_u16PktLen = gPktList.emptyLen();
					s32PktIdx = 0;
					gPktList.m_u16ListCnt = 0;
				}
				//copy file length first
				*( ( __int64 *)&gPktList.m_au8List[ s32PktIdx ] ) = poInfo->m_s64FileLen;
				//copy chopped string that has display name
				memcpy( &gPktList.m_au8List[ s32PktIdx + sizeof( __int64 ) ], poInfo->m_pu8ChoppedDisplayName, u32ItemLen - sizeof( __int64 ) );

				//show trace of file we added to list
				char as8Buf[ 256 * 2 ];
				VxUnchopStr( poInfo->m_pu8ChoppedDisplayName, as8Buf );
				LogMsg( LOG_INFO, "Added to PktFileList file size %d file %s\n", 
						*( ( unsigned long *)&gPktList.m_au8List[ s32PktIdx ] ),
						as8Buf );


				//increment vars for next item
				s32PktIdx += u32ItemLen;
				gPktList.m_u16ListCnt++;
				gPktList.m_u16PktLen += (unsigned short)u32ItemLen;
			}	
		}
	}
	gPktList.m_u16PktLen = (( gPktList.m_u16PktLen + 15 ) & ~15 );
	paoRetListPkts->Add( gPktList.makeCopy() );
 	return 0;
}
*/
