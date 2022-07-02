#include "SoundTestLogic.h"
#include "AudioUtils.h"

#include "VxDebug.h"

#include <QSurface>
#include <qmath.h>
#include <QTimer>

//============================================================================
SoundTestLogic::SoundTestLogic( Waveform * waveForm, QWidget *parent )
: m_WaveForm( waveForm )
, m_AudioIoMgr( *this, this )
{
    int displayBufCnt = 17; // 16 * 80ms = 1.280 seconds
    int readReqLen = 16384;

    // m_WaveForm->initialize( m_AudioIoMgr.getAudioOutFormat(), AUDIO_BUF_SIZE_48000_1_S16, AUDIO_MS_SPEAKERS * (displayBufCnt / 2) * 1000, AUDIO_BUF_SIZE_48000_1_S16 * ( displayBufCnt / 2 ) );
    m_WaveForm->initialize( m_AudioIoMgr.getAudioOutFormat(), readReqLen, AUDIO_MS_SPEAKERS * (displayBufCnt / 2) * 1000, readReqLen * (displayBufCnt / 2) );

    QAudioFormat kodiSpeakerFormat;
    kodiSpeakerFormat.setSampleRate( 48000 );
    kodiSpeakerFormat.setChannelCount( 2 );
    kodiSpeakerFormat.setSampleFormat(QAudioFormat::Float);
 
    m_AudioIoMgr.initAudioIoSystem();

    // I have no clue why.. maybe some low freq filter but in qt/android a tone of 100 hz is ignored.. use 200Hz or higher
    int toneSampleRateHz = 200;
    double fps = 1.0f / (double)toneSampleRateHz;
    int durationMicroSeconds = fps * 1000000;
    m_Test100HzGenerator.reset( new AudioTestGenerator( m_AudioIoMgr.getAudioInFormat(), durationMicroSeconds, toneSampleRateHz ) );
    m_Test100HzGenerator->start();

    toneSampleRateHz = 400;
    fps = 1.0f / (double)toneSampleRateHz;
    durationMicroSeconds = fps * 1000000;
    m_Test400HzGenerator.reset( new AudioTestGenerator( m_AudioIoMgr.getAudioInFormat(), durationMicroSeconds, toneSampleRateHz ) );
    m_Test400HzGenerator->start();

    toneSampleRateHz = 600;
    fps = 1.0f / (double)toneSampleRateHz;
    durationMicroSeconds = fps * 1000000;
    m_Test600HzKodiFloatGenerator.reset( new AudioTestGenerator( kodiSpeakerFormat, durationMicroSeconds, toneSampleRateHz ) );
    m_Test600HzKodiFloatGenerator->start();

    QAudioFormat mixerFormat;
    mixerFormat.setSampleRate( 8000 );
    mixerFormat.setChannelCount( 1 );
    mixerFormat.setSampleFormat( QAudioFormat::Int16 );

    toneSampleRateHz = 200;
    fps = 1.0 / (double)toneSampleRateHz;
    durationMicroSeconds = fps * 1000000;
    m_Test8000HzMono160HzToneGenerator.reset( new AudioTestGenerator( mixerFormat, durationMicroSeconds, toneSampleRateHz ) );

    // kodi float sound thread not currently supported
    //m_SoundTestThread = new SoundTestThread( *this );
    //setSoundThreadShouldRun( true );
    //m_SoundTestThread->startSoundTestThread(); 
}

//============================================================================
void SoundTestLogic::aboutToDestroy()
{
    //setSoundThreadShouldRun( false );
    //if( m_SoundTestThread )
    //{
    //    m_SoundTestThread->quit(); // some platforms may not have windows to close so ensure quit()
    //    m_SoundTestThread->wait();
    //    delete m_SoundTestThread;
    //    m_SoundTestThread = nullptr;
    //}
 }

//============================================================================
/// Called when need more sound for speaker output
void SoundTestLogic::fromGuiAudioOutSpaceAvail( int freeSpaceLen )
{  
    if( freeSpaceLen >= AUDIO_BUF_SIZE_8000_1_S16 )
    {
        playP2p400HzIfEnabled( freeSpaceLen );
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
/// Microphone sound capture with info for echo cancel ( 8000hz PCM 16 bit data, 80ms of sound )
void SoundTestLogic::fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift )
{
    m_AudioIoMgr.fromGuiMicrophoneData( eAppModuleTest, pcmData, pcmDataLenBytes, isSilence );
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
void SoundTestLogic::pauseVoipState( int state )
{
    m_PauseVoip = state ? true : false;
    m_AudioIoMgr.toGuiWantSpeakerOutput( eAppModulePtoP, !m_PauseVoip );
}

//============================================================================
void SoundTestLogic::pauseKodiState( int state )
{
    m_PauseKodi = state ? true : false;
    m_AudioIoMgr.toGuiWantSpeakerOutput( eAppModuleKodi, !m_PauseKodi );
    // kodi only sound generator thread
    // m_SoundTestThread->pauseSound( m_PauseKodi );
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
void SoundTestLogic::fromAudioOutResumed( void )
{
    playP2p400HzIfEnabled();
}

//============================================================================
/// Called when need more sound for speaker output
void SoundTestLogic::playP2p400HzIfEnabled( int mixerSpaceAvailable )
{
    if( !m_PauseVoip )
    {
        int frameCount = mixerSpaceAvailable ? mixerSpaceAvailable / MIXER_BUF_SIZE_8000_1_S16 : 1;
        QAudioFormat audioFormat = m_Test100HzGenerator->getAudioFormat();
        int divideCnt = (audioFormat.sampleRate() * audioFormat.channelCount()) / 8000;
        int genBufLen = MIXER_BUF_SIZE_8000_1_S16 * divideCnt;
        char* genBuf = new char[ genBufLen ];
        static char audioBuf[ MIXER_BUF_SIZE_8000_1_S16 ];

        for( int i = 0; i < frameCount; i++ )
        {
            if( genBufLen == m_Test100HzGenerator->readData( genBuf, genBufLen ) )
            {
                if( divideCnt == 1 )
                {
                    m_AudioIoMgr.getAudioOutMixer().toMixerPcm8000HzMonoChannel( eAppModulePtoP, (int16_t*)genBuf, AUDIO_BUF_SIZE_8000_1_S16, false );
                }
                else
                {
                    int16_t* pcmData = (int16_t*)genBuf;
                    int16_t lastVal = pcmData[ 0 ];
                    for( int i = 0; i < 100; i++ )
                    {
                        int dif = std::abs( std::abs( lastVal ) - std::abs( pcmData[ i ] ) );
                        if( dif > 2000 )
                        {
                            LogMsg( LOG_VERBOSE, "playP2p400HzIfEnabled 48000Hz ix %d val %d lastVal %d dif %d", i, pcmData[ i ], lastVal, dif );
                        }

                        lastVal = pcmData[ i ];
                    }

                    // down sample to PCM 8000Hz Mono channel
                    AudioUtils::dnsamplePcmAudio( (int16_t*)genBuf, AUDIO_BUF_SIZE_8000_1_S16 / 2, divideCnt, (int16_t*)audioBuf );
                    pcmData = (int16_t*)audioBuf;
                    lastVal = pcmData[ 0 ];
                    for( int i = 0; i < 100; i++ )
                    {
                        int dif = std::abs( std::abs( lastVal ) - std::abs( pcmData[ i ] ) );
                        if( dif > 11000 )
                        {
                            LogMsg( LOG_VERBOSE, "playP2p400HzIfEnabled 8000Hz ix %d val %d lastVal %d  dif %d", i, pcmData[ i ], lastVal, dif );
                        }

                        lastVal = pcmData[ i ];
                    }

                    m_AudioIoMgr.getAudioOutMixer().toMixerPcm8000HzMonoChannel( eAppModulePtoP, (int16_t*)audioBuf, AUDIO_BUF_SIZE_8000_1_S16, false );
                }
            }
        }       

        delete[] genBuf;
    }
}

//============================================================================
int SoundTestLogic::getAudioInPeakAmplitude( void )
{
    return m_AudioIoMgr.getAudioInPeakAmplitude();
}

//============================================================================
int SoundTestLogic::getAudioOutPeakAmplitude( void )
{
    return m_AudioIoMgr.getAudioOutPeakAmplitude();
}

//============================================================================
void SoundTestLogic::microphoneVolumeChanged( int volume )
{
    m_AudioIoMgr.setMicrophoneVolume( (float)volume );
}

//============================================================================
void SoundTestLogic::speakerVolumeChanged( int volume )
{
    m_AudioIoMgr.getAudioOutIo().setSpeakerVolume( (float)volume );
}

//============================================================================
int SoundTestLogic::readGenerated4800HzMono100HzToneData( char* data, int maxlen )
{
    if( !m_PauseVoip && maxlen )
    {

        // read directly from tone generator
        m_Test100HzGenerator->readData( data, maxlen );
        return maxlen;

        /*
        // read from tone generarto and downsample to 8000Hz then back up to 48898Hz.. 
        QAudioFormat audioFormat = m_Test100HzGenerator->getAudioFormat();

        // Note: A small clicking sound will be heard if maxlen is not a multiple of divide cnt and cannot be avoided
        int divideCnt = (audioFormat.sampleRate() * audioFormat.channelCount()) / 8000;

        int genBufLen = maxlen;
        char* genBuf = new char[ genBufLen ];
        if( genBufLen == m_Test100HzGenerator->readData( genBuf, genBufLen ) )
        {

            static int16_t lastSavedSample = 0;
            const qreal divBufLen = maxlen / divideCnt;
            char* divBuf = new char[ divBufLen ];

            // down sample to PCM 8000Hz Mono channel
            AudioUtils::dnsamplePcmAudio( (int16_t*)genBuf, divBufLen / 2, divideCnt, (int16_t*)divBuf );

            static int16_t lastSample = 0;
            // AudioUtils::upsamplePcmAudio( (int16_t*)divBuf, lastSavedSample, divBufLen, divideCnt, (int16_t*)data, lastSample );

            delete divBuf;
            lastSavedSample = ((int16_t*)data)[ maxlen - 2 ];
        }



        delete genBuf;
                */


       
        /*
        QAudioFormat audioFormat = m_Test100HzGenerator->getAudioFormat();
        int divideCnt = (audioFormat.sampleRate() * audioFormat.channelCount()) / 8000;

        int mixerSpaceAvailable = maxlen;
        int frameCount = mixerSpaceAvailable ? mixerSpaceAvailable / MIXER_BUF_SIZE_8000_1_S16 : 1;
        int genBufLen = MIXER_BUF_SIZE_8000_1_S16 * divideCnt;
        char* genBuf = new char[ genBufLen ];
        static char audioBuf[ MIXER_BUF_SIZE_8000_1_S16 ];

        for( int i = 0; i < frameCount; i++ )
        {
            if( genBufLen == m_Test100HzGenerator->readData( genBuf, genBufLen ) )
            {
                if( divideCnt == 1 )
                {
                    m_AudioIoMgr.getAudioOutMixer().toMixerPcm8000HzMonoChannel( eAppModulePtoP, (int16_t*)genBuf, AUDIO_BUF_SIZE_8000_1_S16, false );
                }
                else
                {
                    int16_t* pcmData = (int16_t*)genBuf;
                    int16_t lastVal = pcmData[ 0 ];
                    for( int i = 0; i < 100; i++ )
                    {
                        int dif = std::abs( std::abs( lastVal ) - std::abs( pcmData[ i ] ) );
                        if( dif > 2000 )
                        {
                            LogMsg( LOG_VERBOSE, "readGenerated4888HzData 48000Hz ix %d val %d lastVal %d dif %d", i, pcmData[ i ], lastVal, dif );
                        }

                        lastVal = pcmData[ i ];
                    }

                    // down sample to PCM 8000Hz Mono channel
                    AudioUtils::dnsamplePcmAudio( (int16_t*)genBuf, AUDIO_BUF_SIZE_8000_1_S16 / 2, divideCnt, (int16_t*)audioBuf );
                    pcmData = (int16_t*)audioBuf;
                    lastVal = pcmData[ 0 ];
                    for( int i = 0; i < 100; i++ )
                    {
                        int dif = std::abs( std::abs( lastVal ) - std::abs( pcmData[ i ] ) );
                        if( dif > 11000 )
                        {
                            LogMsg( LOG_VERBOSE, "readGenerated4888HzData 8000Hz ix %d val %d lastVal %d  dif %d", i, pcmData[ i ], lastVal, dif );
                        }

                        lastVal = pcmData[ i ];
                    }

                    m_AudioIoMgr.getAudioOutMixer().toMixerPcm8000HzMonoChannel( eAppModulePtoP, (int16_t*)audioBuf, AUDIO_BUF_SIZE_8000_1_S16, false );
                }
            }
        }

        delete genBuf;
        */

        return maxlen;
    }

    return 0;

}

//============================================================================
int SoundTestLogic::readGenerated8000HzMono160HzToneData( char* data, int maxlen, int16_t& peekNextSample )
{
    if( maxlen )
    {
        m_Test8000HzMono160HzToneGenerator->readData( data, maxlen );
        peekNextSample = m_Test8000HzMono160HzToneGenerator->peekNextSample();
    }

    return maxlen;
}
