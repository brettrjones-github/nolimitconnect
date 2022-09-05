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

#include <QObject>

class AppCommon;
class AudioIoMgr;
class AudioEchoCancelImpl;
class AudioSampleBuf;

class QAudioFormat;
class QMutex;

class AudioEchoCancel : public QObject
{
	Q_OBJECT;

public:
	AudioEchoCancel( AppCommon& appCommon, AudioIoMgr& audioIoMgr, QObject* parent );
	virtual ~AudioEchoCancel() = default;

	void						echoCancelStartup( void );

	void						enableEchoCancel( bool enable );
	bool						isEchoCancelEnabled( void );

	void                        setEchoDelayMsParam( int delayMs );

	void						speakerReadSamples( int16_t* speakerReadData, int sampleCnt, int64_t speakerReadTailTimeMs, bool stableTimestamp );

	void						micWroteSamples( int16_t* micWritedData, int sampleCnt, int64_t micWriteTailTimeMs, bool stableTimestamp );

	void						processEchoCancelThreaded( AudioSampleBuf& speakerProcessed8000Buf, QMutex& speakerProcessedMutex );

	void						setEchoCancelerNeedsReset( bool needReset );

	bool						getIsInSync( void );

	void						setPeakAmplitudeDebugEnable( bool enableDebug );
	bool						getPeakAmplitudeDebugEnable( void );

	AppCommon&					m_MyApp;
	AudioIoMgr&					m_AudioIoMgr;
	AudioEchoCancelImpl&		m_AudioEchoCancelImpl;
};
