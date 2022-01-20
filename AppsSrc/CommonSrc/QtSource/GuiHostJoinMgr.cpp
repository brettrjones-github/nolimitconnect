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

#include "GuiHostJoinMgr.h"
#include "GuiHostJoinCallback.h"
#include "AppCommon.h"
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinInfo.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>

//============================================================================
GuiHostJoinMgr::GuiHostJoinMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiHostJoinMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalHostJoinRequested( HostJoinInfo* ) ),	                                this, SLOT( slotInternalHostJoinRequested( HostJoinInfo* ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinUpdated( HostJoinInfo* ) ),                                    this, SLOT( slotInternalHostJoinUpdated( HostJoinInfo* ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinRemoved( VxGUID, EPluginType ) ),	                            this, SLOT( slotInternalHostJoinRemoved( VxGUID, EPluginType ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinOfferState( VxGUID, EPluginType, EJoinState ) ),               this, SLOT( slotInternalHostJoinOfferState( VxGUID, EPluginType, EJoinState ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinOnlineState( VxGUID, EPluginType, EOnlineState, VxGUID ) ),    this, SLOT( slotInternalHostJoinOnlineState( VxGUID, EPluginType, EOnlineState, VxGUID ) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getHostJoinMgr().addHostJoinMgrClient( this, true );
}

//============================================================================
bool GuiHostJoinMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiHostJoinMgr::onMessengerReady( bool ready )
{
}

//============================================================================
void GuiHostJoinMgr::onSystemReady( bool ready )
{
    if( ready )
    {
        std::vector<HostJoinInfo*> hostJoinList;
        m_MyApp.getEngine().getHostJoinMgr().lockResources();
        m_MyApp.getEngine().getHostJoinMgr().fromGuiGetJoinedStateList( ePluginTypeHostGroup, eJoinStateJoinRequested, hostJoinList );
        for( auto hostJoinInfo : hostJoinList )
        {
            updateHostJoin( hostJoinInfo );
        }

        m_MyApp.getEngine().getHostJoinMgr().unlockResources();
        updateHostRequestCount( true );
    }
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinRequested( HostJoinInfo* hostJoinInfo )
{
    if( !hostJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::callbackHostJoinAdded null hostJoinInfo" );
        return;
    }

    HostJoinInfo* hostJoin = new HostJoinInfo( *hostJoinInfo );

    emit signalInternalHostJoinRequested( hostJoin );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinUpdated( HostJoinInfo* hostJoinInfo )
{
    if( !hostJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::callbackHostJoinAdded null hostJoinInfo" );
        return;
    }

    // there is a possiblility of the hostJoinInfo delete while in signal queue so make copy and delete in slot
    HostJoinInfo* hostJoin = new HostJoinInfo( *hostJoinInfo );

    emit signalInternalHostJoinUpdated( hostJoin );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType )
{
    emit signalInternalHostJoinRemoved( hostOnlineId, pluginType );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinOfferState( VxGUID& hostOnlineId, EPluginType pluginType, EJoinState joinOfferState )
{
    emit signalInternalHostJoinOfferState( hostOnlineId, pluginType, joinOfferState );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinOnlineState( VxGUID& hostOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId )
{
    emit signalInternalHostJoinOnlineState( hostOnlineId, pluginType, onlineState, connectionId );
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinRequested( HostJoinInfo* hostJoinInfo )
{
    updateHostJoin( hostJoinInfo );
    updateHostRequestCount();
    delete hostJoinInfo; // was created new on callback
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinUpdated( HostJoinInfo* hostJoinInfo )
{
    updateHostJoin( hostJoinInfo );
    updateHostRequestCount();
    delete hostJoinInfo;
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinRemoved( VxGUID onlineId, EPluginType pluginType )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    auto iter = m_HostJoinList.find( onlineId );
    GuiHostJoin* joinInfo = nullptr;
    if( iter != m_HostJoinList.end() && hostType != eHostTypeUnknown )
    {
        emit signalHostJoinRemoved( onlineId, hostType );
        joinInfo = iter->second;
        joinInfo->getUser()->removeHostType( hostType );
        if( !joinInfo->getUser()->hostTypeCount() )
        {
            m_HostJoinList.erase( iter );
            joinInfo->deleteLater();
        }
    }

    updateHostRequestCount();
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinOfferState( VxGUID userOnlineId, EPluginType pluginType, EJoinState joinOfferState )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    GuiHostJoin* joinInfo = findHostJoin( userOnlineId );
    if( joinInfo && hostType != eHostTypeUnknown && joinOfferState != eJoinStateNone )
    {
        if( joinInfo->getJoinState( hostType ) != joinOfferState )
        {
            joinInfo->setJoinState( hostType, joinOfferState );
            emit signalHostJoinOfferStateChange( userOnlineId, hostType, joinOfferState );
        }
    }

    updateHostRequestCount();
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinOnlineState( VxGUID userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    GuiHostJoin* joinInfo = findHostJoin( userOnlineId );
    bool isOnline = onlineState == eOnlineStateOnline ? true : false;
    if( joinInfo && hostType != eHostTypeUnknown && isOnline != joinInfo->isOnline() )
    {
        joinInfo->setOnlineStatus( isOnline );
        emit signalHostJoinOnlineStatus( joinInfo, isOnline );
    }

    updateHostRequestCount();
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::findHostJoin( VxGUID& onlineId )
{
    GuiHostJoin* user = nullptr;
    auto iter = m_HostJoinList.find( onlineId );
    if( iter != m_HostJoinList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
bool GuiHostJoinMgr::isHostJoinInSession( VxGUID& onlineId )
{
    GuiHostJoin* guiHostJoin = findHostJoin( onlineId );
    return guiHostJoin && guiHostJoin->isInSession();
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::getHostJoin( VxGUID& onlineId )
{
    return findHostJoin( onlineId );
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::updateHostJoin( HostJoinInfo* hostJoinInfo )
{
    if( !hostJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::updateHostJoin invalid hostJoinInfo param" );
        return nullptr;
    }

    if( !hostJoinInfo->getNetIdent() )
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::updateHostJoin hostJoinInfo does not contain a net ident" );
        return nullptr;
    }

    EHostType hostType = PluginTypeToHostType( hostJoinInfo->getPluginType() );
    GuiHostJoin* guiHostJoin = findHostJoin( hostJoinInfo->getOnlineId() );
    GuiUser* user = m_MyApp.getUserMgr().updateUser( hostJoinInfo->getNetIdent(), hostType );
    if( user )
    {
        if( guiHostJoin )
        {
            guiHostJoin->getUser()->addHostType( hostType );
            guiHostJoin->setJoinState( hostType, hostJoinInfo->getJoinState() );
            onHostJoinUpdated( guiHostJoin );
        }
        else
        {
            guiHostJoin = new GuiHostJoin( m_MyApp );
            guiHostJoin->setUser( user );
            guiHostJoin->getUser()->setNetIdent( hostJoinInfo->getNetIdent() );
            guiHostJoin->getUser()->addHostType( hostType );
            guiHostJoin->setJoinState( hostType, hostJoinInfo->getJoinState() );
            m_HostJoinList[guiHostJoin->getUser()->getMyOnlineId()] = guiHostJoin;
            onHostJoinAdded( guiHostJoin );
        }
    }


    return guiHostJoin;
}

//============================================================================
void GuiHostJoinMgr::setHostJoinOffline( VxGUID& onlineId )
{
    GuiHostJoin* guiHostJoin = findHostJoin( onlineId );
    if( guiHostJoin )
    {
        guiHostJoin->setOnlineStatus( false );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinAdded( GuiHostJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinRequested( user );
    }
}

//============================================================================
void GuiHostJoinMgr::onUserOnlineStatusChange( GuiHostJoin* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinUpdated( GuiHostJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinUpdated( user );
    }
}

//============================================================================
void GuiHostJoinMgr::updateHostRequestCount( bool forceEmit )
{
    int hostRequestCount = 0;
    for( auto& item : m_HostJoinList )
    {
        GuiHostJoin* joinInfo = item.second;
        hostRequestCount += joinInfo->getHostRequestCount();
    }

    if( hostRequestCount != m_HostRequestCount || forceEmit )
    {
        m_HostRequestCount = hostRequestCount;
        emit signalHostJoinRequestCount( m_HostRequestCount );
    }
}

//============================================================================
void GuiHostJoinMgr::joinAccepted( GuiHostJoin* hostJoin, EHostType hostType )
{
    if( hostJoin && hostJoin->setJoinState( hostType, eJoinStateJoinAccepted ) )
    {
        m_MyApp.getEngine().getHostJoinMgr().changeJoinState( hostJoin->getUser()->getMyOnlineId(), HostTypeToHostPlugin( hostType ), eJoinStateJoinAccepted );
        emit signalHostJoinUpdated( hostJoin );
    }
}

//============================================================================
void GuiHostJoinMgr::joinRejected( GuiHostJoin* hostJoin, EHostType hostType )
{
    if( hostJoin && hostJoin->setJoinState( hostType, eJoinStateJoinDenied ) )
    {
        m_MyApp.getEngine().getHostJoinMgr().changeJoinState( hostJoin->getUser()->getMyOnlineId(), HostTypeToHostPlugin( hostType ), eJoinStateJoinDenied );
        emit signalHostJoinUpdated( hostJoin );
    }
}

//============================================================================
void GuiHostJoinMgr::wantHostJoinCallbacks( GuiHostJoinCallback* client, bool enable )
{
    for( auto iter = m_HostJoinClients.begin(); iter != m_HostJoinClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_HostJoinClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_HostJoinClients.push_back( client );
    }
}

//============================================================================
void GuiHostJoinMgr::announceHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinRequested( groupieId, guiHostJoin );
    }
}

//============================================================================
void GuiHostJoinMgr::announceHostJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinGranted( groupieId, guiHostJoin );
    }
}

//============================================================================
void GuiHostJoinMgr::announceHostJoinDenied( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinDenied( groupieId, guiHostJoin );
    }
}

//============================================================================
void GuiHostJoinMgr::announceHostJoinLeaveHost( GroupieId& groupieId )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinLeaveHost( groupieId );
    }
}

//============================================================================
void GuiHostJoinMgr::announceHostJoinRemoved( GroupieId& groupieId )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinRemoved( groupieId );
    }
}
