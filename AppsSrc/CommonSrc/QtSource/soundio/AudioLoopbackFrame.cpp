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

#include "AudioLoopbackFrame.h"

#include "AppCommon.h"
#include "AudioIoMgr.h"
#include "AudioUtils.h"
#include "AudioSampleBuf.h"
#include "SoundMgr.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AudioLoopbackFrame::AudioLoopbackFrame()
{
	memset( m_MixerBuf, 0, sizeof( m_MixerBuf ) );
	memset( m_SpeakerBuf, 0, sizeof( m_SpeakerBuf ) );
}

//============================================================================
void AudioLoopbackFrame::clearFrame( bool fillSilence )
{
	m_MixerSamplesWrote = 0;
	m_MixerSamplesRead = 0;
	m_SpeakerSamplesRead = 0;
	m_SpeakerSamplesWrote = 0;

	m_InputIds.clear();

	m_IsSilentSamples = true;
	m_IsProcessed = false;

	if( fillSilence )
	{
		memset( m_MixerBuf, 0, sizeof( m_MixerBuf ) );
		m_MixerSamplesWrote = MIXER_CHUNK_LEN_SAMPLES;
		memset( m_SpeakerBuf, 0, sizeof( m_SpeakerBuf ) );
		m_SpeakerSamplesWrote = SPEAKER_CHUNK_LEN_SAMPLES;
		m_IsProcessed = true;
	}
}

//============================================================================
int AudioLoopbackFrame::audioLenInUse( void )
{
	return m_MixerSamplesWrote - m_MixerSamplesRead; // how many bytes is written and not read by audio out
}

//============================================================================
int AudioLoopbackFrame::audioLenFreeSpace( void )
{
	return MIXER_CHUNK_LEN_BYTES - (m_MixerSamplesWrote - m_MixerSamplesRead); // how many bytes is available to write into frame
}

//============================================================================
int AudioLoopbackFrame::toMixerPcm8000HzMonoChannel( EAppModule appModule, int16_t* pcmData, bool isSilenceIn )
{
	static int64_t timeNow = 0;
	static int64_t lastMixerPcmTime{ 0 };

	lastMixerPcmTime = timeNow;
	timeNow = GetHighResolutionTimeMs();

	if( hasModuleAudio( appModule ) )
	{
		if( m_AudioIoMgr->getFrameTimingEnable() )
		{		
			static int funcCallCnt{ 0 };
			funcCallCnt++;
			if( lastMixerPcmTime )
			{
				int timeInterval = (int)(timeNow - lastMixerPcmTime);
				LogMsg( LOG_VERBOSE, "W Frame %d call cnt %d toMixerPcm8000HzMonoChannel module %s elapsed %d ms overrrun ", getFrameIndex(), funcCallCnt,
					DescribeAppModule( appModule ), timeInterval );
			}
		}
		else
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_WARNING, "W Frame %d AudioLoopbackFrame::toMixerPcm8000HzMonoChannel module %s elapsed %d ms overrun", getFrameIndex(), DescribeAppModule( appModule ), timeInterval );
		}
		
		return 0;
	}

	if( m_IsSilentSamples && isSilenceIn )
	{
		memset( m_MixerBuf, 0, MIXER_CHUNK_LEN_BYTES );
	}
	else if( m_IsSilentSamples && !isSilenceIn )
	{
		memcpy( m_MixerBuf, pcmData, MIXER_CHUNK_LEN_BYTES );
		m_IsSilentSamples = false;
	}
	else if( !m_IsSilentSamples && !isSilenceIn )
	{
		AudioUtils::mixPcmAudio( (int16_t*)pcmData, (int16_t*)m_MixerBuf, MIXER_CHUNK_LEN_BYTES );
		m_IsSilentSamples = false;
	}

	if( m_InputIds.empty() )
	{
		// first write after has been read.. reset partial read counters
		m_MixerSamplesWrote = MIXER_CHUNK_LEN_SAMPLES;
	}
	
	m_InputIds.push_back( appModule );

	if( m_AudioIoMgr->getFrameTimingEnable() )
	{
		int64_t timeNow = GetHighResolutionTimeMs();
		static int64_t lastMixerPcmTime{ 0 };
		static int funcCallCnt{ 0 };
		funcCallCnt++;
		if( lastMixerPcmTime )
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_VERBOSE, "W Frame %d call cnt %d toMixerPcm8000HzMonoChannel module %s elapsed %d ms", getFrameIndex(), funcCallCnt,
				DescribeAppModule( appModule ), timeInterval );
		}

		lastMixerPcmTime = timeNow;
	}

	return MIXER_CHUNK_LEN_BYTES;
}

//============================================================================
void AudioLoopbackFrame::processFrameForSpeakerOutputThreaded( int16_t prevFrameSample )
{
	if( (m_AudioIoMgr->getFrameTimingEnable() || m_AudioIoMgr->getFrameIndexDebugEnable()) && m_AudioIoMgr->getIsEchoCancelInSync() )
	{
		int64_t timeNow = GetHighResolutionTimeMs();
		static int64_t lastMixerPcmTime{ 0 };
		static int funcCallCnt{ 0 };
		funcCallCnt++;
		if( lastMixerPcmTime )
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_VERBOSE, "P Frame %d processFrameForSpeakerOutputThreaded processing  elapsed %d ms", getFrameIndex(), timeInterval );
		}	
	}

	m_SpeakerSamplesRead = 0;
	m_SpeakerSamplesWrote = SPEAKER_CHUNK_LEN_SAMPLES;

	m_MixerSamplesRead = 0;
	m_MixerSamplesWrote = MIXER_CHUNK_LEN_SAMPLES;

	m_IsProcessed = true;

	if( m_InputIds.empty() )
	{
		if( m_AudioIoMgr->getIsEchoCancelInSync() )
		{
			LogMsg( LOG_WARNING, "P Frame %d processFrameForSpeakerOutputThreaded processing empty frame", getFrameIndex() );
		}

		memset( m_MixerBuf, 0, MIXER_CHUNK_LEN_BYTES );
		memset( m_SpeakerBuf, 0, SPEAKER_CHUNK_LEN_BYTES );
		m_PeakValue0to100 = 0;
		return;
	}

	if( m_IsSilentSamples )
	{
		LogMsg( LOG_WARNING, "P Frame %d processFrameForSpeakerOutputThreaded silent frame", getFrameIndex() );
		memset( m_MixerBuf, 0, MIXER_CHUNK_LEN_BYTES );
		memset( m_SpeakerBuf, 0, SPEAKER_CHUNK_LEN_BYTES );
		m_PeakValue0to100 = 0;
	}
	else
	{
		AudioUtils::upsamplePcmAudioLerpPrev( m_MixerBuf, MIXER_CHUNK_LEN_SAMPLES, MIXER_TO_SPEAKER_MULTIPLIER, prevFrameSample, m_SpeakerBuf );
		m_PeakValue0to100 = AudioUtils::peakPcmAmplitude0to100( m_MixerBuf, MIXER_CHUNK_LEN_SAMPLES );
	}	

	if( m_AudioIoMgr->getFrameTimingEnable() )
	{
		int64_t timeNow = GetHighResolutionTimeMs();
		static int64_t lastMixerPcmTime{ 0 };
		static int funcCallCnt{ 0 };
		funcCallCnt++;
		if( lastMixerPcmTime )
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_VERBOSE, "P Frame %d call cnt %d processFrameForSpeakerOutputThreaded elapsed %d ms app %d ms id cnt %d peak value %d", getFrameIndex(), funcCallCnt,
				timeInterval, (int)timeNow, m_InputIds.size(), m_PeakValue0to100 );
		}

		lastMixerPcmTime = timeNow;
	}
}

//============================================================================
int16_t AudioLoopbackFrame::getLastEchoSample( void )
{
	return m_MixerBuf[ MIXER_CHUNK_LEN_SAMPLES - 1 ];
}

//============================================================================
int AudioLoopbackFrame::readSpeakerData( int16_t* pcmReadData, int speakerSamplesRequested, int& retSpeakerSamplesRead, 
									AudioSampleBuf& echoFarBuf, int echoSamplesRequested, int& retEchoSamplesRead, int& peakValue0to100 )
{
	if( m_PeakValue0to100 > peakValue0to100 )
	{
		peakValue0to100 = m_PeakValue0to100;
	}

	int speakerSamplesToRead = std::min( speakerSamplesAvailable(), speakerSamplesRequested );
	memcpy( pcmReadData, &m_SpeakerBuf[ m_SpeakerSamplesRead ], speakerSamplesToRead * 2 );
	retSpeakerSamplesRead += speakerSamplesToRead;
	speakerSamplesWereRead( speakerSamplesToRead );

	int echoSamplesToRead = std::min( echoSamplesAvailable(), echoSamplesRequested );
	echoFarBuf.writeSamples( &m_MixerBuf[ m_MixerSamplesRead ], echoSamplesToRead );
	retEchoSamplesRead += echoSamplesToRead;
	echoSamplesWereRead( echoSamplesToRead );

	if( m_AudioIoMgr->getFrameTimingEnable() && !speakerSamplesAvailable() )
	{
		int64_t timeNow = GetHighResolutionTimeMs();
		static int64_t lastMixerPcmTime{ 0 };
		static int funcCallCnt{ 0 };
		funcCallCnt++;
		if( lastMixerPcmTime )
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_VERBOSE, "R Frame %d call cnt %d readSpeakerData elapsed %d ms samples echo %d speaker %d left to read echo %d speaker %d", getFrameIndex(), funcCallCnt,
				 timeInterval, echoSamplesToRead, speakerSamplesToRead, echoSamplesAvailable(), speakerSamplesAvailable() );
		}

		lastMixerPcmTime = timeNow;
	}
	else if( m_AudioIoMgr->getFrameTimingEnable() )
	{
		LogMsg( LOG_VERBOSE, "R Frame %d partial readSpeakerData samples echo %d speaker %d left to read echo %d speaker %d", getFrameIndex(), 
			echoSamplesToRead, speakerSamplesToRead, echoSamplesAvailable(), speakerSamplesAvailable() );
	}

	return speakerSamplesToRead;
}