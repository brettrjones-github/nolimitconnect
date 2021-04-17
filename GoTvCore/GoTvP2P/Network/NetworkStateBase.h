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

#include <CoreLib/VxDefs.h>

#include <GuiInterface/IToGui.h>
#include <GuiInterface/IFromGui.h>

class P2PEngine;
class NetworkStateMachine;
class NetServicesMgr;
class VxNetIdent;
class PktAnnounce;
class VxConnectInfo;
class PktRelayServiceReply;
class VxSktBase;

class NetworkStateBase
{
public:
	NetworkStateBase( NetworkStateMachine& stateMachine );
    virtual ~NetworkStateBase(){};

	virtual ENetworkStateType	getNetworkStateType( void )									{ return m_ENetworkStateType; }
	virtual void				setNetworkStateType( ENetworkStateType eStateType )			{ m_ENetworkStateType = eStateType; }

	virtual void				enterNetworkState( void )									{};
	virtual void				runNetworkState( void )										{};
	virtual void				exitNetworkState( void )									{};

	virtual void				fromGuiUserLoggedOn( VxNetIdent * netIdent )				{};
	virtual void				fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork = false ){};
	virtual void				fromGuiNetworkLost( void )									{};
    virtual ENetLayerState	    fromGuiGetNetLayerState( ENetLayerType netLayer = eNetLayerTypeInternet ) { return eNetLayerStateUndefined; }

	virtual void				fromGuiUseRelay( VxConnectInfo& connectInfo, bool useRelay ) {}; // so if searching for relay can try immediately 

	virtual void				onPktRelayServiceReply( VxSktBase * sktBase, PktRelayServiceReply * pkt ) {};

	virtual void				checkAndHandleNetworkEvents( void );

protected:
	bool						checkForAbortOrShutdown( void );
	bool						networkStateChangePending( void );

	//=== vars ===//
	P2PEngine&					m_Engine;
	PktAnnounce&				m_PktAnn;
	NetServicesMgr&				m_NetServicesMgr;
	NetworkStateMachine&		m_NetworkStateMachine;
	ENetworkStateType			m_ENetworkStateType;
};


