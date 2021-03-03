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

#include <app_precompiled_hdr.h>
#include "MyIcons.h"
#include "AppCommon.h"
#include "AppGlobals.h"
#include "AccountMgr.h"
#include "ToGuiActivityInterface.h"

#include <CoreLib/VxGlobals.h>

//============================================================================
void AppCommon::updateFriendList( VxNetIdent * netIdent, bool sessionTimeChange )
{
	//ui.mainFriendList->updateFriend( netIdent, sessionTimeChange );
}

//============================================================================
//! called when friend goes offline
void AppCommon::toGuiContactOffline( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactOffline %s \n", netIdent->getOnlineName());
	emit signalContactOffline( netIdent );

	LogMsg( LOG_INFO, "toGuiContactOffline: toGuiActivityClientsLock\n" );
	// don't put in slot because want to call from thread so can return and avoid callback mutex deadlock
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiContactOffline( client.m_UserData, netIdent );
	}

	toGuiActivityClientsUnlock();
}

//============================================================================
//! called when friend goes online
void AppCommon::toGuiContactOnline( VxNetIdent * netIdent, bool newContact )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	//LogMsg( LOG_INFO, "AppCommon::toGuiContactOnline %s \n", netIdent->getOnlineName());

	//LogMsg( LOG_INFO, "toGuiContactOnline: toGuiActivityClientsLock\n" );
	// don't put in slot because want to call from thread so can return and avoid callback mutex deadlock
	toGuiActivityClientsLock();
	std::vector<ToGuiActivityClient>::iterator iter;
	for( iter = m_ToGuiActivityClientList.begin(); iter != m_ToGuiActivityClientList.end(); ++iter )
	{
		ToGuiActivityClient& client = *iter;
		client.m_Callback->toGuiContactOnline( client.m_UserData, netIdent, newContact );
	}

	toGuiActivityClientsUnlock();

	emit signalContactOnline( netIdent, newContact );
}

//============================================================================
//! called when friend is nearby
void AppCommon::toGuiContactNearby( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactNearby %s \n", netIdent->getOnlineName());
	emit signalContactNearby( netIdent );
}

//============================================================================
//! called when friend leaves udp network
void AppCommon::toGuiContactNotNearby( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactNotNearby %s \n", netIdent->getOnlineName());
	emit signalContactNotNearby( netIdent );
}

//============================================================================
//! called when name changes
void AppCommon::toGuiContactNameChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactNameChange %s \n", netIdent->getOnlineName());
	emit signalContactNameChange( netIdent );
}

//============================================================================
//! called when description changes
void AppCommon::toGuiContactDescChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::toGuiContactDescChange %s \n", netIdent->getOnlineName());
	emit signalContactDescChange( netIdent );
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
	emit signalContactMyFriendshipChange( netIdent );
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
	emit signalContactHisFriendshipChange( netIdent );
}

//============================================================================
//! called when plugin permission changes
void AppCommon::toGuiPluginPermissionChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiPluginPermissionChange %s\n", netIdent->getOnlineName());
	emit signalContactPluginPermissionChange( netIdent );
}

//============================================================================
//! called when search flags changes
void AppCommon::toGuiContactSearchFlagsChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactSearchFlagsChange %s\n", netIdent->getOnlineName());
	emit signalContactSearchFlagsChange( netIdent );
}

//============================================================================
//! called when connection info changes
void AppCommon::toGuiContactConnectionChange( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiContactConnectionChange %s\n", netIdent->getOnlineName());
	//emit signalContactSearchFlagsChange( netIdent );
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
	emit signalContactLastSessionTimeChange( netIdent );
}

//============================================================================
void AppCommon::toGuiUpdateMyIdent( VxNetIdent * netIdent )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	LogMsg( LOG_INFO, "AppCommon::toGuiUpdateMyIdent %s\n", netIdent->getOnlineName());
	emit signalUpdateMyIdent( netIdent );
}

//============================================================================
//=== slot to handle corresponding signals from other threads ===//
//============================================================================

//============================================================================
//! remove contact
void AppCommon::slotRemoveContact( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::slotRemoveContact %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}

//============================================================================
//! called when friend goes offline
void AppCommon::slotContactOffline( VxNetIdent * netIdent )
{
	if( false == VxIsAppShuttingDown() )
	{
		LogMsg( LOG_INFO, "AppCommon::slotContactOffline %s\n", netIdent->getOnlineName());
		updateFriendList( netIdent );
	}
}

//============================================================================
//! called when friend goes online
void AppCommon::slotContactOnline( VxNetIdent * netIdent, bool newContact )
{
	if( false == VxIsAppShuttingDown() )
	{
		LogMsg( LOG_INFO, "AppCommon::slotContactOnline %s\n", netIdent->getOnlineName());
		updateFriendList( netIdent );
	}
}

//============================================================================
//! called when friend is nearby
void AppCommon::slotContactNearby( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::slotContactNearby %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}

//============================================================================
//! called when friend leaves udp network
void AppCommon::onContactNotNearby( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactNotNearby %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}

//============================================================================
//! called when name changes
void AppCommon::onContactNameChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactNameChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}

//============================================================================
//! called when description changes
void AppCommon::onContactDescChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactDescChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}
//============================================================================
//! called when my friendship to him changes
void AppCommon::onContactMyFriendshipChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactMyFriendshipChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}

//============================================================================
//! called when his friendship to me changes
void AppCommon::onContactHisFriendshipChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactHisFriendshipChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent );
}

//============================================================================
//! called when plugin permission changes
void AppCommon::onContactPluginPermissionChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactPluginPermissionChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent, false );
}

//============================================================================
//! called when search flags changes
void AppCommon::onContactSearchFlagsChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactSearchFlagsChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent, false );
}

//============================================================================
//! called when search flags changes
void AppCommon::onContactLastSessionTimeChange( VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "AppCommon::onContactLastSessionTimeChange %s\n", netIdent->getOnlineName());
	updateFriendList( netIdent, true );
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

//============================================================================
//! called when friend in list is changed
void AppCommon::onFriendAdded( VxNetIdent * netIdent ) 
{
	onFriendUpdated( netIdent );
}

//============================================================================
//! called when friend in list is changed or added
void AppCommon::onFriendUpdated( VxNetIdent * netIdent ) 
{
	//ui.mainFriendList->updateFriend( netIdent );
}

//============================================================================
//! called when friend in list is removed
void AppCommon::onFriendRemoved( VxNetIdent * netIdent )
{
	//ui.mainFriendList->removeFriend( netIdent );
}

//============================================================================
//!	return true if should show friend in current list
bool AppCommon::shouldShowFriend( VxNetIdent * poFriend )
{
	/*
	EFriendState eMyFriendshipToHim = poFriend->getMyFriendshipToHim();

	switch( ui.mainFriendList->getFriendViewType() )
	{
	case eFriendViewEverybody:
		return true;

	case eFriendViewFriendsAndGuests:
		if( (eFriendStateFriend == eMyFriendshipToHim) || (eFriendStateGuest == eMyFriendshipToHim) )
		{
			return true;
		}
		return false;

	case eFriendViewAnonymous:
		if( eFriendStateAnonymous == eMyFriendshipToHim )
		{
			return true;
		}
		return false;

	case eFriendViewIgnored:
        if( eFriendStateIgnore == eMyFriendshipToHim )
		{
			return true;
		}
		return false;

	case eFriendViewMyProxies:
		if( poFriend->isMyPreferedRelay() )
		{
			return true;
		}
		return false;

	case eFriendViewAllProxies:
		if( ( poFriend->isMyAccessAllowedFromHim( ePluginTypeRelay ) ) &&
			( false == poFriend->requiresRelay() ) )
		{
			return true;
		}
		return false;
	default:
		return false;
	}*/

	return false;
}

