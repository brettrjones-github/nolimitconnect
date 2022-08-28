
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

#include "AudioEchoCancelImpl.h"
#include "AudioUtils.h"
#include "AudioIoMgr.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#ifdef USE_ECHO_CANCEL_3
#include <api/audio/echo_canceller3_config.h>
#include <api/audio/echo_canceller3_factory.h>
#endif // USE_ECHO_CANCEL_3

#include <CoreLib/VxTime.h>
#include <CoreLib/VxDebug.h>

#include <QAudioFormat>


namespace
{
	//============================================================================
	static void* AudioEchoProcessThreadFunc( void* pvContext )
	{
		VxThread* poThread = (VxThread*)pvContext;
		poThread->setIsThreadRunning( true );
		AudioEchoCancelImpl* processor = (AudioEchoCancelImpl*)poThread->getThreadUserParam();
		if( processor )
		{
			processor->processEchoRunThreaded();
		}

		poThread->threadAboutToExit();
		return nullptr;
	}
}


//============================================================================
AudioEchoCancelImpl::AudioEchoCancelImpl( AppCommon& appCommon, AudioIoMgr& audioIoMgr, QObject* parent )
	: QObject( parent )
	, m_MyApp( appCommon )
	, m_AudioIoMgr( audioIoMgr )
{
	memset( m_QuietSamplesBuf, 0, sizeof( m_QuietSamplesBuf ) );
	m_ProcessEchoThread.startThread( (VX_THREAD_FUNCTION_T)AudioEchoProcessThreadFunc, this, "AudioEchoProcessor" );

	// setSyncDebugEnabled( true ); // log echo sync even when in sync
}

//============================================================================
AudioEchoCancelImpl::~AudioEchoCancelImpl()
{
#if defined(USE_WEB_RTC_ECHO_CANCEL_1) || defined(USE_WEB_RTC_ECHO_CANCEL_3)
	delete m_MicEchoBuf10ms;
	delete m_MicRemainderBuf;
	delete m_SpeakerBuf10ms;
#endif //#if defined(USE_WEB_RTC_ECHO_CANCEL_1) || defined(USE_WEB_RTC_ECHO_CANCEL_3)
}

//============================================================================
void AudioEchoCancelImpl::echoCancelStartup( void )
{
#if defined( USE_SPEEX_ECHO_CANCEL )
	// do nothing yet.. need the microphone frame size in order to setup
	m_EchoCancelNeedsReset = true;
#elif defined( USE_WEB_RTC_ECHO_CANCEL_1 )
	m_SamplesPer10ms = ECHO_SAMPLE_RATE / 100;

	// Qt does not always have nice 10ms even divisible mic write or speaker reads so need a remainder buffer
	m_MicEchoBuf10ms = new int16_t[  m_SamplesPer10ms ];
	m_MicRemainderBuf = new int16_t[ m_SamplesPer10ms ];
	m_MicRemainderSampleCnt = 0;
	m_SpeakerBuf10ms = new int16_t[ m_SamplesPer10ms ];
#elif defined( USE_WEB_RTC_ECHO_CANCEL_3 )
	int ref_sample_rate = ECHO_SAMPLE_RATE_HZ;
	int ref_channels = ECHO_SAMPLE_CHANNELS;
	int rec_sample_rate = ECHO_SAMPLE_RATE_HZ;
	int rec_channels = ECHO_SAMPLE_CHANNELS;
	int rec_bits_per_sample = 16;

	webrtc::EchoCanceller3Config aec_config;
	webrtc::EchoCanceller3Factory aec_factory = webrtc::EchoCanceller3Factory( aec_config );

	m_EchoControler = aec_factory.Create( ref_sample_rate, ref_channels, rec_channels );
	m_HighPassFilter = std::make_unique< webrtc::HighPassFilter>( rec_sample_rate, rec_channels );

	int sample_rate = rec_sample_rate;
	int channels = rec_channels;
	int bits_per_sample = rec_bits_per_sample;
	m_MicStreamConfig = webrtc::StreamConfig( sample_rate, channels );
	m_SpeakerStreamConfig = webrtc::StreamConfig( sample_rate, channels );

	m_MicAudioBuf = std::make_unique<webrtc::AudioBuffer>(
		m_MicStreamConfig.sample_rate_hz(), m_MicStreamConfig.num_channels(),
		m_MicStreamConfig.sample_rate_hz(), m_MicStreamConfig.num_channels(),
		m_MicStreamConfig.sample_rate_hz(), m_MicStreamConfig.num_channels() );


	m_SpeakerAudioBuf = std::make_unique<webrtc::AudioBuffer>(
		m_SpeakerStreamConfig.sample_rate_hz(), m_SpeakerStreamConfig.num_channels(),
		m_SpeakerStreamConfig.sample_rate_hz(), m_SpeakerStreamConfig.num_channels(),
		m_SpeakerStreamConfig.sample_rate_hz(), m_SpeakerStreamConfig.num_channels() );
#endif // USE_WEB_RTC_ECHO_CANCEL_3
}

//============================================================================
void AudioEchoCancelImpl::echoCancelShutdown( void )
{
	m_ProcessEchoThread.abortThreadRun( true );
	m_ProcessEchoSemaphore.signal();
}

//============================================================================
void AudioEchoCancelImpl::enableEchoCancel( bool enable )
{
	m_EchoCancelEnable = enable;
	resetEchoCancel();
}

//============================================================================
void AudioEchoCancelImpl::resetEchoCancel( void )
{
#if defined(USE_SPEEX_ECHO_CANCEL)
	if( m_SpeexState )
	{
		speex_echo_state_reset( m_SpeexState );
	}
#endif // defined(USE_SPEEX_ECHO_CANCEL)
}

//============================================================================
void AudioEchoCancelImpl::speakerReadSamples( int16_t* speakerReadData, int sampleCnt, int64_t speakerReadTailTimeMs, bool stableTimestamp )
{
	m_StableSpeakerTimestamp = stableTimestamp;
	m_SpeakerSamplesMutex.lock();
	m_SpeakerSamples.writeSamples( speakerReadData, sampleCnt );
	m_TailSpeakerSamplesMs = speakerReadTailTimeMs;
	m_HeadSpeakerSamplesMs = speakerReadTailTimeMs - AudioUtils::audioDurationMs( 8000, m_SpeakerSamples.getSampleCnt() );

	m_SpeakerSamplesMutex.unlock();

	m_ProcessEchoSemaphore.signal();
}

//============================================================================
void AudioEchoCancelImpl::micWriteSamples( int16_t* micWriteData, int sampleCnt, int64_t micWriteTailTimeMs, bool stableTimestamp )
{
	m_StableMicTimestamp = stableTimestamp;
	m_MicSamplesMutex.lock();
	m_MicSamples.writeSamples( micWriteData, sampleCnt );
	m_TailMicSamplesMs = micWriteTailTimeMs;
	m_HeadMicSamplesMs = micWriteTailTimeMs - AudioUtils::audioDurationMs( 8000, m_MicSamples.getSampleCnt() );

	m_MicSamplesMutex.unlock();

	m_ProcessEchoSemaphore.signal();
}

//============================================================================
void AudioEchoCancelImpl::processEchoRunThreaded()
{
	while( false == m_ProcessEchoThread.isAborted() )
	{
		m_ProcessEchoSemaphore.wait();
		if( m_ProcessEchoThread.isAborted() )
		{
			LogMsg( LOG_VERBOSE, "AudioEchoCancelImpl::processEchoRunThreaded aborting" );
			break;
		}

		processEchoCancelThreaded();
	}
}

//============================================================================
void AudioEchoCancelImpl::processEchoCancelThreaded()
{
	if( m_SpeakerSamples.getSampleCnt() >= MONO_8000HZ_SAMPLES_PER_10MS && m_MicSamples.getSampleCnt() >= MONO_8000HZ_SAMPLES_PER_10MS )
	{
		m_EchoCancelInSync = attemptEchoSync();
		if( m_EchoCancelInSync )
		{

			if( m_EchoCancelNeedsReset )
			{
				m_EchoCanceledSamplesMutex.lock();
				m_EchoCanceledSamples.clear();
				m_EchoCanceledSamplesMutex.unlock();

				// for now always use 10ms frames of samples. webRtc requires it and speex will work with it
				resetSpeex( MONO_8000HZ_SAMPLES_PER_10MS );
			}

			static int64_t startMs = 0;
			if( m_AudioIoMgr.getAudioTimingEnable() )
			{
				startMs = GetHighResolutionTimeMs();
			}

			m_SpeakerSamplesMutex.lock();
			m_MicSamplesMutex.lock();
			m_EchoCanceledSamplesMutex.lock();

			int framesToProcess = std::min( m_MicSamples.getSampleCnt() / MONO_8000HZ_SAMPLES_PER_10MS, m_SpeakerSamples.getSampleCnt() / MONO_8000HZ_SAMPLES_PER_10MS );
			framesToProcess = std::min( framesToProcess, m_EchoCanceledSamples.freeSpaceSampleCount() / MONO_8000HZ_SAMPLES_PER_10MS );
			if( framesToProcess )
			{
				// on a fast windows machine the echo processing takes about 4ms. this seems acceptable but keeping the buffers locked delays mic write to long
				// unfortunately the only solution seems to be to make copies to work on and release the buffers immediately

				int samplesToProcess = framesToProcess * MONO_8000HZ_SAMPLES_PER_10MS;
				m_ProcessSpeakerSamples.writeSamples( m_SpeakerSamples.getSampleBuffer(), samplesToProcess );
				m_ProcessMicSamples.writeSamples( m_MicSamples.getSampleBuffer(), samplesToProcess );
				m_SpeakerSamples.samplesWereRead( samplesToProcess );
				m_MicSamples.samplesWereRead( samplesToProcess );

				int64_t preProceesMicTimeMs = m_HeadMicSamplesMs;
				int samplesProcessedMs = AudioUtils::audioDurationMs( 8000, samplesToProcess );
				m_HeadMicSamplesMs += samplesProcessedMs;
				m_HeadSpeakerSamplesMs += samplesProcessedMs;

				m_SpeakerSamplesMutex.unlock();
				m_MicSamplesMutex.unlock();

				int16_t* micBuf = m_ProcessMicSamples.getSampleBuffer();
				int16_t* speakerBuf = m_ProcessSpeakerSamples.getSampleBuffer();
				int16_t* echoCanceledBuf = m_EchoCanceledSamples.getSampleBuffer();
				echoCanceledBuf = &echoCanceledBuf[ m_EchoCanceledSamples.getSampleCnt() ];

				for( int i = 0; i < framesToProcess; i++ )
				{
					processEchoCancelFrame( micBuf, speakerBuf, echoCanceledBuf );

					micBuf += MONO_8000HZ_SAMPLES_PER_10MS;
					speakerBuf += MONO_8000HZ_SAMPLES_PER_10MS;
					echoCanceledBuf += MONO_8000HZ_SAMPLES_PER_10MS;
				}

				m_ProcessMicSamples.clear();
				m_ProcessSpeakerSamples.clear();

				int existingEchoMs = m_EchoCanceledSamples.getAudioDurationMs();
				m_EchoCanceledSamples.setSampleCnt( m_EchoCanceledSamples.getSampleCnt() + samplesToProcess );

				m_HeadEchoCanceledSamplesMs = preProceesMicTimeMs - existingEchoMs;

				// LogMsg( LOG_VERBOSE, "processSpeexEchoCancel echo canceled seqNum %d cnt %d left %d of %d", echoCancelSequenceCnt, sampleCnt, echoSamples.getSampleCnt(), echoSampleCnt );		
			}
			else
			{
				m_SpeakerSamplesMutex.unlock();
				m_MicSamplesMutex.unlock();
			}

			m_EchoCanceledSamplesMutex.unlock();
			if( m_AudioIoMgr.getAudioTimingEnable() )
			{
				int64_t endtMs = GetHighResolutionTimeMs();
				if( endtMs - startMs > 10 )
				{
					LogMsg( LOG_VERBOSE, "AudioEchoCancelImpl::processEchoCancelThreaded took %d ms", (int)(endtMs - startMs) );
				}
			}
		}
		else
		{
			// not in sync
		}

		checkFor80msFrameElapsedThreaded();
	}
	else
	{
		// not enough samples to do echo cancel
	}


	//if( m_AudioIoMgr.getAudioTimingEnable() )
	//{
	//	static int64_t lastMicWriteTime{ 0 };
	//	static int funcCallCnt{ 0 };
	//	funcCallCnt++;
	//	if( lastMicWriteTime )
	//	{
	//		int64_t timeInterval = micWriteTime - lastMicWriteTime;
	//		LogMsg( LOG_VERBOSE, "AudioEchoCancelImpl::microphoneWroteSamples %d elapsed %d ms app %d ms", funcCallCnt, (int)timeInterval, (int)GetApplicationAliveMs() );
	//	}

	//	lastMicWriteTime = micWriteTime;
	//}
}

//============================================================================
void AudioEchoCancelImpl::checkFor80msFrameElapsedThreaded( void )
{
	if( m_Frame80msElapsed )
	{
		m_Frame80msElapsed = false;
		bool echoCanceledFrameWasWritten{ false };
		bool micMuted = m_AudioIoMgr.getIsMicrophoneMuted();

		int64_t startMs = 0;
		if( m_AudioIoMgr.getAudioTimingEnable() || m_AudioIoMgr.getFrameTimingEnable() )
		{
			startMs = GetHighResolutionTimeMs();
		}

		m_EchoCanceledSamplesMutex.lock();
		if( m_EchoCanceledSamples.getSampleCnt() >= MIXER_CHUNK_LEN_SAMPLES )
		{
			int16_t* sampleOutData = micMuted ? m_QuietSamplesBuf : m_EchoCanceledSamples.getSampleBuffer();

			if( m_AudioIoMgr.getAudioLoopbackEnable() )
			{
				m_AudioIoMgr.getAudioLoopback().fromGuiEchoCanceledSamplesThreaded( sampleOutData, MIXER_CHUNK_LEN_SAMPLES, m_HeadEchoCanceledSamplesMs, micMuted );
			}
			else
			{
				m_MyApp.getEngine().getMediaProcessor().fromGuiMicrophoneSamples( sampleOutData, MIXER_CHUNK_LEN_SAMPLES, m_HeadEchoCanceledSamplesMs );
			}

			m_HeadEchoCanceledSamplesMs += MIXER_CHUNK_LEN_MS;
			m_EchoCanceledSamples.samplesWereRead( MIXER_CHUNK_LEN_SAMPLES );
			echoCanceledFrameWasWritten = true;
		}

		m_EchoCanceledSamplesMutex.unlock();
		if( m_AudioIoMgr.getAudioTimingEnable() )
		{
			int64_t endtMs = GetHighResolutionTimeMs();
			if( endtMs - startMs > 2 )
			{
				LogMsg( LOG_VERBOSE, "AudioEchoCancelImpl::checkFor80msFrameElapsed took %d ms", (int)(endtMs - startMs) );
			}
		}

		if( m_AudioIoMgr.getFrameTimingEnable() && echoCanceledFrameWasWritten )
		{
			static int64_t lastFrameTime{ 0 };
			static int funcCallCnt{ 0 };
			funcCallCnt++;
			if( lastFrameTime )
			{
			    int64_t timeInterval = startMs - lastFrameTime;
			    LogMsg( LOG_VERBOSE, "call cnt %d checkFor80msFrameElapsed elapsed %d ms app %d ms", funcCallCnt, (int)timeInterval, (int)GetApplicationAliveMs() );
			}

			lastFrameTime = startMs;
		}
	}
}

//============================================================================
bool AudioEchoCancelImpl::attemptEchoSync( void )
{
	bool inSync{ m_EchoCancelInSync };
	// timing issues
	// 1.) micWriteTime and startOfSpeakerSamplesTimeMs may be off by 30ms or so
	// 2.) Sound input device and output device may run on different clocks if separate devices like usb web cam microphone and hdmi monitor speakers
	//     from what I have found nobody has solved this issue unless have access to hardware to calculate the clock drift of input and output devices
	// 3.) Echo canceler will attempt to determine what output samples correspond to the mic input sample but they must be close ( probably must be within 10ms )
	//     once the canceler is in sync the input and ouput samples must stay in sync or echo canceler gets confused
	// 4.) Qt will not allow hardly any cpu processing or delay (approx > 2ms) in audio read and write. it indicates too much processing by doing a read of 0 length and popping noise
	//     practical\y all processing of all audio must be done in threads

	// to attempt to fix these issues
	// 1.) When echo cancel is out of sync return same samples without doing echo cancel and mark echo canceler for reset
	// 2.) if drifted out of jitter window skip echo cancel and attempt resync

	m_SpeakerSamplesMutex.lock();
	m_MicSamplesMutex.lock();

	int64_t targetSpeakerTimeMs = m_HeadMicSamplesMs + m_EchoDelayMsConstant;

	if( inSync )
	{
		// if we are in sync see if we have drifted to far out of sync
		// give a lot of room for jitter and drift if we are in sync
		if( m_HeadSpeakerSamplesMs < targetSpeakerTimeMs - 90 || m_HeadSpeakerSamplesMs > targetSpeakerTimeMs + 90 )
		{
			LogMsg( LOG_VERBOSE, "attemptEchoSync fell OUT of sync diff %d ms mic sample ms %d speaker sample ms %d", (int)(m_HeadSpeakerSamplesMs - targetSpeakerTimeMs),
				m_MicSamples.getAudioDurationMs(), m_SpeakerSamples.getAudioDurationMs() );
			inSync = false;
			m_EchoCancelNeedsReset = true;
			m_AudioIoMgr.echoCancelSyncState( false );
		}
		else if( getSyncDebugEnabled() )
		{
			LogMsg( LOG_VERBOSE, "attemptEchoSync is IN sync diff %d ms mic sample ms %d speaker sample ms %d cnt %d", (int)(m_HeadSpeakerSamplesMs - targetSpeakerTimeMs),
				m_MicSamples.getAudioDurationMs(), m_SpeakerSamples.getAudioDurationMs(), m_SpeakerSamples.getSampleCnt() );
		}
	}

	if( !inSync )
	{
		// out of sync.. see if can achive resync

		m_HeadMicSamplesMs = m_TailMicSamplesMs - m_MicSamples.getAudioDurationMs();
		targetSpeakerTimeMs = m_HeadMicSamplesMs + m_EchoDelayMsConstant;
		
		if( targetSpeakerTimeMs >= m_HeadSpeakerSamplesMs && targetSpeakerTimeMs <= m_TailSpeakerSamplesMs - 100 )
		{
			// strip samples up to point of begining of what we believe is the in sync speaker samples
			int samplesToStrip = AudioUtils::audioSamplesRequiredForGivenMs( 8000, (int)(targetSpeakerTimeMs - m_HeadSpeakerSamplesMs) );
			m_SpeakerSamples.samplesWereRead( samplesToStrip );
			m_HeadSpeakerSamplesMs = m_TailSpeakerSamplesMs - m_SpeakerSamples.getAudioDurationMs();
			LogMsg( LOG_VERBOSE, "processSpeexEchoCancel stripping speaker sample cnt %d to get sync samples left %d ms time diff %d ms",
				samplesToStrip, m_SpeakerSamples.getAudioDurationMs(), (int)(targetSpeakerTimeMs - m_HeadSpeakerSamplesMs) );
			inSync = true;
			m_AudioIoMgr.echoCancelSyncState( true );
		}
		else if( getSyncDebugEnabled() )
		{
			LogMsg( LOG_VERBOSE, "processSpeexEchoCancel out of sync mic samples  %d ms speaker samples %d ms target time dif %d",
				m_MicSamples.getAudioDurationMs(), m_SpeakerSamples.getAudioDurationMs(), 
				(int)(targetSpeakerTimeMs - m_HeadSpeakerSamplesMs) );
		}
	}

	m_SpeakerSamplesMutex.unlock();
	m_MicSamplesMutex.unlock();

	return inSync;
}

//============================================================================
void AudioEchoCancelImpl::frame80msElapsed( void )
{
	m_Frame80msElapsed = true;
	m_ProcessEchoSemaphore.signal();
}

//============================================================================
bool AudioEchoCancelImpl::processWebRtcEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData )
{

	if( !m_EchoCancelEnable )
	{
		memcpy( echoCanceledData, micWriteBuf, sampleCnt * 2 );
		return false;
	}

	bool takeBufferOwnership{ false };

#if defined( USE_WEB_RTC_ECHO_CANCEL_1 )

#elif defined( USE_WEB_RTC_ECHO_CANCEL_3 )
	m_MicFrame.UpdateFrame( 0, micWriteBuf, sampleCnt, ECHO_SAMPLE_RATE_HZ, webrtc::AudioFrame::kNormalSpeech, webrtc::AudioFrame::kVadActive, 1 );
	m_MicAudioBuf->CopyFrom( m_MicFrame );

	m_SpeakerAudioBuf->CopyFrom( m_SpeakerFrame );

	m_SpeakerAudioBuf->SplitIntoFrequencyBands();
	m_EchoControler->AnalyzeRender( m_SpeakerAudioBuf.get() );
	m_SpeakerAudioBuf->MergeFrequencyBands();
	m_EchoControler->AnalyzeCapture( m_SpeakerAudioBuf.get() );

	m_MicAudioBuf->SplitIntoFrequencyBands();
	m_HighPassFilter->Process( m_MicAudioBuf.get(), true );
	m_EchoControler->SetAudioBufferDelay( 0 );
	m_EchoControler->ProcessCapture( m_MicAudioBuf.get(), m_AecLinearAudioBuf.get(), false );
	m_MicAudioBuf->MergeFrequencyBands();

	m_MicAudioBuf->CopyTo( m_MicFrame );
	memcpy( echoCanceledData, m_MicFrame.data(), sampleCnt * 2 );
#endif // USE_SPEEX_ECHO_CANCEL

	return takeBufferOwnership;
}

#if defined(USE_SPEEX_ECHO_CANCEL)
//============================================================================
void AudioEchoCancelImpl::startupSpeex( int sampleCnt )
{
	resetSpeex( sampleCnt );
}

//============================================================================
void AudioEchoCancelImpl::resetSpeex( int sampleCnt )
{
	if( sampleCnt > 10 )
	{
		if( !m_SpeexState || sampleCnt != m_MicSamplesPerWrite )
		{
			// initialize or complete rebuild of speex
			shutdownSpeex();

			m_MicSamplesPerWrite = sampleCnt;

			int sampleRate = ECHO_SAMPLE_RATE;
			// speex documentation says the total buffer size should be about 100ms
			// we want the total buffer size to be a multiple of the microphone frame size
			// compromize with closest multple of frame size to 160ms
			int totalFrames = AUDIO_SAMPLES_8000_1_S16 * 2 / sampleCnt;
			if( totalFrames > 7 && totalFrames < 12 )
			{
				// prefer a power of 2 but must have at least 100 ms of tail time
				if( 100 < AudioUtils::audioDurationMs( 8000, 8 * sampleCnt ) )
				{
					totalFrames = 8;
				}
			}

			m_SpeexState = speex_echo_state_init( sampleCnt, totalFrames * sampleCnt );
			speex_echo_ctl( m_SpeexState, SPEEX_ECHO_SET_SAMPLING_RATE, &sampleRate );

			m_SpeexPreprocess = speex_preprocess_state_init( sampleCnt, sampleRate );
			speex_preprocess_ctl( m_SpeexPreprocess, SPEEX_PREPROCESS_SET_ECHO_STATE, m_SpeexState );
			bool enableDenoise = true;
			speex_preprocess_ctl( m_SpeexPreprocess, SPEEX_PREPROCESS_SET_DENOISE, &enableDenoise );

			int frameSize;
			speex_echo_ctl( m_SpeexState, SPEEX_ECHO_GET_FRAME_SIZE, &frameSize );

			LogMsg( LOG_VERBOSE, "resetSpeex frame size %d", frameSize );
		}
		else
		{
			if( !m_EchoCancelInSync )
			{
				// wait for sync between speaker read and mic write
				return;
			}

			LogMsg( LOG_VERBOSE, "resetSpeex just echo state" );
			// just reset the echo canceler state
			speex_echo_state_reset( m_SpeexState );
		}

		m_MicSamplesPerWrite = sampleCnt;
		m_SpeexInitialized = true;
		m_EchoCancelNeedsReset = false;
	}
}

//============================================================================
void AudioEchoCancelImpl::shutdownSpeex( void )
{
	if( m_SpeexState )
	{
		speex_echo_state_destroy( m_SpeexState );
		m_SpeexState = nullptr;
	}

	if( m_SpeexPreprocess )
	{
		speex_preprocess_state_destroy( m_SpeexPreprocess );
		m_SpeexPreprocess = nullptr;
	}

	m_SpeexInitialized = false;
}

#endif // defined(USE_SPEEX_ECHO_CANCEL)

//============================================================================
void AudioEchoCancelImpl::processEchoCancelFrame( int16_t* micBuf, int16_t* speakerBuf, int16_t* echoCanceledBuf )
{
	speex_echo_cancellation( m_SpeexState, micBuf, speakerBuf, echoCanceledBuf );
	//preprocess output frame (optional)
	speex_preprocess_run( m_SpeexPreprocess, echoCanceledBuf );
}

