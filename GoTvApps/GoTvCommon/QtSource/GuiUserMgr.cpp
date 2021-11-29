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

#include "GuiUserMgr.h"
#include "AppCommon.h"
#include "GuiUserMgrGuiUserUpdateClient.h"
#include "GuiUserMgrGuiUserUpdateInterface.h"

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
    connect( this, SIGNAL( signalInternalIndentListUpdate( EFriendListType, VxGUID, uint64_t ) ), this, SLOT( slotInternalIndentListUpdate( EFriendListType, VxGUID, uint64_t ) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalIndentListRemove( EFriendListType, VxGUID ) ),         this, SLOT( slotInternalIndentListRemove( EFriendListType, VxGUID ) ), Qt::QueuedConnection );

    connect( this, SIGNAL( signalInternalUpdateUser(VxNetIdent*,EHostType) ),	                this, SLOT( slotInternalUpdateUser(VxNetIdent*,EHostType) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserRemoved(VxGUID) ),	                                this, SLOT( slotInternalUserRemoved(VxGUID) ), Qt::QueuedConnection );
    connect( this, SIGNAL( signalInternalUserOnlineStatus( VxNetIdent*, EHostType, bool ) ),    this, SLOT( slotInternalUserOnlineStatus( VxNetIdent*, EHostType, bool ) ), Qt::QueuedConnection );

    m_MyApp.wantToGuiUserUpdateCallbacks( this, true );
}

//============================================================================
bool GuiUserMgr::isMessengerReady( void )
{
    return m_MyApp.isMessengerReady();
}

//============================================================================
void GuiUserMgr::toGuiIndentListUpdate( EFriendListType listType, VxGUID& onlineId, uint64_t timestamp )
{
    emit signalInternalIndentListUpdate( listType, onlineId, timestamp );
}

//============================================================================
void GuiUserMgr::toGuiIndentListRemove( EFriendListType listType, VxGUID& onlineId )
{
    emit signalInternalIndentListRemove( listType, onlineId );
}

//============================================================================
void GuiUserMgr::toGuiContactAdded( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactRemoved( VxGUID& onlineId )
{
    emit signalInternalUserRemoved( onlineId );
}

//============================================================================
void GuiUserMgr::toGuiContactOnline( VxNetIdent * netIdent, EHostType hostType, bool newContact )
{
    emit signalInternalUserOnlineStatus( new VxNetIdent( *netIdent ), hostType, true );
}

//============================================================================
void GuiUserMgr::toGuiContactOffline( VxNetIdent * netIdent )
{
    emit signalInternalUserOnlineStatus( new VxNetIdent( *netIdent ), eHostTypeUnknown, false );
}

//============================================================================
void GuiUserMgr::toGuiContactNameChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactDescChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
    emit signalInternalUpdateMyIdent( new VxNetIdent( *netIdent ) );
}

//============================================================================
void GuiUserMgr::slotInternalIndentListUpdate( EFriendListType listType, VxGUID onlineId, uint64_t timestamp )
{
    if( isMessengerReady() && onlineId.isVxGUIDValid() )
    {
        guiUserUpdateClientsLock();
        for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
        {
            GuiUserMgrGuiUserUpdateClient& client = *iter;
            if( client.m_Callback )
            {
                client.m_Callback->callbackIndentListUpdate( listType, onlineId, timestamp );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserMgr::slotInternalIndentListUpdate invalid callback" );
            }
        }

        guiUserUpdateClientsUnlock();
    }
}

//============================================================================
void GuiUserMgr::slotInternalIndentListRemove( EFriendListType listType, VxGUID onlineId )
{
    if( isMessengerReady() && onlineId.isVxGUIDValid() )
    {
        guiUserUpdateClientsLock();
        for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
        {
            GuiUserMgrGuiUserUpdateClient& client = *iter;
            if( client.m_Callback )
            {
                client.m_Callback->callbackIndentListRemove( listType, onlineId );
            }
            else
            {
                LogMsg( LOG_ERROR, "GuiUserMgr::slotInternalIndentListRemove invalid callback" );
            }
        }

        guiUserUpdateClientsUnlock();
    }
}

//============================================================================
void GuiUserMgr::slotInternalUpdateUser( VxNetIdent* netIdent, EHostType hostType )
{
    updateUser( netIdent, hostType );
    delete netIdent;
}

//============================================================================
void GuiUserMgr::slotInternalUpdateMyIdent( VxNetIdent* netIdent )
{
    updateMyIdent( netIdent );
    delete netIdent;
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
void GuiUserMgr::slotInternalUserOnlineStatus( VxNetIdent* netIdent, EHostType hostType, bool online )
{
    GuiUser* guiUser = updateUser( netIdent, hostType );
    if( guiUser )
    {
        guiUser->setOnlineStatus( online );
    }

    delete netIdent;
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
GuiUser* GuiUserMgr::updateUser( VxNetIdent* hisIdent, EHostType hostType )
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
            guiUser->addHostType( hostType );
            onUserUpdated( guiUser );
        }
        else
        {
            guiUser = new GuiUser( m_MyApp );
            guiUser->setNetIdent( hisIdent );
            guiUser->addHostType( hostType );
            m_UserList[guiUser->getMyOnlineId()] = guiUser;
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
        guiUser->setOnlineStatus( false );
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
            GuiUserMgrGuiUserUpdateClient& client = *iter;
            if( client.m_Callback )
            {
                client.m_Callback->callbackOnUserAdded( guiUser );
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
            GuiUserMgrGuiUserUpdateClient& client = *iter;
            if( client.m_Callback )
            {
                client.m_Callback->callbackOnUserRemoved( onlineId );
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
void GuiUserMgr::onUserOnlineStatusChange( GuiUser* user )
{
    if( isMessengerReady() )
    {
        emit signalUserOnlineStatusChange( user );
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
void GuiUserMgr::wantGuiUserMgrGuiUserUpdateCallbacks( GuiUserMgrGuiUserUpdateInterface* callback, bool wantCallback )
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
            GuiUserMgrGuiUserUpdateClient& client = *iter;
            if( client.m_Callback == callback )
            {
                LogMsg( LOG_INFO, "WARNING. Ignoring New wantToGuiUserUpdateCallbacks because already in list" );
                guiUserUpdateClientsUnlock();
                return;
            }
        }

        GuiUserMgrGuiUserUpdateClient newClient( callback );
        m_GuiUserUpdateClientList.push_back( newClient );
        guiUserUpdateClientsUnlock();
        return;
    }

    for( auto iter = m_GuiUserUpdateClientList.begin(); iter != m_GuiUserUpdateClientList.end(); ++iter )
    {
        GuiUserMgrGuiUserUpdateClient& client = *iter;
        if( client.m_Callback == callback )
        {
            m_GuiUserUpdateClientList.erase( iter );
            guiUserUpdateClientsUnlock();
            return;
        }
    }

    LogMsg( LOG_INFO, "WARNING. ToGuiUserUpdateClient remove not found in list" );
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
            GuiUserMgrGuiUserUpdateClient& client = *iter;
            if( client.m_Callback )
            {
                client.m_Callback->callbackOnUserUpdated( guiUser );
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
