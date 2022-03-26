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

#include "AssetVoiceWidget.h"
#include "AppCommon.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
AssetVoiceWidget::AssetVoiceWidget( QWidget * parent )
	: AssetBaseWidget( GetAppInstance(), parent )
	, m_ActivityCallbacksEnabled( false )
	, m_IsPlaying( false )
	, m_SliderIsPressed( false )
{
	initAssetVoiceWidget();
}

//============================================================================
AssetVoiceWidget::AssetVoiceWidget( AppCommon& appCommon, QWidget * parent )
: AssetBaseWidget( appCommon, parent )
, m_ActivityCallbacksEnabled( false )
, m_IsPlaying( false )
, m_SliderIsPressed( false )
{
	initAssetVoiceWidget();
}

//============================================================================
void AssetVoiceWidget::initAssetVoiceWidget( void )
{
	ui.setupUi( this );
    QSize buttonSize( 24, 24 );
    ui.m_PlayPauseButton->setFixedSizeAbsolute( buttonSize );

	setXferBar( ui.m_XferProgressBar );

	ui.m_PlayPauseButton->setIcons( eMyIconPlayNormal );
	ui.m_PlayPauseButton->setPressedSound( eSndDefNone );
	ui.m_PlayPosSlider->setRange( 0, 100000 );
	ui.m_PlayPosSlider->setMinimum( 0 );
	ui.m_PlayPosSlider->setMaximum( 100000 );
	ui.m_TagFrame->setVisible( false );

	connect( ui.m_PlayPauseButton, SIGNAL( clicked() ), this, SLOT( slotPlayButtonClicked() ) );
	connect( ui.m_LeftAvatarBar, SIGNAL( signalShredAsset() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_RightAvatarBar, SIGNAL( signalShredAsset() ), this, SLOT( slotShredAsset() ) );
	connect( ui.m_PlayPosSlider, SIGNAL( sliderPressed() ), this, SLOT( slotSliderPressed() ) );
	connect( ui.m_PlayPosSlider, SIGNAL( sliderReleased() ), this, SLOT( slotSliderReleased() ) );
	connect( ui.m_LeftAvatarBar, SIGNAL( signalResendAsset() ), this, SLOT( slotResendAsset() ) );

    ui.m_PlayPosSlider->setVisible( true );
}

//============================================================================
void AssetVoiceWidget::setAssetInfo( AssetBaseInfo& assetInfo )
{
	AssetBaseWidget::setAssetInfo( assetInfo );
	ui.m_TagLabel->setAssetInfo( &getAssetInfo() );
	ui.m_FileNameLabel->setText( getAssetInfo().getRemoteAssetName().c_str() );
	//ui.m_ShredButton->setShredFile( getAssetInfo().getRemoteAssetName().c_str() );
	//ui.m_LeftAvatarBar->setShredFile( getAssetInfo().getRemoteAssetName().c_str() );
	//ui.m_RightAvatarBar->setShredFile( getAssetInfo().getRemoteAssetName().c_str() );

	if( ui.m_TagLabel->text().isEmpty() )
	{
		ui.m_TagLabel->setVisible( false );
		ui.m_TagTitleLabel->setVisible( false );
        this->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ), ( int )( 46 * GuiParams::getGuiScale() ) ) );
    }
	else
	{
		ui.m_TagLabel->setVisible( true );
		ui.m_TagTitleLabel->setVisible( true );
        this->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ), ( int )( 62 * GuiParams::getGuiScale() ) ) );
	}

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
void AssetVoiceWidget::toGuiClientAssetAction( EAssetAction assetAction, VxGUID& assetId, int pos0to100000 )
{
	switch( assetAction )
	{
	case eAssetActionPlayProgress:
		if( false == m_SliderIsPressed )
		{
			ui.m_PlayPosSlider->setValue( pos0to100000 );
			updateGuiPlayControls( true );
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
void AssetVoiceWidget::slotSliderPressed( void )
{
	m_SliderIsPressed = true;
}

//============================================================================
void AssetVoiceWidget::slotSliderReleased( void )
{
	int posVal = ui.m_PlayPosSlider->value();
	m_Engine.fromGuiAssetAction( eAssetActionPlayBegin, m_AssetInfo, posVal );
	m_SliderIsPressed = false;
}

//============================================================================
void AssetVoiceWidget::slotPlayButtonClicked( void )
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
void AssetVoiceWidget::startMediaPlay( int startPos )
{	
    updateGuiPlayControls( true ); // assume will start playing
    setReadyForCallbacks( true );
	bool playStarted = m_Engine.fromGuiAssetAction( eAssetActionPlayBegin, m_AssetInfo, startPos );
	updateGuiPlayControls( playStarted ); // in case failed to start
    setReadyForCallbacks( playStarted );
	if( false == playStarted )
	{
		m_MyApp.toGuiStatusMessage( "Voice Play FAILED TO Begin" );
	}
}

//========================================================================
void AssetVoiceWidget::updateGuiPlayControls( bool isPlaying )
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
            setReadyForCallbacks( false );
			ui.m_PlayPauseButton->setIcons( eMyIconPlayNormal );
			ui.m_PlayPosSlider->setValue( 0 );        
		}
	}
}

//============================================================================
void AssetVoiceWidget::onActivityStop( void )
{
	setReadyForCallbacks( false );
	stopMediaIfPlaying();
}

//============================================================================
void AssetVoiceWidget::stopMediaIfPlaying( void )
{
	if( m_IsPlaying )
	{
		m_MyApp.toGuiStatusMessage( "" );
		m_Engine.fromGuiAssetAction( eAssetActionPlayEnd, m_AssetInfo, 0 );
	}

	updateGuiPlayControls( false );
}

//============================================================================
void AssetVoiceWidget::setReadyForCallbacks( bool isReady )
{
	if( m_ActivityCallbacksEnabled != isReady )
	{
		m_ActivityCallbacksEnabled = isReady;
		m_MyApp.wantToGuiActivityCallbacks( this, m_ActivityCallbacksEnabled );
	}
}

//============================================================================
void AssetVoiceWidget::showSendFail( bool show, bool permissionErr )
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
void AssetVoiceWidget::showResendButton( bool show )
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
void AssetVoiceWidget::showShredder( bool show )
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
void AssetVoiceWidget::showXferProgress( bool show )
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
void AssetVoiceWidget::setXferProgress( int xferProgress )
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
