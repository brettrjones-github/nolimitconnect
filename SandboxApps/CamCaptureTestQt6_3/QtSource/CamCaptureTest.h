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

#include <QDialog>
#if defined (Q_OS_WIN)
# include <Winsock2.h> // for stupid redef struct when compiling on windows
#endif // defined (Q_OS_WIN)

#include "ui_CamCaptureTest.h"

#include "CamLogic.h"



class CamCaptureTest : public QDialog
{
	Q_OBJECT
public:
    const QSize CAM_FRAME_SIZE = QSize(320, 240);
	CamCaptureTest( QWidget * parent = NULL );
	virtual ~CamCaptureTest() override;

    CamLogic&                   getCamLogic( void ) { return m_CamLogic; }
    CamCaptureTest&             getEngine( void ) { return *this; }
    int                         getCamCaptureRotation( void ) { return 0; }

    /// Called when raw camera capture video data is available ( u32FourCc is the video data format )
    virtual void				fromGuiVideoData( uint32_t u32FourCc, uint8_t* pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation );

signals:
    void						signalSnapshotImage( QImage snapshotImage );

public slots:
    void						onSnapShotButClick( void );
    void						onCancelButClick( void );
    void                        onDoneButClick( void );
    void                        onCamFrontBackButClick( void );
    void                        onCamStopStartButClick( void );

    void                        slotCameraDescription( QString camDescription );
    void                        slotSinkFrameAvailable( int frameNum, QImage& image );

protected:


    Ui::CamCaptureTestUi		ui;
    bool 					    m_CameraSourceAvail = false;
    bool 					    m_SnapShotPending = false;
    QImage	                    m_ImageBitmap;
    CamLogic                    m_CamLogic;
};
