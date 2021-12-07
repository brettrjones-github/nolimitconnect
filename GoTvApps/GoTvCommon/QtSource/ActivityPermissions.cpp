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
#include "ActivityPermissions.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
ActivityPermissions::ActivityPermissions( AppCommon& app, QWidget * parent )
	: ActivityBase( OBJNAME_ACTIVITY_PERMISSIONS, app, parent, eAppletMessengerFrame, true )
{
	ui.setupUi(this);

    connectBarWidgets();

	connect( ui.AnybodyButton,				SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.FriendsOnlyButton,			SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.AdminsOnlyButton,			SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.GuestsAndFriendsButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.NobodyButton,				SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.OkButton,					SIGNAL(clicked()), this, SLOT(onOkButClick()) );
	connect( ui.CancelButton,				SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
	connect( ui.m_TitleBarWidget,			SIGNAL(signalBackButtonClicked()), this, SLOT(onCancelButClick()) );
}

//============================================================================
//! Set plugin to set permissions on    
void ActivityPermissions::setPluginType( EPluginType ePluginType )
{
	m_ePluginType = ePluginType;
	int iPluginPermission = m_Engine.getPluginPermission( m_ePluginType );
	setPluginIcon( m_ePluginType, (EFriendState)iPluginPermission );
	setCheckedPermission( (EFriendState)iPluginPermission );

	switch( ePluginType )
	{
	case ePluginTypeWebServer:	// web server plugin ( for profile web page )
		ui.PermissionLabel->setText( "Who can view my profile" );
		ui.AdminsOnlyButton->setEnabled( false );
		ui.FriendsOnlyButton->setEnabled( false );
		ui.GuestsAndFriendsButton->setEnabled( false );
		break;
	case ePluginTypeRelay:	// proxy plugin
		ui.PermissionLabel->setText( "Who can use my device as a relay" );
		break;
	case ePluginTypeFileXfer:	
		ui.PermissionLabel->setText( "Who can offer to send a file" );
		break;
	case ePluginTypeFileServer:	
		ui.PermissionLabel->setText( "Who can access shared files" );
		break;
	case 	ePluginTypeCamServer:	
		ui.PermissionLabel->setText( "Who can access web cam broadcast" );
		break;
	case 	ePluginTypeMessenger:	
		ui.PermissionLabel->setText( "Who can offer chat session" );
		break;
	case 	ePluginTypeVoicePhone:	
		ui.PermissionLabel->setText( "Who can offer voice phone (VOIP)" );
		break;
	case 	ePluginTypePushToTalk:
		ui.PermissionLabel->setText( "Who can push to talk to me (Walkie Talkie)" );
		break;
	case 	ePluginTypeVideoPhone:	
		ui.PermissionLabel->setText( "Who can offer video chat" );
		break;
	case 	ePluginTypeTruthOrDare:	
		ui.PermissionLabel->setText( "Who can offer to play truth or dare" );
		break;

	case 	ePluginTypeStoryboard:	
		ui.PermissionLabel->setText( "Who can read my story board" );
		break;

	case 	ePluginTypeInvalid:
    default:
        ui.PermissionLabel->setText( "Unknown" );
        break;
    }
}

//============================================================================
//! Set plugin icon based on permission level
void ActivityPermissions::setPluginIcon( EPluginType ePluginType, EFriendState ePluginPermission )
{
	if( eFriendStateIgnore == ePluginPermission )
	{
		ui.PermissionButton->setIcon( getMyIcons().getPluginIcon(ePluginType, ePluginAccessDisabled ) );
	}
	else
	{
		ui.PermissionButton->setIcon( getMyIcons().getPluginIcon(ePluginType, ePluginAccessOk ) );
	}
}

//============================================================================
//! get current permission selection
EFriendState ActivityPermissions::getPermissionSelection( void )
{
	EFriendState ePluginPermission = eFriendStateFriend;
	if( ui.NobodyButton->isChecked() )
	{
		ePluginPermission = eFriendStateIgnore;
	}
	else if( ui.AnybodyButton->isChecked() )
	{
		ePluginPermission = eFriendStateAnonymous;
	} 
	else if( ui.GuestsAndFriendsButton->isChecked() )
	{
		ePluginPermission = eFriendStateGuest;
	} 
	else if( ui.FriendsOnlyButton->isChecked() )
	{
		ePluginPermission = eFriendStateFriend;
	}
	else if( ui.AdminsOnlyButton->isChecked() )
	{
		ePluginPermission = eFriendStateAdmin;
	}

	return ePluginPermission;
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivityPermissions::onOkButClick( void )
{
	m_MyApp.getAppGlobals().updatePluginPermission( m_ePluginType, getPermissionSelection() );
	accept();
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivityPermissions::onCancelButClick( void )
{
	reject();
}

//============================================================================
//! Implement the OnClickListener callback    
void ActivityPermissions::onPermissionClick( void )
{
	EFriendState ePluginPermission = eFriendStateFriend;
	if( ui.NobodyButton->isChecked() )
	{
		ePluginPermission = eFriendStateIgnore;
	}
	else if( ui.AnybodyButton->isChecked() )
	{
		ePluginPermission = eFriendStateAnonymous;
	} 
	else if( ui.GuestsAndFriendsButton->isChecked() )
	{
		ePluginPermission = eFriendStateGuest;
	} 
	else if( ui.FriendsOnlyButton->isChecked() )
	{
		ePluginPermission = eFriendStateFriend;
	}
	else if( ui.AdminsOnlyButton->isChecked() )
	{
		ePluginPermission = eFriendStateAdmin;
	}

	setPluginIcon( m_ePluginType, ePluginPermission );
}

//============================================================================
//! set which radio button is checked
void ActivityPermissions::setCheckedPermission( EFriendState ePluginPermission )
{
	ui.AdminsOnlyButton->setChecked( false );
	ui.FriendsOnlyButton->setChecked( false );
	ui.GuestsAndFriendsButton->setChecked( false );
	ui.AnybodyButton->setChecked( false );
	ui.NobodyButton->setChecked( false );

	switch( ePluginPermission )
	{
	case eFriendStateAdmin:
		ui.AdminsOnlyButton->setChecked( true );
		break;
	case eFriendStateFriend:
		ui.FriendsOnlyButton->setChecked( true );
		break;
	case eFriendStateGuest:
		ui.GuestsAndFriendsButton->setChecked( true );
		break;
	case eFriendStateAnonymous:
		ui.AnybodyButton->setChecked( true );
		break;
	case eFriendStateIgnore:
		ui.NobodyButton->setChecked( true );
		break;
	}
}

