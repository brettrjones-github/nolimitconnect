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
#pragma once

#include "AudioMixerFrame.h"
#include "AudioMixerThread.h"

#include <QMutex>
#include <QWidget>
#include <QElapsedTimer>
#include <QAudioFormat>

class AudioIoMgr;
class IAudioCallbacks;

class AudioMixer : public QWidget
{
    Q_OBJECT
public:
    explicit AudioMixer( AudioIoMgr& audioIoMgr, IAudioCallbacks& audioCallbacks, QWidget * parent );

    IAudioCallbacks&            getAudioCallbacks()                     { return m_AudioCallbacks; }
    void                        lockAudioCallbacks()                    { m_AudioCallbackMutex.lock(); }
    void                        unlockAudioCallbacks()                  { m_AudioCallbackMutex.unlock(); }

    int                         getMixerFrameSize( void )               { return MIXER_BUF_SIZE_8000_1_S16; }
    int                         getMixerSamplesPerFrame( void )         { return MIXER_BUF_SIZE_8000_1_S16 / 2; }

    int                         getAudioOutPeakAmplitude( void )        { return m_PeakAmplitude; }

    QAudioFormat&               getMixerFormat( void )                  { return m_MixerFormat; }

    void                        lockMixer()                             { m_MixerMutex.lock(); }
    void                        unlockMixer()                           { m_MixerMutex.unlock(); }

    void                        shutdownAudioMixer( void );

    void                        fromGuiMuteSpeaker( bool mute )         { m_SpeakersMuted = mute; }
    void                        wantSpeakerOutput( bool enableOutput );

    // add audio data to mixer.. assumes pcm signed short mono channel 8000 Hz.. return total written to buffer
    virtual int				    toMixerPcm8000HzMonoChannel( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence );

    // read audio data from mixer.. assumes upSampleMult is multiplier to upsample pcm 1 channel 8000 Hz
    qint64                      readRequestFromSpeaker( char *data, qint64 maxlen, int upSampleMult );

    // read mono 8000Hz pcm audio data from mixer.. fill silence for underrun of data. return number of silenced samples
    int                         readDataFromMixer( int16_t* pcmRetBuf, int sampleCnt, int16_t& peekAtNextSample );

    void                        mixerWasReadByOutput( int readLen, int upSampleMult );
 
    // space available and not used by any application
    int                         getAudioMixerFreeSpace( bool mixerIsLocked = false );

    // get length of data buffered and ready for speaker out
    int                         getDataReadyForSpeakersLen( bool mixerIsLocked = false );

    // get length of data buffered and ready for speaker in total milliseconds of audio
    int                         getDataReadyForSpeakersMs( bool mixerIsLocked = false );

    /// space available to que audio data into buffer
    int                         audioQueFreeSpace( EAppModule appModule, bool mixerIsLocked = false );

    /// space used in audio que buffer
    int                         audioQueUsedSpace( EAppModule appModule, bool mixerIsLocked = false );

    int                         calcualateMixerBytesToMs( int bytesAudio8000Hz );

protected:
    int                         getModuleFrameIndex( EAppModule appModule )         { return m_ModuleBufIndex[ appModule ]; }
    void                        incrementModuleFrameIndex( EAppModule appModule )   { m_ModuleBufIndex[ appModule ]++; if( m_ModuleBufIndex[ appModule ] >= MAX_MIXER_FRAMES ) m_ModuleBufIndex[ appModule ] = 0; }
    int                         incrementMixerReadIndex( void ) { m_MixerReadIdx++; if( m_MixerReadIdx >= MAX_MIXER_FRAMES ) m_MixerReadIdx = 0; return m_MixerReadIdx;  }

    AudioIoMgr&                 m_AudioIoMgr;
    IAudioCallbacks&            m_AudioCallbacks;
    QMutex                      m_AudioCallbackMutex;

    AudioMixerThread            m_MixerThread;
    QMutex                      m_MixerMutex;

    AudioMixerFrame             m_MixerFrames[ MAX_MIXER_FRAMES ];
    int                         m_MixerReadIdx{ 0 };
    int                         m_ModuleBufIndex[ eMaxAppModule ];

    bool                        m_SpeakersMuted{ false };
    bool                        m_WasReset{ true };
    int                         m_AudioOutRead{ 0 };
    int                         m_PeakAmplitude{ 0 };

    int                         m_PrevLerpedSamplesCnt{ 0 };
    int16_t                     m_PrevLerpedSampleValue{ 0 };

    QElapsedTimer               m_ElapsedTimer;
    QAudioFormat                m_MixerFormat;
 };
    
