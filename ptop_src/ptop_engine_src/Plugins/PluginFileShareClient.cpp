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

#include "PluginFileShareClient.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
PluginFileShareClient::PluginFileShareClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBaseFilesClient( engine, pluginMgr, myIdent, pluginType, "FileShareClient.db3" )
{
	setPluginType( ePluginTypeFileShareClient );
}

//============================================================================
void PluginFileShareClient::onAfterUserLogOnThreaded( void )
{
	m_RootFileFolder = VxGetAppDirectory( eAppDirStoryBoardPageClient );
	getFileInfoMgr().setRootFolder( m_RootFileFolder );

	getFileInfoMgr().onAfterUserLogOnThreaded();
}

//============================================================================
void PluginFileShareClient::onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	LogMsg( LOG_VERBOSE, "PluginFileShareClient::onLoadedFilesReady" );
	checkIsWebPageClientReady();
}

//============================================================================
void PluginFileShareClient::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	checkIsWebPageClientReady();
}

//============================================================================
bool PluginFileShareClient::onFileDownloadComplete( VxNetIdent* netIdent, VxSktBase* sktBase, VxGUID& lclSessionId, std::string& fileName, VxGUID& assetId, VxSha1Hash& sha11Hash )
{
	bool result = netIdent && sktBase && lclSessionId.isVxGUIDValid() && !fileName.empty() && assetId.isVxGUIDValid() && sha11Hash.isHashValid();
	if( result )
	{
		result = false;
		// move from in progress to completed
		lockInProgressFileList();
		for( auto iter = m_InProgressFileInfoList.begin(); iter != m_InProgressFileInfoList.end(); ++iter )
		{
			FileInfo& fileInfo = *iter;
			if( fileInfo.getAssetId() == assetId && fileInfo.getFileHashId() == sha11Hash )
			{
				lockCompletedFileList();
				fileInfo.setFileName( fileName );
				fileInfo.setIsDirty( false );
				m_CompletedFileInfoList.push_back( fileInfo );
				m_InProgressFileInfoList.erase( iter );
				result = true;
				unlockCompletedFileList();
				break;
			}
		}

		unlockInProgressFileList();
		if( result )
		{
			result = false;
			if( m_SearchFileInfoList.empty() && m_InProgressFileInfoList.empty() )
			{
				// find the index file and send to gui
				FileInfo indexFileInfo;
				lockCompletedFileList();
				for( auto iter = m_CompletedFileInfoList.begin(); iter != m_CompletedFileInfoList.end(); ++iter )
				{
					FileInfo& fileInfo = *iter;
					if( fileInfo.getShortFileName() == getWebIndexFileName() )
					{
						indexFileInfo = fileInfo;
						result = true;
						break;
					}
				}

				unlockCompletedFileList();
				if( result )
				{
					// all done
					m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgDownloadComplete, indexFileInfo.getFullFileName() );
				}
				else
				{
					// failed to find the web index file in downloaded files
					m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgDownloadFailed, "", 0 );
				}
			}
			else
			{
				result = startDownload( lclSessionId, sktBase, netIdent );
			}
		}
		else
		{
			m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgDownloadFailed, "", 0 );
		}
	}
	else
	{
		m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgInvalidParam, "", 0 );
	}

	return result;
}

//============================================================================
void PluginFileShareClient::checkIsWebPageClientReady( void )
{
	setIsWebPageClientReady( getFileInfoMgr().getIsInitialized() );
}

//============================================================================
void PluginFileShareClient::setIsWebPageClientReady( bool isReady )
{
	if( m_WebPageClientReady != isReady )
	{
		m_WebPageClientReady = isReady;
		onWebPageClientReady( isReady );
	}
}

//============================================================================
void PluginFileShareClient::onWebPageClientReady( bool isReady )
{

}

//============================================================================
std::string	PluginFileShareClient::getIncompleteFileXferDirectory( VxGUID& onlineId )
{
	std::string incompleteDir{ "" };
	if( onlineId.isVxGUIDValid() )
	{
		incompleteDir = m_RootFileFolder + onlineId.toHexString().c_str() + "/";
		VxFileUtil::makeDirectory( incompleteDir.c_str() );
		if( VxFileUtil::directoryExists( incompleteDir.c_str() ) )
		{
			int64_t diskFreeSpace = VxFileUtil::getDiskFreeSpace( incompleteDir.c_str() );

			if( diskFreeSpace && diskFreeSpace < VxFileUtil::SIZE_1GB )
			{
				m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgLowDiskSpace, "" );
			}
		}
		else
		{
			m_Engine.getToGui().toGuiPluginMsg( getPluginType(), onlineId, ePluginMsgPermissionError, incompleteDir.c_str() );
		}
	}

	return incompleteDir;
}

//============================================================================
bool PluginFileShareClient::fromGuiDownloadWebPage( EWebPageType webPageType, VxGUID& onlineId )
{
	bool result{ false };
	if( (eWebPageTypeAboutMe == webPageType || eWebPageTypeStoryboard == webPageType ) && onlineId.isVxGUIDValid() )
	{
		m_HisOnlineId = onlineId;
		m_DownloadFileFolder = getIncompleteFileXferDirectory( onlineId );
		if( VxFileUtil::directoryExists( m_DownloadFileFolder.c_str() ) )
		{
			// must clear any previous files or download will make duplicates filename_1 filename_2 etc
			VxFileUtil::deleteFilesInFolder( m_DownloadFileFolder, true );
			m_WebPageIndexFile = m_DownloadFileFolder + getWebIndexFileName();
			int64_t diskFreeSpace = VxFileUtil::getDiskFreeSpace( m_DownloadFileFolder.c_str() );

			if( diskFreeSpace && diskFreeSpace < VxFileUtil::SIZE_1GB )
			{
				m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgLowDiskSpace, "" );
			}
			else
			{
				m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgConnecting, "" );
				connectForWebPageDownload( onlineId );
			}
		}
		else
		{
			m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgPermissionError, m_DownloadFileFolder.c_str() );
		}
	}
	else
	{
		LogMsg( LOG_VERBOSE, "PluginFileShareClient::fromGuiDownloadWebPage invalid EWebPageType" );
	}

	return result;
}

//============================================================================
bool PluginFileShareClient::fromGuiCancelWebPage( EWebPageType webPageType, VxGUID& onlineId )
{
	bool result{ false };
	if( eWebPageTypeAboutMe == webPageType )
	{
		cancelDownload();
		m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgCanceled, "" );

	}
	else
	{
		LogMsg( LOG_VERBOSE, "PluginFileShareClient::fromGuiCancelWebPage invalid EWebPageType" );
	}

	return result;
}


//============================================================================
bool PluginFileShareClient::fileInfoSearchResult( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, FileInfo& fileInfo )
{
	bool result{ false };
	if( fileInfo.determineFullFileName( m_DownloadFileFolder ) )
	{
		result = fileInfo.isValid( true );
		if( result )
		{
			lockSearchFileList();
			m_SearchFileInfoList.push_back( fileInfo );
			unlockSearchFileList();
		}
	}

	return result;
}

//============================================================================
void PluginFileShareClient::fileInfoSearchCompleted( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr )
{
	if( commErr == eCommErrNone )
	{
		LogMsg( LOG_VERBOSE, "PluginFileShareClient::fileInfoSearchCompleted with no errors" );
		bool webIndexFileFound{ false };
		for( auto& fileInfo : m_SearchFileInfoList )
		{
			if( fileInfo.getFileName() == m_WebPageIndexFile )
			{
				webIndexFileFound = true;
				break;
			}
		}

		if( webIndexFileFound )
		{
			m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgDownloading, "" );
			if( !startDownload( searchSessionId, sktBase, netIdent ) )
			{
				m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgDownloadFailed, "" );
				cancelDownload();
			}
		}
		else
		{
			cancelDownload();
			m_Engine.getToGui().toGuiPluginCommError( getPluginType(), m_HisOnlineId, ePluginMsgRetrieveInfoFailed, eCommErrInvalidParam );
		}
	}
	else
	{
		cancelDownload();
		LogMsg( LOG_ERROR, "PluginFileShareClient::fileInfoSearchCompleted with error %s from %s", DescribeCommError( commErr ), sktBase->describeSktConnection().c_str() );
		m_Engine.getToGui().toGuiPluginCommError( getPluginType(), m_HisOnlineId, ePluginMsgRetrieveInfoFailed, commErr );
	}
}

//============================================================================
void PluginFileShareClient::cancelDownload( void )
{
	lockSearchFileList();
	for( auto& fileInfo : m_SearchFileInfoList )
	{
		m_FileInfoMgr.cancelAndDelete( fileInfo.getAssetId() );
	}

	m_SearchFileInfoList.clear();
	unlockSearchFileList();

	lockInProgressFileList();
	for( auto& fileInfo : m_InProgressFileInfoList )
	{
		m_FileInfoMgr.cancelAndDelete( fileInfo.getAssetId() );
	}

	m_InProgressFileInfoList.clear();
	unlockInProgressFileList();

	lockCompletedFileList();
	for( auto& fileInfo : m_InProgressFileInfoList )
	{
		m_FileInfoMgr.cancelAndDelete( fileInfo.getAssetId() );
	}

	m_InProgressFileInfoList.clear();
	unlockCompletedFileList();

	// clear out files on cancel
	VxFileUtil::deleteFilesInFolder( m_DownloadFileFolder, true );
}

//============================================================================
bool PluginFileShareClient::startDownload( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	bool result{ false };
	lockSearchFileList();
	for( auto iter = m_SearchFileInfoList.begin(); iter != m_SearchFileInfoList.end(); ++iter )
	{
		FileInfo& fileInfo = *iter;
		lockInProgressFileList();
		VxGUID xferSessionId = fileInfo.initializeNewXferSessionId();
		fileInfo.setIsDirty( true );
		m_InProgressFileInfoList.push_back( fileInfo );
		if( m_FileInfoMgr.startDownload( *iter, xferSessionId, sktBase, netIdent ) )
		{
			result = true;
			m_SearchFileInfoList.erase( iter );
		}
		else
		{
			m_InProgressFileInfoList.pop_back();
		}

		unlockInProgressFileList();
		break;
	}

	unlockSearchFileList();
	return result;
}

//============================================================================
bool PluginFileShareClient::fromGuiDownloadFileList( VxGUID& onlineId, VxGUID& sessionId, uint8_t fileTypes )
{
	bool result{ false };
	m_HisOnlineId = onlineId;
	m_SearchSessionId = sessionId;
	m_LclSessionId = sessionId;
	m_DownloadFileFolder = getIncompleteFileXferDirectory( onlineId );
	if( VxFileUtil::directoryExists( m_DownloadFileFolder.c_str() ) )
	{
		int64_t diskFreeSpace = VxFileUtil::getDiskFreeSpace( m_DownloadFileFolder.c_str() );

		if( diskFreeSpace && diskFreeSpace < VxFileUtil::SIZE_1GB )
		{
			m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgLowDiskSpace, "" );
		}
		else
		{
			m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgConnecting, "" );
			if( !fileTypes )
			{
				fileTypes = VXFILE_TYPE_ALLNOTEXE;
			}

			setSearchFileTypes( fileTypes );
			result = connectForFileListDownload( onlineId );
		}
	}
	else
	{
		m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgPermissionError, m_DownloadFileFolder.c_str() );
	}

	return result;
}

//============================================================================
bool PluginFileShareClient::fromGuiDownloadFileListCancel( VxGUID& onlineId, VxGUID& sessionId )
{

	return false;
}