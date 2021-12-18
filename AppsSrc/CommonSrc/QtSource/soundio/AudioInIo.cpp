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
// http://www.nolimitconnect.com
//============================================================================


#include "AudioInIo.h"
#include "AudioIoMgr.h"

#include <CoreLib/VxDebug.h>

#include <QDebug>
#include <QtEndian>
#include <math.h>

//============================================================================
AudioInIo::AudioInIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent )
: QIODevice( parent )
, m_AudioIoMgr( mgr )
, m_AudioBufMutex( audioOutMutex )
, m_AudioInThread( mgr, *this )
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
, m_MediaDevices( new QMediaDevices( this ) )
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
{
    memset( m_MicSilence, 0, sizeof( m_MicSilence ) );
    connect( this, SIGNAL( signalCheckForBufferUnderun() ), this, SLOT( slotCheckForBufferUnderun() ) );
}

//============================================================================
AudioInIo::~AudioInIo()
{
    m_AudioInThread.setThreadShouldRun( false );
    m_AudioInThread.stopAudioInThread();
}

//============================================================================
bool AudioInIo::initAudioIn( QAudioFormat& audioFormat )
{
    if( !m_initialized )
    {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        m_AudioFormat = audioFormat;
        setDivideSamplesCount( 1 );
        bool inputSupported = false;
        const QAudioDevice& defaultDeviceInfo = m_MediaDevices->defaultAudioInput();
        inputSupported = defaultDeviceInfo.isFormatSupported( audioFormat );

        if( !inputSupported )
        {
            // it seems Qt no longer always supports sample rate 8000.. try 48000
            audioFormat.setSampleRate(48000);
            inputSupported = defaultDeviceInfo.isFormatSupported( audioFormat );
            if( inputSupported )
            {
                setDivideSamplesCount( 8 );
                m_AudioFormat = audioFormat;
            }
        }

        if( inputSupported )
        {
            m_AudioInputDevice = new QAudioSource( defaultDeviceInfo, m_AudioFormat, this );
            m_initialized = m_AudioInputDevice != nullptr;
            if( m_initialized )
            {
                // Set constant values to new audio input
                connect( m_AudioInputDevice, SIGNAL( notify() ), SLOT( slotAudioNotified() ) );
                connect( m_AudioInputDevice, SIGNAL( stateChanged( QAudio::State ) ), SLOT( onAudioDeviceStateChanged( QAudio::State ) ) );
                m_AudioInThread.setThreadShouldRun( true );
                m_AudioInThread.startAudioInThread();
            }
        }
        else
        {
            LogMsg( LOG_DEBUG, "AudioInIo Format not supported rate %d size %d", audioFormat.sampleRate(), audioFormat.bytesPerSample() );
        }
#else
        m_AudioFormat = audioFormat;
        m_initialized = setAudioDevice(QAudioDeviceInfo::defaultInputDevice());
        m_AudioInThread.setThreadShouldRun(true);
        m_AudioInThread.startAudioInThread();
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    }

    return m_initialized;
}
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
//============================================================================
bool AudioInIo::setAudioDevice( QAudioDeviceInfo deviceInfo )
{
    if( !deviceInfo.isFormatSupported( m_AudioFormat ) ) 
	{
        qDebug() << "Format not supported!";
        return false;
    }

    m_deviceInfo = deviceInfo;
    delete m_AudioInputDevice;
    m_AudioInputDevice = new QAudioInput( m_deviceInfo, m_AudioFormat, this );

    // Set constant values to new audio output
    connect( m_AudioInputDevice, SIGNAL( notify() ), SLOT( slotAudioNotified() ) );
    connect( m_AudioInputDevice, SIGNAL( stateChanged( QAudio::State ) ), SLOT( onAudioDeviceStateChanged( QAudio::State ) ) );

    return true;
}
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
void AudioInIo::reinit()
{
    if (m_initialized)
    {
        this->stopAudio();
        this->startAudio();
    }
}

//============================================================================
void AudioInIo::startAudio()
{
    if( m_initialized )
    {
        m_AudioInThread.setThreadShouldRun(true);
        m_AudioInThread.startAudioInThread();

        this->open(QIODevice::WriteOnly);
        m_AudioInputDevice->start(this);
    }

    //LogMsg( LOG_DEBUG, "AudioInIo default buffer size %d periodic size %d", m_AudioInputDevice->bufferSize(), m_AudioInputDevice->periodSize() );
}

//============================================================================
void AudioInIo::setVolume( float volume )
{
    m_volume = volume;
}

//============================================================================
void AudioInIo::flush()
{
    if (m_initialized)
    {
        // Flushing buffers is a bit tricky...
        // Don't modify this unless you're sure
        this->stopAudio();

        if (m_AudioInputDevice)
        {
            m_AudioInputDevice->reset();
        }

        this->startAudio();
    }
}

//============================================================================
void AudioInIo::stopAudio()
{
    if (m_initialized)
    {
        m_AudioInThread.setThreadShouldRun(false);
        m_AudioInThread.stopAudioInThread();

        if (m_AudioInputDevice && m_AudioInputDevice->state() != QAudio::StoppedState)
        {
            // Stop audio output
            m_AudioInputDevice->stop();
            this->close();
        }
    }
}

////============================================================================
//static void apply_s16le_volume( float volume, uchar *data, int datalen )
//{
//    int samples = datalen / 2;
//    float mult = pow( 10.0, 0.05 * volume );

//    for( int i = 0; i < samples; i++ ) {
//        qint16 val = qFromLittleEndian<qint16>( data + i * 2 )*mult;
//        qToLittleEndian<qint16>( val, data + i * 2 );
//    }
//}

//============================================================================
qint64 AudioInIo::readData( char *data, qint64 maxlen )
{
    // not used
    Q_UNUSED( data );

    return maxlen;
}

//============================================================================
qint64 AudioInIo::writeData( const char * data, qint64 len )
{
    m_AudioBufMutex.lock();
    m_AudioBuffer.append( data, len );
    updateAtomicBufferSize();
    m_AudioBufMutex.unlock();
    m_AudioInThread.releaseAudioInThread();

    return len;
}

//============================================================================
/// best guess at delay time
int AudioInIo::calculateMicrophonDelayMs()
{
    return (int)( ( getAtomicBufferSize() * ( BYTES_TO_MS_MULTIPLIER_MICROPHONE / m_DivideCnt )  ) + m_AudioIoMgr.toGuiGetAudioDelayMs( eAppModulePtoP ) );
}

//============================================================================
/// space available to que audio data into buffer
int AudioInIo::audioQueFreeSpace()
{
	int freeSpace = AUDIO_OUT_CACHE_USABLE_SIZE - m_AtomicBufferSize;
	if( freeSpace < 0 )
	{
		freeSpace = 0;
	}

	return freeSpace;
}

//============================================================================
/// space used in audio que buffer
int AudioInIo::audioQueUsedSpace()
{
	emit signalCheckForBufferUnderun();

	return m_AudioBuffer.size();
}

//============================================================================
qint64 AudioInIo::bytesAvailable() const
{
	//m_AudioBufMutex.lock();
	qint64 audioBytesAvailableToRead = m_AudioBuffer.size() + QIODevice::bytesAvailable();
	//m_AudioBufMutex.unlock();

    return audioBytesAvailableToRead;
}

//============================================================================
void AudioInIo::slotAudioNotified()
{
}

//============================================================================
void AudioInIo::onAudioDeviceStateChanged( QAudio::State state )
{
    if( m_AudioInState != state )
    {
        LogMsg( LOG_DEBUG, "Audio In state change from %s to %s ", m_AudioIoMgr.describeAudioState( m_AudioInState ), m_AudioIoMgr.describeAudioState( state ) );
        m_AudioInState = state;
    }
}

//============================================================================
// resume qt audio if needed
void AudioInIo::slotCheckForBufferUnderun()
{
	int bufferedAudioData = m_AtomicBufferSize;

    if( bufferedAudioData && m_AudioInputDevice )
	{
        QAudio::State audioState = m_AudioInputDevice->state();
		//qWarning() << "checkForBufferUnderun audioState = " << audioState;
        QAudio::Error audioError = m_AudioInputDevice->error();
		//qWarning() << "checkForBufferUnderun audioError = " << audioError;
//		qWarning() << "checkForBufferUnderun bufferSize = " << m_AudioInputDevice->bufferSize();
		//qWarning() << "checkForBufferUnderun bytesAvail = " << bufferedAudioData;

		switch( audioState )
		{
		case QAudio::ActiveState:
			break;
		case QAudio::IdleState:
			if( audioError == QAudio::UnderrunError )
			{
                LogMsg( LOG_DEBUG, "microphone suspending due to underrun" );
 //               m_AudioInputDevice->suspend();
			}
			else if( bufferedAudioData )
			{
                LogMsg( LOG_DEBUG, "microphone starting due to idle and have data" );
//				this->startAudio();
                //m_AudioInputDevice->start( this );
			}
			else
			{
				// already stopped
                //m_AudioInputDevice->stop();
			}
			break;

		case QAudio::SuspendedState:
			if( bufferedAudioData )
			{ 
                LogMsg( LOG_DEBUG, "microphone restarting due to suspended and have data" );
 //               m_AudioInputDevice->start( this );
			}
			break;

		case QAudio::StoppedState: 
			if( bufferedAudioData )
			{
                LogMsg( LOG_DEBUG, "microphone starting due to stopped and have data" );
 //               m_AudioInputDevice->start( this );
			}
			break;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
		case QAudio::InterruptedState:
            LogMsg( LOG_DEBUG, "Interrupted state.. how to handle?" );
			break;
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
        default:
            LogMsg(LOG_DEBUG, "Unknown AudioIn State");
            break;
		};
	}
}
