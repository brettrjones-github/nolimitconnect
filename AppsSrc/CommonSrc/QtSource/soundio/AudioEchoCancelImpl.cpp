
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
void AudioEchoCancelImpl::enableEchoCancel( bool enable )
{
	m_EchoCancelEnable = enable;
	resetEchoCancel();
}

//============================================================================
void AudioEchoCancelImpl::resetEchoCancel( void )
{

	for( auto& micBuf : m_MicBufs )
	{
		micBuf.cleanupBuffer();
	}

	m_MicBufs.clear();

	for( auto& speakerBuf : m_SpeakerBufs )
	{
		speakerBuf.cleanupBuffer();
	}

	m_SpeakerBufs.clear();

	resetMicWriteTimeEstimator();
	resetSpeakerReadTimeEstimator();
}

//============================================================================
void AudioEchoCancelImpl::resetMicWriteTimeEstimator( void )
{
	m_MicWriteTimes.clear();
}

//============================================================================
int16_t	AudioEchoCancelImpl::micWriteTimeEstimation( int16_t systemTimeMs )
{
	return estimateTime( systemTimeMs, m_MicWriteTimes, m_MicWriteSamplesUs );
}

//============================================================================
void AudioEchoCancelImpl::resetSpeakerReadTimeEstimator( void )
{
	m_SpeakerReadTimes.clear();
}

//============================================================================
int16_t	AudioEchoCancelImpl::speakerReadTimeEstimation( int16_t systemTimeMs )
{
	return estimateTime( systemTimeMs, m_SpeakerReadTimes, m_MicWriteSamplesUs );
}

//============================================================================
int64_t AudioEchoCancelImpl::estimateTime( int16_t& systemTimeMs, std::vector<int64_t>& timesMsList, int64_t& timeIntervalUs )
{
	if( !timeIntervalUs )
	{
		return systemTimeMs;
	}

	if( systemTimeMs - timesMsList.back() > ( timeIntervalUs * 2 ) / 1000 )
	{
		// interval greater than twice the known interval between sound samples read/write so
		// sound device was probably paused or turned off or changed for some time 
		timesMsList.clear();
	}
	
	if( timesMsList.size() < MIN_TIME_ESTIMATE_COUNT )
	{
		// not enough times to make an estimate	
		timesMsList.push_back( systemTimeMs );
		return systemTimeMs;
	}

	int64_t sumOfTime{ 0 };
	for( auto& timeStamp : timesMsList )
	{
		sumOfTime += timeStamp;
	}

	sumOfTime /= timesMsList.size();
	int64_t estTime = sumOfTime - (timeIntervalUs * (timesMsList.size() + 1) / 1000);
	int diffTime = systemTimeMs - sumOfTime;
	if( std::abs( diffTime ) > ( timeIntervalUs * 2 ) / 1000 )
	{
		LogMsg( LOG_ERROR, "ERROR estimateTime diviation %d ms interval %d ms", diffTime, (int)( timeIntervalUs / 1000 ) );
	}
	else if( m_DbgLogEnable )
	{
		LogMsg( LOG_ERROR, "estimateTime diviation %d ms with %d times interval %d ms", diffTime, timesMsList.size(), (int)( timeIntervalUs / 1000 ) );
	}
	
	timesMsList.push_back( systemTimeMs );
	if( timesMsList.size() > MAX_TIME_ESTIMATE_COUNT )
	{
		timesMsList.erase( timesMsList.begin() );
	}

	return estTime;
}

//============================================================================
bool AudioEchoCancelImpl::microphoneWroteSamples( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData )
{
	if( !m_EchoCancelEnable || m_AudioIoMgr.fromGuiIsMicrophoneMuted() || m_AudioIoMgr.fromGuiIsSpeakerMuted() )
	{
		memcpy( micWriteBuf, echoCanceledData, sampleCnt * 2 );
		return false;
	}

	if( m_MicSamplesPerWrite != sampleCnt )
	{
		m_MicSamplesPerWrite = sampleCnt;
		m_MicWriteSamplesUs = calculateEchoSamplesUs( sampleCnt );
		resetMicWriteTimeEstimator();
#if defined(USE_SPEEX_ECHO_CANCEL)
		delete m_SpeexProcessBuf;
		m_SpeexProcessBuf = new int16_t[ m_MicSamplesPerWrite ];
		startupSpeex( m_MicSamplesPerWrite );
#endif // defined(USE_SPEEX_ECHO_CANCEL)
	}

#if defined(USE_SPEEX_ECHO_CANCEL)
	processSpeexEchoCancel( micWriteBuf, sampleCnt, echoCanceledData );
	return false;
#elif defined( USE_WEB_RTC_ECHO_CANCEL_1 ) }} defined( USE_WEB_RTC_ECHO_CANCEL_3 )
	if( !processWebRtcEchoCancel( micWriteBuf, sampleCnt, echoCanceledData ) )
	{
		memcpy( echoCanceledData, micWriteBuf, sampleCnt * 2 );
		return false;
	}

	return true;
#endif // defined(USE_SPEEX_ECHO_CANCEL)
}

//============================================================================
bool AudioEchoCancelImpl::speakerReadSamples( int16_t* speakerReadBuf, int sampleCnt )
{
	if( !m_EchoCancelEnable || m_AudioIoMgr.fromGuiIsMicrophoneMuted() || m_AudioIoMgr.fromGuiIsSpeakerMuted() )
	{
		m_SpeakerBufs.clear();
		return false;
	}

	if( m_SpeakerSamplesPerRead != sampleCnt )
	{
		m_SpeakerSamplesPerRead = sampleCnt;
		m_SpeakerReadSamplesUs = calculateEchoSamplesUs( sampleCnt );
		resetSpeakerReadTimeEstimator();
	}

	m_SpeakerBufs.push_back( AudioEchoBuf( speakerReadTimeEstimation( GetGmtTimeMs() ), speakerReadBuf, sampleCnt, m_AudioIoMgr.fromGuiIsSpeakerMuted() ) );
	return true;
}

//============================================================================
bool AudioEchoCancelImpl::processWebRtcEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData )
{

	if( !m_EchoCancelEnable || m_AudioIoMgr.fromGuiIsMicrophoneMuted() || m_AudioIoMgr.fromGuiIsSpeakerMuted() )
	{
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

//============================================================================
bool AudioEchoCancelImpl::fetchdSpeakerEchoSamples( int16_t* sampleBuf, int sampleCnt, int64_t timeEstimate )
{
	int samplesTotalRead = 0;
	for( auto iter = m_SpeakerBufs.begin(); iter != m_SpeakerBufs.end(); )
	{
		int sampleFromBuf = iter->readSamples( &sampleBuf[ samplesTotalRead ], sampleCnt - samplesTotalRead, timeEstimate + calculateEchoSamplesMs( samplesTotalRead ) );
		samplesTotalRead += sampleFromBuf;
		if( 0 == sampleFromBuf || iter->isEmpty() )
		{
			iter->cleanupBuffer();
			iter = m_SpeakerBufs.erase( iter );
		}
		else
		{
			iter++;
		}
	}

	return samplesTotalRead;
}

#if defined(USE_SPEEX_ECHO_CANCEL)
//============================================================================
void AudioEchoCancelImpl::startupSpeex( int sampleCnt )
{
	shutdownSpeex();
	int sampleRate = ECHO_SAMPLE_RATE;
	m_SpeexState = speex_echo_state_init( sampleCnt * ECHO_BYTES_PER_SAMPLE, sampleCnt * ECHO_BYTES_PER_SAMPLE * ECHO_BUFFER_COUNT );
	m_SpeexPreprocess = speex_preprocess_state_init( sampleCnt * ECHO_BYTES_PER_SAMPLE, sampleRate );
	speex_echo_ctl( m_SpeexState, SPEEX_ECHO_SET_SAMPLING_RATE, &sampleRate );
	speex_preprocess_ctl( m_SpeexPreprocess, SPEEX_PREPROCESS_SET_ECHO_STATE, m_SpeexState );
	m_SpeexInitialized = true;
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

//============================================================================
void AudioEchoCancelImpl::processSpeexEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData )
{
	int64_t estMicFrameTime = micWriteTimeEstimation( GetGmtTimeMs() );

	if( fetchdSpeakerEchoSamples( m_SpeexProcessBuf, sampleCnt, estMicFrameTime + getEchoDelayMsConstant() ) )
	{
		speex_echo_cancellation( m_SpeexState, micWriteBuf, m_SpeexProcessBuf, echoCanceledData );
		//preprocess output frame
		speex_preprocess_run( m_SpeexPreprocess, echoCanceledData );
	}
	else
	{
		memcpy( echoCanceledData, micWriteBuf, sampleCnt * 2 );
	}

}

#endif // defined(USE_SPEEX_ECHO_CANCEL)