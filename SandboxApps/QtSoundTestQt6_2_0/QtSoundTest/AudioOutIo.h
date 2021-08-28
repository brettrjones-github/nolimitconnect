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
// http://www.nolimitconnect.com
//============================================================================


#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QIODevice>
#include <QMutex>
#include <QElapsedTimer>

#include "VxTimer.h"
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
# include <QAudioSink>
#else
# include <QAudioDeviceInfo>
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#include "AudioOutThread.h"

class AudioIoMgr;
class QTimer;

class AudioOutIo : public QIODevice
{
    Q_OBJECT
public:
    explicit AudioOutIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent = 0 );
     ~AudioOutIo() override;

    bool                        initAudioOut( QAudioFormat& audioFormat );

    void                        stopAudio();
    void                        startAudio();

 

    void                        setVolume( float volume );
    void                        flush();

    QAudio::State               getState()      { return ( m_AudioOutputDevice ? m_AudioOutState : QAudio::StoppedState); }
    QAudio::Error               getError()      { return ( m_AudioOutputDevice ? m_AudioOutputDevice->error() : QAudio::NoError); }
    void                        suspend() { if (m_AudioOutputDevice) m_AudioOutputDevice->suspend(); }
    void                        resume() { if (m_AudioOutputDevice) m_AudioOutputDevice->resume(); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QAudioSink*                 getAudioOut() { return m_AudioOutputDevice; }
#else

    QAudioOutput *              getAudioOut( )  { return m_AudioOutputDevice; }
    bool                        setAudioDevice(QAudioDeviceInfo deviceInfo);
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

signals:
    void						signalCheckForBufferUnderun();

protected slots:
    void                        slotAudioNotify();
    void						slotCheckForBufferUnderun();
    void                        onAudioDeviceStateChanged( QAudio::State state );
    void                        slotAvailableSpeakerBytesChanged( int availBytes );

protected:

	qint64                      readData( char *data, qint64 maxlen ) override;
    qint64                      writeData( const char *data, qint64 len )  override;
    qint64                      size() const override;
    qint64                      bytesAvailable() const override;
    bool						isSequential() const  override { return false; } // if true then could not reposition data position

private:
    void                        reinit();

private:
    AudioIoMgr&                 m_AudioIoMgr;
    QMutex&                     m_AudioBufMutex;

    bool                        m_initialized = 0;
    QAudioFormat                m_AudioFormat;
    qint64                      m_ProccessedMs = 0;
    float                       m_volume = 1.0f;
    QTimer *                    m_PeriodicTimer;
    QElapsedTimer               m_ElapsedTimer;
    QAudio::State               m_AudioOutState{ QAudio::State::StoppedState };
    AudioOutThread              m_AudioOutThread;
    int                         m_AudioOutBufferSize = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QAudioSink*                 m_AudioOutputDevice = nullptr;
#else
    QAudioOutput*               m_AudioOutputDevice = nullptr;
    QAudioDeviceInfo            m_deviceInfo;
#endif // QT_VERSION >= QT_VERSION_CHECK(6,0,0)
};
