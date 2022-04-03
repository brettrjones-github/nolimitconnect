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
#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiGroupieListMgr.h"
#include "GuiGroupieListCallback.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/GroupieListMgr/GroupieInfo.h>
#include <ptop_src/ptop_engine_src/GroupieListMgr/GroupieListMgr.h>
#include <PktLib/VxCommon.h>

//============================================================================
GuiGroupieListMgr::GuiGroupieListMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiGroupieListMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalGroupieUpdated( GroupieInfo* ) ), this, SLOT( slotInternalGroupieUpdated( GroupieInfo* ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalGroupieRemoved( VxGUID, VxGUID, EHostType ) ), this, SLOT( slotInternalGroupieRemoved( VxGUID, VxGUID, EHostType ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalGroupieSearchResult( GroupieInfo*, VxGUID) ), this, SLOT( slotInternalGroupieSearchResult( GroupieInfo*, VxGUID ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalGroupieSearchComplete( EHostType, VxGUID ) ), this, SLOT( slotInternalGroupieSearchComplete( EHostType, VxGUID ) ), Qt::QueuedConnection );

    m_MyApp.getEngine().getGroupieListMgr().addGroupieListMgrClient( dynamic_cast< GroupieListCallbackInterface*>(this), true );
}

//============================================================================
bool GuiGroupieListMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiGroupieListMgr::callbackGroupieInfoListUpdated( GroupieInfo* groupieInfo )
{
    if( !groupieInfo )
    {
        LogMsg( LOG_ERROR, "GuiGroupieListMgr::callbackGroupieAdded null groupieInfo" );
        return;
    }

    emit signalInternalGroupieUpdated( new GroupieInfo( *groupieInfo ) );
}

//============================================================================
void GuiGroupieListMgr::callbackGroupieInfoListRemoved( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
    emit signalInternalGroupieRemoved( groupieOnlineId, hostOnlineId, hostType );
}

//============================================================================
void GuiGroupieListMgr::callbackGroupieInfoListSearchResult( GroupieInfo* groupieInfo, VxGUID& sessionId )
{
    if( groupieInfo && groupieInfo->isGroupieValid() )
    {
        emit signalInternalGroupieSearchResult( new GroupieInfo( *groupieInfo ), sessionId );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiGroupieListMgr::callbackGroupieInfoListSearchResult invalid invite" );
    }
}

//============================================================================
void GuiGroupieListMgr::slotInternalGroupieUpdated( GroupieInfo* groupieInfo )
{
    if( groupieInfo )
    {
        updateGroupieInfo( *groupieInfo );
        delete groupieInfo;
    }
}

//============================================================================
void GuiGroupieListMgr::slotInternalGroupieRemoved( VxGUID groupieOnlineId, VxGUID hostOnlineId, EHostType hostType )
{
    GroupieId groupieId( groupieOnlineId, hostOnlineId, hostType );
    auto iter = m_GroupieList.find( groupieId );
    if( iter != m_GroupieList.end() && hostType != eHostTypeUnknown )
    {
        emit signalGroupieRemoved( groupieOnlineId, hostOnlineId, hostType );
    }
}

//============================================================================
void GuiGroupieListMgr::toGuiGroupieSearchResult( EHostType hostType, VxGUID& sessionId, GroupieInfo& groupieInfo )
{
    if( groupieInfo.isGroupieValid() )
    {
        GroupieInfo* newGroupieInfo = new GroupieInfo( groupieInfo );
        emit signalInternalGroupieSearchResult( newGroupieInfo, sessionId );
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiGroupieListMgr::toGuiHostSearchResult invalid invite" );
    }
}

//============================================================================
void GuiGroupieListMgr::toGuiGroupieSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    emit signalInternalGroupieSearchComplete( hostType, sessionId );
}

//============================================================================
void GuiGroupieListMgr::slotInternalGroupieSearchResult( GroupieInfo* groupieInfo, VxGUID sessionId )
{
    updateHostSearchResult( *groupieInfo, sessionId );
    delete groupieInfo;
}

//============================================================================
void GuiGroupieListMgr::slotInternalGroupieSearchComplete( EHostType hostType, VxGUID sessionId )
{
    announceGroupieListSearchComplete( hostType, sessionId );
}

//============================================================================
GuiGroupie* GuiGroupieListMgr::findGroupie( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
    GroupieId groupieId( groupieOnlineId, hostOnlineId, hostType );
    return findGroupie( groupieId );
}

//============================================================================
GuiGroupie* GuiGroupieListMgr::findGroupie( GroupieId& groupieId )
{
    GuiGroupie* user = nullptr;
    auto iter = m_GroupieList.find( groupieId );
    if( iter != m_GroupieList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiGroupieListMgr::removeGroupie( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
    GroupieId groupieId( groupieOnlineId, hostOnlineId, hostType );
    auto iter = m_GroupieList.find( groupieId );
    if( iter != m_GroupieList.end() )
    {
        iter->second->deleteLater();
        m_GroupieList.erase( iter );
    }
}

//============================================================================
bool GuiGroupieListMgr::isGroupieInSession( GroupieId& groupieId )
{
    // TODO ?
    return false;
}

//============================================================================
GuiGroupie* GuiGroupieListMgr::updateGroupie( VxNetIdent* hisIdent, EHostType hostType )
{
    /*
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiGroupieListMgr::updateGroupieOnline invalid param" );
        return nullptr;
    }

    GroupieId groupieId( hisIdent->getMyOnlineId(), hostType );
    GuiGroupie* guiGroupie = findGroupie( hisIdent->getMyOnlineId(), hostType );
    GuiUser * user = m_MyApp.getUserMgr().updateUser( hisIdent, hostType );
    if( user )
    {
        if( guiGroupie && guiGroupie->getUser()->getMyOnlineId() == hisIdent->getMyOnlineId() )
        {
            guiGroupie->getUser()->addHostType( hostType );
            onGroupieUpdated( guiGroupie );
        }
        else
        {
            guiGroupie = new GuiGroupie( m_MyApp );
            guiGroupie->setUser( user );
            guiGroupie->getUser()->setNetIdent( hisIdent );
            guiGroupie->getUser()->addHostType( hostType );
            m_GroupieList[groupieId] = guiGroupie;
            onGroupieAdded( guiGroupie );
        }
    }

    return guiGroupie;
    */
    return nullptr;
}

//============================================================================
GuiGroupie* GuiGroupieListMgr::updateGroupieInfo( GroupieInfo& groupieInfo )
{
    GuiGroupie* guiGroupie = findGroupie( groupieInfo.getGroupieId() );
    if( !guiGroupie )
    {
        GuiUser* guiUser = m_MyApp.getUserMgr().getUser( groupieInfo.getGroupieOnlineId() );
        // make a new one
        guiGroupie = new GuiGroupie( m_MyApp, guiUser, groupieInfo.getGroupieOnlineId(), groupieInfo );

        m_GroupieList[groupieInfo.getGroupieId()] = guiGroupie;
        onGroupieAdded( guiGroupie );
    }
    else
    {
        // make sure is up to date. search results should be the latest info
        guiGroupie->setGroupieInfoTimestamp( groupieInfo.getGroupieInfoTimestamp() );
        // skip setIsFavorite.. is probably not set correctly in search result
        guiGroupie->setGroupieUrl( groupieInfo.getGroupieUrl() );
        guiGroupie->setGroupieTitle( groupieInfo.getGroupieTitle() );
        guiGroupie->setGroupieDescription( groupieInfo.getGroupieDescription() );
    }

    return guiGroupie;
}

//============================================================================
void GuiGroupieListMgr::setGroupieOffline( GroupieId& groupieId )
{
    /*
    GuiGroupie* guiGroupie = findGroupie( onlineId );
    if( guiGroupie )
    {
        guiGroupie->setOnlineStatus( false );
    }*/
}

//============================================================================
void GuiGroupieListMgr::onGroupieAdded( GuiGroupie* guiGroupie )
{
    if( isMessengerReady() )
    {
        emit signalGroupieRequested( guiGroupie );
    }
}

//============================================================================
void GuiGroupieListMgr::onGroupieRemoved( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
    if( isMessengerReady() )
    {
        emit signalGroupieRemoved( groupieOnlineId, hostOnlineId, hostType );
    }
}

//============================================================================
void GuiGroupieListMgr::onUserOnlineStatusChange( GuiGroupie* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalGroupieOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiGroupieListMgr::onGroupieUpdated( GuiGroupie* guiGroupie )
{
    if( isMessengerReady() )
    {
        emit signalGroupieUpdated( guiGroupie );
    }
}

//============================================================================
void GuiGroupieListMgr::onMyIdentUpdated( GuiGroupie* guiGroupie )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( guiGroupie );
    }
}

//============================================================================
void GuiGroupieListMgr::updateHostSearchResult( GroupieInfo& groupieInfo, VxGUID& sessionId )
{
    // hosted info is temporary and will soon be deleted so make copy if required
    if( groupieInfo.isGroupieValid() )
    {
        GuiGroupie* guiGroupie = updateGroupieInfo( groupieInfo );
        if( guiGroupie )
        {
            announceGroupieListSearchResult( groupieInfo.getGroupieId(), guiGroupie, sessionId );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiGroupieListMgr::updateHostSearchResult invalid invite" );
    }
}

//============================================================================
void GuiGroupieListMgr::wantGroupieListCallbacks( GuiGroupieListCallback* client, bool enable )
{
    for( auto iter = m_GroupieListClients.begin(); iter != m_GroupieListClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_GroupieListClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_GroupieListClients.push_back( client );
    }
}

//============================================================================
void GuiGroupieListMgr::announceGroupieListUpdated( GroupieId& groupieId, GuiGroupie* guiGroupie )
{
    for( auto client : m_GroupieListClients )
    {
        client->callbackGuiGroupieListUpdated( groupieId, guiGroupie );
    }
}

//============================================================================
void GuiGroupieListMgr::announceGroupieListRemoved( GroupieId& groupieId )
{
    for( auto client : m_GroupieListClients )
    {
        client->callbackGuiGroupieListRemoved( groupieId );
    }
}

//============================================================================
void GuiGroupieListMgr::announceGroupieListSearchResult( GroupieId& groupieId, GuiGroupie* guiGroupie, VxGUID& sessionId )
{
    for( auto client : m_GroupieListClients )
    {
        client->callbackGuiGroupieListSearchResult( groupieId, guiGroupie, sessionId );
    }
}

//============================================================================
void GuiGroupieListMgr::announceGroupieListSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    for( auto client : m_GroupieListClients )
    {
        client->callbackGuiGroupieListSearchComplete( hostType, sessionId );
    }
}
