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

#include "AudioLoopback.h"

#include "Appcommon.h"
#include "AudioIoMgr.h"
#include "AudioUtils.h"

#include <ptop_src/ptop_engine_src/MediaProcessor/RawAudio.h>

#include <CoreLib/VxTime.h>

const int FRAME_INTERVAL_US = 80000;

namespace
{
	//============================================================================
	static void* AudioOutProcessThreadFunc( void* pvContext )
	{
		VxThread* poThread = (VxThread*)pvContext;
		poThread->setIsThreadRunning( true );
		AudioLoopback* processor = (AudioLoopback*)poThread->getThreadUserParam();
		if( processor )
		{
			processor->processAudioOutSpaceAvailableThreaded();
		}

		poThread->threadAboutToExit();
		return nullptr;
	}
}

//============================================================================
AudioLoopback::AudioLoopback( AudioIoMgr& audioIoMgr, QObject* parent )
	: QObject( parent )
	, m_AudioIoMgr( audioIoMgr )
	, m_MyApp( audioIoMgr.getMyApp() )
{
	memset( m_QuietAudioBuf, 0, MIXER_CHUNK_LEN_SAMPLES * 2 );
	memset( m_MixerBuf, 0, MIXER_CHUNK_LEN_SAMPLES * 2 );
	memset( m_QuietEchoBuf, 0, MIXER_CHUNK_LEN_SAMPLES * 4 * 2 );

	for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
	{
		m_MixerFrames[ i ].setFrameIndex( i );
		m_MixerFrames[ i ].setAudioIoMgr( &audioIoMgr );
	}

	m_ProcessAudioOutThread.startThread( (VX_THREAD_FUNCTION_T)AudioOutProcessThreadFunc, this, "AudioOutProcessor" );
}

//============================================================================
void AudioLoopback::audioLoopbackShutdown( void )
{
	m_ProcessAudioOutThread.abortThreadRun( true );
	m_AudioOutSemaphore.signal();
}

//============================================================================
qint64 AudioLoopback::readRequestFromSpeaker( char* data, qint64 maxlen, AudioSampleBuf& echoFarBuf, int& peakValue0to100 )
{
	peakValue0to100 = 0;
	if( maxlen <= 0 )
	{
		LogMsg( LOG_DEBUG, "readDataFromMixer %lld bytes ", maxlen );
		return 0;
	}

	// convert request buffer to pcm 2 byte samples
	int16_t* readReqPcmBuf = (int16_t*)data;
	int reqSpeakerSampleCnt = maxlen / 2;

	int beforeEchoSampleCnt = echoFarBuf.getSampleCnt();
	static int readRemainder = 0;
	int reqEchoSampleCnt = reqSpeakerSampleCnt / MIXER_TO_SPEAKER_MULTIPLIER;
	readRemainder += reqSpeakerSampleCnt % MIXER_TO_SPEAKER_MULTIPLIER;
	if( readRemainder >= MIXER_TO_SPEAKER_MULTIPLIER )
	{
		readRemainder -= MIXER_TO_SPEAKER_MULTIPLIER;
		reqEchoSampleCnt++;
	}

	if( !m_AudioIoMgr.isAudioInitialized() || m_MixerWriteIdx == m_MixerReadIdx )
	{
		LogMsg( LOG_DEBUG, "AudioLoopback::readRequestFromSpeaker overrun" );
		memset( data, 0, maxlen );
		echoFarBuf.writeSamples( m_QuietEchoBuf, reqEchoSampleCnt );
		return maxlen;
	}

	// for test if speaker output is working without mixer involved
	//m_AudioIoMgr.getAudioCallbacks().readGenerated4800HzMono100HzToneData( data, maxlen );
	//m_AudioCallbacks.speakerAudioPlayed( getMixerFormat(), (void*)data, maxlen ); // causes audio popping artifacts so comment out if want pure tone
	//mixerWasReadByOutput( (int)maxlen, upSampleMult );
	//return maxlen;

	int samplesAvailable = 0;

	lockMixer();
	int readIndex = m_MixerReadIdx;
	for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
	{
		if( readIndex == m_MixerWriteIdx )
		{
			// would overrun frame currently being processed
			break;
		}

		AudioLoopbackFrame& audioFrame = m_MixerFrames[ readIndex ];
		if( !audioFrame.isProcessed() )
		{
			break;
		}

		int samplesThisFrame = audioFrame.speakerSamplesAvailable();
		if( !samplesThisFrame )
		{
			break;
		}

		samplesAvailable += samplesThisFrame;

		if( samplesAvailable >= reqSpeakerSampleCnt )
		{
			break;
		}

		readIndex++;
		if( readIndex >= MAX_MIXER_FRAMES )
		{
			readIndex = 0;
		}
	}

	static int underrunCnt = 0;
	
	if( samplesAvailable >= reqSpeakerSampleCnt )
	{
		underrunCnt = 0;
		readRemainder = 0;
		int speakerSamplesRead = 0;
		int echoSamplesRead = 0;
		for( int i = 0; i < MAX_MIXER_FRAMES, speakerSamplesRead < reqSpeakerSampleCnt; i++ )
		{
			AudioLoopbackFrame& audioFrame = m_MixerFrames[ m_MixerReadIdx ];

			int speakerSamplesThisFrame = 0;
			int echoSamplesThisFrame = 0;
			audioFrame.readSpeakerData( &readReqPcmBuf[ speakerSamplesRead ], reqSpeakerSampleCnt - speakerSamplesRead, speakerSamplesThisFrame,
				echoFarBuf, reqEchoSampleCnt - echoSamplesRead, echoSamplesThisFrame, peakValue0to100 );
			if( speakerSamplesThisFrame && 0 == audioFrame.speakerSamplesAvailable() )
			{
				audioFrame.clearFrame(false);
				incrementMixerReadIndex();
			}

			if( !speakerSamplesThisFrame )
			{
				LogMsg( LOG_DEBUG, "AudioLoopback::readRequestFromSpeaker few samples ERROR" );
				break;
			}

			speakerSamplesRead += speakerSamplesThisFrame;
			echoSamplesRead += echoSamplesThisFrame;
		}

		if( echoSamplesRead == reqEchoSampleCnt - 1 )
		{
			// this can sometimes happen when going from silence to frame reads and have a remainder of up multiplied samples
			// should be a better way but for now just fulfill the request count
			int16_t lastSample = echoFarBuf.getLastSample();
			echoFarBuf.writeSamples( &lastSample, 1 );
		}
	}
	else
	{
		underrunCnt++;

		LogMsg( LOG_DEBUG, "AudioLoopback::readRequestFromSpeaker underrun %d", underrunCnt );
		memset( data, 0, maxlen );
		echoFarBuf.writeSamples( m_QuietEchoBuf, reqEchoSampleCnt );
	}

	int afterEchoSampleCnt = echoFarBuf.getSampleCnt();
	if( afterEchoSampleCnt - beforeEchoSampleCnt != reqEchoSampleCnt )
	{
		LogMsg( LOG_DEBUG, "AudioLoopback::readRequestFromSpeaker echo samples underrun" );
	}

	//LogMsg( LOG_VERBOSE, "readRequestFromSpeaker unlockMixer thread %d", VxGetCurrentThreadTid() );
	unlockMixer();
	return maxlen;
}

//============================================================================
void AudioLoopback::fromGuiEchoCanceledSamplesThreaded( int16_t* pcmData, int sampleCnt, int64_t samplesHeadTimeMs, bool isSilence )
{
	vx_assert( MIXER_CHUNK_LEN_SAMPLES == sampleCnt );


	//if( m_AudioIoMgr.getAudioTimingEnable() )
	//{
	//	static int64_t lastQueueMixerTime{ 0 };
	//	static int funcCallCnt{ 0 };
	//	funcCallCnt++;
	//	if( lastQueueMixerTime )
	//	{
	//		int64_t timeInterval = micWriteTime - lastQueueMixerTime;
	//		LogMsg( LOG_VERBOSE, "fromGuiMicrophoneSamples queing mixer %d elapsed %d ms app %d ms", funcCallCnt, (int)timeInterval, (int)GetApplicationAliveMs() );
	//	}

	//	lastQueueMixerTime = micWriteTime;
	//	m_MicQueueSystemTime = m_MyApp.elapsedMilliseconds();
	//	m_MicWriteTime = micWriteTime;
	//}

		//LogMsg( LOG_VERBOSE, "processRawAudioInThreaded lockMixer thread %d", VxGetCurrentThreadTid() );
	lockMixer();
	//LogMsg( LOG_VERBOSE, "processRawAudioInThreaded lockMixer thread %d run write idx %", VxGetCurrentThreadTid(), m_MixerWriteIdx );
	AudioLoopbackFrame& audioFrame = getAudioWriteFrame();
	audioFrame.toMixerPcm8000HzMonoChannel( eAppModuleMicrophone, pcmData, isSilence );
	//LogMsg( LOG_VERBOSE, "processRawAudioInThreaded unlockMixer thread %d", VxGetCurrentThreadTid() );
	unlockMixer();
}

//============================================================================
void AudioLoopback::fromGuiAudioOutSpaceAvail( int spaceInBytes )
{
	m_AudioOutSemaphore.signal();
}

//============================================================================
void AudioLoopback::processAudioOutSpaceAvailableThreaded( void )
{
	while( false == m_ProcessAudioOutThread.isAborted() )
	{
		m_AudioOutSemaphore.wait();
		if( m_ProcessAudioOutThread.isAborted() )
		{
			LogMsg( LOG_VERBOSE, "AudioLoopback::processAudioOut aborting1" );
			break;
		}

		if( m_AudioIoMgr.getAudioTimingEnable() )
		{
			int64_t timeNow = m_MyApp.elapsedMilliseconds();
			static int64_t lastSpaceAvailableTime{ 0 };
			static int funcCallCnt{ 0 };
			funcCallCnt++;
			if( lastSpaceAvailableTime )
			{
				int64_t timeInterval = timeNow - lastSpaceAvailableTime;
				//LogMsg( LOG_VERBOSE, "processAudioOutSpaceAvailableThreaded %d elapsed %d ms app %d ms", funcCallCnt, (int)timeInterval, (int)m_MyApp.elapsedMilliseconds() );
			}

			lastSpaceAvailableTime = timeNow;
		}

		static int16_t prevFrameSample = 0;

		// make current frame ready for read by speakers
		lockMixer();
		AudioLoopbackFrame& audioFrame = getAudioWriteFrame();
		audioFrame.processFrameForSpeakerOutputThreaded( prevFrameSample );
		prevFrameSample = audioFrame.getLastEchoSample();

		// move to next frame and clear it so is ready to write to
		incrementMixerWriteIndex();
		AudioLoopbackFrame& nextAudioFrame = getAudioWriteFrame();
		nextAudioFrame.clearFrame(false);
		unlockMixer();

		// do output space available processing
		processOutSpaceAvailable();

		if( m_ProcessAudioOutThread.isAborted() )
		{
			LogMsg( LOG_INFO, "AudioLoopback::processAudioOut aborting3" );
			break;
		}
	}

	LogMsg( LOG_INFO, "AudioLoopback::processAudioOut leaving function" );
}

//============================================================================
/// space used in audio que buffer
int AudioLoopback::audioQueUsedSpace( EAppModule appModule, bool mixerIsLocked )
{
	int audioUsedSpace = 0;
	if( (appModule > 1) && (appModule < eMaxAppModule) )
	{
		if( !mixerIsLocked )
		{
			//LogMsg( LOG_VERBOSE, "audioQueUsedSpace lockMixer thread %d", VxGetCurrentThreadTid() );
			lockMixer();
			//LogMsg( LOG_VERBOSE, "audioQueUsedSpace lockMixer thread %d run", VxGetCurrentThreadTid() );
		}

		int frameIndex = getModuleFrameIndex( appModule );
		for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
		{
			if( m_MixerFrames[ frameIndex ].hasModuleAudio( appModule ) )
			{
				audioUsedSpace += m_MixerFrames[ frameIndex ].audioLenInUse();
			}
			else
			{
				// ran out of audio filled by this module
				break;
			}

			frameIndex++;
			if( frameIndex >= MAX_MIXER_FRAMES )
			{
				frameIndex = 0;
			}
		}

		if( !mixerIsLocked )
		{
			//LogMsg( LOG_VERBOSE, "audioQueUsedSpace unlockMixer thread %d", VxGetCurrentThreadTid() );
			unlockMixer();
		}
	}
	else if( eAppModuleAll == appModule )
	{
		if( !mixerIsLocked )
		{
			//LogMsg( LOG_VERBOSE, "audioQueUsedSpace lockMixer thread %d", VxGetCurrentThreadTid() );
			lockMixer();
			//LogMsg( LOG_VERBOSE, "audioQueUsedSpace lockMixer thread %d run read idx %d", VxGetCurrentThreadTid(), m_MixerReadIdx );
		}

		int frameIndex = m_MixerReadIdx;
		for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
		{
			if( m_MixerFrames[ frameIndex ].hasAnyAudio() )
			{
				audioUsedSpace += m_MixerFrames[ frameIndex ].audioLenInUse();
			}
			else
			{
				// ran out of audio filled by any module
				break;
			}

			frameIndex++;
			if( frameIndex >= MAX_MIXER_FRAMES )
			{
				frameIndex = 0;
			}
		}

		if( !mixerIsLocked )
		{
			//LogMsg( LOG_VERBOSE, "audioQueUsedSpace unlockMixer thread %d", VxGetCurrentThreadTid() );
			unlockMixer();
		}
	}

	return audioUsedSpace;
}

//============================================================================
// get length of data buffered and ready for speaker out
int AudioLoopback::getDataReadyForSpeakersLen( bool mixerIsLocked )
{
	return audioQueUsedSpace( eAppModuleAll, mixerIsLocked );
}

//============================================================================
int AudioLoopback::incrementMixerWriteIndex( void ) 
{ 
	m_MixerWriteIdx++; 
	if( m_MixerWriteIdx >= MAX_MIXER_FRAMES )
	{
		m_MixerWriteIdx = 0;
	}
		
	if( m_AudioIoMgr.getFrameIndexDebugEnable() )
	{
		int64_t timeNow = GetHighResolutionTimeMs();
		static int64_t lastMixerPcmTime{ 0 };
		static int funcCallCnt{ 0 };
		funcCallCnt++;
		if( lastMixerPcmTime )
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_VERBOSE, "W Frame %d call cnt %d incrementMixerWriteIndex elapsed %d ms", m_MixerWriteIdx, funcCallCnt, timeInterval );
		}

		lastMixerPcmTime = timeNow;
	}

	return m_MixerWriteIdx; 
}

//============================================================================
int AudioLoopback::incrementMixerReadIndex( void )
{
	m_MixerReadIdx++;
	if( m_MixerReadIdx >= MAX_MIXER_FRAMES )
	{
		m_MixerReadIdx = 0;
	}

	if( m_AudioIoMgr.getFrameIndexDebugEnable() )
	{
		int64_t timeNow = GetHighResolutionTimeMs();
		static int64_t lastMixerPcmTime{ 0 };
		static int funcCallCnt{ 0 };
		funcCallCnt++;
		if( lastMixerPcmTime )
		{
			int timeInterval = (int)(timeNow - lastMixerPcmTime);
			LogMsg( LOG_VERBOSE, "R Frame %d call cnt %d incrementMixerReadIndex elapsed %d ms", m_MixerReadIdx, funcCallCnt, timeInterval );
		}

		lastMixerPcmTime = timeNow;
	}

	return m_MixerReadIdx;
}

//============================================================================
void AudioLoopback::echoCancelSyncState( bool inSync )
{
	if( inSync )
	{
		// the read timing fluctuates widely
		// fill with silence and set indexes so write should not catch up with read
		lockMixer();
		for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
		{
			m_MixerFrames[ i ].clearFrame( true );
		}

		m_MixerWriteIdx = 3;
		m_MixerReadIdx = 0;

		unlockMixer();
	}
}