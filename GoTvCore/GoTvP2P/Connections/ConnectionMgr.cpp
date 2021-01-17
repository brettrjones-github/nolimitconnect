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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxUrl.h>

//============================================================================
ConnectionMgr::ConnectionMgr( P2PEngine& engine )
    : m_Engine( engine )
    , m_BigListMgr( engine.getBigListMgr() )
    , m_AllList( engine )
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
        m_Engine.getRunUrlAction().runUrlAction( eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), this, callback, eHostTypeUnknown );
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
EConnectStatus ConnectionMgr::attemptConnection( std::string url, VxGUID onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason )
{
    EConnectStatus connectStatus = eConnectStatusConnecting;

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