
#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "SoundDefs.h"
#include "soundio/AudioIoMgr.h"

#include "ToGuiHardwareControlInterface.h"

#include <QVector>
#include <QObject>

const uint32_t					MY_FRAME_DATA_LEN			= 1280;
const uint32_t					MY_FRAME_SAMPLES			= 640;

class AppCommon;
class P2PEngine;
class QIODevice;
class QSound;
class MyQtSoundInput;
class MyQtSoundOutput;
class VxSndInstance;

class MySndMgr : public AudioIoMgr, public ToGuiHardwareControlInterface
{
	Q_OBJECT
public:
	MySndMgr( AppCommon& app );
	virtual ~MySndMgr() override = default;

	AppCommon&				    getApp( void )						{ return m_MyApp; }

	bool						sndMgrStartup( void );
	bool						sndMgrShutdown( void );

    virtual void 				callbackToGuiWantMicrophoneRecording( bool wantMicInput ) override;
    virtual void 				callbackToGuiWantSpeakerOutput( bool wantSpeakerOutput ) override;

	VxSndInstance *			    playSnd( ESndDef sndDef, bool loopContinuous = false );
	void						stopSnd( ESndDef sndDef );

	void						startMicrophoneRecording();
	void						stopMicrophoneRecording();

signals:
	void						signalMicrophoneAudioData( QByteArray audioData );

public slots:
	void						mutePhoneRing( bool bMute );
	void						muteNotifySound( bool bMute );
	void						slotStartPhoneRinging( void );
	void						slotStopPhoneRinging( void );
	void						slotPlayNotifySound( void );
	void						slotPlayShredderSound( void );

private slots:
	void						slotSndFinished( VxSndInstance * sndInstance );

protected:
	void						enableMicrophoneInput( bool enable );
	void						enableSpeakerOutput( bool bEnable );

	//=== vars ===//
	AppCommon&				    m_MyApp;
	P2PEngine&					m_Engine;
	MyQtSoundInput *			m_MicrophoneInput;
	bool						m_bMicrophoneEnabled;

	MyQtSoundOutput *			m_VoiceOutput;
	bool						m_bVoiceOutputEnabled;
	bool						m_bAudioOutputStarted;

	bool						m_bMutePhoneRing;
	bool						m_bMuteNotifySnd;

	QVector<VxSndInstance *>	m_SndList;
	VxSndInstance *			    m_CurSndPlaying;
};

