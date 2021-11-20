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

#include "RcMulticastBroadcast.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <memory.h>

namespace
{
	const int MULTICAST_BROADCAST_INTERVAL_SEC = 5;
}

//============================================================================
RcMulticastBroadcast::RcMulticastBroadcast( NetworkMgr& networkMgr )
: RcMulticastBase( networkMgr )
{
}

//============================================================================
RcMulticastBroadcast::~RcMulticastBroadcast()
{
	m_SktUdp.closeSkt( eSktCloseConnectReasonsEmpty, false );
}

//============================================================================
void RcMulticastBroadcast::setBroadcastEnable( bool enable )
{
	if( enable != m_bBroadcastEnabled )
	{
		if( enable )
		{
			RCODE rc = m_SktUdp.udpOpen( m_LclIp, m_u16MulticastPort, false );
			if( 0 == rc )
			{
				m_bBroadcastEnabled = enable;
			}
			else
			{
				LogModule( eLogMulticast, LOG_ERROR, "RcMulticastBroadcast::setBroadcastEnable failed %d", rc );
			}
		}
		else
		{
			m_SktUdp.closeSkt( eSktCloseConnectReasonsEmpty, false );
		}
	}
}

//============================================================================
void RcMulticastBroadcast::onPktAnnUpdated( void )
{
	if( m_Engine.getNetStatusAccum().getNearbyAvailable() && !m_Engine.getNetStatusAccum().getLanIpAddr().empty() )
	{
		m_MulticastMutex.lock();
		m_Engine.copyMyPktAnnounce( m_PktAnnEncrypted );
		m_PktAnnEncrypted.setMyFriendshipToHim( eFriendStateGuest );
		m_PktAnnEncrypted.setHisFriendshipToMe( eFriendStateGuest );
		// normally it is a bad idea to announce our lan ip but do in multicast so can connect on local network
		InetAddrIPv4 ipV4;
		ipV4.setIp( m_Engine.getNetStatusAccum().getLanIpAddr().c_str() );
		m_PktAnnEncrypted.setLanIPv4( ipV4 );

		std::string networkName;
		m_Engine.getEngineSettings().getNetworkKey( networkName );

		if( networkName.length() )
		{
			setMulticastKey( networkName.c_str() );
			RCODE rc = m_SktUdp.m_TxCrypto.encrypt( ( unsigned char* )&m_PktAnnEncrypted, ( int )sizeof( PktAnnounce ) );
			if( 0 != rc )
			{
				LogMsg( LOG_INFO, "RcMulticastBroadcast::onPktAnnUpdated error %d encrypting pkt announce\n", rc );
			}
			else
			{
				m_bPktAnnUpdated = true;
			}
		}
		else
		{
			LogMsg( LOG_INFO, "RcMulticastBroadcast::onPktAnnUpdated COULD NOT GET NETWORK NAME\n" );
		}

		m_MulticastMutex.unlock();
	}
}

//============================================================================
void RcMulticastBroadcast::onOncePerSecond( void )
{
	if( m_bBroadcastEnabled && m_bPktAnnUpdated && m_Engine.getNetStatusAccum().getNearbyAvailable() )
	{
		m_iBroadcastCountSec++;
		if( m_iBroadcastCountSec >= MULTICAST_BROADCAST_INTERVAL_SEC )
		{
			m_iBroadcastCountSec = 0;
			sendMulticast();
		}
	}
}

//============================================================================
void RcMulticastBroadcast::sendMulticast( void )
{
	if( false == m_bPktAnnUpdated )
	{
		LogMsg( LOG_INFO, "RcMulticastBroadcast::sendMulticast PktAnn HAS NOT BEEN UPDATED" );
		return;
	}
	
	LogModule( eLogMulticast, LOG_INFO, "RcMulticastBroadcast::sendMulticast PktAnn len %d", sizeof( m_PktAnnEncrypted ) );
	m_MulticastMutex.lock();
	RCODE rc = getUdpSkt().sendToMulticast( (const char *)&m_PktAnnEncrypted, sizeof( m_PktAnnEncrypted ), m_strMulticastIp.c_str(), m_u16MulticastPort );
	if( rc )
	{
		LogMsg( LOG_INFO, "RcMulticastBroadcast::sendMulticast error %d", rc );
	}

	m_MulticastMutex.unlock();
}
