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

// in an ideal world the sound out request for data would be 80ms and everthing would clock off of that
// because all the sound packets assume everbodys sound in in sync
// this class will attempt to call space available every 80 ms in sync with sound consumption by speaker output

#include <QTimer>
#include <QElapsedTimer>

class AppCommon;
class AudioIoMgr;

class AudioMasterClock : public QObject
{
	Q_OBJECT
public:
	AudioMasterClock( AudioIoMgr& audioIoMgr, QObject* parent = nullptr);

	void						masterClockShutdown( void )					{ m_AudioTimer->stop(); }

	void                        microphoneDeviceEnabled( bool isEnabled );
	void                        speakerDeviceEnabled( bool isEnabled );

	void						audioMicWriteDurationTime( int64_t writeDurationMs );
	void						audioMicWriteSampleCnt( int64_t writeSampleCnt );

	void						audioSpeakerReadDurationTime( int64_t readSizeTime );
	void						audioSpeakerReadSampleCnt( int64_t readSampleCnt );

protected slots:
	void						slotAudioTimerTimeout( void );

protected:
	void						enableMasterClock( bool enable );
	void						resetMasterClock( void );
	void						updateDeviceEnable( void );

	//=== vars ===//
	AudioIoMgr&					m_AudioIoMgr;
	AppCommon&					m_MyApp;
	QTimer*						m_AudioTimer;

	bool						m_MicDeviceEnabled{ false };
	bool						m_ResetOnFirstMicWrite{ false };

	bool						m_SpeakerDeviceEnabled{ false };
	bool						m_ResetOnFirstSpeakerWrite{ false };

	int64_t						m_MicConsumedTime{ 0 };
	int64_t						m_SpeakerConsumedTime{ 0 };

	int64_t						m_NextFrameTimeMs{ 0 };

	int64_t						m_MicConsumedTotal{ 0 };
	int64_t						m_SpeakerConsumedTotal{ 0 };
	int64_t						m_MicSamplesTotal{ 0 };
	int64_t						m_SpeakerSamplesTotal{ 0 };
};