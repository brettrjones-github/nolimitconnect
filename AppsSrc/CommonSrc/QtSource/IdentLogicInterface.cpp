//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "IdentLogicInterface.h"
#include "AppletPeerChangeFriendship.h"
#include "GuiHelpers.h"
#include "GuiUser.h"
#include "GuiHostJoin.h"
#include "MyIcons.h"
#include "AppCommon.h"

//============================================================================
IdentLogicInterface::IdentLogicInterface( QWidget* parent )
	: QWidget( parent )
	, m_MyApp( GetAppInstance() )
{
}

//============================================================================
void IdentLogicInterface::setupIdentLogic( void ) // call after derived class ui is called
{
	if( getIdentOfferButton() )
	{
		getIdentOfferButton()->setVisible( false );
		connect( getIdentOfferButton(), SIGNAL( clicked() ), this, SLOT( slotIdentOfferButtonClicked() ) );
	}

	if( getIdentPushToTalkButton() )
	{
		getIdentPushToTalkButton()->setVisible( false );
		connect( getIdentPushToTalkButton(), SIGNAL( pressed() ), this, SLOT( slotIdentPushToTalkButtonPressed() ) );
		connect( getIdentPushToTalkButton(), SIGNAL( released() ), this, SLOT( slotIdentPushToTalkButtonReleased() ) );
	}

	connect( getIdentAvatarButton(), SIGNAL( clicked() ), this, SLOT( slotIdentAvatarButtonClicked() ) );
	connect( getIdentFriendshipButton(), SIGNAL( clicked() ), this, SLOT( slotIdentFrienshipButtonClicked() ) );
	connect( getIdentMenuButton(), SIGNAL( clicked() ), this, SLOT( slotIdentMenuButtonClicked() ) );
	onIdentLogicIsSetup();
}

//============================================================================
void IdentLogicInterface::setupIdentLogic( EButtonSize buttonSize ) // call after derived class ui is called.. also calls setIdentWidgetSize
{
	setIdentWidgetSize( buttonSize );
	setupIdentLogic();
}

//============================================================================
void IdentLogicInterface::setIdentWidgetSize( EButtonSize buttonSize )
{
	QSize butSize = GuiParams::getButtonSize( buttonSize );
	if( getIdentLine3() )
	{
		if( buttonSize < eButtonSizeLarge )
		{
			// wont fit the third line
			getIdentLine3()->setVisible( false );
		}
		else
		{
			getIdentLine3()->setVisible( true );
		}
	}

	setFixedHeight( butSize.height() + 4 );
	getIdentAvatarButton()->setFixedSize( buttonSize );
	setIdentAvatarIcon( eMyIconAvatarImage );
	getIdentFriendshipButton()->setFixedSize( buttonSize );
	setIdentFriendshipIcon( eMyIconAnonymous );
	if( getIdentOfferButton() )
	{
		getIdentOfferButton()->setFixedSize( buttonSize );
	}
	
	if( getIdentPushToTalkButton() )
	{
		getIdentPushToTalkButton()->setFixedSize( buttonSize );
		getIdentPushToTalkButton()->setIcon( eMyIconPushToTalkNormal );
	}

	getIdentMenuButton()->setFixedSize( buttonSize );
	setIdentMenuIcon( eMyIconMenu );
}

//============================================================================
void IdentLogicInterface::updateIdentity( GuiUser* netIdent, bool queryThumb )
{
	if( netIdent )
	{
		m_NetIdent = netIdent;
		if( m_NetIdent )
		{
			updateIdentity( &netIdent->getNetIdent(), queryThumb );
		}
	}
}

//============================================================================
void IdentLogicInterface::updateHosted( GuiHosted* guiHosted )
{
	if( guiHosted )
	{
		GuiUser* guiUser = guiHosted->getUser();
		if( guiUser )
		{
			m_NetIdent = guiUser;
			if( m_NetIdent )
			{
				updateIdentity( m_NetIdent, false );
			}

			VxGUID thumbId = m_NetIdent->getHostThumbId( guiHosted->getHostType(), true );
			if( thumbId.isVxGUIDValid() )
			{
				setIdentAvatarThumbnail( thumbId );
			}
		}

		getIdentLine1()->setText( guiHosted->getHostTitle().c_str() );
		getIdentLine2()->setText( guiHosted->getHostDescription().c_str() );
	}
}

//============================================================================
void IdentLogicInterface::updateIdentity( GuiHostJoin* hostIdent, bool queryThumb )
{
	m_NetIdent = hostIdent->getUser();
	updateIdentity( &hostIdent->getUser()->getNetIdent(), queryThumb );
}

//============================================================================
void IdentLogicInterface::updateIdentity( VxNetIdent* netIdent, bool queryThumb )
{
	if( netIdent )
	{
		setIdentOnlineState( netIdent->isOnline() );
		getIdentLine1()->setText( netIdent->getOnlineName() );
		getIdentLine2()->setText( netIdent->getOnlineDescription() );
		getIdentFriendshipButton()->setIcon( m_MyApp.getMyIcons().getFriendshipIcon( netIdent->getMyFriendshipToHim() ) );

		if( netIdent->getMyFriendshipToHim() == eFriendStateFriend && netIdent->getHisFriendshipToMe() == eFriendStateFriend )
		{
			getIdentFriendshipButton()->setIcon( eMyIconFriendJoined );		
		}

		if( getIdentPushToTalkButton() )
		{
			if( m_MyApp.getMembershipAvailableMgr().canPushToTalk( netIdent->getMyOnlineId() ) )
			{
				getIdentPushToTalkButton()->setVisible( true );
			}
			else
			{
				getIdentPushToTalkButton()->setVisible( false );
			}
		}

		if( getIdentLine3() )
		{
			QString truths = QObject::tr( "Truths: " );
			QString dares = QObject::tr( " Dares: " );
			getIdentLine3()->setText( QString( truths + "%1" + dares + "%2" ).arg( netIdent->getTruthCount() ).arg( netIdent->getDareCount() ) );
		}

		if( queryThumb )
		{
			setIdentAvatarThumbnail( netIdent->getAvatarThumbGuid() );
		}

		bool isMyself = netIdent->getMyOnlineId() == m_MyApp.getMyOnlineId();
		if( isMyself )
		{
			getIdentFriendshipButton()->setIcon( eMyIconAdministrator ); // eMyIconAdministrator );
		}

		if( isMyself || netIdent->canDirectConnectToUser() )
		{
			getIdentFriendshipButton()->setNotifyDirectConnectEnabled( true );
		}

		if( isMyself || netIdent->isOnline() )
		{
			getIdentFriendshipButton()->setNotifyOnlineEnabled( true );
			getIdentFriendshipButton()->setNotifyOnlineColor( m_MyApp.getAppTheme().getColor( eLayerNotifyOnlineColor ) );
		}
		else
		{
			getIdentFriendshipButton()->setNotifyOnlineEnabled( true, eMyIconNotifyOfflineOverlay );
			getIdentFriendshipButton()->setNotifyOnlineColor( m_MyApp.getAppTheme().getColor( eLayerNotifyOfflineColor ) );
		}
	}
}

//============================================================================
void IdentLogicInterface::setIdentAvatarButtonVisible( bool visible )
{
	getIdentAvatarButton()->setVisible( visible );
}

//============================================================================
void IdentLogicInterface::setIdentFriendshipButtonVisible( bool visible )
{
	getIdentFriendshipButton()->setVisible( visible );
}

//============================================================================
void IdentLogicInterface::setIdentOfferButtonVisible( bool visible )
{
	getIdentOfferButton()->setVisible( visible );
}

//============================================================================
void IdentLogicInterface::setIdentMenuButtonVisible( bool visible )
{
	getIdentMenuButton()->setVisible( visible );
}

//============================================================================
void IdentLogicInterface::setIdentOnlineState( bool isOnline )
{
	getIdentFriendshipButton()->setNotifyOnlineEnabled( isOnline );
}

//============================================================================
void IdentLogicInterface::setIdentGroupState( bool isInGroup )
{
	getIdentFriendshipButton()->setNotifyInGroupEnabled( isInGroup );
}

//============================================================================
void IdentLogicInterface::setIdentDirectConnectState( bool canDirectConnect )
{
	getIdentFriendshipButton()->setNotifyDirectConnectEnabled( canDirectConnect );
}

//============================================================================
void IdentLogicInterface::setIdentAvatarThumbnail( VxGUID& thumbId )
{
	if( thumbId.isVxGUIDValid() )
	{
		QImage thumbImage;
		if( m_MyApp.getThumbMgr().getThumbImage( thumbId, thumbImage ) )
		{
			getIdentAvatarButton()->setIconOverrideImage( thumbImage );
		}
	}
}

//============================================================================
void IdentLogicInterface::setIdentAvatarIcon( EMyIcons myIcon )
{
	getIdentAvatarButton()->setIcon( myIcon );
}

//============================================================================
void IdentLogicInterface::setIdentFriendshipIcon( EMyIcons myIcon )
{
	getIdentFriendshipButton()->setIcon( myIcon );
}

//============================================================================
void IdentLogicInterface::setIdentOfferIcon( EMyIcons myIcon )
{
	if( getIdentOfferButton() )
	{
		getIdentOfferButton()->setIcon( myIcon );
	}
}

//============================================================================
void IdentLogicInterface::setIdentMenuIcon( EMyIcons myIcon )
{
	getIdentMenuButton()->setIcon( myIcon );
}

//============================================================================
void IdentLogicInterface::slotIdentAvatarButtonClicked( void )
{
	emit signalIdentAvatarButtonClicked();
	onIdentAvatarButtonClicked();
}

//============================================================================
void IdentLogicInterface::slotIdentFrienshipButtonClicked( void )
{
	onIdentFriendshipButtonClicked();
}

//============================================================================
void IdentLogicInterface::slotIdentOfferButtonClicked( void )
{
	emit signalIdentOfferButtonClicked();
	onIdentOfferButtonClicked();
}

//============================================================================
void IdentLogicInterface::slotIdentMenuButtonClicked( void )
{
	emit signalIdentMenuButtonClicked();
	onIdentMenuButtonClicked();
}

//============================================================================
void IdentLogicInterface::onIdentFriendshipButtonClicked( void )
{
	if( m_NetIdent && !m_DisableFriendshipChange )
	{
		QWidget* parentPage = GuiHelpers::findParentPage( dynamic_cast<QWidget*>(parent()) );
		if( parentPage )
		{
			AppletPeerChangeFriendship* applet = dynamic_cast<AppletPeerChangeFriendship*>(m_MyApp.launchApplet( eAppletPeerChangeFriendship, parentPage ));
			if( applet )
			{
				applet->setFriend( m_NetIdent );
			}
		}
	}
}

//============================================================================
void IdentLogicInterface::slotIdentPushToTalkButtonPressed( void )
{
	emit signalIdentPushToTalkButtonPressed();
	onIdentPushToTalkButtonPressed();
}

//============================================================================
void IdentLogicInterface::slotIdentPushToTalkButtonReleased( void )
{
	emit signalIdentPushToTalkButtonReleased();
	onIdentPushToTalkButtonReleased();
}

//============================================================================
void IdentLogicInterface::onIdentPushToTalkButtonPressed( void )
{
	emit signalIdentPushToTalkButtonPressed();
	if( m_NetIdent )
	{
		bool result = m_MyApp.getFromGuiInterface().fromGuiPushToTalk( m_NetIdent->getMyOnlineId(), true );
		if( result && getIdentPushToTalkButton() )
		{
			getIdentPushToTalkButton()->setIconOverrideColor( m_MyApp.getAppTheme().getColor( eLayerNotifyOnlineColor ) );
		}
	}
}

//============================================================================
void IdentLogicInterface::onIdentPushToTalkButtonReleased( void )
{
	emit signalIdentPushToTalkButtonReleased();
	if( m_NetIdent )
	{
		bool result = m_MyApp.getFromGuiInterface().fromGuiPushToTalk( m_NetIdent->getMyOnlineId(), false );
		if( result && getIdentPushToTalkButton() )
		{
			getIdentPushToTalkButton()->setIconOverrideColor( m_MyApp.getAppTheme().getColor( eButtonForegroundNormal ) );
		}
	}
}