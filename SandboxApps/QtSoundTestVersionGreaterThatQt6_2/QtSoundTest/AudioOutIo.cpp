//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "AudioOutIo.h"
#include "AudioIoMgr.h"

#include "VxDebug.h"

#include <QDebug>
#include <QtEndian>
#include <QTimer>
#include <math.h>

//============================================================================
AudioOutIo::AudioOutIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent )
: QIODevice( parent )
, m_AudioIoMgr( mgr )
, m_AudioBufMutex( audioOutMutex )
, m_PeriodicTimer( new QTimer( this ) )
, m_AudioOutState( QAudio::StoppedState )
, m_ResumeTimer( new QTimer( this ) )
{
    m_ResumeTimer->setInterval( 5 );
    m_PeriodicTimer->setInterval( 20 );

    connect( this, SIGNAL( signalCheckForBufferUnderun() ), this, SLOT( slotCheckForBufferUnderun() ) );
    connect( m_ResumeTimer, SIGNAL( timeout() ), this, SLOT( slotSendResumeStatus() ) );
    connect( m_PeriodicTimer, SIGNAL( timeout() ), this, SLOT( slotAudioNotify() ) );
    connect( &m_AudioIoMgr.getAudioOutMixer(), SIGNAL( signalCheckSpeakerOutState() ), this, SLOT( slotCheckForBufferUnderun() ) );
}

//============================================================================
AudioOutIo::~AudioOutIo()
{
}

//============================================================================
bool AudioOutIo::initAudioOut( QAudioFormat& audioFormat, const QAudioDevice& defaultDeviceInfo )
{
    if( !m_initialized )
    {
        m_AudioFormat = audioFormat;
        m_AudioOutputDevice = new QAudioSink( m_AudioFormat, this);
        m_AudioOutputDevice->setBufferSize( AUDIO_BUF_SIZE_48000_1_S16 );

        #ifdef Q_OS_WIN
        // hate to hard code this but qt/windows wants a buffer that is not a multiple of 6 (48000 / 8000)
        // which causes many issues.. the minimum qt will accept in windows is 16384 which is not evenly divisible by 6

        // very stange request 16384 and you get 16384 but request  16390 and you get 16384 so request 16390 but it seems to be reverted to 16384 after a few callse
        m_AudioOutputDevice->setBufferSize( 16390 );
        #endif // Q_OS_WIN

        m_AudioOutputDevice->start();
        int syncBufferSize = m_AudioOutputDevice->bufferSize();
        if( syncBufferSize != AUDIO_BUF_SIZE_48000_1_S16 * 2 )
        {
            LogMsg( LOG_ERROR, "AudioOutIo::initAudioOut syncBufferSize %d != mixer buffer size %d", syncBufferSize, AUDIO_BUF_SIZE_48000_1_S16 );
        }

        m_AudioOutputDevice->stop();
        m_initialized = m_AudioOutputDevice != nullptr;
        QAudioFormat audioResultFormat = m_AudioOutputDevice->format();
        if( audioResultFormat != m_AudioFormat )
        {
            LogMsg( LOG_ERROR, "AudioOutIo::initAudioOut format not supported" );
            m_AudioFormat = audioResultFormat;
        }



        // determine multiply value to go from 8000Hz mono to the format we want
        setUpsampleMultiplier( audioResultFormat.sampleRate() * audioResultFormat.channelCount() / 8000 );

        this->open( QIODevice::ReadOnly );

        connect(m_AudioOutputDevice, SIGNAL(stateChanged(QAudio::State)), SLOT(onAudioDeviceStateChanged(QAudio::State)));
    }

    return m_initialized;
}

//============================================================================
void AudioOutIo::wantSpeakerOutput( bool enableOutput ) 
{ 
    if( m_AudioOutDeviceIsStarted )
    {
        enableOutput ? resume() : suspend();
    }
    else if( enableOutput )
    {
        startAudioOut();
    }
}

//============================================================================
void AudioOutIo::reinit()
{
    this->stopAudioOut();
    this->startAudioOut();
}

//============================================================================
void AudioOutIo::startAudioOut()
{
	// Reinitialize audio output
    m_AudioOutDeviceIsStarted = true;
    m_ProccessedMs = 0;

    m_PeriodicTimer->start();
    m_ElapsedTimer.start();

    m_AudioOutputDevice->start( this );  

    //LogMsg( LOG_DEBUG, "AudioOutIo default buffer size %d periodic size %d", m_AudioOutputDevice->bufferSize(), m_AudioOutputDevice->periodSize() );
}

//============================================================================
void AudioOutIo::stopAudioOut()
{
    m_PeriodicTimer->stop();

    if( m_AudioOutDeviceIsStarted )
    {
        m_AudioOutDeviceIsStarted = false;
        //m_PeriodicTimer->stop();
        if( m_AudioOutputDevice && m_AudioOutputDevice->state() != QAudio::StoppedState )
        {
            // Stop audio output
            m_AudioOutputDevice->stop();
            //this->close();
        }
    }
}

//============================================================================
void AudioOutIo::fromGuiCheckSpeakerOutForUnderrun()
{
    emit signalCheckForBufferUnderun();
}

//============================================================================
void AudioOutIo::setVolume( float volume )
{
    m_volume = volume;
}

//============================================================================
void AudioOutIo::flush()
{
    // Flushing buffers is a bit tricky...
    // Don't modify this unless you're sure
    this->stopAudioOut();
	if( m_AudioOutputDevice )
	{
		m_AudioOutputDevice->reset();
	}

    this->startAudioOut();
}

//============================================================================
qint64 AudioOutIo::readData( char *data, qint64 maxlen )
{
    if( !m_AudioIoMgr.isAudioInitialized() )
    {
        memset( data, 0, maxlen );
        return maxlen;
    }

    static qint64 totalRead = 0;
    static qint64 secondsStart = 0;
    static qint64 secondsLast = 0;
    totalRead += maxlen;
    int64_t secondsNow = time( NULL );
    if( 0 == secondsStart )
    {
        secondsStart = secondsNow;
    }

    /*
    if( secondsNow != secondsLast )
    {
        secondsLast = secondsNow;
        if( secondsNow != secondsStart )
        {
            qint64 byteRate = totalRead / ( secondsNow - secondsStart );
            LogMsg( LOG_DEBUG, "speaker out byte rate = %lld at %lld sec delay %d ms peak %d", byteRate, secondsNow - secondsStart, m_AudioIoMgr.getDataReadyForSpeakersMs(), m_AudioIoMgr.getAudioOutPeakAmplitude() );
            totalRead = 0;
        }
    }

    if( maxlen <= 0 )
    {
        // do not try suspend in readData because in android the state becomes active but readData is never called again
        LogMsg( LOG_DEBUG, "readData with maxlen %d with avail %d", maxlen, m_AudioIoMgr.getDataReadyForSpeakersLen() );
        emit signalCheckForBufferUnderun();
        return maxlen;
    }
    */

    qint64 readAmount = m_AudioIoMgr.getAudioOutMixer().readDataFromMixer( data, maxlen, getUpsampleMultiplier() );
    //totalRead += readAmount;
    //return readAmount;
    if( readAmount != maxlen )
    {
        LogMsg( LOG_DEBUG, "readData mismatch with maxlen %d and read %d", maxlen, readAmount );
    }

    return maxlen;
}

//============================================================================
// not used
qint64 AudioOutIo::writeData( const char *data, qint64 len )
{
    Q_UNUSED( data );
    Q_UNUSED( len );

    return 0;
}

/*
//============================================================================
qint64 AudioOutIo::size() const
{
    return bytesAvailable();
}

//============================================================================
qint64 AudioOutIo::bytesAvailable() const
{
    return m_AudioIoMgr.getAudioOutMixer().getMixerFrameSize();
}
*/

//============================================================================
void AudioOutIo::slotAudioNotify()
{
    /*
    qint64 processedMs = m_AudioOutputDevice->elapsedUSecs() / 1000;
    static int callCnt = 0;
    static uint64_t seconds = 0;
    static uint64_t lastSecond = 0;
    if( ( processedMs - m_ProccessedMs ) >= AUDIO_MS_SPEAKERS )
    {
        // round down to nearest 80 ms
        m_ProccessedMs = processedMs - ( processedMs % AUDIO_MS_SPEAKERS );
        m_AudioOutThread.releaseAudioOutThread();
//        LogMsg( LOG_DEBUG, "AudioOutIo Notify %3.3f ms processed %lld interval %d buf size %d elapsed %d", m_NotifyTimer.elapsedMs(), processedMs, m_AudioOutputDevice->notifyInterval(), 
//            m_AudioOutputDevice->bufferSize(), m_ElapsedTimer.elapsed() );

        callCnt++;
        uint64_t secondsNow = time( NULL );
        if( secondsNow != lastSecond )
        {
            lastSecond = secondsNow;
            if( !seconds )
            {
                m_ElapsedTimer.restart();
                seconds = secondsNow;
            }

            uint64_t elapsedSec = secondsNow - seconds;
            if( elapsedSec )
            {
                //LogMsg( LOG_DEBUG, "* %d calls per second %d-%lld-%lld", callCnt / elapsedSec, elapsedSec, m_ElapsedTimer.elapsed() / 1000, processedMs / 1000 );
            }
        }
    }
    */
}

//============================================================================
void AudioOutIo::onAudioDeviceStateChanged( QAudio::State state )
{
    if( m_AudioOutState != state )
    {
        LogMsg( LOG_DEBUG, "AudioOutIo::onAudioDeviceStateChanged  from %s to %s ", m_AudioIoMgr.describeAudioState( m_AudioOutState ), m_AudioIoMgr.describeAudioState( state ) );
        m_AudioOutState = state;
    }

//#if defined(TARGET_OS_WINDOWS)
    if( m_AudioOutputDevice )
	{
		// Start buffering again in case of underrun...
		// Required on Windows, otherwise it stalls idle
		if( state == QAudio::IdleState && m_AudioOutputDevice->error() == QAudio::UnderrunError )
		{
			// This check is required, because Mac OS X underruns often
			// m_AudioOutputDevice->suspend();
//#ifdef DEBUG_QT_AUDIO
            LogMsg( LOG_DEBUG, "AudioOutIo::onAudioDeviceStateChanged speaker out suspending due to underrun " );
//#endif // DEBUG_QT_AUDIO
			return;
        }
	}
//#endif
}

//============================================================================
// resume qt audio if needed
void AudioOutIo::slotCheckForBufferUnderun()
{
    /*
    int bufferedAudioData = m_AudioIoMgr.getDataReadyForSpeakersLen();
    
	if( m_AudioOutputDevice )
	{
        m_AudioOutPreviousState = m_AudioOutState;
		m_AudioOutState = m_AudioOutputDevice->state();
		//qWarning() << "checkForBufferUnderun audioState = " << audioState;
		QAudio::Error audioError = m_AudioOutputDevice->error();
        if( audioError != QAudio::NoError )
        {
            LogMsg( LOG_DEBUG, "AudioOutIo::slotCheckForBufferUnderun speaker Error %s", m_AudioIoMgr.describeAudioError( audioError ) );
        }
		//qWarning() << "checkForBufferUnderun audioError = " << audioError;
//		qWarning() << "checkForBufferUnderun bufferSize = " << m_AudioOutputDevice->bufferSize();
		//qWarning() << "checkForBufferUnderun bytesAvail = " << bufferedAudioData;

		switch( m_AudioOutState )
		{
		case QAudio::ActiveState:
            if( QAudio::UnderrunError == m_AudioOutPreviousError )
            {
                 m_AudioOutPreviousError = QAudio::NoError;
                 m_ResumeTimer->start();
            }
			break;
		case QAudio::IdleState:
            if( !bufferedAudioData && audioError == QAudio::UnderrunError )
			{
                LogMsg( LOG_DEBUG, "AudioOutIo::slotCheckForBufferUnderun speaker idle due to underrun" );
                m_AudioOutputDevice->suspend();
                m_ResumeTimer->start();
			}
            else if( bufferedAudioData >= m_AudioOutputDevice->bufferSize() )
            {
                LogMsg( LOG_DEBUG, "AudioOutIo::slotCheckForBufferUnderun speaker resuming due to idle and have data" );
                m_ResumeTimer->start();
#if TARGET_OS_WINDOWS
                m_AudioOutputDevice->suspend();
                m_AudioOutputDevice->resume();
#else
                m_AudioOutputDevice->start();
#endif // TARGET_OS_WINDOWS
            }
			else
			{
				// already stopped and have no data
			}
			break;

		case QAudio::SuspendedState:
            if( bufferedAudioData >= AUDIO_BUF_SIZE_8000_1_S16 )
			{ 
                LogMsg( LOG_DEBUG, "AudioOutIo::slotCheckForBufferUnderun speaker out resuming due to suspended and have data" );
                m_AudioOutputDevice->resume();
                m_ResumeTimer->start();
            }
            
			break;

		case QAudio::StoppedState: 
            if( bufferedAudioData >= m_AudioOutBufferSize )
			{
                LogMsg( LOG_DEBUG, "AudioOutIo::slotCheckForBufferUnderun restarting due to stopped and have data" );
                m_AudioOutputDevice->start( this );
                m_ResumeTimer->start();
			}
			break;

        default:
            LogMsg(LOG_DEBUG, "AudioOutIo::slotCheckForBufferUnderun Unknown AudioOut State");
            break;
		};

        if( QAudio::NoError != audioError )
        {
            m_AudioOutPreviousError = audioError;
        }
	}
    */
}

//============================================================================
// send resume callback after qt has processed events
void AudioOutIo::slotSendResumeStatus( void )
{
    m_ResumeTimer->stop();
    m_AudioIoMgr.fromAudioOutResumed();
}
