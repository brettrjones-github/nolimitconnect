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
// http://www.nolimitconnect.org
//============================================================================

#include <QtWidgets>
#include <QVideoFrame>
#include <QVideoFrameFormat>
#include <QVideoWidget>

class CamFrameGrabber : public QVideoWidget
{
    Q_OBJECT

public:
    CamFrameGrabber( QWidget* widget );

    void                         setFps( int fps );
    void                         enableGrab( bool enable );

    //QList<QVideoFrame::PixelFormat> supportedPixelFormats( QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle ) const;
    //bool isFormatSupported( const QVideoSurfaceFormat& format ) const;

    //bool start( const QVideoSurfaceFormat& format );

    //void                        stop();

    //virtual bool                present( const QVideoFrame& frame );

    //QRect                       videoRect() const { return targetRect; }
    //void                        updateVideoRect();

signals:
    void                        signalCamFrameAvailable( QPixmap& frame );

protected slots:
    void                        slotGrabFrame( void );

protected:
    // void                        paint( QPainter* painter ) override;

    QWidget*                    widget;
    QImage::Format              imageFormat;
    QRect                       targetRect;
    QSize                       imageSize;
    QRect                       sourceRect;
    QVideoFrame                 currentFrame;
    int                         m_Fps{ 15 };
    QElapsedTimer               m_ElapsedTimer;
    QTimer*                     m_GrabFrameTimer;
};
