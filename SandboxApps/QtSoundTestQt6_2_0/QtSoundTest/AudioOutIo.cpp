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
, m_AudioOutThread( mgr )
{
    m_PeriodicTimer->setInterval( 20 );
    connect( this, SIGNAL( signalCheckForBufferUnderun() ), this, SLOT( slotCheckForBufferUnderun() ) );
    connect( m_PeriodicTimer, SIGNAL( timeout() ), this, SLOT( slotAudioNotify() ) );
    connect( &m_AudioIoMgr.getAudioOutMixer(), SIGNAL( signalCheckSpeakerOutState() ), this, SLOT( slotCheckForBufferUnderun() ) );
    connect( &m_AudioIoMgr.getAudioOutMixer(), SIGNAL( signalAvailableSpeakerBytesChanged( int ) ), this, SLOT( slotAvailableSpeakerBytesChanged( int ) ) );

}

//============================================================================
AudioOutIo::~AudioOutIo()
{
    m_AudioOutThread.setThreadShouldRun( false );
    m_AudioOutThread.stopAudioOutThread();
}

//============================================================================
bool AudioOutIo::initAudioOut( QAudioFormat& audioFormat )
{
    if( !m_initialized )
    {
        m_AudioFormat = audioFormat;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        m_AudioOutputDevice = new QAudioSink(m_AudioFormat, this);
        m_initialized = m_AudioOutputDevice != nullptr;
        // qt seems to ignore setBufferSize but try for smaller faster calls anyway
        m_AudioOutputDevice->setBufferSize(AUDIO_BUF_SIZE_48000_2_S16 * 2); // must be called before start
        m_AudioOutBufferSize = m_AudioOutputDevice->bufferSize();
        connect(m_AudioOutputDevice, SIGNAL(stateChanged(QAudio::State)), SLOT(onAudioDeviceStateChanged(QAudio::State)));
#else
        m_initialized = setAudioDevice( QAudioDeviceInfo::defaultOutputDevice() );
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    }

    m_AudioOutThread.setThreadShouldRun( true );
    m_AudioOutThread.startAudioOutThread();
    return m_initialized;
}
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
bool AudioOutIo::setAudioDevice( QAudioDeviceInfo deviceInfo )
{
    if( !deviceInfo.isFormatSupported( m_AudioFormat ) ) 
	{
        qDebug() << "Format not supported!";
        return false;
    }

    m_deviceInfo = deviceInfo;
    delete m_AudioOutputDevice;
    m_AudioOutputDevice = new QAudioOutput( m_deviceInfo, m_AudioFormat, this );
    // qt seems to ignore setBufferSize but try for smaller faster calls anyway
    m_AudioOutputDevice->setBufferSize( AUDIO_BUF_SIZE_48000_2_S16 * 2 ); // must be called before start
    m_AudioOutBufferSize = m_AudioOutputDevice->bufferSize();


    connect( m_AudioOutputDevice, SIGNAL( stateChanged( QAudio::State ) ), SLOT( onAudioDeviceStateChanged( QAudio::State ) ) );

//    this->reinit();
    return true;
}
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
void AudioOutIo::reinit()
{
    this->stopAudio();
    this->startAudio();
}

//============================================================================
void AudioOutIo::startAudio()
{
	// Reinitialize audio output
    m_ProccessedMs = 0;

    m_AudioOutThread.setThreadShouldRun( true );
    m_AudioOutThread.startAudioOutThread();

    this->open( QIODevice::ReadOnly );
    m_AudioOutputDevice->start( this );

    m_PeriodicTimer->start();
    m_ElapsedTimer.start();
    //LogMsg( LOG_DEBUG, "AudioOutIo default buffer size %d periodic size %d", m_AudioOutputDevice->bufferSize(), m_AudioOutputDevice->periodSize() );
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
    this->stopAudio();
	if( m_AudioOutputDevice )
	{
		m_AudioOutputDevice->reset();
	}

    this->startAudio();
}

//============================================================================
void AudioOutIo::stopAudio()
{
    //m_PeriodicTimer->stop();
    if( m_AudioOutputDevice && m_AudioOutputDevice->state() != QAudio::StoppedState )
    {
        // Stop audio output
		m_AudioOutputDevice->stop();
        //this->close();
    }

    m_AudioOutThread.setThreadShouldRun( false );
    m_AudioOutThread.stopAudioOutThread();
}

////============================================================================
//static void apply_s16le_volume( float volume, uchar *data, int datalen )
//{
//    int samples = datalen / 2;
//    float mult = pow( 10.0, 0.05*volume );

//    for( int i = 0; i < samples; i++ ) {
//        qint16 val = qFromLittleEndian<qint16>( data + i * 2 )*mult;
//        qToLittleEndian<qint16>( val, data + i * 2 );
//    }
//}

//============================================================================
qint64 AudioOutIo::readData( char *data, qint64 maxlen )
{
    if( !m_AudioIoMgr.isAudioInitialized() )
    {
        return 0;
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

    if( secondsNow != secondsLast )
    {
        secondsLast = secondsNow;
        if( secondsNow != secondsStart )
        {
            qint64 byteRate = totalRead / ( secondsNow - secondsStart );
            LogMsg( LOG_DEBUG, "speaker out byte rate = %lld at %lld sec delay %3.3f ms", byteRate, secondsNow - secondsStart, m_AudioIoMgr.getDataReadyForSpeakersLen() * BYTES_TO_MS_MULTIPLIER_SPEAKERS );
        }
    }

    if( maxlen <= 0 )
    {
        LogMsg( LOG_DEBUG, "readData with maxlen 0 with avail %d", m_AudioIoMgr.getDataReadyForSpeakersLen() );
        emit signalCheckForBufferUnderun();
        return maxlen;
    }

    // do not try suspend in readData because in android the state becomes active but readData is never called again

    // you would think you could just read what bytes are availible but no. It is all or nothing else you will get missing sound
    //int dataReadyLen = std::min( (int)maxlen, m_AudioIoMgr.getDataReadyForSpeakersLen() );
    //if( dataReadyLen )
    //{

    //    m_AudioOutState = m_AudioOutputDevice->state();
    //    if(  ( m_AudioOutState != QAudio::SuspendedState ) && ( m_AudioOutState != QAudio::StoppedState ) )
    //    {
    //        LogMsg( LOG_DEBUG, "readData underrun because avail %d required %lld", m_AudioIoMgr.getDataReadyForSpeakersLen(), maxlen );
    //        // sigh if suspend here then in android the state becomes active but readData is never called again
    //        //m_AudioOutputDevice->suspend();
    //    }
    //    // temp
    //    memset( data, 0, maxlen );
    //    return maxlen;
    //    //return 0;
    //}
    int toReadCnt = std::min( bytesAvailable(), maxlen );
    qint64 readAmount = m_AudioIoMgr.getAudioOutMixer().readDataFromMixer( data, toReadCnt );

    return readAmount;
}

//============================================================================
// not used
qint64 AudioOutIo::writeData( const char *data, qint64 len )
{
    Q_UNUSED( data );
    Q_UNUSED( len );

    return 0;
}

//============================================================================
qint64 AudioOutIo::size() const
{
    return bytesAvailable();
}

//============================================================================
qint64 AudioOutIo::bytesAvailable() const
{
    qint64 avail = std::min( m_AudioIoMgr.getAudioOutMixer().getDataReadyForSpeakersLen(), m_AudioOutBufferSize );
    //avail += QIODevice::bytesAvailable();
    //LogMsg( LOG_DEBUG, "bytesAvailable %lld device avail %lld", avail, QIODevice::bytesAvailable() );
    return avail;
}

//============================================================================
void AudioOutIo::slotAvailableSpeakerBytesChanged( int availBytes )
{
    Q_UNUSED( availBytes );
    //seek( availBytes );
}

//============================================================================
void AudioOutIo::slotAudioNotify()
{
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
}

//============================================================================
void AudioOutIo::onAudioDeviceStateChanged( QAudio::State state )
{
    if( m_AudioOutState != state )
    {
        LogMsg( LOG_DEBUG, "onAudioDeviceStateChanged  from %s to %s ", m_AudioIoMgr.describeAudioState( m_AudioOutState ), m_AudioIoMgr.describeAudioState( state ) );
        m_AudioOutState = state;
    }

#if defined(TARGET_OS_WINDOWS)
    if( m_AudioOutputDevice )
	{
		// Start buffering again in case of underrun...
		// Required on Windows, otherwise it stalls idle
		if( state == QAudio::IdleState && m_AudioOutputDevice->error() == QAudio::UnderrunError )
		{
			// This check is required, because Mac OS X underruns often
			m_AudioOutputDevice->suspend();
//#ifdef DEBUG_QT_AUDIO
            LogMsg( LOG_DEBUG, "onAudioDeviceStateChanged speaker out suspending due to underrun " );
//#endif // DEBUG_QT_AUDIO
			return;
		}      
	}
#endif
}

//============================================================================
// resume qt audio if needed
void AudioOutIo::slotCheckForBufferUnderun()
{
    int bufferedAudioData = m_AudioIoMgr.getDataReadyForSpeakersLen();
    
	if( m_AudioOutputDevice )
	{
		m_AudioOutState = m_AudioOutputDevice->state();
		//qWarning() << "checkForBufferUnderun audioState = " << audioState;
		QAudio::Error audioError = m_AudioOutputDevice->error();
        if( audioError != QAudio::NoError )
        {
            LogMsg( LOG_DEBUG, "speaker Error %s", m_AudioIoMgr.describeAudioError( audioError ) );
        }
		//qWarning() << "checkForBufferUnderun audioError = " << audioError;
//		qWarning() << "checkForBufferUnderun bufferSize = " << m_AudioOutputDevice->bufferSize();
		//qWarning() << "checkForBufferUnderun bytesAvail = " << bufferedAudioData;

		switch( m_AudioOutState )
		{
		case QAudio::ActiveState:
			break;
		case QAudio::IdleState:
            if( !bufferedAudioData && audioError == QAudio::UnderrunError )
			{
                LogMsg( LOG_DEBUG, "speaker idle due to underrun" );
#if defined( TARGET_OS_ANDROID )
                m_AudioOutputDevice->stop();
#elif defined( TARGET_OS_WINDOWS )
                m_AudioOutputDevice->suspend();
#endif // TARGET_OS_WINDOWS
			}
            else if( bufferedAudioData >= m_AudioOutputDevice->bufferSize() )
			{
#if defined( TARGET_OS_ANDROID )
                m_AudioOutputDevice->stop();
                m_AudioOutputDevice->start( this );
#elif defined( TARGET_OS_WINDOWS )
                LogMsg( LOG_DEBUG, "speaker resuming due to idle and have data" );
#ifdef Q_OS_WIN
                // The Windows backend seems to internally go back into ActiveState
                // while still returning SuspendedState, so to ensure that it doesn't
                // ignore the resume() call, we first re-suspend
                m_AudioOutputDevice->suspend();
#endif
                m_AudioOutputDevice->resume();
#endif // TARGET_OS_WINDOWS
            }
			else
			{
				// already stopped and have no data
			}
			break;

		case QAudio::SuspendedState:
            if( bufferedAudioData >= m_AudioOutBufferSize )
			{ 
                LogMsg( LOG_DEBUG, "speaker out resuming due to suspended and have data" );
#if defined( TARGET_OS_ANDROID )
                 m_AudioOutputDevice->resume();
#elif defined( TARGET_OS_WINDOWS )
#ifdef Q_OS_WIN
                // The Windows backend seems to internally go back into ActiveState
                // while still returning SuspendedState, so to ensure that it doesn't
                // ignore the resume() call, we first re-suspend
                m_AudioOutputDevice->suspend();
#endif
                m_AudioOutputDevice->resume();
#endif // TARGET_OS_WINDOWS
            }
			break;

		case QAudio::StoppedState: 
            if( bufferedAudioData >= m_AudioOutBufferSize )
			{
                LogMsg( LOG_DEBUG, "restarting due to stopped and have data" );
                m_AudioOutputDevice->start( this );
			}
			break;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
		case QAudio::InterruptedState:
            LogMsg( LOG_DEBUG, "Interrupted state.. how to handle?" );
			break;
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
        default:
            LogMsg(LOG_DEBUG, "Unknown AudioOut State");
            break;
		};
	}
}
