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
// http://www.nolimitconnect.com
//============================================================================

#include "GuiConnectIdListMgr.h"
#include "GuiConnectIdListCallback.h"

#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/IdentListMgrs/ConnectIdListMgr.h>
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
    connect( this, SIGNAL( signalInternalOnlineStatusChange( VxGUID, bool ) ),              this, SLOT( slotInternalOnlineStatusChange( VxGUID, bool ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalConnectionStatusChange( ConnectId, bool ) ),       this, SLOT( slotInternalConnectionStatusChange( ConnectId, bool ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalConnectionReason( VxGUID, EConnectReason, bool ) ),      this, SLOT( slotInternalConnectionReason( VxGUID, EConnectReason, bool ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalConnectionLost( VxGUID ) ),                        this, SLOT( slotInternalConnectionLost( VxGUID ) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getConnectIdListMgr().wantConnectIdListCallback( this, true );
}

//============================================================================
bool GuiConnectIdListMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiConnectIdListMgr::callbackOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    emit signalInternalOnlineStatusChange( onlineId, isOnline );
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
void GuiConnectIdListMgr::slotInternalOnlineStatusChange( VxGUID onlineId, bool isOnline )
{
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
}

//============================================================================
void GuiConnectIdListMgr::slotInternalConnectionStatusChange( ConnectId connectId, bool isConnected )
{
    if( isConnected )
    {
        if( m_ConnectIdList.find( connectId ) != m_ConnectIdList.end() )
        {
            m_ConnectIdList.insert( connectId );
            onConnectionStatusChange( connectId, isConnected );
        }
    }
    else
    {
        auto iter = m_ConnectIdList.find( connectId );
        if( iter != m_ConnectIdList.end() )
        {
            m_ConnectIdList.erase( iter );
            onConnectionStatusChange( connectId, isConnected );
        }
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
void GuiConnectIdListMgr::onOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    m_MyApp.getUserMgr().checkOnlineStatusChange( onlineId, isOnline );
    announceOnlineStatusChange( onlineId, isOnline );
}

//============================================================================
void GuiConnectIdListMgr::announceOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    if( onlineId.isVxGUIDValid() )
    {
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
void GuiConnectIdListMgr::onConnectionStatusChange( ConnectId& connectId, bool isConnected )
{
    announceConnectionStatusChange( connectId, isConnected );
}

//============================================================================
void GuiConnectIdListMgr::announceConnectionStatusChange( ConnectId& connectId, bool isConnected )
{
    if( connectId.isValid() )
    {
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
bool GuiConnectIdListMgr::isOnline( VxGUID& onlineId )
{
    bool isOnline{ false };
    if( onlineId.isVxGUIDValid() )
    {
        std::map<VxGUID, bool>::iterator iter = m_OnlineList.find( onlineId );
        if( iter != m_OnlineList.end() )
        {
            if( iter->second == true )
            {
                isOnline = true;
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiConnectIdListMgr::announceOnlineStatusChange invalid onlineId" );
    }

    return isOnline;
}
