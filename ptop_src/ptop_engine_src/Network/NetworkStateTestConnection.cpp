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

#include "NetworkStateTestConnection.h"
#include "NetworkStateMachine.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
NetworkStateTestConnection::NetworkStateTestConnection( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
, m_DirectConnectTester( stateMachine.getDirectConnectTester() )
{
	setNetworkStateType( eNetworkStateTypeTestConnection );
}

//============================================================================
void NetworkStateTestConnection::enterNetworkState( void )
{
	m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeTestConnection );
}

//============================================================================
void NetworkStateTestConnection::runNetworkState( void )
{
	DirectConnectTestResults& testResults = m_DirectConnectTester.getDirectConnectTestResults();

	if( false == m_NetworkStateMachine.shouldAbort() )
	{
		m_DirectConnectTester.testCanDirectConnect();
		if( ( eNetCmdErrorNone != testResults.m_eNetCmdErr )
			|| ( false == testResults.getCanDirectConnect() ) )
		{
			m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( testResults.m_MyIpAddr, true );
			m_Engine.getMyPktAnnounce().setRequiresRelay( true );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeRelaySearch );
		}
		else
		{
			m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( testResults.m_MyIpAddr, testResults.getCanDirectConnect() ? false : true );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeAnnounce );
		}
	}
}




