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
void ConnectionMgr::setHostOnlineId( EHostType hostType, VxGUID& hostOnlineId )
{
    m_ConnectionMutex.lock();
    m_HostIdList[hostType] = hostOnlineId;
    m_ConnectionMutex.unlock();
}

//============================================================================
bool ConnectionMgr::getHostOnlineId( EHostType hostType, VxGUID& retHostOnlineId )
{
    bool result = false;
    retHostOnlineId.clearVxGUID();

    m_ConnectionMutex.lock();
    auto iter = m_HostIdList.find( hostType );
    if( iter != m_HostIdList.end() )
    {
        retHostOnlineId = iter->second;
        result = true;
    }

    m_ConnectionMutex.unlock();
    return result;
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
void ConnectionMgr::reseHosttUrl( EHostType hostType )
{
    m_HostIdList[hostType] = VxGUID::nullVxGUID();
    m_HostUrlList[hostType] = "";
    m_HostRequiresOnlineId[hostType] = "";
    m_HostQueryIdFailed[hostType] = eRunTestStatusUnknown;
}

//============================================================================
void ConnectionMgr::applyHostUrl( EHostType hostType, std::string& hostUrl )
{
    m_ConnectionMutex.lock();
    m_HostUrlList[hostType] = hostUrl;
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
                m_HostIdList[hostType] = onlineId;    
                m_ConnectionMutex.unlock();
            }       
        }

        if( needOnlineId )
        {
            m_ConnectionMutex.lock();
            m_HostRequiresOnlineId[hostType] = hostUrl;
            m_ConnectionMutex.unlock();

            std::string myUrl = m_Engine.getMyUrl();
            m_Engine.getRunUrlAction().runUrlAction( eNetCmdQueryHostOnlineIdReq, hostUrl.c_str(), myUrl.c_str(), hostType, this );
        }
    }
}

//============================================================================
void ConnectionMgr::callbackQueryIdSuccess( UrlActionInfo& actionInfo, VxGUID onlineId )
{
    m_ConnectionMutex.lock();
    m_HostIdList[actionInfo.getHostType()] = onlineId;
    m_HostRequiresOnlineId[actionInfo.getHostType()] = "";
    std::string hostUrl = m_HostUrlList[actionInfo.getHostType()];
    m_ConnectionMutex.unlock();

    LogMsg( LOG_VERBOSE, "ConnectionMgr: Success query host %s for online id is %s",  hostUrl.c_str(),
        onlineId.toOnlineIdString().c_str());
}

//============================================================================
void ConnectionMgr::callbackActionFailed( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError )
{
    m_ConnectionMutex.lock();
    m_HostQueryIdFailed[actionInfo.getHostType()] = eStatus;
    std::string hostUrl = m_HostUrlList[actionInfo.getHostType()];
    m_ConnectionMutex.unlock();

    m_HostQueryIdFailed[actionInfo.getHostType()] = eStatus;
    LogMsg( LOG_ERROR, "ConnectionMgr: query host %s for id failed %s %s",  hostUrl.c_str(),
        DescribeRunTestStatus( eStatus ), DescribeNetCmdError( netCmdError ));
}

//============================================================================
bool ConnectionMgr::requestHostConnection( EHostType hostType, EPluginType pluginType, EConnectRequestType connectType, IConnectRequestCallback* callback )
{

    return false;
}

//============================================================================
void ConnectionMgr::doneWithHostConnection( EHostType hostType, EPluginType pluginType, EConnectRequestType connectType, IConnectRequestCallback* callback )
{

}