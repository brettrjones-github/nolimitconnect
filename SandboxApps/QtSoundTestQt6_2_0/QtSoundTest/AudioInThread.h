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

#include <QWidget>
#include <QMutex>
#include <QThread>
#include <QSemaphore>
#include "AudioInThread.h"

class AudioIoMgr;
class AudioInIo;

class AudioInThread : public QThread
{
	Q_OBJECT
public:
    AudioInThread( AudioIoMgr& audioIoMgr, AudioInIo& audioInIo );

    void                        startAudioInThread();
    void                        stopAudioInThread();

	void						setThreadShouldRun(bool shouldRun)      { m_ShouldRun = shouldRun; }
    void                        releaseAudioInThread()                  { m_AudioSemaphore.release(); }
    void                        setDivideSamplesCount( int cnt )
    {
        m_DivideCnt = cnt;
    }

protected:
    virtual void                run() override;

    AudioIoMgr&                 m_AudioIoMgr;
    AudioInIo&                  m_AudioInIo;
    QSemaphore                  m_AudioSemaphore;

	bool						m_ShouldRun = true;
    int                         m_DivideCnt{ 1 };
    int                         m_AudioChunkRxedCnt{ 0 };
};
