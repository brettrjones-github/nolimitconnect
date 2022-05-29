//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "GuiConnectIdListMgr.h"
#include "GuiConnectIdListCallback.h"

#include "AppCommon.h"
#include "GuiUserBase.h"

#include <ptop_src/ptop_engine_src/ConnectIdListMgr/ConnectIdListMgr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
GuiConnectIdListMgr::GuiConnectIdListMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiConnectIdListMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalNearbyStatusChange(VxGUID,int64_t) ),              this, SLOT( slotInternalNearbyStatusChange(VxGUID,int64_t) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalRelayStatusChange( ConnectId,bool) ),              this, SLOT( slotInternalRelayStatusChange( ConnectId,bool) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalOnlineStatusChange(VxGUID,bool) ),                 this, SLOT( slotInternalOnlineStatusChange(VxGUID,bool) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalConnectionStatusChange(ConnectId,bool) ),          this, SLOT( slotInternalConnectionStatusChange(ConnectId,bool) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalConnectionReason(VxGUID,EConnectReason,bool) ),    this, SLOT( slotInternalConnectionReason(VxGUID,EConnectReason,bool) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalConnectionLost(VxGUID) ),                          this, SLOT( slotInternalConnectionLost(VxGUID) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getConnectIdListMgr().wantConnectIdListCallback( this, true );
}

//============================================================================
bool GuiConnectIdListMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiConnectIdListMgr::callbackNearbyStatusChange( VxGUID& onlineId, int64_t nearbyTimeOrZeroIfNot )
{
    emit signalInternalNearbyStatusChange( onlineId, nearbyTimeOrZeroIfNot );
}

//============================================================================
void GuiConnectIdListMgr::callbackRelayStatusChange( ConnectId& connectId, bool isRelayed )
{
    emit signalInternalRelayStatusChange( connectId, isRelayed );
}

//============================================================================
void GuiConnectIdListMgr::callbackConnectionStatusChange( ConnectId& connectId, bool isConnected )
{
    emit signalInternalConnectionStatusChange( connectId, isConnected );
}

//============================================================================
void GuiConnectIdListMgr::callbackConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason, bool enableReason )
{
    emit signalInternalConnectionReason( sktConnectId, connectReason, enableReason );
}

//============================================================================
void GuiConnectIdListMgr::callbackConnectionLost( VxGUID& sktConnectId )
{
    emit signalInternalConnectionLost( sktConnectId );
}

//============================================================================
void GuiConnectIdListMgr::slotInternalNearbyStatusChange( VxGUID onlineId, int64_t nearbyTimeOrZeroIfNot )
{
    LogMsg( LOG_VERBOSE, "GuiConnectIdListMgr::slotInternalNearbyStatusChange nearby time %lld %s", nearbyTimeOrZeroIfNot, m_MyApp.getUserMgr().getUserOnlineName( onlineId ).c_str() );
    auto iter = m_NearbyIdList.find( onlineId );
    if( iter != m_NearbyIdList.end() )
    {
        if( iter->second != nearbyTimeOrZeroIfNot )
        {
            iter->second = nearbyTimeOrZeroIfNot;
            onNearbyStatusChange( onlineId, nearbyTimeOrZeroIfNot );
        }
    }
    else
    {
        m_NearbyIdList[ onlineId ] = nearbyTimeOrZeroIfNot;
        onNearbyStatusChange( onlineId, nearbyTimeOrZeroIfNot );
    }
}

//============================================================================
void GuiConnectIdListMgr::slotInternalRelayStatusChange( ConnectId connectId, bool isNowRelayed )
{
    LogMsg( LOG_VERBOSE, "GuiConnectIdListMgr::slotInternalOnlineStatusChange isRelayed %d %s", isNowRelayed, m_MyApp.getUserMgr().getUserOnlineName( connectId.getGroupieOnlineId() ).c_str() );
    auto iter = m_RelayedIdList.find( connectId );
    if( iter != m_RelayedIdList.end() )
    {
        if( !isNowRelayed )
        {
            m_RelayedIdList.erase( iter );
        }

    }
    else
    {
        if( isNowRelayed )
        {
            m_RelayedIdList.insert( connectId );
        }
    }

    onRelayStatusChange( connectId.getGroupieOnlineId(), isRelayed( connectId.getGroupieOnlineId() ) );
}

//============================================================================
void GuiConnectIdListMgr::slotInternalOnlineStatusChange( VxGUID onlineId, bool isOnline )
{
    /*
    LogMsg( LOG_VERBOSE, "GuiConnectIdListMgr::slotInternalOnlineStatusChange isOnline %d %s ", isOnline,
            m_MyApp.getUserMgr().getUserOnlineName( onlineId ).c_str() );
    auto iter = m_OnlineList.find( onlineId );
    if( iter != m_OnlineList.end() )
    {
        if( iter->second != isOnline )
        {
            iter->second = isOnline;
            onOnlineStatusChange( onlineId, isOnline );
        }
    }
    else
    {
        m_OnlineList[onlineId] = isOnline;
        onOnlineStatusChange( onlineId, isOnline );
    }
    */
}

//============================================================================
void GuiConnectIdListMgr::slotInternalConnectionStatusChange( ConnectId connectId, bool isConnected )
{
    LogMsg( LOG_VERBOSE, "GuiConnectIdListMgr::slotInternalConnectionStatusChange %s isConnect %d to %s", 
        m_MyApp.getUserMgr().getUserOnlineName( connectId.getGroupieOnlineId() ).c_str(), isConnected,
            GuiParams::describeHostType( connectId.getHostType() ).toUtf8().constData() );
    if( isConnected )
    {
        if( m_ConnectIdList.find( connectId ) == m_ConnectIdList.end() )
        {
            m_ConnectIdList.insert( connectId );
        }

        onConnectionStatusChange( connectId, isConnected );
    }
    else
    {
        auto iter = m_ConnectIdList.find( connectId );
        if( iter != m_ConnectIdList.end() )
        {
            m_ConnectIdList.erase( iter );
        }

        onConnectionStatusChange( connectId, isConnected );
    }
}

//============================================================================
void GuiConnectIdListMgr::slotInternalConnectionReason( VxGUID socketId, EConnectReason connectReason, bool enableReason )
{
    auto iter = m_ConnectReasonList.find( socketId );
    if( iter != m_ConnectReasonList.end() )
    {
        if( enableReason )
        {
            iter->second.insert( connectReason );
        }
        else
        {
            auto iterReason = iter->second.find( connectReason );
            if( iterReason != iter->second.end() )
            {
                iter->second.erase( iterReason );
            }
        }
    }
    else
    {
        std::set<EConnectReason> reasonSet{ connectReason };
        m_ConnectReasonList[socketId] = reasonSet;
    }
}

//============================================================================
void GuiConnectIdListMgr::slotInternalConnectionLost( VxGUID socketId )
{
    auto iter = m_ConnectReasonList.find( socketId );
    if( iter != m_ConnectReasonList.end() )
    {
        m_ConnectReasonList.erase( iter );
    }
}

//============================================================================
void GuiConnectIdListMgr::onNearbyStatusChange( VxGUID& onlineId, int64_t nearbyTimeOrZeroIfNot )
{
    announceNearbyStatusChange( onlineId, nearbyTimeOrZeroIfNot );
}

//============================================================================
void GuiConnectIdListMgr::onRelayStatusChange( VxGUID& onlineId, bool isRelayed )
{
    announceRelayStatusChange( onlineId, isRelayed );
}

//============================================================================
void GuiConnectIdListMgr::onOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    announceOnlineStatusChange( onlineId, isOnline );
}

//============================================================================
void GuiConnectIdListMgr::announceOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    if( onlineId.isVxGUIDValid() )
    {
        m_MyApp.getUserMgr().connnectIdOnlineStatusChange( onlineId );
        for( auto iter = m_GuiConnectIdClientList.begin(); iter != m_GuiConnectIdClientList.end(); ++iter )
        {
            GuiConnectIdListCallback* client = *iter;
            if( client )
            {
                client->callbackOnlineStatusChange( onlineId, isOnline );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceOnlineStatusChange invalid callback" );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceOnlineStatusChange invalid onlineId" );
    }
}

//============================================================================
void GuiConnectIdListMgr::announceNearbyStatusChange( VxGUID& onlineId, int64_t nearbyTimeOrZeroIfNot )
{
    if( onlineId.isVxGUIDValid() )
    {
        m_MyApp.getUserMgr().connnectIdNearbyStatusChange( onlineId, nearbyTimeOrZeroIfNot );
        for( auto iter = m_GuiConnectIdClientList.begin(); iter != m_GuiConnectIdClientList.end(); ++iter )
        {
            GuiConnectIdListCallback* client = *iter;
            if( client )
            {
                client->callbackNearbyStatusChange( onlineId, nearbyTimeOrZeroIfNot );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceNearbyStatusChange invalid callback" );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceNearbyStatusChange invalid onlineId" );
    }
}

//============================================================================
void GuiConnectIdListMgr::announceRelayStatusChange( VxGUID& onlineId, bool isRelayed )
{
    if( onlineId.isVxGUIDValid() )
    {
        m_MyApp.getUserMgr().connnectIdRelayStatusChange( onlineId );
        for( auto iter = m_GuiConnectIdClientList.begin(); iter != m_GuiConnectIdClientList.end(); ++iter )
        {
            GuiConnectIdListCallback* client = *iter;
            if( client )
            {
                client->callbackRelayStatusChange( onlineId, isRelayed );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceRelayStatusChange invalid callback" );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceRelayStatusChange invalid onlineId" );
    }
}

//============================================================================
void GuiConnectIdListMgr::onConnectionStatusChange( ConnectId& connectId, bool isConnected )
{
    announceConnectionStatusChange( connectId, isConnected );
}

//============================================================================
void GuiConnectIdListMgr::announceConnectionStatusChange( ConnectId& connectId, bool isConnected )
{
    if( connectId.isValid() )
    {
        m_MyApp.getUserMgr().connnectIdOnlineStatusChange( connectId.getGroupieId().getGroupieOnlineId() );
        for( auto iter = m_GuiConnectIdClientList.begin(); iter != m_GuiConnectIdClientList.end(); ++iter )
        {
            GuiConnectIdListCallback* client = *iter;
            if( client )
            {
                client->callbackConnectionStatusChange( connectId, isConnected );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceConnectionStatusChange invalid callback" );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceConnectionStatusChange invalid connectId" );
    }

}

//============================================================================
void GuiConnectIdListMgr::wantGuiConnectIdCallbacks( GuiConnectIdListCallback* callback, bool wantCallback )
{
    static bool userCallbackShutdownComplete = false;
    if( VxIsAppShuttingDown() )
    {
        if( userCallbackShutdownComplete )
        {
            return;
        }

        userCallbackShutdownComplete = true;
        m_GuiConnectIdClientList.clear();
        return;
    }

    if( wantCallback )
    {
        for( auto iter = m_GuiConnectIdClientList.begin(); iter != m_GuiConnectIdClientList.end(); ++iter )
        {
            GuiConnectIdListCallback* client = *iter;
            if( client == callback )
            {
                LogMsg( LOG_INFO, "WARNING. Ignoring New wantGuiConnectIdCallbacks because already in list" );
                return;
            }
        }

        m_GuiConnectIdClientList.push_back( callback );
        return;
    }

    for( auto iter = m_GuiConnectIdClientList.begin(); iter != m_GuiConnectIdClientList.end(); ++iter )
    {
        GuiConnectIdListCallback* client = *iter;
        if( client == callback )
        {
            m_GuiConnectIdClientList.erase( iter );
            return;
        }
    }

    LogMsg( LOG_INFO, "WARNING. wantGuiConnectIdCallbacks remove not found in list" );
    return;
}

//============================================================================
bool GuiConnectIdListMgr::isDirectConnect( VxGUID& onlineId )
{
    bool isDirectConnect{ false };
    if( onlineId.isVxGUIDValid() )
    {
        for( auto &connectIdIn : m_ConnectIdList )
        {
            ConnectId& connectId = const_cast<ConnectId&>(connectIdIn);
            if( connectId.getGroupieOnlineId() == onlineId && eHostTypePeerUserDirect == connectId.getHostType() )
            {
                isDirectConnect = true;
                break;
            }
        }

        if( isDirectConnect )
        {
            LogMsg( LOG_VERBOSE, "IS direct connect %s", m_MyApp.getUserMgr().getUserOnlineName( onlineId ).c_str() );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "Is NOT direct connect %s", m_MyApp.getUserMgr().getUserOnlineName( onlineId ).c_str() );
        }
    }

    return isDirectConnect;
}

//============================================================================
bool GuiConnectIdListMgr::isRelayed( VxGUID& onlineId )
{
    bool isRelayed{ false };
    if( onlineId.isVxGUIDValid() )
    {
        for( auto &connectIdIn : m_RelayedIdList )
        {
            ConnectId& connectId = const_cast<ConnectId&>(connectIdIn);
            if( connectId.getGroupieOnlineId() == onlineId )
            {
                isRelayed = true;
                break;
            }
        }

        if( isRelayed )
        {
            LogMsg( LOG_VERBOSE, "IS relayed %s", m_MyApp.getUserMgr().getUserOnlineName( onlineId ).c_str() );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "Is NOT relayed %s", m_MyApp.getUserMgr().getUserOnlineName( onlineId ).c_str() );
        }
    }

    return isRelayed;
}

//============================================================================
bool GuiConnectIdListMgr::isNearby( VxGUID& onlineId )
{
    int64_t lastNearbyTime = isNearbyTime( onlineId );
    return lastNearbyTime && GetGmtTimeMs() - lastNearbyTime < GuiUserBase::NEARBY_TIMEOUT_MS;
}

//============================================================================
int64_t GuiConnectIdListMgr::isNearbyTime( VxGUID& onlineId )
{
    if( onlineId.isVxGUIDValid() )
    {
        auto iter = m_NearbyIdList.find( onlineId );
        if( iter != m_NearbyIdList.end() )
        {
            return iter->second;
        }
    }

    return 0;
}
