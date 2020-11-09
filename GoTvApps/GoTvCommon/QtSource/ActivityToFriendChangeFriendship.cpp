//============================================================================
// Copyright (C) 2013 Brett R. Jones
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
#include "ActivityToFriendChangeFriendship.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "AppCommon.h"

//============================================================================
ActivityToFriendChangeFriendship::ActivityToFriendChangeFriendship( AppCommon& app, QWidget * parent )
	: ActivityBase( OBJNAME_ACTIVITY_TO_FRIEND_CHANGE_FRIENDSHIP, app, parent, eAppletMessenger, true )
	, m_Friend(0)
{
	ui.setupUi(this);

    connectBarWidgets();

	connect( ui.ToFriendButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.ToGuestButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.ToAnonymousButton,	SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.ToIgnoreButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.OkButton,			SIGNAL(clicked()), this, SLOT(onOkButClick()) );
	connect( ui.CancelButton,		SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
	connect( ui.m_TitleBarWidget,	SIGNAL(signalBackButtonClicked()), this, SLOT(onCancelButClick()) );
}

//============================================================================
//! set friend to change your permission to him   
void ActivityToFriendChangeFriendship::setFriend( VxNetIdent * poFriend )
{
	vx_assert( poFriend );
	m_Friend = poFriend;
	EFriendState hisPermissionToMe = m_Friend->getHisFriendshipToMe();
	setHisPermissionToMe( hisPermissionToMe );
	EFriendState myPermissionToHim = m_Friend->getMyFriendshipToHim();
	setMyPermissionToHim( myPermissionToHim );
	setCheckedPermission( myPermissionToHim );
}

//============================================================================
void ActivityToFriendChangeFriendship::setHisPermissionToMe( EFriendState hisFriendshipToMe )
{
	ui.HisPermissionButton->setIcon( getMyIcons().getFriendshipIcon( hisFriendshipToMe ) );
	QString strHisFriendship = m_Friend->getOnlineName();
	strHisFriendship += "\'s Friendship To Me-";
	strHisFriendship += DescribeFriendState(hisFriendshipToMe);
	ui.HisPermissionLabel->setText(strHisFriendship);
}

//============================================================================
void ActivityToFriendChangeFriendship::setMyPermissionToHim( EFriendState myFriendshipToHim )
{
	ui.MyPermissionButton->setIcon( getMyIcons().getFriendshipIcon( myFriendshipToHim ) );
	QString strHisName = m_Friend->getOnlineName();
	QString strMyFriendship =  "My Friendship To ";
	strMyFriendship += strHisName + "-" + DescribeFriendState( myFriendshipToHim );
	ui.MyPermissionLabel->setText(strMyFriendship);
}

//============================================================================
//! get current permission selection
EFriendState ActivityToFriendChangeFriendship::getPermissionSelection( void )
{
	EFriendState ePluginPermission = eFriendStateFriend;
	if( ui.ToIgnoreButton->isChecked() )
	{
		ePluginPermission = eFriendStateIgnore;
	}
	else if( ui.ToAnonymousButton->isChecked() )
	{
		ePluginPermission = eFriendStateAnonymous;
	} 
	else if( ui.ToGuestButton->isChecked() )
	{
		ePluginPermission = eFriendStateGuest;
	} 
	else if( ui.ToFriendButton->isChecked() )
	{
		ePluginPermission = eFriendStateFriend;
	}
	else if( ui.ToAdministratorButton->isChecked() )
	{
		ePluginPermission = eFriendStateAdmin;
	}

	return ePluginPermission;
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivityToFriendChangeFriendship::onOkButClick( void )
{
	if( getPermissionSelection() != m_Friend->getMyFriendshipToHim() )
	{
		m_Friend->setMyFriendshipToHim(getPermissionSelection());
		m_Engine.fromGuiChangeMyFriendshipToHim( m_Friend->getMyOnlineId(), m_Friend->getMyFriendshipToHim(), m_Friend->getHisFriendshipToMe() );
		m_MyApp.refreshFriend( m_Friend->getMyOnlineId() );
	}

	accept();
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivityToFriendChangeFriendship::onCancelButClick( void )
{
	reject();
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivityToFriendChangeFriendship::onPermissionClick( void )
{
	EFriendState ePluginPermission = eFriendStateFriend;
	if( ui.ToIgnoreButton->isChecked() )
	{
		ePluginPermission = eFriendStateIgnore;
	}
	else if( ui.ToAnonymousButton->isChecked() )
	{
		ePluginPermission = eFriendStateAnonymous;
	} 
	else if( ui.ToGuestButton->isChecked() )
	{
		ePluginPermission = eFriendStateGuest;
	} 
	else if( ui.ToFriendButton->isChecked() )
	{
		ePluginPermission = eFriendStateFriend;
	}
	else if( ui.ToAdministratorButton->isChecked() )
	{
		ePluginPermission = eFriendStateAdmin;
	}

	setMyPermissionToHim( ePluginPermission );
}

//============================================================================
//! set which radio button is checked
void ActivityToFriendChangeFriendship::setCheckedPermission( EFriendState myFriendshipToHim )
{
	ui.ToAdministratorButton->setChecked( false );
	ui.ToFriendButton->setChecked( false );
	ui.ToGuestButton->setChecked( false );
	ui.ToAnonymousButton->setChecked( false );
	ui.ToIgnoreButton->setChecked( false );

	switch( myFriendshipToHim )
	{
	case eFriendStateAdmin:
		ui.ToAdministratorButton->setChecked( true );
		break;
	case eFriendStateFriend:
		ui.ToFriendButton->setChecked( true );
		break;
	case eFriendStateGuest:
		ui.ToGuestButton->setChecked( true );
		break;
	case eFriendStateAnonymous:
		ui.ToAnonymousButton->setChecked( true );
		break;
	case eFriendStateIgnore:
		ui.ToIgnoreButton->setChecked( true );
		break;
    default:
        break;
    }
}

