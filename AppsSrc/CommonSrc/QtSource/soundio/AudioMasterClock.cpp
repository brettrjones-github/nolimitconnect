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
const int FRAME_INTERVAL_MS = 80;

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
void AudioMasterClock::enableMasterClock( bool enable )
{
	if( enable )
	{
		m_NextFrameTimeMs = GetHighResolutionTimeMs() + FRAME_INTERVAL_MS;
		m_SpeakerConsumedTime = 0;
		m_MicConsumedTime = 0;
		m_MicConsumedTotal = 0;
		m_SpeakerConsumedTotal = 0;
		m_AudioTimer->start();
	}
	else
	{
		m_AudioTimer->stop();
		m_SpeakerConsumedTime = 0;
		m_MicConsumedTime = 0;
	}
}

//============================================================================
void AudioMasterClock::resetMasterClock( void )
{
	int64_t timeNow = GetHighResolutionTimeMs();
	LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout reset timer now %d next %d mic consumed %d consumed %d", (int)timeNow, (int)m_NextFrameTimeMs, (int)m_MicConsumedTime, (int)m_SpeakerConsumedTime );
	m_NextFrameTimeMs = timeNow + FRAME_INTERVAL_MS;
	m_MicConsumedTime = 0;
	m_SpeakerConsumedTime = 0;
}

//============================================================================
void AudioMasterClock::audioMicWriteDurationTime( int64_t writeDurationMs )
{
	m_MicConsumedTime += writeDurationMs;
	m_MicConsumedTotal += writeDurationMs;
	
	if( m_ResetOnFirstMicWrite )
	{
		m_ResetOnFirstMicWrite = false;
		m_NextFrameTimeMs = GetHighResolutionTimeMs() + FRAME_INTERVAL_MS;
		m_MicConsumedTime = 0;
	}

	int64_t timeNow = GetHighResolutionTimeMs();
	static int64_t lastTime = 0;
	int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
	lastTime = timeNow;

	if( timeElapsed && (timeElapsed < 10 || timeElapsed > 20) )
	{
		// LogMsg( LOG_VERBOSE, "AudioMasterClock::audioMicWriteDurationTime invalid elapsed %d ms", timeElapsed );
	}
}

//============================================================================
void AudioMasterClock::audioMicWriteSampleCnt(int64_t writeSampleCnt )
{
	m_MicSamplesTotal += writeSampleCnt;
}

//============================================================================
void AudioMasterClock::audioSpeakerReadDurationTime( int64_t readSizeTime )
{
	m_SpeakerConsumedTime += readSizeTime;
	m_SpeakerConsumedTotal += readSizeTime;
	if( m_ResetOnFirstSpeakerWrite )
	{
		m_ResetOnFirstSpeakerWrite = false;
		m_NextFrameTimeMs = GetHighResolutionTimeMs() + FRAME_INTERVAL_MS;
		m_SpeakerConsumedTime = FRAME_INTERVAL_MS;
	}

	int64_t timeNow = GetHighResolutionTimeMs();
	static int64_t lastTime = 0;
	int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
	lastTime = timeNow;

	if( timeElapsed && ( timeElapsed < 200 || timeElapsed > 500 ) )
	{
		LogMsg( LOG_VERBOSE, "AudioMasterClock::audioSpeakerReadDurationTime invalid elapsed %d ms", timeElapsed );
	}
}

//============================================================================
void AudioMasterClock::audioSpeakerReadSampleCnt( int64_t readSampleCnt )
{
	m_SpeakerSamplesTotal += readSampleCnt;
}

//============================================================================
void AudioMasterClock::slotAudioTimerTimeout( void )
{
	int64_t timeNow = GetHighResolutionTimeMs();

	if( !m_NextFrameTimeMs || timeNow > m_NextFrameTimeMs + 300 )
	{
		// audio was paused or something
		resetMasterClock();
	}

	if( timeNow >= m_NextFrameTimeMs )
	{
		if( m_MicDeviceEnabled )
		{
			static int64_t lastTime = 0;
			int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
			lastTime = timeNow;

			if( m_MicConsumedTime >= FRAME_INTERVAL_MS )
			{
				// LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout frame80msElapsed elapsed %d ms diff %d mic consumed %d", timeElapsed, (int)(timeNow - m_NextFrameTimeMs), (int)m_MicConsumedTime );

				static int frameCnt = 0;
				frameCnt++;
				if( frameCnt > 10 )
				{
					frameCnt = 0;
					if( m_AudioIoMgr.getAudioTimingEnable() )
					{
						LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout mic now %d ms consumed totals mic %lld speaker %lld diff %lld", (int)m_MicConsumedTime, m_MicConsumedTotal, m_SpeakerConsumedTotal, std::abs( m_SpeakerConsumedTotal - m_MicConsumedTotal ) );
					}

					if( m_AudioIoMgr.getSampleCntDebugEnable() )
					{
						LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout sample totals mic %lld speaker %lld diff %lld", m_MicSamplesTotal, m_SpeakerSamplesTotal, std::abs( m_SpeakerSamplesTotal - m_MicSamplesTotal ) );
					}
				}

				m_NextFrameTimeMs += 80;
				m_MicConsumedTime -= FRAME_INTERVAL_MS;
				m_AudioIoMgr.frame80msElapsed();
				if( m_MicConsumedTime > 16 )
				{
					// either GetHighResolutionTimeMs is not accurate or microphone sample rate > 48000hz .. try to catch up
					m_NextFrameTimeMs--;
					if( m_AudioIoMgr.getAudioTimingEnable() )
					{
						LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout mic consumed %d to large elapsed %d ms diff %d ", (int)m_MicConsumedTime, timeElapsed, (int)(timeNow - m_NextFrameTimeMs) );
					}
				}
			}
			else
			{
				LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout skipping mic elapsed %d ms diff %d mic consumed %d", timeElapsed, (int)(timeNow - m_NextFrameTimeMs), (int)m_MicConsumedTime );
			}
		}
		else if( m_SpeakerDeviceEnabled )
		{
			static int64_t lastTime = 0;
			int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
			lastTime = timeNow;

			if( m_SpeakerConsumedTime >= FRAME_INTERVAL_MS )
			{
				// LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout frame80msElapsed elapsed %d ms diff %d consumed %d", timeElapsed, (int)(timeNow - m_NextFrameTimeMs), (int)m_SpeakerConsumedTime );

				m_NextFrameTimeMs += 80;
				m_SpeakerConsumedTime -= FRAME_INTERVAL_MS;
				m_AudioIoMgr.frame80msElapsed();
			}
			else
			{
				LogMsg( LOG_VERBOSE, "AudioMasterClock::slotAudioTimerTimeout skipping speaker elapsed %d ms diff %d speaker consumed %d", timeElapsed, (int)(timeNow - m_NextFrameTimeMs), (int)m_SpeakerConsumedTime );
			}
		}
		else
		{
			LogMsg( LOG_WARNING, "AudioMasterClock::slotAudioTimerTimeout called with no audio device enabled" );
		}
	}
}

//============================================================================
void AudioMasterClock::microphoneDeviceEnabled( bool isEnabled )
{
	m_MicDeviceEnabled = isEnabled;
	updateDeviceEnable();
	if( isEnabled )
	{
		m_ResetOnFirstMicWrite = true;
	}
}

//============================================================================
void AudioMasterClock::speakerDeviceEnabled( bool isEnabled )
{
	m_SpeakerDeviceEnabled = isEnabled;
	updateDeviceEnable();
	if( isEnabled )
	{
		m_ResetOnFirstSpeakerWrite = true;
	}
}

//============================================================================
void AudioMasterClock::updateDeviceEnable( void )
{
	if( !m_MicDeviceEnabled && !m_SpeakerDeviceEnabled )
	{
		enableMasterClock( false );
	}
	else if( m_MicDeviceEnabled && m_SpeakerDeviceEnabled )
	{
		resetMasterClock();
	}
	else if( m_MicDeviceEnabled && !m_SpeakerDeviceEnabled )
	{
		resetMasterClock();
		enableMasterClock( true );
	}
	else if( !m_MicDeviceEnabled && m_SpeakerDeviceEnabled )
	{
		resetMasterClock();
		enableMasterClock( true );
	}
}
