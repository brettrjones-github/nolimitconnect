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
: QWidget(parent)
, m_MyApp( GetAppInstance() )
{
	ui.setupUi(this);
	setIdentWidgetSize( eButtonSizeMedium );
	ui.m_OfferButton->setVisible( false );
	connect( ui.m_AvatarButton, SIGNAL( clicked() ), this, SLOT( slotIdentAvatarButtonClicked() ) );
	connect( ui.m_FriendshipButton, SIGNAL( clicked() ), this, SLOT( slotIdentFrienshipButtonClicked() ) );
	connect( ui.m_OfferButton, SIGNAL( clicked() ), this, SLOT( slotIdentOfferButtonClicked() ) );
	connect( ui.m_FriendMenuButton, SIGNAL(clicked()), this, SLOT( slotIdentMenuButtonClicked()) );
	onIdentWidgetSetup();
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
		ui.m_TodLabel->setVisible( false );
	}
	else
	{
		ui.m_TodLabel->setVisible( true );
	}

	setFixedHeight( butSize.height() + 4 );
	ui.m_AvatarButton->setFixedSize( buttonSize );
	setIdentAvatarIcon( eMyIconAvatarImage );
	ui.m_FriendshipButton->setFixedSize( buttonSize );
	setIdentFriendshipIcon( eMyIconAnonymous );
	ui.m_OfferButton->setFixedSize( buttonSize );
	ui.m_FriendMenuButton->setFixedSize( buttonSize );
	setIdentMenuIcon( eMyIconMenu );
}

//============================================================================
void IdentWidget::updateIdentity( GuiUser * netIdent )
{
	m_NetIdent = netIdent;
	if( m_NetIdent )
	{
		updateIdentity( &netIdent->getNetIdent() );
	}
}

//============================================================================
void IdentWidget::updateIdentity( VxNetIdent* netIdent )
{
	if( netIdent )
	{
		setIdentOnlineState( netIdent->isOnline() );
		ui.m_FriendPrefixLabel->setText( netIdent->describeMyFriendshipToHim() );
		ui.m_FriendNameLabel->setText( netIdent->getOnlineName() );
		ui.m_DescTextLabel->setText( netIdent->getOnlineDescription() );
		ui.m_FriendshipButton->setIcon( getMyIcons().getFriendshipIcon( netIdent->getMyFriendshipToHim() ) );
		QString truths = QObject::tr( "Truths: " );
		QString dares = QObject::tr( " Dares: " );
		ui.m_TodLabel->setText( QString( truths + "%1" + dares + "%2" ).arg( netIdent->getTruthCount() ).arg( netIdent->getDareCount() ) );
		setIdentAvatarThumbnail( netIdent->getAvatarThumbGuid() );
		
		if( !netIdent->requiresRelay() )
		{
			ui.m_AvatarButton->setOverlayIcon( eMyIconDirectConnectedOverlay );
			if( netIdent->isOnline() )
			{
				ui.m_AvatarButton->setOverlayColor( m_MyApp.getAppTheme().getColor( eLayerNotifyOnlineColor ) );
			}
			else
			{
				ui.m_AvatarButton->setOverlayColor( m_MyApp.getAppTheme().getColor( eLayerNotifyOfflineColor ) );
			}
		}
	}
}

//============================================================================
void IdentWidget::setIdentAvatarButtonVisible( bool visible )
{
	ui.m_AvatarButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setIdentFriendshipButtonVisible( bool visible )
{
	ui.m_FriendshipButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setIdentOfferButtonVisible( bool visible )
{
	ui.m_OfferButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setIdentMenuButtonVisible( bool visible )
{
	ui.m_FriendMenuButton->setVisible( visible );
}

//============================================================================
void IdentWidget::setIdentOnlineState( bool isOnline )
{
	ui.m_AvatarButton->setNotifyOnlineEnabled( isOnline );
}

//============================================================================
void IdentWidget::setIdentAvatarThumbnail( VxGUID& thumbId )
{
	QImage thumbImage;
	if( m_MyApp.getThumbMgr().getThumbImage( thumbId, thumbImage ) )
	{
		ui.m_AvatarButton->setIconOverrideImage( thumbImage );
	}
}

//============================================================================
void IdentWidget::setIdentAvatarIcon( EMyIcons myIcon )
{
	ui.m_AvatarButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::setIdentFriendshipIcon( EMyIcons myIcon )
{
	ui.m_FriendshipButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::setIdentOfferIcon( EMyIcons myIcon )
{
	ui.m_OfferButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::setIdentMenuIcon( EMyIcons myIcon )
{
	ui.m_FriendMenuButton->setIcon( myIcon );
}

//============================================================================
void IdentWidget::slotIdentAvatarButtonClicked( void )
{
	onIdentAvatarButtonClicked();
}

//============================================================================
void IdentWidget::slotIdentFrienshipButtonClicked( void )
{
	onIdentFriendshipButtonClicked();
}

//============================================================================
void IdentWidget::slotIdentOfferButtonClicked( void )
{
	onIdentOfferButtonClicked();
}

//============================================================================
void IdentWidget::slotIdentMenuButtonClicked( void )
{
	onIdentMenuButtonClicked();
}

//============================================================================
void IdentWidget::onIdentAvatarButtonClicked( void )
{
	emit signalIdentAvatarButtonClicked();
}

//============================================================================
void IdentWidget::onIdentFriendshipButtonClicked( void )
{
	if( m_NetIdent )
	{
		AppletPeerChangeFriendship* applet = dynamic_cast<AppletPeerChangeFriendship*>(m_MyApp.launchApplet( eAppletPeerChangeFriendship, dynamic_cast<QWidget*>(parent()) ));
		if( applet )
		{
			applet->setFriend( m_NetIdent );
		}
	}
}

//============================================================================
void IdentWidget::onIdentOfferButtonClicked( void )
{
	emit signalIdentOfferButtonClicked();
}

//============================================================================
void IdentWidget::onIdentMenuButtonClicked( void )
{
	emit signalIdentMenuButtonClicked();
}
