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

#include "ConnectionMgr.h"
#include "ConnectedInfo.h"

#include <GoTvCore/GoTvP2P/BigListLib/BigListLib.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateRelaySearch.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktConnect.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktCrypto.h>
#include <NetLib/VxSktUtil.h>

#include <PktLib/PktsRelay.h>

#include <CoreLib/VxTime.h>
#include <CoreLib/VxUrl.h>

namespace
{
    const unsigned int		MIN_TIME_BETWEEN_CONNECT_ATTEMPTS_SEC			= (15 * 60); // 15 minutes
    const unsigned int		TIMEOUT_MILLISEC_STAY_CONNECTED					= 2000; 

    //============================================================================
    void * NetConnectorThreadFunction( void * pvParam )
    {
        VxThread * poThread = (VxThread *)pvParam;
        poThread->setIsThreadRunning( true );
        ConnectionMgr * poMgr = (ConnectionMgr *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->doNetConnectionsThread();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }

    //============================================================================
    void * StayConnectedThreadFunction( void * pvParam )
    {
        VxThread * poThread = (VxThread *)pvParam;
        poThread->setIsThreadRunning( true );
        ConnectionMgr * poMgr = (ConnectionMgr *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->doStayConnectedThread();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }

} // namespace

//============================================================================
ConnectionMgr::ConnectionMgr( P2PEngine& engine )
    : m_Engine( engine )
    , m_BigListMgr( engine.getBigListMgr() )
    , m_AllList( engine )
    , m_PktAnn( engine.getMyPktAnnounce() )
    , m_PeerMgr( engine.getPeerMgr() )
{
}

//============================================================================
std::string ConnectionMgr::getDefaultHostUrl( EHostType hostType )
{
    std::string hostUrl("");
    m_ConnectionMutex.lock();
    auto iter = m_DefaultHostUrlList.find( hostType );
    if( iter != m_DefaultHostUrlList.end() )
    {
        hostUrl = iter->second;
    }

    m_ConnectionMutex.unlock();
    return hostUrl;
}

//============================================================================
void ConnectionMgr::setDefaultHostOnlineId( EHostType hostType, VxGUID& hostOnlineId )
{
    m_ConnectionMutex.lock();
    m_DefaultHostIdList[hostType] = hostOnlineId;
    m_ConnectionMutex.unlock();
}

//============================================================================
bool ConnectionMgr::getDefaultHostOnlineId( EHostType hostType, VxGUID& retHostOnlineId )
{
    bool result = false;
    retHostOnlineId.clearVxGUID();

    m_ConnectionMutex.lock();
    auto iter = m_DefaultHostIdList.find( hostType );
    if( iter != m_DefaultHostIdList.end() )
    {
        retHostOnlineId = iter->second;
        result = true;
    }

    m_ConnectionMutex.unlock();
    return result;
}

//============================================================================
EHostJoinStatus ConnectionMgr::lookupOrQueryId( std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    EHostJoinStatus joinStatus = eHostJoinUnknown;
    if( urlCacheOnlineIdLookup( hostUrl, hostGuid ) )
    {
        joinStatus = eHostJoinQueryIdSuccess;
    }
    else
    {
        joinStatus = eHostJoinQueryIdInProgress;
        std::string myUrl = m_Engine.getMyUrl();
        m_Engine.getRunUrlAction().runUrlAction( eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), this, callback, eHostTypeUnknown, connectReason );
    }

    return joinStatus;
}

//============================================================================
bool ConnectionMgr::onSktConnectedWithPktAnn( VxSktBase* sktBase, BigListInfo * bigListInfo )
{
    bool result = true;
    m_ConnectionMutex.lock();
    ConnectedInfo* connectInfo = m_AllList.getOrAddConnectedInfo( bigListInfo );
    if( nullptr == connectInfo )
    {
        result = false;
        LogMsg( LOG_ERROR, "ConnectionMgr get connection info FAILED" );
    }
    else
    {
        connectInfo->onSktConnected( sktBase );
    }

    m_ConnectionMutex.unlock();
    return result;
}

//============================================================================
void ConnectionMgr::onSktDisconnected( VxSktBase* sktBase )
{
    m_ConnectionMutex.lock();
    m_AllList.onSktDisconnected( sktBase );
    m_ConnectionMutex.unlock();
}

//============================================================================
void ConnectionMgr::callbackInternetStatusChanged( EInternetStatus internetStatus )
{
    bool internetBecameAvailable = m_InternetStatus == eInternetNoInternet &&
        internetStatus != eInternetNoInternet;
    m_ConnectionMutex.lock();
    m_InternetStatus = internetStatus;
    m_ConnectionMutex.unlock();
    if( internetBecameAvailable )
    {
        onInternetAvailable();
    }
}

//============================================================================
void ConnectionMgr::callbackNetAvailStatusChanged( ENetAvailStatus netAvalilStatus )
{
    bool networkBecameAvailable = ( m_NetAvailStatus == eNetAvailNoInternet ) && 
        ( netAvalilStatus != eNetAvailNoInternet );
    m_ConnectionMutex.lock();
    m_NetAvailStatus = netAvalilStatus;
    m_ConnectionMutex.unlock();
    if( networkBecameAvailable )
    {
        onNoLimitNetworkAvailable();
    }
}

//============================================================================
void ConnectionMgr::onInternetAvailable( void )
{

}

//============================================================================
void ConnectionMgr::onNoLimitNetworkAvailable( void )
{

}

//============================================================================
void ConnectionMgr::resetDefaultHostUrl( EHostType hostType )
{
    m_DefaultHostIdList[hostType] = VxGUID::nullVxGUID();
    m_DefaultHostUrlList[hostType] = "";
    m_DefaultHostRequiresOnlineId[hostType] = "";
    m_DefaultHostQueryIdFailed[hostType] = eRunTestStatusUnknown;
}

//============================================================================
void ConnectionMgr::applyDefaultHostUrl( EHostType hostType, std::string& hostUrl )
{
    m_ConnectionMutex.lock();
    m_DefaultHostUrlList[hostType] = hostUrl;
    m_ConnectionMutex.unlock();

    VxUrl parsedUrl( hostUrl.c_str() );
    if( parsedUrl.validateUrl( false ) )
    {
        bool needOnlineId = true;
        if( parsedUrl.hasValidOnlineId() )
        {
            VxGUID onlineId;
            onlineId.fromOnlineIdString( parsedUrl.getOnlineId().c_str() );
            if( onlineId.isVxGUIDValid() )
            {
                needOnlineId = false;

                m_ConnectionMutex.lock();
                m_DefaultHostIdList[hostType] = onlineId;  
                m_ConnectionMutex.unlock();
                updateUrlCache( hostUrl, onlineId );
            }       
        }

        if( needOnlineId )
        {
            m_ConnectionMutex.lock();
            m_DefaultHostRequiresOnlineId[hostType] = hostUrl;
            m_ConnectionMutex.unlock();

            std::string myUrl = m_Engine.getMyUrl();
            m_Engine.getRunUrlAction().runUrlAction( eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), this, nullptr, hostType );
        }
    }
}

//============================================================================
void ConnectionMgr::callbackQueryIdSuccess( UrlActionInfo& actionInfo, VxGUID onlineId )
{
    if( eHostTypeUnknown != actionInfo.getHostType() )
    {
        m_ConnectionMutex.lock();
        m_DefaultHostIdList[actionInfo.getHostType()] = onlineId;
        m_DefaultHostRequiresOnlineId[actionInfo.getHostType()] = "";
        m_ConnectionMutex.unlock();
    }

    std::string hostUrl = actionInfo.getRemoteUrl();
    updateUrlCache( hostUrl, onlineId );
    if( actionInfo.getConnectReqInterface() )
    {
        actionInfo.getConnectReqInterface()->onUrlActionQueryIdSuccess( hostUrl, onlineId, actionInfo.getConnectReason() );
    }

    LogMsg( LOG_VERBOSE, "ConnectionMgr: Success query host %s for online id is %s",  hostUrl.c_str(),
        onlineId.toOnlineIdString().c_str());
}

//============================================================================
void ConnectionMgr::callbackActionFailed( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError )
{
    if( eHostTypeUnknown != actionInfo.getHostType() )
    {
        m_ConnectionMutex.lock();
        m_DefaultHostQueryIdFailed[actionInfo.getHostType()] = eStatus;
        m_ConnectionMutex.unlock();
    }

    std::string hostUrl = actionInfo.getRemoteUrl();
    if( actionInfo.getConnectReqInterface() )
    {
        actionInfo.getConnectReqInterface()->onUrlActionQueryIdFail( hostUrl, eStatus, actionInfo.getConnectReason() );
    }

    LogMsg( LOG_ERROR, "ConnectionMgr: query host %s for id failed %s %s",  hostUrl.c_str(),
        DescribeRunTestStatus( eStatus ), DescribeNetCmdError( netCmdError ));
}

//============================================================================
EConnectStatus ConnectionMgr::requestConnection( std::string url, VxGUID onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectionMgr::requestConnection must have valid online id" );
        return eConnectStatusBadParam;
    }

    // first see if we already have a connection to the requested onlineId
    VxSktBase *sktBase = nullptr;
    m_ConnectionMutex.lock();
    ConnectedInfo* connectInfo = m_AllList.getConnectedInfo( onlineId );
    if( connectInfo )
    {
        sktBase = connectInfo->getSktBase();
        if( sktBase )
        {
            connectInfo->addConnectReason( callback, connectReason );
        }
    }

    m_ConnectionMutex.unlock();
    if( sktBase )
    {
        retSktBase = sktBase;
        return eConnectStatusReady;
    }
    else
    {
        return attemptConnection( url, onlineId, callback, retSktBase, connectReason );
    }

    return eConnectStatusUnknown;
}

//============================================================================
EConnectStatus ConnectionMgr::attemptConnection( std::string url, VxGUID& onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason )
{
    EConnectStatus connectStatus = eConnectStatusConnecting;
    VxUrl connectUrl( url.c_str() );
    if( onlineId.isVxGUIDValid() && connectUrl.validateUrl( false ) )
    {
        connectStatus = directConnectTo( url, onlineId, callback, retSktBase, connectReason );
        if( connectStatus == eConnectStatusConnectSuccess )
        {
            // connected but waiting for PktAnnounce reply
            m_ConnectRequests[onlineId] = std::make_pair( callback, connectReason );
            connectStatus = eConnectStatusReady;
        }
    }
    else
    {
        connectStatus = eConnectStatusBadAddress;
    }

    return connectStatus;
}

//============================================================================
void ConnectionMgr::doneWithConnection( VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    m_ConnectionMutex.lock();
    auto inProgressConnection = m_ConnectRequests.find( onlineId );
    if( inProgressConnection != m_ConnectRequests.end() )
    {
        m_ConnectRequests.erase( inProgressConnection );
    }

    ConnectedInfo* connectInfo = m_AllList.getConnectedInfo( onlineId );
    if( connectInfo )
    {
        connectInfo->removeConnectReason( callback, connectReason, true );
    }

    m_ConnectionMutex.unlock();
}

//============================================================================
void ConnectionMgr::updateUrlCache( std::string& hostUrl, VxGUID& onlineId )
{
    if( !hostUrl.empty() && onlineId.isVxGUIDValid() )
    {
        // there should be only one online id per ip and port however the ip may change
        // only keep the latest url
        m_ConnectionMutex.lock();
        for( auto iter = m_UrlCache.begin(); iter != m_UrlCache.end(); ++iter )
        {
            if( iter->second == onlineId )
            {
                if( iter->first == hostUrl )
                {
                    // already in map
                    m_ConnectionMutex.unlock();
                    return;
                }

                m_UrlCache.erase( iter );
                break;
            }
        }

        m_UrlCache[hostUrl] = onlineId;
        m_ConnectionMutex.unlock();
    }
    else
    {
        LogMsg( LOG_ERROR, "ConnectionMgr::updateUrlCache empty url" );
    }
}

//============================================================================
bool ConnectionMgr::urlCacheOnlineIdLookup( std::string& hostUrl, VxGUID& onlineId )
{
    bool foundId = false;
    onlineId.clearVxGUID();
    m_ConnectionMutex.lock();
    auto iter = m_UrlCache.find( hostUrl );
    if( iter != m_UrlCache.end() )
    {
        onlineId = iter->second;
        foundId = true;
    }

    m_ConnectionMutex.unlock();

    return foundId;
}

//============================================================================
EConnectStatus ConnectionMgr::directConnectTo(  std::string                 url,
                                                VxGUID&                     onlineId,
                                                IConnectRequestCallback*    callback,
                                                VxSktBase*&                 retSktBase,
                                                EConnectReason              connectReason,
                                                int					        iConnectTimeoutMs )
{
    VxUrl connectUrl( url.c_str() );
    std::string ipAddr = connectUrl.getHostString();
    uint16_t port = connectUrl.getPort();
    if( !ipAddr.empty() && port )
    {
        return directConnectTo( ipAddr, port, onlineId, retSktBase, callback, connectReason, iConnectTimeoutMs );
    }
    else
    {
        return eConnectStatusBadParam;
    }
}

//============================================================================-
EConnectStatus ConnectionMgr::directConnectTo(  VxConnectInfo&		        connectInfo,
                                                VxSktBase *&		        ppoRetSkt,		// return pointer to socket if not null
                                                int					        iConnectTimeoutMs,// how long to attempt connect
                                                bool				        bUseUdpIp,
                                                bool				        bUseLanIp,
                                                IConnectRequestCallback*    callback,
                                                EConnectReason              connectReason)
{
    ppoRetSkt = nullptr;

    std::string ipAddr;

    if( bUseLanIp
        && connectInfo.getLanIPv4().isValid() )
    {
        ipAddr = connectInfo.getLanIPv4().toStdString();
    }
    else
    {
        connectInfo.m_DirectConnectId.getIpAddress( ipAddr );
    }

    return directConnectTo( ipAddr, connectInfo.getOnlinePort(), connectInfo.getMyOnlineId(), ppoRetSkt, callback, connectReason, iConnectTimeoutMs );
}

//============================================================================-
EConnectStatus ConnectionMgr::directConnectTo(  std::string                 ipAddr,
                                                uint16_t                    port,
                                                VxGUID                      onlineId,
                                                VxSktBase*&                 retSktBase,
                                                IConnectRequestCallback*    callback,
                                                EConnectReason              connectReason,
                                                int					        iConnectTimeoutMs )
{
    EConnectStatus connectStatus = eConnectStatusConnecting;
    VxSktConnect * sktBase = m_PeerMgr.connectTo(	ipAddr.c_str(),			// remote ip or url 
                                                    port,	                // port to connect to
                                                    iConnectTimeoutMs );	// milli seconds before connect attempt times out
    if( sktBase )
    {
        //LogMsg( LOG_INFO, "NetConnector::directConnectTo: connect SUCCESS to %s:%d\n", strIpAddress.c_str(), connectInfo.getOnlinePort() );
        // generate encryption keys
#ifdef DEBUG_SKTS
        LogMsg( LOG_SKT, "NetworkMgr::DirectConnectTo: connect success.. generating tx key\n" );
#endif // DEBUG_SKTS

        GenerateTxConnectionKey( sktBase, sktBase->getRemoteIp(), port, onlineId, m_Engine.getNetworkMgr().getNetworkKey() );
#ifdef DEBUG_SKTS
        LogMsg( LOG_SKT, "NetworkMgr::DirectConnectTo: connect success.. generating rx key\n" );
#endif // DEBUG_SKTS

        GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_Engine.getNetworkMgr().getNetworkKey() );
#ifdef DEBUG_SKTS
        LogMsg( LOG_SKT, "NetworkMgr::DirectConnectTo: connect success.. sending announce\n" );
#endif // DEBUG_SKTS

        //LogMsg( LOG_INFO, "sendMyPktAnnounce 2\n" ); 
        if( false == sendMyPktAnnounce( onlineId, sktBase, true, false, false, false ) )
        {
            if( IsLogEnabled( eLogConnect ) )
                LogMsg( LOG_DEBUG, "NetworkMgr::DirectConnectTo: connect failed sending announce\n" );
            return eConnectStatusSendPktAnnFailed;
        }

        connectStatus = eConnectStatusConnectSuccess;
        retSktBase = (VxSktBase *)sktBase;
    }
    else
    {
        connectStatus = eConnectStatusConnectFailed;

        //LogMsg( LOG_INFO, "NetConnector::directConnectTo: connect FAIL to %s:%d\n", strIpAddress.c_str(), connectInfo.getOnlinePort() );
        if( IsLogEnabled( eLogConnect ) )
            LogMsg( LOG_DEBUG, "NetworkMgr::DirectConnectTo: failed\n" );
    }

    if( IsLogEnabled( eLogConnect ) )
        LogMsg( LOG_DEBUG, "NetworkMgr::DirectConnectTo: done\n" );

    return connectStatus;
}

//============================================================================
void ConnectionMgr::addConnectRequestToQue( VxConnectInfo& connectInfo, EConnectReason connectReason, bool addToHeadOfQue, bool replaceExisting )
{
    ConnectReqInfo connectRequest( connectInfo, connectReason );
    addConnectRequestToQue( connectRequest, addToHeadOfQue, replaceExisting );
}

//============================================================================
void ConnectionMgr::addConnectRequestToQue( ConnectReqInfo& connectRequest, bool addToHeadOfQue, bool replaceExisting  )
{
    m_NetConnectorMutex.lock();
    if( m_IdentsToConnectList.size() )
    {
        // remove any previous that have the same id and reason
        std::vector<ConnectReqInfo>::iterator iter;
        for( iter = m_IdentsToConnectList.begin(); iter != m_IdentsToConnectList.end(); ++iter )
        {
            if( ( (*iter).getMyOnlineId() == connectRequest.getMyOnlineId() )
                && ( (*iter).getConnectReason() == connectRequest.getConnectReason() ) )
            {
                if( replaceExisting )
                {
                    m_IdentsToConnectList.erase( iter );
                    break;
                }
                else
                {
                    m_NetConnectorMutex.unlock();
                    return;
                }
            }
        }
    }

    if( addToHeadOfQue )
    {
        m_IdentsToConnectList.insert( m_IdentsToConnectList.begin(), connectRequest );
    }
    else
    {
        m_IdentsToConnectList.push_back( connectRequest );
    }

    m_NetConnectorMutex.unlock();
    m_WaitForConnectWorkSemaphore.signal();
}

//============================================================================
bool ConnectionMgr::connectToContact(	VxConnectInfo&		connectInfo, 
                                        VxSktBase *&		ppoRetSkt,
                                        bool&				retIsNewConnection )
{
    bool gotConnected	= false;
    retIsNewConnection	 = false;
    if( connectInfo.getMyOnlineId() == m_PktAnn.getMyOnlineId() )
    {
        LogMsg( LOG_ERROR, "NetConnector::connectToContact: cannot connect to ourself\n" );  
        return false;
    }

#ifdef DEBUG_CONNECTIONS
    LogMsg( LOG_SKT, "connectToContact: id %s %s\n",  
        connectInfo.getOnlineName(),
        connectInfo.getMyOnlineId().describeVxGUID().c_str() );
#endif // DEBUG_CONNECTIONS

    m_ConnectionMutex.lock();
    ConnectedInfo * rmtUserConnectInfo = m_AllList.getConnectedInfo( connectInfo.getMyOnlineId() );
    if( rmtUserConnectInfo )
    {
#ifdef DEBUG_CONNECTIONS
        std::string strId;
        connectInfo.getMyOnlineId(strId);
        LogMsg( LOG_SKT, "connectToContact: User is already connected %s id %s\n", 
            m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
            strId.c_str() );
#endif // DEBUG_CONNECTIONS

        ppoRetSkt = rmtUserConnectInfo->getSktBase();
        m_ConnectionMutex.unlock();
        gotConnected = true;
    }
    else
    {
        m_ConnectionMutex.unlock();
        if( connectUsingTcp( connectInfo, ppoRetSkt ) )
        {
            gotConnected		= true;
            retIsNewConnection	= true;
        }
    }

    return gotConnected;
}

//============================================================================
bool ConnectionMgr::connectUsingTcp( VxConnectInfo&	connectInfo, VxSktBase *& ppoRetSkt )
{
    ppoRetSkt = nullptr;
    VxSktBase* sktBase = nullptr;
    if( false == connectInfo.m_DirectConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "connectUsingTcp: User invalid online id\n" );
        return false;
    }

    std::string strDirectConnectIp;
 
    if( ( connectInfo.getMyOnlineIPv4() == m_PktAnn.getMyOnlineIPv4() )
        && connectInfo.getMyOnlineIPv4().isValid()
        && connectInfo.getLanIPv4().isValid() )
    {
        if( connectInfo.getMyOnlinePort() == m_PktAnn.getMyOnlinePort() )
        {
            LogMsg( LOG_ERROR, "ERROR connectUsingTcp: attempting connect to our ip and port for different id %s\n", connectInfo.getOnlineName() );
            return false;
        }

        // probably on same network so use local ip
        if( eConnectStatusConnectSuccess == directConnectTo(	connectInfo, 
                                                                sktBase, 
                                                                LAN_CONNECT_TIMEOUT,
                                                                false, 
                                                                true ) )
        {
#ifdef DEBUG_CONNECTIONS
            connectInfo.getLanIPv4().toStdString( strDirectConnectIp );
            LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d LAN connect to %s ip %s port %d\n",
                sktBase->m_iSktId,
                connectInfo.getOnlineName(),
                strDirectConnectIp.c_str(),
                connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
            ppoRetSkt = sktBase;
            return true;
        }
#ifdef DEBUG_CONNECTIONS
        else
        {
            LogMsg( LOG_SKT, "connectUsingTcp: FAIL LAN connect to %s ip %s port %d\n",
                connectInfo.getOnlineName(),
                strDirectConnectIp.c_str(),
                connectInfo.m_DirectConnectId.getPort() );
        }
#endif // DEBUG_CONNECTIONS
    }

#ifdef DEBUG_CONNECTIONS
    std::string debugClientOnlineId;
    connectInfo.getMyOnlineId(debugClientOnlineId);
    LogMsg( LOG_INFO, "connectUsingTcp %s id %s \n", 
        connectInfo.getOnlineName(),
        debugClientOnlineId.c_str() );
#endif // DEBUG_CONNECTIONS

    // verify proxy if proxy required
    bool requiresRelay = connectInfo.requiresRelay();
    if( requiresRelay )
    {
        if( connectInfo.m_RelayConnectId.isVxGUIDValid() )
        {
            if( connectInfo.m_RelayConnectId.getOnlineId() == m_PktAnn.getMyOnlineId() )
            {
                // we are this persons proxy
                // dont attempt to connect.. he has to connect to us
#ifdef DEBUG_CONNECTIONS
                LogMsg( LOG_INFO, "We are Users proxy must wait for them to connect to me\n" );
#endif // DEBUG_CONNECTIONS
                // try ipv6 if available
                return tryIPv6Connect( connectInfo, ppoRetSkt );
            }

            //LogMsg( LOG_INFO, "User %s has proxy.. attempting proxy connect\n",  connectInfo.m_as8OnlineName );
        }
        else
        {
            std::string strMyOnlineId;
            connectInfo.getMyOnlineId(strMyOnlineId);
#ifdef DEBUG_CONNECTIONS
            LogMsg( LOG_ERROR, "connectUsingTcp: FAIL User id %s does not have proxy set.. \n", 
                strMyOnlineId.c_str());
#endif // DEBUG_CONNECTIONS
            return tryIPv6Connect( connectInfo, ppoRetSkt );
        }
    }

    connectInfo.m_DirectConnectId.getIpAddress( strDirectConnectIp );

    //LogMsg( LOG_INFO, "User %s requires proxy? %d\n",  connectInfo.m_as8OnlineName, requiresRelay );
    if( false == requiresRelay )
    {
#ifdef DEBUG_CONNECTIONS
        LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: Attempting direct connect to %s ip %s port %d\n",
            m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
            strDirectConnectIp.c_str(),
            connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
        if( eConnectStatusConnectSuccess == directConnectTo( connectInfo, sktBase, DIRECT_CONNECT_TIMEOUT ) )
        {
            //LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: success\n" );
            // direct connection success
#ifdef DEBUG_CONNECTIONS
            LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d direct connect to %s ip %s port %d\n",
                sktBase->m_iSktId,
                m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
                strDirectConnectIp.c_str(),
                connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
            ppoRetSkt = sktBase;

            return true;
        }
        else
        {
#ifdef DEBUG_CONNECTIONS
            LogMsg( LOG_SKT, "directConnectTo: FAIL LAN connect to %s ip %s port %d\n",
                connectInfo.getOnlineName(),
                strDirectConnectIp.c_str(),
                connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
#ifdef SUPPORT_IPV6
            return tryIPv6Connect( connectInfo, ppoRetSkt );
#else
            return false; // no ipv6 support
#endif // SUPPORT_IPV6
        }
    }
    else
    {
        std::string strRelayConnectIp;
        connectInfo.m_RelayConnectId.getIpAddress( strRelayConnectIp );
        std::string strUserId;
        connectInfo.getMyOnlineId( strUserId );
#ifdef DEBUG_MUTEXES
        LogMsg( LOG_SKT, "connectUsingTcp: m_ConnectListMutex.lock()\n" );
#endif // DEBUG_MUTEXES
        m_ConnectionMutex.lock();
        ConnectedInfo * poRelayConnectInfo = m_AllList.getConnectedInfo( connectInfo.m_RelayConnectId.getOnlineId() );
        //LogMsg( LOG_INFO, "P2PEngine::ConnectToContact:User %s existing connection to proxy %d\n", connectInfo.getOnlineName(), poConnectInfo );
        if( poRelayConnectInfo )
        {
            sktBase = poRelayConnectInfo->getSktBase();
            if( !sktBase )
            {
                // no relay connection
                return false;
            }

#ifdef DEBUG_NET_CONNECTOR
            LogMsg( LOG_SKT, "connectUsingTcp: FOUND User %s proxy %s connection skt %d proxy ip %s port %d to ip %s port %d id %s\n",
                m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
                poRelayConnectInfo->m_BigListInfo->getOnlineName(),
                poRelayConnectInfo->m_SktBase->m_iSktId,
                strRelayConnectIp.c_str(),
                connectInfo.m_RelayConnectId.getPort(),
                strDirectConnectIp.c_str(), 
                connectInfo.m_DirectConnectId.getPort(),
                strUserId.c_str() );
#endif // DEBUG_NET_CONNECTOR
            // now send announce to remote user
            bool requestReverseConnection = ( ( false == m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );
            bool requestSTUN = ( ( m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );

            //LogMsg( LOG_INFO, "sendMyPktAnnounce 1\n" ); 
            bool sendAnnResult = sendMyPktAnnounce(	connectInfo.getMyOnlineId(), 
                                                    sktBase, 
                                                    true, 
                                                    false,
                                                    requestReverseConnection,
                                                    requestSTUN );
#ifdef DEBUG_NET_CONNECTOR
            LogMsg( LOG_SKT, "connectUsingTcp: m_AllList.connectListUnlock()\n" );
#endif // DEBUG_NET_CONNECTOR
            m_ConnectionMutex.unlock();
            if( false == sendAnnResult )
            {
                //RCODE rc = sktBase->getLastSktError();
                //LogMsg( LOG_INFO, "Error %d %s Transmitting PktAnn to contact %s\n", 
                //	rc, 
                //	sktBase->describeSktError( rc ),
                //	m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ) );
                sktBase->closeSkt( 2154 );
                sktBase = nullptr;
            }
        }
        else
        {
#ifdef DEBUG_NET_CONNECTOR
            LogMsg( LOG_SKT, "connectUsingTcp: m_AllList.connectListUnlock()\n" );
#endif // DEBUG_NET_CONNECTOR
            m_ConnectionMutex.unlock();
            //LogMsg( LOG_INFO, "P2PEngine::ConnectToContact:User %s RmtUserRelayConnectTo\n", connectInfo.getOnlineName() );
            // attempt to connect to users proxy
            if( eConnectStatusConnectSuccess == rmtUserRelayConnectTo( connectInfo, sktBase, TO_PROXY_CONNECT_TIMEOUT ) )
            {
#ifdef DEBUG_NET_CONNECTOR
                LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS connect proxy skt %d to %s ip %s port %d for id %s\n",
                    sktBase->m_iSktId,
                    m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
                    strRelayConnectIp.c_str(),
                    connectInfo.m_RelayConnectId.getPort(),
                    strUserId.c_str() );
#endif // DEBUG_NET_CONNECTOR
            }
            else
            {
#ifdef DEBUG_NET_CONNECTOR
                if( IsLogEnabled( eLogConnect ) )
                    LogMsg( LOG_SKT, "connectUsingTcp: FAIL connect to proxy to %s with proxy ip %s port %d for id %s\n",
                        m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
                        strRelayConnectIp.c_str(),
                        connectInfo.m_RelayConnectId.getPort(),
                        strUserId.c_str() );
#endif // DEBUG_NET_CONNECTOR
            }
        }

        if( sktBase  )
        {
            // if user is behind proxy but we are not then wait for user to direct connect to us
            if( ( false == m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() )
            {
                for( int i = 0; i < 60; i++ )
                {
                    if( m_Engine.isContactConnected( connectInfo.getMyOnlineId() ) )
                    {
                        break;
                    }

                    VxSleep( 100 );
                }
            }
            else if( sendRequestConnectionThroughRelay(	sktBase, connectInfo ) )
            {
                sktBase->m_RelayEventSemaphore.wait( THROUGH_PROXY_RESPONSE_TIMEOUT );
            }

            m_ConnectionMutex.lock();
            ConnectedInfo * poUserConnectInfo = m_AllList.getConnectedInfo( connectInfo.getMyOnlineId() );
            if( poUserConnectInfo )
            {
#ifdef DEBUG_NET_CONNECTOR
                LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS requestConnectionThroughRelay %s to %s\n",
                    m_Engine.knownContactNameFromId( connectInfo.m_RelayConnectId.getOnlineId() ),
                    m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() )
                );
#endif // DEBUG_NET_CONNECTOR
                ppoRetSkt = sktBase;
            }
            else
            {
#ifdef DEBUG_NET_CONNECTOR
                LogMsg( LOG_SKT, "connectUsingTcp: FAIL requestConnectionThroughRelay %s to %s\n",
                    m_Engine.knownContactNameFromId( connectInfo.m_RelayConnectId.getOnlineId() ),
                    m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() )
                );
#endif // DEBUG_NET_CONNECTOR
            }

            m_ConnectionMutex.unlock();
        }

#ifdef DEBUG_SKTS
        else
        {
            LogMsg( LOG_SKT, "connectUsingTcp: NULL Socket\n" );
        }
#endif // DEBUG_SKTS
    }

#ifdef DEBUG_CONNECTIONS
    //LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: returning skt 0x%x\n", *ppoRetSkt );
#endif // DEBUG_CONNECTIONS
    if( sktBase )
    {
        ppoRetSkt = sktBase;
        return true;
    }

    return tryIPv6Connect( connectInfo, ppoRetSkt );
}

//============================================================================
bool ConnectionMgr::tryIPv6Connect(	VxConnectInfo& connectInfo, VxSktBase *& ppoRetSkt )
{
    bool connectSuccess = false;
    if( m_PktAnn.getMyOnlineIPv6().isValid()
        && connectInfo.getMyOnlineIPv6().isValid() )
    {
        std::string ipv6;
        ipv6 = connectInfo.getMyOnlineIPv6().toStdString();
        // not likely to succeed so just see if we can get a socket
        SOCKET skt = ::VxConnectToIPv6( ipv6.c_str(), connectInfo.getOnlinePort() );
        if( INVALID_SOCKET != skt )
        {
            VxSktBase* sktBase = m_PeerMgr.createConnectionUsingSocket( skt, ipv6.c_str(), connectInfo.getOnlinePort() );
            connectSuccess = ( nullptr != sktBase );
            if( connectSuccess )
            {
                ppoRetSkt = sktBase;
            }
        }
    }

    return connectSuccess;
}

//============================================================================
//! encrypt and send my PktAnnounce to someone of whom we have no recored except from anchor announce
bool ConnectionMgr::sendMyPktAnnounce(  VxGUID&				destinationId,
                                        VxSktBase *			sktBase, 
                                        bool				requestAnnReply,
                                        bool				requestTop10,
                                        bool				requestReverseConnection,
                                        bool				requestSTUN )
{
    m_PktAnn.setAppAliveTimeSec( GetApplicationAliveSec() );
    PktAnnounce pktAnn;
    memcpy( &pktAnn, &m_PktAnn, sizeof(PktAnnounce) );
    pktAnn.setIsPktAnnReplyRequested( requestAnnReply );
    pktAnn.setIsTopTenRequested( requestTop10 );
    pktAnn.setIsPktAnnRevConnectRequested( requestReverseConnection );
    pktAnn.setIsPktAnnStunRequested( requestSTUN );

    EFriendState eMyFriendshipToHim;
    EFriendState eHisFriendshipToMe;
    m_BigListMgr.getFriendships( destinationId, eMyFriendshipToHim, eHisFriendshipToMe );

    pktAnn.setMyFriendshipToHim( eMyFriendshipToHim );
    pktAnn.setHisFriendshipToMe( eHisFriendshipToMe );

    //LogMsg( LOG_INFO, "SendMyPktAnnounce; to %s Hi 0x%llX, Lo 0x%llX skt %d request reply %d\n", 
    //	m_Engine.knownContactNameFromId( destinationId ),
    //	destinationId.getVxGUIDHiPart(),
    //	destinationId.getVxGUIDLoPart(),
    //	sktBase->m_iSktId,
    //	requestAnnReply );

    return txPacket( destinationId, sktBase, &pktAnn );	
}

//============================================================================
bool ConnectionMgr::txPacket(	VxGUID&				destinationId, 
                                VxSktBase *			sktBase, 
                                VxPktHdr *			poPkt )
{
    bool bSendSuccess = false;
    poPkt->setSrcOnlineId( m_PktAnn.getMyOnlineId() );

    if( 0 == (poPkt->getPktLength() & 0xf ) )
    {
        if( sktBase->isConnected() && sktBase->isTxEncryptionKeySet() )
        {
            sktBase->m_u8TxSeqNum++;
            poPkt->setPktSeqNum( sktBase->m_u8TxSeqNum );
            RCODE rc = sktBase->txPacket( destinationId, poPkt, false );
            if( 0 == rc )
            {
                bSendSuccess = true;
            }
            else
            {
                LogMsg( LOG_ERROR, "NetConnector::txPacket: %s error %d\n", sktBase->describeSktType().c_str(), rc );
            }
        }
        else
        {
            if( false == sktBase->isConnected() )
            {
                LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d not connected\n", sktBase->m_iSktId );
            }
            else
            {
                LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d has no encryption key\n", sktBase->m_iSktId );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "P2PEngine::txPluginPkt: Invalid system Packet length %d type %d\n", 
            poPkt->getPktLength(),
            poPkt->getPktType() );
    }

    return bSendSuccess;
}

//============================================================================
void ConnectionMgr::doNetConnectionsThread( void )
{
    while( ( false == m_NetConnectThread.isAborted() )
        && ( false == VxIsAppShuttingDown() ) )
    {
        m_WaitForConnectWorkSemaphore.wait( 1000 );
        while( m_IdentsToConnectList.size() )
        {
            if( m_NetConnectThread.isAborted() )
            {
                break;
            }

            m_NetConnectorMutex.lock();
            ConnectReqInfo connectRequest = m_IdentsToConnectList[0];
            m_IdentsToConnectList.erase( m_IdentsToConnectList.begin() );
            m_NetConnectorMutex.unlock();

            doConnectRequest( connectRequest, false );
        }
    }
}

//============================================================================
void ConnectionMgr::handleConnectSuccess(  BigListInfo * bigListInfo, VxSktBase * skt, bool isNewConnection, EConnectReason connectReason )
{
    if( 0 != bigListInfo )
    {
        int64_t timeNow = GetGmtTimeMs();
        bigListInfo->setTimeLastConnectAttemptMs( timeNow );
        bigListInfo->setIsOnline( true );
        bigListInfo->setIsConnected( true );
        if( eConnectReasonRandomConnectJoin == connectReason )
        {
            m_Engine.getToGui().toGuiSearchResultSuccess( eScanTypeRandomConnect, bigListInfo );
        }
    }
}

//============================================================================
void ConnectionMgr::closeConnection( VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo )
{
    if( nullptr == poInfo )
    {
        poInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
    }

    if( nullptr == poInfo )
    {
        LogMsg( LOG_ERROR, "Failed to find info for %s\n", onlineId.toHexString().c_str() );
        skt->closeSkt( 235 );
        return;
    }

    ConnectedInfo * poRmtUserConnectInfo = m_AllList.getConnectedInfo( onlineId );
    if( poRmtUserConnectInfo )
    {
        /*
        if( poRmtUserConnectInfo->isRelayServer()
            || poRmtUserConnectInfo->isRelayClient() )
        {
            PktRelayUserDisconnect pktRelayDisconnect;
            pktRelayDisconnect.setSrcOnlineId( m_Engine.getMyPktAnnounce().getMyOnlineId() );
            pktRelayDisconnect.m_UserId = onlineId;
            skt->txPacket( onlineId, &pktRelayDisconnect );
        }
        else 
        {
            skt->closeSkt( 236 );
        }
        */
    }
    else
    {
        LogMsg( LOG_ERROR, "Failed to find ConnectedInfo for %s\n", onlineId.toHexString().c_str() );
        skt->closeSkt( 237 );
    }
}


//============================================================================-
//! attempt connect to remote user's proxy ( only connects and send pkt announce )
EConnectStatus ConnectionMgr::rmtUserRelayConnectTo(	VxConnectInfo&		connectInfo,
                                                        VxSktBase *&		ppoRetSkt,			// return pointer to socket if not null
                                                        int					iConnectTimeout )	// seconds before connect attempt times out
{
    EConnectStatus connectStatus = eConnectStatusConnectFailed;
    std::string strIpAddress;
    connectInfo.m_RelayConnectId.getIpAddress( strIpAddress );

    VxSktConnect * sktBase = m_PeerMgr.connectTo(	strIpAddress.c_str(),					// remote ip or url 
                                                    connectInfo.m_RelayConnectId.getPort(),	// port to connect to
                                                    iConnectTimeout );						// seconds before connect attempt times out
    if( sktBase )
    {
        // generate encryption keys
        GenerateTxConnectionKey( sktBase, &connectInfo.m_RelayConnectId, m_Engine.getNetworkMgr().getNetworkKey() );
        GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_Engine.getNetworkMgr().getNetworkKey() );

        // we are connected to users proxy
        // first send announcement to his proxy then to him
        VxGUID& oRelayOnlineId = connectInfo.m_RelayConnectId.getOnlineId();
        LogMsg( LOG_INFO, "sendMyPktAnnounce 3\n" ); 
        bool bResult =  sendMyPktAnnounce(	oRelayOnlineId, 
                                            sktBase, 
                                            eFriendStateAnonymous, 
                                            eFriendStateAnonymous,
                                            true,
                                            false );
        if( true == bResult )
        {
            // now send announce to remote user
            bool requestReverseConnection = ( ( false == m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );
            bool requestSTUN = ( ( m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );
            LogMsg( LOG_INFO, "sendMyPktAnnounce 4\n" ); 
            bResult = sendMyPktAnnounce( connectInfo.getMyOnlineId(), 
                                            sktBase, 
                                            true, 
                                            false,
                                            requestReverseConnection,
                                            requestSTUN );
            if( false == bResult )
            {
                RCODE rc = sktBase->getLastSktError();
                LogMsg( LOG_INFO, "Error %d %s Transmitting PktAnn to contact\n", rc, sktBase->describeSktError( rc ) );
                sktBase->closeSkt( 2154 );
                sktBase = nullptr;
            }
        }
        else
        {
            sktBase->closeSkt( 2155 );
            sktBase = nullptr;
        }

        if( ppoRetSkt && sktBase)
        {
            ppoRetSkt = (VxSktBase *)sktBase;
            connectStatus = eConnectStatusConnectSuccess;
        }
    }

    return connectStatus;
}

//============================================================================
bool ConnectionMgr::sendRequestConnectionThroughRelay( VxSktBase* sktBase, VxConnectInfo& connectInfo )
{
    // request connection to user through his proxy

    PktRelayConnectReq oPkt;
    oPkt.m_DestOnlineId =  connectInfo.m_DirectConnectId;
#ifdef DEBUG_CONNECTIONS
    LogMsg( LOG_INFO, "NetworkMgr::sendRequestConnectionThroughRelay\n" );
#endif // DEBUG_CONNECTIONS
    if( true == m_Engine.txSystemPkt( connectInfo.m_RelayConnectId.getOnlineId(), sktBase, &oPkt ) && sktBase->isConnected() )
    {
        return true;
    }

    return false;
}

//============================================================================
void ConnectionMgr::doStayConnectedThread( void )
{
    int iConnectToIdx							= 0;
    VxMutex * poListMutex						= &m_BigListMgr.m_FriendListMutex;
    std::vector< BigListInfo * >& friendList	= m_BigListMgr.m_FriendList;
    int iSize;
    BigListInfo * poInfo;

    std::vector< BigListInfo * >::iterator iter;
    VxSktBase * sktBase;
    while( ( false == m_StayConnectedThread.isAborted() )
        && ( false == VxIsAppShuttingDown() ) )
    {
        VxSleep( TIMEOUT_MILLISEC_STAY_CONNECTED );
        if( false == m_Engine.getNetworkStateMachine().isP2POnline() )
        {
            // don't ping friends until we are fully online with relay service if required and correct connect info in announcement
            continue;
        }

        if( 0 != friendList.size() )
        {
            //LogMsg( LOG_ERROR, "doStayConnected attempt lock\n" );
            poListMutex->lock();
            //LogMsg( LOG_ERROR, "doStayConnected attempt lock success\n" );
            iSize = (int)friendList.size();
            if( iSize )
            {
                iConnectToIdx++;
                if( iConnectToIdx >= iSize )
                {
                    iConnectToIdx = 0;
                }

                poInfo = friendList[iConnectToIdx];
                if( false == poInfo->isConnected() )
                {
                    if( MIN_TIME_BETWEEN_CONNECT_ATTEMPTS_SEC < ( GetGmtTimeMs() - poInfo->getTimeLastConnectAttemptMs() ) )
                    {
                        bool isNewConnection = false;
                        if( m_Engine.connectToContact( poInfo->getConnectInfo(), &sktBase, isNewConnection, eConnectReasonStayConnected ) )
                        {
                            poInfo->contactWasAttempted( true );
                        }
                        else
                        {
                            poInfo->contactWasAttempted( false );
                        }
                    }
                }
            }

            poListMutex->unlock();
        }

        if( m_StayConnectedThread.isAborted() )
        {
            return;
        }
    }
}

//============================================================================
bool ConnectionMgr::doConnectRequest( ConnectReqInfo& connectRequest, bool ignoreToSoonToConnectAgain )
{
    int64_t timeNow = GetGmtTimeMs();
    VxConnectInfo& connectInfo = connectRequest.getConnectInfo();
    if( false == m_Engine.getNetworkStateMachine().isP2POnline() )
    {
        if( eConnectReasonRelaySearch != connectRequest.getConnectReason() )
        {
            LogMsg( LOG_ERROR, "NetConnector::doConnectRequest when not online\n" );
        }
    }


    P2PConnectList& connectedList = m_Engine.getConnectList();
    connectedList.connectListLock();
    RcConnectInfo *	rcInfo = connectedList.findConnection( connectRequest.getMyOnlineId() );
    if( rcInfo )
    {
        // already connected
        BigListInfo * bigInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
        if( bigInfo )
        {
            connectedList.connectListUnlock();
            bigInfo->setTimeLastTcpContactMs( timeNow );
            bigInfo->setTimeLastConnectAttemptMs( timeNow );
            connectRequest.setTimeLastConnectAttemptMs( timeNow );
            handleConnectSuccess( bigInfo, rcInfo->getSkt(), false, connectRequest.getConnectReason() );
            return true;
        }
    }

    connectedList.connectListUnlock();

    if( ( false == ignoreToSoonToConnectAgain )
        && connectRequest.isTooSoonToAttemptConnectAgain() )
    {
#ifdef DEBUG_CONNECTIONS
        LogMsg( LOG_INFO, "NetConnector::doConnectRequest: to soon to connect again %s\n", m_Engine.describeContact( connectRequest ).c_str() );
#endif // DEBUG_CONNECTIONS
        return false;
    }

    connectRequest.setTimeLastConnectAttemptMs( timeNow );
    BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
    if( bigListInfo )
    {
        bigListInfo->setTimeLastConnectAttemptMs( timeNow );
    }

    VxSktBase * retSktBase = NULL;
    bool isNewConnection = false;
    if( m_Engine.connectToContact( connectInfo, &retSktBase, isNewConnection, connectRequest.getConnectReason() ) )
    {
        // handle success connect
#ifdef DEBUG_CONNECTIONS
        LogMsg( LOG_INFO, "NetConnector::doConnectRequest: success  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
#endif // DEBUG_CONNECTIONS
        if( 0 == bigListInfo )
        {
            // when connected should have created a big list entry when got back a packet announce
            bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
        }

        if( bigListInfo )
        {
            handleConnectSuccess( bigListInfo, retSktBase, isNewConnection, connectRequest.getConnectReason() );
        }
#ifdef DEBUG_CONNECTIONS
        else
        {
            LogMsg( LOG_INFO, "NetConnector::doConnectRequest: No BigList for connected  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
        }
#endif // DEBUG_CONNECTIONS

        return true;
    }

    // handle fail connect
#ifdef DEBUG_CONNECTIONS
    LogMsg( LOG_INFO, "NetConnector::doConnectRequest: connect fail  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
#endif // DEBUG_CONNECTIONS
    return false;
}