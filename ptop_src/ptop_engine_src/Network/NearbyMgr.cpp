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
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

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
            result = setBroadcastEnable( enable );
            LogModule( eLogMulticast, LOG_INFO, "fromGuiNearbyBroadcastEnable enable %d port %d", enable, u16MulticastPort );
        }
        else
        {
            LogMsg( LOG_ERROR, "fromGuiNearbyBroadcastEnable enabled %d INVALID PORT", enable );
        }
    }
    else
    {
        result = setBroadcastEnable( enable );
        LogMsg( LOG_INFO, "fromGuiNearbyBroadcastEnable disabled" );
        m_MulticastIdentMutex.lock();
        m_MulticastIdentList.clear();
        m_MulticastIdentMutex.unlock();
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
        LogMsg( LOG_ERROR, "NearbyMgr::updateNearbyIdent cannot nearby myself" );
        // BRJ temprory disable myself check for testing
        // return;
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
    bool result = multicastEnable( enable );
    if( !result )
    {
        LogModule( eLogMulticast, LOG_ERROR, "setBroadcastEnable %d failed", enable);
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
        LogModule( eLogMulticast, LOG_VERBOSE, "NetworkMgr:Multicast Skt %d connected from %s port %d thread 0x%x", sktBase->m_iSktId, sktBase->getRemoteIp().c_str(), sktBase->m_LclIp.getPort(), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonData:
        doUdpDataCallback( sktBase );
        break;

    case eSktCallbackReasonClosed:
        LogModule( eLogMulticast, LOG_VERBOSE, "NetworkMgr:Multicast Skt %d closed %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonError:
        LogModule( eLogMulticast, LOG_ERROR, "NetworkMgr:Multicast Skt %d error %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonClosing:
        LogModule( eLogMulticast, LOG_VERBOSE, "NetworkMgr:Multicast eSktCallbackReasonClosing Skt %d thread 0x%x", sktBase->m_iSktId, VxGetCurrentThreadId() );
        break;

    case eSktCallbackReasonConnecting:
        LogModule( eLogMulticast, LOG_VERBOSE, "NetworkMgr:Multicast eSktCallbackReasonConnecting Skt %d thread 0x%x", sktBase->m_iSktId, VxGetCurrentThreadId() );
        break;

    default:
        LogMsg( LOG_ERROR, "NetworkMgr:Multicast: Skt %d error %s thread 0x%x", sktBase->m_iSktId, sktBase->describeSktError( sktBase->getLastSktError() ), VxGetCurrentThreadId() );
        break;
    }
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

//============================================================================
void NearbyMgr::multicastPktAnnounceAvail( VxSktBase* skt, PktAnnounce* pktAnnounce )
{
    LogModule( eLogMulticast, LOG_VERBOSE, "NearbyMgr::multicastPktAnnounceAvail" );
    if( !pktAnnounce->getMyOnlineId().isVxGUIDValid() )
    {
        LogModule( eLogMulticast, LOG_ERROR, "NearbyMgr multicast pktAnnounce invalid " );
    }

    if( pktAnnounce->getMyOnlineId() == m_Engine.getMyOnlineId() )
    {
        LogModule( eLogMulticast, LOG_VERBOSE, "NearbyMgr multicast pktAnnounce cannot nearby myself" );
        return;
    }

    VxNetIdent* netIdent = ( VxNetIdent* )pktAnnounce;
    // ignore duplicates 
    m_MulticastIdentMutex.lock();
    auto iter = m_MulticastIdentList.find( netIdent->getMyOnlineId() );
    if( iter != m_MulticastIdentList.end() )
    {
        if( 0 == memcmp( &iter->second, netIdent, sizeof( VxNetIdent ) ) )
        {
            LogModule( eLogMulticast, LOG_VERBOSE, "NearbyMgr multicast pktAnnounce is duplicate" );
            m_MulticastIdentMutex.unlock();
            return;
        }
        else
        {
            memcpy( &iter->second, netIdent, sizeof( VxNetIdent ) );
        }
    }
    else
    {
        m_MulticastIdentList.emplace( netIdent->getMyOnlineId(), *netIdent );
    }

    m_MulticastIdentMutex.unlock();

    if( !netIdent->getLanIPv4().isIPv4() || 80 > netIdent->getOnlinePort() )
    {
        // not valid to connect to across lan
        return;
    }

    if( !m_Engine.validateIdent( netIdent ) )
    {
        LogModule( eLogMulticast, LOG_ERROR, "NearbyMgr multicast pktAnnounce ident invalid " );
        return;
    }

    int64_t timeNowMs = GetGmtTimeMs();
    bool isConnected = m_Engine.isContactConnected( netIdent->getMyOnlineId() );
    bool isIgnored = false;
    // see if we have big list info already
    BigListInfo* hisInfo = m_Engine.getBigListMgr().findBigListInfo( netIdent->getMyOnlineId() );
    if( hisInfo )
    {
        isIgnored = hisInfo->isIgnored();
        if( isIgnored )
        {
            LogModule( eLogMulticast, LOG_ERROR, "NearbyMgr multicast pktAnnounce ignoring %s ", netIdent->getOnlineName() );
            return;
        }

        if( isConnected )
        {
            if( !hisInfo->isNearby() )
            {
                hisInfo->setIsNearby( true );
                onNearbyUserUpdated( hisInfo, timeNowMs );
            }
        }
    }

    if( !isConnected )
    {
        if( hisInfo )
        {
            if( timeNowMs - hisInfo->getTimeLastConnectAttemptMs() < MIN_TIME_MULTICAST_CONNECT_ATTEMPT )
            {
                // too early to try again
                return;
            }
            else
            {
                hisInfo->setTimeLastConnectAttemptMs( timeNowMs );
            }
        }

        VxSktBase* sktBase = nullptr;
        bool newConnection = false;
        if( true == m_Engine.connectToContact( netIdent->getConnectInfo(), &sktBase, newConnection, eConnectReasonNearbyLan ) )
        {
            LogModule( eLogMulticast, LOG_VERBOSE, "NearbyMgr SUCCESS connect on lan to %s ", netIdent->getOnlineName() );
        }
        else
        {
            LogModule( eLogMulticast, LOG_VERBOSE, "NearbyMgr FAILED to connect on lan to %s ", netIdent->getOnlineName() );
        }
    }
}

//============================================================================
void NearbyMgr::onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId )
{
    bool isIgnored = false;
    BigListInfo* hisInfo = m_Engine.getBigListMgr().findBigListInfo( peerOnlineId );
    if( hisInfo )
    {
        isIgnored = hisInfo->isIgnored();
        hisInfo->setIsNearby( false );
    }

    m_Engine.getNearbyListMgr().removeIdent( peerOnlineId );
    if( !isIgnored )
    {
        // remove from list so will get multicast updates again
        m_MulticastIdentMutex.lock();
        auto iter = m_MulticastIdentList.find( peerOnlineId );
        if( iter != m_MulticastIdentList.end() )
        {
            m_MulticastIdentList.erase( peerOnlineId );
        }
        m_MulticastIdentMutex.unlock();
    }
}

//============================================================================
void NearbyMgr::onNearbyUserUpdated( VxNetIdent* netIdent, int64_t timestamp )
{
    m_Engine.getNearbyListMgr().updateIdent( netIdent->getMyOnlineId(), timestamp ? timestamp : GetGmtTimeMs() );
}

//============================================================================
void NearbyMgr::handleTcpLanConnectSuccess( BigListInfo* bigListInfo, VxSktBase* sktBase, bool isNewConnection, EConnectReason connectReason )
{
    if( bigListInfo )
    {
        bigListInfo->setIsNearby( true );
        if( !bigListInfo->isIgnored() )
        {
            LogModule( eLogMulticast, LOG_VERBOSE, "NearbyMgr connected to to %s ", bigListInfo->getOnlineName() );
            onNearbyUserUpdated( bigListInfo, GetGmtTimeMs() );
        }
    }
}