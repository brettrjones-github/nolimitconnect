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

#include "GuiUserJoinMgr.h"
#include "GuiUserJoinCallback.h"
#include "AppCommon.h"
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinInfo.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <PktLib/VxCommon.h>

//============================================================================
GuiUserJoinMgr::GuiUserJoinMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiUserJoinMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalUserJoinRequested( UserJoinInfo ) ), this, SLOT( slotInternalUserJoinRequested( UserJoinInfo ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinUpdated( UserJoinInfo ) ), this, SLOT( slotInternalUserJoinUpdated( UserJoinInfo ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinRemoved( GroupieId ) ), this, SLOT( slotInternalUserJoinRemoved( GroupieId ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinOfferState( GroupieId, EJoinState ) ), this, SLOT( slotInternalUserJoinOfferState( GroupieId, EJoinState ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinOnlineState( GroupieId, EOnlineState, VxGUID ) ), this, SLOT( slotInternalUserJoinOnlineState( GroupieId, EOnlineState, VxGUID ) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getUserJoinMgr().addUserJoinMgrClient( this, true );
}

//============================================================================
bool GuiUserJoinMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinAdded( UserJoinInfo* userJoinInfo )
{
    if( !userJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiUserJoinMgr::callbackUserJoinAdded null userJoinInfo" );
        return;
    }

    emit signalInternalUserJoinRequested( new UserJoinInfo( *userJoinInfo ) );
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinUpdated( UserJoinInfo* userJoinInfo )
{
    if( !userJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiUserJoinMgr::callbackUserJoinAdded null userJoinInfo" );
        return;
    }

    emit signalInternalUserJoinUpdated( new UserJoinInfo(*userJoinInfo) );
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinRemoved( GroupieId& groupieId )
{
    emit signalInternalUserJoinRemoved( groupieId );
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinOfferState( GroupieId& groupieId, EJoinState userOfferState )
{
    emit signalInternalUserJoinOfferState( groupieId, userOfferState );
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinOnlineState( GroupieId& groupieId, EOnlineState onlineState, VxGUID& connectionId )
{
    emit signalInternalUserJoinOnlineState( groupieId, onlineState, connectionId );
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinRequested( UserJoinInfo* userJoinInfo )
{
    updateUserJoin( userJoinInfo );
    delete userJoinInfo;
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinUpdated( UserJoinInfo* userJoinInfo )
{
    updateUserJoin( userJoinInfo );
    delete userJoinInfo;
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinRemoved( GroupieId groupieId )
{
    auto iter = m_UserJoinList.find( groupieId );
    GuiUserJoin* guiUserJoin = nullptr;
    if( iter != m_UserJoinList.end() )
    {
        emit signalUserJoinRemoved( groupieId );
        guiUserJoin = iter->second;
        m_UserJoinList.erase( iter );
        guiUserJoin->deleteLater();
    }
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinOfferState( GroupieId groupieId, EJoinState joinOfferState )
{
    GuiUserJoin* guiUserJoin = findUserJoin( groupieId );
    if( guiUserJoin && joinOfferState != eJoinStateNone )
    {
        if( guiUserJoin->setJoinState( joinOfferState ) )
        {
            emit signalUserJoinOfferStateChange( groupieId, joinOfferState );
        }
    }
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinOnlineState( GroupieId groupieId, EOnlineState onlineState, VxGUID connectionId )
{
    GuiUserJoin* guiUserJoin = findUserJoin( groupieId );
    bool isOnline = onlineState == eOnlineStateOnline ? true : false;
    if( guiUserJoin && isOnline != guiUserJoin->isOnline() )
    {
        guiUserJoin->setOnlineStatus( isOnline );
        emit signalUserJoinOnlineStatus( guiUserJoin, isOnline );
    }
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::findUserJoin( GroupieId& groupieId )
{
    GuiUserJoin* user = nullptr;
    auto iter = m_UserJoinList.find( groupieId );
    if( iter != m_UserJoinList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiUserJoinMgr::removeUserJoin( GroupieId& groupieId )
{
    auto iter = m_UserJoinList.find( groupieId );
    if( iter != m_UserJoinList.end() )
    {
        iter->second->deleteLater();
        m_UserJoinList.erase( iter );
    }
}

//============================================================================
bool GuiUserJoinMgr::isUserJoinInSession( GroupieId& groupieId )
{
    GuiUserJoin* guiUserJoin = findUserJoin( groupieId );
    return guiUserJoin && guiUserJoin->isInSession();
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::updateUserJoin( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserJoinMgr::updateUserJoinOnline invalid param" );
        return nullptr;
    }

    VxGUID myOnlineId = m_MyApp.getMyOnlineId();
    GroupieId groupieId( myOnlineId, hisIdent->getMyOnlineId(), hostType );
    GuiUserJoin* guiUserJoin = findUserJoin( groupieId );
    GuiUser * guiUser = m_MyApp.getUserMgr().updateUser( hisIdent );
    if( guiUser )
    {
        if( guiUserJoin )
        {
            onUserJoinUpdated( guiUserJoin );
        }
        else
        {
            guiUserJoin = new GuiUserJoin( m_MyApp );
            guiUserJoin->setGroupieId( groupieId );
            guiUserJoin->setUser( guiUser );
            m_UserJoinList[guiUserJoin->getGroupieId()] = guiUserJoin;
            onUserJoinAdded( guiUserJoin );
        }
    }

    return guiUserJoin;
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::updateUserJoin( UserJoinInfo* userJoinInfo )
{
    GuiUserJoin* guiUserJoin = findUserJoin( userJoinInfo->getGroupieId() );
    GuiUser* user = m_MyApp.getUserMgr().updateUser( userJoinInfo->getNetIdent() );
    if( user )
    {
        if( guiUserJoin )
        {
            guiUserJoin->setJoinState( userJoinInfo->getJoinState() );
            onUserJoinUpdated( guiUserJoin );
        }
        else
        {
            guiUserJoin = new GuiUserJoin( m_MyApp );
            guiUserJoin->setUser( user );
            guiUserJoin->setJoinState( userJoinInfo->getJoinState() );
            m_UserJoinList[guiUserJoin->getGroupieId()] = guiUserJoin;
            onUserJoinAdded( guiUserJoin );
        }
    }

    return guiUserJoin;
}

//============================================================================
void GuiUserJoinMgr::setUserJoinOffline( GroupieId& groupieId )
{
    GuiUserJoin* guiUserJoin = findUserJoin( groupieId );
    if( guiUserJoin )
    {
        guiUserJoin->setOnlineStatus( false );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinAdded( GuiUserJoin* guiUserJoin )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinRequested( guiUserJoin );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinRemoved( GroupieId& groupieId )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinRemoved( groupieId );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserOnlineStatusChange( GuiUserJoin* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinUpdated( GuiUserJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinUpdated( user );
    }
}

//============================================================================
void GuiUserJoinMgr::onMyIdentUpdated( GuiUserJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( user );
    }
}

//============================================================================
void GuiUserJoinMgr::wantUserJoinCallbacks( GuiUserJoinCallback* client, bool enable )
{
    for( auto iter = m_UserJoinClients.begin(); iter != m_UserJoinClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_UserJoinClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_UserJoinClients.push_back( client );
    }
}

//============================================================================
void GuiUserJoinMgr::announceUserJoinRequested( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    for( auto client : m_UserJoinClients )
    {
        client->callbackGuiUserJoinRequested( groupieId, guiUserJoin );
    }
}

//============================================================================
void GuiUserJoinMgr::announceUserJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    for( auto client : m_UserJoinClients )
    {
        client->callbackGuiUserJoinGranted( groupieId, guiUserJoin );
    }
}

//============================================================================
void GuiUserJoinMgr::announceUserJoinDenied( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
    for( auto client : m_UserJoinClients )
    {
        client->callbackGuiUserJoinDenied( groupieId, guiUserJoin );
    }
}

//============================================================================
void GuiUserJoinMgr::announceUserJoinLeaveHost( GroupieId& groupieId )
{
    for( auto client : m_UserJoinClients )
    {
        client->callbackGuiUserJoinLeaveHost( groupieId );
    }
}

//============================================================================
void GuiUserJoinMgr::announceUserJoinRemoved( GroupieId& groupieId )
{
    for( auto client : m_UserJoinClients )
    {
        client->callbackGuiUserJoinRemoved( groupieId );
    }
}
