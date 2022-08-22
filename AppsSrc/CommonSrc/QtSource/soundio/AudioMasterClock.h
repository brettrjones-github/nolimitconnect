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

	void						audioMasterClockStartup( void );
	void						audioMasterClockShutdown( void );

	void						audioSpeakerReadUs( int64_t readSizeUs, bool resetTimer );

protected slots:
	void						slotAudioTimerTimeout( void );

protected:
	//=== vars ===//
	AudioIoMgr&					m_AudioIoMgr;
	AppCommon&					m_MyApp;
	QTimer*						m_AudioTimer;

	int64_t						m_SpeakerConsumedUs{ 0 };
	int64_t						m_NextFrameTimeMs{ 0 };
};