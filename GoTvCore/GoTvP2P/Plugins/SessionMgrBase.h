#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include <CoreLib/VxMutex.h>

class P2PEngine;
class PluginBase;
class PluginMgr;
class IToGui;
class VxNetIdent;
class VxSktBase;

class SessionMgrBase
{
public:
	class AutoSessionMgrLock
	{
	public:
		AutoSessionMgrLock( SessionMgrBase * mgr ) : m_SessionMgrMutex(mgr->getSessionMgrMutex())	{ m_SessionMgrMutex.lock(); }
		~AutoSessionMgrLock()																		{ m_SessionMgrMutex.unlock(); }
		VxMutex&				m_SessionMgrMutex;
	};

	SessionMgrBase( P2PEngine& engine, PluginBase& plugin, PluginMgr& pluginMgr );
	virtual ~SessionMgrBase() = default;

	PluginBase&					getPlugin( void )							{ return m_Plugin; }
	PluginMgr&					getPluginMgr( void )						{ return m_PluginMgr; }
    IToGui&						getToGui( void );
	VxMutex&					getSessionMgrMutex( void )					{ return m_SessionMgrMutex; }

	bool						isPluginSingleSession( void );

	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) = 0;
	virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase ) = 0;
	virtual void				onConnectionLost( VxSktBase * sktBase ) = 0;


protected:
	//=== vars ===//
    P2PEngine&                  m_Engine;
	PluginBase&					m_Plugin;
	PluginMgr&					m_PluginMgr;
	
	VxMutex						m_SessionMgrMutex;

private:
	SessionMgrBase(); // do not allow
};
