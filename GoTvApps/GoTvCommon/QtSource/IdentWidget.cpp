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
#include "IdentWidget.h"
#include "AppletPeerChangeFriendship.h"
#include "GuiUser.h"
#include "MyIcons.h"
#include "AppCommon.h"

//============================================================================
IdentWidget::IdentWidget(QWidget *parent)
: QPushButton(parent)
, m_MyApp( GetAppInstance() )
{
	setupUi(this);
	setIdentWidgetSize( eButtonSizeMedium );
	this->m_OfferButton->setVisible( false );
	connect( m_AvatarButton, SIGNAL( clicked() ), this, SIGNAL( signalAvatarButtonClicked() ) );
	connect( m_FriendshipButton, SIGNAL( clicked() ), this, SLOT( slotFrienshipButtonClicked() ) );
	connect( m_OfferButton, SIGNAL( clicked() ), this, SIGNAL( signalOfferButtonClicked() ) );
	connect( m_FriendMenuButton, SIGNAL(clicked()), this, SIGNAL(signalFriendMenuButtonClicked()) );
	m_MyApp.getAppTheme().applyTheme( this );
}

//============================================================================
MyIcons&  IdentWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void IdentWidget::setIdentWidgetSize( EButtonSize buttonSize )
{
	QSize butSize = GuiParams::getButtonSize( buttonSize );
	if( buttonSize < eButtonSizeMedium )
	{
		// wont fit the third line
		this->m_TodLabel->setVisible( false );
	}
	else
	{
		this->m_TodLabel->setVisible( true );
	}

	this->setFixedHeight( butSize.height() + 4 );
	this->m_AvatarButton->setFixedSize( buttonSize );
	setAvatarIcon( eMyIconAvatarImage );
	this->m_FriendshipButton->setFixedSize( buttonSize );
	setFriendshipIcon( eMyIconAnonymous );
	this->m_OfferButton->setFixedSize( buttonSize );
	this->m_FriendMenuButton->setFixedSize( buttonSize );
	setMenuIcon( eMyIconMenu );
}

//============================================================================
void IdentWidget::updateGuiFromData( GuiUser * netIdent )
{
	m_NetIdent = netIdent;
	if( m_NetIdent )
	{
		setOnlineState( netIdent->isOnline() );
		this->m_FriendPrefixLabel->setText( netIdent->describeMyFriendshipToHim() );
		this->m_FriendNameLabel->setText( netIdent->getOnlineName().c_str() );
		this->m_DescTextLabel->setText( netIdent->getOnlineDescription().c_str() );
		this->m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( netIdent->getMyFriendshipToHim() ) );
		QString truths = QObject::tr( "Truths: " );
		QString dares = QObject::tr( " Dares: " );
		this->m_TodLabel->setText( QString( truths + "%1" + dares + "%2" ).arg( netIdent->getTruthCount() ).arg( netIdent->getDareCount() ) );
		setAvatarThumbnail( netIdent->getAvatarThumbId() );
	}
}

//============================================================================
void IdentWidget::setAvatarButtonVisible( bool visible )
{
	this->m_AvatarButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setFriendshipButtonVisible( bool visible )
{
	this->m_FriendshipButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setOfferButtonVisible( bool visible )
{
	this->m_OfferButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setMenuButtonVisible( bool visible )
{
	this->m_FriendMenuButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setOnlineState( bool isOnline )
{
	this->m_AvatarButton->setNotifyOnlineEnabled( isOnline );
}

//============================================================================
void IdentWidget::setAvatarThumbnail( VxGUID& thumbId )
{
	QImage thumbImage;
	if( m_MyApp.getThumbMgr().getThumbImage( thumbId, thumbImage ) )
	{
		this->m_AvatarButton->setIconOverrideImage( thumbImage );
	}
}

//============================================================================
void IdentWidget::setAvatarIcon( EMyIcons myIcon )
{
	this->m_AvatarButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::setFriendshipIcon( EMyIcons myIcon )
{
	this->m_FriendshipButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::setOfferIcon( EMyIcons myIcon )
{
	this->m_OfferButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::setMenuIcon( EMyIcons myIcon )
{
	this->m_FriendMenuButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::slotFrienshipButtonClicked( void )
{
	if( m_NetIdent )
	{
		AppletPeerChangeFriendship* applet = dynamic_cast<AppletPeerChangeFriendship*>(m_MyApp.launchApplet( eAppletPeerChangeFriendship, dynamic_cast<QWidget*>(this->parent()) ));
		if( applet )
		{
			applet->setFriend( m_NetIdent );
		}
	}
}