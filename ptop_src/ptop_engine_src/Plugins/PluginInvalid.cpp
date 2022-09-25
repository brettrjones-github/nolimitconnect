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
// http://www.nolimitconnect.org
//============================================================================
#include "PluginInvalid.h"

PluginInvalid::PluginInvalid( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
{
	m_ePluginType = ePluginTypeInvalid;
}

//============================================================================
PluginInvalid::~PluginInvalid()
{
}

//============================================================================
//=== overrides ===//
//============================================================================

//============================================================================
//! handle app state change
void PluginInvalid::onAppStateChange( EAppState eAppState )
{
	LogMsg( LOG_INFO, "PluginInvalid::onAppStateChange %d", eAppState );
	switch( eAppState)
	{
	case eAppStateStartup:	// app has started
		break;
	case eAppStateShutdown:	// app shutdown
		break;
	case eAppStateSleep:	// app sleep
		break;
	case eAppStateWake:		// app wake
		break;
	case eAppStatePause:	// pause app
		break;
	case eAppStateResume:	// resume
		break;
	default:
		LogMsg( LOG_ERROR, "PluginInvalid::onAppStateChange unknown state");
	}
}
//============================================================================
//=== plugin session ===//
//============================================================================
//============================================================================
//! called to start service or session with remote friend
void PluginInvalid::fromGuiStartPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{

}
//============================================================================
//! called to stop service or session with remote friend
void PluginInvalid::fromGuiStopPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{

}
//============================================================================
//! return true if is plugin session
bool PluginInvalid::fromGuiIsPluginInSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return true;
}

//============================================================================
EPluginAccess PluginInvalid::canAcceptNewSession( VxNetIdent* netIdent )
{
	return ePluginAccessDisabled;
}

//============================================================================
//=== methods ===//
//============================================================================



