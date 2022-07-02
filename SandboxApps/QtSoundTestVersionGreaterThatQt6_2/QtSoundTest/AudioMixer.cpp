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

#include "AudioMixer.h"
#include "IAudioInterface.h"
#include "AudioIoMgr.h"
#include "AudioUtils.h"

#include "VxDebug.h"
#include <algorithm>
#include <memory.h>

//============================================================================
AudioMixer::AudioMixer( AudioIoMgr& audioIoMgr, IAudioCallbacks& audioCallbacks, QWidget * parent )
: QWidget( parent )
, m_AudioIoMgr( audioIoMgr  )
, m_AudioCallbacks( audioCallbacks )
, m_MixerThread( audioIoMgr )
, m_MixerFormat()
 {
     m_MixerFormat.setSampleRate( 8000 );
     m_MixerFormat.setChannelCount( 1 );
     m_MixerFormat.setSampleFormat( QAudioFormat::Int16 );
     memset( m_ModuleBufIndex, 0, sizeof( m_ModuleBufIndex ) );

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
    AudioMixerFrame& mixerFrame = m_MixerFrames[ getModuleFrameIndex( appModule ) ];
    int written = mixerFrame.toMixerPcm8000HzMonoChannel( appModule, pcmData, isSilence );
    incrementModuleFrameIndex( appModule );
    m_MixerMutex.unlock();
    m_AudioCallbacks.speakerAudioPlayed( getMixerFormat(), (void*)pcmData, written );
    // LogMsg( LOG_VERBOSE, "AudioMixer::toMixerPcm8000HzMonoChannel module %d len %d audio pcm data peak %d silence %d", appModule, pcmDataLenInBytes,
    //    AudioUtils::getPeakPcmAmplitude( pcmData, pcmDataLenInBytes ), AudioUtils::hasSomeSilence( pcmData, pcmDataLenInBytes ) );

    return written;
}

//============================================================================
// read audio data from mixer.
qint64 AudioMixer::readDataFromMixer( char* data, qint64 maxlen, int upSampleMult )
{
    // because the read size cannot be controled with Qt's QAudioSink->setBufferSize the audio read request may not be a multiple of upSampleMult
    // this means we may need to finish lerp of previous read and predictively lerp some samples at end of the request using a peak at the 
    // value that will be the first sample of the next read. Also handle cases where partial lerped samples at end of last read need 
    // prepended to next

    // example of lerped samples for 12 as upsample of sample 1 = 0 and sample 2 = 1200 (first number is index and second is the lerped value)
    // 0 (0), 1 (100), 2 (200), 3 (300), 4 (400), 5 (500), 6 (600), 7 (700), 8 (800), 9 (900), 10 (1000), 11 (1100), 12 (1200)

    if( maxlen <= 0 )
    {
        //        LogMsg( LOG_DEBUG, "readDataFromMixer %lld bytes ", maxlen );
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
    int samplesRead = m_AudioIoMgr.getAudioCallbacks().readGenerated8000HzMono160HzToneData( (char*)mixerReadBuf, (upSamplesToRead + readAppendLerpSample) * 2, peekNextSample );
    int lenRead = samplesRead;
    samplesRead = samplesRead >> 1;

    samplesRead -= readAppendLerpSample; // only count fully upsampleable samples.. the appended partial lerp sample will be handled seperately

    int16_t appendLerpSampleValue = readAppendLerpSample ? mixerReadBuf[ samplesRead ] : mixerReadBuf[ samplesRead - 1 ];
    int16_t nextUpSampleValue = readAppendLerpSample ? appendLerpSampleValue : peekNextSample;

    LogMsg( LOG_VERBOSE, "readDataFromMixer prev lerp cnat %d prev samp %d first %d last %d nextUpVal %d append lerp value %d  peek %d", 
           m_PrevLerpedSamplesCnt, m_PrevLerpedSampleValue, mixerReadBuf[0], mixerReadBuf[ samplesRead - 1 ], nextUpSampleValue, appendLerpSampleValue, peekNextSample );

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
        //AudioUtils::upsamplePcmAudioLerpNext( mixerReadBuf, samplesRead, upSampleMult, nextUpSampleValue, &readReqPcmBuf[ prependLerpCnt ] );
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
    LogMsg( LOG_VERBOSE, "readDataFromMixer m_PrevLerpedSamplesCnt %d", m_PrevLerpedSamplesCnt );

    delete[] mixerReadBuf;
    mixerWasReadByOutput( (int)maxlen, upSampleMult );
    return maxlen;

    // showing wave form causes to much delay and will hear some snapping in the sound.. use at your own risk
    //m_AudioCallbacks.speakerAudioPlayed( getMixerFormat(), (void*)readReqPcmBuf, maxlen );

    int16_t testSamp1 = readReqPcmBuf[ 0 ];
    for( int i = 1; i < maxlen / 2; i++ )
    {
        int16_t testSamp2 = readReqPcmBuf[ i ];
        if( std::abs( testSamp2 - testSamp1 ) > 4000 )
        {
            LogMsg( LOG_VERBOSE, "Read Glitch req %d index %d val1 %d val2 %d", readRequestCnt, i, testSamp1, testSamp2 );
        }

        testSamp1 = testSamp2;
    }

    if( lastSampleOutLastRead )
    {
        if( std::abs( lastSampleOutLastRead - readReqPcmBuf[ 0 ] ) > 4000 )
        {
            LogMsg( LOG_VERBOSE, "Betwen Req Glitch req %d last %d first %d", readRequestCnt, lastSampleOutLastRead, readReqPcmBuf[ 0 ] );
        }
    }

    lastSampleOutLastRead = readReqPcmBuf[ reqSampleCnt - 1 ];

    delete[] mixerReadBuf;
#if 0
    m_MixerMutex.lock();
    int samplesAvailable = getDataReadyForSpeakersLen( true ) / 2;
    int samplesThatWillBeSilence = 0;
    if( samplesAvailable < totalSamplesToRead )
    {
        // we do not have enough audio to satisfiy the audio out requirements
        samplesThatWillBeSilence = totalSamplesToRead - samplesAvailable;
        if( m_WasReset )
        {
            // after reset do not read from mixer until the mixer has enough to satisfy at least on read request size
            memset( data, 0, maxlen );
            totalSamplesToRead = 0;
            samplesThatWillBeSilence = 0;
        }
        else
        {
            LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer will read %d samples past end of mixer data", samplesThatWillBeSilence );
        }
    }

    int samplesRead = 0;
    int samplesToRead = std::min( samplesAvailable, totalSamplesToRead );
    char* mixerReadBuf{ nullptr };
    if( samplesToRead )
    {
        mixerReadBuf = new char[ samplesToRead * 2 ];
        int frameIndex = m_MixerReadIdx;
        for( int i = 0; i < MAX_MIXER_FRAMES; ++i )
        {
            int samplesLeftToRead = samplesToRead - samplesRead;
            if( !samplesLeftToRead )
            {
                break;
            }

            AudioMixerFrame& mixerFrame = m_MixerFrames[ frameIndex ];

            int maxSamplesThisRead = std::min( samplesLeftToRead, getMixerSamplesPerFrame() );
            /*
            if( maxSamplesThisRead != getMixerSamplesPerFrame() )
            {
                LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer reading %d samples", maxSamplesThisRead );
            }
            */

            int samplesThisRead = mixerFrame.readMixerFrame( &mixerReadBuf[ samplesRead * 2 ], m_SpeakersMuted, maxSamplesThisRead, 1, m_PrevLastsample, m_PrevLastsample );
            if( !samplesThisRead )
            {
                // ran out of samples
                LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer failed to read %d samples", samplesThisRead );
                break;
            }
            /*
            else
            {
                LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer samplesThisRead %d peak %d has silence %d", samplesThisRead,
                    AudioUtils::getPeakPcmAmplitude( (int16_t*)(&data[ samplesRead * 2 * upSampleMult ]), samplesThisRead * 2 ),
                    AudioUtils::hasSomeSilence( (int16_t*)(&data[ samplesRead * 2 * upSampleMult ]), samplesThisRead * 2 ) );
            }
            */

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

    /*
    if( samplesThatWillBeSilence )
    {
        // fill in at end of buffer
        int toFillLen = maxlen - (samplesRead * 2);
        memset( &data[ samplesRead * 2 ], 0, toFillLen );
        samplesRead += samplesThatWillBeSilence;
    }

    else
    {
        int hasSomSilence = AudioUtils::hasSomeSilence( (int16_t*)(data), maxlen );
        if( !m_WasReset && hasSomSilence )
        {
            LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer has %d samples of silence", hasSomSilence );
        }
    }
    */

    if( m_WasReset && samplesRead )
    {
        // started reading from mixer.. if do not have enough next read it will be a underrun condition
        m_WasReset = false;
    }

    m_MixerMutex.unlock();

    int samplesRemainder = (maxlen % upSampleMult) / 2;
    if( samplesRemainder && samplesRead )
    {
        // not a multiple of what we can up sample
        // best we can do is replicate the last sample
        /*
        int16_t* pcmData = (int16_t*)&data[ ( samplesRead - 1 ) * 2 ];
        int16_t lastSample = *pcmData;
        for( int i = 0; i < samplesRemainder; i++ )
        {
            pcmData++;
            *pcmData = lastSample;
        }
        */
    }

    if( samplesRead && mixerReadBuf )
    {
        //        m_AudioCallbacks.speakerAudioPlayed( getMixerFormat(), (void*)mixerReadBuf, samplesRead * 2 );
    }


#endif // 0
    // m_AudioIoMgr.getAudioCallbacks().readGenerated4888HzData( data, maxlen );
    mixerWasReadByOutput( (int)maxlen, upSampleMult );
    return maxlen;
}

//============================================================================
void AudioMixer::mixerWasReadByOutput( int readLen, int upSampleMult )
{
    // to keep everything in mixer block size we say how many blocks would be read this time if available and keep track of the partial frame part to add to the next call
    m_AudioOutRead += readLen;
    int mixerFramesRead = m_AudioOutRead / (getMixerFrameSize() * upSampleMult);
    int announceAvailableSpace = mixerFramesRead * getMixerFrameSize();
    m_AudioOutRead -= announceAvailableSpace * upSampleMult;

    m_MixerThread.setMixerSpaceAvailable( announceAvailableSpace );
    m_MixerThread.releaseAudioMixerThread();
}

//============================================================================
/// space available to que audio data into buffer
int AudioMixer::audioQueFreeSpace( EAppModule appModule, bool mixerIsLocked)
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
