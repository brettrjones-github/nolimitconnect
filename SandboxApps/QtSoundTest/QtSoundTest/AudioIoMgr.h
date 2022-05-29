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

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <QWidget>


#include "AudioMixer.h"
#include "AudioOutIo.h"
#include "AudioInIo.h"
#include "IAudioInterface.h"


class AudioIoMgr : public QWidget, public IAudioRequests
{
    Q_OBJECT
public:
    explicit AudioIoMgr( IAudioCallbacks& audioCallbacks, QWidget * parent );

    ~AudioIoMgr() override = default;

    void                        initAudioIoSystem();
    void                        destroyAudioIoSystem();

    bool                        isAudioInitialized()       { return m_AudioIoInitialized;  }
    IAudioCallbacks&            getAudioCallbacks()        { return m_AudioCallbacks; }
    QAudioFormat&               getAudioOutFormat()        { return m_AudioOutFormat; }
    QAudioFormat&               getAudioInFormat()         { return m_AudioInFormat; }
    AudioMixer&                 getAudioOutMixer()         { return m_AudioOutMixer; }

    const char *                describeAudioState( QAudio::State state );
    const char *                describeAudioError( QAudio::Error err );

    void                        lockAudioIn() { m_AudioInMutex.lock(); }
    void                        unlockAudioIn() { m_AudioInMutex.unlock(); }

    // volume is from 0.0 to 1.0
    void						setVolume( float volume );

    void                        pauseAudioOut();
    void                        resumeAudioOut();

    bool                        isMicrophoneAvailable()                 { return m_MicrophoneAvailable; }
    bool                        isMicrophoneEnabled()                   { return m_MicrophoneEnabled; }

    void                        fromGuiMuteMicrophone( bool mute );
    bool                        fromGuiIsMicrophoneMuted()              { return m_MicrophoneMuted; }
    void                        fromGuiMuteSpeaker( bool mute );
    bool                        fromGuiIsSpeakerMuted()                 { return m_SpeakersMuted; }
    void                        fromGuiEchoCancelEnable( bool enable )  { m_EchoCancelEnabled = enable; }
    bool                        fromGuiIsEchoCancelEnabled()            { return m_EchoCancelEnabled; }

    //=== IAudioRequests ===//
    // enable disable microphone data callback
    virtual void				toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput ) override;
    // enable disable sound out
    virtual void				toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput ) override;
    // add audio data to play.. assumes float 2 channel 48000 Hz so convert float to s16 pcm data before calling writeData
    virtual int				    toGuiPlayAudio( EAppModule appModule, float * audioSamples48000, int dataLenInBytes ) override;
    // add audio data to play.. assumes pcm mono 8000 Hz so convert to 2 channel 48000 hz pcm before calling writeData
    virtual int				    toGuiPlayAudio( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence ) override;
    // delay of audio calculated from amount of data in queue
    virtual double				toGuiGetAudioDelayMs( EAppModule appModule );
    // delay of audio calculated from amount of data in queue
    virtual double				toGuiGetAudioDelaySeconds( EAppModule appModule ) override;
    // maximum queue cache size in seconds
    virtual double				toGuiGetAudioCacheTotalSeconds( EAppModule appModule ) override;
    // amount of free queue space in bytes
    virtual int				    toGuiGetAudioCacheFreeSpace( EAppModule appModule ) override;

    // read speaker output from mixer
    qint64                      readDataFromOutMixer( char * data, qint64 maxlen );

    // get length of data ready for write to speakers
    int                         getDataReadyForSpeakersLen();

    int                         getCachedMaxLength() { return AUDIO_OUT_CACHE_USABLE_SIZE; }
    void                        resetLastSample( EAppModule appModule ) { m_MyLastAudioOutSample[ appModule ] = 0; }

signals:
    void                        signalNeedMoreAudioData( int requiredLen );

public slots:
    void                        speakerStateChanged( QAudio::State state );
    void                        microphoneStateChanged( QAudio::State state );

protected:
    void						stopAudioOut( );
    void                        stopAudioIn();
 
    int                         getCachedDataLength( EAppModule appModule );

    void                        aboutToDestroy();

    // update speakers to current mode and output
    void                        updateSpeakers();
    // update microphone output
    void                        updateMicrophone();

private:
    IAudioCallbacks&            m_AudioCallbacks;

    bool                        m_AudioIoInitialized = false;
    bool                        m_MicrophoneAvailable = false;
    bool                        m_MicrophoneEnabled = false;
    bool                        m_MicrophoneMuted = false;
    bool                        m_SpeakerAvailable = false;
    bool                        m_SpeakersMuted = false;
    bool                        m_EchoCancelEnabled = false;

    QMutex                      m_AudioOutMutex;
    QMutex                      m_AudioInMutex;

    AudioMixer                  m_AudioOutMixer;

    QAudioFormat                m_AudioOutFormat;
    AudioOutIo                  m_AudioOutIo;
    QAudioDeviceInfo            m_AudioOutDeviceInfo;

    QAudioFormat                m_AudioInFormat;
    AudioInIo                   m_AudioInIo;
    QAudioDeviceInfo            m_AudioInDeviceInfo;

    bool                        m_IsOutPaused = false;
    bool                        m_IsTestMode = true;
    int                         m_CacheAuidioLen = 0;
    QMutex                      m_AudioQueueMutex;
    int                         m_OutWriteCount = 0;

    int16_t                     m_MyLastAudioOutSample[ eMaxAppModule ];
    bool                        m_MicrophoneEnable[ eMaxAppModule ];
    bool                        m_SpeakerEnable[ eMaxAppModule ];

    bool                        m_WantMicrophone = false;
    bool                        m_WantSpeakerOutput = false;
};
