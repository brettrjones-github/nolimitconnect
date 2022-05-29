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
// http://www.nolimitconnect.org
//============================================================================

#include "VideoSinkGrabber.h"
#include "GuiParams.h"

#include <QDebug>

//============================================================================
VideoSinkGrabber::VideoSinkGrabber( QObject* widget )
    : QVideoSink( widget )
{
    connect( this, SIGNAL( videoFrameChanged( const QVideoFrame& ) ), this, SLOT( slotVideoFrameChanged( const QVideoFrame& ) ) );
    enableGrab( true );
}

//============================================================================
void VideoSinkGrabber::setFps( int fps )
{
    if( fps > 0 )
    {
        m_Fps = fps;
        m_MinFrameIntervalMs = 1000 / m_Fps;
    }
}

//============================================================================
void VideoSinkGrabber::enableGrab( bool enable )
{
    m_DesiredFrameSize = GuiParams::getSnapshotDesiredSize();
    m_availFrames.clear();
    m_GrabEnabled = enable;  
    m_ElapsedTimer.start();
}

//============================================================================
void VideoSinkGrabber::slotVideoFrameChanged( const QVideoFrame& frame )
{
    // int64_t elapsedMs = m_ElapsedTimer.elapsed();
    // qDebug() << "slotVideoFrameChanged elapsed " << elapsedMs;
    m_ElapsedTimer.start();
    static int frameNum = 0;
    frameNum++;

    if( m_availFrames.empty() )
    {
        QImage frameImage = frame.toImage();
        lockGrabberQueue();
        m_availFrames.push_back( std::make_pair( m_DesiredFrameSize == frameImage.size() ? frameImage : frameImage.scaled( m_DesiredFrameSize ), frameNum ) );
        unlockGrabberQueue();
    }


    /*

    // do toImage every time because if is not done every frame on android then it will stop working
    // toImage is broken on Android version 11 on Galaxy Tab A7 model SM-T500
    // QTBUG-99135 Qt 6.2.2 QVideoFrame.toImage unsupported format or has no image.. just white color
    QImage image = frame.toImage();
    if( elapsedMs >= m_MinFrameIntervalMs && !image.isNull() )
    {
        static int frameNum = 0;
        frameNum++;
        qDebug() << "slotVideoFrameChanged valid frame  w " << image.width() << " h " << image.height() << " f " << frameNum << " elapsed " << elapsedMs;
        m_ElapsedTimer.start();
        emit signalSinkFrameAvailable( frameNum, image );
    }
    */
}
