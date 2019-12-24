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

#include "NetworkEventAvail.h"
#include "NetworkStateMachine.h"
#include "GoTvDebugConfig.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <NetLib/VxPeerMgr.h>
#include <PktLib/PktAnnounce.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
NetworkEventAvail::NetworkEventAvail( NetworkStateMachine& stateMachine, const char * lclIp, bool isCellularNetwork )
: NetworkEventBase( stateMachine )
{
	setNetworkEventType( eNetworkEventTypeAvail );
	if( lclIp )
	{
		m_LclIp = lclIp;
	}
	else
	{
		m_LclIp = "";
	}

	VxSetLclIpAddress( m_LclIp.c_str() );
	m_bIsCellNetwork = isCellularNetwork;
}

//============================================================================
void NetworkEventAvail::runNetworkEvent( void )
{
	LogModule( eLogModuleNetworkState, LOG_VERBOSE, "NetworkEventAvail::runNetworkEvent start\n" );
	m_NetworkStateMachine.resolveWebsiteUrls();
    if( !m_LclIp.empty() )
    {
        uint16_t listenPort = m_Engine.getEngineSettings().getTcpIpPort();
        if( !m_Engine.getPeerMgr().isListening() || ( listenPort != m_Engine.getPeerMgr().getListenPort() ) )
        {
            if( !m_Engine.getPeerMgr().isListening() )
            {
                m_Engine.getPeerMgr().stopListening();
            }

            m_Engine.getPeerMgr().startListening( m_LclIp.c_str(), m_Engine.getEngineSettings().getTcpIpPort() );
        }
    }
	//m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeAvail );

	//m_PktAnn.getLanIPv4().setIp( m_LclIp.c_str() );
	//uint16_t u16TcpPort;
	//m_Engine.getEngineSettings().getTcpIpPort();
	//m_PktAnn.setOnlinePort( u16TcpPort );
	//m_Engine.getToGui().toGuiUpdateMyIdent( &m_PktAnn );

    LogModule( eLogModuleNetworkState, LOG_VERBOSE, "NetworkEventAvail::runNetworkEvent done\n" );
}


