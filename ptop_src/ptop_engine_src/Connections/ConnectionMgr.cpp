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
// http://www.nolimitconnect.org
//============================================================================

#include "ConnectionMgr.h"
#include "ConnectedInfo.h"
#include "HandshakeInfo.h"

#include <ptop_src/ptop_engine_src/BigListLib/BigListLib.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <ptop_src/ptop_engine_src/UrlMgr/UrlMgr.h>

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
    lockConnectionList();
    auto iter = m_DefaultHostUrlList.find( hostType );
    if( iter != m_DefaultHostUrlList.end() )
    {
        hostUrl = iter->second;
    }

    unlockConnectionList();
    return hostUrl;
}

//============================================================================
void ConnectionMgr::setDefaultHostOnlineId( EHostType hostType, VxGUID& hostOnlineId )
{
    lockConnectionList();
    m_DefaultHostIdList[hostType] = hostOnlineId;
    unlockConnectionList();
}

//============================================================================
bool ConnectionMgr::getDefaultHostOnlineId( EHostType hostType, VxGUID& retHostOnlineId )
{
    bool result = false;
    retHostOnlineId.clearVxGUID();

    lockConnectionList();
    auto iter = m_DefaultHostIdList.find( hostType );
    if( iter != m_DefaultHostIdList.end() )
    {
        retHostOnlineId = iter->second;
        result = true;
    }

    unlockConnectionList();
    return result;
}

//============================================================================
EHostAnnounceStatus ConnectionMgr::lookupOrQueryAnnounceId( EHostType hostType, VxGUID& sessionId, std::string hostUrl, 
    VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    EHostAnnounceStatus hostStatus = eHostAnnounceUnknown;
    if( urlCacheOnlineIdLookup( hostUrl, hostGuid ) )
    {
        hostStatus = eHostAnnounceQueryIdSuccess;
        if( hostType != eHostTypeUnknown )
        {
            m_Engine.getHostUrlListMgr().updateHostUrl( hostType, hostGuid, hostUrl );
        }
    }
    else if( getQueryIdFailedCount( hostType ) > 2 )
    {
        // dont keep hammering server if is sending an error
        hostStatus = eHostAnnounceQueryIdFailed;
    }
    else
    {
        hostStatus = eHostAnnounceQueryIdInProgress;
        std::string myUrl = m_Engine.getMyOnlineUrl();
        m_Engine.getRunUrlAction().runUrlAction( sessionId, eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), this, callback, hostType, connectReason );
    }

    return hostStatus;
}

//============================================================================
EHostJoinStatus ConnectionMgr::lookupOrQueryJoinId( VxGUID& sessionId, std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    EHostJoinStatus joinStatus = eHostJoinUnknown;
    if( urlCacheOnlineIdLookup( hostUrl, hostGuid ) )
    {
        joinStatus = eHostJoinQueryIdSuccess;
    }
    else
    {
        joinStatus = eHostJoinQueryIdInProgress;
        std::string myUrl = m_Engine.getMyOnlineUrl();
        m_Engine.getRunUrlAction().runUrlAction( sessionId, eNetCmdQueryHostOnlineIdReq, 
            m_Engine.getUrlMgr().resolveUrl(hostUrl).c_str(), myUrl.c_str(), this, callback, eHostTypeUnknown, connectReason );
    }

    return joinStatus;
}

//============================================================================
EHostSearchStatus ConnectionMgr::lookupOrQuerySearchId( VxGUID& sessionId, std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    EHostSearchStatus joinStatus = eHostSearchUnknown;
    if( urlCacheOnlineIdLookup( hostUrl, hostGuid ) )
    {
        joinStatus = eHostSearchQueryIdSuccess;
    }
    else
    {
        joinStatus = eHostSearchQueryIdInProgress;
        std::string myUrl = m_Engine.getMyOnlineUrl();
        m_Engine.getRunUrlAction().runUrlAction( sessionId, eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), this, callback, eHostTypeUnknown, connectReason );
    }

    return joinStatus;
}


//============================================================================
void ConnectionMgr::onSktConnectedWithPktAnn( VxSktBase* sktBase, BigListInfo * bigListInfo )
{
    bool result = true;
    std::vector<HandshakeInfo> shakeList;
    std::vector<HandshakeInfo> timedOutList;
    m_HandshakeMutex.lock();
    m_HandshakeList.getAndRemoveHandshakeInfo( bigListInfo->getMyOnlineId(), shakeList, timedOutList );
    m_HandshakeMutex.unlock();

    if( !timedOutList.empty() )
    {
        for( HandshakeInfo& shakeInfo : timedOutList )
        {
            shakeInfo.onHandshakeTimeout();
        }
    }

    if( !shakeList.empty() )
    {
        lockConnectionList();
        ConnectedInfo* connectInfo = m_AllList.getOrAddConnectedInfo( bigListInfo );
        if( nullptr == connectInfo )
        {
            result = false;
            LogMsg( LOG_ERROR, "ConnectionMgr get connection info FAILED" );
        }
        else
        {
            for( HandshakeInfo& shakeInfo : shakeList )
            {
                connectInfo->addConnectReason( shakeInfo );
            }
        }

        // if we do the onContactConnected() while locked then may deadlock if doneWithConnection is called in onContactConnected
        unlockConnectionList();
       
        for( HandshakeInfo& shakeInfo : shakeList )
        {
            shakeInfo.onContactConnected();
        }
    }
}

//============================================================================
void ConnectionMgr::onSktDisconnected( VxSktBase* sktBase )
{
    m_HandshakeMutex.lock();
    m_HandshakeList.onSktDisconnected( sktBase );
    m_HandshakeMutex.unlock();

    lockConnectionList();
    m_AllList.onSktDisconnected( sktBase );
    unlockConnectionList();
}

//============================================================================
void ConnectionMgr::callbackInternetStatusChanged( EInternetStatus internetStatus )
{
    bool internetBecameAvailable = m_InternetStatus == eInternetNoInternet &&
        internetStatus != eInternetNoInternet;
    lockConnectionList();
    m_InternetStatus = internetStatus;
    unlockConnectionList();
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
    lockConnectionList();
    m_NetAvailStatus = netAvalilStatus;
    unlockConnectionList();
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
    m_DefaultHostQueryIdFailed[hostType] = std::make_pair(eRunTestStatusUnknown, 0);
}

//============================================================================
void ConnectionMgr::applyDefaultHostUrl( EHostType hostType, std::string& hostUrlIn )
{
    std::string hostUrl = m_Engine.getUrlMgr().resolveUrl( hostUrlIn );

    lockConnectionList();
    m_DefaultHostUrlList[hostType] = hostUrl;
    unlockConnectionList();

    VxUrl parsedUrl( hostUrl.c_str() );
    if( parsedUrl.validateUrl( false ) )
    {
        bool needOnlineId = true;
        VxGUID onlineId = parsedUrl.getOnlineId();
        if( onlineId.isVxGUIDValid() )
        {
            needOnlineId = false;

            lockConnectionList();
            m_DefaultHostIdList[hostType] = onlineId;  
            unlockConnectionList();
            updateUrlCache( hostUrl, onlineId );   
            m_Engine.getUrlMgr().updateUrlCache( hostUrl, onlineId );
            if( hostType != eHostTypeUnknown )
            {
                std::string resolvedUrl = m_Engine.getUrlMgr().resolveUrl( hostUrl );
                m_Engine.getHostUrlListMgr().updateHostUrl( hostType, onlineId, resolvedUrl );
            }
        }

        if( needOnlineId )
        {
            lockConnectionList();
            m_DefaultHostRequiresOnlineId[hostType] = hostUrl;
            unlockConnectionList();

            std::string myUrl = m_Engine.getMyOnlineUrl();
            static VxGUID sessionId;
            VxGUID::generateNewVxGUID( sessionId );
            m_Engine.getRunUrlAction().runUrlAction( sessionId, eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), this, nullptr, hostType );
        }
    }
}

//============================================================================
void ConnectionMgr::callbackQueryIdSuccess( UrlActionInfo& actionInfo, VxGUID onlineId )
{
    if( eHostTypeUnknown != actionInfo.getHostType() )
    {
        lockConnectionList();
        m_DefaultHostIdList[actionInfo.getHostType()] = onlineId;
        m_DefaultHostRequiresOnlineId[actionInfo.getHostType()] = "";
        unlockConnectionList();
    }

    std::string hostUrl = actionInfo.getRemoteUrl();
    updateUrlCache( hostUrl, onlineId );
    if( actionInfo.getHostType() != eHostTypeUnknown )
    {
        std::string resolvedUrl = m_Engine.getUrlMgr().resolveUrl( hostUrl );
        m_Engine.getHostUrlListMgr().updateHostUrl( actionInfo.getHostType(), onlineId, resolvedUrl );
    }

    if( actionInfo.getConnectReqInterface() )
    {
        actionInfo.getConnectReqInterface()->onUrlActionQueryIdSuccess( actionInfo.getSessionId(), hostUrl, onlineId, actionInfo.getConnectReason() );
    }

    LogMsg( LOG_VERBOSE, "ConnectionMgr: Success query host %s for online id is %s",  hostUrl.c_str(),
        onlineId.toOnlineIdString().c_str());
}

//============================================================================
void ConnectionMgr::callbackActionFailed( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError )
{
    if( eHostTypeUnknown != actionInfo.getHostType() )
    {
        int failedCount = getQueryIdFailedCount( actionInfo.getHostType() );
        lockConnectionList();
        m_DefaultHostQueryIdFailed[actionInfo.getHostType()] = std::make_pair(eStatus, failedCount);
        unlockConnectionList();
    }

    std::string hostUrl = actionInfo.getRemoteUrl();
    if( actionInfo.getConnectReqInterface() )
    {
        actionInfo.getConnectReqInterface()->onUrlActionQueryIdFail( actionInfo.getSessionId(), hostUrl, eStatus, actionInfo.getConnectReason() );
    }

    LogMsg( LOG_ERROR, "ConnectionMgr: query host %s for id failed %s %s",  hostUrl.c_str(),
        DescribeRunTestStatus( eStatus ), DescribeNetCmdError( netCmdError ));
}

//============================================================================
void ConnectionMgr::setQueryIdFailedCount( EHostType hostType, int failedCount )
{
    lockConnectionList();
    auto iter = m_DefaultHostQueryIdFailed.find( hostType );
    if( iter != m_DefaultHostQueryIdFailed.end() )
    {
        iter->second.second = failedCount;
    }

    unlockConnectionList();
}

//============================================================================
int ConnectionMgr::getQueryIdFailedCount( EHostType hostType )
{
    int failedCount = 0;
    lockConnectionList();
    auto iter = m_DefaultHostQueryIdFailed.find( hostType );
    if( iter != m_DefaultHostQueryIdFailed.end() )
    {
        failedCount = iter->second.second;
    }

    unlockConnectionList();
    return failedCount;
}

//============================================================================
EConnectStatus ConnectionMgr::requestConnection( VxGUID& sessionId, std::string url, VxGUID onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectionMgr::requestConnection must have valid online id" );
        return eConnectStatusBadParam;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        LogMsg( LOG_DEBUG, "ConnectionMgr::requestConnection %s Loopback Socket", DescribeConnectReason( connectReason ) );
        retSktBase = m_Engine.getSktLoopback();
        int64_t timeMs = GetGmtTimeMs();
        retSktBase->setLastActiveTimeMs( timeMs );
        retSktBase->setLastSessionTimeMs( timeMs );
        retSktBase->setLastImAliveTimeMs( timeMs );

        if( callback )
        {
            callback->onContactConnected( sessionId, retSktBase, onlineId, connectReason );
        }

        return eConnectStatusReady;
    }

    LogMsg( LOG_DEBUG, "ConnectionMgr::requestConnection %s", DescribeConnectReason( connectReason ) );
    // first see if we already have a connection to the requested onlineId
    VxSktBase* sktBase = nullptr;
    bool isDisconnected = false;

    // see if we already have a connection for a different reason
    bool isOnline = m_Engine.getConnectIdListMgr().isOnline( onlineId );
    if( isOnline )
    {
        sktBase = m_Engine.getConnectIdListMgr().findAnyOnlineConnection( onlineId );
    }

    if( !sktBase )
    {
        lockConnectionList();
        ConnectedInfo* connectInfo = m_AllList.getConnectedInfo( onlineId );
        if( connectInfo )
        {
            sktBase = connectInfo->getSktBase();
            if( sktBase )
            {
                if( sktBase->isConnected() )
                {
                    uint64_t timeNow = GetTimeStampMs();
                    HandshakeInfo shakeInfo( sktBase, sessionId, onlineId, callback, connectReason, timeNow );
                    connectInfo->addConnectReason( shakeInfo );
                    isDisconnected = false;
                }
                else
                {
                    isDisconnected = true;
                }
            }
        }

        unlockConnectionList();
    }

    if( sktBase )
    {
        if( eConnectReasonUnknown == sktBase->getConnectReason() )
        {
            // set the connect reason so is marked as temporary connection and not announced to gui
            sktBase->setConnectReason( connectReason );
        }

        m_Engine.getConnectIdListMgr().addConnectionReason( sktBase->getSocketId(), connectReason );
    }

    if( isDisconnected && sktBase )
    {
        onSktDisconnected( sktBase );
        sktBase = nullptr;
    }
    else if( sktBase && callback )
    {
        callback->onContactConnected( sessionId, sktBase, onlineId, connectReason );
    }

    if( sktBase )
    {
        retSktBase = sktBase;
        return eConnectStatusReady;
    }
    else
    {
        return attemptConnection( sessionId, url, onlineId, callback, retSktBase, connectReason );
    }

    return eConnectStatusUnknown;
}

//============================================================================
EConnectStatus ConnectionMgr::attemptConnection( VxGUID& sessionId, std::string url, VxGUID& onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason )
{
    EConnectStatus connectStatus = eConnectStatusConnecting;
    VxUrl connectUrl( url.c_str() );
    if( onlineId.isVxGUIDValid() && connectUrl.validateUrl( false ) )
    {
        connectStatus = directConnectTo( url, onlineId, callback, retSktBase, sessionId, connectReason );
        if( connectStatus == eConnectStatusConnectSuccess )
        {
            // connected but waiting for PktAnnounce reply
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
void ConnectionMgr::doneWithConnection( VxGUID sessionId, VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    LogModule( eLogHostConnect, LOG_DEBUG, "HostBaseMgr::doneWithConnection %s", DescribeConnectReason( connectReason ));
    m_HandshakeMutex.lock();
    m_HandshakeList.removeHandshakeInfo( sessionId );
    m_HandshakeMutex.unlock();

    bool sktDisconnected{ false };
    VxSktBase* sktBase{ nullptr };
    lockConnectionList();
    ConnectedInfo* connectInfo = m_AllList.getConnectedInfo( onlineId );
    if( connectInfo )
    {
        sktDisconnected = connectInfo->removeConnectReason( sessionId, callback, connectReason, &sktBase );
    }

    unlockConnectionList();
    if( sktBase )
    {
        m_Engine.getConnectIdListMgr().removeConnectionReason( sktBase->getSocketId(), connectReason );
    }

    if( sktDisconnected && sktBase )
    {
        // this has to be done after list is unlocked or mutex delock can occur
        // let the normal socket disconnected code do the work of removing the connection
        sktBase->closeSkt( eSktCloseConnectReasonsEmpty );
    }
}

//============================================================================
void ConnectionMgr::updateUrlCache( std::string& hostUrl, VxGUID& onlineId )
{
    if( !hostUrl.empty() && onlineId.isVxGUIDValid() )
    {
        // there should be only one online id per ip and port however the ip may change
        // only keep the latest url
        m_Engine.getUrlMgr().updateUrlCache( hostUrl, onlineId );
        lockConnectionList();
        for( auto iter = m_UrlCache.begin(); iter != m_UrlCache.end(); ++iter )
        {
            if( iter->second == onlineId )
            {
                if( iter->first == hostUrl )
                {
                    // already in map
                    unlockConnectionList();
                    return;
                }

                m_UrlCache.erase( iter );
                break;
            }
        }

        m_UrlCache[hostUrl] = onlineId;
        unlockConnectionList();
    }
    else
    {
        LogMsg( LOG_ERROR, "ConnectionMgr::updateUrlCache empty url" );
    }
}

//============================================================================
bool ConnectionMgr::urlCacheOnlineIdLookup( std::string& hostUrl, VxGUID& onlineId )
{
    onlineId.clearVxGUID();
    bool foundId = m_Engine.getUrlMgr().lookupOnlineId( hostUrl, onlineId );
    if( foundId )
    {
        LogMsg( LOG_VERBOSE, "ConnectionMgr::urlCacheOnlineIdLookup found online Id in cache url %s", hostUrl.c_str() );
        return foundId;
    }

    // it may be part of the url .. if so no lookup required
    VxUrl testUrl( hostUrl );
    if( testUrl.hasValidOnlineId() )
    {
        onlineId = testUrl.getOnlineId();
        foundId = true;
    }

    if( !foundId )
    {
        // not part of url.. see if is in cache
        lockConnectionList();
        auto iter = m_UrlCache.find( hostUrl );
        if( iter != m_UrlCache.end() )
        {
            onlineId = iter->second;
            foundId = true;
        }

        unlockConnectionList();
    }

    return foundId;
}

//============================================================================
EConnectStatus ConnectionMgr::directConnectTo(  std::string                 url,
                                                VxGUID&                     onlineId,
                                                IConnectRequestCallback*    callback,
                                                VxSktBase*&                 retSktBase,
                                                VxGUID                      sessionId,
                                                EConnectReason              connectReason,
                                                int					        iConnectTimeoutMs )
{
    VxUrl connectUrl( url.c_str() );
    std::string ipAddr = connectUrl.getHostString();
    uint16_t port = connectUrl.getPort();
    if( !ipAddr.empty() && port )
    {
        return directConnectTo( ipAddr, port, onlineId, retSktBase, callback, sessionId, connectReason, iConnectTimeoutMs );
    }
    else
    {
        return eConnectStatusBadParam;
    }
}

//============================================================================-
EConnectStatus ConnectionMgr::directConnectTo(  VxConnectInfo&		        connectInfo,
                                                VxSktBase *&		        ppoRetSkt,		// return pointer to socket if not null
                                                VxGUID                      sessionId, 
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

    return directConnectTo( ipAddr, connectInfo.getOnlinePort(), connectInfo.getMyOnlineId(), ppoRetSkt, callback, sessionId, connectReason, iConnectTimeoutMs );
}

//============================================================================-
EConnectStatus ConnectionMgr::directConnectTo(  std::string                 ipAddr,
                                                uint16_t                    port,
                                                VxGUID                      onlineId,
                                                VxSktBase*&                 retSktBase,
                                                IConnectRequestCallback*    callback,
                                                VxGUID                      sessionId, 
                                                EConnectReason              connectReason,
                                                int					        iConnectTimeoutMs )
{
    EConnectStatus connectStatus = eConnectStatusConnecting;

    lockConnectionList();
    VxSktConnect * sktBase = m_PeerMgr.connectTo(	ipAddr.c_str(),			// remote ip or url 
                                                    port,	                // port to connect to
                                                    iConnectTimeoutMs );	// milli seconds before connect attempt times out
    if( sktBase )
    {
        if( sktBase->getConnectReason() == eConnectReasonUnknown )
        {
            sktBase->setConnectReason( connectReason );
        }

        // LogModule( eLogConnect, LOG_VERBOSE, "NetConnector::directConnectTo: connect SUCCESS to %s:%d", ipAddr.c_str(), port );
        // generate encryption keys

        LogModule( eLogTcpData, LOG_VERBOSE, "NetworkMgr::DirectConnectTo: connect success.. generating tx key %s:%d %s", sktBase->getRemoteIp().c_str(), port, onlineId.toHexString().c_str() );

        GenerateTxConnectionKey( sktBase, sktBase->getRemoteIp(), port, onlineId, m_Engine.getNetworkMgr().getNetworkKey() );

        LogModule( eLogTcpData, LOG_VERBOSE, "NetworkMgr::DirectConnectTo: connect success.. generating rx key" );

        GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_Engine.getNetworkMgr().getNetworkKey() );

        LogModule( eLogTcpData, LOG_VERBOSE, "NetworkMgr::DirectConnectTo: connect success.. sending announce" );

        //LogMsg( LOG_INFO, "sendMyPktAnnounce 2\n" ); 
        if( false == sendMyPktAnnounce( onlineId, sktBase, true, false, false ) )
        {
            LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: connect failed sending announce" );
            unlockConnectionList();
            return eConnectStatusSendPktAnnFailed;
        }

        m_Engine.getConnectIdListMgr().addConnectionReason( sktBase->getSocketId(), connectReason );
        m_HandshakeMutex.lock();
        m_HandshakeList.addHandshake(sktBase, sessionId, onlineId, callback, connectReason);
        m_HandshakeMutex.unlock();
        connectStatus = eConnectStatusHandshaking;
        retSktBase = (VxSktBase *)sktBase;
        
    }
    else
    {
        connectStatus = eConnectStatusConnectFailed;

        //LogMsg( LOG_INFO, "NetConnector::directConnectTo: connect FAIL to %s:%d\n", strIpAddress.c_str(), connectInfo.getOnlinePort() );
        LogModule( eLogConnect, LOG_DEBUG, "ConnectionMgr::DirectConnectTo: failed" );
    }

    LogModule( eLogConnect, LOG_DEBUG, "ConnectionMgr::DirectConnectTo: done" );
    unlockConnectionList();
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
                                        VxGUID&             sessionId,
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

    lockConnectionList();
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
        unlockConnectionList();
        gotConnected = true;
    }
    else
    {
        unlockConnectionList();
        if( connectUsingTcp( connectInfo, ppoRetSkt, sessionId ) )
        {
            gotConnected		= true;
            retIsNewConnection	= true;
        }
    }

    return gotConnected;
}

//============================================================================
bool ConnectionMgr::connectUsingTcp( VxConnectInfo&	connectInfo, VxSktBase *& ppoRetSkt, VxGUID& sessionId )
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
                                                                sessionId,
                                                                LAN_CONNECT_TIMEOUT,
                                                                false, 
                                                                true ) )
        {
#ifdef DEBUG_CONNECTIONS
            connectInfo.getLanIPv4().toStdString( strDirectConnectIp );
            LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d LAN connect to %s ip %s port %d\n",
                sktBase->m_SktNumber,
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
        std::string strMyOnlineId;
        connectInfo.getMyOnlineId(strMyOnlineId);
        return tryIPv6Connect( connectInfo, ppoRetSkt );
    }

    connectInfo.m_DirectConnectId.getIpAddress( strDirectConnectIp );

    //LogMsg( LOG_INFO, "User %s requires proxy? %d",  connectInfo.m_as8OnlineName, requiresRelay );
    if( false == requiresRelay )
    {
#ifdef DEBUG_CONNECTIONS
        LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: Attempting direct connect to %s ip %s port %d",
            m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
            strDirectConnectIp.c_str(),
            connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
        if( eConnectStatusConnectSuccess == directConnectTo( connectInfo, sktBase, sessionId, DIRECT_CONNECT_TIMEOUT ) )
        {
            //LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: success\n" );
            // direct connection success
#ifdef DEBUG_CONNECTIONS
            LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d direct connect to %s ip %s port %d",
                sktBase->m_SktNumber,
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
            LogMsg( LOG_SKT, "directConnectTo: FAIL LAN connect to %s ip %s port %d",
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
                                        bool				requestReverseConnection,
                                        bool				requestSTUN )
{
    m_PktAnn.setAppAliveTimeSec( GetApplicationAliveSec() );
    PktAnnounce pktAnn;
    memcpy( &pktAnn, &m_PktAnn, sizeof(PktAnnounce) );
    pktAnn.setIsPktAnnReplyRequested( requestAnnReply );
    pktAnn.setIsPktAnnRevConnectRequested( requestReverseConnection );
    pktAnn.setIsPktAnnStunRequested( requestSTUN );

    EFriendState eMyFriendshipToHim;
    EFriendState eHisFriendshipToMe;
    m_BigListMgr.getFriendships( destinationId, eMyFriendshipToHim, eHisFriendshipToMe );

    pktAnn.setMyFriendshipToHim( eMyFriendshipToHim );
    pktAnn.setHisFriendshipToMe( eHisFriendshipToMe );

    return txPacket( destinationId, sktBase, &pktAnn );	
}

//============================================================================
bool ConnectionMgr::txPacket(	VxGUID&				destinationId, 
                                VxSktBase *			sktBase, 
                                VxPktHdr *			poPkt )
{
    bool bSendSuccess = false;
    poPkt->setSrcOnlineId( m_PktAnn.getMyOnlineId() );
    vx_assert( poPkt->getPktType() == PKT_TYPE_ANNOUNCE );

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
                LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d not connected\n", sktBase->m_SktNumber );
            }
            else
            {
                LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d has no encryption key\n", sktBase->m_SktNumber );
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
        bigListInfo->setIsConnected( true );
        if( eConnectReasonRandomConnectJoin == connectReason )
        {
            m_Engine.getToGui().toGuiScanResultSuccess( eScanTypeRandomConnect, bigListInfo );
        }
    }
}

//============================================================================
void ConnectionMgr::closeConnection( ESktCloseReason closeReason, VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo )
{
    if( nullptr == poInfo )
    {
        poInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
    }

    if( nullptr == poInfo )
    {
        LogMsg( LOG_ERROR, "Failed to find info for %s\n", onlineId.toHexString().c_str() );
        skt->closeSkt( eSktCloseFindBigInfoFail );
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

        skt->closeSkt( closeReason );
    }
    else
    {
        LogMsg( LOG_ERROR, "Failed to find ConnectedInfo for %s\n", onlineId.toHexString().c_str() );
        skt->closeSkt( eSktCloseFindConnectedInfoFail );
    }
}

//============================================================================
void ConnectionMgr::doStayConnectedThread( void )
{
    int iConnectToIdx							= 0;
    VxMutex * poListMutex						= &m_BigListMgr.m_FriendListMutex;
    std::vector< BigListInfo * >& friendList	= m_BigListMgr.m_FriendList;
    int iSize;
    BigListInfo * poInfo;

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
         LogMsg( LOG_ERROR, "NetConnector::doConnectRequest when not online" );
    }

    P2PConnectList& connectedList = m_Engine.getConnectList();
    connectedList.connectListLock();
    RcConnectInfo *	rcInfo = connectedList.findConnection( connectRequest.getMyOnlineId(), true );
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
