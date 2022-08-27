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
#include "AudioUtils.h"
#include "AppCommon.h"
#include "AppSettings.h"

#include <QSurface>
#include <qmath.h>
#include <QTimer>

#include <CoreLib/VxDebug.h>

namespace
{
    //============================================================================
    static void* AudioOutProcessThreadFunc( void* pvContext )
    {
        VxThread* poThread = (VxThread*)pvContext;
        poThread->setIsThreadRunning( true );
        AudioIoMgr* processor = (AudioIoMgr*)poThread->getThreadUserParam();
        if( processor )
        {
            processor->processAudioOutThreaded();
        }

        poThread->threadAboutToExit();
        return nullptr;
    }
}


//============================================================================
AudioIoMgr::AudioIoMgr( AppCommon& app, IAudioCallbacks& audioCallbacks, QWidget* parent )
    : QWidget( parent )
    , m_MyApp( app )
    , m_MediaDevices( new QMediaDevices( this ) )
    , m_AudioCallbacks( audioCallbacks )
    , m_AudioOutMixer( *this, audioCallbacks, this )
    , m_AudioOutIo( *this, m_AudioOutMutex, this )
    , m_AudioInIo( *this, m_AudioInMutex, this )
    , m_AudioEchoCancel( app, *this, this )
    , m_AudioTestTimer( new QTimer( this ) )
    , m_AudioMasterClock( *this, this )
    , m_AudioLoopback( *this )
{
    m_AudioTestTimer->setInterval( 600 );
    connect( m_AudioTestTimer, SIGNAL(timeout()), this, SLOT(slotAudioTestTimer()) );

    memset( m_MyLastAudioOutSample, 0, sizeof( m_MyLastAudioOutSample ) );
    memset( m_MicrophoneEnable, 0, sizeof( m_MicrophoneEnable ) );
    memset( m_SpeakerEnable, 0, sizeof( m_SpeakerEnable ) );

    m_SpeakerAvailable = true;
    m_AudioOutFormat.setSampleRate( 48000 );
    m_AudioOutFormat.setChannelCount( 2 );
    m_AudioOutFormat.setSampleFormat(QAudioFormat::Int16);

    const QAudioDevice& defaultOutDeviceInfo = m_MediaDevices->defaultAudioOutput();
    if( !defaultOutDeviceInfo.isFormatSupported( m_AudioOutFormat ) )
    {
         LogMsg( LOG_DEBUG, "Default audio out format not supported " );
         m_SpeakerAvailable = false;
         LogMsg( LOG_DEBUG, "No Speakers available" );
    }

    m_MicrophoneAvailable = true;
    // Qt no longer supports 8000 hz. seems to support 48000 hz on all platforms
    m_AudioInFormat.setSampleRate( 48000 );
    m_AudioInFormat.setChannelCount( 1 );
    m_AudioInFormat.setSampleFormat( QAudioFormat::Int16 );

    const QAudioDevice& defaultInDeviceInfo = m_MediaDevices->defaultAudioInput();
    if( !defaultInDeviceInfo.isFormatSupported( m_AudioInFormat ) )
    {
        m_MicrophoneAvailable = false;
        LogMsg( LOG_DEBUG, "No Microphone available" );
    }

    // BRJ temp for testing
    setAudioLoopbackEnable( true );
    // setAudioTimingEnable( true );
    setFrameTimingEnable( true );
}

//============================================================================
P2PEngine& AudioIoMgr::getEngine( void )
{
    return m_MyApp.getEngine();
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
        enableMicrophone( enableMic );
    }
}

//============================================================================
 // update microphone output
void AudioIoMgr::enableMicrophone( bool enable )
{
    m_AudioInIo.wantMicrophoneInput( enable );
}

//============================================================================
// enable disable sound out
void AudioIoMgr::toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput )
{
    m_SpeakerEnable[ appModule ] = wantSpeakerOutput;
    bool shouldEnable = false;
    for( int i = 0; i < eMaxAppModule; i++ )
    {
        if( m_SpeakerEnable[ appModule ] )
        {
            shouldEnable = true;
            break;
        }
    }

    if( shouldEnable != m_WantSpeakerOutput )
    {
        m_WantSpeakerOutput = shouldEnable;
        enableSpeakers( shouldEnable );
    }
}

//============================================================================
// enable disable sound out
void AudioIoMgr::enableSpeakers( bool enable )
{
    m_AudioOutMixer.wantSpeakerOutput( enable );
    m_AudioOutIo.wantSpeakerOutput( enable );
}

//============================================================================
int AudioIoMgr::fromGuiMicrophoneData( EAppModule appModule, int16_t* pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    // this microphone data is already downsampled if needed to Mono channel 8000 Hz PCM audio
    //if( m_MicrophoneVolume != 100.0f )
    //{
    //    AudioUtils::applyPcmVolume( m_MicrophoneVolume, (uchar*)pu16PcmData, pcmDataLenInBytes );
    //}

    return m_AudioOutMixer.toMixerPcm8000HzMonoChannel( appModule, pu16PcmData, pcmDataLenInBytes, isSilence );
}

#if ENABLE_KODI
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
#endif // ENABLE_KODI

//============================================================================
int AudioIoMgr::toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence )
{
    if( isSilence )
    {
        m_MyLastAudioOutSample[ appModule ] = 0;
        return pcmDataLenInBytes;
    }

    return m_AudioOutMixer.toMixerPcm8000HzMonoChannel( appModule, pu16PcmData, pcmDataLenInBytes, isSilence );
 }

//============================================================================
void AudioIoMgr::audioIoSystemStartup()
{
    if( !m_AudioIoInitialized )
    {
        m_EchoCancelEnabled = m_MyApp.getAppSettings().getEchoCancelEnable();
        m_AudioEchoCancel.enableEchoCancel( m_EchoCancelEnabled );

        m_AudioOutIo.initAudioOut( m_AudioOutFormat, m_MediaDevices->defaultAudioOutput() );

        connect( m_AudioOutIo.getAudioOut(), SIGNAL( stateChanged( QAudio::State ) ), this, SLOT( speakerStateChanged( QAudio::State ) ) );

        if( isMicrophoneAvailable() )
        {
            m_AudioInIo.initAudioIn( m_AudioInFormat, m_MediaDevices->defaultAudioInput() );

            connect( m_AudioInIo.getAudioIn(), SIGNAL( stateChanged( QAudio::State ) ), this, SLOT( microphoneStateChanged( QAudio::State ) ) );
        }

        m_AudioMasterClock.audioMasterClockStartup();
        m_AudioIoInitialized = true;
    }
}

//============================================================================
void AudioIoMgr::audioIoSystemShutdown()
{
    m_AudioLoopback.audioLoopbackShutdown();
    m_AudioMasterClock.audioMasterClockShutdown();
    m_AudioIoInitialized = false;
    stopAudioOut();
    stopAudioIn();
}

//============================================================================
void AudioIoMgr::stopAudioOut()
{
    m_AudioOutIo.stopAudioOut();
}

//============================================================================
void AudioIoMgr::stopAudioIn()
{
    m_AudioInIo.stopAudio();
}

//============================================================================
// volume is from 0.0 to 1.0
void AudioIoMgr::setSpeakerVolume( float volume0to1 )
{
    m_AudioOutIo.setSpeakerVolume( (int)(volume0to1 * 100 ) );
}

//============================================================================
int AudioIoMgr::getMicrophonePeakValue0To100( void )
{
    return m_AudioInIo.getAudioInPeakAmplitude();
}

//============================================================================
double AudioIoMgr::toGuiGetAudioDelayMs( EAppModule appModule )
{
    return (double)m_AudioOutMixer.calcualateMixerBytesToMs( getCachedDataLength( appModule ) );
}

//============================================================================
double AudioIoMgr::toGuiGetAudioDelaySeconds( EAppModule appModule )
{
    return toGuiGetAudioDelayMs( appModule ) * 1000;
}

//============================================================================
double AudioIoMgr::toGuiGetAudioCacheTotalSeconds( EAppModule appModule )
{
    return m_AudioOutMixer.getDataReadyForSpeakersMs() * 1000;
}

//============================================================================
double AudioIoMgr::toGuiGetAudioCacheTotalMs( void )
{
    return m_AudioOutMixer.getDataReadyForSpeakersMs();
}

//============================================================================
int AudioIoMgr::getCachedDataLength( EAppModule appModule )
{
    return m_AudioOutMixer.audioQueUsedSpace( appModule );
}

//============================================================================
int AudioIoMgr::toGuiGetAudioCacheFreeSpace( EAppModule appModule )
{
    int freeSpace = m_AudioOutMixer.audioQueFreeSpace( appModule ) - getCachedDataLength( appModule );
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
int AudioIoMgr::getDataReadyForSpeakersMs()
{
    return m_AudioOutMixer.getDataReadyForSpeakersMs();
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

//============================================================================
void AudioIoMgr::fromMixerAvailablbleMixerSpace( int pcmMixerAvailableSpace )
{
}
    
//============================================================================
int AudioIoMgr::getAudioInPeakAmplitude( void )
{
    return m_AudioInIo.getAudioInPeakAmplitude();
}

//============================================================================
int AudioIoMgr::getAudioOutPeakAmplitude( void )
{
    return m_AudioOutMixer.getAudioOutPeakAmplitude();
}

//============================================================================
bool AudioIoMgr::setSoundInDeviceIndex( int sndInDeviceIndex )
{
    bool wasSet = false;

    m_MyApp.getAppSettings().setSoundInDeviceIndex( sndInDeviceIndex );
    return wasSet;
}

//============================================================================
bool AudioIoMgr::getSoundInDeviceIndex( int& retDeviceIndex )
{
    retDeviceIndex = m_MyApp.getAppSettings().getSoundInDeviceIndex();
    return true;
}

//============================================================================
bool AudioIoMgr::setSoundOutDeviceIndex( int sndOutDeviceIndex )
{
    bool wasSet = false;

    m_MyApp.getAppSettings().setSoundOutDeviceIndex( sndOutDeviceIndex );
    return wasSet;
}

//============================================================================
bool AudioIoMgr::getSoundOutDeviceIndex( int& retDeviceIndex )
{
    retDeviceIndex = m_MyApp.getAppSettings().getSoundOutDeviceIndex();
    return true;
}

//============================================================================
void AudioIoMgr::getSoundInDevices( std::vector< std::pair<QString, QAudioDevice> >& retInDeviceList )
{
    retInDeviceList.clear();
    const QAudioDevice& defaultInDeviceInfo = m_MediaDevices->defaultAudioInput();
    retInDeviceList.push_back( std::make_pair( defaultInDeviceInfo.description(), defaultInDeviceInfo ) );
    for( auto& deviceInfo : m_MediaDevices->audioInputs() )
    {
        if( deviceInfo != defaultInDeviceInfo )
        {
            retInDeviceList.push_back( std::make_pair( deviceInfo.description(), deviceInfo ) );
        }
    }
}

//============================================================================
void AudioIoMgr::getSoundOutDevices( std::vector< std::pair<QString, QAudioDevice> >& retOutDeviceList )
{
    retOutDeviceList.clear();
    const QAudioDevice& defaultOutDeviceInfo = m_MediaDevices->defaultAudioOutput();
    retOutDeviceList.push_back( std::make_pair(defaultOutDeviceInfo.description(),  defaultOutDeviceInfo ) );
    for( auto& deviceInfo : m_MediaDevices->audioOutputs() )
    {
        if( deviceInfo != defaultOutDeviceInfo )
        {
            retOutDeviceList.push_back( std::make_pair( deviceInfo.description(),  deviceInfo ) );
        }
    }
}

//============================================================================
void AudioIoMgr::soundInDeviceChanged( int deviceIndex )
{
    m_AudioInIo.soundInDeviceChanged( deviceIndex );
}

//============================================================================
void AudioIoMgr::soundOutDeviceChanged( int deviceIndex )
{
    m_AudioOutIo.soundOutDeviceChanged( deviceIndex );
}

//============================================================================
bool AudioIoMgr::runAudioDelayTest( void )
{
    if( m_AudioTestState != eAudioTestStateNone )
    {
        LogMsg( LOG_ERROR, "AudioIoMgr::runAudioTest already running" );
        return false;
    }

    m_EchoDelayCurrentInteration = 0;
    m_EchoDelayResultList.clear();

    m_AudioTestMicEnable = m_AudioInIo.isMicrophoneInputWanted();
    m_AudioTestSpeakerEnable = m_AudioOutIo.isSpeakerOutputWanted();

    if( !m_AudioTestMicEnable )
    {
        m_AudioInIo.wantMicrophoneInput( true );
    }
    
    if( !m_AudioTestSpeakerEnable )
    {
        m_AudioOutIo.wantSpeakerOutput( true );
    }

    setAudioTestState( eAudioTestStateInit );

    m_AudioTestTimer->start();
    return true;
}

//============================================================================
void AudioIoMgr::slotAudioTestTimer( void )
{
    switch( m_AudioTestState )
    {
    case eAudioTestStateInit:
        // waited for sound to be quiet
        LogMsg( LOG_VERBOSE, "Echo Delay Test Init" );
        setAudioTestState( eAudioTestStateRun );
        break;

    case eAudioTestStateRun:
        // send sound test cherp
        LogMsg( LOG_VERBOSE, "Echo Delay Test Run" );
        setAudioTestState( eAudioTestStateResult );
        break;

    case eAudioTestStateResult:
        // get the delay time result
        LogMsg( LOG_VERBOSE, "Echo Delay Test Result" );
        if( !handleAudioTestResult( m_AudioOutIo.getAudioTestSentTime(), m_AudioInIo.getAudioTestDetectTime() ) )
        {
            LogMsg( LOG_WARNING, "Echo Delay Test Faled" );
            
            setAudioTestState( eAudioTestStateDone );
            break;
        }

        m_EchoDelayCurrentInteration++;
        if( m_EchoDelayCurrentInteration < m_EchoDelayTestMaxInterations )
        {
            // start test again
            LogMsg( LOG_VERBOSE, "Echo Delay Test Run Again" );
            setAudioTestState( eAudioTestStateInit );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "Echo Delay Test Done" );
            setAudioTestState( eAudioTestStateDone );
        }

        break;


    case eAudioTestStateDone:
        LogMsg( LOG_VERBOSE, "Echo Delay Test Restore Mic/Speaker states and finish" );
        m_AudioTestTimer->stop();
        setAudioTestState( eAudioTestStateNone );

        if( !m_AudioTestMicEnable )
        {
            m_AudioInIo.wantMicrophoneInput( m_AudioTestMicEnable );
        }

        if( !m_AudioTestSpeakerEnable )
        {
            m_AudioOutIo.wantSpeakerOutput( m_AudioTestSpeakerEnable );
        }

        break;

    case eAudioTestStateNone:
    default:
        break;
    }
}

//============================================================================
void AudioIoMgr::setAudioTestState( EAudioTestState audioTestState )
{
    m_AudioTestState = audioTestState;
    m_AudioInIo.setAudioTestState( m_AudioTestState );
    m_AudioOutIo.setAudioTestState( m_AudioTestState );
    emit signalAudioTestState( audioTestState );
}

//============================================================================
bool AudioIoMgr::handleAudioTestResult( int64_t soundOutTimeMs, int64_t soundDetectTimeMs )
{
    bool isValid{ false };
    QString resultMsg;
    int64_t timeDif = soundDetectTimeMs - soundOutTimeMs;
    if(!soundOutTimeMs || !soundDetectTimeMs)
    {
        timeDif = 0;
        resultMsg = QObject::tr("Sound Delay Not Detected. Check speaker volume and that microphone is on ");
    }
    else if( timeDif < 50 )
    {
        resultMsg = QObject::tr("Sound Delay too short.. probably noise ");
    }
    else if( timeDif > 500 )
    {
        resultMsg = QObject::tr("Sound Delay too long.. probably mic level low ");
    }
    else
    {
        resultMsg = QObject::tr("Sound Delay is ");
        isValid = true;
    }

    resultMsg += QString::number((int)timeDif);

    LogMsg( LOG_VERBOSE, "AudioIoMgr::handleAudioTestResult %s", resultMsg.toUtf8().constData() );

    m_EchoDelayResultList.push_back( (int)timeDif );
    // to avoid to much sound thread cpu time used Qt::QueuedConnection when connecting to these signals
    emit signalAudioTestMsg( resultMsg );
    emit signalTestedSoundDelay( (int)timeDif );
    return isValid;
}

//============================================================================
void AudioIoMgr::fromGuiEchoCancelEnable( bool enable ) 
{ 
    m_EchoCancelEnabled = enable; 
    m_AudioEchoCancel.enableEchoCancel( m_EchoCancelEnabled );
    m_MyApp.getAppSettings().setEchoCancelEnable( m_EchoCancelEnabled );
}

//============================================================================
void AudioIoMgr::frame80msElapsed( void )
{
    if( getAudioLoopbackEnable() )
    {
        m_AudioLoopback.fromGuiAudioOutSpaceAvail( AUDIO_BUF_SIZE_8000_1_S16 );
    }
    else
    {
        IAudioCallbacks& audioCallbacks = getAudioCallbacks();
        audioCallbacks.fromGuiAudioOutSpaceAvail( AUDIO_BUF_SIZE_8000_1_S16 );
    }

    if( m_EchoCancelEnabled )
    {
        m_AudioEchoCancel.frame80msElapsed();
    }
}

//============================================================================
void AudioIoMgr::processAudioOutThreaded( void )
{
    if( getAudioLoopbackEnable() )
    {
        m_AudioLoopback.fromGuiAudioOutSpaceAvail( AUDIO_BUF_SIZE_8000_1_S16 );
    }
    else
    {
        IAudioCallbacks& audioCallbacks = getAudioCallbacks();
        audioCallbacks.fromGuiAudioOutSpaceAvail( AUDIO_BUF_SIZE_8000_1_S16 );
    }
}

//============================================================================
void AudioIoMgr::setEchoCancelerNeedsReset( bool needReset )
{
    m_AudioEchoCancel.setEchoCancelerNeedsReset( needReset );
}

//============================================================================
bool AudioIoMgr::getFrameTimingEnable( void )
{
    return m_FrameTimingEnabled && m_AudioEchoCancel.getIsInSync();
}