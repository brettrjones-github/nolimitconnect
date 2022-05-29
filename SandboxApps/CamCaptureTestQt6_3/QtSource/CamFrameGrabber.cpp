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

#include "CamFrameGrabber.h"

//============================================================================
CamFrameGrabber::CamFrameGrabber( QWidget* widget )
    : QVideoWidget( widget )
    , widget( widget )
    , imageFormat( QImage::Format_Invalid )
    , m_GrabFrameTimer( new QTimer( this ))
{
    connect( m_GrabFrameTimer, SIGNAL( timeout() ), this, SLOT( slotGrabFrame() ) );
    setFps( m_Fps * 2 );
    enableGrab( true );
}

//============================================================================
void CamFrameGrabber::setFps( int fps )
{
    m_Fps = fps;
    m_GrabFrameTimer->setInterval( 1000 / m_Fps );
}

//============================================================================
void CamFrameGrabber::enableGrab( bool enable )
{
    if( enable )
    {
        m_GrabFrameTimer->setInterval( 1000 / m_Fps );
        m_GrabFrameTimer->start();
    }
    else
    {
        m_GrabFrameTimer->stop();
    }
}

//============================================================================
void CamFrameGrabber::slotGrabFrame( void )
{
    QPixmap pixmap = grab();
    qDebug() << "grab frame elapsed " << m_ElapsedTimer.elapsed() << " valid ? " << !pixmap.isNull();
    m_ElapsedTimer.start();
    if( !pixmap.isNull() )
    {
        emit signalCamFrameAvailable( pixmap );
    }
}

/*
QList<QVideoFrame::PixelFormat> CamFrameGrabber::supportedPixelFormats( QAbstractVideoBuffer::HandleType handleType ) const
{
    Q_UNUSED( handleType );
    return QList<QVideoFrame::PixelFormat>()
        << QVideoFrame::Format_ARGB32
        << QVideoFrame::Format_ARGB32_Premultiplied
        << QVideoFrame::Format_RGB32
        << QVideoFrame::Format_RGB24
        << QVideoFrame::Format_RGB565
        << QVideoFrame::Format_RGB555
        << QVideoFrame::Format_ARGB8565_Premultiplied
        << QVideoFrame::Format_BGRA32
        << QVideoFrame::Format_BGRA32_Premultiplied
        << QVideoFrame::Format_BGR32
        << QVideoFrame::Format_BGR24
        << QVideoFrame::Format_BGR565
        << QVideoFrame::Format_BGR555
        << QVideoFrame::Format_BGRA5658_Premultiplied
        << QVideoFrame::Format_AYUV444
        << QVideoFrame::Format_AYUV444_Premultiplied
        << QVideoFrame::Format_YUV444
        << QVideoFrame::Format_YUV420P
        << QVideoFrame::Format_YV12
        << QVideoFrame::Format_UYVY
        << QVideoFrame::Format_YUYV
        << QVideoFrame::Format_NV12
        << QVideoFrame::Format_NV21
        << QVideoFrame::Format_IMC1
        << QVideoFrame::Format_IMC2
        << QVideoFrame::Format_IMC3
        << QVideoFrame::Format_IMC4
        << QVideoFrame::Format_Y8
        << QVideoFrame::Format_Y16
        << QVideoFrame::Format_Jpeg
        << QVideoFrame::Format_CameraRaw
        << QVideoFrame::Format_AdobeDng;
}

bool CamFrameGrabber::isFormatSupported( const QVideoSurfaceFormat& format ) const
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat( format.pixelFormat() );
    const QSize size = format.frameSize();

    return imageFormat != QImage::Format_Invalid
        && !size.isEmpty()
        && format.handleType() == QAbstractVideoBuffer::NoHandle;
}

bool CamFrameGrabber::start( const QVideoSurfaceFormat& format )
{
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat( format.pixelFormat() );
    const QSize size = format.frameSize();

    if( imageFormat != QImage::Format_Invalid && !size.isEmpty() ) {
        this->imageFormat = imageFormat;
        imageSize = size;
        sourceRect = format.viewport();

        QAbstractVideoSurface::start( format );

        widget->updateGeometry();
        updateVideoRect();

        return true;
    }
    else {
        return false;
    }
}

void CamFrameGrabber::stop()
{
    currentFrame = QVideoFrame();
    targetRect = QRect();

    //QAbstractVideoSurface::stop();

    widget->update();
}

bool CamFrameGrabber::present( const QVideoFrame& frame )
{
   
    if( frame.isValid() )
    {
        QVideoFrame cloneFrame( frame );
        cloneFrame.map( QAbstractVideoBuffer::ReadOnly );
        const QImage image( cloneFrame.bits(),
            cloneFrame.width(),
            cloneFrame.height(),
            QVideoFrame::imageFormatFromPixelFormat( cloneFrame.pixelFormat() ) );
        emit frameAvailable( image ); // this is very important
        cloneFrame.unmap();
    }

    if( surfaceFormat().pixelFormat() != frame.pixelFormat()
        || surfaceFormat().frameSize() != frame.size() ) {
        setError( IncorrectFormatError );
        stop();

        return false;
    }
    else {
        currentFrame = frame;

        widget->repaint( targetRect );

        return true;
    }
    
    return true;
}

void CamFrameGrabber::updateVideoRect()
{
    
    QSize size = surfaceFormat().sizeHint();
    size.scale( widget->size().boundedTo( size ), Qt::KeepAspectRatio );

    targetRect = QRect( QPoint( 0, 0 ), size );
    targetRect.moveCenter( widget->rect().center() );
    
}

void CamFrameGrabber::paint( QPainter* painter )
{
    
    if( currentFrame.map( QAbstractVideoBuffer::ReadOnly ) ) {
        const QTransform oldTransform = painter->transform();

        if( surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop ) {
            painter->scale( 1, -1 );
            painter->translate( 0, -widget->height() );
        }

        QImage image(
            currentFrame.bits(),
            currentFrame.width(),
            currentFrame.height(),
            currentFrame.bytesPerLine(),
            imageFormat );

        painter->drawImage( targetRect, image, sourceRect );

        painter->setTransform( oldTransform );

        currentFrame.unmap();
    }
    
}
*/
