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

#include <PktLib/PktsStoryBoard.h>

//============================================================================
PluginAboutMePageClient::PluginAboutMePageClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
{
	setPluginType( ePluginTypeAboutMePageClient );
}

//============================================================================
//! return true if is plugin session
bool PluginAboutMePageClient::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return true;
}

//============================================================================
void PluginAboutMePageClient::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	//m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginAboutMePageClient::onConnectionLost( VxSktBase * sktBase )
{
	//m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginAboutMePageClient::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	//m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}
