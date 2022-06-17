//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AudioIoMgr.h"

#include <QSurface>
#include <qmath.h>
#include <QTimer>

#include "VxDebug.h"


//============================================================================
AudioIoMgr::AudioIoMgr( IAudioCallbacks& audioCallbacks, QWidget * parent )
: QWidget( parent )
, m_AudioCallbacks( audioCallbacks )
, m_AudioOutMixer( *this, audioCallbacks, this )
, m_AudioOutIo( *this, m_AudioOutMutex, this )
, m_AudioInIo( *this, m_AudioInMutex, this )
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
, m_AudioOutDeviceInfo( QAudioDeviceInfo::defaultOutputDevice() )
, m_AudioInDeviceInfo( QAudioDeviceInfo::defaultInputDevice() )
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
{
    memset( m_MyLastAudioOutSample, 0, sizeof( m_MyLastAudioOutSample ) );
    memset( m_MicrophoneEnable, 0, sizeof( m_MicrophoneEnable ) );
    memset( m_SpeakerEnable, 0, sizeof( m_SpeakerEnable ) );

    m_AudioOutFormat.setSampleRate( 48000 );
    m_AudioOutFormat.setChannelCount( 2 );
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    m_AudioOutFormat.setSampleFormat(QAudioFormat::Int16);
    m_AudioOutFormat.setChannelConfig( QAudioFormat::ChannelConfigMono );
#else
    m_AudioOutFormat.setBytesPerSample(2);
    m_AudioOutFormat.setSampleSize( 16 );
    m_AudioOutFormat.setCodec( "audio/pcm" );
    m_AudioOutFormat.setByteOrder( QAudioFormat::LittleEndian );
    m_AudioOutFormat.setSampleType( QAudioFormat::SignedInt );

    m_SpeakerAvailable = true;
    QAudioDeviceInfo infoOut( m_AudioOutDeviceInfo );
    if( !infoOut.isFormatSupported( m_AudioOutFormat ) )
    {
 //       LogMsg( LOG_DEBUG, "Default audio out format not supported - trying to use nearest" );
 //       m_AudioOutFormat = infoOut.nearestFormat( m_AudioOutFormat );
 //       if( !m_AudioInFormat.isValid() )
        {
            m_SpeakerAvailable = false;
            LogMsg( LOG_DEBUG, "No Speakers available" );
        }
    }
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    m_MicrophoneAvailable = true;

    m_AudioInFormat.setSampleRate( 8000 );
    m_AudioInFormat.setChannelCount( 1 );
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    m_AudioInFormat.setSampleFormat( QAudioFormat::Int16 );
    m_AudioInFormat.setChannelConfig( QAudioFormat::ChannelConfigMono );
#else
    m_AudioInFormat.setSampleSize( 16 );
    m_AudioInFormat.setCodec( QStringLiteral("audio/pcm") );
    m_AudioInFormat.setByteOrder( QAudioFormat::LittleEndian );
    m_AudioInFormat.setSampleType( QAudioFormat::SignedInt );

    QAudioDeviceInfo infoIn( m_AudioInDeviceInfo );
    if( !infoIn.isFormatSupported( m_AudioInFormat ) )
    {
        //LogMsg( LOG_DEBUG, "Default audio in format not supported - trying to use nearest" );
        //m_AudioInFormat = infoIn.nearestFormat( m_AudioInFormat );
        //if( !m_AudioInFormat.isValid() )
        {
            m_MicrophoneAvailable = false;
            LogMsg( LOG_DEBUG, "No Microphone available" );
        }
    }
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
}

//============================================================================
 // update speakers to current mode and output
void AudioIoMgr::updateSpeakers()
{

}

//============================================================================
 // update microphone output
void AudioIoMgr::updateMicrophone()
{
}

//============================================================================
// enable disable microphone data callback
void AudioIoMgr::toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput )
{
    m_MicrophoneEnable[ appModule ] = wantMicInput;
    bool enableMic = false;
    for( int i = 0; i < eMaxAppModule; i++ )
    {
        if( m_MicrophoneEnable[ appModule ] )
        {
            enableMic = true;
            break;
        }
    }

    if( enableMic != m_WantMicrophone )
    {
        m_WantMicrophone = enableMic;
        updateMicrophone();
    }
}

//============================================================================
// enable disable sound out
void AudioIoMgr::toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput )
{
    m_SpeakerEnable[ appModule ] = wantSpeakerOutput;
    bool enableSpeakers = false;
    for( int i = 0; i < eMaxAppModule; i++ )
    {
        if( m_SpeakerEnable[ appModule ] )
        {
            enableSpeakers = true;
            break;
        }
    }

    if( enableSpeakers != m_WantSpeakerOutput )
    {
        m_WantSpeakerOutput = enableSpeakers;
        updateSpeakers();
    }
}

//============================================================================
// add audio data to play.. assumes float 2 channel 48000 Hz so convert float to s16 pcm data before calling writeData
int AudioIoMgr::toGuiPlayAudio( EAppModule appModule, float * audioSamples48000, int dataLenInBytes )
{
    // it seems Qt does not handle anything but pcm 16 bit signed samples correctly so convert to what Qt can handle
    int wroteByteCnt = 0;
    int desiredWriteByteCnt = 0;

    // convert float 48000 2 channel to pcm 16 bit 2 channel
    int sampleCnt = dataLenInBytes / sizeof( float );
    int16_t * outAudioData = new int16_t[ sampleCnt ];
    float * inSampleData = audioSamples48000;

    for( int i = 0; i < sampleCnt; i++ )
    {
        outAudioData[ i ] = (int16_t)( inSampleData[ i ] * 32767.0f );
    }

    //LogMsg( LOG_DEBUG, "to mixer float %3.3f second %3.3f last %3.3f ", inSampleData[ 0 ], inSampleData[ 2 ], outAudioData[ sampleCnt - 1 ] );
    //LogMsg( LOG_DEBUG, "to mixer s16 %d second %d last %d ", outAudioData[ 0 ], outAudioData[ 2 ], outAudioData[ sampleCnt - 1 ] );

    desiredWriteByteCnt = sampleCnt * sizeof( int16_t );
    wroteByteCnt = m_AudioOutMixer.enqueueAudioData( appModule, outAudioData, desiredWriteByteCnt, false );
    if( desiredWriteByteCnt != wroteByteCnt )
    {
        LogMsg( LOG_DEBUG, "toGuiPlayAudio desired write len %d wrote %d ", desiredWriteByteCnt, wroteByteCnt );
    }

    wroteByteCnt *= sizeof( float ) / sizeof( int16_t ); // amount written needs int16 to float size as far as kodi is concerned

    delete[] outAudioData;

    return wroteByteCnt;
}

//============================================================================
// add audio data to play.. assumes pcm mono 8000 Hz so convert to 2 channel 48000 hz pcm before calling writeData
int AudioIoMgr::toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    if( isSilence )
    {
        m_MyLastAudioOutSample[ appModule ] = 0;
        return pcmDataLenInBytes;
    }

    // it seems Qt does not handle anything but pcm 16 bit signed samples correctly so convert to what Qt can handle
    int wroteByteCnt = 0;
 
    // convert 8000hz mono to 48000hz 2 channel
    float upResampleMultiplier = 48000 / 8000;
    upResampleMultiplier *= 2; // 2 channels
    int iDestIdx = 0;
 
    int outBufSize = (int)( pcmDataLenInBytes * upResampleMultiplier );
    unsigned char * outAudioData = new unsigned char[ outBufSize ];
    if( isSilence )
    {
        memset( outAudioData, 0, outBufSize );
        m_MyLastAudioOutSample[ appModule ] = 0;
        wroteByteCnt = m_AudioOutMixer.enqueueAudioData( appModule, (int16_t *)outAudioData, outBufSize, isSilence );
        delete[] outAudioData;
    }
    else
    {
        int mySampleCnt = pcmDataLenInBytes >> 1;
        int16_t * srcSamples = pu16PcmData;
        int16_t * destSamples = (int16_t *)outAudioData;
        int16_t firstSample = m_MyLastAudioOutSample[ appModule ];
        int16_t secondSample = firstSample;
        float sampleStep;
        for( int i = 0; i < mySampleCnt; i++ )
        {
            firstSample = secondSample;
            secondSample = srcSamples[ i ];
            if( secondSample >= firstSample )
            {
                // ramp up
                sampleStep = ( ( secondSample - firstSample ) / upResampleMultiplier );
            }
            else
            {
                // ramp down
                sampleStep = -( ( firstSample - secondSample ) / upResampleMultiplier );
            }

            if( 0 == sampleStep )
            {
                for( int j = 0; j < upResampleMultiplier; ++j )
                {
                    destSamples[ iDestIdx ] = firstSample;
                    iDestIdx++;
                }
            }
            else
            {
                float sampleOffs = sampleStep;
                int resampleCnt = (int)upResampleMultiplier;
                for( int j = 0; j < resampleCnt; ++j )
                {
                    destSamples[ iDestIdx ] = (int16_t)( firstSample + sampleOffs );
                    iDestIdx++;
                    sampleOffs += sampleStep;
                }
            }

            // save the last sample to be used as first sample reference in next frame
            m_MyLastAudioOutSample[ appModule ] = srcSamples[ mySampleCnt - 1 ];
        }

        wroteByteCnt = m_AudioOutMixer.enqueueAudioData( appModule, (int16_t *)outAudioData, outBufSize, isSilence );
        delete[] outAudioData;
    }

    wroteByteCnt = (int)( wroteByteCnt / upResampleMultiplier );
    return wroteByteCnt;
}

//============================================================================
void AudioIoMgr::pauseAudioOut()
{
    if( !m_IsOutPaused )
    {
        if( m_AudioOutIo.getState() == QAudio::SuspendedState )
        {
            //m_AudioDevice->resume();
        }
        else if( m_AudioOutIo.getState() == QAudio::ActiveState )
        {
            m_IsOutPaused = true;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            m_AudioOutIo.suspend();
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
        }
        else if( m_AudioOutIo.getState() == QAudio::StoppedState )
        {
            // m_AudioDevice->resume();
        }
        else if( m_AudioOutIo.getState() == QAudio::IdleState )
        {
            // no-op
        }
    }
}

//============================================================================
void AudioIoMgr::resumeAudioOut()
{
    if( m_IsOutPaused )
    {
        if( m_AudioOutIo.getState() == QAudio::SuspendedState )
        {
            m_IsOutPaused = false;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            m_AudioOutIo.resume();
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
        }
        else if( m_AudioOutIo.getState() == QAudio::ActiveState )
        {
            //m_AudioDevice->suspend();
        }
        else if( m_AudioOutIo.getState() == QAudio::StoppedState )
        {
            m_IsOutPaused = false;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            m_AudioOutIo.resume();
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
        }
        else if( m_AudioOutIo.getState() == QAudio::IdleState )
        {
            // no-op
        }
    }
}

//============================================================================
void AudioIoMgr::initAudioIoSystem()
{

    if( !m_AudioIoInitialized )
    {
        m_AudioOutIo.initAudioOut( m_AudioOutFormat );

        connect( m_AudioOutIo.getAudioOut(), SIGNAL( stateChanged( QAudio::State ) ), this, SLOT( speakerStateChanged( QAudio::State ) ) );
        //connect( m_AudioOutIo.getAudioOut(), SIGNAL( notify() ), SLOT( notifyNeedData() ) );
        //connect( this, SIGNAL( signalNeedMoreAudioData( int ) ), SLOT( slotNeedMoreAudioData( int ) ) );
        m_AudioOutIo.startAudio();

        if( isMicrophoneAvailable() )
        {
            m_AudioInIo.initAudioIn( m_AudioInFormat );

            connect( m_AudioInIo.getAudioIn(), SIGNAL( stateChanged( QAudio::State ) ), this, SLOT( microphoneStateChanged( QAudio::State ) ) );
            //connect( m_AudioInIo.getAudioOut(), SIGNAL( notify() ), SLOT( notifyNeedData() ) );
            //connect( this, SIGNAL( signalNeedMoreAudioData( int ) ), SLOT( slotNeedMoreAudioData( int ) ) );
            m_AudioInIo.startAudio();
        }

        m_AudioIoInitialized = true;
    }
}

//============================================================================
void AudioIoMgr::destroyAudioIoSystem()
{
    m_AudioIoInitialized = false;
    stopAudioOut();
    stopAudioIn();
}

//============================================================================
void AudioIoMgr::stopAudioOut()
{
    m_AudioOutIo.stopAudio();
}

//============================================================================
void AudioIoMgr::stopAudioIn()
{
    m_AudioInIo.stopAudio();
}

//============================================================================
// volume is from 0.0 to 1.0
void AudioIoMgr::setVolume( float volume )
{
    m_AudioOutIo.setVolume( volume );
}

//============================================================================
double AudioIoMgr::toGuiGetAudioDelayMs( EAppModule appModule )
{
    return ( (double)getCachedDataLength( appModule ) * BYTES_TO_MS_MULTIPLIER_SPEAKERS );
}

//============================================================================
double AudioIoMgr::toGuiGetAudioDelaySeconds( EAppModule appModule )
{
    return toGuiGetAudioDelayMs( appModule ) / 1000;
}

//============================================================================
double AudioIoMgr::toGuiGetAudioCacheTotalSeconds( EAppModule appModule )
{
    int bytesInCache = m_AudioOutMixer.audioQueUsedSpace( appModule );

    double sndSeconds = ( (double)bytesInCache / (double)( 48000 * 2 * 2 )); //48000 * 2 bytes per sample * 2 channels
    //LogMsg( LOG_DEBUG, "Max soundBuffer seconds %3.3f", sndSeconds );
    return sndSeconds;
}

//============================================================================
int AudioIoMgr::getCachedDataLength( EAppModule appModule )
{
    int bytesInCache = m_AudioOutMixer.audioQueUsedSpace( appModule );
    
    //LogMsg( LOG_DEBUG, "Bytes in snd buffer %d", bytesInCache );

    return bytesInCache;
}

//============================================================================
int AudioIoMgr::toGuiGetAudioCacheFreeSpace( EAppModule appModule )
{
    int freeSpace =  getCachedMaxLength() - getCachedDataLength( appModule );
    if( freeSpace < 0 ) // can happen because getCachedDataLength is all awaiting read including what is in the device
    {
        freeSpace = 0;
    }

    //LogMsg( LOG_DEBUG, "snd buffer free space %d", freeSpace );

    if( eAppModuleKodi == appModule )
    {
        // for kodi the sound goes through a float to 16 bit pcm conversion so report what kodi wants
        freeSpace *= sizeof( float ) / sizeof( uint16_t );
    }

    return freeSpace;
}

//============================================================================
uint16_t SwapEndian16( uint16_t src )
{
    return ( ( src & 0xFF00 ) >> 8 ) | ( ( src & 0x00FF ) << 8 );
}

//static uint32_t SwapEndian32( uint32_t x ) {
//    return( ( x << 24 ) | ( ( x << 8 ) & 0x00FF0000 ) | ( ( x >> 8 ) & 0x0000FF00 ) | ( x >> 24 ) );
//}

//============================================================================
void AudioIoMgr::speakerStateChanged( QAudio::State newState )
{
    LogMsg( LOG_DEBUG, "Speakers now %s state", describeAudioState( newState ) );
}

//============================================================================
void AudioIoMgr::microphoneStateChanged( QAudio::State newState )
{
    LogMsg( LOG_DEBUG, "Microphone now %s state", describeAudioState( newState ) );
}

//============================================================================
void AudioIoMgr::aboutToDestroy()
{
}

//============================================================================
// get length of data ready for write to speakers
int AudioIoMgr::getDataReadyForSpeakersLen()
{
    return m_AudioOutMixer.getDataReadyForSpeakersLen();
}

//============================================================================
// read speaker output from mixer
qint64 AudioIoMgr::readDataFromOutMixer( char * data, qint64 maxlen )
{
    return m_AudioOutMixer.readDataFromMixer( data, maxlen );
}

//============================================================================
void AudioIoMgr::fromGuiMuteMicrophone( bool mute )      
{ 
    if( m_MicrophoneMuted != mute )
    {
        m_MicrophoneMuted = mute;

    }
}

//============================================================================
void AudioIoMgr::fromGuiMuteSpeaker( bool mute )         
{ 
    if( m_SpeakersMuted != mute )
    {
        m_SpeakersMuted = mute; 
        m_AudioOutMixer.fromGuiMuteSpeaker( mute );
    }
}

//============================================================================
const char * AudioIoMgr::describeAudioState( QAudio::State state )
{
    switch( state )
    {
    case QAudio::State::ActiveState:
        return " ActiveState ";
    case QAudio::State::SuspendedState:
        return " SuspendedState ";
    case QAudio::State::StoppedState:
        return " StoppedState ";
    case QAudio::State::IdleState:
        return " IdleState ";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    case QAudio::State::InterruptedState:
        return " InterruptedState ";
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
    default:
        return " Unknown State ";
    }
}

//============================================================================
const char * AudioIoMgr::describeAudioError( QAudio::Error err )
{
    switch( err )
    {
    case QAudio::Error::NoError:
        return " NoError ";
    case QAudio::Error::OpenError:
        return " OpenError ";
    case QAudio::Error::IOError:
        return " IOError ";
    case QAudio::Error::UnderrunError:
        return " UnderrunError ";
    case QAudio::Error::FatalError:
        return " FatalError ";
    default:
        return " Unknown Error ";
    }
}
