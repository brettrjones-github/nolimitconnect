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

#include "Appcommon.h"
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
, m_MyApp( mgr.getMyApp() )
, m_Engine( mgr.getEngine() )
, m_AudioBufMutex( audioOutMutex )
, m_MediaDevices( new QMediaDevices( this ) )
{
    memset( m_MicSilence, 0, sizeof( m_MicSilence ) );
    connect( this, SIGNAL( signalCheckForBufferUnderun() ), this, SLOT( slotCheckForBufferUnderun() ) );
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
        // m_AudioInThread.setThreadShouldRun(true);
        // m_AudioInThread.startAudioInThread();

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
        // do not attempt anything while being destroyed
        return 0;
    }

    if( !m_AudioIoMgr.isAudioInitialized() )
    {
        return len;
    }

    if( !len )
    {
        LogMsg( LOG_ERROR, "AudioInIo::writeData has 0 maxlen. Probably doing too much cpu processing in AudioInIo::writeData" );
        return len;
    }

    // Qt no longer supports 8000 hz on all devices so everthing in is 48000 hz pcm data
    // ptop engine uses 8000 hz only

    // NOTE: so far Qt has used microphone write lengths evenly divisible down to 8000Hz. If this changes will need to add remainder handling

    int inSampleCnt = (int)len >> 1;
    int16_t* sampleInData = (int16_t*)data;

    int outSampleCnt = inSampleCnt / m_DivideCnt;

    static int16_t* sampleOutData = nullptr;
    static qint64 lastMicWriteLen = 0;
    if( len != lastMicWriteLen )
    {
        // first time or device changed or read buffer len changed
        // unfortunately Qt sometimes changes the microphone write len so also need to handle that
        m_MicWriteDurationUs = (int)AudioUtils::audioDurationUs( m_AudioFormat, len );
        m_MicWriteTimeEstimator.setIntervalUs( m_MicWriteDurationUs );
        if( 0 != lastMicWriteLen )
        {
            LogMsg( LOG_VERBOSE, "AudioInIo::writeData len changed from %d to %d ", lastMicWriteLen, len );
        }

        lastMicWriteLen = len;

        delete[] sampleOutData;
        sampleOutData = new int16_t[ (inSampleCnt / getDivideSamplesCount()) + 1 ]; // plus one for remainder sample if needed

        m_AudioIoMgr.setEchoCancelerNeedsReset( true ); // tell echo canceler parameters have changed and need to restart
    }

    int64_t timeNow = m_MyApp.elapsedMilliseconds();
    int64_t timeStart = timeNow;
    bool timeIntervalTooLong;
    int64_t micWriteTime = m_MicWriteTimeEstimator.estimateTime( timeNow, &timeIntervalTooLong );
    if( std::abs( timeNow - micWriteTime ) > 40 )
    {
        LogMsg( LOG_VERBOSE, "AudioInIo::writeData time estimate excessive time dif %d too long ? %d", (int)std::abs( timeNow - micWriteTime ), timeIntervalTooLong );
    }

    if( m_AudioTestState != eAudioTestStateNone )
    {
        if( m_AudioTestState == eAudioTestStateRun && !m_AudioTestDetectTimeMs )
        {
            audioTestDetectTestSound( sampleInData, inSampleCnt, micWriteTime );
        }

        return len;
    }

    AudioUtils::dnsamplePcmAudio( sampleInData, outSampleCnt, m_DivideCnt, sampleOutData );
    if( m_AudioIoMgr.getAudioTimingEnable() )
    {
        int elapsedInFunction = (m_MyApp.elapsedMilliseconds() - timeNow);
        if( elapsedInFunction > 2 )
        {
            LogMsg( LOG_DEBUG, " AudioInIo::writeData WARNING spent %d ms in dnsamplePcmAudio", elapsedInFunction );
        }

        timeNow = m_MyApp.elapsedMilliseconds();
    }

    if( m_AudioIoMgr.getAudioTimingEnable() )
    {
        int elapsedInFunction = (m_MyApp.elapsedMilliseconds() - timeNow);
        if( elapsedInFunction > 2 )
        {
            LogMsg( LOG_DEBUG, " AudioInIo::writeData WARNING spent %d ms in getPeakPcmAmplitude0to100", elapsedInFunction );
        }

        timeNow = m_MyApp.elapsedMilliseconds();
    }

    int64_t micTailTimeMs = micWriteTime + (m_MicWriteDurationUs / 1000);
    if( m_AudioIoMgr.fromGuiIsEchoCancelEnabled() )
    {
        m_AudioIoMgr.getAudioEchoCancel().micWriteSamples( sampleOutData, outSampleCnt, micTailTimeMs );
    }
    else
    {
        LogMsg( LOG_ERROR, " AudioInIo::writeData ERROR must use echo canceler.. direct write of samples not implemented" );

        //if( m_AudioIoMgr.getAudioLoopbackEnable() )
        //{
        //    m_AudioIoMgr.getAudioLoopback().fromGuiMicrophoneSamples( sampleOutData, outSampleCnt, micWriteTime );
        //}
        //else
        //{
        //    m_Engine.getMediaProcessor().fromGuiMicrophoneSamples( sampleOutData, outSampleCnt, micWriteTime );
        //}
    }

    bool micIsMuted = m_AudioIoMgr.getIsMicrophoneMuted();
    if( micIsMuted )
    {
        m_PeakAmplitude = 0;
    }
    else
    {
        m_PeakAmplitude = AudioUtils::getPeakPcmAmplitude0to100( sampleOutData, outSampleCnt * 2 );
    }
  
    int64_t timeEnd = m_MyApp.elapsedMilliseconds();
    int elapsedInWriteDataFunctionMs = (timeEnd - timeStart);
    if( elapsedInWriteDataFunctionMs > 2 )
    {
        LogMsg( LOG_DEBUG, " AudioInIo::writeData WARNING elapsed time in function %d ms", elapsedInWriteDataFunctionMs );
    }

    //if( m_AudioIoMgr.getAudioTimingEnable() )
    //{
    //    static int64_t lastMicWriteTime{ 0 };
    //    static int funcCallCnt{ 0 };
    //    funcCallCnt++;
    //    if( lastMicWriteTime )
    //    {
    //        int64_t timeInterval = micWriteTime - lastMicWriteTime;
    //        LogMsg( LOG_VERBOSE, "AudioInIo::writeData %d elapsed %d ms app %d ms", funcCallCnt, (int)timeInterval, (int)GetApplicationAliveMs() );
    //    }

    //    lastMicWriteTime = micWriteTime;
    //}

    return len;
}

//============================================================================
qint64 AudioInIo::bytesAvailable() const
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anything while being destroyed
        return 0;
    }

    return m_AudioFormat.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16;
}

//============================================================================
void AudioInIo::onAudioDeviceStateChanged( QAudio::State state )
{
    if( VxIsAppShuttingDown() )
    {
        // do not attempt anything while being destroyed
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
}

//============================================================================
void AudioInIo::wantMicrophoneInput( bool enableInput )
{
    m_MicInputEnabled = enableInput;
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

//============================================================================
void AudioInIo::setAudioTestState( EAudioTestState audioTestState )
{
    switch( audioTestState )
    {
    case eAudioTestStateInit:
        m_AudioTestDetectTimeMs = 0;
        break;
    case eAudioTestStateRun:
        break;
    case eAudioTestStateResult:
        break;
    case  eAudioTestStateDone:
        break;
    case eAudioTestStateNone:
    default:
        break;
    }

    m_AudioTestState = audioTestState;
}

//============================================================================
void AudioInIo::audioTestDetectTestSound( int16_t* sampleInData, int inSampleCnt, int64_t& micWriteTime )
{
    if( m_AudioTestDetectTimeMs )
    {
        return;
    }

    // hopefully there is not too much noise but we also need to detect at low mic input so use 1/4 max volume
    int16_t sampPosVal = 32767 / 4;
    int16_t sampNegVal = -32768 / 4;
    int sampStartIdx = 0;
    int sampPosCnt = 0;
    int sampNegCnt = 0;

    for( int i = 0; i < inSampleCnt; i++ )
    {
        if( sampleInData[ i ] > sampPosVal )
        {
            sampPosCnt++;
            if( !sampStartIdx && sampNegCnt )
            {
                sampStartIdx = i;
            }
        }

        if( sampleInData[ i ] < sampNegVal )
        {
            sampNegCnt++;
            if( !sampStartIdx && sampPosCnt )
            {
                sampStartIdx = i;
            }
        }
    }

    if( sampPosCnt > 2 && sampNegCnt > 2 )
    {
        m_AudioTestDetectTimeMs = micWriteTime + AudioUtils::audioDurationMs( m_AudioFormat, sampStartIdx * 2 );
    }
}
