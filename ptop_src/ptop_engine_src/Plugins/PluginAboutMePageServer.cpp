//============================================================================
// Copyright (C) 2012 Brett R. Jones
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

#include "PluginAboutMePageServer.h"
#include "PluginMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktsStoryBoard.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
PluginAboutMePageServer::PluginAboutMePageServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBaseFiles( engine, pluginMgr, myIdent, pluginType, "AboutMePageFilesServer.db3")
{
	setPluginType( ePluginTypeAboutMePageServer );
}

//============================================================================
void PluginAboutMePageServer::onAfterUserLogOnThreaded( void )
{
	m_RootFileFolder = VxGetAppDirectory( eAppDirAboutMePageServer );
	getFileInfoMgr().setRootFolder( m_RootFileFolder );

	getFileInfoMgr().onAfterUserLogOnThreaded();
}

//============================================================================
void PluginAboutMePageServer::onLoadedFilesReady( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	getFileInfoMgr().getAboutMePageStaticAssets( m_AssetList );

	bool isReady{ true };
	for( auto assetPair : m_AssetList )
	{
		if( !getFileInfoMgr().isFileInLibrary( assetPair.first ) )
		{
			isReady = false;
			std::string fullFileName = m_RootFileFolder + assetPair.second;
			getFileInfoMgr().addFileToLibrary( m_Engine.getMyOnlineId(), fullFileName, assetPair.first );
		}
	}

	if( isReady )
	{
		setIsAboutMePageReady( true );
	}
}

//============================================================================
void PluginAboutMePageServer::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	checkIsAboutMePageReady();
}

//============================================================================
void PluginAboutMePageServer::checkIsAboutMePageReady( void )
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
void PluginAboutMePageServer::setIsAboutMePageReady( bool isReady )
{
	if( m_AboutMePageReady != isReady )
	{
		m_AboutMePageReady = isReady;
		onAboutMePageReady( isReady );
	}
}

//============================================================================
void PluginAboutMePageServer::onAboutMePageReady( bool isReady )
{
	m_AboutMePageReady = isReady;
	// do stuff
}