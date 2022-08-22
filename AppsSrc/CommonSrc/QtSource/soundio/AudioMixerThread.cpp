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

#include "AudioMixerThread.h"
#include "AudioIoMgr.h"
#include "AudioDefs.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

//============================================================================
AudioMixerThread::AudioMixerThread( AudioIoMgr& audioIoMgr )
: QThread()
, m_AudioIoMgr( audioIoMgr )
, m_MyApp( audioIoMgr.getMyApp() )
{
}

//============================================================================
void AudioMixerThread::startAudioMixerThread()
{
    setThreadShouldRun( true );
    start();
}

//============================================================================
void AudioMixerThread::stopAudioMixerThread()
{   
    setThreadShouldRun( false );
    m_AudioSemaphore.release();
    wait();
    terminate();
}

//============================================================================
void AudioMixerThread::run()
{
    LogMsg( LOG_DEBUG, "AudioMixerThread thread start 0x%x", currentThreadId() );

    while( m_ShouldRun )
    {
        m_AudioSemaphore.acquire();
        if( m_ShouldRun )
        {
            while( m_MixerSpaceAvail >= MIXER_BUF_SIZE_8000_1_S16 )
            {
                m_AudioIoMgr.getAudioCallbacks().fromGuiAudioOutSpaceAvail( MIXER_BUF_SIZE_8000_1_S16 );
                m_MixerSpaceAvail -= MIXER_BUF_SIZE_8000_1_S16;
                if( m_MixerSpaceAvail >= MIXER_BUF_SIZE_8000_1_S16 )
                {
                    // because Qt reads of sound are sometimes larger than 80ms at a time we sometimes have multiple callback but 
                    // we reley on them to be approx 80ms appart so that buffers do not overrun/underrun
                    // sleep a bit and catch up later
                    VxSleep( 70 );
                }
            }   
        }
    }
 
    LogMsg( LOG_DEBUG, "AudioMixerThread thread done 0x%x", currentThreadId() );
}
