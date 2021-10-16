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

//============================================================================
GuiUserMgr::GuiUserMgr( AppCommon& app )
    : QObject( &app )
    , m_MyApp( app )
{
}

//============================================================================
void GuiUserMgr::onAppCommonCreated( void )
{
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
void GuiUserMgr::toGuiContactNearby( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
}

//============================================================================
void GuiUserMgr::toGuiContactNotNearby( VxNetIdent * netIdent )
{
    emit signalInternalUpdateUser( new VxNetIdent( *netIdent ), eHostTypeUnknown );
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
GuiUser* GuiUserMgr::updateUser( VxNetIdent* hisIdent, EHostType hostType )
{
    if( !hisIdent )
    {
        LogMsg( LOG_ERROR, "GuiUserMgr::updateUserOnline invalid param" );
        return nullptr;
    }

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

//============================================================================
void GuiUserMgr::updateMyIdent( VxNetIdent* myIdent )
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
void GuiUserMgr::onUserAdded( GuiUser* user )
{
    if( isMessengerReady() )
    {
        emit signalUserAdded( user );
    }
}

//============================================================================
void GuiUserMgr::onUserRemoved( VxGUID& onlineId )
{
    if( isMessengerReady() )
    {
        emit signalUserRemoved( onlineId );
    }
}

//============================================================================
void GuiUserMgr::onUserOnlineStatusChange( GuiUser* user, bool isOnline )
{
    if( isMessengerReady() )
    {
        emit signalUserOnlineStatus( user, isOnline );
    }
}

//============================================================================
void GuiUserMgr::onUserUpdated( GuiUser* user )
{
    if( isMessengerReady() )
    {
        emit signalUserUpdated( user );
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