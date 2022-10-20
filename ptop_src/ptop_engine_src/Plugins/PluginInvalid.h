#pragma once
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
#include "PluginBase.h"

//! implements Invalid plugin to catch errors
class PluginInvalid : public PluginBase
{
public:
	PluginInvalid( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
	virtual ~PluginInvalid();

	//! handle app state change
	virtual void				onAppStateChange( EAppState eAppState );
	//! called to start service or session with remote friend
	virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	//! called to stop service or session with remote friend
	virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	//! return true if is plugin session
	virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	//! can accept a new connection/session
	virtual EPluginAccess		canAcceptNewSession( VxNetIdent* netIdent );	

	virtual void				onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase ) {};
	//! called when new better connection from user
	virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt ){};
	virtual void				onConnectionLost( VxSktBase* sktBase ) {}; 

	//=== methods ===//
	virtual	void				fromGuiStartPluginSession( PluginSessionBase* poOffer ) {};

};
