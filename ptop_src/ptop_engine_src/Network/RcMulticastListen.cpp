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

#include "RcMulticastListen.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktUtil.h>
#include <PktLib/PktAnnounce.h>
#include <CoreLib/VxGlobals.h>

namespace
{

	const int MULTICAST_BROADCAST_INTERVAL_SEC = 5;


	void RcMulticastListenSktCallbackHandler( VxSktBase *  sktBase, void * pvRxCallbackUserData )
	{
		if( eSktCallbackReasonData == sktBase->getCallbackReason() )
		{
			if( pvRxCallbackUserData )
			{
				((RcMulticastListen *)pvRxCallbackUserData)->doUdpDataCallback( sktBase );
			}
		}
	}
}

//============================================================================
RcMulticastListen::RcMulticastListen(  NetworkMgr& networkMgr, IMulticastListenCallback& multicastListenCallback )
: RcMulticastBase( networkMgr)
, m_ListenCallback( multicastListenCallback )
{
	m_SktUdp.setReceiveCallback( RcMulticastListenSktCallbackHandler, this );
}

//============================================================================
RcMulticastListen::~RcMulticastListen()
{
	m_SktUdp.closeSkt( eSktCloseSktDestroy );
}

//============================================================================
bool RcMulticastListen::enableListen( bool enable )
{
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	bool result = false;
	if( enable != m_ListenEnabled || enable && !m_SktUdp.isConnected() )
	{
		if( enable )
		{
			RCODE rc = m_SktUdp.udpOpenMulticast( m_MulticastGroupIp, m_MulticastPort, true );
			if( 0 == rc )
			{

				LogModule( eLogMulticast, LOG_VERBOSE, "RcMulticastListen::enableListen success %s %d", m_MulticastGroupIp.c_str(), m_MulticastPort );
				result = true;
				m_ListenEnabled = true;
				m_bBroadcastEnabled = true;
				m_SktUdp.setIsConnected( true );
			}
			else
			{
				LogModule( eLogMulticast, LOG_ERROR, "RcMulticastListen::enableListen failed error %d %d", rc, VxDescribeSktError( rc ) );
				m_SktUdp.setIsConnected( false );
			}
		}
		else
		{
			m_SktUdp.closeSkt( eSktCloseMulticastListenDone );
			result = true;
			m_ListenEnabled = false;
			m_bBroadcastEnabled = false;
			m_SktUdp.setIsConnected( false );
		}
	}
	else
	{
		result = true;
	}

	return result;
}

//============================================================================
void RcMulticastListen::doUdpDataCallback( VxSktBase * skt )
{
	if( !VxIsAppShuttingDown() )
	{
		unsigned char* data = skt->getSktReadBuf();
		int dataLen = skt->getSktBufDataLen();

		attemptDecodePktAnnounce( skt, data, dataLen );

		skt->sktBufAmountRead( dataLen );
	}
}

//============================================================================
void RcMulticastListen::attemptDecodePktAnnounce( VxSktBase * skt, unsigned char * data, int dataLen )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( dataLen != sizeof( PktAnnounce ) )
	{
		return;
	}

	skt->decryptReceiveData();

	PktAnnounce * pktAnnounce = (PktAnnounce *)data;
	if( false == pktAnnounce->isValidPktAnn() )
	{
		LogMsg( LOG_INFO, "attemptDecodePktAnnounce invalid PktAnn" );
		return;
	}

	m_ListenCallback.multicastPktAnnounceAvail( skt, pktAnnounce );
}


//============================================================================
void RcMulticastListen::onPktAnnUpdated( void )
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
			m_SktUdp.m_TxCrypto.setPassword( networkName.c_str(), networkName.length() );
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
void RcMulticastListen::onOncePerSecond( void )
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
void RcMulticastListen::sendMulticast( void )
{
	if( false == m_bPktAnnUpdated )
	{
		LogMsg( LOG_INFO, "RcMulticastBroadcast::sendMulticast PktAnn HAS NOT BEEN UPDATED" );
		return;
	}

	if( m_bPktAnnUpdated && m_bBroadcastEnabled )
	{
		getUdpSkt().setIsConnected( true );
	}

	if( getUdpSkt().isConnected() )
	{
		LogModule( eLogMulticast, LOG_INFO, "RcMulticastBroadcast::sendMulticast PktAnn len %d", sizeof( m_PktAnnEncrypted ) );
		m_MulticastMutex.lock();
		RCODE rc = getUdpSkt().sendToMulticast( ( const char* )&m_PktAnnEncrypted, sizeof( m_PktAnnEncrypted ), m_MulticastGroupIp.c_str(), getUdpSkt().getMulticastPort() );
		if( rc )
		{
			LogMsg( LOG_INFO, "RcMulticastBroadcast::sendMulticast error %d", rc );
		}

		m_MulticastMutex.unlock();
	}
	else
	{
		LogMsg( LOG_INFO, "RcMulticastBroadcast::sendMulticast socket not open" );

	}
}
