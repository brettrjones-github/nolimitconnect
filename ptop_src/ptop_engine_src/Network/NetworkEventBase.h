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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/VxDefs.h>

enum ENetworkEventType
{
	eNetworkEventTypeUnknown				= 0,
	eNetworkEventTypeLost					= 1,
	eNetworkEventTypeAvail					= 2,

	eMaxNetworkEventType
};

class P2PEngine;
class NetworkStateMachine;
class NetServicesMgr;
class VxNetIdent;
class PktAnnounce;
class VxConnectInfo;
class PktRelayServiceReply;
class VxSktBase;

class NetworkEventBase
{
public:
	NetworkEventBase( NetworkStateMachine& stateMachine );

	virtual ENetworkEventType	getNetworkEventType( void )									{ return m_eNetworkEventType; }
	virtual void				setNetworkEventType( ENetworkEventType eStateType )			{ m_eNetworkEventType = eStateType; }

	bool						checkForAbortOrShutdown( void );

	//=== vars ===//
	P2PEngine&					m_Engine;
	PktAnnounce&				m_PktAnn;
	NetServicesMgr&				m_NetServicesMgr;
	NetworkStateMachine&		m_NetworkStateMachine;
	ENetworkEventType			m_eNetworkEventType;
};


