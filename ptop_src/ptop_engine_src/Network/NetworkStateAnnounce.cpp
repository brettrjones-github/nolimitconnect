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

#include "NetworkStateAnnounce.h"
#include "NetworkStateMachine.h"

#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
NetworkStateAnnounce::NetworkStateAnnounce( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
, m_ConnectionList( stateMachine.getEngine().getConnectList() )
, m_DirectConnectTester( stateMachine.getDirectConnectTester() )
{
	setNetworkStateType( eNetworkStateTypeAnnounce );
}

//============================================================================
void NetworkStateAnnounce::runNetworkState( void )
{
	//LogMsg( LOG_INFO, "111 NetworkStateAnnounce::runNetworkState start\n" ); 
	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	if( m_PktAnn.requiresRelay() )
	{
		// try one more time to see if port is open
        EFirewallTestType firewallTestType = m_Engine.getEngineSettings().getFirewallTestSetting();
		if( eFirewallTestUrlConnectionTest == firewallTestType )
		{
			//LogMsg( LOG_INFO, "111 NetworkStateRelaySearch Starting Direct connect Test\n" );
			DirectConnectTestResults& directConnectTestResults = m_DirectConnectTester.getDirectConnectTestResults();
			m_DirectConnectTester.testCanDirectConnect();
			while( ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
				&& ( false == m_NetworkStateMachine.getDirectConnectTester().isDirectConnectTestComplete() ) )
			{
				VxSleep( 250 );
			}

			if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
			{
				return;
			}

			if( directConnectTestResults.getCanDirectConnect() )
			{
				LogMsg( LOG_INFO, "NetworkStateAnnounce Can Direct Connect.. switching to announce\n" );
				m_ConnectionList.cancelRelayService();
				m_PktAnn.setRequiresRelay( false );
				m_PktAnn.setHasRelay( false );			
			}
			else
			{
				LogMsg( LOG_INFO, "NetworkStateAnnounce Cannot Direct Connect.. announcing with relay\n" );
			}
		}
	}

	m_NetServicesMgr.announceToHost( m_NetworkStateMachine.getHostIp(), m_NetworkStateMachine.getHostPort() );

	if( m_PktAnn.requiresRelay() )
	{
		m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeOnlineThroughRelay );
	}
	else
	{
		m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeOnlineDirect );
	}

	//LogMsg( LOG_INFO, "111 NetworkStateAnnounce::runNetworkState done\n" ); 
}



