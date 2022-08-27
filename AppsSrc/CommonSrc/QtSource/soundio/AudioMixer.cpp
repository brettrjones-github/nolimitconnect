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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "Appcommon.h"
#include "AudioMixer.h"
#include "AudioIoMgr.h"
#include "AudioUtils.h"

#include <GuiInterface/IAudioInterface.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <algorithm>
#include <memory.h>

//============================================================================
AudioMixer::AudioMixer( AudioIoMgr& audioIoMgr, IAudioCallbacks& audioCallbacks, QWidget * parent )
: QWidget( parent )
, m_AudioIoMgr( audioIoMgr )
, m_MyApp( audioIoMgr.getMyApp() )
, m_AudioCallbacks( audioCallbacks )
, m_MixerThread( audioIoMgr )
, m_MixerFormat()
 {
     m_MixerFormat.setSampleRate( 8000 );
     m_MixerFormat.setChannelCount( 1 );
     m_MixerFormat.setSampleFormat( QAudioFormat::Int16 );
     memset( m_ModuleBufIndex, 0, sizeof( m_ModuleBufIndex ) );

     for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
     {
         m_MixerFrames[ i ].setFrameIndex( i );
     }

     m_MixerThread.setThreadShouldRun( true );
     m_MixerThread.startAudioMixerThread();

     m_ElapsedTimer.restart();
 }

//============================================================================
void AudioMixer::shutdownAudioMixer( void )
{
    m_MixerThread.stopAudioMixerThread();
}

//============================================================================
void AudioMixer::wantSpeakerOutput( bool enableOutput )
{
    if( enableOutput )
    {
        // reset everthing to initial start positions
        m_MixerMutex.lock();   
        for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
        {
            m_MixerFrames[ i ].clearFrame();
        }

        memset( m_ModuleBufIndex, 0, sizeof( m_ModuleBufIndex ) );
        m_MixerReadIdx = 0;
        m_WasReset = true;
        m_AudioOutRead = 0;
        m_PrevLerpedSamplesCnt = 0;
        m_PrevLerpedSampleValue = 0;
        m_MixerMutex.unlock();

        m_ElapsedTimer.restart();
    }
}

//============================================================================
int AudioMixer::toMixerPcm8000HzMonoChannel( EAppModule appModule, int16_t * pcmData, int pcmDataLenInBytes, bool isSilence )
{
    // add audio data to mixer.. assumes pcm signed short mono channel 8000 Hz. Mix into any existing audio if required return total written to buffer
    if( pcmDataLenInBytes != getMixerFrameSize() )
    {
        LogMsg( LOG_ERROR, "AudioMixer::toMixerPcm8000HzMonoChannel audio pcm data length must be %d", getMixerFrameSize() );
        return pcmDataLenInBytes;
    }

    m_MixerMutex.lock();
    static bool firstWrite = true;
    if( firstWrite )
    {
        m_ModuleBufIndex[ appModule ] = m_MixerReadIdx + 1;
        if( m_ModuleBufIndex[ appModule ] >= MAX_MIXER_FRAMES )
        {
            m_ModuleBufIndex[ appModule ] = 0;
        }

        firstWrite = false;
    }

    AudioMixerFrame& mixerFrame = m_MixerFrames[ getModuleFrameIndex( appModule ) ];
    int written = mixerFrame.toMixerPcm8000HzMonoChannel( appModule, pcmData, isSilence );
    incrementModuleFrameIndex( appModule );
    m_MixerMutex.unlock();
    // m_AudioCallbacks.speakerAudioPlayed( getMixerFormat(), (void*)pcmData, written );
    // LogMsg( LOG_VERBOSE, "AudioMixer::toMixerPcm8000HzMonoChannel module %d len %d audio pcm data peak %d silence %d", appModule, pcmDataLenInBytes,
    //    AudioUtils::getPeakPcmAmplitude( pcmData, pcmDataLenInBytes ), AudioUtils::hasSomeSilence( pcmData, pcmDataLenInBytes ) );

    return written;
}

//============================================================================
// read audio data from mixer.
qint64 AudioMixer::readRequestFromSpeaker( char* data, qint64 maxlen, int upSampleMult, AudioSampleBuf& echoFarBuf )
{
    // because the read size cannot be controled with Qt's QAudioSink->setBufferSize the audio read request may not be a multiple of upSampleMult
    // this means we may need to finish lerp of previous read and predictively lerp some samples at end of the request using a peak at the 
    // value that will be the first sample of the next read. Also handle cases where partial lerped samples at end of last read need 
    // prepended to next

    /*
    if( maxlen <= 0 )
    {
         // LogMsg( LOG_DEBUG, "readDataFromMixer %lld bytes ", maxlen );
         return 0;
    }

    if( !m_AudioIoMgr.isAudioInitialized() )
    {
        memset( data, 0, maxlen );
        return maxlen;
    }

    // for test if speaker output is working without mixer involved
    //m_AudioIoMgr.getAudioCallbacks().readGenerated4800HzMono100HzToneData( data, maxlen );
    //m_AudioCallbacks.speakerAudioPlayed( getMixerFormat(), (void*)data, maxlen ); // causes audio popping artifacts so comment out if want pure tone
    //mixerWasReadByOutput( (int)maxlen, upSampleMult );
    //return maxlen;

    static int readRequestCnt = 0;
    readRequestCnt++;
    static int16_t lastSampleOutLastRead = 0;

    // convert request buffer to pcm 2 byte samples
    int16_t* readReqPcmBuf = (int16_t*)data;
    int reqSampleCnt = maxlen / 2;

    // all data in mixer in 8000 Hz mono pcm (Int16).. use upSampleMult to convert to sample rate required by audio out
    // you would think you could just read what bytes are availible but no. It is all or nothing else you will get missing sound and qt wigs out
    int upSamplesToRead = reqSampleCnt / upSampleMult; // how many samples can be up sampled fully
    int remainderSamples = reqSampleCnt % upSampleMult; // how many samples have to be partially lerped

    // calculate how many lerp samples should be prepended and appended to read
    int prependLerpCnt{ 0 };
    int appendLerpCnt{ 0 };

    if( remainderSamples )
    {
        if( m_PrevLerpedSamplesCnt )
        {
            // finish lerping the last read into beginning of this read
            prependLerpCnt = upSampleMult - m_PrevLerpedSamplesCnt;
            if( prependLerpCnt > remainderSamples )
            {
                // will need a partial read so decrement the fully upsample count
                upSamplesToRead--;
            }
        }

        appendLerpCnt = reqSampleCnt - ((upSamplesToRead * upSampleMult) + prependLerpCnt);
    }

    int readAppendLerpSample = 0;
    if( appendLerpCnt )
    {
        readAppendLerpSample = 1;
    }

    int16_t* mixerReadBuf = new int16_t[ upSamplesToRead + readAppendLerpSample ];

    int16_t peekNextSample;

    // for test without mixer read involve
    //int samplesRead = m_AudioIoMgr.getAudioCallbacks().readGenerated8000HzMono160HzToneData( (char*)mixerReadBuf, (upSamplesToRead + readAppendLerpSample) * 2, peekNextSample );
    //int lenRead = samplesRead;
    //samplesRead = samplesRead >> 1;

    // read samples from mixer
    int mixerSampleCnt = readDataFromMixer( mixerReadBuf, upSamplesToRead + readAppendLerpSample, peekNextSample );

    // put the 8000Hz data into the echo buffer for processing
    std::vector<int16_t> inPcmData( &mixerReadBuf[ 0 ], &mixerReadBuf[ upSamplesToRead + readAppendLerpSample - 1 ] );
    echoFarBuf.insert( echoFarBuf.end(), inPcmData.begin(), inPcmData.end() );

    m_PeakAmplitude = AudioUtils::getPeakPcmAmplitude0to100( mixerReadBuf, mixerSampleCnt );

    int samplesRead = upSamplesToRead + readAppendLerpSample;

    samplesRead -= readAppendLerpSample; // only count fully upsampleable samples.. the appended partial lerp sample will be handled seperately

    int16_t appendLerpSampleValue = readAppendLerpSample ? mixerReadBuf[ samplesRead ] : mixerReadBuf[ samplesRead - 1 ];
    int16_t nextUpSampleValue = readAppendLerpSample ? appendLerpSampleValue : peekNextSample;

    //LogMsg( LOG_VERBOSE, "readDataFromMixer prev lerp cnat %d prev samp %d first %d last %d nextUpVal %d append lerp value %d  peek %d", 
    //       m_PrevLerpedSamplesCnt, m_PrevLerpedSampleValue, mixerReadBuf[0], mixerReadBuf[ samplesRead - 1 ], nextUpSampleValue, appendLerpSampleValue, peekNextSample );

    int calcSampleCnt = (samplesRead * upSampleMult) + appendLerpCnt + prependLerpCnt;
    if( calcSampleCnt != reqSampleCnt )
    {
        LogMsg( LOG_VERBOSE, "readDataFromMixer incorrect sample count requested %d to read %d", reqSampleCnt, calcSampleCnt );
    }

    // fill prepended lerps
    if( prependLerpCnt && samplesRead )
    {
        for( int i = 0; i < prependLerpCnt; ++i )
        {
            readReqPcmBuf[ i ] = AudioUtils::lerpPcm( m_PrevLerpedSampleValue, mixerReadBuf[ 0 ], upSampleMult, m_PrevLerpedSamplesCnt + i );
        }
    }

    // fill upsamples of mixer read
    if( 1 == upSampleMult )
    {
        memcpy( &readReqPcmBuf[ prependLerpCnt ], mixerReadBuf, samplesRead * 2 );
    }
    else
    {
        AudioUtils::upsamplePcmAudioLerpNext( mixerReadBuf, samplesRead, upSampleMult, nextUpSampleValue, &readReqPcmBuf[ 0 ] );
    }

    // fill apended lerps
    if( appendLerpCnt && samplesRead )
    {
        // LogMsg( LOG_VERBOSE, "readDataFromMixer appending cnt %d from %d to %d", appendLerpCnt, appendLerpSampleValue, peekNextSample );
        for( int i = 0; i < appendLerpCnt; ++i )
        {
            int reqBufIndex = i + prependLerpCnt + (samplesRead * upSampleMult);
            if( reqBufIndex >= reqSampleCnt )
            {
                LogMsg( LOG_VERBOSE, "append lerp index %d is past end of request buffer size %d", reqBufIndex, reqSampleCnt );
            }

            readReqPcmBuf[ i + prependLerpCnt + (samplesRead * upSampleMult) ] = AudioUtils::lerpPcm( appendLerpSampleValue, peekNextSample, (float)upSampleMult, i );
        }
    }

//    LogMsg( LOG_VERBOSE, "readDataFromMixer reqlen %d prepend lerp %d len %d full samples %d len %d append lerp %d len %d read append lerp sample %d", 
//        maxlen, prependLerpCnt, prependLerpCnt * 2, samplesRead, samplesRead * upSampleMult * 2, appendLerpCnt, appendLerpCnt * 2 , readAppendLerpSample );

    // determine last sample for next read
    m_PrevLerpedSampleValue = appendLerpSampleValue;
    m_PrevLerpedSamplesCnt = appendLerpCnt;
    //LogMsg( LOG_VERBOSE, "readDataFromMixer m_PrevLerpedSamplesCnt %d", m_PrevLerpedSamplesCnt );

    delete[] mixerReadBuf;
    mixerWasReadByOutput( (int)maxlen, upSampleMult );
    return maxlen;
    */
    return maxlen;
}

//============================================================================
void AudioMixer::mixerWasReadByOutput( int readLen, int upSampleMult )
{
    // to keep everything in mixer block size we say how many blocks would be read this time if available and keep track of the partial frame part to add to the next call
    //m_AudioOutRead += readLen;
    //int mixerFramesRead = m_AudioOutRead / (getMixerFrameSize() * upSampleMult);
    //int announceAvailableSpace = mixerFramesRead * getMixerFrameSize();
    //m_AudioOutRead -= announceAvailableSpace * upSampleMult;

    // BRJ mixer thread will go away with the new design of having a timer event AudioMasterClock determine when to call fromGuiAudioOutSpaceAvail
    // m_MixerThread.setMixerSpaceAvailable( announceAvailableSpace );
    // m_MixerThread.releaseAudioMixerThread();
}

//============================================================================
/// space available to que audio data into buffer
int AudioMixer::audioQueFreeSpace( EAppModule appModule, bool mixerIsLocked )
{
    int freeSpace = MAX_MIXER_FRAMES * getMixerFrameSize() - audioQueUsedSpace( appModule, mixerIsLocked );
    if( freeSpace < 0 )
    {
        freeSpace = 0;
    }

    return freeSpace;
}

//============================================================================
/// space available to que audio data into mixer
int AudioMixer::getAudioMixerFreeSpace( bool mixerIsLocked )
{
    return MAX_MIXER_FRAMES * getMixerFrameSize() - audioQueUsedSpace( eAppModuleAll, mixerIsLocked );
}

//============================================================================
/// space used in audio que buffer
int AudioMixer::audioQueUsedSpace( EAppModule appModule, bool mixerIsLocked )
{
    int audioUsedSpace = 0;
    if( ( appModule > 1 ) && ( appModule < eMaxAppModule ) )
    {
        if( !mixerIsLocked )
        {
            lockMixer();
        }

        int frameIndex = getModuleFrameIndex( appModule );
        for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
        {
            if( m_MixerFrames[ frameIndex ].hasModuleAudio( appModule ) )
            {
                audioUsedSpace += m_MixerFrames[ frameIndex ].audioLenInUse();
            }
            else
            {
                // ran out of audio filled by this module
                break;
            }

            frameIndex++;
            if( frameIndex >= MAX_MIXER_FRAMES )
            {
                frameIndex = 0;
            }
        }

        if( !mixerIsLocked )
        {
            unlockMixer();
        }     
    }
    else if( eAppModuleAll == appModule )
    {
        if( !mixerIsLocked )
        {
            lockMixer();
        }

        int frameIndex = m_MixerReadIdx;
        for( int i = 0; i < MAX_MIXER_FRAMES; i++ )
        {
            if( m_MixerFrames[ frameIndex ].hasAnyAudio() )
            {
                audioUsedSpace += m_MixerFrames[ frameIndex ].audioLenInUse();
            }
            else
            {
                // ran out of audio filled by any module
                break;
            }

            frameIndex++;
            if( frameIndex >= MAX_MIXER_FRAMES )
            {
                frameIndex = 0;
            }
        }

        if( !mixerIsLocked )
        {
            unlockMixer();
        }
    }

    return audioUsedSpace;
}

//============================================================================
// get length of data buffered and ready for speaker out
int AudioMixer::getDataReadyForSpeakersLen( bool mixerIsLocked )
{
    return audioQueUsedSpace( eAppModuleAll, mixerIsLocked );
}

//============================================================================
// get length of data buffered and ready for speaker total milliseconds in duration
int AudioMixer::getDataReadyForSpeakersMs( bool mixerIsLocked )
{
    int bytesAudio = getDataReadyForSpeakersLen( mixerIsLocked );
    return calcualateMixerBytesToMs( bytesAudio );
}
    
//============================================================================
int AudioMixer::calcualateMixerBytesToMs( int bytesAudio8000Hz )
{
    // 8000 Hz = 8000 samples per second = 8 samples per ms
    return ( bytesAudio8000Hz / 2 ) / 8;
}

//============================================================================
int AudioMixer::calcualateAudioOutDelayMs( void )
{
    int elapsedMs = (int)(m_MyApp.elapsedMilliseconds() - m_LastReadTimeStamp);
    int msLeftInQtOut;
    if( m_LastReadSamplesMs > elapsedMs )
    {
        msLeftInQtOut = m_LastReadSamplesMs - elapsedMs;
    }
    else
    {
        msLeftInQtOut = 0;
    }

    return getDataReadyForSpeakersMs() + msLeftInQtOut;
}

//============================================================================
// read mono 8000Hz pcm audio data from mixer.. fill silence for underrun of data. return number of silenced samples
int AudioMixer::readDataFromMixer( int16_t* pcmRetBuf, int samplesRequested, int16_t& peekAtNextSample )
{
    // if want to just fill with tone generated data
    //m_AudioIoMgr.getAudioCallbacks().readGenerated8000HzMono160HzToneData( (char*)pcmRetBuf, sampleCnt * 2, peekAtNextSample );
    //return 0;

    int sampleCnt = samplesRequested;

    m_MixerMutex.lock();
    int samplesAvailable = getDataReadyForSpeakersLen( true ) / 2;
    int samplesThatWillBeSilence = 0;
    if( samplesAvailable < sampleCnt )
    {
        // we do not have enough audio to satisfiy the audio out requirements
        samplesThatWillBeSilence = sampleCnt - samplesAvailable;
        if( m_WasReset )
        {
            // after reset do not read from mixer until the mixer has enough to satisfy at least one read request size
            // memset( pcmRetBuf, 0, sampleCnt * 2 ); // not required.. will get set below
            sampleCnt = 0;
            samplesThatWillBeSilence = 0;
        }
        else
        {
            LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer will read %d samples past end of mixer data", samplesThatWillBeSilence );
        }
    }

    int samplesRead = 0;
    int samplesToRead = std::min( samplesAvailable, sampleCnt );
    if( samplesToRead )
    {
        int frameIndex = m_MixerReadIdx;
        for( int i = 0; i < MAX_MIXER_FRAMES; ++i )
        {
            int samplesLeftToRead = samplesToRead - samplesRead;
            if( !samplesLeftToRead )
            {
                break;
            }

            AudioMixerFrame& mixerFrame = m_MixerFrames[ frameIndex ];
            if( !mixerFrame.audioLenInUse() )
            {
                LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer Mixer Frame %d is empty", frameIndex );
                break;
            }

            int maxSamplesThisRead = std::min( samplesLeftToRead, getMixerSamplesPerFrame() );

            int samplesThisRead = mixerFrame.readMixerFrame( &pcmRetBuf[ samplesRead ], m_SpeakersMuted, maxSamplesThisRead );
            if( !samplesThisRead )
            {
                // ran out of samples
                LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer failed to read %d samples", samplesThisRead );
                break;
            }

            if( !mixerFrame.audioLenInUse() )
            {
                frameIndex = incrementMixerReadIndex();
            }

            if( samplesThisRead < 0 )
            {
                // programmer error
                LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer negative samplesThisRead %d", samplesThisRead );
                break;
            }

            if( samplesThisRead > maxSamplesThisRead )
            {
                // programmer error
                LogMsg( LOG_ERROR, "AudioMixer::readDataFromMixer read more samples then should samplesThisRead %d", samplesThisRead );
                break;
            }

            samplesRead += samplesThisRead;
        }
    }

    if( m_WasReset && samplesRead )
    {
        // started reading from mixer.. if do not have enough next read it will be a underrun condition
        m_WasReset = false;
    }

    AudioMixerFrame& peekMixerFrame = m_MixerFrames[ m_MixerReadIdx ];
    peekAtNextSample = peekMixerFrame.peekAtNextSampleToRead();

    m_MixerMutex.unlock();

    if( samplesRead + samplesThatWillBeSilence < samplesRequested )
    {
        // not enough samples available
        memset( &pcmRetBuf[ samplesRead ], 0, (samplesRequested - samplesRead) * 2 );
    }

    m_LastReadSamplesMs = AudioUtils::audioDurationMs( m_MixerFormat, samplesRequested * 2 );
    m_LastReadTimeStamp = m_MyApp.elapsedMilliseconds();

    return samplesRequested;
}
