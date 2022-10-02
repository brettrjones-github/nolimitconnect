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


#include <QWidget>
#include <QIODevice>
#include <QByteArray>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <QMutex>
#include <QElapsedTimer>

# include <QAudioSink>

#include "AudioSampleBuf.h"
#include "AudioDefs.h"

#include <CoreLib/TimeIntervalEstimator.h>

class AppCommon;
class AudioIoMgr;
class QTimer;

class AudioOutIo : public QIODevice
{
    Q_OBJECT

public:
    explicit AudioOutIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent = 0 );
     ~AudioOutIo() override = default;

     void						audioOutShutdown( void )                        { stopAudioOut(); }

    bool                        initAudioOut( QAudioFormat& audioFormat, const QAudioDevice& defaultDeviceInfo );
    bool                        soundOutDeviceChanged( int deviceIndex );

    QAudioSink*                 getAudioOut( void )                             { return m_AudioOutputDevice.data(); }
    QAudio::State               getState( void )                                { return (m_AudioOutputDevice.data() ? m_AudioOutState : QAudio::StoppedState); }
    QAudio::Error               getError( void )                                { return (m_AudioOutputDevice.data() ? m_AudioOutputDevice->error() : QAudio::NoError); }

    int                         getAudioOutPeakAmplitude( void )                { return m_PeakAudioOutAmplitude; }

    void                        wantSpeakerOutput( bool enableOutput );
    bool                        isSpeakerOutputWanted( void )                   { return m_SpeakerOutputEnabled; }

    void                        flush( void );

    void                        suspend( void )                                 { if (m_AudioOutputDevice.data() ) m_AudioOutputDevice->suspend(); }
    void                        resume( void )                                  { if (m_AudioOutputDevice.data() ) m_AudioOutputDevice->resume(); }

    void                        setUpsampleMultiplier( int upSampleMult )       { m_UpsampleMutiplier = upSampleMult; }
    int                         getUpsampleMultiplier( void )                   { return m_UpsampleMutiplier; }

    void                        setSpeakerVolume( int volume0to100 );

    void                        setAudioTestState( EAudioTestState audioTestState );
    int64_t                     getAudioTestSentTime( void )                    { return m_AudioTestSentTimeMs; }

    AudioSampleBuf&             getSpeakerEchoSamples( int64_t& tailOfSpeakerSamplestimeMs ) { tailOfSpeakerSamplestimeMs = m_EndOfEchoBufferTimestamp; return m_EchoFarBuffer; }

    void						echoCancelSyncStateThreaded( bool inSync );

protected slots:
    void                        slotUpdateSpeakerOutputEnable( void );
    void                        onAudioDeviceStateChanged( QAudio::State state );

protected:
    void                        startAudioOut( void );
    void                        stopAudioOut( void );

	qint64                      readData( char *data, qint64 maxlen ) override;
    qint64                      writeData( const char*data, qint64 len )  override;
    qint64                      bytesAvailable( void ) const override;
    qint64                      size( void ) const override;

private:
    AudioIoMgr&                 m_AudioIoMgr;
    AppCommon&                  m_MyApp;
    QMutex&                     m_AudioBufMutex;

    bool                        m_initialized{ false };
    bool                        m_SpeakerOutputEnabled{ false };

    QAudioFormat                m_AudioFormat;
    
    qint64                      m_ProccessedMs = 0;
    QAudio::State               m_AudioOutState{ QAudio::State::StoppedState };
    QAudio::State               m_AudioOutPreviousState{ QAudio::State::StoppedState };
    QAudio::Error               m_AudioOutPreviousError{ QAudio::UnderrunError };
    QScopedPointer<QAudioSink>  m_AudioOutputDevice;

    int                         m_UpsampleMutiplier{ 0 };

    int16_t                     m_PrevLastsample{ 0 };
    int                         m_PrevLerpedSamplesCnt{ 0 };

    EAudioTestState             m_AudioTestState{ eAudioTestStateNone };
    int64_t                     m_AudioTestSentTimeMs{ 0 };
    int16_t*                    m_AudioTestSoundOutBuf{ nullptr };
    int                         m_AudioTestBufLen{ 0 };
    int                         m_AudioTestBufIdx{ 0 };

    TimeIntervalEstimator       m_SpeakerReadTimeEstimator;

    int64_t                     m_EndOfEchoBufferTimestamp{ 0 };
    AudioSampleBuf              m_EchoFarBuffer;

    int                         m_PeakAudioOutAmplitude{ 0 };
    QElapsedTimer               m_ElapsedOutTimer;
    QTimer*                     m_SpeakerOutputEnableTimer{ nullptr };
};
