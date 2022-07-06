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

#include "GuiUserMgr.h"
#include "AppCommon.h"
#include "AccountMgr.h"
#include "GuiUserUpdateCallback.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
GuiUserMgr::GuiUserMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiUserMgr::onAppCommonCreated( void )
{
    connect( this, SIGNAL( signalInternalIndentListUpdate(EUserViewType,VxGUID,uint64_t) ),     this, SLOT( slotInternalIndentListUpdate(EUserViewType,VxGUID,uint64_t) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalIndentListRemove(EUserViewType,VxGUID) ),              this, SLOT( slotInternalIndentListRemove(EUserViewType,VxGUID) ), Qt::QueuedConnection );

    connect( this, SIGNAL( signalInternalUpdateUser(VxNetIdent*) ),                             this, SLOT( slotInternalUpdateUser(VxNetIdent*) ), Qt::QueuedConnection);
    connect( this, SIGNAL( signalInternalUserRemoved(VxGUID) ),	                                this, SLOT( slotInternalUserRemoved(VxGUID) ), Qt::QueuedConnection);
    connect( this, SIGNAL( signalInternalUserOnlineStatus(VxNetIdent*,bool) ),                  this, SLOT( slotInternalUserOnlineStatus(VxNetIdent*,bool) ), Qt::QueuedConnection);
    connect( this, SIGNAL( signalInternalSaveMyIdent(VxNetIdent*) ),                            this, SLOT( slotInternalSaveMyIdent(VxNetIdent*) ), Qt::QueuedConnection);

    m_MyApp.wantToGuiUserUpdateCallbacks( this, true );
}

//============================================================================
GuiUser* GuiUserMgr::getMyIdent( void )
{
    if( !m_MyIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserMgr::getMyIdent null m_MyIdent");
        return nullptr;
    }

    if( !m_MyIdent->getNetIdent().isValidNetIdent() )
    {
        if( !m_MyApp.isMessengerReady() )
        {
            LogMsg( LOG_ERROR, "GuiUserMgr::getMyIdent called before ready");
        }

        m_MyIdent->setNetIdent( m_MyApp.getMyIdentity() );
        if( !m_MyIdent->getNetIdent().isValidNetIdent() )
        {
            LogMsg( LOG_ERROR, "GuiUserMgr::getMyIdent called but net ident is invalid");
            return nullptr;
        }
    }

    return m_MyIdent;
}

//============================================================================
bool GuiUserMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiUserMgr::toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiSaveMyIdent invalid onlineId" );
        return;
    }

    emit signalInternalIndentListUpdate( listType, onlineId, timestamp );
}

//============================================================================
void GuiUserMgr::toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiIndentListRemove invalid onlineId" );
        return;
    }


    emit signalInternalIndentListRemove( listType, onlineId );
}

//============================================================================
void GuiUserMgr::toGuiContactAdded( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "GuiUserMgr::toGuiContactAdded invalid netIdent" );
        return;
    }

    updateUser( netIdent );
}

//============================================================================
void GuiUserMgr::toGuiContactRemoved( VxGUID& onlineId )
{
    emit signalInternalUserRemoved( onlineId );
}

//============================================================================
void GuiUserMgr::toGuiContactOnline( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOnline invalid netIdent" );
        return;
    }

    updateOnlineStatus( netIdent, true );
}

//============================================================================
void GuiUserMgr::toGuiContactOffline( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOffline invalid netIdent" );
        return;
    }

    updateOnlineStatus( netIdent, false );
}

//============================================================================
void GuiUserMgr::toGuiContactOffline( VxGUID& onlineId )
{

}

//============================================================================
void GuiUserMgr::toGuiContactNameChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactNameChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiContactDescChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactDescChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactMyFriendshipChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactHisFriendshipChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiPluginPermissionChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactSearchFlagsChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactLastSessionTimeChange invalid netIdent" );
        return;
    }

    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiUpdateMyIdent( VxNetIdent* netIdent )
{
    // NOT sure we really need this
    // emit signalInternalSaveMyIdent( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::toGuiSaveMyIdent( VxNetIdent * netIdent )
{
    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiSaveMyIdent invalid netIdent" );
        return;
    }

    emit signalInternalSaveMyIdent( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::slotInternalIndentListUpdate( EUserViewType listType, VxGUID onlineId, uint64_t timestamp )
{
    GuiUser* user = getOrQueryUser( onlineId );
    if( user )
    {
        switch( listType )
        {
        case eUserViewTypeDirectConnect:
            if( !user->getNetIdent().isDirectConnected() )
            {
                LogMsg( LOG_VERBOSE, "GuiUserMgr::slotInternalIndentListUpdate is direct connect %s", user->getNetIdent().getOnlineName() );
            }
            break;
        case eUserViewTypeNearby:
            if( !user->getNetIdent().isNearby() )
            {
                user->getNetIdent().setIsNearby( true );
                LogMsg( LOG_VERBOSE, "GuiUserMgr::slotInternalIndentListUpdate is nearby %s", user->getNetIdent().getOnlineName() );
            }
            break;
        case eUserViewTypeOnline:
            if( !user->getNetIdent().isOnline() )
            {
                LogMsg( LOG_VERBOSE, "GuiUserMgr::slotInternalIndentListUpdate is NOT online %s", user->getNetIdent().getOnlineName() );
            }
            break;

        default:
            break;
        }

        user->setLastUpdateTime( timestamp );
        if( isMessengerReady() && onlineId.isVxGUIDValid() )
        {
            guiUserUpdateClientsLock();
            for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
            {
                GuiUserUpdateCallback* client = *iter;
                if( client )
                {
                    client->callbackIndentListUpdate( listType, onlineId, timestamp );
                    client->callbackOnUserUpdated( user );
                }
                else
                {
                    LogMsg( LOG_ERROR, "GuiUserMgr::slotInternalIndentListUpdate invalid callback" );
                }
            }

            guiUserUpdateClientsUnlock();
        }
    }
}

//============================================================================
void GuiUserMgr::slotInternalIndentListRemove( EUserViewType listType, VxGUID onlineId )
{
    GuiUser* user = getOrQueryUser( onlineId );
    if( user )
    {
        switch( listType )
        {
        case eUserViewTypeDirectConnect:
            if( user->getNetIdent().isDirectConnected() )
            {
                LogMsg( LOG_VERBOSE, "GuiUserMgr::slotInternalIndentListRemove Was direct connect %s", user->getNetIdent().getOnlineName() );
            }
            break;

        case eUserViewTypeNearby:
            if( user->getNetIdent().isNearby() )
            {
                user->getNetIdent().setIsNearby( false );
                LogMsg( LOG_VERBOSE, "GuiUserMgr::slotInternalIndentListRemove Was nearby %s", user->getNetIdent().getOnlineName() );
            }
            break;

        case eUserViewTypeOnline:
            if( user->getNetIdent().isOnline() )
            {
                LogMsg( LOG_VERBOSE, "GuiUserMgr::slotInternalIndentListRemove Was online %s", user->getNetIdent().getOnlineName() );
            }
            break;

        default:
            break;
        }

        if( isMessengerReady() && onlineId.isVxGUIDValid() )
        {
            guiUserUpdateClientsLock();
            for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
            {
                GuiUserUpdateCallback* client = *iter;
                if( client )
                {
                    client->callbackIndentListRemove( listType, onlineId );
                    client->callbackOnUserUpdated( user );
                }
                else
                {
                    LogMsg( LOG_ERROR, "GuiUserMgr::slotInternalIndentListRemove invalid callback" );
                }
            }

            guiUserUpdateClientsUnlock();
        }
    }
}

//============================================================================
void GuiUserMgr::slotInternalUpdateUser( VxNetIdent* netIdent )
{
    updateUser( netIdent );
    delete netIdent;
}

//============================================================================
void GuiUserMgr::slotInternalSaveMyIdent( VxNetIdent* myIdent )
{
    memcpy( m_MyApp.getAppGlobals().getUserIdent(), myIdent, sizeof( VxNetIdent ) );
    m_MyApp.getAccountMgr().updateAccount( *myIdent );
    delete myIdent;
}

//============================================================================
void GuiUserMgr::slotInternalUserRemoved( VxGUID onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        onUserRemoved( onlineId );
    }

    removeUser( onlineId );
}

//============================================================================
void GuiUserMgr::updateOnlineStatus( VxNetIdent* netIdent, bool online )
{
    updateUser( netIdent );
}

//============================================================================
GuiUser* GuiUserMgr::findUser( VxGUID& onlineId )
{
    GuiUser* user = nullptr;
    auto iter = m_UserList.find( onlineId );
    if( iter != m_UserList.end() )
    {
        user = iter->second;
    }

    return user;
}

//============================================================================
void GuiUserMgr::removeUser( VxGUID& onlineId )
{
    auto iter = m_UserList.find( onlineId );
    if( iter != m_UserList.end() )
    {
        iter->second->deleteLater();
        m_UserList.erase( iter );
    }
}

//============================================================================
bool GuiUserMgr::isUserRelayed( VxGUID& onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        guiUser->updateIsRelayed();
        return guiUser->isRelayed();

    }

    return false;
}

//============================================================================
bool GuiUserMgr::isUserOnline( VxGUID& onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        guiUser->updateIsOnline();
        return guiUser->isOnline();

    }

    return false;
}

//============================================================================
bool GuiUserMgr::isUserInSession( VxGUID& onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    return guiUser && guiUser->isInSession();
}

//============================================================================
GuiUser* GuiUserMgr::getUser( VxGUID& onlineId )
{
    GuiUser* guiUser = nullptr;
    if( onlineId == getMyOnlineId() )
    {
        LogMsg( LOG_WARNING, "GuiUserMgr::getUser getting my ident" );
        guiUser = getMyIdent();
    }
    else
    {
        guiUser = findUser( onlineId );
    }

    return guiUser;
}

//============================================================================
std::string GuiUserMgr::getUserOnlineName( VxGUID& onlineId )
{
    GuiUser* guiUser = getUser( onlineId );
    if( guiUser )
    {
        return guiUser->getOnlineName();
    }
    else
    {
        std::string onlineName( "Unknown User: " );
        onlineName += onlineId.toOnlineIdString();
        return onlineName;
    }
}

//============================================================================
GuiUser* GuiUserMgr::getOrQueryUser( VxGUID& onlineId )
{
    GuiUser* guiUser = getUser( onlineId );
    if( !guiUser )
    {
        VxNetIdent userIdent;
        if( m_MyApp.getEngine().getBigListMgr().queryIdent( onlineId, userIdent ) )
        {
            guiUser = updateUser( &userIdent );
        }
    }

    return guiUser;
}

//============================================================================
GuiUser* GuiUserMgr::updateUser( VxNetIdent* hisIdent )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserMgr::updateUserOnline invalid param" );
        return nullptr;
    }

    if( hisIdent->getMyOnlineId() == m_MyApp.getMyOnlineId() )
    {
        return updateMyIdent( hisIdent );
    }
    else
    {
        GuiUser* guiUser = findUser( hisIdent->getMyOnlineId() );
        if( guiUser && guiUser->getMyOnlineId() == hisIdent->getMyOnlineId() )
        {
            guiUser->updateIsOnline();
            onUserUpdated( guiUser );
        }
        else
        {
            guiUser = new GuiUser( m_MyApp );
            guiUser->setNetIdent( hisIdent );
            m_UserList[guiUser->getMyOnlineId()] = guiUser;
            guiUser->updateIsOnline();
            onUserAdded( guiUser );
        }

        return guiUser;
    }
}

//============================================================================
GuiUser* GuiUserMgr::updateMyIdent( VxNetIdent* myIdent )
{
    if( !m_MyIdent )
    {
        GuiUser* guiUser = new GuiUser( m_MyApp );
        guiUser->setNetIdent( myIdent );
        m_MyIdent = guiUser;
        m_MyOnlineId = m_MyIdent->getMyOnlineId();
    }
    else
    {
        m_MyIdent->setNetIdent( myIdent );
        m_MyOnlineId = m_MyIdent->getMyOnlineId();
    }

    emit signalMyIdentUpdated( m_MyIdent );
    return m_MyIdent;
}

//============================================================================
void GuiUserMgr::setUserOffline( VxGUID& onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        guiUser->updateIsOnline();
    }
}

//============================================================================
void GuiUserMgr::onUserAdded( GuiUser* guiUser )
{
    if( isMessengerReady() && guiUser )
    {
        emit signalUserAdded( guiUser );
        guiUserUpdateClientsLock();
        for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
        {
            GuiUserUpdateCallback* client = *iter;
            if( client )
            {
                client->callbackOnUserAdded( guiUser );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserMgr::onUserAdded invalid callback" );
            }
        }

        guiUserUpdateClientsUnlock();
    }
}

//============================================================================
void GuiUserMgr::onUserRemoved( VxGUID& onlineId )
{
    if( isMessengerReady() )
    {
        emit signalUserRemoved( onlineId );
        guiUserUpdateClientsLock();
        for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
        {
            GuiUserUpdateCallback* client = *iter;
            if( client )
            {
                client->callbackOnUserRemoved( onlineId );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserMgr::onUserRemoved invalid callback" );
            }
        }

        guiUserUpdateClientsUnlock();
    }
}

//============================================================================
void GuiUserMgr::onUserNearbyStatusChange( GuiUser* user )
{
    if( isMessengerReady() )
    {
        sendUserUpdatedToCallbacks( user );
    }
}

//============================================================================
void GuiUserMgr::onUserRelayStatusChange( GuiUser* user )
{
    if( isMessengerReady() )
    {
        user->updateIsOnline();
        emit signalUserOnlineStatus( user );
        sendUserUpdatedToCallbacks( user );
    }
}

//============================================================================
void GuiUserMgr::onUserOnlineStatusChange( GuiUser* user )
{
    if( isMessengerReady() )
    {
        user->updateIsOnline();
        emit signalUserOnlineStatus( user );
        sendUserUpdatedToCallbacks( user );
    }
}

//============================================================================
void GuiUserMgr::onUserUpdated( GuiUser* user )
{
    if( isMessengerReady() )
    {
        emit signalUserUpdated( user );
        sendUserUpdatedToCallbacks( user );
    }
}

//============================================================================
void GuiUserMgr::onMyIdentUpdated( GuiUser* user )
{
    if( isMessengerReady() )
    {
        emit signalMyIdentUpdated( user );
    }
}

//============================================================================
void GuiUserMgr::guiUserUpdateClientsLock( void )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    m_GuiUserUpdateClientMutex.lock();
}

//============================================================================
void GuiUserMgr::guiUserUpdateClientsUnlock( void )
{
    m_GuiUserUpdateClientMutex.unlock();
}

//============================================================================
void GuiUserMgr::wantGuiUserUpdateCallbacks( GuiUserUpdateCallback* callback, bool wantCallback )
{
    static bool userCallbackShutdownComplete = false;
    if( VxIsAppShuttingDown() )
    {
        if( userCallbackShutdownComplete )
        {
            return;
        }

        userCallbackShutdownComplete = true;
        clearGuiUserUpdateClientList();
        return;
    }

    guiUserUpdateClientsLock();

    if( wantCallback )
    {
        for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
        {
            GuiUserUpdateCallback* client = *iter;
            if( client == callback )
            {
                LogMsg( LOG_INFO, "WARNING. Ignoring New wantToGuiUserUpdateCallbacks because already in list" );
                guiUserUpdateClientsUnlock();
                return;
            }
        }

        m_GuiUserUpdateClientList.push_back( callback );
        guiUserUpdateClientsUnlock();
        return;
    }

    for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
    {
        GuiUserUpdateCallback* client = *iter;
        if( client == callback )
        {
            m_GuiUserUpdateClientList.erase( iter );
            guiUserUpdateClientsUnlock();
            return;
        }
    }

    LogMsg( LOG_INFO, "WARNING. ToGuiUserUpdateInterface remove not found in list" );
    guiUserUpdateClientsUnlock();
    return;
}

//============================================================================
void GuiUserMgr::clearGuiUserUpdateClientList( void )
{
    if( m_GuiUserUpdateClientList.size() )
    {
        guiUserUpdateClientsLock();
        m_GuiUserUpdateClientList.clear();
        guiUserUpdateClientsUnlock();
    }
}

//============================================================================
void GuiUserMgr::sendUserUpdatedToCallbacks( GuiUser* guiUser )
{
    if( guiUser )
    {
        guiUserUpdateClientsLock();
        for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
        {
            GuiUserUpdateCallback* client = *iter;
            if( client )
            {
                client->callbackOnUserUpdated( guiUser );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserMgr::sendUserUpdatedToCallbacks invalid callback" );
            }
        }

        guiUserUpdateClientsUnlock();
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiUserMgr::sendUserUpdatedToCallbacks invalid guiUser" );
    }
}

//============================================================================
void GuiUserMgr::toGuiUserOnlineStatus( VxNetIdent* netIdent, bool isOnline )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    updateUser( netIdent );
}

//============================================================================
void GuiUserMgr::connnectIdNearbyStatusChange( VxGUID& onlineId, uint64_t nearbyTimeOrZeroIfNot )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        guiUser->updateIsNearby();
        sendUserUpdatedToCallbacks( guiUser );
    }
}

//============================================================================
void GuiUserMgr::connnectIdRelayStatusChange( VxGUID& onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        
        guiUser->updateIsRelayed();
        bool isRelayed = guiUser->isRelayed();
        LogMsg( LOG_VERBOSE, "GuiUserMgr::connnectIdRelayStatusChange is relayed ? %d %s", isRelayed, getUserOnlineName( onlineId ).c_str() );
        sendUserUpdatedToCallbacks( guiUser );
    }
}

//============================================================================
void GuiUserMgr::connnectIdOnlineStatusChange( VxGUID& onlineId )
{
    GuiUser* guiUser = findUser( onlineId );
    if( guiUser )
    {
        guiUser->updateIsOnline();
        bool isOnline = guiUser->isOnline();
        LogMsg( LOG_VERBOSE, "GuiUserMgr::connnectIdOnlineStatusChange is online ? %d %s", isOnline, getUserOnlineName( onlineId ).c_str() );
        emit signalUserOnlineStatus( guiUser );
        sendUserUpdatedToCallbacks( guiUser );
    }
}
