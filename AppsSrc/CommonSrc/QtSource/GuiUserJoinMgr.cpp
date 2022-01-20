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
    connect( this, SIGNAL( signalInternalUserJoinRemoved( VxGUID, EPluginType ) ), this, SLOT( slotInternalUserJoinRemoved( VxGUID, EPluginType ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinOfferState( VxGUID, EPluginType, EJoinState ) ), this, SLOT( slotInternalUserJoinOfferState( VxGUID, EPluginType, EJoinState ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserJoinOnlineState( VxGUID, EPluginType, EOnlineState, VxGUID ) ), this, SLOT( slotInternalUserJoinOnlineState( VxGUID, EPluginType, EOnlineState, VxGUID ) ), Qt::QueuedConnection );

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
void GuiUserJoinMgr::callbackUserJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType )
{
    emit signalInternalUserJoinRemoved( hostOnlineId, pluginType );
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinOfferState( VxGUID& hostOnlineId, EPluginType pluginType, EJoinState userOfferState )
{
    emit signalInternalUserJoinOfferState( hostOnlineId, pluginType, userOfferState );
}

//============================================================================
void GuiUserJoinMgr::callbackUserJoinOnlineState( VxGUID& hostOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId )
{
    emit signalInternalUserJoinOnlineState( hostOnlineId, pluginType, onlineState, connectionId );
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
void GuiUserJoinMgr::slotInternalUserJoinRemoved( VxGUID onlineId, EPluginType pluginType )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    auto iter = m_UserJoinList.find( onlineId );
    GuiUserJoin* joinInfo = nullptr;
    if( iter != m_UserJoinList.end() && hostType != eHostTypeUnknown )
    {
        emit signalUserJoinRemoved( onlineId, hostType );
        joinInfo = iter->second;
        GuiUser* user = joinInfo->getUser();
        if( user )
        {
            user->removeHostType( hostType );
            if( !user->hostTypeCount() )
            {
                m_UserJoinList.erase( iter );
                joinInfo->deleteLater();
            }
        }
    }
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinOfferState( VxGUID userOnlineId, EPluginType pluginType, EJoinState joinOfferState )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    GuiUserJoin* joinInfo = findUserJoin( userOnlineId );
    if( joinInfo && hostType != eHostTypeUnknown && joinOfferState != eJoinStateNone )
    {
        if( joinInfo->getJoinState( hostType ) != joinOfferState )
        {
            joinInfo->setJoinState( hostType, joinOfferState );
            emit signalUserJoinOfferStateChange( userOnlineId, hostType, joinOfferState );
        }
    }
}

//============================================================================
void GuiUserJoinMgr::slotInternalUserJoinOnlineState( VxGUID userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    GuiUserJoin* joinInfo = findUserJoin( userOnlineId );
    bool isOnline = onlineState == eOnlineStateOnline ? true : false;
    if( joinInfo && hostType != eHostTypeUnknown && isOnline != joinInfo->isOnline() )
    {
        joinInfo->setOnlineStatus( isOnline );
        emit signalUserJoinOnlineStatus( joinInfo, isOnline );
    }
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::findUserJoin( VxGUID& onlineId )
{
    GuiUserJoin* user = nullptr;
    auto iter = m_UserJoinList.find( onlineId );
    if( iter != m_UserJoinList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiUserJoinMgr::removeUserJoin( VxGUID& onlineId )
{
    auto iter = m_UserJoinList.find( onlineId );
    if( iter != m_UserJoinList.end() )
    {
        iter->second->deleteLater();
        m_UserJoinList.erase( iter );
    }
}

//============================================================================
bool GuiUserJoinMgr::isUserJoinInSession( VxGUID& onlineId )
{
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    return guiUserJoin && guiUserJoin->isInSession();
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::getUserJoin( VxGUID& onlineId )
{
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    return guiUserJoin;
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::updateUserJoin( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserJoinMgr::updateUserJoinOnline invalid param" );
        return nullptr;
    }

    GuiUserJoin* guiUserJoin = findUserJoin( hisIdent->getMyOnlineId() );
    GuiUser * user = m_MyApp.getUserMgr().updateUser( hisIdent, hostType );
    if( user )
    {
        if( guiUserJoin && guiUserJoin->getUser()->getMyOnlineId() == hisIdent->getMyOnlineId() )
        {
            guiUserJoin->getUser()->addHostType( hostType );
            onUserJoinUpdated( guiUserJoin );
        }
        else
        {
            guiUserJoin = new GuiUserJoin( m_MyApp );
            guiUserJoin->setUser( user );
            guiUserJoin->getUser()->setNetIdent( hisIdent );
            guiUserJoin->getUser()->addHostType( hostType );
            m_UserJoinList[guiUserJoin->getUser()->getMyOnlineId()] = guiUserJoin;
            onUserJoinAdded( guiUserJoin );
        }
    }

    return guiUserJoin;
}

//============================================================================
GuiUserJoin* GuiUserJoinMgr::updateUserJoin( UserJoinInfo* hostJoinInfo )
{
    EHostType hostType = PluginTypeToHostType( hostJoinInfo->getPluginType() );
    GuiUserJoin* guiUserJoin = findUserJoin( hostJoinInfo->getOnlineId() );
    GuiUser* user = m_MyApp.getUserMgr().updateUser( hostJoinInfo->getNetIdent(), hostType );
    if( user )
    {
        if( guiUserJoin )
        {
            guiUserJoin->getUser()->addHostType( hostType );
            guiUserJoin->setJoinState( hostType, hostJoinInfo->getJoinState() );
            onUserJoinUpdated( guiUserJoin );
        }
        else
        {
            guiUserJoin = new GuiUserJoin( m_MyApp );
            guiUserJoin->setUser( user );
            guiUserJoin->getUser()->setNetIdent( hostJoinInfo->getNetIdent() );
            guiUserJoin->getUser()->addHostType( hostType );
            guiUserJoin->setJoinState( hostType, hostJoinInfo->getJoinState() );
            m_UserJoinList[guiUserJoin->getUser()->getMyOnlineId()] = guiUserJoin;
            onUserJoinAdded( guiUserJoin );
        }
    }

    return guiUserJoin;
}

//============================================================================
void GuiUserJoinMgr::setUserJoinOffline( VxGUID& onlineId )
{
    GuiUserJoin* guiUserJoin = findUserJoin( onlineId );
    if( guiUserJoin )
    {
        guiUserJoin->setOnlineStatus( false );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinAdded( GuiUserJoin* user )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinRequested( user );
    }
}

//============================================================================
void GuiUserJoinMgr::onUserJoinRemoved( VxGUID& onlineId, EHostType hostType )
{
    if( isMessengerReady() )
    {
        emit signalUserJoinRemoved( onlineId, hostType );
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
