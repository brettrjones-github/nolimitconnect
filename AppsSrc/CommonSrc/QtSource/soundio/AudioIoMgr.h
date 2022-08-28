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

#include "AudioEchoCancel.h"
#include "AudioLoopback.h"
#include "AudioMasterClock.h"
#include "AudioMixer.h"
#include "AudioOutIo.h"
#include "AudioInIo.h"
#include <GuiInterface/IAudioInterface.h>

class AppCommon;

class AudioIoMgr : public QWidget, public IAudioRequests
{
    Q_OBJECT
public:
    explicit AudioIoMgr( AppCommon& app, IAudioCallbacks& audioCallbacks, QWidget * parent );

    ~AudioIoMgr() override = default;

    void                        audioIoSystemStartup();
    void                        audioIoSystemShutdown();

    AppCommon&                  getMyApp( void )                        { return m_MyApp; }
    P2PEngine&                  getEngine( void );
    QMediaDevices*              getMediaDevices( void )                 { return m_MediaDevices; }
    AudioEchoCancel&            getAudioEchoCancel( void )              { return m_AudioEchoCancel; }

    virtual int				    getMicrophonePeakValue0To100( void );

    virtual int				    fromGuiMicrophoneData( EAppModule appModule, int16_t* pu16PcmData, int pcmDataLenInBytes, bool isSilence );
    virtual void				fromMixerAvailablbleMixerSpace( int pcmMixerAvailableSpace );

    void                        setMicrophoneVolume( float volume )     { m_MicrophoneVolume = volume; }

    bool                        isAudioInitialized( void )              { return m_AudioIoInitialized;  }
    IAudioCallbacks&            getAudioCallbacks( void )               { return m_AudioCallbacks; }
    QAudioFormat&               getAudioOutFormat( void )               { return m_AudioOutFormat; }
    QAudioFormat&               getAudioInFormat( void )                { return m_AudioInFormat; }

    AudioMixer&                 getAudioOutMixer( void )                { return m_AudioOutMixer; }
    AudioInIo&                  getAudioInIo( void )                    { return m_AudioInIo; }
    AudioOutIo&                 getAudioOutIo( void )                   { return m_AudioOutIo; }

    AudioMasterClock&           getAudioMasterClock( void )             { return m_AudioMasterClock; }
    AudioLoopback&              getAudioLoopback( void )                { return m_AudioLoopback; }

    const char *                describeAudioState( QAudio::State state );
    const char *                describeAudioError( QAudio::Error err );

    void                        lockAudioIn()                           { m_AudioInMutex.lock(); }
    void                        unlockAudioIn()                         { m_AudioInMutex.unlock(); }

    // volume is from 0.0 to 1.0
    void						setSpeakerVolume( float volume0to1 );

    bool                        isMicrophoneAvailable( void )           { return m_MicrophoneAvailable; }
    bool                        isMicrophoneEnabled( void )             { return m_WantMicrophone; }
    bool                        isMicrophoneInputWanted( void )         { return m_WantMicrophone; }

    void                        setMuteMicrophone( bool mute );
    bool                        getIsMicrophoneMuted( void )            { return m_MicrophoneMuted; }

    bool                        isSpeakerAvailable( void )              { return m_SpeakerAvailable; }
    bool                        isSpeakerEnabled( void )                { return m_WantSpeakerOutput; }
    bool                        isSpeakerOutputWanted( void )           { return m_WantSpeakerOutput; }

    void                        setMuteSpeaker( bool mute );
    bool                        getIsSpeakerMuted()                     { return m_SpeakersMuted; }

    void                        setEchoCancelEnable( bool enable );
    bool                        getIsEchoCancelEnabled( void )          { return m_EchoCancelEnabled; }

    void                        setAudioLoopbackEnable( bool enable )   { m_AudioLoopbackEnabled = enable; }
    bool                        getAudioLoopbackEnable( void )          { return m_AudioLoopbackEnabled; }

    void                        setAudioTimingEnable( bool enable )     { m_AudioTimingEnabled = enable; }
    bool                        getAudioTimingEnable( void )            { return m_AudioTimingEnabled; }

    void                        setFrameTimingEnable( bool enable )     { m_FrameTimingEnabled = enable; }
    bool                        getFrameTimingEnable( void );

    void                        setPeakAmplitudeDebugEnable( bool enable );
    bool                        getPeakAmplitudeDebugEnable( void )         { return m_PeakAmplitudeDebug; }

    void                        setEchoCancelerNeedsReset( bool needReset );

    //=== IAudioRequests ===//
    // enable disable microphone data callback
    virtual void				toGuiWantMicrophoneRecording( EAppModule appModule, bool wantMicInput ) override;
    // enable disable sound out
    virtual void				toGuiWantSpeakerOutput( EAppModule appModule, bool wantSpeakerOutput ) override;
    // add audio data to play.. assumes pcm mono 8000 Hz of mixer buffer length
    virtual int				    toGuiPlayAudio( EAppModule appModule, int16_t* pu16PcmData, int pcmDataLenInBytes, bool isSilence ) override;
    // delay of audio calculated from amount of data in queue
    virtual double				toGuiGetAudioDelayMs( EAppModule appModule );
    // delay of audio calculated from amount of data in queue
    virtual double				toGuiGetAudioDelaySeconds( EAppModule appModule ) override;
    // maximum queue cache size in seconds
    virtual double				toGuiGetAudioCacheTotalMs( void ) override;
    // maximum queue cache size in seconds
    virtual double				toGuiGetAudioCacheTotalSeconds( EAppModule appModule ) override;
    // amount of free queue space in bytes
    virtual int				    toGuiGetAudioCacheFreeSpace( EAppModule appModule ) override;

    // get length of data ready for write to speakers
    int                         getDataReadyForSpeakersLen();
    // get length in milliseconds of data ready for write to speakers
    int                         getDataReadyForSpeakersMs();

    int                         getAudioOutCachedMaxLength() { return AUDIO_OUT_CACHE_USABLE_SIZE; }
    void                        resetLastSample( EAppModule appModule ) { m_MyLastAudioOutSample[ appModule ] = 0; }

    int                         getAudioInPeakAmplitude( void );
    int                         getAudioOutPeakAmplitude( void );

    bool                        setSoundInDeviceIndex( int sndInDeviceIndex );
    bool                        getSoundInDeviceIndex( int& retDeviceIndex );

    bool                        setSoundOutDeviceIndex( int sndOutDeviceIndex );
    bool                        getSoundOutDeviceIndex( int& retDeviceIndex );

    void                        getSoundInDevices( std::vector< std::pair<QString, QAudioDevice> >& retInDeviceList );
    void                        getSoundOutDevices( std::vector< std::pair<QString, QAudioDevice> >& retOutDeviceList );

    void                        soundInDeviceChanged( int deviceIndex );
    void                        soundOutDeviceChanged( int deviceIndex );

    bool                        runAudioDelayTest( void );

    void                        setEchoDelayMsParam( int delayMs )      { m_AudioEchoCancel.setEchoDelayMsParam( delayMs ); }

    void                        frame80msElapsed( void );

    void                        processAudioOutThreaded( void );

signals:
    void                        signalNeedMoreAudioData( int requiredLen );
    void                        signalAudioTestState( EAudioTestState audioTestState );
    void                        signalTestedSoundDelay( int echoDelayMs );
    void                        signalAudioTestMsg(QString audioTestMsg);

public slots:
    void                        speakerStateChanged( QAudio::State state );
    void                        microphoneStateChanged( QAudio::State state );

    void                        slotAudioTestTimer( void );

protected:
    void						stopAudioOut( );
    void                        stopAudioIn();
 
    int                         getCachedDataLength( EAppModule appModule );

    void                        aboutToDestroy();
    // update speakers to current mode and output
    void                        enableSpeakers( bool enable );
    // update microphone output
    void                        enableMicrophone( bool enable );

    void                        setAudioTestState( EAudioTestState audioTestState );
    bool                        handleAudioTestResult( int64_t soundOutTimeMs, int64_t soundDetectTimeMs );

    AppCommon&                  m_MyApp;
    QMediaDevices*              m_MediaDevices{ nullptr };

    IAudioCallbacks&            m_AudioCallbacks;

    bool                        m_AudioIoInitialized = false;

    bool                        m_MicrophoneAvailable = false;
    bool                        m_MicrophoneMuted = false;

    bool                        m_SpeakerAvailable = false;
    bool                        m_SpeakersMuted = false;

    bool                        m_EchoCancelEnabled = false;

    QMutex                      m_AudioOutMutex;
    QMutex                      m_AudioInMutex;

    AudioMixer                  m_AudioOutMixer;

    QAudioFormat                m_AudioOutFormat;
    AudioOutIo                  m_AudioOutIo;

    QAudioFormat                m_AudioInFormat;
    AudioInIo                   m_AudioInIo;

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
    float                       m_MicrophoneVolume{ 100.0f };

    AudioEchoCancel             m_AudioEchoCancel;

    QTimer*                     m_AudioTestTimer{ nullptr };
    EAudioTestState             m_AudioTestState{ eAudioTestStateNone };
    bool                        m_AudioTestMicEnable{ false };
    bool                        m_AudioTestSpeakerEnable{ false };
    int                         m_EchoDelayTestMaxInterations{ 3 };
    int                         m_EchoDelayCurrentInteration{ 0 };
    std::vector<int>            m_EchoDelayResultList;

    AudioMasterClock            m_AudioMasterClock;

    AudioLoopback               m_AudioLoopback;
    bool                        m_AudioLoopbackEnabled{ false };
    bool                        m_AudioTimingEnabled{ false };
    bool                        m_FrameTimingEnabled{ false };
    bool                        m_PeakAmplitudeDebug{ false };
};
