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
        m_PartialFrameRead = 0;
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

    LogMsg( LOG_VERBOSE, "AudioMixer::toMixerPcm8000HzMonoChannel module %d len %d audio pcm data peak %d silence %d", appModule, pcmDataLenInBytes,
        AudioUtils::getPeakPcmAmplitude( pcmData, pcmDataLenInBytes ), AudioUtils::hasSomeSilence( pcmData, pcmDataLenInBytes ) );

    return written;
}

//============================================================================
// read audio data from mixer.
qint64 AudioMixer::readDataFromMixer( char* data, qint64 maxlen, int upSampleMult )
{
    // return m_AudioIoMgr.getAudioCallbacks().readGenerated4888HzData( data, maxlen );
    if( maxlen <= 0 )
    {
        //        LogMsg( LOG_DEBUG, "readDataFromMixer %lld bytes ", maxlen );
        m_PartialFrameRead = 0;
        return 0;
    }

    if( !m_AudioIoMgr.isAudioInitialized() )
    {
        memset( data, 0, maxlen );
        m_PartialFrameRead = 0;
        return maxlen;
    }

    // all data in mixer in 8000 Hz mono pcm (Int16).. use upSampleMult to convert to sample rate required by audio out
    // you would think you could just read what bytes are availible but no. It is all or nothing else you will get missing sound and qt wigs out
    int totalSamplesToRead = (maxlen / upSampleMult) / 2;
    // int remainderSamples += (maxlen % upSampleMult) / 2;

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

            int maxSamplesThisRead = std::min( samplesLeftToRead, getMixerSamplesPerFrame() );
            if( maxSamplesThisRead != getMixerSamplesPerFrame() )
            {
                LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer reading %d samples", maxSamplesThisRead );
            }

            int samplesThisRead = mixerFrame.readMixerFrame( &data[ samplesRead * 2 * upSampleMult ], m_SpeakersMuted, maxSamplesThisRead, upSampleMult, m_PrevLastsample, m_PrevLastsample );
            if( !samplesThisRead )
            {
                // ran out of samples
                LogMsg( LOG_WARN, "AudioMixer::readDataFromMixer failed to read %d samples", samplesThisRead );
                break;
            }
            else
            {
                LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer samplesThisRead %d peak %d has silence %d", samplesThisRead,
                    AudioUtils::getPeakPcmAmplitude( (int16_t*)(&data[ samplesRead * 2 * upSampleMult ]), samplesThisRead * 2 ),
                    AudioUtils::hasSomeSilence( (int16_t*)(&data[ samplesRead * 2 * upSampleMult ]), samplesThisRead * 2 ) );
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

    if( samplesThatWillBeSilence )
    {
        // fill in at end of buffer
        int toFillLen = maxlen - (samplesRead * 2);
        memset( &data[ samplesRead * 2 ], 0, toFillLen );
        samplesRead += samplesThatWillBeSilence;
    }

    int hasSomSilence = AudioUtils::hasSomeSilence( (int16_t*)(data), maxlen );
    if( !m_WasReset && hasSomSilence )
    {
        LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer has %d samples of silence", hasSomSilence );
    }

    if( m_WasReset && samplesRead )
    {
        // started reading from mixer.. if do not have enough next read it will be a underrun condition
        m_WasReset = false;
        m_PartialFrameRead = 0;
    }

    m_MixerMutex.unlock();

    if( m_MixerVolume != 100.0f )
    {
        AudioUtils::applyPcmVolume( m_MixerVolume, (uint8_t*)data, maxlen );
    }

    
    int samplesRemainder = (maxlen % upSampleMult) / 2;
    if( samplesRemainder && samplesRead )
    {
        // not a multiple of what we can up sample
        // best we can do is replicate the last sample
        int16_t* pcmData = (int16_t*)&data[ ( samplesRead - 1 ) * 2 ];
        int16_t lastSample = *pcmData;
        for( int i = 0; i < samplesRemainder; i++ )
        {
            pcmData++;
            *pcmData = lastSample;
        }
    }

    m_AudioCallbacks.speakerAudioPlayed( m_AudioIoMgr.getAudioOutFormat(), (void*)data, maxlen );

    // to keep everything in mixer block size we say how many blocks would be read this time if available and keep track of the partial frame part to add to the next call
    int totalMixerLenRequested = m_PartialFrameRead + (maxlen / upSampleMult);
    int framesRead = totalMixerLenRequested / getMixerFrameSize();
    m_PartialFrameRead = totalMixerLenRequested % getMixerFrameSize();

    m_MixerThread.setMixerSpaceAvailable( framesRead * getMixerFrameSize() );
    m_MixerThread.releaseAudioMixerThread();

    // LogMsg( LOG_VERBOSE, "AudioMixer::readDataFromMixer read %d bytes peak amplitude %d", maxlen, AudioUtils::getPeakPcmAmplitude( (int16_t*)data, maxlen ) );
    
    return maxlen;
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