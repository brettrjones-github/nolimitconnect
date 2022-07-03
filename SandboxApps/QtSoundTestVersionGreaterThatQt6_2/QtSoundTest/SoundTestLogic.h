#ifndef SoundTestLOGIC_H
#define SoundTestLOGIC_H

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

#include <QVector3D>
#include <QMatrix4x4>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <QWindow>

#include "SoundTestThread.h"
#include "AudioIoMgr.h"
#include "AudioTestGenerator.h"
#include "WaveForm.h"

class SoundTestWidget;
class SoundTestThread;

class SoundTestLogic : public QWidget, public IAudioCallbacks
{
    Q_OBJECT
public:
    explicit SoundTestLogic( Waveform * waveForm, QWidget *parent );

    AudioIoMgr&                 getAudioIoMgr()             { return m_AudioIoMgr; }
    AudioTestGenerator*         getAudio100HzGenerator()    { return m_Test100HzGenerator.get(); }
    AudioTestGenerator *        getAudio400HzGenerator()    { return m_Test400HzGenerator.get(); }

    SoundTestThread *           getSoundTestThread()        { return m_SoundTestThread; }

    void						setSoundThreadShouldRun( bool shouldRun );
    void						setRenderWindowVisible( bool isVisible ) { m_RenderWindowVisible = isVisible; }

    bool                        getIsRenderInitialized() { return m_RenderInitialized; }

    void                        startStartSoundTestThread();
    void                        stopStartSoundTestThread();


    virtual bool                initSoundTestSystem();
    virtual bool                destroySoundTestSystem();

    virtual bool                beginSoundTest();
    virtual bool                endSoundTest();

    void                        lockRenderer() { m_renderMutex.lock(); }
    void                        unlockRenderer() { m_renderMutex.unlock(); }

    void                        aboutToDestroy();

    /// for visualization of audio output
    virtual void                speakerAudioPlayed( QAudioFormat& /*format*/, void * /*data*/, int /*dataLen*/ ) override;
    /// for visualization of audio input
    virtual void                microphoneAudioRecieved( QAudioFormat& /*format*/, void * /*data*/, int /*dataLen*/ ) override;

    /// Microphone sound capture with info for echo cancel ( 8000hz PCM 16 bit data, 80ms of sound )
    virtual void				fromGuiMicrophoneDataWithInfo( int16_t * pcmData, int pcmDataLenBytes, bool isSilence, int totalDelayTimeMs, int clockDrift ) override;
    /// Mute/Unmute microphone
    virtual void				fromGuiMuteMicrophone( bool muteMic ) override;
    /// Returns true if microphone is muted
    virtual bool				fromGuiIsMicrophoneMuted( void ) override;
    /// Mute/Unmute speaker
    virtual void				fromGuiMuteSpeaker(	bool muteSpeaker ) override;
    /// Returns true if speaker is muted
    virtual bool				fromGuiIsSpeakerMuted( void ) override;
    /// Enable/Disable echo cancellation
    virtual void				fromGuiEchoCancelEnable( bool enableEchoCancel ) override;
    /// Returns true if echo cancellation is enabled
    virtual bool				fromGuiIsEchoCancelEnabled( void ) override;
    /// Called when need more sound for speaker output
    virtual void				fromGuiAudioOutSpaceAvail( int freeSpaceLen ) override;
    /// speaker output has resumed
    virtual void				fromAudioOutResumed( void ) override;

    void                        playP2p400HzIfEnabled( int mixerSpaceAvailable = 0 );

    int                         getAudioInPeakAmplitude( void );
    int                         getAudioOutPeakAmplitude( void );

    void                        microphoneVolumeChanged( int volume );
    void                        speakerVolumeChanged( int volume );
    void                        voipVolumeChanged( int volume );

    // temp for testing conversions
    void                        generateAudioTone( QByteArray retAudioData, const QAudioFormat& format, qint64 durationUs, int toneHz );
    virtual int                 readGenerated4800HzMono100HzToneData( char* data, int maxlen ) override;
    int                         readGenerated8000HzMono160HzToneData( char* data, int maxlen, int16_t& peekNextSample ) override;

signals:
    void                        signalFrameRendered();

public slots:

    void                        pauseVoipState( int );
    void                        pauseKodiState( int );
    void                        pauseMicrophoneState( int state );
    void                        muteSpeakerState( int );
    void                        muteMicrophoneState( int );

private:
    Waveform *                  m_WaveForm;

    AudioIoMgr                  m_AudioIoMgr;

    SoundTestThread *			m_SoundTestThread = nullptr;
    bool                        m_PauseVoip{ true };
    bool                        m_PauseKodi{ true };
    bool                        m_PauseMicrophone{ true };

    bool                        m_RenderInitialized = false;
  
    QMutex                      m_renderMutex;

    bool                        m_exiting = false;

    bool                        m_RenderWindowVisible = false;

    QScopedPointer<AudioTestGenerator>   m_Test100HzGenerator;
    QScopedPointer<AudioTestGenerator>   m_Test400HzGenerator;
    QScopedPointer<AudioTestGenerator>   m_Test600HzKodiFloatGenerator;

    QScopedPointer<AudioTestGenerator>   m_Test8000HzMono160HzToneGenerator;
};

#endif // SoundTestLOGIC_H
