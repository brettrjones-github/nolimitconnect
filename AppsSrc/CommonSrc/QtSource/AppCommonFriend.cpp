//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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


#include "MyIcons.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "AccountMgr.h"
#include "ToGuiActivityInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::updateFriendList( GuiUser * netIdent, bool sessionTimeChange )
{
	//ui.mainFriendList->updateFriend( netIdent, sessionTimeChange );
}

//============================================================================
void AppCommon::toGuiIndentListUpdate( EUserViewType listType, VxGUID& onlineId, uint64_t timestamp )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit signalInternalToGuiIndentListUpdate( listType, onlineId, timestamp );
}

//============================================================================
void AppCommon::slotInternalToGuiIndentListUpdate( EUserViewType listType, VxGUID onlineId, uint64_t timestamp )
{
    LogMsg( LOG_INFO, "AppCommon::toGuiIndentListUpdate %d", listType );
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiIndentListUpdate( listType, onlineId, timestamp );
    }

    getUserMgr().toGuiIndentListUpdate( listType, onlineId, timestamp );
}

//============================================================================
void AppCommon::toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit slotInternalToGuiIndentListRemove( listType, onlineId );
}

//============================================================================
void AppCommon::slotInternalToGuiIndentListRemove( EUserViewType listType, VxGUID onlineId )
{
    LogMsg( LOG_INFO, "AppCommon::toGuiIndentListRemove %d", listType );
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiIndentListRemove( listType, onlineId);
    }

    getUserMgr().toGuiIndentListRemove( listType, onlineId );
}

//============================================================================
void AppCommon::toGuiContactAdded( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactAdded invalid netIdent" );
        return;
    }

    emit slotInternalToGuiContactAdded( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactAdded( VxNetIdent netIdent )
{
    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiContactAdded( &netIdent );
    }

    getUserMgr().toGuiContactAdded( &netIdent );
}

//============================================================================
void AppCommon::toGuiContactRemoved( VxGUID& onlineId )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    emit slotInternalToGuiContactRemoved( onlineId );
}

//============================================================================
void AppCommon::slotInternalToGuiContactRemoved( VxGUID onlineId )
{
    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiContactRemoved( onlineId );
    }

    getUserMgr().toGuiContactRemoved( onlineId );
}

//============================================================================
void AppCommon::toGuiContactOffline( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOffline invalid netIdent" );
        return;
    }

    emit slotInternalToGuiContactOffline( netIdent->getMyOnlineId() );
}

//============================================================================
void AppCommon::slotInternalToGuiContactOffline( VxGUID onlineId )
{
    GuiUser *user = m_UserMgr.getUser( onlineId );
    if( user )
    {
        LogMsg( LOG_VERBOSE, "AppCommon::toGuiContactOffline user %s", user->getOnlineName().c_str() );
	    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
	    {
		    ToGuiActivityInterface* client = *iter;
		    client->toGuiContactOffline( user );
	    }
    }
    else
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOffline null user" );
    }
}

//============================================================================
void AppCommon::toGuiContactOnline( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOnline invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactOnline( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactOnline( VxNetIdent netIdent )
{
    LogMsg( LOG_VERBOSE, "AppCommon::toGuiContactOnline user %s", netIdent.getOnlineName() );
    for( auto iter = m_ToGuiActivityInterfaceList.begin(); iter != m_ToGuiActivityInterfaceList.end(); ++iter )
    {
        ToGuiActivityInterface* client = *iter;
        client->toGuiContactOnline( &netIdent );
    }
}

//============================================================================
void AppCommon::toGuiContactNameChange( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactNameChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactNameChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactNameChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactNameChange %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiContactNameChange( &netIdent );
    }

    getUserMgr().toGuiContactNameChange( &netIdent );
}

//============================================================================
//! called when description changes
void AppCommon::toGuiContactDescChange( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactDescChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactDescChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactDescChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactDescChange %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiContactDescChange( &netIdent );
    }

    getUserMgr().toGuiContactDescChange( &netIdent );
}

//============================================================================
//! called when my friendship to him changes
void AppCommon::toGuiContactMyFriendshipChange( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactMyFriendshipChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactMyFriendshipChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactMyFriendshipChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactMyFriendshipChange %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiContactMyFriendshipChange( &netIdent );
    }

    getUserMgr().toGuiContactMyFriendshipChange( &netIdent );
}

//============================================================================
//! called when his friendship to me changes
void AppCommon::toGuiContactHisFriendshipChange( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactHisFriendshipChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactHisFriendshipChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactHisFriendshipChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactHisFriendshipChange %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiContactHisFriendshipChange( &netIdent );
    }

    getUserMgr().toGuiContactHisFriendshipChange( &netIdent );
}

//============================================================================
//! called when plugin permission changes
void AppCommon::toGuiPluginPermissionChange( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiPluginPermissionChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiPluginPermissionChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiPluginPermissionChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiPluginPermissionChange %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiPluginPermissionChange( &netIdent );
    }

    getUserMgr().toGuiPluginPermissionChange( &netIdent );
}

//============================================================================
//! called when search flags changes
void AppCommon::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactSearchFlagsChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactSearchFlagsChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactSearchFlagsChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactSearchFlagsChange %s", netIdent.getOnlineName()); 
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiContactSearchFlagsChange( &netIdent );
    }

    getUserMgr().toGuiContactSearchFlagsChange( &netIdent );
}

//============================================================================
//! called when connection info changes
void AppCommon::toGuiContactConnectionChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactConnectionChange %s ??", netIdent->getOnlineName()); // BRJ is this needed??
}

//============================================================================
//! called when any contact info changes ( including any of the above )
void AppCommon::toGuiContactAnythingChange( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    LogMsg( LOG_INFO, "AppCommon::toGuiContactAnythingChange %s", netIdent->getOnlineName() );
}

//============================================================================
//! called when any contact info changes ( including any of the above )
void AppCommon::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactLastSessionTimeChange invalid netIdent" );
        return;
    }

    emit signalInternalToGuiContactLastSessionTimeChange( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiContactLastSessionTimeChange( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactLastSessionTimeChange %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiContactLastSessionTimeChange( &netIdent );
    }
}

//============================================================================
void AppCommon::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiUpdateMyIdent invalid netIdent" );
        return;
    }

    emit signalInternalToGuiUpdateMyIdent( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiUpdateMyIdent( VxNetIdent netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiUpdateMyIdent %s", netIdent.getOnlineName());
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateInterface* client = *iter;
        client->toGuiUpdateMyIdent( &netIdent );
    }

    getUserMgr().toGuiUpdateMyIdent( &netIdent );
}

//============================================================================
void AppCommon::toGuiSaveMyIdent( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    if( !netIdent || !netIdent->isValidNetIdent() )
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiSaveMyIdent invalid netIdent" );
        return;
    }

    emit signalInternalToGuiSaveMyIdent( *netIdent );
}

//============================================================================
void AppCommon::slotInternalToGuiSaveMyIdent( VxNetIdent netIdent )
{
    getUserMgr().toGuiSaveMyIdent( &netIdent );
}

//============================================================================
//! called when my identity changes
void AppCommon::onUpdateMyIdent( VxNetIdent * netIdent )
{
    if( netIdent )
    {
	    memcpy( getAppGlobals().getUserIdent(), netIdent, sizeof( VxNetIdent ) );
	    m_AccountMgr.updateAccount(*netIdent);
    }
}
