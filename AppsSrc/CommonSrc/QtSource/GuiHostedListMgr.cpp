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

#include "GuiHostedListMgr.h"
#include "GuiHostedListCallback.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include <ptop_src/ptop_engine_src/HostListMgr/HostedInfo.h>
#include <ptop_src/ptop_engine_src/HostListMgr/HostedListMgr.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <PktLib/VxCommon.h>
#include <CoreLib/VxPtopUrl.h>

//============================================================================
GuiHostedListMgr::GuiHostedListMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiHostedListMgr::onAppCommonCreated( void )
{
    m_MyApp.getAppSettings().getFavoriteHostGroupUrl( m_FavoriteHostGroup );

    connect( this, SIGNAL( signalInternalHostedUpdated( HostedInfo* ) ), this, SLOT( slotInternalHostedUpdated( HostedInfo* ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostedRemoved( VxGUID, EHostType ) ), this, SLOT( slotInternalHostedRemoved( VxGUID, EHostType ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostSearchResult( HostedInfo*, VxGUID ) ), this, SLOT( slotInternalHostSearchResult( HostedInfo*, VxGUID ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalHostSearchComplete( EHostType, VxGUID) ), this, SLOT( slotInternalHostSearchComplete( EHostType, VxGUID ) ), Qt::QueuedConnection );


    m_MyApp.getEngine().getHostedListMgr().wantHostedListCallback( dynamic_cast< HostedListCallbackInterface*>(this), true );
}

//============================================================================
bool GuiHostedListMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiHostedListMgr::callbackHostedInfoListUpdated( HostedInfo* hostedInfo )
{
    if( !hostedInfo )
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::callbackHostedAdded null hostedInfo" );
        return;
    }

    emit signalInternalHostedUpdated( new HostedInfo( *hostedInfo ) );
}

//============================================================================
void GuiHostedListMgr::callbackHostedInfoListRemoved( VxGUID& hostOnlineId, EHostType hostType )
{
    emit signalInternalHostedRemoved( hostOnlineId, hostType );
}

//============================================================================
void GuiHostedListMgr::callbackHostedInfoListSearchResult( HostedInfo* hostedInfo, VxGUID& sessionId )
{
    if( hostedInfo && hostedInfo->isHostInviteValid() )
    {
        emit signalInternalHostSearchResult( new HostedInfo( *hostedInfo ), sessionId );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::callbackHostedInfoListSearchResult invalid invite" );
    }
}

//============================================================================
void GuiHostedListMgr::callbackHostedInfoListSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    if( hostType != eHostTypeUnknown )
    {
        emit signalInternalHostSearchComplete( hostType, sessionId );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::callbackHostedInfoListSearchResult invalid host type" );
    }
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedUpdated( HostedInfo* hostedInfo )
{
    if( hostedInfo )
    {
        updateHostedInfo( *hostedInfo );
        delete hostedInfo;
    }
}

//============================================================================
void GuiHostedListMgr::slotInternalHostedRemoved( VxGUID hostOnlineId, EHostType hostType )
{
    HostedId hostedId( hostOnlineId, hostType );
    auto iter = m_HostedList.find( hostedId );
    if( iter != m_HostedList.end() && hostType != eHostTypeUnknown )
    {
        emit signalHostedRemoved( hostOnlineId, hostType );
    }
}

//============================================================================
void GuiHostedListMgr::slotInternalHostSearchComplete( EHostType hostType, VxGUID sessionId )
{
    announceHostedListSearchComplete( hostType, sessionId );
}

//============================================================================
void GuiHostedListMgr::toGuiHostSearchResult( EHostType hostType, VxGUID& sessionId, HostedInfo& hostedInfo )
{
    if( m_MyApp.getEngine().getIgnoreListMgr().isHostIgnored( hostedInfo.getOnlineId() ) )
    {
        LogMsg( LOG_VERBOSE, "GuiHostedListMgr::toGuiHostSearchResult ignored host %s", hostedInfo.getHostTitle().c_str() );
        return;
    }

    if( hostedInfo.isHostInviteValid() )
    {
        HostedInfo* newHostedInfo = new HostedInfo( hostedInfo );
        emit signalInternalHostSearchResult( newHostedInfo, sessionId );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::toGuiHostSearchResult invalid invite" );
    }
}

//============================================================================
void GuiHostedListMgr::toGuiHostSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    emit signalInternalHostSearchComplete( hostType, sessionId );
}

//============================================================================
void GuiHostedListMgr::slotInternalHostSearchResult( HostedInfo* hostedInfo, VxGUID sessionId )
{
    updateHostSearchResult( *hostedInfo, sessionId );
    delete hostedInfo;
}

//============================================================================
GuiHosted* GuiHostedListMgr::findHosted( VxGUID& onlineId, EHostType hostType )
{
    HostedId hostTypeId( onlineId, hostType );
    return findHosted( hostTypeId );
}

//============================================================================
GuiHosted* GuiHostedListMgr::findHosted( HostedId& hostTypeId )
{
    GuiHosted* user = nullptr;
    auto iter = m_HostedList.find( hostTypeId );
    if( iter != m_HostedList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiHostedListMgr::removeHosted( VxGUID& onlineId, EHostType hostType )
{
    HostedId hostTypeId( onlineId, hostType );
    auto iter = m_HostedList.find( hostTypeId );
    if( iter != m_HostedList.end() )
    {
        iter->second->deleteLater();
        m_HostedList.erase( iter );
    }
}

//============================================================================
bool GuiHostedListMgr::isHostedInSession( VxGUID& onlineId )
{
    // TODO ?
    return false;
}

//============================================================================
GuiHosted* GuiHostedListMgr::updateHosted( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::updateHostedOnline invalid param" );
        return nullptr;
    }

    HostedId hostTypeId( hisIdent->getMyOnlineId(), hostType );
    GuiHosted* guiHosted = findHosted( hisIdent->getMyOnlineId(), hostType );
    GuiUser * user = m_MyApp.getUserMgr().updateUser( hisIdent );
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
            m_HostedList[hostTypeId] = guiHosted;
            onHostedAdded( guiHosted );
        }
    }

    return guiHosted;
}

//============================================================================
GuiHosted* GuiHostedListMgr::updateHostedInfo( HostedInfo& hostedInfo )
{
    EHostType hostType = hostedInfo.getHostType();
    HostedId hostTypeId( hostedInfo.getOnlineId(), hostType );

    GuiHosted* guiHosted = findHosted( hostedInfo.getOnlineId(), hostType );
    if( !guiHosted )
    {
        GuiUser* guiUser = m_MyApp.getUserMgr().getUser( hostedInfo.getOnlineId() );
        // make a new one
        guiHosted = new GuiHosted( m_MyApp, guiUser, hostedInfo.getOnlineId(), hostedInfo );

        m_HostedList[hostTypeId] = guiHosted;
        onHostedAdded( guiHosted );
    }
    else
    {
        // make sure is up to date. search results should be the latest info
        guiHosted->setHostType( hostedInfo.getHostType() );
        // skip setIsFavorite.. is probably not set correctly in search result
        bool updated = guiHosted->getJoinedTimestamp() != hostedInfo.getJoinedTimestamp() || guiHosted->getHostInfoTimestamp() != hostedInfo.getHostInfoTimestamp();

        guiHosted->setConnectedTimestamp( hostedInfo.getConnectedTimestamp() );
        guiHosted->setJoinedTimestamp( hostedInfo.getJoinedTimestamp() );
        guiHosted->setHostInfoTimestamp( hostedInfo.getHostInfoTimestamp() );
        guiHosted->setHostInviteUrl( hostedInfo.getHostInviteUrl() );
        guiHosted->setHostTitle( hostedInfo.getHostTitle() );
        guiHosted->setHostDescription( hostedInfo.getHostDescription() );
        if( updated )
        {
            onHostedUpdated( guiHosted );
        }
    }

    return guiHosted;
}

//============================================================================
void GuiHostedListMgr::setHostedOffline( VxGUID& onlineId )
{
    /*
    GuiHosted* guiHosted = findHosted( onlineId );
    if( guiHosted )
    {
        guiHosted->setOnlineStatus( false );
    }*/
}

//============================================================================
void GuiHostedListMgr::onHostedAdded( GuiHosted* guiHosted )
{
    if( isMessengerReady() )
    {
        emit signalHostedRequested( guiHosted );
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
void GuiHostedListMgr::onHostedUpdated( GuiHosted* guiHosted )
{
    if( isMessengerReady() )
    {
        emit signalHostedUpdated( guiHosted );
    }
}

//============================================================================
void GuiHostedListMgr::onMyIdentUpdated( GuiHosted* guiHosted )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( guiHosted );
    }
}

//============================================================================
void GuiHostedListMgr::updateHostSearchResult( HostedInfo& hostedInfo, VxGUID& sessionId )
{
    // hosted info is temporary and will soon be deleted so make copy if required
    if( hostedInfo.isHostInviteValid() )
    {
        HostedId hostedId( hostedInfo.getHostedId() );
        GuiHosted* guiHosted = updateHostedInfo( hostedInfo );
        if( guiHosted )
        {
            announceHostedListSearchResult( hostedId, guiHosted, sessionId );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiHostedListMgr::updateHostSearchResult invalid invite" );
    }
}

//============================================================================
void GuiHostedListMgr::wantHostedListCallbacks( GuiHostedListCallback* client, bool enable )
{
    for( auto iter = m_HostedListClients.begin(); iter != m_HostedListClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_HostedListClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_HostedListClients.push_back( client );
    }
}

//============================================================================
void GuiHostedListMgr::announceHostedListUpdated( HostedId& hostedId, GuiHosted* guiHosted )
{
    for( auto client : m_HostedListClients )
    {
        client->callbackGuiHostedListUpdated( hostedId, guiHosted );
    }
}

//============================================================================
void GuiHostedListMgr::announceHostedListRemoved( HostedId& hostedId )
{
    for( auto client : m_HostedListClients )
    {
        client->callbackGuiHostedListRemoved( hostedId );
    }
}

//============================================================================
void GuiHostedListMgr::announceHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
    for( auto client : m_HostedListClients )
    {
        client->callbackGuiHostedListSearchResult( hostedId, guiHosted, sessionId );
    }
}

//============================================================================
void GuiHostedListMgr::announceHostedListSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    for( auto client : m_HostedListClients )
    {
        client->callbackGuiHostedListSearchComplete( hostType, sessionId );
    }
}

//============================================================================
void GuiHostedListMgr::setJoinOnStartup( std::string& hostUrl, bool enable )
{
    if( !enable && isJoinOnStartup( hostUrl ) )
    {
        m_FavoriteHostGroup = "";
        m_MyApp.getAppSettings().setFavoriteHostGroupUrl( m_FavoriteHostGroup );
    }
    else if( enable && !hostUrl.empty() )
    {
        m_FavoriteHostGroup = hostUrl;
        m_MyApp.getAppSettings().setFavoriteHostGroupUrl( m_FavoriteHostGroup );
    }
}

//============================================================================
bool GuiHostedListMgr::isJoinOnStartup( std::string& hostUrl )
{
    VxPtopUrl nowUrl( m_FavoriteHostGroup );
    VxPtopUrl checkUrl( hostUrl );
    return nowUrl.isValid() && checkUrl.isValid() && nowUrl.getOnlineId() == checkUrl.getOnlineId() && nowUrl.getHostType() == checkUrl.getHostType();
}

//============================================================================
void GuiHostedListMgr::slotNetAvailableStatus( ENetAvailStatus eNetAvailStatus )
{
    if( eNetAvailStatus >= eNetAvailOnlineButNoRelay )
    {
        checkAutoJoinGroupHost();
    }
}

//============================================================================
void GuiHostedListMgr::checkAutoJoinGroupHost( void )
{
    if( !m_AttemptedJoinHostGroup && !m_FavoriteHostGroup.empty() )
    {
        LogMsg( LOG_VERBOSE, "checkAutoJoinGroupHost attempt join %s", m_FavoriteHostGroup.c_str() );

        m_AttemptedJoinHostGroup = true;
        VxGUID sessionId;
        m_MyApp.getFromGuiInterface().fromGuiJoinHost( eHostTypeGroup, sessionId, m_FavoriteHostGroup );
    }
}

//============================================================================
EJoinState GuiHostedListMgr::getHostJoinState( GroupieId& groupieId )
{
    return m_MyApp.getEngine().getHostJoinMgr().getHostJoinState( groupieId );
}
