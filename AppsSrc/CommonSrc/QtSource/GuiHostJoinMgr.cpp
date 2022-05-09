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
    connect( this, SIGNAL( signalInternalHostUnJoin( GroupieId ) ),                                             this, SLOT( slotInternalHostUnJoin( GroupieId ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinRemoved( GroupieId ) ),	                                    this, SLOT( slotInternalHostJoinRemoved( GroupieId ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinOfferState( GroupieId, EJoinState ) ),                         this, SLOT( slotInternalHostJoinOfferState( GroupieId, EJoinState ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostJoinOnlineState( GroupieId, EOnlineState, VxGUID ) ),              this, SLOT( slotInternalHostJoinOnlineState( GroupieId, EOnlineState, VxGUID ) ), Qt::QueuedConnection );

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
void GuiHostJoinMgr::callbackHostUnJoin( GroupieId& groupieId )
{
    emit signalInternalHostUnJoin( groupieId );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinRemoved( GroupieId& groupieId )
{
    emit signalInternalHostJoinRemoved( groupieId );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinOfferState( GroupieId& groupieId, EJoinState joinOfferState )
{
    emit signalInternalHostJoinOfferState( groupieId, joinOfferState );
}

//============================================================================
void GuiHostJoinMgr::callbackHostJoinOnlineState( GroupieId& groupieId, EOnlineState onlineState, VxGUID& connectionId )
{
    emit signalInternalHostJoinOnlineState( groupieId, onlineState, connectionId );
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
void GuiHostJoinMgr::slotInternalHostUnJoin( GroupieId groupieId )
{
    announceHostUnJoin( groupieId );

    auto iter = m_HostJoinList.find( groupieId );
    GuiHostJoin* joinInfo = nullptr;
    if( iter != m_HostJoinList.end() && groupieId.getHostType() != eHostTypeUnknown )
    {
        //emit signalHostJoinRemoved( onlineId, hostType );
        joinInfo = iter->second;
        joinInfo->getUser()->removeHostType( groupieId.getHostType() );
        if( !joinInfo->getUser()->hostTypeCount() )
        {
            m_HostJoinList.erase( iter );
            joinInfo->deleteLater();
        }
    }

    updateHostRequestCount();
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinRemoved( GroupieId groupieId )
{
    auto iter = m_HostJoinList.find( groupieId );
    GuiHostJoin* joinInfo = nullptr;
    if( iter != m_HostJoinList.end() && groupieId.getHostType() != eHostTypeUnknown )
    {
        //emit signalHostJoinRemoved( onlineId, hostType );
        joinInfo = iter->second;
        joinInfo->getUser()->removeHostType( groupieId.getHostType() );
        if( !joinInfo->getUser()->hostTypeCount() )
        {
            m_HostJoinList.erase( iter );
            joinInfo->deleteLater();
        }
    }

    updateHostRequestCount();
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinOfferState( GroupieId groupieId, EJoinState joinOfferState )
{
    GuiHostJoin* guiHostJoin = findHostJoin( groupieId );
    if( guiHostJoin && joinOfferState != eJoinStateNone )
    {
        if( guiHostJoin->getJoinState() != joinOfferState )
        {
            guiHostJoin->setJoinState( joinOfferState );
            emit signalHostJoinOfferStateChange( groupieId, joinOfferState );
        }
    }

    updateHostRequestCount();
}

//============================================================================
void GuiHostJoinMgr::slotInternalHostJoinOnlineState( GroupieId groupieId, EOnlineState onlineState, VxGUID connectionId )
{
    GuiHostJoin* guiHostJoin = findHostJoin( groupieId );
    bool isOnline = onlineState == eOnlineStateOnline ? true : false;
    if( guiHostJoin && isOnline != guiHostJoin->isOnline() )
    {
        guiHostJoin->setHostOnlineStatus( isOnline );
        emit signalHostJoinOnlineStatus( guiHostJoin, isOnline );
    }

    updateHostRequestCount();
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::findHostJoin( GroupieId groupieId )
{
    GuiHostJoin* guiHostJoin = nullptr;
    auto iter = m_HostJoinList.find( groupieId );
    if( iter != m_HostJoinList.end() )
    {
        guiHostJoin = iter->second;
    }

    return guiHostJoin;
}

//============================================================================
bool GuiHostJoinMgr::isHostJoinInSession( GroupieId& groupieId )
{
    GuiHostJoin* guiHostJoin = findHostJoin( groupieId );
    return guiHostJoin && guiHostJoin->isInSession();
}

//============================================================================
GuiHostJoin* GuiHostJoinMgr::getHostJoin( GroupieId& groupieId )
{
    return findHostJoin( groupieId );
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

    GuiHostJoin* guiHostJoin = findHostJoin( hostJoinInfo->getGroupieId() );
    GuiUser* user = m_MyApp.getUserMgr().updateUser( hostJoinInfo->getNetIdent() );
    if( user )
    {
        if( guiHostJoin )
        {
            EJoinState prevState = guiHostJoin->getJoinState();
            guiHostJoin->setJoinState( hostJoinInfo->getJoinState() );
            onHostJoinUpdated( guiHostJoin, prevState );
        }
        else
        {
            guiHostJoin = new GuiHostJoin( m_MyApp );
            guiHostJoin->setGroupieId( hostJoinInfo->getGroupieId() );
            guiHostJoin->setUser( user );
            guiHostJoin->getUser()->setNetIdent( hostJoinInfo->getNetIdent() );
            guiHostJoin->setJoinState( hostJoinInfo->getJoinState() );
            m_HostJoinList[hostJoinInfo->getGroupieId()] = guiHostJoin;
            onHostJoinAdded( guiHostJoin );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::updateHostJoin no user found" );
    }

    return guiHostJoin;
}

//============================================================================
void GuiHostJoinMgr::setHostJoinOffline( GroupieId& groupieId )
{
    GuiHostJoin* guiHostJoin = findHostJoin( groupieId );
    if( guiHostJoin )
    {
        guiHostJoin->setHostOnlineStatus( false );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinAdded( GuiHostJoin* guiHostJoin )
{
    announceJoinState( guiHostJoin, guiHostJoin->getJoinState() );
}

//============================================================================
void GuiHostJoinMgr::onUserOnlineStatusChange( GuiHostJoin* guiHostJoin, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalHostJoinOnlineStatus( guiHostJoin, isOnline );
    }
}

//============================================================================
void GuiHostJoinMgr::onHostJoinUpdated( GuiHostJoin* guiHostJoin, EJoinState prevState )
{
    // BRJ temp always update for testing
    // if( prevState != guiHostJoin->getJoinState() )
    {
        announceJoinState( guiHostJoin, guiHostJoin->getJoinState() );
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
void GuiHostJoinMgr::joinAccepted( GuiHostJoin* guiHostJoin )
{
    if( guiHostJoin && guiHostJoin->setJoinState( eJoinStateJoinIsGranted ) )
    {
        m_MyApp.getEngine().getHostJoinMgr().changeJoinState( guiHostJoin->getGroupieId(), eJoinStateJoinIsGranted );
        announceJoinState( guiHostJoin, eJoinStateJoinIsGranted );
    }
}

//============================================================================
void GuiHostJoinMgr::joinRejected( GuiHostJoin* guiHostJoin )
{
    if( guiHostJoin && guiHostJoin->setJoinState( eJoinStateJoinDenied ) )
    {
        m_MyApp.getEngine().getHostJoinMgr().changeJoinState( guiHostJoin->getGroupieId(), eJoinStateJoinDenied );
        announceJoinState( guiHostJoin, eJoinStateJoinDenied );
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
void GuiHostJoinMgr::announceJoinState( GuiHostJoin* guiHostJoin, EJoinState joinState )
{
    if( guiHostJoin->getJoinState() != joinState )
    {
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::announceJoinState join state %s != param state %s",
                GuiParams::describeJoinState( guiHostJoin->getJoinState() ).toUtf8().constData(),
                GuiParams::describeJoinState( joinState ).toUtf8().constData() );
    }

    switch( joinState )
    {
    case eJoinStateSending:
    case eJoinStateSendFail:
    case eJoinStateSendAcked:
    case eJoinStateJoinRequested:
        announceHostJoinRequested( guiHostJoin->getGroupieId(), guiHostJoin );
        break;
    case eJoinStateJoinWasGranted:
        announceHostJoinWasGranted( guiHostJoin->getGroupieId(), guiHostJoin );
        break;
    case eJoinStateJoinIsGranted:
        announceHostJoinIsGranted( guiHostJoin->getGroupieId(), guiHostJoin );
        break;
    case eJoinStateJoinDenied:
        announceHostJoinDenied( guiHostJoin->getGroupieId(), guiHostJoin );
        break;
    case eJoinStateJoinLeaveHost:
        announceHostJoinLeaveHost( guiHostJoin->getGroupieId() );
        break;
    default:
        LogMsg( LOG_ERROR, "GuiHostJoinMgr::announceJoinState unknown join state %s",
                GuiParams::describeJoinState( joinState ).toUtf8().constData() );
        break;
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
void GuiHostJoinMgr::announceHostJoinWasGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinWasGranted( groupieId, guiHostJoin );
    }
}

//============================================================================
void GuiHostJoinMgr::announceHostJoinIsGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostJoinIsGranted( groupieId, guiHostJoin );
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
void GuiHostJoinMgr::announceHostUnJoin( GroupieId& groupieId )
{
    for( auto client : m_HostJoinClients )
    {
        client->callbackGuiHostUnJoin( groupieId );
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

//============================================================================
EJoinState GuiHostJoinMgr::getHostJoinState( GroupieId& groupieId )
{
    return m_MyApp.getEngine().getHostJoinMgr().getHostJoinState( groupieId );
}
