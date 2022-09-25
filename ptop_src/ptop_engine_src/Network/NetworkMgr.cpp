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

#include "NetworkMgr.h"
#include "NetworkDefs.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PConnectList.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListMgr.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetworkMonitor.h>

#include <NetLib/VxSktBase.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktConnect.h>
#include <NetLib/VxSktCrypto.h>

#include <PktLib/VxCommon.h>
#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsRelay.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

namespace
{
	void NetworkPeerSktCallbackHandler( VxSktBase *  sktBase, void * pvUserCallbackData )
	{
        if( pvUserCallbackData )
		{
            NetworkMgr * netMgr = (NetworkMgr *)pvUserCallbackData;
            netMgr->handleTcpSktCallback( sktBase );
		}
	}
}

//============================================================================
NetworkMgr::NetworkMgr( P2PEngine&		engine, 
						VxPeerMgr&		peerMgr,
						BigListMgr&		bigListMgr,
						P2PConnectList&	connectionList
						)
: m_Engine( engine )
, m_PktAnn( engine.getMyPktAnnounce() )
, m_PeerMgr( peerMgr )
, m_BigListMgr( bigListMgr )
, m_ConnectList( connectionList )
, m_NearbyMgr( engine, *this )
{
	VxSetNetworkLoopbackAllowed( false );

	m_PeerMgr.setReceiveCallback( NetworkPeerSktCallbackHandler, this );
}

//============================================================================
void NetworkMgr::networkMgrStartup( void )
{
	m_NearbyMgr.networkMgrStartup();
}

//============================================================================
void NetworkMgr::networkMgrShutdown( void )
{
	m_NearbyMgr.networkMgrShutdown();
	m_PeerMgr.sktMgrShutdown();
}

//============================================================================
void NetworkMgr::fromGuiNetworkAvailable( const char * lclIp, bool isCellularNetwork )
{
    if( !lclIp )
    {
        LogMsg( LOG_SEVERE, "fromGuiNetworkAvailable invalid param lclIp is null" );
        return;
    }

    std::string strIp = lclIp;
    if( strIp.empty() )
    {
        LogMsg( LOG_ERROR, "fromGuiNetworkAvailable param lclIp is empty" );
        return;
    }

    if( m_bNetworkAvailable 
        && ( m_strLocalIpAddr == strIp )
        && ( m_bIsCellularNetwork == isCellularNetwork ) )
    {
        LogModule( eLogNetworkMgr, LOG_DEBUG, "fromGuiNetworkAvailable but network already set to %s\n", m_strLocalIpAddr.c_str() );
        return;
    }

	m_bIsCellularNetwork = isCellularNetwork;
	m_strLocalIpAddr = lclIp;
	m_LocalIp.setIp( lclIp );
    m_bNetworkAvailable = true;

	m_PeerMgr.setLocalIp( m_LocalIp );

	if( m_LocalIp.isIPv4() && m_LocalIp.isValid() )
	{
		m_Engine.getMyPktAnnounce().getLanIPv4().setIp( lclIp );
	}
	else
	{
		m_Engine.getMyPktAnnounce().getLanIPv4().setToInvalid();
	}

	m_NearbyMgr.fromGuiNetworkAvailable( lclIp, isCellularNetwork );
}

//============================================================================
void NetworkMgr::fromGuiNetworkLost( void )
{
	m_bNetworkAvailable =  false ;
	m_NearbyMgr.fromGuiNetworkLost();;
}

//============================================================================
ENetLayerState NetworkMgr::fromGuiGetNetLayerState( ENetLayerType netLayer )
{
    ENetLayerState netState = eNetLayerStateWrongType;
    if( netLayer == eNetLayerTypeInternet )
    {
        NetworkMonitor&	netMonitor = m_Engine.getNetworkMonitor();

        if( !netMonitor.getIsInitialized() )
        {
            netState = eNetLayerStateUndefined;
        }
        else
        {
            netState = netMonitor.getIsInternetAvailable() ? eNetLayerStateAvailable : eNetLayerStateFailed;
        }
    }

    return netState;
}

//============================================================================
void NetworkMgr::onPktAnnUpdated( void )
{
	m_NearbyMgr.onPktAnnUpdated();
}

//============================================================================
void NetworkMgr::onOncePerSecond( void )
{
	if ( VxIsAppShuttingDown() )
	{
		return;
	}
	
	m_NearbyMgr.onOncePerSecond();
}

//============================================================================
void NetworkMgr::handleTcpSktCallback( VxSktBase* sktBase )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	sktBase->setLastActiveTimeMs( GetGmtTimeMs() );

	switch( sktBase->getCallbackReason() )
	{
	case eSktCallbackReasonConnectError:
        LogModule( eLogNetworkMgr, LOG_ERROR, "NetworkMgr:TCP skt %d skt id %d connect error %s thread 0x%x",
                    sktBase->getSktHandle(), sktBase->getSktNumber(), sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
		break;

	case eSktCallbackReasonConnected:
        LogModule( eLogNetworkMgr, LOG_INFO, "NetworkMgr:TCP skt %d skt id %d %s port %d to local port %d thread 0x%x",
                    sktBase->getSktHandle(), sktBase->getSktNumber(), sktBase->describeSktType().c_str(), sktBase->m_RmtIp.getPort(), sktBase->m_LclIp.getPort(), VxGetCurrentThreadId() );
		break;

	case eSktCallbackReasonData:
		m_Engine.handleTcpData( sktBase );
		break;

	case eSktCallbackReasonClosed:
		m_Engine.onConnectionLost( sktBase );
        LogModule( eLogNetworkMgr, LOG_INFO, "NetworkMgr:TCP skt %d skt id %d closed %s thread 0x%x",
                    sktBase->getSktHandle(), sktBase->getSktNumber(), sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
		break;

	case eSktCallbackReasonError:
 		LogModule( eLogNetworkMgr, LOG_ERROR, "NetworkMgr:TCP skt %d skt id %d error %s thread 0x%x",
                    sktBase->getSktHandle(), sktBase->getSktNumber(), sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
		break;

	case eSktCallbackReasonClosing:
		m_Engine.onConnectionLost( sktBase );
        LogModule( eLogNetworkMgr, LOG_INFO, "NetworkMgr:TCP eSktCallbackReasonClosing skt %d skt id %d thread 0x%x", sktBase->getSktHandle(), sktBase->getSktNumber(), VxGetCurrentThreadId() );
		break;

	case eSktCallbackReasonConnecting:
        LogModule( eLogNetworkMgr, LOG_INFO, "NetworkMgr:TCP eSktCallbackReasonConnecting skt %d skt id %d thread 0x%x", sktBase->getSktHandle(), sktBase->getSktNumber(), VxGetCurrentThreadId() );
		break;

	default:
		LogMsg( LOG_ERROR, "NetworkMgrTCP: UNKNOWN CallbackReason %d skt %d skt id %d error %s thread 0x%x", 
                sktBase->getCallbackReason(), sktBase->getSktHandle(), sktBase->getSktNumber(), sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
		break;
	}
}
