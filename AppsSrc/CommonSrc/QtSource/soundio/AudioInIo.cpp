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

#include "AudioInIo.h"
#include "AudioIoMgr.h"
#include "AudioMixer.h"
#include "AudioUtils.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <QDebug>
#include <QtEndian>
#include <QMessageBox>
#include <math.h>

//============================================================================
AudioInIo::AudioInIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent )
: QIODevice( parent )
, m_AudioIoMgr( mgr )
, m_Engine( mgr.getEngine() )
, m_AudioBufMutex( audioOutMutex )
, m_AudioInThread( mgr, *this )
, m_MediaDevices( new QMediaDevices( this ) )
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
bool AudioInIo::initAudioIn( QAudioFormat& audioFormat, const QAudioDevice& defaultDeviceInfo )
{
    if( !m_initialized )
    {
        m_AudioFormat = audioFormat;
        setDivideSamplesCount( audioFormat.sampleRate() * audioFormat.channelCount() / 8000 );
        int deviceIndex = 0;
        m_AudioIoMgr.getSoundInDeviceIndex( deviceIndex );
        m_initialized = soundInDeviceChanged( deviceIndex );
    }

    return m_initialized;
}

//============================================================================
bool AudioInIo::soundInDeviceChanged( int deviceIndex )
{
    if( !m_AudioInputDevice.isNull() )
    {
        m_AudioInputDevice->disconnect( this );
        stopAudio();
    }

    std::vector< std::pair<QString, QAudioDevice> > inDeviceList;
    m_AudioIoMgr.getSoundInDevices( inDeviceList );
    if( inDeviceList.empty() )
    {
        QMessageBox::information( nullptr, QObject::tr( "Sound In Device" ), QObject::tr( "No Sound Input Devices Avalable" ), QMessageBox::Ok );
        return false;
    }

    if( deviceIndex >= inDeviceList.size() )
    {
        QMessageBox::information( nullptr, QObject::tr( "Sound In Device" ), QObject::tr( "Sound Input Device Index Out Of Range. Will Use Default Device" ), QMessageBox::Ok );
        deviceIndex = 0;
    }

    QString deviceDesc = inDeviceList[ deviceIndex ].first;
    const QAudioDevice& deviceInfo = inDeviceList[ deviceIndex ].second;

    QAudioFormat format = deviceInfo.preferredFormat();

    format.setSampleRate( m_AudioFormat.sampleRate() );
    format.setChannelCount( m_AudioFormat.channelCount() );
    format.setSampleFormat( QAudioFormat::Int16 ); // only pcm is allowed

    if( !deviceInfo.isFormatSupported( format ) )
    {
        LogMsg( LOG_DEBUG, "AudioInIo Format not supported rate %d channels %d size %d", format.sampleRate(), format.channelCount(), format.bytesPerSample() );
        QMessageBox::information( nullptr, QObject::tr( "Sound In Format Not supported" ), QObject::tr( "Audio In Format Not supported" ), QMessageBox::Ok );
        return false;
    }

    m_AudioInputDevice.reset( new QAudioSource( deviceInfo, format, this ) );
    m_AudioInputDevice->setBufferSize( format.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16 );
    m_initialized = true;

    connect( m_AudioInputDevice.data(), SIGNAL( stateChanged( QAudio::State ) ), SLOT( onAudioDeviceStateChanged( QAudio::State ) ) );
    LogMsg( LOG_VERBOSE, "AudioInIo Format supported rate %d channels %d size %d", format.sampleRate(), format.channelCount(), format.bytesPerSample() );

    m_AudioBuffer.clear();
    updateAtomicBufferSize();

    this->open( QIODevice::WriteOnly );

    if( m_AudioIoMgr.isMicrophoneInputWanted() )
    {
        startAudio();
        wantMicrophoneInput( true );
    }

    return true;
}

//============================================================================
void AudioInIo::startAudio()
{
    if( m_initialized )
    {
        m_AudioInThread.setThreadShouldRun(true);
        m_AudioInThread.startAudioInThread();

        m_AudioInputDevice->start(this);
        m_AudioInputDevice->setBufferSize( m_AudioFormat.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16 );
    }
}

//============================================================================
void AudioInIo::setMicrophoneVolume( int volume0to100 )
{
    qreal linearVolume = QAudio::convertVolume( volume0to100 / qreal( 100 ),
        QAudio::LogarithmicVolumeScale,
        QAudio::LinearVolumeScale );

    m_AudioInputDevice->setVolume( linearVolume );
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
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return 0;
    }

    /*
    m_AudioBufMutex.lock();
    m_AudioBuffer.append( data, len );
    updateAtomicBufferSize();
    m_AudioBufMutex.unlock();

    m_AudioInThread.releaseAudioInThread();
    */
    if( m_AudioIoMgr.fromGuiIsMicrophoneMuted() )
    {
        m_PeakAmplitude = 0;
    }
    else
    {
        m_PeakAmplitude = AudioUtils::getPeakPcmAmplitude0to100( (int16_t*)data, len );
    }
    
    m_Engine.getMediaProcesser().fromGuiMicrophoneSamples( (int16_t*)data, (int)len >> 1, m_PeakAmplitude, m_DivideCnt, m_AudioIoMgr.getAudioOutMixer().calcualateAudioOutDelayMs() );
    return len;
}

//============================================================================
/// best guess at delay time
int AudioInIo::calculateMicrophonDelayMs()
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return 0;
    }

    return (int)( AudioUtils::audioDurationUs( m_AudioFormat, getAtomicBufferSize() ) / 1000 + m_AudioIoMgr.toGuiGetAudioCacheTotalMs() );
}

//============================================================================
/// space available to que audio data into buffer
int AudioInIo::audioQueFreeSpace()
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return 0;
    }

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
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return 0;
    }

	//m_AudioBufMutex.lock();
	qint64 audioBytesAvailableToRead = m_AudioBuffer.size() + QIODevice::bytesAvailable();
	//m_AudioBufMutex.unlock();

    return audioBytesAvailableToRead;
}

//============================================================================
void AudioInIo::onAudioDeviceStateChanged( QAudio::State state )
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return;
    }

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
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anythig while being destroyed
        return;
    }

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

        default:
            LogMsg(LOG_DEBUG, "Unknown AudioIn State");
            break;
		};
	}
}

//============================================================================
void AudioInIo::wantMicrophoneInput( bool enableInput )
{
    m_AudioInputDevice->stop();
    if( !m_AudioInDeviceIsStarted )
    {
        m_AudioInDeviceIsStarted = true;
    }

    if( enableInput )
    {
        if( m_AudioInputDevice->state() == QAudio::SuspendedState || m_AudioInputDevice->state() == QAudio::StoppedState )
        {
            m_AudioInputDevice->resume();
        }
        else if( m_AudioInputDevice->state() == QAudio::ActiveState )
        {
            // already enabled
        }
        else if( m_AudioInputDevice->state() == QAudio::IdleState )
        {
            // no-op
        }

        // start in pull mode.. qt will call writeData as needed for microphone input
        m_AudioInputDevice->start( this );
        m_AudioInThread.setThreadShouldRun( true );
        m_AudioInThread.startAudioInThread();
    }
    else
    {
        if( m_AudioInputDevice->state() == QAudio::SuspendedState || m_AudioInputDevice->state() == QAudio::StoppedState )
        {
            // already stopped
        }
        else if( m_AudioInputDevice->state() == QAudio::ActiveState )
        {
            m_AudioInputDevice->suspend();
        }
        else if( m_AudioInputDevice->state() == QAudio::IdleState )
        {
            // no-op
        }
	}
}
