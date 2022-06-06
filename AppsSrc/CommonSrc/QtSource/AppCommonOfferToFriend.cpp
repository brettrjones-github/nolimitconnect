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
// http://www.nolimitconnect.org
//============================================================================


#include "AppCommon.h"
#include "AppGlobals.h"
#include "GuiOfferClientMgr.h"
#include "GuiOfferHostMgr.h"

#include "AppletPeerChangeFriendship.h"
#include "AppletPeerViewSharedFiles.h"
#include "AppletPeerSelectFileToSend.h"
#include "ActivityMessageBox.h"
#include "MyIcons.h"
#include "GuiParams.h"

#include <QDesktopServices>
#include <QUrl>

//============================================================================
bool AppCommon::offerToFriendPluginSession( GuiUser * poFriend, EPluginType ePluginType, bool inGroup, QWidget * parent )
{
	bool showedActivity = false;
	if( poFriend->isMyAccessAllowedFromHim( ePluginType, inGroup ) )
	{
		startActivity( ePluginType, poFriend, parent );
		showedActivity = true;
	}
	else
	{
		EPluginAccess ePluginAccess = poFriend->getMyAccessPermissionFromHim( ePluginType );
        ActivityMessageBox errMsgBox( GetAppInstance(), this, LOG_INFO, "%s's %s", poFriend->getOnlineName().c_str(), GuiParams::describePluginAction( poFriend, ePluginType, ePluginAccess ).toStdString().c_str() );
		errMsgBox.exec();
	}

	removePluginSessionOffer( ePluginType, poFriend );
	return showedActivity;
}

//============================================================================
// view shared files
void AppCommon::offerToFriendViewSharedFiles( GuiUser * poFriend, bool inGroup, QWidget* parent )
{
	//AppletPeerViewSharedFiles oDlg( *this, poFriend, this  );
	//oDlg.exec();
}

//============================================================================
// offer to send a file
void AppCommon::offerToFriendSendFile( GuiUser * poFriend, bool inGroup, QWidget* parent )
{
	//ActivitySelectFileToSend * dlg = new ActivitySelectFileToSend( *this, poFriend, this);
	//dlg->exec();
}

//============================================================================
// change friendship
void AppCommon::offerToFriendChangeFriendship( GuiUser * poFriend )
{
	//AppletPeerChangeFriendship * poDlg = new AppletPeerChangeFriendship( *this, this);
	//poDlg->setFriend( poFriend );
	//poDlg->exec();
}

//============================================================================
void AppCommon::removePluginSessionOffer( EPluginType ePluginType, GuiUser * poFriend )
{
    getOfferClientMgr().removePluginSessionOffer( ePluginType, poFriend );
}
