#pragma once
//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "config_apps.h"

#include <QVideoFrame>
#include <QElapsedTimer>
#include <QVideoSink>

class VideoSinkGrabber : public QVideoSink
{
    Q_OBJECT

public:
    VideoSinkGrabber( QObject* widget );

    void                         setFps( int fps );
    void                         enableGrab( bool enable );

signals:
    void                        signalSinkFrameAvailable( QImage& frame );

protected slots:
    void                        slotVideoFrameChanged( const QVideoFrame& frame );

protected:
    int                         m_Fps{ 15 };
    int64_t                     m_MinFrameIntervalMs{ 1000 / 15 };
    bool                        m_GrabEnabled{ true };
    QElapsedTimer               m_ElapsedTimer;
};
