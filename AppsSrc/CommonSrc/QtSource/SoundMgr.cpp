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

#include "SoundMgr.h"
#include "VxSndInstance.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <QDebug>
//#include <QSound>
#include <QMessageBox>

#undef USE_ECHO_CANCEL

//============================================================================
SoundMgr& GetSndMgrInstance( void )
{
	return GetAppInstance().getSoundMgr();
}

//============================================================================
SoundMgr::SoundMgr( AppCommon& app )
: AudioIoMgr( app, app, &app )
, m_Engine( app.getEngine() )
, m_MicrophoneInput(NULL)
, m_bMicrophoneEnabled(true)
, m_VoiceOutput(NULL)
, m_bAudioOutputStarted(false)
, m_bMutePhoneRing(false)
, m_bMuteNotifySnd(false)
, m_CurSndPlaying( 0 )
{
}

//============================================================================ 
void SoundMgr::slotStartPhoneRinging( void )
{
	playSnd( eSndDefPhoneRing1, true );
}

//============================================================================ 
void SoundMgr::slotStopPhoneRinging( void )
{
	if( m_CurSndPlaying 
		&& ( eSndDefPhoneRing1 == m_CurSndPlaying->getSndDef() ) )
	{
		m_CurSndPlaying->stopPlay();
	}
}

//============================================================================
void SoundMgr::slotPlayNotifySound( void )
{
	playSnd( eSndDefNotify1, true );
}

//============================================================================
void SoundMgr::slotPlayShredderSound( void )
{
	playSnd( eSndDefPaperShredder, true );
}

//============================================================================
void SoundMgr::mutePhoneRing( bool bMute )
{
	m_bMutePhoneRing = bMute;
	if( bMute )
	{
		slotStopPhoneRinging();
	}
}

//============================================================================
void SoundMgr::muteNotifySound( bool bMute )
{
	m_bMuteNotifySnd = bMute;
}

//============================================================================
bool SoundMgr::sndMgrStartup( void )
{
    for( int i = 0; i < eMaxSndDef; i++ )
    {
        VxSndInstance * sndInstance = new VxSndInstance( (ESndDef)i, this );
        connect( sndInstance, SIGNAL(sndFinished(VxSndInstance*)), this, SLOT(slotSndFinished(VxSndInstance*)) );
        m_SndList.push_back( sndInstance );
    }

	m_MyApp.wantToGuiHardwareCtrlCallbacks( this, true );
	return true;
}


//============================================================================
bool SoundMgr::sndMgrShutdown( void )
{
	m_MyApp.wantToGuiHardwareCtrlCallbacks( this, false );
    destroyAudioIoSystem();
	return true;
}

//============================================================================
void SoundMgr::callbackToGuiWantMicrophoneRecording( bool wantMicInput )
{
	if( wantMicInput )
	{
		startMicrophoneRecording();
	}
	else
	{
		stopMicrophoneRecording();
	}
}

//============================================================================
void SoundMgr::callbackToGuiWantSpeakerOutput( bool wantSpeakerOutput )
{
	enableSpeakerOutput( wantSpeakerOutput );
}

//============================================================================
VxSndInstance * SoundMgr::playSnd( ESndDef sndDef, bool loopContinuous  )
{

#ifdef DISABLE_AUDIO
    return 0;
#endif // DISABLE_AUDIO

	if( m_bMutePhoneRing 
		&& ( eSndDefPhoneRing1 == sndDef ) )
	{
		return m_SndList[ eSndDefNone ];
	}

	if( m_bMuteNotifySnd 
		&& ( ( eSndDefNotify1 == sndDef ) || ( eSndDefNotify2 == sndDef ) ) )
	{
		return m_SndList[ eSndDefNone ];
	}

    if( ( sndDef < m_SndList.size() )
		&& ( 0 <= sndDef ) )
	{
		if( m_CurSndPlaying )
		{
			m_CurSndPlaying->stopPlay();
		}

		m_CurSndPlaying = m_SndList[ sndDef ];
		m_CurSndPlaying->startPlay( loopContinuous );
		return m_CurSndPlaying;
	}
	else
	{
		return m_SndList[ eSndDefNone ];
	}
}

//============================================================================
void SoundMgr::stopSnd( ESndDef sndDef )
{
	if( m_CurSndPlaying 
		&& ( sndDef == m_CurSndPlaying->getSndDef() ) )
	{
		m_CurSndPlaying->stopPlay();
		m_CurSndPlaying = 0;
	}
}

//============================================================================
void SoundMgr::slotSndFinished( VxSndInstance * sndInstance )
{
	if( m_CurSndPlaying == sndInstance )
	{
		m_CurSndPlaying = 0;
	}
}

//============================================================================
//! called when microphone recoding is needed
void SoundMgr::startMicrophoneRecording()
{
	enableMicrophoneInput( true );
}

//============================================================================
//! called when microphone recoding not needed
void SoundMgr::stopMicrophoneRecording()
{
	enableMicrophoneInput( false );
}

//============================================================================
void SoundMgr::enableMicrophoneInput( bool enable )
{
	if( enable != m_bMicrophoneEnabled )
	{
		m_bMicrophoneEnabled = enable;
		LogMsg( LOG_INFO, "Microphone Enable %d\n", enable );
	}
}

//============================================================================
void SoundMgr::enableSpeakerOutput( bool bEnable )
{
	if( bEnable != m_bVoiceOutputEnabled )
	{
		m_bVoiceOutputEnabled = bEnable;
	}
}
