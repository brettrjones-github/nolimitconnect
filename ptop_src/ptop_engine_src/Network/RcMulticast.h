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
// http://www.nolimitconnect.org
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
	const char*			RC_DEFAULT_MULTICAST_ADDR = "226.1.2.3";
	const uint16_t		RC_DEFAULT_MULTICAST_PORT = 45123;

	RcMulticast( NetworkMgr& networkMgr, IMulticastListenCallback& multicastListenCallback );
	virtual ~RcMulticast();

	VxSktUdp&					getUdpSkt( void )							{ return m_SktUdp; }
	void						setLocalIp( InetAddress& newLocalIp );

	void						setMulticastKey( std::string networkNameKey );

	void						setMulticastPort( uint16_t multicastPort )		{ m_MulticastPort = multicastPort; }
	uint16_t					getMulticastPort( void )						{ return m_MulticastPort; }
	void 						setMulticastGroupIp( std::string groupIpAddr )	{ m_MulticastGroupIp = groupIpAddr; }
	std::string 				getMulticastGroupIp( void )						{ return m_MulticastGroupIp; }

	bool 						multicastEnable( bool enable );

	virtual void				doUdpDataCallback( VxSktBase* skt );

	virtual void				onPktAnnUpdated( void );
	virtual void				onOncePerSecond( void );

protected:
	void						sendMulticast( void );

	void						attemptDecodePktAnnounce( VxSktBase* skt, unsigned char* data, int dataLen );

	NetworkMgr&					m_NetworkMgr;
	P2PEngine&					m_Engine;
	VxMutex						m_MulticastMutex;
	VxCrypto					m_RxCrypto;			            // encryption object for receive
	VxCrypto					m_TxCrypto;			            // encryption object for transmit
	std::string					m_NetworkNameKey{ "" };

	IMulticastListenCallback&	m_ListenCallback;
    VxSktUdp					m_SktUdp;

	bool						m_BroadcastEnabled{ false };
	int							m_iBroadcastCountSec{ 0 };
	bool						m_bPktAnnUpdated{ false };
	PktAnnounce					m_PktAnnEncrypted;

	InetAddress					m_LclIp;
	uint16_t					m_MulticastPort{ RC_DEFAULT_MULTICAST_PORT };
	std::string					m_MulticastGroupIp{ RC_DEFAULT_MULTICAST_ADDR };
};


