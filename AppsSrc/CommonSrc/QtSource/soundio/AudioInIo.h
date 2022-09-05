#pragma once
//============================================================================
// Copyright (C) 2018 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "AudioDefs.h"
#include "AudioBitrate.h"

#include <CoreLib/TimeIntervalEstimator.h>

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QAudioFormat>
#include <QAudioInput>
#include <QElapsedTimer>
#include <QIODevice>
#include <QMutex>
#include <QAudioSource>
#include <QMediaDevices>

class AudioIoMgr;
class AppCommon;
class P2PEngine;

class AudioInIo : public QIODevice
{
    Q_OBJECT
public:
    explicit AudioInIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent = 0 );
    ~AudioInIo() override = default;

    void						audioInShutdown( void )             { stopAudioIn(); }

    bool                        initAudioIn( QAudioFormat& audioFormat, const QAudioDevice& defaultDeviceInfo );
    bool                        soundInDeviceChanged( int deviceIndex );

    void                        stopAudio();
    void                        startAudio();

    void                        wantMicrophoneInput( bool enableInput );
    bool                        isMicrophoneInputWanted( void )     { return m_MicInputEnabled; }

    void                        setMicrophoneVolume( float volume ) { m_MicrophoneVolume = volume; }

    int                         getAudioInPeakAmplitude( void )     { return m_PeakAmplitude; }

    void                        lockAudioIn( void )                 { m_AudioBufMutex.lock(); }
    void                        unlockAudioIn( void )               { m_AudioBufMutex.unlock(); }
 
    void                        setMicrophoneVolume( int volume0to100 );

    void                        flush();

    char *                      getMicSilence()                     { return m_MicSilence; }

    void                        suspend()                           { if (m_AudioInputDevice.data() ) m_AudioInputDevice->suspend(); }
    void                        resume()                            { if (m_AudioInputDevice.data() ) m_AudioInputDevice->resume(); }

    QAudio::State               getState()                          { return (m_AudioInputDevice.data() ? m_AudioInputDevice->state() : QAudio::StoppedState); }
    QAudio::Error               getError()                          { return (m_AudioInputDevice.data() ? m_AudioInputDevice->error() : QAudio::NoError); }

    QAudioSource*               getAudioIn()                        { return m_AudioInputDevice.data(); }

    void                        setDivideSamplesCount( int cnt )    { m_DivideCnt = cnt; }
    int                         getDivideSamplesCount( void )       { return m_DivideCnt; }

    int                         getMicWriteDurationUs( void )       { return m_MicWriteDurationUs; }

    void                        setAudioTestState( EAudioTestState audioTestState );
    int64_t                     getAudioTestDetectTime( void )      { return m_AudioTestDetectTimeMs; }

    void						echoCancelSyncStateThreaded( bool inSync );

signals:
    void						signalCheckForBufferUnderun();

protected slots:
    void						slotCheckForBufferUnderun();
    void                        onAudioDeviceStateChanged( QAudio::State state );

protected:
    void                        startAudioIn( void );
    void                        stopAudioIn( void );

	qint64                      bytesAvailable() const override;

	qint64                      readData( char *data, qint64 maxlen ) override;
    qint64                      writeData( const char *data, qint64 len )  override;

    void                        audioTestDetectTestSound( int16_t* sampleInData, int inSampleCnt, int64_t& micWriteTime );

    AudioIoMgr&                 m_AudioIoMgr;
    AppCommon&                  m_MyApp;
    P2PEngine&                  m_Engine;
    QMutex&                     m_AudioBufMutex;

    bool                        m_initialized{ false };
    bool                        m_MicInputEnabled{ false };

    bool                        m_AudioInDeviceIsStarted{ false };
    QAudioFormat                m_AudioFormat;

    QAudio::State               m_AudioInState = QAudio::State::StoppedState;
    char                        m_MicSilence[ AUDIO_BUF_SIZE_8000_1_S16 ];
    int                         m_DivideCnt{ 1 };
    int                         m_PeakAmplitude{ 0 };
    float                       m_MicrophoneVolume{ 0.0f };

    QMediaDevices*              m_MediaDevices = nullptr;
    QScopedPointer<QAudioSource> m_AudioInputDevice;

    EAudioTestState             m_AudioTestState{ eAudioTestStateNone };
    int16_t                     m_AudioTestDetectTimeMs{ 0 };

    TimeIntervalEstimator       m_MicWriteTimeEstimator;
    int                         m_MicWriteDurationUs{ 0 };

    AudioBitrate                m_MicInBitrate;
    AudioBitrate                m_MicOutBitrate;
    bool                        m_EchoCancelInSync{ false };
};
