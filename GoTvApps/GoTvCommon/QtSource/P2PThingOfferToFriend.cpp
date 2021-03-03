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
#include "AppCommon.h"
#include "AppGlobals.h"
#include "OffersMgr.h"

#include "ActivityToFriendChangeFriendship.h"
#include "ActivityToFriendRequestRelay.h"
#include "ActivityToFriendViewSharedFiles.h"
#include "ActivitySelectFileToSend.h"
#include "ActivityMessageBox.h"
#include "MyIcons.h"
#include "GuiParams.h"

#include <QDesktopServices>
#include <QUrl>

//============================================================================
bool AppCommon::offerToFriendPluginSession( VxNetIdent * poFriend, EPluginType ePluginType, QWidget * parent )
{
	bool showedActivity = false;
	if( poFriend->isMyAccessAllowedFromHim( ePluginType ) )
	{
		startActivity( ePluginType, poFriend, parent );
		showedActivity = true;
	}
	else
	{
		EPluginAccess ePluginAccess = poFriend->getMyAccessPermissionFromHim( ePluginType );
        ActivityMessageBox errMsgBox( GetAppInstance(), this, LOG_INFO, "%s's %s", poFriend->getOnlineName(), GuiParams::describePluginAction( poFriend, ePluginType, ePluginAccess ).toStdString().c_str() );
		errMsgBox.exec();
	}

	removePluginSessionOffer( ePluginType, poFriend );
	return showedActivity;
}

//============================================================================
void AppCommon::offerToFriendViewProfile( VxNetIdent * poFriend )
{
	viewWebServerPage( poFriend, "index.htm" );
}

//============================================================================
void AppCommon::offerToFriendViewStoryboard( VxNetIdent * poFriend )
{
	viewWebServerPage( poFriend, "story_board.htm" );
}

//============================================================================
void AppCommon::viewWebServerPage( VxNetIdentBase * netIdent, const char * webPageFileName )
{
	std::string uri;
	std::string myExternIp;
	if( ( netIdent->getMyOnlineIPv4() == m_Engine.getMyPktAnnounce().getMyOnlineIPv4() )
		&& netIdent->getLanIPv4().isValid() )
	{
		// is on same sub network as us.. use LAN ip
		myExternIp = netIdent->getLanIPv4().toStdString();
	}
	else
	{
		myExternIp = netIdent->getMyOnlineIPv4().toStdString();
	}

	if( netIdent->getMyOnlineId() == m_Engine.getMyPktAnnounce().getMyOnlineId() )
	{
		if( m_Engine.getMyPktAnnounce().getLanIPv4().isValid() )
		{
			myExternIp = m_Engine.getMyPktAnnounce().getLanIPv4().toStdString();
		}
	}

	netIdent->getProfileUri( uri, myExternIp.c_str(), webPageFileName );
	getAppGlobals().launchWebBrowser( uri.c_str() );
}

//============================================================================
// view shared files
void AppCommon::offerToFriendViewSharedFiles( VxNetIdent * poFriend )
{
	ActivityToFriendViewSharedFiles oDlg( *this, poFriend, this  );
	oDlg.exec();
}

//============================================================================
// offer to send a file
void AppCommon::offerToFriendSendFile( VxNetIdent * poFriend )
{
	ActivitySelectFileToSend * dlg = new ActivitySelectFileToSend( *this, poFriend, this);
	dlg->exec();
}

//============================================================================
// change friendship
void AppCommon::offerToFriendChangeFriendship( VxNetIdent * poFriend )
{
	ActivityToFriendChangeFriendship * poDlg = new ActivityToFriendChangeFriendship( *this, this);
	poDlg->setFriend( poFriend );
	poDlg->exec();
}

//============================================================================
// see if user wants to change his preferred proxy
void AppCommon::offerToFriendUseAsRelay( VxNetIdent * poFriend )
{
	ActivityToFriendRequestRelay * poDlg = new ActivityToFriendRequestRelay( *this, poFriend, this);
	poDlg->exec();
}

//============================================================================
void AppCommon::removePluginSessionOffer( EPluginType ePluginType, VxNetIdent * poFriend )
{
	getOffersMgr().removePluginSessionOffer( ePluginType, poFriend );
}
