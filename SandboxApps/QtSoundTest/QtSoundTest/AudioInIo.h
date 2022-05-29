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
#include "AudioInThread.h"

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QIODevice>
#include <QMutex>


class AudioIoMgr;

class AudioInIo : public QIODevice
{
    Q_OBJECT
public:
    explicit AudioInIo( AudioIoMgr& mgr, QMutex& audioOutMutex, QObject *parent = 0 );
    ~AudioInIo() override;

    bool                        initAudioIn( QAudioFormat& audioFormat );

    void                        stopAudio();
    void                        startAudio();

    void                        lockAudioIn()       { m_AudioBufMutex.lock(); }
    void                        unlockAudioIn()     { m_AudioBufMutex.unlock(); }

    void                        suspend()           { if( m_AudioInputDevice ) m_AudioInputDevice->suspend(); }
    void                        resume()            { if( m_AudioInputDevice ) m_AudioInputDevice->resume(); }
 
    bool                        setAudioDevice( QAudioDeviceInfo deviceInfo );

    void                        setVolume( float volume );
    void                        flush();

    QAudio::State               getState()          { return ( m_AudioInputDevice ? m_AudioInputDevice->state() : QAudio::StoppedState); }
    QAudio::Error               getError()          { return ( m_AudioInputDevice ? m_AudioInputDevice->error() : QAudio::NoError); }

    QAudioInput *               getAudioIn()        { return m_AudioInputDevice; }
    QByteArray&					getAudioBuffer()    { return m_AudioBuffer; }
    char *                      getMicSilence()     { return m_MicSilence; }

    int                         getAtomicBufferSize() { return m_AtomicBufferSize; }
    void                        updateAtomicBufferSize() { m_AtomicBufferSize = m_AudioBuffer.size(); }

	/// space available to que audio data into buffer
	int							audioQueFreeSpace();

	/// space used in audio que buffer
	int							audioQueUsedSpace();

    int                         calculateMicrophonDelayMs();

signals:
    void						signalCheckForBufferUnderun();

protected slots:
    void                        slotAudioNotified();
    void						slotCheckForBufferUnderun();
    void                        onAudioDeviceStateChanged( QAudio::State state );

protected:
	qint64                      bytesAvailable() const override;

	qint64                      readData( char *data, qint64 maxlen ) override;
    qint64                      writeData( const char *data, qint64 len )  override;
    bool						isSequential() const  override { return true; }

private:
    void                        reinit();

    AudioIoMgr&                 m_AudioIoMgr;
    QMutex&                     m_AudioBufMutex;

    bool                        m_initialized = false;
    QAudioFormat                m_AudioFormat;
    QAudioDeviceInfo            m_deviceInfo;
    QAudioInput*                m_AudioInputDevice = nullptr;
    float                       m_volume = 1.0f;

    QByteArray					m_AudioBuffer;
    QAudio::State               m_AudioInState = QAudio::State::StoppedState;
    char                        m_MicSilence[ AUDIO_BUF_SIZE_8000_1_S16 ];
    AudioInThread               m_AudioInThread;
    QAtomicInt                  m_AtomicBufferSize;
};
