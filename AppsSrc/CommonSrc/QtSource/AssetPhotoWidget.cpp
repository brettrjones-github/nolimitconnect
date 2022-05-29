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
#include "AssetPhotoWidget.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
AssetPhotoWidget::AssetPhotoWidget( QWidget * parent )
	: AssetBaseWidget( GetAppInstance(), parent )
{
	initAssetPhotoWidget();
}

//============================================================================
AssetPhotoWidget::AssetPhotoWidget( AppCommon& appCommon, QWidget * parent )
	: AssetBaseWidget( appCommon, parent )
{
	initAssetPhotoWidget();
}

//============================================================================
void AssetPhotoWidget::initAssetPhotoWidget( void )
{
	ui.setupUi( this );
	setXferBar( ui.m_XferProgressBar );
	ui.m_ShredButton->setIcon( eMyIconShredderNormal );

	ui.m_SendButton->setVisible( false );
	ui.m_ButtonFrame->setVisible( false );
	ui.m_TagLabel->setVisible( false );
	ui.m_VidWidget->disablePreview( true );
	ui.m_VidWidget->disableRecordControls( true );
	
	connect( ui.m_VidWidget,		SIGNAL(clicked()),				this, SLOT(slotAssetWasClicked()) );
	connect( ui.m_ShredButton,		SIGNAL(clicked()),				this, SLOT(slotShredAsset()) );
	connect( ui.m_LeftAvatarBar,	SIGNAL(signalShredAsset()),		this, SLOT(slotShredAsset()) );
	connect( ui.m_RightAvatarBar,	SIGNAL(signalShredAsset()),		this, SLOT(slotShredAsset()) );
	connect( ui.m_LeftAvatarBar,	SIGNAL( signalResendAsset() ),	this, SLOT( slotResendAsset() ) );
}

//============================================================================
void AssetPhotoWidget::setAssetInfo( AssetBaseInfo& assetInfo )
{
	AssetBaseWidget::setAssetInfo( assetInfo );
	this->setSizeHint( QSize( 100, 224 - 16 ) );
	ui.m_VidWidget->setImageFromFile( assetInfo.getAssetName().c_str() );
	if( assetInfo.isMine() )
	{
		ui.m_LeftAvatarBar->setTime( m_AssetInfo.getCreationTime() );
	}
	else
	{
		ui.m_RightAvatarBar->setTime( m_AssetInfo.getCreationTime() );
	}

	if( assetInfo.isFileAsset() )
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
		ui.m_RightAvatarBar->setShredButtonIcon( eMyIconShredderNormal );
	}
	else
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconTrash );
		ui.m_RightAvatarBar->setShredButtonIcon( eMyIconTrash );
	}

	updateFromAssetInfo();
}

//============================================================================
void AssetPhotoWidget::slotAssetWasClicked( void )
{
	bool isVisible = ui.m_ButtonFrame->isVisible();
	ui.m_ButtonFrame->setVisible( !isVisible );
}

//============================================================================
void AssetPhotoWidget::showSendFail( bool show, bool permissionErr )
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
void AssetPhotoWidget::showResendButton( bool show )
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
void AssetPhotoWidget::showShredder( bool show )
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
void AssetPhotoWidget::showXferProgress( bool show )
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
void AssetPhotoWidget::setXferProgress( int xferProgress )
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
