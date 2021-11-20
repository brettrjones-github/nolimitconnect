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

#include "NetworkStateGetRelayList.h"
#include "NetworkStateMachine.h"

#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h> 

//============================================================================
NetworkStateGetRelayList::NetworkStateGetRelayList( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
{
	setNetworkStateType( eNetworkStateTypeGetRelayList );
}

//============================================================================
void NetworkStateGetRelayList::enterNetworkState( void )
{
	m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeGetRelayList );
}

//============================================================================
void NetworkStateGetRelayList::runNetworkState( void )
{
	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	LogModule( eLogNetworkState, LOG_DEBUG, "NetworkStateGetRelayList anchor ip %s", m_NetworkStateMachine.getHostIp().c_str() );
	if( 0 == m_NetworkStateMachine.getHostIp().length() )
	{
#ifdef DEBUG_PTOP_NETWORK_STATE
        AppErr( eAppErrFailedConnectHost, "Failed connect to anchor. Please check network settings"  );
#endif // DEBUG_PTOP_NETWORK_STATE
		m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeLost);
		return;
	}

	m_NetServicesMgr.announceToHost( m_NetworkStateMachine.getHostIp(), m_NetworkStateMachine.getHostPort(), eHostActionRelaysOnly );

	m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeRelaySearch );
}



