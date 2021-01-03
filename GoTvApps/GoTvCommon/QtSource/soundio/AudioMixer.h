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
// http://www.nolimitconnect.com
//============================================================================
#pragma once

#include "GoTvInterface/IDefs.h"
#include "AudioDefs.h"

#include <QMutex>
#include <QWidget>
#include <QElapsedTimer>

class AudioIoMgr;
class IAudioCallbacks;

class AudioMixer : public QWidget
{
    Q_OBJECT
public:
    explicit AudioMixer( AudioIoMgr& audioIoMgr, IAudioCallbacks& audioCallbacks, QWidget * parent );

    IAudioCallbacks&            getAudioCallbacks() { return m_AudioCallbacks; }


    void                        lockMixer() { m_MixerMutex.lock(); }
    void                        unlockMixer() { m_MixerMutex.unlock(); }

    void                        fromGuiMuteSpeaker( bool mute )         { m_SpeakersMuted = mute; }


    // add audio data to mixer.. assumes pcm signed short 2 channel 48000 Hz.. return total written to buffer
    virtual int				    enqueueAudioData( EAppModule appModule, int16_t * pu16PcmData, int pcmDataLenInBytes, bool isSilence );

    // read audio data from mixer.. assumes pcm 2 channel 48000 Hz
    qint64                      readDataFromMixer( char *data, qint64 maxlen );

    /// space available to que audio data into buffer
    int                         audioQueFreeSpace( EAppModule appModule );

    /// space used in audio que buffer
    int                         audioQueUsedSpace( EAppModule appModule );
 
    // get length of data buffered and ready for speaker out
    int                         getDataReadyForSpeakersLen();

signals:
    void                        signalCheckSpeakerOutState();
    void                        signalAvailableSpeakerBytesChanged( int availBytes );

private:
    // update indexes that applications write data into buffer at ( subtract data count read by speakers out )
    void                        updateReadBufferIndexes( int byteCnt );

    // update indexes that applications write data into buffer at ( add data count written to mixer )
    void                        updateWriteBufferIndex( EAppModule appModule, int byteCnt );

    // for test verification only
    void                        verifySpeakerSamples();

    AudioIoMgr&                 m_AudioIoMgr;
    IAudioCallbacks&            m_AudioCallbacks;
    QByteArray					m_AudioBuffer;
    QMutex                      m_MixerMutex;

    QAtomicInt                  m_BufIndex[ eMaxAppModule ];
    bool                        m_SpeakersMuted = false;
    QElapsedTimer               m_ElapsedTimer;
    QAtomicInt                  m_AtomicBufferSize;
 };
    
