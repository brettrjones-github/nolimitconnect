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
// http://www.nolimitconnect.com
//============================================================================

#include "AppCommon.h"
#include "AssetFaceWidget.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
AssetFaceWidget::AssetFaceWidget( QWidget * parent )
	: AssetBaseWidget( GetAppInstance(), parent )
{
	initAssetFaceWidget();
}

//============================================================================
AssetFaceWidget::AssetFaceWidget( AppCommon& appCommon, QWidget * parent )
: AssetBaseWidget( appCommon, parent )
{
	initAssetFaceWidget();
}

//============================================================================
void AssetFaceWidget::initAssetFaceWidget( void )
{
	ui.setupUi( this );
    QSize buttonSize(  GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_ShredButton->setFixedSizeAbsolute( buttonSize );
    ui.m_SendButton->setFixedSizeAbsolute( buttonSize );

	setXferBar( ui.m_XferProgressBar );
	ui.m_ShredButton->setIcon( eMyIconTrash );
	ui.m_SendButton->setVisible( false );
	ui.m_ButtonFrame->setVisible( false );
	ui.m_TagFrame->setVisible( false );

	connect( ui.m_FaceLabel, SIGNAL( clicked() ), this, SLOT( slotAssetWasClicked() ) );
	connect( ui.m_ShredButton, SIGNAL( clicked() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_LeftAvatarBar, SIGNAL( signalShredAsset() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_RightAvatarBar, SIGNAL( signalShredAsset() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_LeftAvatarBar, SIGNAL( signalResendAsset() ), this, SLOT( slotResendAsset() ) );
}

//============================================================================
void AssetFaceWidget::setAssetInfo( AssetBaseInfo& assetInfo )
{
	AssetBaseWidget::setAssetInfo( assetInfo );
	if( false == m_AssetInfo.isValid() )
	{
		return;
	}

	QString faceRes = ":/AppRes/Resources/";
	faceRes += m_AssetInfo.getAssetName().c_str();
	faceRes += ".svg";
	QPixmap faceImage( faceRes );
	if( faceImage.isNull() )
	{
		LogMsg( LOG_ERROR, "FAIL AssetFaceWidget::setAssetInfo null resource %s\n", faceRes.toUtf8().constData() );
		return;
	}

	ui.m_FaceLabel->setPixmap( faceImage );
	ui.m_FaceLabel->setFixedSize( faceImage.width() + 4,  faceImage.height() + 4 );
	if( assetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->setTime( m_AssetInfo.getCreationTime() );
		ui.m_RightSpacer->changeSize( 0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed );
	}
	else
	{
		ui.m_RightAvatarBar->setTime( m_AssetInfo.getCreationTime() );
		ui.m_LeftSpacer->changeSize( 0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed );
	}

	if( assetInfo.isFileAsset() )
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
		ui.m_RightAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
	}
	else
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconTrash );
		ui.m_RightAvatarBar->setShredButtonIcon(  eMyIconTrash );
	}

	calculateHint();
	updateFromAssetInfo();
}

//============================================================================
void AssetFaceWidget::slotAssetWasClicked( void )
{
	bool isVisible = ui.m_ButtonFrame->isVisible();
	ui.m_ButtonFrame->setVisible( !isVisible );
	calculateHint();
}

//============================================================================
void AssetFaceWidget::calculateHint( void )
{
	int totalHeight = ui.m_FaceLabel->height() + 4;
	if( ui.m_TagFrame->isVisible() )
	{
        QFontMetrics thisFontMetrics = fontMetrics();
        int tagTextHeight = GuiHelpers::calculateTextHeight( thisFontMetrics, ui.m_TagTextLabel->text() ) + 4;
		ui.m_TagTextLabel->setFixedHeight( tagTextHeight );
		totalHeight += tagTextHeight;
	}

	if( ui.m_ButtonFrame->isVisible() )
	{
		totalHeight +=  ui.m_ButtonFrame->height() + 4;
	}

	this->setSizeHint( QSize( 100, totalHeight ) );
}

//============================================================================
void AssetFaceWidget::showSendFail( bool show, bool permissionErr )
{
	if( m_AssetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->showSendFail( show, permissionErr );
		ui.m_LeftAvatarBar->showResendButton( show );
	}
	else
	{
		ui.m_RightAvatarBar->showSendFail( show, permissionErr );
		ui.m_RightAvatarBar->showResendButton( show );
	}
}

//============================================================================
void AssetFaceWidget::showResendButton( bool show )
{
	if( m_AssetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->showResendButton( show );
	}
	else
	{
		ui.m_RightAvatarBar->showResendButton( show );
	}
}

//============================================================================
void AssetFaceWidget::showShredder( bool show )
{
	if( m_AssetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->showShredder( show );
	}
	else
	{
		ui.m_RightAvatarBar->showShredder( show );
	}
}

//============================================================================
void AssetFaceWidget::showXferProgress( bool show )
{
	if( m_AssetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->showXferProgress( show );
	}
	else
	{
		ui.m_RightAvatarBar->showXferProgress( show );
	}
}

//============================================================================
void AssetFaceWidget::setXferProgress( int xferProgress )
{
	if( m_AssetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->setXferProgress( xferProgress );
	}
	else
	{
		ui.m_RightAvatarBar->setXferProgress( xferProgress );
	}
}
