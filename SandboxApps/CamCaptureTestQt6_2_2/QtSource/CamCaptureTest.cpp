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

#include "CamCaptureTest.h"
//#include "CamFrameGrabber.h"

#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#include <QResizeEvent>
#include <QMessageBox>
#include <QTimer>

//============================================================================
CamCaptureTest::CamCaptureTest( QWidget * parent )
: QDialog( parent )
, m_CamLogic( *this )
{
    ui.setupUi( this );

    ui.m_ImageScreen->setFixedSize( GuiParams::getSnapshotScaledSize() );
    ui.m_SnapshotScreen->setFixedSize( GuiParams::getSnapshotScaledSize() );

    // in order for capture to work on android the parent of VideoSinkGrabber must be a application parented widget
    m_CamLogic.setVideoPreviewWidget( ui.m_VideoPreviewWidget );
    m_CamLogic.getVideoSinkGrabber().setParent( ui.m_ImageScreen );

    connect( ui.snapshotButton, SIGNAL( clicked() ), this, SLOT( onSnapShotButClick() ) );
    connect( ui.m_DoneButton, SIGNAL( clicked() ), this, SLOT( onDoneButClick() ) );
    connect( ui.m_CamFrontBackButton, SIGNAL( clicked() ), this, SLOT( onCamFrontBackButClick() ) );
    connect( ui.m_CamStopStartButton, SIGNAL( clicked() ), this, SLOT( onCamStopStartButClick() ) );

    connect( &getCamLogic(), SIGNAL( signalCameraDescription(QString) ), this, SLOT( slotCameraDescription( QString ) ) );
    connect( &getCamLogic().getVideoSinkGrabber(), SIGNAL( signalSinkFrameAvailable( int, QImage & ) ), this, SLOT( slotSinkFrameAvailable( int, QImage & ) ) );


    ui.m_CamNameLabel->setText( getCamLogic().getCamDescription() );
   
    m_CamLogic.cameraEnable( true );
}

//============================================================================
CamCaptureTest::~CamCaptureTest()
{
}

//============================================================================
//! take picture for me
void CamCaptureTest::onSnapShotButClick( void )
{
    m_SnapShotPending = true;
}

//============================================================================
//! Implement the OnClickListener callback    
void CamCaptureTest::onCancelButClick( void )
{
}

//============================================================================ 
void CamCaptureTest::onDoneButClick( void )
{
    close();
}

//============================================================================ 
void CamCaptureTest::onCamFrontBackButClick( void )
{
    getCamLogic().nextCamera();
}

//============================================================================ 
void CamCaptureTest::onCamStopStartButClick( void )
{
    getCamLogic().cameraEnable( !getCamLogic().isCamStarted() );
}

//============================================================================
void CamCaptureTest::slotCameraDescription( QString camDescription )
{
    ui.m_CamNameLabel->setText( camDescription );
}

//============================================================================
void CamCaptureTest::fromGuiVideoData( uint32_t u32FourCc, uint8_t* pu8VidDataIn, int iWidth, int iHeight, uint32_t u32VidDataLen, int iRotation )
{  
    ui.m_ImageScreen->playVideoFrame( pu8VidDataIn,  u32VidDataLen, iWidth, iHeight );
}

//============================================================================
void CamCaptureTest::slotSinkFrameAvailable( int frameNum, QImage& pixmap )
{
    if( !pixmap.isNull() )
    {
        LogMsg( LOG_WARNING, "CamCaptureTest::slotSinkFrameAvailable pixmap w %d h %d", pixmap.width(), pixmap.height() );
        ui.m_SnapshotScreen->showScaledImage( pixmap );
    }
    else
    {
        LogMsg( LOG_WARNING, "CamCaptureTest::slotSinkFrameAvailable null pixmap" );
    }
}
