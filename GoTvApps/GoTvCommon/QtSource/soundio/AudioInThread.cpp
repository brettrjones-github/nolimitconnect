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

#include <app_precompiled_hdr.h>
#include "AudioInThread.h"
#include "AudioIoMgr.h"
#include <QDebug>
#include "AudioDefs.h"

//============================================================================
AudioInThread::AudioInThread( AudioIoMgr& audioIoMgr, AudioInIo& audioInIo )
: QThread()
, m_AudioIoMgr( audioIoMgr )
, m_AudioInIo( audioInIo )
{
}

//============================================================================
void AudioInThread::startAudioInThread()
{
    setThreadShouldRun( true );
    start();
}

//============================================================================
void AudioInThread::stopAudioInThread()
{   
    setThreadShouldRun( false );
    m_AudioSemaphore.release();
    wait();
    terminate();
}

//============================================================================
void AudioInThread::run()
{
    LogMsg( LOG_DEBUG, "AudioInThread thread start 0x%x", VxGetCurrentThreadId() );
    while( m_ShouldRun )
    {
        m_AudioSemaphore.acquire();
        if( m_ShouldRun )
        {
            m_AudioInIo.lockAudioIn();
            qint64 audioByteLen = m_AudioInIo.getAtomicBufferSize();
            if( audioByteLen >= AUDIO_BUF_SIZE_8000_1_S16 )
            {
                IAudioCallbacks& audioCallbacks = m_AudioIoMgr.getAudioCallbacks();
                while( audioByteLen >= AUDIO_BUF_SIZE_8000_1_S16 )
                {
                    m_AudioChunkRxedCnt++;
                    if( m_AudioChunkRxedCnt == m_DivideCnt )
                    {
                        m_AudioChunkRxedCnt = 0;
                        if( m_AudioIoMgr.fromGuiIsMicrophoneMuted() )
                        {
                            audioCallbacks.fromGuiMicrophoneDataWithInfo( (int16_t*)m_AudioInIo.getMicSilence(), AUDIO_BUF_SIZE_8000_1_S16, true, m_AudioInIo.calculateMicrophonDelayMs(), 0 );
                        }
                        else
                        {
                            audioCallbacks.fromGuiMicrophoneDataWithInfo( (int16_t*)m_AudioInIo.getAudioBuffer().data(), AUDIO_BUF_SIZE_8000_1_S16, false, m_AudioInIo.calculateMicrophonDelayMs(), 0 );
                        }
                    }

                    m_AudioInIo.getAudioBuffer().remove( 0, AUDIO_BUF_SIZE_8000_1_S16 ); //pop front what is written
                    m_AudioInIo.updateAtomicBufferSize();
                    audioByteLen -= AUDIO_BUF_SIZE_8000_1_S16;
                }
            }

            m_AudioInIo.unlockAudioIn();
        }
    }
 
    LogMsg( LOG_DEBUG, "AudioInThread done thread 0x%x", VxGetCurrentThreadId() );
}
