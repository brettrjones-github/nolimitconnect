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

#include "AssetVideoWidget.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
AssetVideoWidget::AssetVideoWidget( QWidget * parent )
	: AssetBaseWidget( GetAppInstance(), parent )
{
	initAssetVideoWidget();
}

//============================================================================
AssetVideoWidget::AssetVideoWidget( AppCommon& appCommon, QWidget * parent )
	: AssetBaseWidget( appCommon, parent )
{
	initAssetVideoWidget();
}

//============================================================================
void AssetVideoWidget::initAssetVideoWidget( void )
{
	ui.setupUi( this );
	setXferBar( ui.m_XferProgressBar );
	ui.m_TagFrame->setVisible( false );

	ui.m_VidWidget->showAllControls( false );
	ui.m_VidWidget->disablePreview( true );
	ui.m_VidWidget->disableRecordControls( true );

	ui.m_PlayPauseButton->setIcons( eMyIconPlayNormal );
	ui.m_PlayPauseButton->setPressedSound( eSndDefNone );
	ui.m_PlayPosSlider->setRange( 0, 100000 );

	connect( ui.m_PlayPauseButton,	SIGNAL(clicked()),						this, SLOT(slotPlayButtonClicked()) );
	connect( ui.m_LeftAvatarBar,	SIGNAL(signalShredAsset()),				this, SLOT(slotShredAsset()) );
	connect( ui.m_RightAvatarBar,	SIGNAL(signalShredAsset()),				this, SLOT(slotShredAsset()) );
	connect( ui.m_PlayPosSlider,	SIGNAL(sliderPressed()),				this, SLOT(slotSliderPressed()) );
	connect( ui.m_PlayPosSlider,	SIGNAL(sliderReleased()),				this, SLOT(slotSliderReleased()) );

	connect( this,					SIGNAL(signalPlayProgress(int)),		this, SLOT(slotPlayProgress(int)) );
	connect( this,					SIGNAL(signalPlayEnd()),				this, SLOT(slotPlayEnd()) );
	connect( ui.m_LeftAvatarBar,	SIGNAL(signalResendAsset()),			this, SLOT(slotResendAsset()) );
    ui.m_VidWidget->setVidImageRotation( 0 );
}

//============================================================================
void AssetVideoWidget::setAssetInfo( AssetBaseInfo& assetInfo )
{
	AssetBaseWidget::setAssetInfo( assetInfo );
	ui.m_TagLabel->setAssetInfo( &getAssetInfo() );
	ui.m_FileNameLabel->setText( getAssetInfo().getRemoteAssetName().c_str() );

	ui.m_VidWidget->setVideoFeedId( m_AssetInfo.getAssetUniqueId(), getAppModule() );
	if( ui.m_TagLabel->text().isEmpty() )
	{
		ui.m_TagLabel->setVisible( false );
		ui.m_TagTitleLabel->setVisible( false );
		this->setSizeHint( QSize( 100 * GuiParams::getGuiScale(), 224 * GuiParams::getGuiScale() - 16 ) );
	}
	else
	{
		ui.m_TagLabel->setVisible( true );
		ui.m_TagTitleLabel->setVisible( true );
		this->setSizeHint( QSize( 100 * GuiParams::getGuiScale(), 224 * GuiParams::getGuiScale() ) );
	}

	ui.m_LeftAvatarBar->setOnlineId( m_AssetInfo.getOnlineId() );
	ui.m_RightAvatarBar->setOnlineId( m_AssetInfo.getOnlineId() );
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
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconShredderNormal  );
		ui.m_RightAvatarBar->setShredButtonIcon( eMyIconShredderNormal  );
	}
	else
	{
		ui.m_LeftAvatarBar->setShredButtonIcon( eMyIconTrash );
		ui.m_RightAvatarBar->setShredButtonIcon( eMyIconTrash );
	}

	updateFromAssetInfo();
}

//============================================================================
void AssetVideoWidget::showEvent(QShowEvent * showEvent)
{
	AssetBaseWidget::showEvent(showEvent);
	if( ( false == VxIsAppShuttingDown() )
		&& m_AssetInfo.isValid()
		&& !m_IsPlaying )
	{
		setReadyForCallbacks( true );
		m_Engine.fromGuiAssetAction( eAssetActionPlayOneFrame, m_AssetInfo, 0 );
	}
}

//============================================================================
void AssetVideoWidget::resizeEvent( QResizeEvent * ev )
{
	AssetBaseWidget::resizeEvent( ev );
	if( ( false == VxIsAppShuttingDown() )
		&& m_AssetInfo.isValid()
		&& !m_IsPlaying
		&& isVisible() )
	{
		setReadyForCallbacks( true );
		m_Engine.fromGuiAssetAction( eAssetActionPlayOneFrame, m_AssetInfo, 0 );
	}
}

//============================================================================
void AssetVideoWidget::toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	AssetBaseWidget::toGuiClientAssetAction( assetAction, assetId, pos0to100000 );
	switch( assetAction )
	{
	case eAssetActionPlayProgress:
		if( false == m_SliderIsPressed )
		{
			updateGuiPlayControls( true );
			ui.m_PlayPosSlider->setValue( pos0to100000 );
		}

		break;

	case eAssetActionPlayEnd:
		if( false == m_SliderIsPressed )
		{
			updateGuiPlayControls( false );
		}

		break;

	default:
		break;
	}
}

//============================================================================
void AssetVideoWidget::slotSliderPressed( void )
{
	m_SliderIsPressed = true;
}

//============================================================================
void AssetVideoWidget::slotSliderReleased( void )
{
	m_SliderIsPressed = false;
	int posVal = ui.m_PlayPosSlider->value();
	startMediaPlay( posVal );
}

//============================================================================
void AssetVideoWidget::slotPlayButtonClicked( void )
{
	if( m_IsPlaying )
	{
		stopMediaIfPlaying();
	}
	else
	{
		startMediaPlay( 0 );
	}
}

//========================================================================
void AssetVideoWidget::startMediaPlay( int startPos )
{	
	bool playStarted = m_Engine.fromGuiAssetAction( eAssetActionPlayBegin, m_AssetInfo, startPos );
	updateGuiPlayControls( playStarted );
	if( false == playStarted )
	{
		m_MyApp.toGuiStatusMessage( "Video Play FAILED TO Begin" );
	}
}

//========================================================================
void AssetVideoWidget::updateGuiPlayControls( bool isPlaying )
{	
	if( m_IsPlaying != isPlaying )
	{
		m_IsPlaying = isPlaying;
		if( m_IsPlaying )
		{
			// start playing
			ui.m_PlayPauseButton->setIcons( eMyIconPauseNormal );
			setReadyForCallbacks( true );
		}
		else
		{
			// stop playing
			ui.m_PlayPauseButton->setIcons( eMyIconPlayNormal );
			ui.m_PlayPosSlider->setValue( 0 );
		}
	}
}

//============================================================================
void AssetVideoWidget::onActivityStop( void )
{
	setReadyForCallbacks( false );
	stopMediaIfPlaying();
}

//============================================================================
void AssetVideoWidget::stopMediaIfPlaying( void )
{
	if( m_IsPlaying )
	{
		m_MyApp.toGuiStatusMessage( "" );
		m_Engine.fromGuiAssetAction( eAssetActionPlayEnd, m_AssetInfo, 0 );
	}

	updateGuiPlayControls( false );
}

//============================================================================
void AssetVideoWidget::setReadyForCallbacks( bool isReady )
{
	if( m_ActivityCallbacksEnabled != isReady )
	{
		m_ActivityCallbacksEnabled = isReady;
		m_MyApp.wantToGuiActivityCallbacks( this, isReady );
	}
}

//============================================================================
void AssetVideoWidget::slotShredAsset( void )
{
	onActivityStop();
	emit signalShreddingAsset( this );
}

//============================================================================
void AssetVideoWidget::toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * pu8Jpg, uint32_t u32JpgLen, int motion0To100000 )
{
	ui.m_VidWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgLen, motion0To100000 );
}

//============================================================================
int AssetVideoWidget::toGuiClientPlayVideoFrame( VxGUID& onlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    return ui.m_VidWidget->playVideoFrame( onlineId, picBuf, picBufLen, picWidth, picHeight );
}

//============================================================================
void AssetVideoWidget::slotPlayProgress( int pos0to100000 )
{
	if( m_IsPlaying && ( false == m_SliderIsPressed ) )
	{
		ui.m_PlayPosSlider->setValue( pos0to100000 );
	}
}

//============================================================================
void AssetVideoWidget::slotPlayEnd( void )
{
	//updateGuiPlayControls( false );
}

//============================================================================
void AssetVideoWidget::showSendFail( bool show, bool permissionErr )
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
void AssetVideoWidget::showResendButton( bool show )
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
void AssetVideoWidget::showShredder( bool show )
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
void AssetVideoWidget::showXferProgress( bool show )
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
void AssetVideoWidget::setXferProgress( int xferProgress )
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
