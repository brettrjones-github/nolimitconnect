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
#include <PktLib/PktsFileInfo.h>

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
	std::vector<std::string> toDeleteFiles;
	std::map<VxGUID, FileInfo>	dbFileList;

	lockFileList();
	m_FileInfoDb.dbShutdown();
	m_FileInfoDb.dbStartup( 1, dbName );

	m_FileInfoDb.getAllFiles( dbFileList );

	for( auto iter = dbFileList.begin(); iter != dbFileList.end(); ++iter )
	{
		FileInfo& fileInfo = iter->second;
		uint64_t curFileLen = VxFileUtil::fileExists( fileInfo.getFileName().c_str() );
		if( curFileLen && fileInfo.isValid( false ) )
		{
			if( curFileLen == fileInfo.getFileLength() && fileInfo.isValid( true ) )
			{
				// because file length is same and sha1 is valid assume is ready for use
				m_FileInfoList[fileInfo.getAssetId()] = fileInfo;
			}
			else
			{
				fileInfo.setFileLength( curFileLen );
				m_FileInfoNeedHashAndSaveList.push_back( fileInfo );
				addFileToGenHashQue( fileInfo.getAssetId(), fileInfo.getFileName() );
			}
		}
		else
		{
			// file no longer exists
			toDeleteFiles.push_back( fileInfo.getFileName() );
		}

		if( VxIsAppShuttingDown() )
		{
			unlockFileList();
			return;
		}
	}

	unlockFileList();
	updateFileTypes();

	// delete from db files that no longer exist
	for( auto assetId : toDeleteFiles )
	{
		lockFileList();
		m_FileInfoDb.removeFile( assetId );
		unlockFileList();
	}

	checkForInitializeCompleted();
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

	m_FileInfoList.clear();


	m_FileInfoNeedHashAndSaveList.clear();
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
// returns -1 if unknown else percent downloaded
int FileInfoMgr::fromGuiGetFileDownloadState( uint8_t * fileHashId )
{
	int result = -1;
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		FileInfo& fileInfo = iter->second;
		if( fileInfo.getFileHashId().isEqualTo( fileHashId ) )
		{
			result = 100;
			break;
		}
	}

	unlockFileList();
	return result;
}

//============================================================================
bool FileInfoMgr::addFileToLibrary( VxGUID& onlineId, std::string& fileName, VxGUID& assetId )
{
	if( fileName.empty() )
	{
		LogMsg( LOG_ERROR, "FileInfoMgr::addFileToLibrary invalid param empty fileName" );
		return false;
	}


	// file is not currently in library and should be
	uint64_t fileLen = VxFileUtil::fileExists( fileName.c_str() );
	uint8_t fileType = VxFileExtensionToFileTypeFlag( fileName.c_str() );
	if( ( false == isAllowedFileOrDir( fileName ) )
		|| ( 0 == fileLen ) )
	{
		return false;
	}

	FileInfo fileInfo( onlineId, fileName, fileLen, fileType, assetId );
	if( !addFileToLibrary( fileInfo ) )
	{
		return false;
	}

	return true;
}

//============================================================================
bool FileInfoMgr::addFileToLibrary( FileInfo& fileInfoIn )
{
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileInfoIn.getFileName() == iter->second.getFileName() )
		{
			m_FileInfoDb.removeFile( fileInfoIn.getFileName() );

			m_FileInfoList.erase( iter );
			unlockFileList();
			updateFileTypes();
			break;
		}
	}

	unlockFileList();
	bool result{ false };

	if( fileInfoIn.isValid( true ) )
	{
		lockFileList();
		m_FileInfoList[fileInfoIn.getAssetId()] = fileInfoIn;
		m_FileInfoDb.addFile( fileInfoIn );
		unlockFileList();
		result = true;
	}
	else if( fileInfoIn.isValid( false ) )
	{
		// needs file hash then save
		m_FileInfoNeedHashAndSaveList.push_back( fileInfoIn );
		addFileToGenHashQue( fileInfoIn.getAssetId(), fileInfoIn.getFileName() );
		result = true;
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoMgr::fromGuiAddFileToLibrary invalid %s", fileInfoIn.getFileName().c_str() );
	}

	return result;
}

//============================================================================
bool FileInfoMgr::addFileToLibrary( VxGUID&			onlineId,
									VxGUID&			assetId, 
									std::string		fileName,
									uint64_t		fileLen, 
									uint8_t			fileType,
									VxSha1Hash&		fileHashId )
{
	FileInfo fileInfo( onlineId, fileName, fileLen, fileType, assetId, fileHashId );
	return addFileToLibrary( fileInfo );
}

//============================================================================
bool FileInfoMgr::cancelAndDelete( VxGUID& assetId )
{
	bool result{ false };

	lockFileList();
	auto iter = m_FileInfoList.find( assetId );
	if( iter != m_FileInfoList.end() )
	{
		m_FileInfoXferMgr.fileAboutToBeDeleted( iter->second.getFileName() );
		m_FileInfoDb.removeFile( iter->second.getFileName() );
		VxFileUtil::deleteFile( iter->second.getFileName().c_str() );
		m_FileInfoList.erase( iter );
	}

	unlockFileList();
	updateFileTypes();
	return result;
}

//============================================================================
bool FileInfoMgr::isFileInLibrary( std::string& fileName )
{
	bool isInLib = false;
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == iter->second.getFileName() )
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
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileHashId == iter->second.getFileHashId() )
		{
			isInLib = true;
			fileName = iter->second.getFileName();
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
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( assetId == iter->second.getAssetId() )
		{
			isInLib = true;
			fileName = iter->second.getFileName();
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
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == iter->second.getFileName() )
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
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileHashId == iter->second.getFileHashId() )
		{
			isShared = true;
			retFileFullName = iter->second.getFullFileName();
			break;
		}
	}

	unlockFileList();
	return isShared;
}

//============================================================================
bool FileInfoMgr::getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId )
{
	bool foundHash = false;
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileFullName == iter->second.getFullFileName() )
		{
			retFileHashId = iter->second.getFileHashId();
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
	for( auto iter = m_FileInfoNeedHashAndSaveList.begin(); iter != m_FileInfoNeedHashAndSaveList.end(); )
	{
		bool wasErased{ false };
		if( assetId == ( *iter ).getAssetId() )
		{
			if( eSha1GenResultNoError == sha1GenResult )
			{
				iter->setFileHashId( sha1Info.getSha1Hash() );
				m_FileInfoDb.addFile( *iter );
				m_FileInfoList[( *iter ).getAssetId()] = *iter;

				iter = m_FileInfoNeedHashAndSaveList.erase( iter );
				wasErased = true;
			}
			else
			{
				LogMsg( LOG_VERBOSE, "FileInfoMgr::callbackSha1GenerateResult failed %s", DescribeSha1GenResult( sha1GenResult ) );
			}

			break;
		}

		if( !wasErased )
		{
			iter++;
		}
	}

	unlockFileList();
	updateFileTypes();
	checkForInitializeCompleted();
}

//============================================================================
bool FileInfoMgr::fromGuiAddFileToLibrary( const char* fileNameIn, bool addFile, uint8_t* fileHashId )
{
	std::string fileName = fileNameIn;
	VxGUID assetId;
	assetId.initializeWithNewVxGUID();
	return addFileToLibrary( m_Engine.getMyOnlineId(), fileName, assetId );
}

//============================================================================
bool FileInfoMgr::loadAboutMePageStaticAssets( void )
{
	/*
	static std::vector<VxGUID>	g_AboutMeIdList{
		{ 7440246806584140214U, 18100947260721918349U },	// !6741110CEBAEA1B6FB337BA17568F18D! no limit icon
		{ 5928192941618969970U, 9804703070566852284U },		// !52452C124D2D3D7288114D7EC151A6BC! about me thumb
		{ 17745883519647430189U, 3508611113527476619U },	// !F6460B0A160B362D30B11737E4CB018B! about me index
		{ 1319370582698595072U, 4306035094495659427U },		// !124F588DFCD993003BC21C1AA5C785A3! about me me.png
	};
	*/

	static std::vector<std::string>	g_AboutMeNameList{
		{ "favicon.ico" },			
		{ "aboutme_thumb.png" },	
		{ "index.htm" },			
		{ "me.png" },					
	};

	if( m_RootFileFolder.empty() )
	{
		LogMsg( LOG_ERROR, "getAboutMePageStaticAssets No Root File Folder" );
		return false;
	}

	int fileCount{ 0 };
	bool result{ true };
	for( auto &fileShortName : g_AboutMeNameList )
	{
		std::string fullFileName = m_RootFileFolder + fileShortName;
		int64_t fileLen = VxFileUtil::fileExists( fullFileName.c_str() );
		VxGUID assetId;
		if( fileLen )
		{
			assetId.initializeWithNewVxGUID();
			// we can generate the sha1Hash now instead of in thread because there is only a few small files
			VxSha1Hash sha1Hash;
			if( sha1Hash.generateHashFromFile( fullFileName.c_str() ) )
			{
				FileInfo fileInfo( m_Engine.getMyOnlineId(), fullFileName, fileLen, VxFileExtensionToFileTypeFlag( fullFileName.c_str() ), assetId, sha1Hash );
				if( fileInfo.isValid( true ) )
				{
					lockFileList();
					m_FileInfoList[assetId] = fileInfo;
					unlockFileList();
					fileCount++;
				}
				else
				{
					LogMsg( LOG_ERROR, "getAboutMePageStaticAssets Invalid File %s", fullFileName.c_str() );
					result = false;
					break;
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "getAboutMePageStaticAssets Generate Sha1Hash Failed File %s", fullFileName.c_str() );
				result = false;
				break;
			}			
		}
		else
		{
			LogMsg( LOG_ERROR, "getAboutMePageStaticAssets 0 file Len File %s", fullFileName.c_str() );
			result = false;
			break;
		}
	}

	return result && fileCount == g_AboutMeNameList.size();
}

//============================================================================
void FileInfoMgr::getStoryboardStaticAssets( std::vector<std::pair<VxGUID, std::string>>& assetList )
{
	static std::vector<VxGUID>	g_StoryboardIdList{
		{ 883460556043267518U, 1348434257746011027U },		// !0C42AEB1E8072DBE12B699D027EB2393! no limit icon
		{ 17888782341603930641U, 12762868320513151386U },	// !F841B8C2C7953211B11ED11DEF45D19A! story_board thumb
		{ 17745883519647430189U, 3508611113527476619U },	// !F6460B0A160B362D30B11737E4CB018B! story_board index
		{ 1319370582698595072U, 4306035094495659427U },		// !124F588DFCD993003BC21C1AA5C785A3! story_board background
		{ 5928192941618969970U, 9804703070566852284U },		// !52452C124D2D3D7288114D7EC151A6BC! story_board me.png
	};

	static std::vector<std::string>	g_StoryboardNameList{
		{ "favicon.ico" },				// !0C42AEB1E8072DBE12B699D027EB2393! no limit icon
		{ "storyboard_thumb.png" },		// !F841B8C2C7953211B11ED11DEF45D19A! story_board thumb
		{ "story_board.htm" },			// !F6460B0A160B362D30B11737E4CB018B! story_board index
		{ "storyboard_background.png" }, // !124F588DFCD993003BC21C1AA5C785A3! story_board background
		{ "me.png" },					// !52452C124D2D3D7288114D7EC151A6BC! story_board me.png
	};

	for( int i = 0; i < g_StoryboardIdList.size(); i++ )
	{
		assetList.push_back( std::make_pair( g_StoryboardIdList[i], g_StoryboardNameList[i] ) );
	}
}

//============================================================================
void FileInfoMgr::updateFileTypes( void )
{
	int64_t	newUpdateTime{ 0 };
	int64_t	newTotalBytes{ 0 };
	uint16_t newFileTypes{ 0 };

	lockFileList();
	int64_t	oldUpdateTime = m_LastUpdateTime;
	int64_t	oldTotalBytes =	m_s64TotalByteCnt;
	uint16_t oldFileTypes = m_u16FileTypes;

	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		newTotalBytes += iter->second.getFileLength();
		newFileTypes |= iter->second.getFileType();
		if( iter->second.getFileTime() > newUpdateTime )
		{
			newUpdateTime = iter->second.getFileTime();
		}
	}

	unlockFileList();
	if( oldTotalBytes != newTotalBytes || oldFileTypes != newFileTypes || oldUpdateTime != newUpdateTime )
	{
		lockFileList();
		m_s64TotalByteCnt = newTotalBytes;
		m_u16FileTypes = newFileTypes;
		unlockFileList();

		if( m_FilesInitialized )
		{
			m_Plugin.onFilesChanged( newUpdateTime, newTotalBytes, newFileTypes );
		}
	}
}

//============================================================================
void FileInfoMgr::checkForInitializeCompleted( void )
{
	if( !m_FilesInitialized && m_FileInfoNeedHashAndSaveList.empty() )
	{	
		lockFileList();
		m_FilesInitialized = true;
		int64_t	lastUpdateTime = m_LastUpdateTime;
		int64_t	totalByteCnt = m_s64TotalByteCnt;
		uint16_t FileTypes = m_u16FileTypes;
		unlockFileList();

		m_Plugin.onLoadedFilesReady( lastUpdateTime, totalByteCnt, FileTypes );
	}
}

//============================================================================
bool FileInfoMgr::findFileAsset( VxGUID& fileAssetId, FileInfo& fileInfo)
{
	auto iter = m_FileInfoList.find( fileAssetId );
	if( iter != m_FileInfoList.end() )
	{
		fileInfo = iter->second;
		return true;
	}

	return false;
}

//============================================================================
ECommErr FileInfoMgr::searchRequest( PktFileInfoSearchReply& pktReply, VxGUID& specificAssetId, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	ECommErr searchErr = m_FilesInitialized ? eCommErrNone : eCommErrPluginNotEnabled;
	if( eCommErrNone == searchErr )
	{
		uint64_t timeNow = GetGmtTimeMs();
		pktReply.setCommError( eCommErrNotFound );
		lockFileList();

		if( specificAssetId.isVxGUIDValid() )
		{
			FileInfo fileInfo;
			if( findFileAsset( specificAssetId, fileInfo ) && fileInfo.isValid() )
			{
				if( fileInfo.addToBlob( pktReply.getBlobEntry() ) )
				{
					pktReply.setCommError( eCommErrNone );
				}
			}
		}
		else if( searchStr.length() >= FileInfo::FILE_INFO_SHORTEST_SEARCH_TEXT_LEN && searchStr.length() <= FileInfo::FILE_INFO_LONGEST_SEARCH_TEXT_LEN )
		{
			pktReply.setCommError( eCommErrSearchNoMatch );
			bool foundMatch = false;
			for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter  )
			{
				FileInfo& fileInfo = iter->second;
				if( fileInfo.isValid() && fileInfo.matchText( searchStr ) )
				{
					if( pktReply.getBlobEntry().haveRoom( fileInfo.calcBlobLen() ) )
					{
						if( fileInfo.addToBlob( pktReply.getBlobEntry() ) )
						{
							pktReply.incrementFileInfoCount();
							foundMatch = true;
						}
					}
					else
					{
						// there are more to match
						pktReply.setMoreFileInfosExist( true );
						pktReply.setNextSearchAssetId( fileInfo.getAssetId() );
						break;
					}
				}
			}

			if( foundMatch )
			{
				pktReply.setCommError( eCommErrNone );
			}
		}
		else
		{
			if( !searchStr.empty() )
			{
				LogMsg( LOG_VERBOSE, "FileInfoMgr::searchRequest Warning search text was to short.. sending all files" );
			}

			// all files list
			bool foundMatch = false;
			for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
			{
				FileInfo& fileInfo = iter->second;
				if( fileInfo.isValid() )
				{
					if( pktReply.getBlobEntry().haveRoom( fileInfo.calcBlobLen() ) )
					{
						if( fileInfo.addToBlob( pktReply.getBlobEntry() ) )
						{
							pktReply.incrementFileInfoCount();
							foundMatch = true;
						}
					}
					else
					{
						// there are more to match
						pktReply.setMoreFileInfosExist( true );
						pktReply.setNextSearchAssetId( fileInfo.getAssetId() );
						break;
					}
				}
			}

			if( foundMatch )
			{
				pktReply.setCommError( eCommErrNone );
			}
		}

		unlockFileList();
	}

	pktReply.calcPktLen();
	return pktReply.getCommError();
}

//============================================================================
ECommErr FileInfoMgr::searchMoreRequest( PktFileInfoMoreReply& pktReply, VxGUID& nextFileAssetId, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	ECommErr searchErr = m_FilesInitialized ? eCommErrNone : eCommErrPluginNotEnabled;
	if( eCommErrNone == searchErr && !nextFileAssetId.isVxGUIDValid() )
	{
		searchErr = eCommErrInvalidParam;
	}

	if( eCommErrNone == searchErr )
	{
		uint64_t timeNow = GetGmtTimeMs();
		pktReply.setCommError( eCommErrNotFound );
		bool foundMatch = false;		
		lockFileList();
		for( auto iter = m_FileInfoList.find( nextFileAssetId ); iter != m_FileInfoList.end(); ++iter )
		{
			FileInfo& fileInfo = iter->second;
			if( fileInfo.isValid() )
			{
				if( searchStr.length() >= FileInfo::FILE_INFO_SHORTEST_SEARCH_TEXT_LEN && searchStr.length() <= FileInfo::FILE_INFO_LONGEST_SEARCH_TEXT_LEN )
				{
					if( fileInfo.matchText( searchStr ) )
					{
						if( pktReply.getBlobEntry().haveRoom( fileInfo.calcBlobLen() ) )
						{
							if( fileInfo.addToBlob( pktReply.getBlobEntry() ) )
							{
								pktReply.incrementFileInfoCount();
								foundMatch = true;
							}
						}
						else
						{
							// there are more to match
							pktReply.setMoreFileInfosExist( true );
							pktReply.setNextSearchAssetId( fileInfo.getAssetId() );
							break;
						}
					}
				}
				else
				{
					if( pktReply.getBlobEntry().haveRoom( fileInfo.calcBlobLen() ) )
					{
						if( fileInfo.addToBlob( pktReply.getBlobEntry() ) )
						{
							pktReply.incrementFileInfoCount();
							foundMatch = true;
						}
					}
					else
					{
						// there are more to match
						pktReply.setMoreFileInfosExist( true );
						pktReply.setNextSearchAssetId( fileInfo.getAssetId() );
						break;
					}
				}
			}
		}

		unlockFileList();
		if( foundMatch )
		{
			pktReply.setCommError( eCommErrNone );
		}
	}

	pktReply.calcPktLen();
	return pktReply.getCommError();
}

//============================================================================
bool FileInfoMgr::startDownload( FileInfo& fileInfo, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return m_FileInfoXferMgr.startDownload( fileInfo, searchSessionId, sktBase, netIdent );
}