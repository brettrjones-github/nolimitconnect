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

#include "AppCommon.h"
#include "AssetTextWidget.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
AssetTextWidget::AssetTextWidget( QWidget * parent )
	: AssetBaseWidget( GetAppInstance(), parent )
{
	initAssetTextWidget();
}

//============================================================================
AssetTextWidget::AssetTextWidget( AppCommon& appCommon, QWidget * parent )
: AssetBaseWidget( appCommon, parent )
{
	initAssetTextWidget();
}

//============================================================================
void AssetTextWidget::initAssetTextWidget( void )
{
	ui.setupUi( this );
    QSize buttonSize( GuiParams::getButtonSize( eButtonSizeSmall ) );
    ui.m_SendButton->setFixedSizeAbsolute( buttonSize );
    ui.m_CopyTextButton->setFixedSizeAbsolute( buttonSize );

	setXferBar( ui.m_XferProgressBar );
	ui.m_CopyTextButton->setIcon( eMyIconEditPaste );

	ui.m_SendButton->setVisible( false );
	ui.m_ButtonFrame->setVisible( false );
	ui.m_TagFrame->setVisible( false );

	connect( ui.m_ChatTextLabel, SIGNAL( clicked() ), this, SLOT( slotAssetWasClicked() ) );
	connect( ui.m_TagTextLabel, SIGNAL( clicked() ), this, SLOT( slotAssetWasClicked() ) );
	connect( ui.m_CopyTextButton, SIGNAL( clicked() ), this, SLOT( slotCopyTextToClipboardButClick() ) );
	connect( ui.m_LeftAvatarBar, SIGNAL( signalShredAsset() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_RightAvatarBar, SIGNAL( signalShredAsset() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_LeftAvatarBar, SIGNAL( signalResendAsset() ), this, SLOT( slotResendAsset() ) );
	calculateHint();
}

//============================================================================
void AssetTextWidget::slotAssetWasClicked( void )
{
	bool isVisible = ui.m_ButtonFrame->isVisible();
	ui.m_ButtonFrame->setVisible( !isVisible );
	calculateHint();
}

//============================================================================
void AssetTextWidget::slotShredAsset( void )
{
	//this->m_Engine.fromGuiAssetAction( eAssetActionShreadFile, m_AssetInfo.getAssetUniqueId(), 0 );
	emit signalShreddingAsset( this );
}

//============================================================================
void AssetTextWidget::setAssetInfo( AssetBaseInfo& assetInfo )
{
	AssetBaseWidget::setAssetInfo( assetInfo );
	if( false == m_AssetInfo.isValid() )
	{
		return;
	}

	ui.m_ChatTextLabel->setText( assetInfo.getAssetName().c_str() );
	ui.m_LeftAvatarBar->setOnlineId( m_AssetInfo.getOnlineId() );
	ui.m_RightAvatarBar->setOnlineId( m_AssetInfo.getOnlineId() );
	if( assetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->setTime( m_AssetInfo.getCreationTime() );
		ui.m_RightSpacer->changeSize( 0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed );
		ui.m_RightAvatarBar->showAvatar( true );
	}
	else
	{
		ui.m_RightAvatarBar->setTime( m_AssetInfo.getCreationTime() );
		ui.m_LeftSpacer->changeSize( 0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed );
		ui.m_LeftAvatarBar->showAvatar( true );
	}

	if( assetInfo.isFileAsset() )
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
		ui.m_RightAvatarBar->setShredButtonIcon(eMyIconShredderNormal );
	}
	else
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconTrash );
		ui.m_RightAvatarBar->setShredButtonIcon( eMyIconTrash );
	}

	calculateHint();
	updateFromAssetInfo();
}

//============================================================================
void AssetTextWidget::calculateHint( void )
{
    QFontMetrics thisFontMetrics = fontMetrics();
    int totalHeight = GuiHelpers::calculateTextHeight( thisFontMetrics, ui.m_ChatTextLabel->text() ) + 4;
	ui.m_ChatTextLabel->setFixedHeight( totalHeight );
	if( ui.m_TagFrame->isVisible() )
	{
        int tagTextHeight = GuiHelpers::calculateTextHeight( thisFontMetrics, ui.m_TagTextLabel->text() ) + 4;
		ui.m_TagTextLabel->setFixedHeight( tagTextHeight );
		totalHeight += tagTextHeight;
	}

	if( ui.m_ButtonFrame->isVisible() )
	{
		totalHeight +=  ui.m_ButtonFrame->height() + 4;
	}

	this->setSizeHint( QSize( 100, totalHeight + 60 ) );
}

//============================================================================
void AssetTextWidget::showSendFail( bool show, bool permissionErr )
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
void AssetTextWidget::showResendButton( bool show )
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
void AssetTextWidget::showShredder( bool show )
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
void AssetTextWidget::showXferProgress( bool show )
{
	if( m_AssetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->showShredder( show );
		ui.m_LeftAvatarBar->showXferProgress( show );
	}
	else
	{
		ui.m_RightAvatarBar->showShredder( show );
		ui.m_RightAvatarBar->showXferProgress( show );
	}
}

//============================================================================
void AssetTextWidget::setXferProgress( int xferProgress )
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

//============================================================================
void AssetTextWidget::slotCopyTextToClipboardButClick( void )
{
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText( ui.m_ChatTextLabel->text() );
	QMessageBox::information( this, QObject::tr( "Clipboard" ), QObject::tr( "Text was copied to clipboard" ), QMessageBox::Ok );
}