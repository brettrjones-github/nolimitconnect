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

#include "PluginStoryboardServer.h"
#include "PluginMgr.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
PluginStoryboardServer::PluginStoryboardServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
	: PluginBaseFilesServer( engine, pluginMgr, myIdent, pluginType, "StoryboardFilesServer.db3" )
{
	setPluginType( ePluginTypeStoryboardServer );
}

//============================================================================
void PluginStoryboardServer::onAfterUserLogOnThreaded( void )
{
	m_RootFileFolder = VxGetAppDirectory( eAppDirStoryboardPageServer );
	getFileInfoMgr().setRootFolder( m_RootFileFolder );

	getFileInfoMgr().onAfterUserLogOnThreaded();
}

//============================================================================
void PluginStoryboardServer::onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	if( !getFileInfoMgr().loadStoryboardPageFileAssets() )
	{
		LogMsg( LOG_ERROR, "PluginAboutMePageServer::onLoadedFilesReady failed or missing web files" );
	}
	else
	{
		setIsWebPageServerReady( true );
	}
}

//============================================================================
void PluginStoryboardServer::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	checkIsWebPageServerReady();
}

//============================================================================
void PluginStoryboardServer::checkIsWebPageServerReady( void )
{
}

//============================================================================
void PluginStoryboardServer::setIsWebPageServerReady( bool isReady )
{
	if( m_WebPageServerReady != isReady )
	{
		m_WebPageServerReady = isReady;
		onWebPageServerReady( isReady );
	}
}

//============================================================================
void PluginStoryboardServer::onWebPageServerReady( bool isReady )
{
	m_WebPageServerReady = isReady;
	// do stuff
}
