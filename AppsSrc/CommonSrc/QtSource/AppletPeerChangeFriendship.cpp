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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletPeerChangeFriendship.h"
#include "MyIcons.h"
#include "AppGlobals.h"
#include "AppCommon.h"

//============================================================================
AppletPeerChangeFriendship::AppletPeerChangeFriendship( AppCommon& app, QWidget* parent )
	: AppletPeerBase( OBJNAME_ACTIVITY_TO_FRIEND_CHANGE_FRIENDSHIP, app, parent )
{
    setAppletType( eAppletPeerChangeFriendship );
	ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	ui.m_OkButton->setIcon( eMyIconCheckMark );
	ui.m_OkButton->setFixedSize( eButtonSizeLarge );

	ui.m_CancelButton->setIcon( eMyIconRedX );
	ui.m_CancelButton->setFixedSize( eButtonSizeLarge );

	ui.m_MakeFriendButton->setIcon( getMyIcons().getFriendshipIcon( eFriendStateFriend ) );
	ui.m_MakeFriendButton->setFixedSize( eButtonSizeMedium );

	ui.m_IgnoreButton->setIcon( getMyIcons().getFriendshipIcon( eFriendStateIgnore ) );
	ui.m_IgnoreButton->setFixedSize( eButtonSizeMedium );

	ui.m_HisPermissionButton->setFixedSize( eButtonSizeMedium );
	ui.m_MyPermissionButton->setFixedSize( eButtonSizeMedium );

	ui.m_IdentWidget->setDisableFriendshipChange( true );
	ui.m_IdentWidget->setIdentWidgetSize( eButtonSizeMedium );

	ui.m_PreferredButton->setFixedSize( eButtonSizeMedium );
	ui.m_PreferredButton->setAppIcon( eMyIconApp, this ); // use setAppIcon so has color

	connect( ui.ToAdministratorButton, SIGNAL( clicked() ), this, SLOT( onPermissionClick() ) );
	connect( ui.ToFriendButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.ToGuestButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.ToAnonymousButton,	SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.ToIgnoreButton,		SIGNAL(clicked()), this, SLOT(onPermissionClick()) );
	connect( ui.m_OkButton,			SIGNAL(clicked()), this, SLOT(onOkButClick()) );
	connect( ui.m_CancelButton,		SIGNAL(clicked()), this, SLOT(onCancelButClick()) );
	connect( ui.m_MakeFriendButton, SIGNAL(clicked()), this, SLOT(onMakeFriendButClick()) );
	connect( ui.m_IgnoreButton,		SIGNAL(clicked()), this, SLOT(onIgnoreButClick()) );
	connect( ui.m_PreferredButton,	SIGNAL(clicked()), this, SLOT(onPreferredButClick()) );
}

//============================================================================
//! set friend to change your permission to him   
void AppletPeerChangeFriendship::setFriend( GuiUser* poFriend )
{
	vx_assert( poFriend );
	m_Friend = poFriend;
	ui.m_IdentWidget->updateIdentity( m_Friend );
	EFriendState hisPermissionToMe = m_Friend->getHisFriendshipToMe();
	setHisPermissionToMe( hisPermissionToMe );
	EFriendState myPermissionToHim = m_Friend->getMyFriendshipToHim();
	setMyPermissionToHim( myPermissionToHim );
	setCheckedPermission( myPermissionToHim );
	ui.m_AgeTextLabel->setText( GuiParams::describeAge( m_Friend->getNetIdent().getAgeType() ) );
	ui.m_ContentTextLabel->setText( GuiParams::describeContentRating( ( EContentRating)m_Friend->getNetIdent().getPreferredContent() ) );
	ui.m_GenderTextLabel->setText( GuiParams::describeGender( ( EGenderType )m_Friend->getNetIdent().getGender() ) );
	ui.m_LanguageTextLabel->setText( GuiParams::describeLanguage( ( ELanguageType )m_Friend->getNetIdent().getPrimaryLanguage() ) );
	m_PreferredUser = m_MyApp.getGuiFavoriteMgr().getIsFavorite( poFriend->getMyOnlineId() );
	updatePreferredText();
}

//============================================================================
void AppletPeerChangeFriendship::setHisPermissionToMe( EFriendState hisFriendshipToMe )
{
	ui.m_HisPermissionButton->setIcon( getMyIcons().getFriendshipIcon( hisFriendshipToMe ) );
	QString strHisFriendship = m_Friend->getOnlineName().c_str();
	strHisFriendship += QObject::tr("\'s Friendship To Me -");
	strHisFriendship += DescribeFriendState(hisFriendshipToMe);
	ui.m_HisPermissionLabel->setText(strHisFriendship);
}

//============================================================================
void AppletPeerChangeFriendship::setMyPermissionToHim( EFriendState myFriendshipToHim )
{
	ui.m_IdentWidget->getIdentFriendshipButton()->setIcon( getMyIcons().getFriendshipIcon( myFriendshipToHim ) );
	ui.m_MyPermissionButton->setIcon( getMyIcons().getFriendshipIcon( myFriendshipToHim ) );
	QString strHisName = m_Friend->getOnlineName().c_str();
	QString strMyFriendship =   QObject::tr("My Friendship To ");
	strMyFriendship += strHisName + "-" + DescribeFriendState( myFriendshipToHim );
	ui.m_MyPermissionLabel->setText(strMyFriendship);
}

//============================================================================
//! get current permission selection
EFriendState AppletPeerChangeFriendship::getPermissionSelection( void )
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
void AppletPeerChangeFriendship::onOkButClick( void )
{
	if( getPermissionSelection() != m_Friend->getMyFriendshipToHim() )
	{
		m_Friend->setMyFriendshipToHim(getPermissionSelection());
		m_Engine.fromGuiChangeMyFriendshipToHim( m_Friend->getMyOnlineId(), m_Friend->getMyFriendshipToHim(), m_Friend->getHisFriendshipToMe() );
		m_MyApp.refreshFriend( m_Friend->getMyOnlineId() );
	}

	if( m_MyApp.getGuiFavoriteMgr().getIsFavorite( m_Friend->getMyOnlineId() ) != m_PreferredUser )
	{
		m_MyApp.getGuiFavoriteMgr().setIsFavorite( m_Friend->getMyOnlineId(), m_PreferredUser );
	}

	accept();
}

//============================================================================   
void AppletPeerChangeFriendship::onCancelButClick( void )
{
	reject();
}

//============================================================================
//! Implement the OnClickListener callback    
void AppletPeerChangeFriendship::onPermissionClick( void )
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
void AppletPeerChangeFriendship::setCheckedPermission( EFriendState myFriendshipToHim )
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

	setMyPermissionToHim( myFriendshipToHim );
}

//============================================================================   
void AppletPeerChangeFriendship::onMakeFriendButClick( void )
{
	setCheckedPermission( eFriendStateFriend );
}

//============================================================================   
void AppletPeerChangeFriendship::onIgnoreButClick( void )
{
	setCheckedPermission( eFriendStateIgnore );
}

//============================================================================   
void AppletPeerChangeFriendship::onPreferredButClick( void )
{
	m_PreferredUser = !m_PreferredUser;
	updatePreferredText();
}

//============================================================================   
void AppletPeerChangeFriendship::updatePreferredText( void )
{
	if( m_PreferredUser )
	{
		ui.m_PreferredLabel->setText( QObject::tr( "Not Preferred (No priority given to user)" ) );
	}
	else
	{
		ui.m_PreferredLabel->setText( QObject::tr( "Is Preferred (High priority given to user)" ) );
	}
}