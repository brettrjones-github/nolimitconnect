//============================================================================
// Copyright (C) 2015 Brett R. Jones
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


#include "AvatarBar.h"
#include "AppCommon.h"
#include "MyIcons.h"
#include "GuiParams.h"
#include "GuiUser.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTimeUtil.h>

//============================================================================
AvatarBar::AvatarBar( QWidget * parent )
: QWidget( parent )
, m_MyApp( GetAppInstance() )
{
	ui.setupUi( this );
    QSize buttonSize( GuiParams::getButtonSize() );
    ui.m_ShredButton->setFixedSizeAbsolute( buttonSize );
    ui.m_ResendButton->setFixedSize( buttonSize );

	this->setFixedWidth( GuiParams::getButtonSize().width() + 20 );
	setShredButtonIcon( eMyIconShredderNormal  );
	ui.m_ProgressSpinner->setVisible( false );
	ui.m_ShredButton->setVisible( false );
	ui.m_TimeLabel->setVisible( false );
	ui.m_ResendButton->setVisible( false );

	connect( ui.m_ShredButton, SIGNAL( clicked() ),		this, SIGNAL( signalShredAsset() ) );
	connect( ui.m_ResendButton, SIGNAL( clicked() ),	this, SIGNAL( signalResendAsset() ) );
}

//============================================================================
MyIcons& AvatarBar::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void AvatarBar::setTime( time_t creationTime )
{
	std::string timeStr = VxTimeUtil::formatTimeStampIntoHoursAndMinutes( creationTime );
	ui.m_TimeLabel->setText( timeStr.c_str() );
	ui.m_TimeLabel->setVisible( true );
	showAvatar( false );
}

//============================================================================
void AvatarBar::setOnlineId( VxGUID& onlineId )
{
	m_OnlineId = onlineId;
	if( !m_ThumbnailQueried )
	{
		m_ThumbnailQueried = true;
		GuiUser* guiUser = m_MyApp.getUserMgr().getOrQueryUser( onlineId );
		if( guiUser )
		{
			QImage image;
			VxGUID thumbId = guiUser->getAvatarThumbId();
			if( thumbId.isVxGUIDValid() && m_MyApp.getThumbMgr().getAvatarImage( thumbId, image ) )
			{
				ui.m_Avatar->setImage( image );
			}
		}
	}
}

//============================================================================
void AvatarBar::showAvatar( bool show )
{
	ui.m_Avatar->setVisible( show );
}

//============================================================================
void AvatarBar::setShredButtonIcon( EMyIcons iconTrash )
{
	ui.m_ShredButton->setIcon( iconTrash );
}

//============================================================================
void AvatarBar::setShredFile( QString fileName )
{
	//ui.m_ShredButton->setShredFile( fileName );
}

//============================================================================
void AvatarBar::showSendFail( bool show, bool permissionErr )
{
	ui.m_Avatar->setVisible( show );
	if( show )
	{
		if( permissionErr )
		{
			ui.m_Avatar->setImage( ":/AppRes/Resources/msg_state_fail_permission.png" );
		}
		else
		{
			ui.m_Avatar->setImage( ":/AppRes/Resources/msg_state_fail_resend.png" );
		}
	}

	if( permissionErr )
	{
		showResendButton( false );
	}
	else
	{
		showResendButton( show );
	}
}

//============================================================================
void AvatarBar::showResendButton( bool show )
{
	ui.m_ResendButton->setVisible( show );
	if( show )
	{
		ui.m_ResendButton->setIcon( getMyIcons().getIcon( eMyIconSendArrowNormal ) );
	}
}

//============================================================================
void AvatarBar::showShredder( bool show )
{
	ui.m_ShredButton->setVisible( show );
}

//============================================================================
void AvatarBar::showXferProgress( bool show )
{
	ui.m_ProgressSpinner->setVisible( show );
	if( show )
	{
		ui.m_ProgressSpinner->setImage( 48, 48, ":/AppRes/Resources/spinner-clockwise.svg"  );
	}
}

//============================================================================
void AvatarBar::setXferProgress( int xferProgress )
{
	ui.m_ProgressSpinner->setValue( xferProgress );
}
