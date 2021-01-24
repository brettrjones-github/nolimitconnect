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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <memory.h>

namespace
{
	const int MULTICAST_BROADCAST_INTERVAL_SEC = 5;
}

//============================================================================
RcMulticastBroadcast::RcMulticastBroadcast( NetworkMgr& networkMgr )
: RcMulticastBase( networkMgr )
, m_bBroadcastEnabled( true )
, m_iBroadcastCountSec( 0 )
, m_bPktAnnUpdated( false )
, m_bOnline( false )
{
}

//============================================================================
RcMulticastBroadcast::~RcMulticastBroadcast()
{
}

//============================================================================
void RcMulticastBroadcast::setBroadcastEnable( bool enable )
{
	m_bBroadcastEnabled = enable;
}

//============================================================================
void RcMulticastBroadcast::onPktAnnUpdated( void )
{
	m_MulticastMutex.lock();
	m_Engine.copyMyPktAnnounce(m_PktAnnEncrypted);
	m_PktAnnEncrypted.setMyFriendshipToHim(eFriendStateGuest);
	m_PktAnnEncrypted.setHisFriendshipToMe(eFriendStateGuest);

	std::string networkName;
	m_Engine.getEngineSettings().getNetworkKey( networkName );

	if( networkName.length() )
	{
		setMulticastKey( networkName.c_str() );
		RCODE rc =	m_SktUdp.m_TxCrypto.encrypt( (unsigned char *)&m_PktAnnEncrypted, (int)sizeof( PktAnnounce ) );
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

//============================================================================
void RcMulticastBroadcast::onOncePerSecond( void )
{
	if( m_bBroadcastEnabled && m_bOnline )
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
		//LogMsg( LOG_INFO, "RcMulticastBroadcast::sendMulticast PktAnn HAS NOT BEEN UPDATED\n" );
		return;
	}
	// TODO send
}

//============================================================================
void RcMulticastBroadcast::goOnline( bool online )
{
	m_bOnline = online;
}
