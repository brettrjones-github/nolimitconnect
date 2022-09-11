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

#include "AudioMixerInterface.h"
#include "AudioMixerFrame.h"

#include "AudioBitrate.h"
#include "AudioDefs.h"
#include "AudioSampleBuf.h"
#include "AudioSpeakerBuf.h"

#include <QMutex>
#include <QWidget>
#include <QElapsedTimer>
#include <QAudioFormat>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>

class AppCommon;
class AudioIoMgr;
class IAudioCallbacks;
class AudioSampleBuf;

class AudioMixer : public QWidget, public AudioMixerInterface
{
    Q_OBJECT
public:
    explicit AudioMixer( AudioIoMgr& audioIoMgr, IAudioCallbacks& audioCallbacks, QWidget * parent );

    void                        shutdownAudioMixer( void );
    void                        resetMixer( void );

    void                        lockMixer( void )                       { m_MixerMutex.lock(); }
    void                        unlockMixer( void )                     { m_MixerMutex.unlock(); }

    IAudioCallbacks&            getAudioCallbacks()                     { return m_AudioCallbacks; }
    void                        lockAudioCallbacks()                    { m_AudioCallbackMutex.lock(); }
    void                        unlockAudioCallbacks()                  { m_AudioCallbackMutex.unlock(); }

    int                         getMixerFrameSize( void )               { return MIXER_CHUNK_LEN_BYTES; }
    int                         getMixerSamplesPerFrame( void )         { return MIXER_CHUNK_LEN_SAMPLES; }

    QAudioFormat&               getMixerFormat( void )                  { return m_MixerFormat; }


    virtual void                microphoneDeviceEnabled( bool isEnabled ) override;
    virtual void                speakerDeviceEnabled( bool isEnabled ) override;

    virtual qint64				readRequestFromSpeaker( char* data, qint64 maxlen ) override;

    virtual void				echoCancelSyncStateThreaded( bool inSync ) override;

    virtual void				frame80msElapsed( void ) override;
    // assumes 80 ms of pcm 8000hz mono audio
    virtual int				    toGuiAudioFrameThreaded( EAppModule appModule, int16_t* pcmData, bool isSilenceIn ) override;

    virtual void				fromGuiEchoCanceledSamplesThreaded( int16_t* pcmData, int sampleCnts, bool isSilence ) override;

    void                        fromGuiMuteSpeaker( bool mute )         { m_SpeakersMuted = mute; }
    void                        wantSpeakerOutput( bool enableOutput );

    void                        processAudioMixerThreaded( void );

protected:
    int                         incrementMixerWriteIndex( void );
    AudioMixerFrame&            getAudioWriteFrame( void )              { return m_MixerFrames[ m_MixerWriteIdx ]; };

    int                         incrementMixerReadIndex( void );
    AudioMixerFrame&            getAudioReadFrame( void )               { return m_MixerFrames[ m_MixerReadIdx ]; };

    void						processOutSpaceAvailableThreaded( void );

    //=== vars ===//
    AudioIoMgr&                 m_AudioIoMgr;
    AppCommon&                  m_MyApp;
    IAudioCallbacks&            m_AudioCallbacks;
    QMutex                      m_AudioCallbackMutex;

    QMutex                      m_MixerMutex;
    VxThread					m_ProcessAudioMixerThread;
    VxSemaphore					m_AudioMixerSemaphore;

    int16_t						m_MixerBuf[ MIXER_CHUNK_LEN_SAMPLES ];
    int16_t						m_QuietEchoBuf[ MIXER_CHUNK_LEN_SAMPLES * 8 ];

    bool                        m_SpeakersMuted{ false };
    bool                        m_WasReset{ true };

    int                         m_PrevLerpedSamplesCnt{ 0 };
    int16_t                     m_PrevLerpedSampleValue{ 0 };

    AudioMixerFrame             m_MixerFrames[ MAX_GUI_MIXER_FRAMES ];
    int                         m_MixerWriteIdx{ 0 };
    int                         m_MixerReadIdx{ 0 };

    QElapsedTimer               m_ElapsedTimer;
    QAudioFormat                m_MixerFormat;

    AudioSpeakerBuf				m_SpeakerProcessedBuf;
    AudioSpeakerBuf				m_EchoProcessedBuf;
    QMutex                      m_ProcessedBufMutex;

    AudioBitrate                m_EchoCanceledBitrate;
    AudioBitrate                m_ProcessFrameBitrate;
    AudioBitrate                m_ProcessSpeakerBitrate;
    AudioBitrate                m_SpeakerReadBitrate;
 };
    
