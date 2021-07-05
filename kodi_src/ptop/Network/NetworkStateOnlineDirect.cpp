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

#include "NetworkStateOnlineDirect.h"
#include "NetworkStateMachine.h"
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/HostMgr/HostDb.h>
#include <GoTvCore/GoTvP2P/HostMgr/HostList.h>

//============================================================================
NetworkStateOnlineDirect::NetworkStateOnlineDirect( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
{
	setNetworkStateType( eNetworkStateTypeOnlineDirect );
}

//============================================================================
void NetworkStateOnlineDirect::enterNetworkState( void )
{
	// force update of ourself in anchor db in case we are being used as anchor
	HostList			anchorListIn;
	anchorListIn.addEntry( &m_Engine.getMyPktAnnounce() );
	HostList			anchorListOut;

	m_Engine.getNetServicesMgr().getNetServiceHost().getHostDb().handleAnnounce( anchorListIn, anchorListOut );

	m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeOnlineDirect );
}

//============================================================================
void NetworkStateOnlineDirect::runNetworkState( void )
{
	//int loopCnt = 0;
	while( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		if( m_PktAnn.requiresRelay() 
			&& ( false == m_NetworkStateMachine.isRelayServiceConnected() ) )
		{
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeRelaySearch );
			return;
		}
		//else
		//{
		//	loopCnt++;
		//	if( loopCnt >= ((1000 * 60 * 10) / 2000) )
		//	{
		//		// every 10 minutes refresh UPNP
		//		loopCnt = 0;
		//		m_NetworkStateMachine.startUpnpOpenPort();
		//	}
		//}

		VxSleep( 2000 );
	}
}



