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

#include "AppCommon.h"
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

    m_MicInBitrate.setLogMessagePrefix( "Mic in " );
    m_MicOutBitrate.setLogMessagePrefix( "Mic out " );

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

    if( deviceIndex >= (int)inDeviceList.size() )
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
        m_AudioInputDevice->start(this);
        m_AudioInputDevice->setBufferSize( m_AudioFormat.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16 );
    }
}

//============================================================================
void AudioInIo::stopAudio()
{
    if( m_initialized )
    {
        if( m_AudioInputDevice && m_AudioInputDevice->state() != QAudio::StoppedState )
        {
            // Stop audio output
            m_AudioInputDevice->stop();
            this->close();
        }
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
qint64 AudioInIo::writeData( const char* data, qint64 len )
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

    static int64_t lastTime = 0;
    int64_t timeNow = GetHighResolutionTimeMs();
    int timeElapsed = lastTime ? (int)(timeNow - lastTime) : 0;
    lastTime = timeNow;

    int inSampleCnt = (int)len / 2;
    int16_t* sampleInData = (int16_t*)data;

    if( timeElapsed )
    {
        m_MicInBitrate.addSamplesAndInterval( inSampleCnt, timeElapsed );
    }

    int outSampleCnt = inSampleCnt / m_DivideCnt;
    if( inSampleCnt % m_DivideCnt || m_DivideCnt != 6)
    {
        LogMsg( LOG_VERBOSE, "AudioInIo::writeData samples have a downsample remainder %d of sample cnt %d ", inSampleCnt % m_DivideCnt, inSampleCnt );
    }

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

    int64_t micWriteTime = m_MicWriteTimeEstimator.estimateTime( timeNow );

    if( m_AudioTestState != eAudioTestStateNone )
    {
        m_AudioIoMgr.getAudioMasterClock().audioMicWriteDurationTime( m_MicWriteDurationUs / 1000, micWriteTime );
        if( m_AudioIoMgr.getSampleCntDebugEnable() )
        {
            m_AudioIoMgr.getAudioMasterClock().audioMicWriteSampleCnt( inSampleCnt );
        }

        if( m_AudioTestState == eAudioTestStateRun )
        {
            audioTestDetectTestSound( sampleInData, inSampleCnt, micWriteTime );
        }

        return len;
    }

    AudioUtils::dnsamplePcmAudio( sampleInData, outSampleCnt, m_DivideCnt, sampleOutData );
    if( timeElapsed )
    {
        m_MicOutBitrate.addSamplesAndInterval( outSampleCnt, timeElapsed );
    }

    int64_t micTailTimeMs = micWriteTime + (m_MicWriteDurationUs / 1000);
    if( m_AudioIoMgr.getIsEchoCancelEnabled() )
    {
        m_AudioIoMgr.getAudioEchoCancel().micWroteSamples( sampleOutData, outSampleCnt, micTailTimeMs, m_MicWriteTimeEstimator.getHasMaxTimestamps() );
    }
    else
    {
        LogMsg( LOG_ERROR, " AudioInIo::writeData ERROR must use echo canceler.. direct write of samples not implemented" );
    }

    bool micIsMuted = m_AudioIoMgr.getIsMicrophoneMuted();
    if( micIsMuted )
    {
        m_PeakAmplitude = 0;
    }
    else
    {
        m_PeakAmplitude = AudioUtils::peakPcmAmplitude0to100( sampleOutData, outSampleCnt );
    }

    m_AudioIoMgr.getAudioMasterClock().audioMicWriteDurationTime( m_MicWriteDurationUs / 1000, micWriteTime );
    if( m_AudioIoMgr.getSampleCntDebugEnable() )
    {
        m_AudioIoMgr.getAudioMasterClock().audioMicWriteSampleCnt( inSampleCnt );
    }

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
    if( !m_AudioInDeviceIsStarted && enableInput )
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
        startAudioIn();
    }
    else
    {
        if( m_AudioInputDevice->state() == QAudio::SuspendedState || m_AudioInputDevice->state() == QAudio::StoppedState )
        {
            // already stopped
        }
        else if( m_AudioInputDevice->state() == QAudio::ActiveState )
        {
            stopAudioIn();
        }
        else if( m_AudioInputDevice->state() == QAudio::IdleState )
        {
            // no-op
        }
	}
}

//============================================================================
void AudioInIo::startAudioIn( void )
{
    // start in pull mode.. qt will call readData as needed for sound output
    m_AudioInputDevice->start( this );
    // unlike speaker we always use fixed buffer size so the microphone writes are faster
    m_AudioInputDevice->setBufferSize( m_AudioFormat.channelCount() == 1 ? AUDIO_BUF_SIZE_48000_1_S16 : AUDIO_BUF_SIZE_48000_2_S16 );

    m_AudioInDeviceIsStarted = true;
    m_AudioIoMgr.microphoneDeviceEnabled( true );
}

//============================================================================
void AudioInIo::stopAudioIn( void )
{
    m_AudioInputDevice->suspend();
    m_AudioInDeviceIsStarted = false;
    m_AudioIoMgr.microphoneDeviceEnabled( false );
}

//============================================================================
void AudioInIo::setAudioTestState( EAudioTestState audioTestState )
{
    switch( audioTestState )
    {
    case eAudioTestStateInit:
        m_DelayTestDetectList.clear();
        break;
    case eAudioTestStateRun:
        m_DelayTestDetectList.clear();
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
    // find peak value and time
    int samplePosVal = 0;
    int64_t sampleTimeMs = 0;

    int16_t sampCompareValue = 32768 / 10;

    for( int i = 0; i < inSampleCnt; i++ )
    {
        if( sampleInData[ i ] > samplePosVal && sampleInData[ i ] > sampCompareValue )
        {
            samplePosVal = sampleInData[ i ];
            sampleTimeMs = micWriteTime + AudioUtils::audioDurationMs( m_AudioFormat, i * 2 );
        }
    }

    if( samplePosVal && sampleTimeMs )
    {
        m_DelayTestDetectList.push_back( std::make_pair( sampleTimeMs, samplePosVal) );
    }
}

//============================================================================
int64_t AudioInIo::getAudioTestDetectTime( int& peakValue )
{
    int64_t detectTime = 0;
    peakValue = 0;

    for( auto& pair : m_DelayTestDetectList )
    {
        if( pair.second > peakValue )
        {
            peakValue = pair.second;
            detectTime = pair.first;
        }
    }

    return detectTime;
}

//============================================================================
void AudioInIo::echoCancelSyncStateThreaded( bool inSync )
{
    m_EchoCancelInSync = inSync;
    // mic write 48000 is eratic timing.. commented out to not clutter the log
    // m_MicInBitrate.setIsBitrateLogEnabled( inSync && m_AudioIoMgr.getBitrateDebugEnable() );
    m_MicOutBitrate.setIsBitrateLogEnabled( inSync && m_AudioIoMgr.getBitrateDebugEnable() );
}
