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
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktsStoryBoard.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
PluginAboutMePageClient::PluginAboutMePageClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
	: PluginBaseFiles( engine, pluginMgr, myIdent, pluginType, "AboutMePageFilesClient.db3" )
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

	setIsAboutMePageReady( isReady );
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
	if( eWebPageTypeAboutMe == webPageType )
	{
		m_HisOnlineId = onlineId;

		m_Engine.getToGui().toGuiPluginMessage( getPluginType(), m_HisOnlineId, ePluginMsgConnecting, "" );
	}
	else
	{
		LogMsg( LOG_VERBOSE, "PluginAboutMePageClient::fromGuiDownloadWebPage invalid EWebPageType" );
	}

	return result;
}