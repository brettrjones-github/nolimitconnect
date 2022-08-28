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

#include "AudioSampleBuf.h"

#include <CoreLib/TimeIntervalEstimator.h>
#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>

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

	AudioEchoCancelImpl( AppCommon& appCommon, AudioIoMgr& audioIoMgr, QObject* parent );
	virtual ~AudioEchoCancelImpl();

	void						echoCancelStartup( void );
	void						echoCancelShutdown( void );

	void						enableEchoCancel( bool enable );
	bool						isEchoCancelEnabled( void )						{ return m_EchoCancelEnable; };

	void						setEchoDelayMsConstant( int delayMs )			{ m_EchoDelayMsConstant = delayMs; }
	int							getEchoDelayMsConstant( void )					{ return m_EchoDelayMsConstant; }

	void						setPeakAmplitudeDebugEnable( bool enableDebug ) { m_PeakAmplitudeDebug = enableDebug; }
	bool						getPeakAmplitudeDebugEnable( void )				{ return m_PeakAmplitudeDebug; }

	void						setSyncDebugEnabled( bool enableSyncDebug )		{ m_SyncDebugEnabled = enableSyncDebug; }
	bool						getSyncDebugEnabled( void )						{ return m_SyncDebugEnabled; }

	bool						getIsInSync( void ) { return m_EchoCancelInSync; }

	void						speakerReadSamples( int16_t* speakerReadData, int sampleCnt, int64_t speakerReadTailTimeMs, bool stableTimestamp );

	void						micWriteSamples( int16_t* micWriteData, int sampleCnt, int64_t micWriteTailTimeMs, bool stableTimestamp );

	void						frame80msElapsed( void ); // called from master clock

	void                        setEchoCancelerNeedsReset( bool needReset )		{ m_EchoCancelNeedsReset = needReset; }

	static int64_t				calculateEchoSamplesUs( int sampleCnt )			{ return (sampleCnt * 1000000) / ECHO_SAMPLE_RATE; }
	static int64_t				calculateEchoSamplesMs( int sampleCnt )			{ return (sampleCnt * 1000) / ECHO_SAMPLE_RATE; }

	void						processEchoRunThreaded( void );

protected:
	void						processEchoCancelThreaded( void );

	void						checkFor80msFrameElapsedThreaded( void );

	bool						attemptEchoSync( void );

	void						resetEchoCancel( void );

	void						processEchoCancelFrame( int16_t* micBuf, int16_t* speakerBuf, int16_t* echoCanceledBuf );

	bool						processWebRtcEchoCancel( int16_t* micWriteBuf, int sampleCnt, int16_t* echoCanceledData );

#if defined(USE_SPEEX_ECHO_CANCEL)
	void						startupSpeex( int sampleCnt );
	void						resetSpeex( int sampleCnt );
	void						shutdownSpeex( void );
#endif // defined(USE_SPEEX_ECHO_CANCEL)
	
	//=== vars ===//
	AppCommon&					m_MyApp;
	AudioIoMgr&					m_AudioIoMgr;
	bool						m_DbgLogEnable{ false };

	bool						m_EchoCancelEnable{ false };
	bool						m_EchoCancelNeedsReset{ false };

	int							m_EchoDelayMsConstant{ 100 };

	int							m_MicSamplesPerWrite{ 0 };
	int64_t						m_MicWriteSamplesUs{ 0 };

	int							m_SpeakerSamplesPerRead{ 0 };
	int64_t						m_SpeakerReadSamplesUs{ 0 };

	bool						m_EchoCancelInSync{ false };
	bool						m_SyncDebugEnabled{ false };
	bool						m_PeakAmplitudeDebug{ false };

	bool						m_Frame80msElapsed{ false };

	VxMutex						m_SpeakerSamplesMutex;
	AudioSampleBuf				m_SpeakerSamples;
	int64_t						m_TailSpeakerSamplesMs{ 0 };
	int64_t						m_HeadSpeakerSamplesMs{ 0 };
	bool						m_StableSpeakerTimestamp{ false };

	VxMutex						m_MicSamplesMutex;
	AudioSampleBuf				m_MicSamples;
	int64_t						m_TailMicSamplesMs{ 0 };
	int64_t						m_HeadMicSamplesMs{ 0 };
	bool						m_StableMicTimestamp{ false };

	VxMutex						m_EchoCanceledSamplesMutex;
	AudioSampleBuf				m_EchoCanceledSamples;
	int64_t						m_TailEchoCanceledSamplesMs{ 0 };
	int64_t						m_HeadEchoCanceledSamplesMs{ 0 };

	VxSemaphore					m_ProcessEchoSemaphore;
	VxThread					m_ProcessEchoThread;

	AudioSampleBuf				m_ProcessSpeakerSamples;
	AudioSampleBuf				m_ProcessMicSamples;

	int16_t						m_QuietSamplesBuf[ MIXER_CHUNK_LEN_SAMPLES ];

#if defined(USE_SPEEX_ECHO_CANCEL)
	SpeexEchoState*				m_SpeexState{ nullptr };
	SpeexPreprocessState*		m_SpeexPreprocess{ nullptr };
	bool						m_SpeexInitialized{ false };

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
	int16_t* m_MicEchoBuf10ms{ nullptr };
	int16_t* m_MicRemainderBuf{ nullptr };
	int							m_MicRemainderSampleCnt{ 0 };
	int16_t* m_SpeakerBuf10ms{ nullptr };
#endif //#if defined(USE_WEB_RTC_ECHO_CANCEL_1) || defined(USE_WEB_RTC_ECHO_CANCEL_3)

};
