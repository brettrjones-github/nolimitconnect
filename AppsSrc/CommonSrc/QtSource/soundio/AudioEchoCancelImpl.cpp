
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

//============================================================================
AudioEchoCancelImpl::AudioEchoCancelImpl( AppCommon& appCommon, AudioIoMgr& audioIoMgr, QObject* parent )
	: QObject( parent )
	, m_MyApp( appCommon )
	, m_AudioIoMgr( audioIoMgr )
{
	memset( m_QuietSamplesBuf, 0, sizeof( m_QuietSamplesBuf ) );
}

//============================================================================
AudioEchoCancelImpl::~AudioEchoCancelImpl()
{
}

//============================================================================
void AudioEchoCancelImpl::echoCancelStartup( void )
{
#if defined( USE_SPEEX_ECHO_CANCEL )
	// do nothing yet.. need the microphone frame size in order to setup
	m_EchoCancelNeedsReset = true;
#elif defined( USE_WEB_RTC_ECHO_CANCEL_1 )

	m_EchoCancel.startupEchoCancel();

#elif defined( USE_WEB_RTC_ECHO_CANCEL_3 )
	int ref_sample_rate = 8000;
	int ref_channels = 1;
	int rec_sample_rate = 8000;
	int rec_channels = 1;
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
}

//============================================================================
void AudioEchoCancelImpl::enableEchoCancel( bool enable )
{
	m_EchoCancelEnable = enable;
}

//============================================================================
void AudioEchoCancelImpl::resetEchoCanceler( int samplesPerEchoFrame )
{
#if defined(USE_SPEEX_ECHO_CANCEL)
	resetSpeex( MONO_8000HZ_SAMPLES_PER_10MS );
#elif defined(USE_WEB_RTC_ECHO_CANCEL_1)

#endif // defined(USE_SPEEX_ECHO_CANCEL)
}

//============================================================================
void AudioEchoCancelImpl::speakerReadSamples( int16_t* speakerReadData, int sampleCnt, int64_t speakerReadTailTimeMs, bool stableTimestamp )
{
	m_StableSpeakerTimestamp = stableTimestamp;
	m_SpeakerLastCallTimestamp = GetHighResolutionTimeMs();
	m_SpeakerSamplesMutex.lock();
	m_SpeakerSamples.writeSamples( speakerReadData, sampleCnt );
	m_TailSpeakerSamplesMs = speakerReadTailTimeMs;
	m_HeadSpeakerSamplesMs = speakerReadTailTimeMs - AudioUtils::audioDurationMs( 8000, m_SpeakerSamples.getSampleCnt() );

	m_SpeakerSamplesMutex.unlock();
}

//============================================================================
void AudioEchoCancelImpl::micWroteSamples( int16_t* micWriteData, int sampleCnt, int64_t micWriteTailTimeMs, bool stableTimestamp )
{
	m_StableMicTimestamp = stableTimestamp;
	m_MicLastCallTimestamp = GetHighResolutionTimeMs();
	m_MicSamplesMutex.lock();
	m_MicSamples.writeSamples( micWriteData, sampleCnt );
	m_TailMicSamplesMs = micWriteTailTimeMs;
	m_HeadMicSamplesMs = micWriteTailTimeMs - AudioUtils::audioDurationMs( 8000, m_MicSamples.getSampleCnt() );

	m_MicSamplesMutex.unlock();
}

//============================================================================
void AudioEchoCancelImpl::processEchoCancelThreaded( AudioSampleBuf& speakerProcessed8000Buf, QMutex& speakerProcessedMutex )
{
	// release speaker samples as soon as possible to avoid stalling the device speaker read call
	m_SpeakerSamplesMutex.lock();
	m_ProcessSpeakerSamples.writeSamples( speakerProcessed8000Buf.getSampleBuffer(), speakerProcessed8000Buf.getSampleCnt() );
	m_SpeakerSamplesMutex.unlock();

	speakerProcessed8000Buf.clear();
	speakerProcessedMutex.unlock();

	// release mic samples as soon as possible to avoid stalling the device microphone write call
	m_MicSamplesMutex.lock();
	m_ProcessMicSamples.writeSamples( m_MicSamples.getSampleBuffer(), m_MicSamples.getSampleCnt() );
	m_MicSamples.clear();
	m_MicSamplesMutex.unlock();

	static int64_t lastTime = 0;
	int64_t timeNow = GetHighResolutionTimeMs();
	int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
	lastTime = timeNow;

	if( m_ProcessSpeakerSamples.getSampleCnt() >= MIXER_CHUNK_LEN_SAMPLES && m_ProcessMicSamples.getSampleCnt() >= MIXER_CHUNK_LEN_SAMPLES )
	{
		m_EchoCancelInSync = attemptEchoSyncThreaded();
		if( m_EchoCancelInSync )
		{
			if( m_EchoCancelNeedsReset )
			{
				m_EchoCanceledSamplesMutex.lock();
				m_EchoCanceledSamples.clear();
				m_EchoCanceledSamplesMutex.unlock();

				// for now always use 10ms frames of samples. webRtc requires it and speex will work with it
				resetEchoCanceler( MONO_8000HZ_SAMPLES_PER_10MS );
			}

			static int64_t startMs = 0;
			if( m_AudioIoMgr.getAudioTimingEnable() )
			{
				startMs = timeNow;
			}

			int framesToProcess = std::min( m_ProcessMicSamples.getSampleCnt() / MONO_8000HZ_SAMPLES_PER_10MS, m_ProcessSpeakerSamples.getSampleCnt() / MONO_8000HZ_SAMPLES_PER_10MS );
			framesToProcess = std::min( framesToProcess, m_EchoCanceledSamples.freeSpaceSampleCount() / MONO_8000HZ_SAMPLES_PER_10MS );
			// not sure this is neccesary.. limit processing to one mixer buffer size
			framesToProcess = std::min( framesToProcess, MIXER_CHUNK_LEN_SAMPLES / MONO_8000HZ_SAMPLES_PER_10MS );

			if( framesToProcess )
			{
				int samplesToProcess = framesToProcess * MONO_8000HZ_SAMPLES_PER_10MS;

				int64_t preProceesMicTimeMs = m_HeadMicSamplesMs;
				int samplesProcessedMs = AudioUtils::audioDurationMs( 8000, samplesToProcess );
				m_HeadMicSamplesMs += samplesProcessedMs;
				m_HeadSpeakerSamplesMs += samplesProcessedMs;

				// only this thread touches the processed samples.. might be able to do away with echo canceled mutex also
				m_EchoCanceledSamplesMutex.lock();

				int16_t* micBuf = m_ProcessMicSamples.getSampleBuffer();
				int16_t* speakerBuf = m_ProcessSpeakerSamples.getSampleBuffer();
				int16_t* echoCanceledBuf = m_EchoCanceledSamples.getSampleBuffer();
				echoCanceledBuf = &echoCanceledBuf[ m_EchoCanceledSamples.getSampleCnt() ];

				for( int i = 0; i < framesToProcess; i++ )
				{
					processEchoCancelFrame( micBuf, speakerBuf, MONO_8000HZ_SAMPLES_PER_10MS, echoCanceledBuf );

					micBuf += MONO_8000HZ_SAMPLES_PER_10MS;
					speakerBuf += MONO_8000HZ_SAMPLES_PER_10MS;
					echoCanceledBuf += MONO_8000HZ_SAMPLES_PER_10MS;
				}

				m_EchoCanceledSamples.setSampleCnt( m_EchoCanceledSamples.getSampleCnt() + samplesToProcess );
				m_EchoCanceledSamplesMutex.unlock();

				m_ProcessMicSamples.samplesWereRead( samplesToProcess );
				m_ProcessSpeakerSamples.samplesWereRead( samplesToProcess );

				if( m_AudioIoMgr.getIsEchoCancelInSync() )
				{
					// LogMsg( LOG_VERBOSE, "P processSpeexEchoCancel echo canceled samples %d total now %d", samplesToProcess, m_EchoCanceledSamples.getSampleCnt() );
				}
			}

			if( m_AudioIoMgr.getAudioTimingEnable() )
			{
				int64_t endtMs = GetHighResolutionTimeMs();
				if( endtMs - startMs > 10 )
				{
					LogMsg( LOG_VERBOSE, "P AudioEchoCancelImpl::processEchoCancelThreaded took %d ms", (int)(endtMs - startMs) );
				}
			}

			// only this thread touches the processed samples.. might be able to do away with echo canceled mutex also
			m_EchoCanceledSamplesMutex.lock();

			if( m_EchoCanceledSamples.getSampleCnt() >= MIXER_CHUNK_LEN_SAMPLES )
			{
				if( m_AudioIoMgr.getIsMicrophoneMuted() )
				{
					m_AudioIoMgr.fromGuiEchoCanceledSamplesThreaded( m_QuietSamplesBuf, MIXER_CHUNK_LEN_SAMPLES, true );
				}
				else
				{
					m_AudioIoMgr.fromGuiEchoCanceledSamplesThreaded( m_EchoCanceledSamples.getSampleBuffer(), MIXER_CHUNK_LEN_SAMPLES, false );
				}
				
				m_EchoCanceledSamples.samplesWereRead( MIXER_CHUNK_LEN_SAMPLES );
			}
		}
		else
		{
			// not in sync
			if( m_AudioIoMgr.getIsEchoCancelInSync() )
			{
				LogMsg( LOG_VERBOSE, "P AudioEchoCancelImpl::processEchoCancelThreaded no frames to process" );
			}
		}

		m_EchoCanceledSamplesMutex.unlock();
	}
	else
	{
		// not enough samples to do echo cancel
		//if( m_AudioIoMgr.getIsEchoCancelInSync() )
		//{
		//	LogMsg( LOG_VERBOSE, "P AudioEchoCancelImpl::processEchoCancelThreaded elapsed %d ms not enough samples echo %d speaker %d", 
		//		timeElapsed, m_MicSamples.getSampleCnt(), m_SpeakerSamples.getSampleCnt() );
		//}
	}
}

//============================================================================
bool AudioEchoCancelImpl::attemptEchoSyncThreaded( void )
{
	bool inSync{ m_EchoCancelInSync };
	return true; // BRJ for testing just say in sync

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
		if( m_HeadSpeakerSamplesMs < targetSpeakerTimeMs - 300 || m_HeadSpeakerSamplesMs > targetSpeakerTimeMs + 300 )
		{
			LogMsg( LOG_VERBOSE, "attemptEchoSync fell OUT of sync diff %d ms mic sample ms %d speaker sample ms %d", (int)(m_HeadSpeakerSamplesMs - targetSpeakerTimeMs),
				m_MicSamples.getAudioDurationMs(), m_SpeakerSamples.getAudioDurationMs() );
			inSync = false;
			m_EchoCancelNeedsReset = true;
			m_AudioIoMgr.echoCancelSyncStateThreaded( false );
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
			m_AudioIoMgr.echoCancelSyncStateThreaded( true );
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
void AudioEchoCancelImpl::processWebRtc1EchoCancel( int16_t* micWriteBuf, int16_t* speakerBuf, int sampleCnt, int16_t* echoCanceledData )
{

	if( !m_EchoCancelEnable )
	{
		memcpy( echoCanceledData, micWriteBuf, sampleCnt * 2 );
		return;
	}

#if defined( USE_WEB_RTC_ECHO_CANCEL_1 )
	m_EchoCancel.processEchoCancel( micWriteBuf, sampleCnt, speakerBuf, echoCanceledData, getEchoDelayMsConstant() );
#endif // USE_WEB_RTC_ECHO_CANCEL_1
}

//============================================================================
void AudioEchoCancelImpl::processWebRtc3EchoCancel( int16_t* micWriteBuf, int16_t* speakerBuf, int sampleCnt, int16_t* echoCanceledData )
{

	if( !m_EchoCancelEnable )
	{
		memcpy( echoCanceledData, micWriteBuf, sampleCnt * 2 );
		return;
	}

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
#endif // USE_WEB_RTC_ECHO_CANCEL_1
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
void AudioEchoCancelImpl::processEchoCancelFrame( int16_t* micBuf, int16_t* speakerBuf, int samplesPerFrame, int16_t* echoCanceledBuf )
{
#if defined(USE_SPEEX_ECHO_CANCEL)
	speex_echo_cancellation( m_SpeexState, micBuf, speakerBuf, echoCanceledBuf );
	//preprocess output frame (optional)
	speex_preprocess_run( m_SpeexPreprocess, echoCanceledBuf );
#elif defined(USE_WEB_RTC_ECHO_CANCEL_1)
	processWebRtc1EchoCancel( micBuf, speakerBuf, samplesPerFrame, echoCanceledBuf );
#elif defined(USE_WEB_RTC_ECHO_CANCEL_3)
	processWebRtc3EchoCancel( micBuf, speakerBuf, samplesPerFrame, echoCanceledBuf );
#endif // defined(USE_SPEEX_ECHO_CANCEL)
}

