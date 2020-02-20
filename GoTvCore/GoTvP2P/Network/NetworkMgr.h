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

#include "NetworkDefs.h"
#include "RcMulticastBroadcast.h"
#include "RcMulticastListen.h"

#include <CoreLib/VxDefs.h>
#include <PktLib/VxFriendMatch.h>

#include <GoTvInterface/IDefs.h>

class P2PEngine;
class VxPeerMgr;
class BigListMgr;
class P2PConnectList;
class VxNetIdentBase;
class VxSktBase;
class VxGUID;
class InetAddress;
class PktAnnounce;

class NetworkMgr : public IMulticastListenCallback
{
public:
	NetworkMgr( P2PEngine&		engine, 
				VxPeerMgr&		peerMgr,
				BigListMgr&		bigListMgr,
				P2PConnectList&	connectionList );
	virtual ~NetworkMgr() = default;

	P2PEngine&					getEngine( void )											{ return m_Engine; }
	VxPeerMgr&					getPeerMgr( void )											{ return m_PeerMgr; }
	std::string					getLocalIpAddress( void )									{ return m_strLocalIpAddr; }
	void						setNetworkKey( const char * networkName )					{ m_NetworkName = networkName; }
	const char *				getNetworkKey( void )										{ return m_NetworkName.c_str(); }

	void						networkMgrStartup( void );
	void						networkMgrShutdown( void );

	virtual void				fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork = false );
	virtual void				fromGuiNetworkLost( void );
    virtual ENetLayerState	    fromGuiGetNetLayerState( ENetLayerType netLayer = eNetLayerTypeInternet );

    virtual bool				isInternetAvailable( void )									{ return eNetLayerStateAvailable == fromGuiGetNetLayerState( eNetLayerTypeInternet ); }

	virtual bool				isNetworkAvailable( void )									{ return m_bNetworkAvailable; }
	virtual bool				isCellularNetwork( void )									{ return m_bIsCellularNetwork; }

	void						setBroadcastPort( uint16_t u16Port );
	void						setBroadcastEnable( bool enable );

	virtual	void				multicastPktAnnounceAvail( VxSktBase * skt, PktAnnounce * pktAnnounce );

	virtual void				onPktAnnUpdated( void );
	virtual void				onOncePerSecond( void );

	virtual	void				handleTcpSktCallback( VxSktBase * sktBase );
	virtual	void				handleMulticastSktCallback( VxSktBase * sktBase );

protected:
	P2PEngine&					m_Engine;
    PktAnnounce&				m_PktAnn;
    VxPeerMgr&					m_PeerMgr;
	BigListMgr&					m_BigListMgr;
	P2PConnectList&				m_ConnectList;
#ifdef ENABLE_MULTICAST
	RcMulticastBroadcast		m_MulticastBroadcast;
	RcMulticastListen			m_MulticastListen;
#endif // ENABLE_MULTICAST
	std::string					m_NetworkName;

    bool						m_bNetworkAvailable{ false };
	bool						m_bIsCellularNetwork{ false };
	std::string					m_strLocalIpAddr;
	InetAddress					m_LocalIp;
};

