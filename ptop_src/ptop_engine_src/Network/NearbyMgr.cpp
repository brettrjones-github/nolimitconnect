//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "NearbyMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

namespace
{
    void NetworkMulticastCallbackHandler( VxSktBase *  sktBase, void * pvUserCallbackData )
    {
    	if( pvUserCallbackData )
    	{
    		(( NearbyMgr*)pvUserCallbackData)->handleMulticastSktCallback( sktBase );
    	}
    }
}

//============================================================================
NearbyMgr::NearbyMgr( P2PEngine& engine, NetworkMgr& networkMgr )
    : RcMulticast( networkMgr, *this )
{
    getUdpSkt().setReceiveCallback( NetworkMulticastCallbackHandler, this );
}

//============================================================================
void NearbyMgr::networkMgrStartup( void )
{
    m_Engine.getNetStatusAccum().addNetStatusCallback( this );
}

//============================================================================
void NearbyMgr::networkMgrShutdown( void )
{
    m_Engine.getNetStatusAccum().removeNetStatusCallback( this );
    setBroadcastEnable( false );
    getUdpSkt().closeSkt( eSktCloseSktDestroy, false );
}

//============================================================================
bool NearbyMgr::fromGuiNearbyBroadcastEnable( bool enable )
{
    bool result = false;
    if( enable )
    {
        uint16_t u16MulticastPort;
        m_Engine.getEngineSettings().getMulticastPort( u16MulticastPort );
        if( u16MulticastPort )
        {
            setBroadcastPort( u16MulticastPort );
            setBroadcastEnable( enable );
            LogModule( eLogMulticast, LOG_INFO, "fromGuiNearbyBroadcastEnable enabled on port %d", enable, u16MulticastPort );
            result = true;
        }
        else
        {
            LogMsg( LOG_ERROR, "fromGuiNearbyBroadcastEnable enabled %d INVALID PORT", enable );
        }
    }
    else
    {
        setBroadcastEnable( enable );
        LogMsg( LOG_INFO, "fromGuiNearbyBroadcastEnable disabled" );
        result = true;
    }

    return result;
}

//============================================================================
void NearbyMgr::fromGuiNetworkAvailable( const char* lclIp, bool isCellularNetwork )
{
    //#ifdef ENABLE_MULTICAST // do not specify local address .. does not work with vpn
    //	m_MulticastListen.setLocalIp( m_LocalIp );
    //	m_MulticastBroadcast.setLocalIp( m_LocalIp );
    //#endif // ENABLE_MULTICAST

    // m_MulticastListen.beginListen();
}

//============================================================================
void NearbyMgr::fromGuiNetworkLost( void )
{
    // m_MulticastListen.stopListen();
}

//============================================================================
bool NearbyMgr::isNearby( VxGUID& onlineId )
{
    bool isNearbyd = false;
    lockList();
    for( auto iter = m_NearbyIdentList.begin(); iter != m_NearbyIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            isNearbyd = true;
            break;
        }
    }

    unlockList();
    return isNearbyd;
}

//============================================================================
void NearbyMgr::updateIdent( VxGUID& onlineId, int64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "NearbyMgr::updateNearbyIdent invalid id" );
        return;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "NearbyMgr::updateNearbyIdent cannot ignore myself" );
        return;
    }

    bool wasInserted = false;
    bool wasErased = false;
    lockList();
    for( auto iter = m_NearbyIdentList.begin(); iter != m_NearbyIdentList.end(); )
    {
        if( iter->first == onlineId )
        {
            iter = m_NearbyIdentList.erase( iter );
            wasErased = true;
        }
        else if( !wasInserted )
        {
            if( timestamp > iter->second )
            {
                iter = m_NearbyIdentList.insert( iter, std::make_pair( onlineId, timestamp ) );
                wasInserted = true;
            }
            else
            {
                ++iter;
            }
        }
        else
        {
            ++iter;
        }

        if( wasErased && wasInserted )
        {
            break;
        }
    }

    if( !wasInserted )
    {
        m_NearbyIdentList.push_back( std::make_pair( onlineId, timestamp ) );
    }

    unlockList();
}

//============================================================================
void NearbyMgr::removeIdent( VxGUID& onlineId )
{
    lockList();
    for( auto iter = m_NearbyIdentList.begin(); iter != m_NearbyIdentList.end(); ++iter )
    {
        if( iter->first == onlineId )
        {
            m_NearbyIdentList.erase( iter );
            break;
        }
    }

    unlockList();
}

//============================================================================
void NearbyMgr::setBroadcastPort( uint16_t u16Port )
{
    setMulticastPort( u16Port );
}

//============================================================================
bool NearbyMgr::setBroadcastEnable( bool enable )
{
    bool result = enableListen( enable );
    if( result )
    {
        m_bBroadcastEnabled = true;
    }
    else
    {
        LogModule( eLogMulticast, LOG_ERROR, "setBroadcastEnable %d failed", enable);
        m_bBroadcastEnabled = false;
    }

    return result;
}

//============================================================================
void NearbyMgr::handleMulticastSktCallback( VxSktBase* sktBase )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    switch( sktBase->getCallbackReason() )
    {
    case eSktCallbackReasonConnectError:
        LogModule( eLogMulticast, LOG_ERROR, "NetworkMgr:Multicast Skt %d connect error %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonConnected:
        LogModule( eLogMulticast, LOG_INFO, "NetworkMgr:Multicast Skt %d connected from %s port %d thread 0x%x", sktBase->m_iSktId, sktBase->getRemoteIp().c_str(), sktBase->m_LclIp.getPort(), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonData:
        doUdpDataCallback( sktBase );
        break;

    case eSktCallbackReasonClosed:
        LogModule( eLogMulticast, LOG_INFO, "NetworkMgr:Multicast Skt %d closed %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonError:
        LogModule( eLogMulticast, LOG_ERROR, "NetworkMgr:Multicast Skt %d error %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonClosing:
        LogModule( eLogMulticast, LOG_INFO, "NetworkMgr:Multicast eSktCallbackReasonClosing Skt %d thread 0x%x", sktBase->m_iSktId, VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonConnecting:
        LogModule( eLogMulticast, LOG_INFO, "NetworkMgr:Multicast eSktCallbackReasonConnecting Skt %d thread 0x%x", sktBase->m_iSktId, VxGetCurrentThreadId() );
        break;

    default:
        LogMsg( LOG_ERROR, "NetworkMgr:Multicast: Skt %d error %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;
    }
}

//============================================================================
void NearbyMgr::multicastPktAnnounceAvail( VxSktBase* skt, PktAnnounce* pktAnnounce )
{
    //BRJ TODO handle multicast
    LogMsg( LOG_DEBUG, "NearbyMgr::multicastPktAnnounceAvail" );
}

//============================================================================
void NearbyMgr::callbackInternetStatusChanged( EInternetStatus internetStatus )
{
}

//============================================================================
void NearbyMgr::callbackNetAvailStatusChanged( ENetAvailStatus netAvalilStatus )
{
    if( eNetAvailP2PAvail <= netAvalilStatus )
    { 
        if( VxIsAppShuttingDown() )
        {
            return;
        }

        onPktAnnUpdated();
    }
}
