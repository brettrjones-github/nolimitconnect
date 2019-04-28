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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "NetworkStateGetRelayList.h"
#include "NetworkStateMachine.h"

#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h> 

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

#ifdef DEBUG_PTOP_NETWORK_STATE
    LogMsg( LOG_INFO, "NetworkStateGetRelayList anchor ip %s", m_NetworkStateMachine.getAnchorIp().c_str() );
#endif // DEBUG_PTOP_NETWORK_STATE
	if( 0 == m_NetworkStateMachine.getAnchorIp().length() )
	{
#ifdef DEBUG_PTOP_NETWORK_STATE
        AppErr( eAppErrFailedConnectAnchor, "Failed connect to anchor. Please check network settings"  );
#endif // DEBUG_PTOP_NETWORK_STATE
		m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeLost);
		return;
	}

	m_NetServicesMgr.announceToAnchor( m_NetworkStateMachine.getAnchorIp(), m_NetworkStateMachine.getAnchorPort(), eAnchorActionRelaysOnly );

	m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeRelaySearch );
}



