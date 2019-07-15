#include "SoundTestLogic.h"

#include <QSurface>
#include <qmath.h>
#include <QTimer>

#include "VxDebug.h"


//============================================================================
SoundTestLogic::SoundTestLogic( Waveform * waveForm, QWidget *parent )
: m_WaveForm( waveForm )
, m_AudioIoMgr( *this, this )
{
    int displayBufCnt = 17; // 16 * 80ms = 1.280 seconds
    m_WaveForm->initialize( m_AudioIoMgr.getAudioOutFormat(), AUDIO_BUF_SIZE_48000_2_S16, AUDIO_MS_SPEAKERS * displayBufCnt * 1000, AUDIO_BUF_SIZE_48000_2_S16 * displayBufCnt );

    //...do pre-caching etc of the sounds here
    m_AudioGenIn = new AudioTestGenerator( m_AudioIoMgr.getAudioInFormat(), AUDIO_MS_MICROPHONE * 1000, 400, this );

    QAudioFormat kodiSpeakerFormat;
    kodiSpeakerFormat.setSampleRate( 48000 );
    kodiSpeakerFormat.setChannelCount( 2 );
    kodiSpeakerFormat.setSampleSize( sizeof(float) * 8 );
    kodiSpeakerFormat.setCodec( QStringLiteral( "audio/pcm" ) );
    kodiSpeakerFormat.setByteOrder( QAudioFormat::LittleEndian );
    kodiSpeakerFormat.setSampleType( QAudioFormat::Float );

    m_AudioGenOut = new AudioTestGenerator( kodiSpeakerFormat, AUDIO_MS_KODI * 1000, 100, this );
 
    m_AudioIoMgr.toGuiSetSpeakerMode( eSpeakerModePush );
    m_AudioIoMgr.initAudioIoSystem();

    m_SoundTestThread = new SoundTestThread( *this );
    setSoundThreadShouldRun( true );
    m_SoundTestThread->startSoundTestThread();
    
}

//============================================================================
void SoundTestLogic::aboutToDestroy()
{
    setSoundThreadShouldRun( false );
    if( m_SoundTestThread )
    {
        m_SoundTestThread->quit(); // some platforms may not have windows to close so ensure quit()
        m_SoundTestThread->wait();
        delete m_SoundTestThread;
        m_SoundTestThread = nullptr;
    }
 }

//============================================================================
/// Called when need more sound for speaker output
void SoundTestLogic::fromGuiAudioOutSpaceAvail( int freeSpaceLen )
{
    if( freeSpaceLen >= AUDIO_BUF_SIZE_8000_1_S16 )
    {
        const qreal audioBufLen = AUDIO_BUF_SIZE_8000_1_S16;
        static char audioBuf[ AUDIO_BUF_SIZE_8000_1_S16 ];
        static qreal amountRead = 0;
        if( !amountRead )
        {
            amountRead = m_AudioGenIn->readDataNoPositionUpdate( audioBuf, audioBufLen );
        }

        if( amountRead && !m_PauseVoip )
        {
            //LogMsg( LOG_DEBUG, "Space Avail %3.3f ms", m_VoipTimer.elapsedMs() );
            m_VoipTimer.resetTimer();
            m_AudioIoMgr.toGuiPlayAudio( eAppModulePtoP, (int16_t *)audioBuf, AUDIO_BUF_SIZE_8000_1_S16, false );
        }
    }
}

//============================================================================
/// for visualization of audio output
void SoundTestLogic::speakerAudioPlayed( QAudioFormat& format, void * data, int dataLen )
{
    if( m_WaveForm )
    {
        m_WaveForm->speakerAudioPlayed( data, dataLen );
    }
}

//============================================================================
/// for visualization of audio input
void SoundTestLogic::microphoneAudioRecieved( QAudioFormat& /*format*/, void * /*data*/, int /*dataLen*/ )
{

}

//============================================================================
/// Microphone sound capture ( 8000hz PCM 16 bit data, 80ms of sound )
void SoundTestLogic::fromGuiMicrophoneData( int16_t* pcmData, uint16_t pcmDataLenBytes, bool isSilence )
{
    // normally this would go to video chat etc.. but here we just send it to audio out
    m_AudioIoMgr.toGuiPlayAudio( eAppModuleTest, pcmData, pcmDataLenBytes, isSilence );
}

//============================================================================
/// Microphone sound capture with info for echo cancel ( 8000hz PCM 16 bit data, 80ms of sound )
void SoundTestLogic::fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift )
{
    m_AudioIoMgr.toGuiPlayAudio( eAppModuleTest, pcmData, pcmDataLenBytes, isSilence );
}

//============================================================================
/// Mute/Unmute microphone
void SoundTestLogic::fromGuiMuteMicrophone( bool muteMic )
{
    m_AudioIoMgr.fromGuiMuteMicrophone( muteMic );
}

//============================================================================
/// Returns true if microphone is muted
bool SoundTestLogic::fromGuiIsMicrophoneMuted()
{
    return m_AudioIoMgr.fromGuiIsMicrophoneMuted();
}

//============================================================================
/// Mute/Unmute speaker
void SoundTestLogic::fromGuiMuteSpeaker( bool muteSpeaker )
{
    m_AudioIoMgr.fromGuiMuteSpeaker( muteSpeaker );
}

//============================================================================
/// Returns true if speaker is muted
bool SoundTestLogic::fromGuiIsSpeakerMuted()
{
    return m_AudioIoMgr.fromGuiIsSpeakerMuted();
}

//============================================================================
/// Enable/Disable echo cancellation
void SoundTestLogic::fromGuiEchoCancelEnable( bool enable )
{
    m_AudioIoMgr.fromGuiEchoCancelEnable( enable );
}

//============================================================================
/// Returns true if echo cancellation is enabled
bool SoundTestLogic::fromGuiIsEchoCancelEnabled( void )
{
    return true;
}

//============================================================================
void SoundTestLogic::setSoundThreadShouldRun( bool shouldRun )
{
    if( m_SoundTestThread )
    {
        m_SoundTestThread->setThreadShouldRun( shouldRun );
    }
}

//============================================================================
void SoundTestLogic::startStartSoundTestThread()
{ 
    if( m_SoundTestThread )
    {
        m_SoundTestThread->startSoundTestThread();
    }
}

//============================================================================
void SoundTestLogic::stopStartSoundTestThread()
{
    if( m_SoundTestThread )
    {
        m_SoundTestThread->stopSoundTestThread();
    }
}

//============================================================================
bool SoundTestLogic::initSoundTestSystem()
{

    return true;
}

//============================================================================
bool SoundTestLogic::destroySoundTestSystem()
{
    return true;
}

//============================================================================
bool SoundTestLogic::beginSoundTest()
{

    return true;
}

//============================================================================
bool SoundTestLogic::endSoundTest()
{


    return true;
}

//============================================================================
void SoundTestLogic::audioOutNoneClicked()
{

}

//============================================================================
void SoundTestLogic::audioOutPushButtonClicked()
{

}

//============================================================================
void SoundTestLogic::audioOutPullButtonClicked()
{

}

//============================================================================
void SoundTestLogic::audioOutPushPullButtonClicked()
{

}

//============================================================================
void SoundTestLogic::pauseVoipState( int state )
{
    m_PauseVoip = state ? true : false;
}

//============================================================================
void SoundTestLogic::pauseKodiState( int state )
{
    bool isChecked = state ? true : false;
    m_SoundTestThread->pauseSound( isChecked );
}

//============================================================================
void SoundTestLogic::muteSpeakerState( int state )
{
    bool isChecked = state ? true : false;
    m_AudioIoMgr.fromGuiMuteSpeaker( isChecked );
}

//============================================================================
void SoundTestLogic::muteMicrophoneState( int state )
{
    bool isChecked = state ? true : false;
    m_AudioIoMgr.fromGuiMuteMicrophone( isChecked );
}

//============================================================================
