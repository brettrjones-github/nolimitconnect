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
// http://www.nolimitconnect.com
//============================================================================

#include <app_precompiled_hdr.h>
#include "MySndMgr.h"
#include "VxSndInstance.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <QDebug>
//#include <QSound>
#include <QMessageBox>

#undef USE_ECHO_CANCEL

//============================================================================
MySndMgr& GetSndMgrInstance( void )
{
	return GetAppInstance().getSoundMgr();
}

//============================================================================
MySndMgr::MySndMgr( AppCommon& app )
: AudioIoMgr( app, &app )
, m_MyApp( app )
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
void MySndMgr::slotStartPhoneRinging( void )
{
	playSnd( eSndDefPhoneRing1, true );
}

//============================================================================ 
void MySndMgr::slotStopPhoneRinging( void )
{
	if( m_CurSndPlaying 
		&& ( eSndDefPhoneRing1 == m_CurSndPlaying->getSndDef() ) )
	{
		m_CurSndPlaying->stopPlay();
	}
}

//============================================================================
void MySndMgr::slotPlayNotifySound( void )
{
	playSnd( eSndDefNotify1, true );
}

//============================================================================
void MySndMgr::slotPlayShredderSound( void )
{
	playSnd( eSndDefPaperShredder, true );
}

//============================================================================
void MySndMgr::mutePhoneRing( bool bMute )
{
	m_bMutePhoneRing = bMute;
	if( bMute )
	{
		slotStopPhoneRinging();
	}
}

//============================================================================
void MySndMgr::muteNotifySound( bool bMute )
{
	m_bMuteNotifySnd = bMute;
}

//============================================================================
bool MySndMgr::sndMgrStartup( void )
{
    for( int i = 0; i < eMaxSndDef; i++ )
    {
        VxSndInstance * sndInstance = new VxSndInstance( (ESndDef)i, this );
        connect( sndInstance, SIGNAL(sndFinished(VxSndInstance *)), this, SLOT(slotSndFinished(VxSndInstance *)) );
        m_SndList.push_back( sndInstance );
    }

	m_MyApp.wantToGuiHardwareCtrlCallbacks( this, true );
	return true;
}


//============================================================================
bool MySndMgr::sndMgrShutdown( void )
{
	m_MyApp.wantToGuiHardwareCtrlCallbacks( this, false );
    destroyAudioIoSystem();
	return true;
}

//============================================================================
void MySndMgr::doGuiWantMicrophoneRecording( bool wantMicInput )
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
void MySndMgr::doGuiWantSpeakerOutput( bool wantSpeakerOutput )
{
	enableSpeakerOutput( wantSpeakerOutput );
}

//============================================================================
VxSndInstance * MySndMgr::playSnd( ESndDef sndDef, bool loopContinuous  )
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
void MySndMgr::stopSnd( ESndDef sndDef )
{
	if( m_CurSndPlaying 
		&& ( sndDef == m_CurSndPlaying->getSndDef() ) )
	{
		m_CurSndPlaying->stopPlay();
		m_CurSndPlaying = 0;
	}
}

//============================================================================
void MySndMgr::slotSndFinished( VxSndInstance * sndInstance )
{
	if( m_CurSndPlaying == sndInstance )
	{
		m_CurSndPlaying = 0;
	}
}

//============================================================================
//! called when microphone recoding is needed
void MySndMgr::startMicrophoneRecording()
{
	enableMicrophoneInput( true );
}

//============================================================================
//! called when microphone recoding not needed
void MySndMgr::stopMicrophoneRecording()
{
	enableMicrophoneInput( false );
}

//============================================================================
void MySndMgr::enableMicrophoneInput( bool enable )
{
	if( enable != m_bMicrophoneEnabled )
	{
		m_bMicrophoneEnabled = enable;
		LogMsg( LOG_INFO, "Microphone Enable %d\n", enable );
	}
}

//============================================================================
void MySndMgr::enableSpeakerOutput( bool bEnable )
{
	if( bEnable != m_bVoiceOutputEnabled )
	{
		m_bVoiceOutputEnabled = bEnable;
	}
}
