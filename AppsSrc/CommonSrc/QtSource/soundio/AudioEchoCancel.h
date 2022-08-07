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
class QAudioFormat;
class AudioEchoCancelImpl;

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

	// return false if echo cancel does not take ownership of buffer. if true caller should not delete buffer
	bool						speakerReadSamples( int16_t* mono8000ReadBuf, int sampleCnt);
	// return false if echo cancel does not take ownership of buffer. if true caller should not delete buffer
	bool						microphoneWroteSamples( int16_t* mono8000WriteBuf, int sampleCnt, int16_t* echoCanceledData );

	AppCommon&					m_MyApp;
	AudioIoMgr&					m_AudioIoMgr;
	AudioEchoCancelImpl&		m_AudioEchoCancelImpl;
};
