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

#include "AudioOutThread.h"
#include "AudioIoMgr.h"
#include <QDebug>
#include "VxDebug.h"
#include "VxTime.h"
#include "AudioDefs.h"

//============================================================================
AudioOutThread::AudioOutThread( AudioIoMgr& audioIoMgr )
: QThread()
, m_AudioIoMgr( audioIoMgr )
{
}

//============================================================================
void AudioOutThread::startAudioOutThread()
{
    setThreadShouldRun( true );
    start();
}

//============================================================================
void AudioOutThread::stopAudioOutThread()
{   
    setThreadShouldRun( false );
    m_AudioSemaphore.release();
    wait();
    terminate();
}

//============================================================================
void AudioOutThread::run()
{
    LogMsg( LOG_DEBUG, "AudioOutThread thread start 0x%x", currentThreadId() );
    while( m_ShouldRun )
    {
        m_AudioSemaphore.acquire();
        if( m_ShouldRun )
        {
            m_AudioIoMgr.getAudioCallbacks().fromGuiAudioOutSpaceAvail( AUDIO_BUF_SIZE_8000_1_S16 );
        }
    }
 
    LogMsg( LOG_DEBUG, "AudioOutThread thread done 0x%x", currentThreadId() );
}
