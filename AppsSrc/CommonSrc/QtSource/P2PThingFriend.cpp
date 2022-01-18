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

    LogMsg( LOG_INFO, "AppCommon::toGuiIndentListUpdate %d", listType );
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiIndentListUpdate( listType, onlineId, timestamp );
    }

    toGuiUserUpdateClientsUnlock();;
}

//============================================================================
void AppCommon::toGuiIndentListRemove( EUserViewType listType, VxGUID& onlineId )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    LogMsg( LOG_INFO, "AppCommon::toGuiIndentListRemove %d", listType );
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiIndentListRemove( listType, onlineId);
    }

    toGuiUserUpdateClientsUnlock();;
}

//============================================================================
void AppCommon::toGuiContactAdded( VxNetIdent * netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    toGuiActivityClientsLock();
    std::vector<ToGuiActivityClient>::iterator iter;
    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
    {
        ToGuiActivityClient& client = *iter;
        client.m_Callback->toGuiContactAdded( client.m_UserData, netIdent );
    }

    toGuiActivityClientsUnlock();
}

//============================================================================
void AppCommon::toGuiContactRemoved( VxGUID& onlineId )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    toGuiActivityClientsLock();
    std::vector<ToGuiActivityClient>::iterator iter;
    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
    {
        ToGuiActivityClient& client = *iter;
        client.m_Callback->toGuiContactRemoved( client.m_UserData, onlineId );
    }

    toGuiActivityClientsUnlock();
}

//============================================================================
//! called when friend goes offline
void AppCommon::toGuiContactOffline( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    GuiUser *user = m_UserMgr.getUser( netIdent->getMyOnlineId() );
    if( user )
    {
        LogMsg( LOG_VERBOSE, "AppCommon::toGuiContactOffline user %s", netIdent->getOnlineName() );
	    toGuiActivityClientsLock();
	    std::vector<ToGuiActivityClient>::iterator iter;
	    for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	    {
		    ToGuiActivityClient& client = *iter;
		    client.m_Callback->toGuiContactOffline( client.m_UserData, user );
	    }

	    toGuiActivityClientsUnlock();
    }
    else
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOffline null user for %s", netIdent->getOnlineName() );
    }
}

//============================================================================
//! called when friend goes online
void AppCommon::toGuiContactOnline( VxNetIdent * netIdent, EHostType hostType, bool newContact )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

    if( netIdent )
    {
        LogMsg( LOG_VERBOSE, "AppCommon::toGuiContactOnline user %s host type %s", netIdent->getOnlineName(), DescribeHostType( hostType ) );

        toGuiActivityClientsLock();
        std::vector<ToGuiActivityClient>::iterator iter;
        for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
        {
            ToGuiActivityClient& client = *iter;
            client.m_Callback->toGuiContactOnline( client.m_UserData, netIdent, hostType, newContact );
        }

        toGuiActivityClientsUnlock();
    }
    else
    {
        LogMsg( LOG_ERROR, "AppCommon::toGuiContactOffline null netIdent" );
    }
}

//============================================================================
//! called when name changes
void AppCommon::toGuiContactNameChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactNameChange %s", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiContactNameChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
//! called when description changes
void AppCommon::toGuiContactDescChange( VxNetIdent * netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

	LogMsg( LOG_INFO, "AppCommon::toGuiContactDescChange %s", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiContactDescChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
//! called when my friendship to him changes
void AppCommon::toGuiContactMyFriendshipChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactMyFriendshipChange %s \n", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiContactMyFriendshipChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
//! called when his friendship to me changes
void AppCommon::toGuiContactHisFriendshipChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactHisFriendshipChange %s \n", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiContactHisFriendshipChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
//! called when plugin permission changes
void AppCommon::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiPluginPermissionChange %s", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiPluginPermissionChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
//! called when search flags changes
void AppCommon::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactSearchFlagsChange %s", netIdent->getOnlineName()); 
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiContactSearchFlagsChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
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
void AppCommon::toGuiContactAnythingChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactAnythingChange %s\n", netIdent->getOnlineName());
	//emit signalContactSearchFlagsChange( netIdent );
}

//============================================================================
//! called when any contact info changes ( including any of the above )
void AppCommon::toGuiContactLastSessionTimeChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactLastSessionTimeChange %s\n", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiContactLastSessionTimeChange( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
void AppCommon::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiUpdateMyIdent %s", netIdent->getOnlineName());
    toGuiUserUpdateClientsLock();
    for( auto iter = m_ToGuiUserUpdateClientList.begin(); iter != m_ToGuiUserUpdateClientList.end(); ++iter )
    {
        ToGuiUserUpdateClient& client = *iter;
        client.m_Callback->toGuiUpdateMyIdent( netIdent );
    }

    toGuiUserUpdateClientsUnlock();
}

//============================================================================
void AppCommon::toGuiSaveMyIdent( VxNetIdent* netIdent )
{
    if( VxIsAppShuttingDown() )
    {
        return;
    }

    getUserMgr().toGuiSaveMyIdent( netIdent );
}

//============================================================================
void AppCommon::onEngineStatusMsg( QString msg )
{
	//ui.statusBar->showMessage(msg);
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
