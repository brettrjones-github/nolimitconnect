#pragma once
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

#include <NetLib/VxSktUdp.h>

class IMulticastListenCallback
{
public:
	virtual void				multicastPktAnnounceAvail( VxSktBase* skt, PktAnnounce* pktAnnounce ) = 0;
};

class NetworkMgr;
class P2PEngine;

class RcMulticast
{
public:
	// const char*			RC_DEFAULT_MULTICAST_ADDR = "45.123.45.123";
	const char*			RC_DEFAULT_MULTICAST_ADDR = "226.1.1.1";
	const uint16_t		RC_DEFAULT_MULTICAST_PORT = 45123;

	RcMulticast( NetworkMgr& networkMgr, IMulticastListenCallback& multicastListenCallback );
	virtual ~RcMulticast();

	VxSktUdp&					getUdpSkt( void )							{ return m_SktUdp; }
	void						setLocalIp( InetAddress& newLocalIp );

	void						setMulticastKey( const char * networkName );

	void						setMulticastPort( uint16_t multicastPort ) { m_MulticastPort = multicastPort; }
	uint16_t					getMulticastPort( void ) { return m_MulticastPort; }
	void 						setMulticastGroupIp( std::string groupIpAddr ) { m_MulticastGroupIp = groupIpAddr; }
	std::string 				getMulticastGroupIp( void ) { return m_MulticastGroupIp; }

	bool 						enableListen( bool enable );
	void						doUdpDataCallback( VxSktBase* skt );

	// bool 						setBroadcastEnable( bool enable );
	void						onOncePerSecond( void );
	virtual void				onPktAnnUpdated( void );

	bool 						setBroadcastEnable( bool enable );


protected:
	void						sendMulticast( void );

	void						attemptDecodePktAnnounce( VxSktBase* skt, unsigned char* data, int dataLen );

	bool						m_ListenEnabled{ false };
	IMulticastListenCallback&	m_ListenCallback;

	bool						m_bBroadcastEnabled{ false };
	int							m_iBroadcastCountSec{ 0 };
	bool						m_bPktAnnUpdated{ false };
	PktAnnounce					m_PktAnnEncrypted;

	NetworkMgr&					m_NetworkMgr;
	P2PEngine&					m_Engine;
	VxSktUdp					m_SktUdp;
	InetAddress					m_LclIp;
	uint16_t					m_MulticastPort{ RC_DEFAULT_MULTICAST_PORT };
	std::string					m_MulticastGroupIp{ RC_DEFAULT_MULTICAST_ADDR };
	VxMutex						m_MulticastMutex;
};


