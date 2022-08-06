
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
	delete m_MicEchoBuf10ms;
	delete m_MicRemainderBuf;
	delete m_SpeakerBuf10ms;
}

//============================================================================
void AudioEchoCancelImpl::echoCancelStartup( void )
{
	m_SamplesPer10ms = EchoCancel::ECHO_SAMPLE_RATE_HZ / 100;

	// Qt does not always have nice 10ms even divisible mic write or speaker reads so need a remainder buffer
	m_MicEchoBuf10ms = new int16_t[  m_SamplesPer10ms ];
	m_MicRemainderBuf = new int16_t[ m_SamplesPer10ms ];
	m_MicRemainderSampleCnt = 0;
	m_SpeakerBuf10ms = new int16_t[ m_SamplesPer10ms ];

#ifdef USE_ECHO_CANCEL_3
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
#endif // USE_ECHO_CANCEL_3
}

//============================================================================
int64_t AudioEchoCancelImpl::calculateSamplesUs( int sampleCnt )
{
	return (sampleCnt * 1000000) / EchoCancel::ECHO_SAMPLE_RATE_HZ;
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
int16_t AudioEchoCancelImpl::estimateTime( int16_t systemTimeMs, std::vector<int64_t>& timesMsList, int64_t timeIntervalUs )
{
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
	if( m_EchoCancelEnable )
	{
		if( m_MicSamplesPerWrite != sampleCnt )
		{
			m_MicSamplesPerWrite = sampleCnt;
			m_MicWriteSamplesUs = calculateSamplesUs( sampleCnt );
			resetMicWriteTimeEstimator();
		}

		if( !processEchoCancel( micWriteBuf, sampleCnt, echoCanceledData ) )
		{
			memcpy( echoCanceledData, micWriteBuf, sampleCnt * 2 );
		}

		return true;
	}
	else
	{
		memcpy( micWriteBuf, echoCanceledData, sampleCnt * 2 );
		return false;
	}
}

//============================================================================
bool AudioEchoCancelImpl::speakerReadSamples( int16_t* speakerReadBuf, int sampleCnt )
{
	if( m_EchoCancelEnable )
	{
		if( m_SpeakerSamplesPerRead != sampleCnt )
		{
			m_SpeakerSamplesPerRead = sampleCnt;
			m_SpeakerReadSamplesUs = calculateSamplesUs( sampleCnt );
			resetSpeakerReadTimeEstimator();
		}

		m_SpeakerBufs.push_back( AudioEchoBuf( speakerReadTimeEstimation( GetGmtTimeMs() ), speakerReadBuf, sampleCnt, m_AudioIoMgr.fromGuiIsSpeakerMuted() ) );
		return true;
	}

	return false;
}

//============================================================================
bool AudioEchoCancelImpl::processEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData )
{
    // int64_t micTimeMs = micWriteTimeEstimation( GetGmtTimeMs() );
	bool micIsMuted = m_AudioIoMgr.fromGuiIsMicrophoneMuted();
	bool speakerMuted = m_AudioIoMgr.fromGuiIsSpeakerMuted();
	if( !m_EchoCancelEnable || micIsMuted || speakerMuted )
	{
		return false;
	}

	bool wasEchoCanceled{ false };
    // int samplesProcessed{ 0 };
	bool echoProcessing = true;

	while( echoProcessing && m_SpeakerBufs.size() )
	{
		echoProcessing = false;
		// always work backwards from end of buffer to begining which is oldest to newest samples
		//int micSamplesInBufCnt = fillMicEchoBuf( micWriteBuf, sampleCnt, samplesProcessed );
		//if( micSamplesInBufCnt != m_SamplesPer10ms )
		//{
		//	// not enough mic samples to continue;
		//	break;
		//}

#ifndef USE_ECHO_CANCEL_3

#else



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
#endif // USE_ECHO_CANCEL_3
	}


	return wasEchoCanceled;
}
