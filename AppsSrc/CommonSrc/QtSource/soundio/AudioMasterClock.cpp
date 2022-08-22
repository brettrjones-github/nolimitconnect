//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include "AudioMasterClock.h"

#include "AppCommon.h"
#include "AudioIoMgr.h"

const int FRAME_INTERVAL_US = 80000;

//============================================================================
AudioMasterClock::AudioMasterClock( AudioIoMgr& audioIoMgr, QObject* parent )
	: QObject( parent )
	, m_AudioIoMgr( audioIoMgr )
	, m_MyApp( audioIoMgr.getMyApp() )
	, m_AudioTimer(new QTimer(this))
{
	m_AudioTimer->setInterval( 10 );
	connect( m_AudioTimer, SIGNAL(timeout()), this, SLOT(slotAudioTimerTimeout()) );
}

//============================================================================
void AudioMasterClock::audioMasterClockStartup( void )
{
	m_AudioTimer->start();
}

//============================================================================
void AudioMasterClock::audioMasterClockShutdown( void )
{
	m_AudioTimer->stop();
}

//============================================================================
void AudioMasterClock::audioSpeakerReadUs( int64_t readSizeUs, bool resetTimer )
{
	if( resetTimer )
	{
		m_SpeakerConsumedUs = 0;
	}

	m_SpeakerConsumedUs += readSizeUs;
}

//============================================================================
void AudioMasterClock::slotAudioTimerTimeout( void )
{
	int64_t timeNow = m_MyApp.elapsedMilliseconds();
	if( !m_NextFrameTimeMs || timeNow > m_NextFrameTimeMs + 160 )
	{
		// speaker audio was paused or something
		m_NextFrameTimeMs = timeNow;
		m_SpeakerConsumedUs = FRAME_INTERVAL_US;
	}

	if( m_SpeakerConsumedUs >= FRAME_INTERVAL_US && timeNow >= m_NextFrameTimeMs )
	{
		m_NextFrameTimeMs += 80;
		m_SpeakerConsumedUs -= FRAME_INTERVAL_US;
		m_AudioIoMgr.frame80msElapsed();
	}
}