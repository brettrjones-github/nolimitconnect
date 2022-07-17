//============================================================================
// Copyright (C) 2015 Brett R. Jones
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


#include "VxSndInstance.h"
#include "SoundMgr.h"
#include "VxResourceToRealFile.h"

#include <CoreLib/VxDebug.h>

#include <QTimer>

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include <QSoundEffect>
#else
#include <QSound>
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)

namespace
{

	const char * g_SndResourcePaths[] = {
			":/AppRes/Resources/snd_joshua_no.wav",			// eSndDefNone, 0
			":/AppRes/Resources/snd_joshua_no.wav",			// eSndDefIgnore,1
			":/AppRes/Resources/snd_cancel1.wav",			// eSndDefCancel,2
			":/AppRes/Resources/snd_alarm2.wav",			// eSndDefAlarmPleasant,3
			":/AppRes/Resources/snd_alarmclock.wav",		// eSndDefAlarmAnoying,4
			":/AppRes/Resources/snd_keyclick2.wav",			// eSndDefButtonClick,5
			":/AppRes/Resources/snd_choice1.wav",			// eSndDefChoice1,6
			":/AppRes/Resources/snd_choice2.wav",			// eSndDefChoice2,7
			":/AppRes/Resources/snd_morse_code.wav",		// eSndDefSending,8
			":/AppRes/Resources/snd_notify1.wav",			// eSndDefNotify1,9
			":/AppRes/Resources/snd_notify2.wav",			// eSndDefNotify2,10
			":/AppRes/Resources/snd_papershredder1.wav",	// eSndDefPaperShredder,11
			":/AppRes/Resources/snd_phone_ring1.wav",		// eSndDefPhoneRing1,12
			":/AppRes/Resources/snd_stomp_hall.wav",		// eSndDefLibrary,13
			":/AppRes/Resources/snd_ouhouh.wav",			// eSndDefShare,14
			":/AppRes/Resources/snd_byebye.wav",			// eSndDefByeBye,15
			":/AppRes/Resources/snd_beep_spring.wav",		// eSndDefMessageArrived,16
			":/AppRes/Resources/snd_microwave_beep.wav",	// eSndDefOfferAccepted,17
			":/AppRes/Resources/snd_joshua_no.wav",			// eSndDefOfferRejected,18
			":/AppRes/Resources/snd_camera_snapshot.wav",	// eSndDefCameraClick,19
			":/AppRes/Resources/snd_busy_phone_signal.wav",	// eSndDefBusy,20
			":/AppRes/Resources/snd_sonar.wav",				// eSndDefOfferStillWaiting,21
			":/AppRes/Resources/snd_file_xfer_complete.wav",	// eSndDefFileXferComplete,22
			":/AppRes/Resources/snd_bike_bell.wav",			// eSndDefUserBellMessage,23
			":/AppRes/Resources/snd_neck_snap.wav",			// eSndDefNeckSnap,24
			":/AppRes/Resources/snd_yes.wav",				// eSndDefYes,25
			":/AppRes/Resources/snd_byebye.wav",			// eSndDefAppShutdown,26
	};
};

//============================================================================
VxSndInstance::VxSndInstance( ESndDef sndDef, QObject * parent )
: QObject( parent )
, m_SndDef( sndDef )
, m_QSound( 0 )
, m_CheckFinishTimer( new QTimer( this ) )
, m_IsPlaying( false )
, m_IsInitialized( false )
{
    setObjectName( "VxSndInstance" );
	connect( m_CheckFinishTimer, SIGNAL( timeout() ), this, SLOT(slotCheckForFinish()) );
}

//============================================================================
VxSndInstance::~VxSndInstance()
{
	stopPlay();
}

//============================================================================
void VxSndInstance::startPlay( bool loopContinuous )
{
	if( ( eSndDefNone >= m_SndDef )
		|| ( eMaxSndDef <= m_SndDef ) )
	{
		// invalid or no sound
		emit sndFinished( this );
		return;
	}

	if( !m_IsInitialized )
	{
		m_IsInitialized = true;
		initSndInstance();
	}

	if( !m_IsPlaying )
	{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		m_QSound->setLoopCount(loopContinuous ? QSoundEffect::Infinite : 1);
#else
		m_QSound->setLoops(loopContinuous ? -1 : 1);
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		
		m_QSound->play();
		m_CheckFinishTimer->start( 100 );
		m_IsPlaying = true;
	}
}

//============================================================================
void VxSndInstance::stopPlay( void )
{
	if( m_IsPlaying )
	{
		if( m_QSound )
		{
			m_QSound->stop();
		}

		slotCheckForFinish();
	}
}

//============================================================================
void VxSndInstance::slotCheckForFinish( void )
{
	if( m_IsPlaying )
	{
		if( 0 == m_QSound )
		{
			m_CheckFinishTimer->stop();
			emit sndFinished( this );
			m_IsPlaying = false;
			return;
		}
		
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		if (!m_QSound->isPlaying())
#elif QT_VERSION >= QT_VERSION_CHECK(5,0,0)
		if( m_QSound->isFinished() )
#else
		if( m_QSound->isAvailable()  )
#endif // QT_5_OR_GREATER
		{
			m_CheckFinishTimer->stop();
			emit sndFinished( this );
			m_IsPlaying = false;
			return;
		}
	}
}

//============================================================================
void VxSndInstance::initSndInstance( void )
{
	QString resourceFile = g_SndResourcePaths[ m_SndDef ];
	VxResourceToRealFile realFile( resourceFile, this );
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
	m_QSound = new QSoundEffect(this);
	m_QSound->setSource(QUrl::fromLocalFile(realFile.fileName()));
#else
	m_QSound = new QSound( realFile.fileName(), this );
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}
