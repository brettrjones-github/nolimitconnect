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
// http://www.nolimitconnect.org
//============================================================================

#include "FileInfoBaseMgr.h"
#include "FileInfo.h"

#include "PluginBase.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
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
FileInfoBaseMgr::FileInfoBaseMgr( P2PEngine& engine, PluginBase& plugin, FileInfoDb& fileInfoDb )
: m_PrivateEngine( engine )
, m_Plugin( plugin )
, m_FileShredder( GetVxFileShredder() )
, m_FileInfoDb( fileInfoDb )
, m_FileInfoXferMgr( engine, plugin, *this )
{
	LogMsg( LOG_VERBOSE, "FileInfoBaseMgr::FileInfoBaseMgr %s %p", DescribePluginType( plugin.getPluginType() ), this );
}

//============================================================================
FileInfoBaseMgr::~FileInfoBaseMgr()
{
	fileInfoMgrShutdown();
}

//============================================================================
std::string FileInfoBaseMgr::getIncompleteFileXferDirectory( VxGUID& onlineId )
{
	std::string incompleteDir = m_Plugin.getIncompleteFileXferDirectory( onlineId );
	if( incompleteDir.empty() )
	{
		incompleteDir = VxGetIncompleteDirectory();
	}

	return incompleteDir;
}

//============================================================================
void FileInfoBaseMgr::onAfterUserLogOnThreaded( void )
{
	// user specific directory should be set
	std::string dbName = VxGetUserSpecificDataDirectory() + "settings/";
	dbName += m_FileInfoDb.getFileInfoDbName();
	std::vector<std::string> toDeleteFiles;
	std::map<VxGUID, FileInfo>	dbFileList;

	LogMsg( LOG_VERBOSE, "onAfterUserLogOnThreadedr::onAfterUserLogOnThreaded lock" );
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

	LogMsg( LOG_VERBOSE, "onAfterUserLogOnThreadedr::onAfterUserLogOnThreaded lock" );
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
void FileInfoBaseMgr::fileInfoMgrShutdown( void )
{
	lockFileList();
	clearLibraryFileList();
	m_FileInfoDb.dbShutdown();
	unlockFileList();
}

//============================================================================
void FileInfoBaseMgr::addFileToGenHashQue( VxGUID& fileId, std::string fileName )
{
	GetSha1GeneratorMgr().generateSha1( fileId, fileName, this );
}

//============================================================================
void FileInfoBaseMgr::clearLibraryFileList( void )
{
	m_u16FileTypes = 0;
	m_s64TotalByteCnt = 0;

	m_FileInfoList.clear();


	m_FileInfoNeedHashAndSaveList.clear();
}

//============================================================================
bool FileInfoBaseMgr::isAllowedFileOrDir( std::string strFileName )
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
int FileInfoBaseMgr::fromGuiGetFileDownloadState( uint8_t * fileHashId )
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
bool FileInfoBaseMgr::addFileToDbAndList( VxGUID& onlineId, std::string& fileName, VxGUID& assetId )
{
	if( fileName.empty() )
	{
		LogMsg( LOG_ERROR, "FileInfoBaseMgr::addFileToDbAndList invalid param empty fileName" );
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
	if( !addFileToDbAndList( fileInfo ) )
	{
		return false;
	}

	return true;
}

//============================================================================
bool FileInfoBaseMgr::addFileToDbAndList( FileInfo& fileInfoIn )
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
		lockFileList();
		m_FileInfoNeedHashAndSaveList.push_back( fileInfoIn );
		unlockFileList();
		addFileToGenHashQue( fileInfoIn.getAssetId(), fileInfoIn.getFileName() );
		result = true;
	}
	else
	{
		LogMsg( LOG_ERROR, "FileInfoBaseMgr::fromGuiAddFileToLibrary invalid %s", fileInfoIn.getFileName().c_str() );
	}

	return result;
}

//============================================================================
bool FileInfoBaseMgr::addFileToDbAndList( VxGUID&			onlineId,
											VxGUID&			assetId, 
											std::string		fileName,
											uint64_t		fileLen, 
											uint8_t			fileType,
											VxSha1Hash&		fileHashId )
{
	FileInfo fileInfo( onlineId, fileName, fileLen, fileType, assetId, fileHashId );
	return addFileToDbAndList( fileInfo );
}

//============================================================================
bool FileInfoBaseMgr::cancelAndDelete( VxGUID& assetId )
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
bool FileInfoBaseMgr::isFileShared( std::string& fileName, bool listIsLocked )
{
	bool isInLib = false;
	if( !listIsLocked )
	{
		lockFileList();
	}

	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == iter->second.getFileName() )
		{
			isInLib = true;
			break;
		}
	}

	if( !listIsLocked )
	{
		unlockFileList();
	}

	if( isInLib )
	{
		if( 0 == VxFileUtil::fileExists( fileName.c_str() ) )
		{
			removeFromDbAndList( fileName );
			isInLib = false;
		}
	}

	return isInLib;
}

//============================================================================
bool FileInfoBaseMgr::isFileShared( VxSha1Hash& fileHashId, bool listIsLocked )
{
	bool isInLib = false;
	std::string fileName("");
	if( !listIsLocked )
	{
		lockFileList();
	}

	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileHashId == iter->second.getFileHashId() )
		{
			isInLib = true;
			fileName = iter->second.getFileName();
			break;
		}
	}

	if( !listIsLocked )
	{
		unlockFileList();
	}

	if( isInLib && fileName.size() )
	{
		if( 0 == VxFileUtil::fileExists( fileName.c_str() ) )
		{
			removeFromDbAndList( fileName );
			isInLib = false;
		}
	}

	return isInLib;
}

//============================================================================
bool FileInfoBaseMgr::isFileShared( VxGUID& assetId, bool listIsLocked )
{
	bool isInLib = false;
	std::string fileName( "" );
	if( !listIsLocked )
	{
		lockFileList();
	}

	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( assetId == iter->second.getAssetId() )
		{
			isInLib = true;
			fileName = iter->second.getFileName();
			break;
		}
	}

	if( !listIsLocked )
	{
		unlockFileList();
	}

	if( isInLib && fileName.size() )
	{
		if( 0 == VxFileUtil::fileExists( fileName.c_str() ) )
		{
			removeFromDbAndList( fileName, listIsLocked );
			isInLib = false;
		}
	}

	return isInLib;
}

//============================================================================
bool FileInfoBaseMgr::isFileShared( VxGUID& assetId, VxSha1Hash& fileHashId, bool listIsLocked )
{
	bool isShared = isFileShared( assetId, listIsLocked );
	if( isShared && fileHashId.isHashValid() )
	{
		isShared &= isFileShared( fileHashId, listIsLocked );
	}

	return isShared;
}

//============================================================================
void FileInfoBaseMgr::removeFromDbAndList( std::string& fileName, bool listIsLocked )
{
	if( !listIsLocked )
	{
		lockFileList();
	}

	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( fileName == iter->second.getFileName() )
		{
			m_FileInfoDb.removeFile( fileName );
			m_FileInfoList.erase( iter );
			break;
		}
	}

	if( !listIsLocked )
	{
		unlockFileList();
	}
}

//============================================================================
bool FileInfoBaseMgr::getFileFullName( VxGUID& assetId, VxSha1Hash& fileHashId, std::string& retFileFullName )
{
	bool isShared = false;
	lockFileList();
	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter )
	{
		if( assetId == iter->second.getAssetId() && fileHashId == iter->second.getFileHashId() )
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
bool FileInfoBaseMgr::getFileHashId( std::string& fileFullName, VxSha1Hash& retFileHashId )
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

	unlockFileList();
	return foundHash;
}

//============================================================================
void FileInfoBaseMgr::callbackSha1GenerateResult( ESha1GenResult sha1GenResult, VxGUID& assetId, Sha1Info& sha1Info )
{
	lockFileList();
	for( auto iter = m_FileInfoNeedHashAndSaveList.begin(); iter != m_FileInfoNeedHashAndSaveList.end(); )
	{
		bool wasErased{ false };
		FileInfo& fileInfo = *iter;
		if( assetId == fileInfo.getAssetId() )
		{
			if( eSha1GenResultNoError == sha1GenResult )
			{
				fileInfo.setFileHashId( sha1Info.getSha1Hash() );
				fileInfo.setFileTime( GetHighResolutionTimeMs() );
				m_FileInfoDb.addFile( fileInfo );
				m_FileInfoList[ fileInfo.getAssetId() ] = fileInfo;

				iter = m_FileInfoNeedHashAndSaveList.erase( iter );
				wasErased = true;
			}
			else
			{
				LogMsg( LOG_VERBOSE, "FileInfoBaseMgr::callbackSha1GenerateResult failed %s", DescribeSha1GenResult( sha1GenResult ) );
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
bool FileInfoBaseMgr::fromGuiAddSharedFile( const char* fileNameIn, bool addFile, uint8_t* fileHashId )
{
	std::string fileName = fileNameIn;
	VxGUID assetId;
	assetId.initializeWithNewVxGUID();
	return addFileToDbAndList( getEngine().getMyOnlineId(), fileName, assetId );
}

//============================================================================
bool FileInfoBaseMgr::loadAboutMePageStaticAssets( void )
{
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
				FileInfo fileInfo( getEngine().getMyOnlineId(), fullFileName, fileLen, VxFileExtensionToFileTypeFlag( fullFileName.c_str() ), assetId, sha1Hash );
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
bool FileInfoBaseMgr::loadStoryboardPageFileAssets( void )
{
	static std::vector<std::string>	g_StoryboardNameList{
		{ "favicon.ico" },				// !0C42AEB1E8072DBE12B699D027EB2393! no limit icon
		{ "storyboard_thumb.png" },		// !F841B8C2C7953211B11ED11DEF45D19A! story_board thumb
		{ "story_board.htm" },			// !F6460B0A160B362D30B11737E4CB018B! story_board index
		{ "storyboard_background.png" }, // !124F588DFCD993003BC21C1AA5C785A3! story_board background
		{ "me.png" },					// !52452C124D2D3D7288114D7EC151A6BC! story_board me.png
	};


	if( m_RootFileFolder.empty() )
	{
		LogMsg( LOG_ERROR, "loadStoryboardPageFileAssets No Root File Folder" );
		return false;
	}

	std::vector<std::string> fileList;
	VxFileUtil::listFilesInDirectory( m_RootFileFolder.c_str(), fileList );
	if( fileList.size() < g_StoryboardNameList.size() )
	{
		LogMsg( LOG_ERROR, "loadStoryboardPageFileAssets Missing Files only %d found", fileList.size() );
		return false;
	}

	// TODO loading and generate has should probably be put in a unique thread for people how create a very long storyboard web page
	int fileCount{ 0 };
	bool result{ true };
	for( auto& fullFileName : fileList )
	{
		int64_t fileLen = VxFileUtil::fileExists( fullFileName.c_str() );
		VxGUID assetId;
		if( fileLen )
		{
			assetId.initializeWithNewVxGUID();
			VxSha1Hash sha1Hash;
			if( sha1Hash.generateHashFromFile( fullFileName.c_str() ) )
			{
				FileInfo fileInfo( getEngine().getMyOnlineId(), fullFileName, fileLen, VxFileExtensionToFileTypeFlag( fullFileName.c_str() ), assetId, sha1Hash );
				if( fileInfo.isValid( true ) )
				{
					lockFileList();
					m_FileInfoList[assetId] = fileInfo;
					unlockFileList();
					fileCount++;
				}
				else
				{
					LogMsg( LOG_ERROR, "loadStoryboardPageFileAssets Invalid File %s", fullFileName.c_str() );
					result = false;
					break;
				}
			}
			else
			{
				LogMsg( LOG_ERROR, "loadStoryboardPageFileAssets Generate Sha1Hash Failed File %s", fullFileName.c_str() );
				result = false;
				break;
			}
		}
		else
		{
			LogMsg( LOG_ERROR, "loadStoryboardPageFileAssets 0 file Len File %s", fullFileName.c_str() );
			result = false;
			break;
		}
	}

	return result && fileCount == fileList.size();
}

//============================================================================
void FileInfoBaseMgr::updateFileTypes( void )
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
void FileInfoBaseMgr::checkForInitializeCompleted( void )
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
bool FileInfoBaseMgr::findFileAsset( VxGUID& fileAssetId, FileInfo& fileInfo)
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
ECommErr FileInfoBaseMgr::searchRequest( PktFileInfoSearchReply& pktReply, VxGUID& specificAssetId, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
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
				LogMsg( LOG_VERBOSE, "FileInfoBaseMgr::searchRequest Warning search text was to short.. sending all files" );
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
ECommErr FileInfoBaseMgr::searchMoreRequest( PktFileInfoMoreReply& pktReply, VxGUID& nextFileAssetId, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
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
bool FileInfoBaseMgr::startDownload( FileInfo& fileInfo, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
return m_FileInfoXferMgr.startDownload( fileInfo, searchSessionId, sktBase, netIdent );
}

//============================================================================
bool FileInfoBaseMgr::onFileDownloadComplete( VxNetIdent* netIdent, VxSktBase* sktBase, VxGUID& lclSessionId, std::string& fileName, VxGUID& assetId, VxSha1Hash& sha11Hash )
{
	LogMsg( LOG_VERBOSE, "FileInfoBaseMgr::onFileDownloadComplete %s", fileName.c_str() );
	return m_Plugin.onFileDownloadComplete( netIdent, sktBase, lclSessionId, fileName, assetId, sha11Hash );
}

//============================================================================
void FileInfoBaseMgr::toGuiRxedPluginOffer( VxNetIdent* netIdent,				// identity of friend
	EPluginType		ePluginType,			// plugin type
	const char* pOfferMsg,				// offer message
	int				pvUserData,				// plugin defined data
	const char* pFileName,		// filename if any
	uint8_t* fileHashData,
	VxGUID& lclSessionId,
	VxGUID& rmtSessionId )
{
	m_Plugin.toGuiRxedPluginOffer( netIdent, ePluginType, pOfferMsg, pvUserData, pFileName, fileHashData, lclSessionId, rmtSessionId );
}

//============================================================================
void FileInfoBaseMgr::toGuiRxedOfferReply( VxNetIdent* netIdent,
	EPluginType		ePluginType,
	int				pvUserData,
	EOfferResponse	eOfferResponse,
	const char* pFileName,
	uint8_t* fileHashData,
	VxGUID& lclSessionId,
	VxGUID& rmtSessionId )
{
	m_Plugin.toGuiRxedOfferReply( netIdent, ePluginType, pvUserData, eOfferResponse, pFileName, fileHashData, lclSessionId, rmtSessionId );
}

//============================================================================
void FileInfoBaseMgr::toGuiStartUpload( VxNetIdent* netIdent,
	EPluginType		ePluginType,
	VxGUID& lclSessionId,
	uint8_t			u8FileType,
	uint64_t		u64FileLen,
	const char* pFileName,
	VxGUID			assetId,
	uint8_t* fileHashData )
{
	m_Plugin.toGuiStartUpload( netIdent, ePluginType, lclSessionId, u8FileType, u64FileLen, pFileName, assetId, fileHashData );
}

//============================================================================
void FileInfoBaseMgr::toGuiStartDownload( VxNetIdent* netIdent,
	EPluginType		ePluginType,
	VxGUID& lclSessionId,
	uint8_t			u8FileType,
	uint64_t		u64FileLen,
	const char* pFileName,
	VxGUID			assetId,
	uint8_t* fileHashData )
{
	m_Plugin.toGuiStartUpload( netIdent, ePluginType, lclSessionId, u8FileType, u64FileLen, pFileName, assetId, fileHashData );
}

//============================================================================
void FileInfoBaseMgr::updateToGuiFileXferState( VxGUID& lclSessionId, EXferState xferState, EXferError xferErr, int param )
{
	LogMsg( LOG_VERBOSE, "FileInfoBaseMgr::toGuiFileXferState xferState %s xferErr %s param %d", DescribeXferState( xferState ), DescribeXferError( xferErr ), param );
	m_Plugin.toGuiFileXferState( lclSessionId, xferState, xferErr, param );
}

//============================================================================
void FileInfoBaseMgr::toGuiFileDownloadComplete( VxGUID& lclSessionId, const char* newFileName, EXferError xferError )
{
	m_Plugin.toGuiFileDownloadComplete( lclSessionId, newFileName, xferError );
}

//============================================================================
void FileInfoBaseMgr::toGuiFileUploadComplete( VxGUID& lclSessionId, EXferError xferError )
{
	m_Plugin.toGuiFileUploadComplete( lclSessionId, xferError );
}

//============================================================================
bool FileInfoBaseMgr::fromGuiGetSharedFiles( uint8_t fileTypeFilter )
{
	int sharedFilesCnt{ 0 };
	lockFileList();

	for( auto fileInfoMap : m_FileInfoList )
	{
		FileInfo& fileInfo = fileInfoMap.second;
		if( 0 != (fileTypeFilter & fileInfo.getFileType()) )
		{
			bool isShared{ false };
			bool isInLibrary{ false };
			if( isFileShareServer() )
			{
				isShared = true;
				isInLibrary = getEngine().fromGuiGetIsFileInLibrary( fileInfo.getLocalFileName().c_str() );
			}
			else if( isLibraryServer() )
			{
				isInLibrary = true;
				isShared = getEngine().fromGuiGetIsFileShared( fileInfo.getLocalFileName().c_str() );
			}
			else
			{
				isInLibrary = getEngine().fromGuiGetIsFileInLibrary( fileInfo.getLocalFileName().c_str() );
				isShared = getEngine().fromGuiGetIsFileShared( fileInfo.getLocalFileName().c_str() );
			}

			IToGui::getToGui().toGuiFileList( fileInfo.getFileName().c_str(),
												fileInfo.getFileLength(),
												fileInfo.getFileType(),
												isShared,
												isInLibrary,
												fileInfo.getAssetId(),
												fileInfo.getFileHashId().getHashData()
												);

			sharedFilesCnt++;
		}
	}

	unlockFileList();
	IToGui::getToGui().toGuiFileList( "", 0, 0, false, false, VxGUID::nullVxGUID() );
	return sharedFilesCnt;
}

//============================================================================
bool FileInfoBaseMgr::fromGuiSetFileIsShared( std::string fileName, bool isShared, uint8_t* fileHashId )
{
	if( fileName.empty() )
	{
		LogMsg( LOG_ERROR, "FileInfoBaseMgr::fromGuiSetFileIsShared empty file name" );
		return false;
	}

	lockFileList();

	for( auto iter = m_FileInfoList.begin(); iter != m_FileInfoList.end(); ++iter)
	{
		FileInfo& fileInfo = iter->second;
		if( fileName == fileInfo.getFileName() )
		{
			if( false == isShared )
			{
				m_FileInfoDb.removeFile( fileName );
				m_FileInfoList.erase( iter );
				unlockFileList();
				updateFileTypes();
				return true;
			}
			else
			{
				unlockFileList();
				return true;
			}
		}
	}

	unlockFileList();

	if( isShared )
	{
		// file is not currently shared and should be
		uint64_t fileLen = VxFileUtil::fileExists( fileName.c_str() );
		uint8_t fileType = VxFileExtensionToFileTypeFlag( fileName.c_str() );
		if( (false == isAllowedFileOrDir( fileName ))
			|| (0 == fileLen) )
		{
			LogMsg( LOG_ERROR, "FileInfoBaseMgr::fromGuiSetFileIsShared file does not exist or not allowed %s", fileName.c_str() );
			return false;
		}

		VxGUID newAssetId;
		newAssetId.initializeWithNewVxGUID();

		return addFileToDbAndList( getEngine().getMyOnlineId(), fileName, newAssetId );

	}

	return false;
}

//============================================================================
bool FileInfoBaseMgr::fromGuiGetIsFileShared( std::string fileName )
{
	return isFileShared( fileName );
}

//============================================================================
bool FileInfoBaseMgr::isFileShareServer( void )
{
	return m_Plugin.getPluginType() == ePluginTypeFileShareServer;
}

//============================================================================
bool FileInfoBaseMgr::isLibraryServer( void )
{
	return m_Plugin.getPluginType() == ePluginTypeLibraryServer;
}