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


#include "SoundCtrlWidget.h"
#include "AppCommon.h"

#include "AppGlobals.h"
#include "MySndMgr.h"
#include "MyIcons.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

//============================================================================
SoundCtrlWidget::SoundCtrlWidget(QWidget *parent)
: QWidget(parent)
, m_MyApp( GetAppInstance() )
, m_SndMgr(m_MyApp.getSoundMgr())
, m_SpeakerIsMuted( false )
, m_MicIsMuted( false )
{
	ui.setupUi(this);
	connect( ui.m_MuteMicButton, SIGNAL(clicked()), this, SLOT(slotMuteMicButtonClicked()) );
	connect( ui.m_MuteSpeakerButton, SIGNAL(clicked()), this, SLOT(slotMuteSpeakerButtonClicked()) );
	connect(this, SIGNAL(signalSoundLevel(int)), ui.SoundBar, SLOT(setValue(int)) );
	setMicMute( false );
	setSpeakerMute( false );
}

//============================================================================
MyIcons&  SoundCtrlWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void SoundCtrlWidget::startup(	AppCommon&		app,
								EPluginType			ePluginType,
								VxNetIdent *		netIdent )
{
//	m_MyApp			= &app;
//	m_SndMgr		= m_MyApp.getSoundMgr(),
	//m_ePluginType	= ePluginType;
	//m_Ident			= netIdent;
	setSoundChannelId( netIdent->getMyOnlineId() );

	connect(	&m_SndMgr, 
		SIGNAL(signalMicrophoneAudioData(QByteArray)),
		this,
		SLOT(slotSoundPcmData(QByteArray)) );
}

//============================================================================
void SoundCtrlWidget::setSoundLabel( QString strText )
{
	ui.SoundLabel->setText( strText );
}

//============================================================================
void SoundCtrlWidget::slotSoundPcmData( QByteArray audioData )
{
	if( false == m_MicIsMuted )
	{
		int soundLevel = 0;
		int16_t * pcmData = (int16_t *)audioData.data();
		uint16_t pcmSampleCnt = (uint16_t)(audioData.length()/2);

		for( int i = 0; i < pcmSampleCnt; ++i )
		{
			soundLevel +=  pcmData[i] < 0 ? -pcmData[i] : pcmData[i];
		}

		soundLevel = ((soundLevel * 100) / pcmSampleCnt)/32767;
		emit signalSoundLevel( soundLevel );
	}
}

//============================================================================
void SoundCtrlWidget::slotMuteMicButtonClicked()
{
	setMicMute( !m_MicIsMuted );
}

//============================================================================
void SoundCtrlWidget::slotMuteSpeakerButtonClicked()
{
	setSpeakerMute( !m_SpeakerIsMuted );
}

//============================================================================
void SoundCtrlWidget::setMicMute( bool mute )
{
	m_MicIsMuted = mute;
	emit signalMicIsMuted( m_MicIsMuted );
	ui.m_MuteMicButton->setIcon( m_MicIsMuted ? eMyIconMicrophoneCancelNormal : eMyIconMicrophoneOn );
	if( m_MicIsMuted )
	{
		ui.SoundBar->setValue(0);
		m_SndMgr.stopMicrophoneRecording();
	}
	else
	{
		m_SndMgr.startMicrophoneRecording();
	}
}

//============================================================================
void SoundCtrlWidget::setSpeakerMute( bool mute )
{
	m_SpeakerIsMuted = mute;
	emit signalSpeakerIsMuted( m_SpeakerIsMuted );
	ui.m_MuteSpeakerButton->setIcon( m_SpeakerIsMuted ? eMyIconSpeakerOff : eMyIconSpeakerOn );
}

//============================================================================
void SoundCtrlWidget::setSoundChannelId( VxGUID oChannelId )
{
	m_ChannelId = oChannelId;
}
