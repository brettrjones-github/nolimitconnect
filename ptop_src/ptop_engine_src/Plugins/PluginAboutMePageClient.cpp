//============================================================================
// Copyright (C) 2010 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "PluginAboutMePageClient.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/Plugins/FileInfo.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktsStoryBoard.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
PluginAboutMePageClient::PluginAboutMePageClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
	: PluginBaseFilesClient( engine, pluginMgr, myIdent, pluginType, "AboutMePageFilesClient.db3" )
{
	setPluginType( ePluginTypeAboutMePageClient );
}

//============================================================================
void PluginAboutMePageClient::onAfterUserLogOnThreaded( void )
{
	m_RootFileFolder = VxGetAppDirectory( eAppDirAboutMePageClient );
	getFileInfoMgr().setRootFolder( m_RootFileFolder );

	getFileInfoMgr().onAfterUserLogOnThreaded();
}

//============================================================================
void PluginAboutMePageClient::onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	LogMsg( LOG_VERBOSE, "PluginAboutMePageClient::onLoadedFilesReady" );
	checkIsAboutMePageReady();
}

//============================================================================
void PluginAboutMePageClient::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	checkIsAboutMePageReady();
}

//============================================================================
void PluginAboutMePageClient::checkIsAboutMePageReady( void )
{
	bool isReady{ true };
	for( auto assetPair : m_AssetList )
	{
		if( !getFileInfoMgr().isFileInLibrary( assetPair.first ) )
		{
			isReady = false;
			false;
		}
	}

	setIsAboutMePageReady( isReady && getFileInfoMgr().getIsInitialized() );
}

//============================================================================
void PluginAboutMePageClient::setIsAboutMePageReady( bool isReady )
{
	if( m_AboutMePageReady != isReady )
	{
		m_AboutMePageReady = isReady;
		onAboutMePageReady( isReady );
	}
}

//============================================================================
void PluginAboutMePageClient::onAboutMePageReady( bool isReady )
{

}

//============================================================================
bool PluginAboutMePageClient::fromGuiDownloadWebPage( EWebPageType webPageType, VxGUID& onlineId )
{
	bool result{ false };
	if( eWebPageTypeAboutMe == webPageType && onlineId.isVxGUIDValid() )
	{
		m_HisOnlineId = onlineId;
		m_DownloadFileFolder = m_RootFileFolder + m_HisOnlineId.toHexString().c_str() + "/";
		VxFileUtil::makeDirectory( m_DownloadFileFolder.c_str() );
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
		LogMsg( LOG_VERBOSE, "PluginAboutMePageClient::fromGuiDownloadWebPage invalid EWebPageType" );
	}

	return result;
}

//============================================================================
bool PluginAboutMePageClient::fromGuiCancelWebPage( EWebPageType webPageType, VxGUID& onlineId )
{
	bool result{ false };
	if( eWebPageTypeAboutMe == webPageType )
	{
		lockSearchFileList();
		for( auto &fileInfo : m_SearchFileInfoList )
		{
			m_FileInfoMgr.cancelAndDelete( fileInfo.getAssetId() );
		}

		unlockSearchFileList();
		m_Engine.getToGui().toGuiPluginMsg( getPluginType(), m_HisOnlineId, ePluginMsgCanceled, "" );

	}
	else
	{
		LogMsg( LOG_VERBOSE, "PluginAboutMePageClient::fromGuiCancelWebPage invalid EWebPageType" );
	}

	return result;
}


//============================================================================
void PluginAboutMePageClient::fileInfoSearchResult( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, FileInfo& fileInfo )
{

}

//============================================================================
void PluginAboutMePageClient::fileInfoSearchCompleted( VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr )
{
	if( commErr )
	{
		LogMsg( LOG_ERROR, "FileInfoListMgr::hostSearchCompleted with error %s from %s", DescribeCommError( commErr ), sktBase->describeSktConnection().c_str() );
	}
	else
	{
		LogMsg( LOG_VERBOSE, "FileInfoListMgr::hostSearchCompleted with no errors" );
	}
}
