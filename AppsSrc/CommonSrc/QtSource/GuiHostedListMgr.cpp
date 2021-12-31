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

#include "GuiHostedListMgr.h"
#include "AppCommon.h"
#include <ptop_src/ptop_engine_src/HostListMgr/HostedInfo.h>
#include <ptop_src/ptop_engine_src/HostListMgr/HostedListMgr.h>
#include <PktLib/VxCommon.h>

//============================================================================
GuiHostedListMgr::GuiHostedListMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiHostedListMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalHostedRequested( HostedInfo ) ), this, SLOT( slotInternalHostedRequested( HostedInfo ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostedUpdated( HostedInfo ) ), this, SLOT( slotInternalHostedUpdated( HostedInfo ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostedRemoved( VxGUID, EPluginType ) ), this, SLOT( slotInternalHostedRemoved( VxGUID, EPluginType ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostedOfferState( VxGUID, EPluginType, EJoinState ) ), this, SLOT( slotInternalHostedOfferState( VxGUID, EPluginType, EJoinState ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostedOnlineState( VxGUID, EPluginType, EOnlineState, VxGUID ) ), this, SLOT( slotInternalHostedOnlineState( VxGUID, EPluginType, EOnlineState, VxGUID ) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getHostedListMgr().addHostedListMgrClient( dynamic_cast< HostedListCallbackInterface*>(this), true );
}

//============================================================================
bool GuiHostedListMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiHostedListMgr::callbackHostedRequested( HostedInfo* userJoinInfo )
{
    if( !userJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::callbackHostedAdded null userJoinInfo" );
        return;
    }

    emit signalInternalHostedRequested( new HostedInfo( *userJoinInfo ) );
}

//============================================================================
void GuiHostedListMgr::callbackHostedUpdated( HostedInfo* userJoinInfo )
{
    if( !userJoinInfo )
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::callbackHostedAdded null userJoinInfo" );
        return;
    }

    emit signalInternalHostedUpdated( new HostedInfo(*userJoinInfo) );
}

//============================================================================
void GuiHostedListMgr::callbackHostedRemoved( VxGUID& hostOnlineId, EPluginType pluginType )
{
    emit signalInternalHostedRemoved( hostOnlineId, pluginType );
}

//============================================================================
void GuiHostedListMgr::callbackHostedOfferState( VxGUID& hostOnlineId, EPluginType pluginType, EJoinState userOfferState )
{
    emit signalInternalHostedOfferState( hostOnlineId, pluginType, userOfferState );
}

//============================================================================
void GuiHostedListMgr::callbackHostedOnlineState( VxGUID& hostOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId )
{
    emit signalInternalHostedOnlineState( hostOnlineId, pluginType, onlineState, connectionId );
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedRequested( HostedInfo* userJoinInfo )
{
    updateHosted( userJoinInfo );
    delete userJoinInfo;
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedUpdated( HostedInfo* userJoinInfo )
{
    updateHosted( userJoinInfo );
    delete userJoinInfo;
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedRemoved( VxGUID onlineId, EPluginType pluginType )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    auto iter = m_HostedList.find( onlineId );
    GuiHosted* joinInfo = nullptr;
    if( iter != m_HostedList.end() && hostType != eHostTypeUnknown )
    {
        emit signalHostedRemoved( onlineId, hostType );
        joinInfo = iter->second;
        GuiUser* user = joinInfo->getUser();
        if( user )
        {
            user->removeHostType( hostType );
            if( !user->hostTypeCount() )
            {
                m_HostedList.erase( iter );
                joinInfo->deleteLater();
            }
        }
    }
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedOfferState( VxGUID userOnlineId, EPluginType pluginType, EJoinState joinOfferState )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    GuiHosted* joinInfo = findHosted( userOnlineId );
    if( joinInfo && hostType != eHostTypeUnknown && joinOfferState != eJoinStateNone )
    {
        if( joinInfo->getJoinState( hostType ) != joinOfferState )
        {
            joinInfo->setJoinState( hostType, joinOfferState );
            emit signalHostedOfferStateChange( userOnlineId, hostType, joinOfferState );
        }
    }
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedOnlineState( VxGUID userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID connectionId )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    GuiHosted* joinInfo = findHosted( userOnlineId );
    bool isOnline = onlineState == eOnlineStateOnline ? true : false;
    if( joinInfo && hostType != eHostTypeUnknown && isOnline != joinInfo->isOnline() )
    {
        joinInfo->setOnlineStatus( isOnline );
        emit signalHostedOnlineStatus( joinInfo, isOnline );
    }
}

//============================================================================
GuiHosted* GuiHostedListMgr::findHosted( VxGUID& onlineId )
{
    GuiHosted* user = nullptr;
    auto iter = m_HostedList.find( onlineId );
    if( iter != m_HostedList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiHostedListMgr::removeHosted( VxGUID& onlineId )
{
    auto iter = m_HostedList.find( onlineId );
    if( iter != m_HostedList.end() )
    {
        iter->second->deleteLater();
        m_HostedList.erase( iter );
    }
}

//============================================================================
bool GuiHostedListMgr::isHostedInSession( VxGUID& onlineId )
{
    GuiHosted* guiHosted = findHosted( onlineId );
    return guiHosted && guiHosted->isInSession();
}

//============================================================================
GuiHosted* GuiHostedListMgr::getHosted( VxGUID& onlineId )
{
    GuiHosted* guiHosted = findHosted( onlineId );
    return guiHosted;
}

//============================================================================
GuiHosted* GuiHostedListMgr::updateHosted( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::updateHostedOnline invalid param" );
        return nullptr;
    }

    GuiHosted* guiHosted = findHosted( hisIdent->getMyOnlineId() );
    GuiUser * user = m_MyApp.getUserMgr().updateUser( hisIdent, hostType );
    if( user )
    {
        if( guiHosted && guiHosted->getUser()->getMyOnlineId() == hisIdent->getMyOnlineId() )
        {
            guiHosted->getUser()->addHostType( hostType );
            onHostedUpdated( guiHosted );
        }
        else
        {
            guiHosted = new GuiHosted( m_MyApp );
            guiHosted->setUser( user );
            guiHosted->getUser()->setNetIdent( hisIdent );
            guiHosted->getUser()->addHostType( hostType );
            m_HostedList[guiHosted->getUser()->getMyOnlineId()] = guiHosted;
            onHostedAdded( guiHosted );
        }
    }

    return guiHosted;
}

//============================================================================
GuiHosted* GuiHostedListMgr::updateHosted( HostedInfo* hostJoinInfo )
{
    /*
    EHostType hostType = PluginTypeToHostType( hostJoinInfo->getPluginType() );
    GuiHosted* guiHosted = findHosted( hostJoinInfo->getOnlineId() );
    GuiUser* user = m_MyApp.getUserMgr().updateUser( hostJoinInfo->getNetIdent(), hostType );
    if( user )
    {
        if( guiHosted )
        {
            guiHosted->getUser()->addHostType( hostType );
            guiHosted->setJoinState( hostType, hostJoinInfo->getJoinState() );
            onHostedUpdated( guiHosted );
        }
        else
        {
            guiHosted = new GuiHosted( m_MyApp );
            guiHosted->setUser( user );
            guiHosted->getUser()->setNetIdent( hostJoinInfo->getNetIdent() );
            guiHosted->getUser()->addHostType( hostType );
            guiHosted->setJoinState( hostType, hostJoinInfo->getJoinState() );
            m_HostedList[guiHosted->getUser()->getMyOnlineId()] = guiHosted;
            onHostedAdded( guiHosted );
        }
    }

    return guiHosted;
        */
    return nullptr;
}

//============================================================================
void GuiHostedListMgr::setHostedOffline( VxGUID& onlineId )
{
    GuiHosted* guiHosted = findHosted( onlineId );
    if( guiHosted )
    {
        guiHosted->setOnlineStatus( false );
    }
}

//============================================================================
void GuiHostedListMgr::onHostedAdded( GuiHosted* user )
{
    if( isMessengerReady() )
    {
        emit signalHostedRequested( user );
    }
}

//============================================================================
void GuiHostedListMgr::onHostedRemoved( VxGUID& onlineId, EHostType hostType )
{
    if( isMessengerReady() )
    {
        emit signalHostedRemoved( onlineId, hostType );
    }
}

//============================================================================
void GuiHostedListMgr::onUserOnlineStatusChange( GuiHosted* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalHostedOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiHostedListMgr::onHostedUpdated( GuiHosted* user )
{
    if( isMessengerReady() )
    {
        emit signalHostedUpdated( user );
    }
}

//============================================================================
void GuiHostedListMgr::onMyIdentUpdated( GuiHosted* user )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( user );
    }
}
