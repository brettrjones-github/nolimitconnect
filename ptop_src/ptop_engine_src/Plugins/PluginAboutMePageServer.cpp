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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginAboutMePageServer.h"
#include "PluginMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktsFileInfo.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
PluginAboutMePageServer::PluginAboutMePageServer( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBaseFilesServer( engine, pluginMgr, myIdent, pluginType, "AboutMePageFilesServer.db3")
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
	if( !getFileInfoMgr().loadAboutMePageStaticAssets() )
	{
		LogMsg( LOG_ERROR, "PluginAboutMePageServer::onLoadedFilesReady failed or missing web files" );
	}
	else
	{
		setIsWebPageServerReady( true );
	}
}

//============================================================================
void PluginAboutMePageServer::onFilesChanged( int64_t lastFileUpdateTime, int64_t totalBytes, uint16_t fileTypes )
{
	checkIsWebPageServerReady();
}

//============================================================================
void PluginAboutMePageServer::checkIsWebPageServerReady( void )
{
}

//============================================================================
void PluginAboutMePageServer::setIsWebPageServerReady( bool isReady )
{
	if( m_WebPageServerReady != isReady )
	{
		m_WebPageServerReady = isReady;
		onWebPageServerReady( isReady );
	}
}

//============================================================================
void PluginAboutMePageServer::onWebPageServerReady( bool isReady )
{
	m_WebPageServerReady = isReady;
	// do stuff
}

//============================================================================
void PluginAboutMePageServer::onNetworkConnectionReady( bool requiresRelay )
{
	if( eFriendStateIgnore != m_MyIdent->getPluginPermission( getPluginType() ) )
	{
		updateHasProfilePictureChanged();
	}
}

//============================================================================
void PluginAboutMePageServer::updateHasProfilePictureChanged( void )
{
	std::string profilePicFile = VxGetAppDirectory( eAppDirAboutMePageServer );
	profilePicFile += "me.png";
	uint64_t fileSize = VxFileUtil::fileExists( profilePicFile.c_str() );

	// TODO.. DO NOT HARDCODE DEFAULT PROFILE PIC SIZE
	if( fileSize && 2500 != fileSize )
	{
		m_MyIdent->setHasProfilePicture( true );
	}
}