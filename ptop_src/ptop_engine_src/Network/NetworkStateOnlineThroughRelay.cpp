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

#include "NetworkStateOnlineThroughRelay.h"
#include "NetworkStateMachine.h"
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
NetworkStateOnlineThroughRelay::NetworkStateOnlineThroughRelay( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
{
	setNetworkStateType( eNetworkStateTypeOnlineThroughRelay );
}

//============================================================================
void NetworkStateOnlineThroughRelay::enterNetworkState( void )
{
	m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeOnlineThroughRelay );
}

//============================================================================
void NetworkStateOnlineThroughRelay::runNetworkState( void )
{
	while( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		if( m_PktAnn.requiresRelay() 
			&& ( false == m_NetworkStateMachine.isRelayServiceConnected() ) )
		{
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeWaitForRelay );
			return;
		}

		VxSleep( 2000 );
	}
}



