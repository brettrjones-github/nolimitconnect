#pragma once
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AudioDefs.h"

#include "AudioEchoBuf.h"

#if defined(USE_SPEEX_ECHO_CANCEL)
# include <libspeex/speex/speex_echo.h>
# include <libspeex/speex/speex_preprocess.h>
#elif defined(USE_WEB_RTC_ECHO_CANCEL_1)
# include <libechocancel/EchoCancel.h>
#elif defined(USE_WEB_RTC_ECHO_CANCEL_3)
# include <libdsp-webrtc/modules/audio_processing/aec3/echo_canceller3.h>
# include <api/audio/audio_frame.h>
# include <modules/audio_processing/high_pass_filter.h>
# include <modules/audio_processing/include/audio_processing.h>
#endif // USE_SPEEX_ECHO_CANCEL

#include <QObject>

class AppCommon;
class AudioIoMgr;
class QAudioFormat;

class AudioEchoCancelImpl : public QObject
{
	Q_OBJECT;

public:

	static const int			MIN_TIME_ESTIMATE_COUNT = 3; // minimun number of system times to estimate what the next time read/write samples time should be
	static const int			MAX_TIME_ESTIMATE_COUNT = 8; // dont grow the number of sample times past this or takes to much cpu time for little gain in accuracy


	AudioEchoCancelImpl( AppCommon& appCommon, AudioIoMgr& audioIoMgr, QObject* parent );
	virtual ~AudioEchoCancelImpl();

	void						echoCancelStartup( void );
	void						echoCancelShutdown( void );

	void						enableEchoCancel( bool enable );
	bool						isEchoCancelEnabled( void )			{ return m_EchoCancelEnable; };

	int							getEchoDelayMsConstant( void )		{ return m_EchoDelayMsConstant; }

	// return false if echo cancel does not take ownership of buffer. if true caller should not delete buffer
	bool						microphoneWroteSamples( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData );

	// return false if echo cancel does not take ownership of buffer. if true caller should not delete buffer
	bool						speakerReadSamples( int16_t* speakerReadBuf, int sampleCnt );

	static int64_t				calculateEchoSamplesUs( int sampleCnt ) { return (sampleCnt * 1000000) / ECHO_SAMPLE_RATE; }
	static int64_t				calculateEchoSamplesMs( int sampleCnt ) { return (sampleCnt * 1000) / ECHO_SAMPLE_RATE; }

protected:

	void						resetEchoCancel( void );

	void						resetMicWriteTimeEstimator( void );
	int16_t						micWriteTimeEstimation( int16_t systemTimeMs );

	void						resetSpeakerReadTimeEstimator( void );
	int16_t						speakerReadTimeEstimation( int16_t systemTimeMs );

	int64_t						estimateTime( int16_t& systemTimeMs, std::vector<int64_t>& timesMsList, int64_t& timeIntervalUs );

	bool						fetchdSpeakerEchoSamples( int16_t* sampleBuf, int sampleCnt, int64_t timeEstimate );

	bool						processWebRtcEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData );

#if defined(USE_SPEEX_ECHO_CANCEL)
	void						startupSpeex( int sampleCnt );
	void						shutdownSpeex( void );
	void						processSpeexEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData );
#endif // defined(USE_SPEEX_ECHO_CANCEL)
	
	//=== vars ===//
	AppCommon&					m_MyApp;
	AudioIoMgr&					m_AudioIoMgr;
	bool						m_EchoCancelEnable{ false };
	bool						m_DbgLogEnable{ false };

	int							m_EchoDelayMsConstant{ 100 };

	int							m_MicSamplesPerWrite{ 0 };
	int64_t						m_MicWriteSamplesUs{ 0 };
	std::vector<int64_t>		m_MicWriteTimes;
	std::vector<AudioEchoBuf>	m_MicBufs;

	int							m_SpeakerSamplesPerRead{ 0 };
	int64_t						m_SpeakerReadSamplesUs{ 0 };
	std::vector<int64_t>		m_SpeakerReadTimes;
	std::vector<AudioEchoBuf>	m_SpeakerBufs;

#if defined(USE_SPEEX_ECHO_CANCEL)
	SpeexEchoState*				m_SpeexState{ nullptr };
	SpeexPreprocessState*		m_SpeexPreprocess{ nullptr };
	bool						m_SpeexInitialized{ false };
	int16_t*					m_SpeexProcessBuf{ nullptr };
#elif defined(USE_WEB_RTC_ECHO_CANCEL_1)
	EchoCancel					m_EchoCancel;
#elif defined(USE_WEB_RTC_ECHO_CANCEL_3)
	std::unique_ptr<webrtc::EchoControl>	m_EchoControler;
	std::unique_ptr<webrtc::HighPassFilter> m_HighPassFilter;

	webrtc::StreamConfig					m_MicStreamConfig;
	std::unique_ptr<webrtc::AudioBuffer>	m_MicAudioBuf;
	webrtc::AudioFrame						m_MicFrame;

	webrtc::StreamConfig					m_SpeakerStreamConfig;
	std::unique_ptr<webrtc::AudioBuffer>	m_SpeakerAudioBuf;
	webrtc::AudioFrame						m_SpeakerFrame;
#endif // USE_SPEEX_ECHO_CANCEL

#if defined(USE_WEB_RTC_ECHO_CANCEL_1) || defined(USE_WEB_RTC_ECHO_CANCEL_3)
	int							m_SamplesPer10ms{ 0 };
	int16_t*					m_MicEchoBuf10ms{ nullptr };
	int16_t*					m_MicRemainderBuf{ nullptr };
	int							m_MicRemainderSampleCnt{ 0 };
	int16_t*					m_SpeakerBuf10ms{ nullptr };
#endif //#if defined(USE_WEB_RTC_ECHO_CANCEL_1) || defined(USE_WEB_RTC_ECHO_CANCEL_3)
};
