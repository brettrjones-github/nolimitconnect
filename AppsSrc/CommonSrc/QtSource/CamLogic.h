#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>

#include "VideoSinkGrabber.h"

#include <QTimer>
#include <QKeyEvent>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QPalette>
#include <QtWidgets>
#include <QMediaRecorder>

# include <QCamera>
# include <QCameraDevice>
# include <QCameraFormat>
# include <QImageCapture>
# include <QMediaCaptureSession>

class AppCommon;
class QVideoWidget;

class CamLogic : public QWidget
{
    Q_OBJECT

public:
    const int CAM_SNAPSHOT_INTERVAL_MS = 60;

    CamLogic( AppCommon& myApp );
    virtual ~CamLogic() = default;

    void                        cameraEnable( bool wantVidCapture );
    bool                        assureCamInitiated( void );
    bool                        isCamCaptureRunning( void );
    bool                        isCamAvailable( void );
    bool                        updateCamAvailable( void ); // recheck devices for available cam

    // set application is exiting.. return true if cam is busy with capture
    bool                        setAppIsExiting( bool isExiting );
    bool                        getAppIsExiting( void )                                 { return m_applicationExiting; }

    /// TODO implement option to select cam hardware
    void						setCamSourceId( uint32_t camId )                        { m_CamId = camId; }
    uint32_t					getCamSourceId( void )                                  { return m_CamId; }

    void						setCamShowPreview( bool showPreview )                   { m_ShowPreview = showPreview; };
    bool						getCamShowPreview( void )                               { return m_ShowPreview; }

    void						setCamRotation( uint32_t camId, uint32_t camRotation )  { m_CamRotation = camRotation; };
    uint32_t					getCamRotation( uint32_t camId )                        { return m_CamRotation; }

    void						setVidFeedRotation( uint32_t feedRotation )             { m_FeedRotation = feedRotation; };
    uint32_t					getVidFeedRotation( void )                              { return m_FeedRotation; }

    void                        toGuiWantVideoCapture( EAppModule appModule, bool wantVidCapture );

    QString                     getCamDescription( void )                               { return m_CamDescription; }

signals:
    void                        signalCameraDescription( QString camDescription );

public slots:

    void                        setCamera( const QCameraDevice& cameraDevice );

    void                        startCamera();
    void                        stopCamera();

    void                        nextCamera( void );

    void                        record();
    void                        pause();
    void                        stop();
    void                        setMuted( bool );

    void                        toggleLock();
 
    void                        configureCaptureSettings();
    void                        configureVideoSettings();
    void                        configureImageSettings();

    void                        displayRecorderError();
    void                        displayCameraError();

    void                        updateCameraDevice( QAction *action );

    void                        updateCaptureMode( int );
    void                        setExposureCompensation( int index );

    void                        updateRecordTime();

    void                        processCapturedImage( int requestId, const QImage &img );

    void                        updateCameraActive( bool active );
    void                        updateRecorderState( QMediaRecorder::RecorderState state );
    void                        displayCaptureError( int, QImageCapture::Error, const QString& errorString );

    void                        displayViewfinder();
    void                        displayCapturedImage();

    void                        readyForCapture( bool ready );
    void                        imageSaved( int id, const QString &fileName );

    void                        slotTakeSnapshot( void );

protected:
    void                        keyPressEvent( QKeyEvent *event ) override;
    void                        keyReleaseEvent( QKeyEvent *event ) override;
    void                        closeEvent( QCloseEvent *event ) override;

    void                        selectVideoFormat( const QCameraDevice& cameraDevice );

    AppCommon&                  m_MyApp;
    uint32_t                    m_CamId{ 1 };
    bool                        m_CamsEnumerated{ false };
    bool                        m_CamInitiated{ false };
    bool                        m_ShowPreview{ false };
    uint32_t                    m_CamRotation{ 0 };
    uint32_t                    m_FeedRotation{ 0 };

    QTimer *                    m_SnapshotTimer{ nullptr };

    QScopedPointer<QCamera>     m_camera;

    QString                     m_videoContainerFormat;
    bool                        m_isCapturingImage{ false };
    bool                        m_applicationExiting{ false };
    bool                        m_CamIsStarted{ false };
    bool                        m_ReadyForCapture{ false };

    QImageCapture*              m_imageCapture;
    QMediaCaptureSession        m_captureSession;
    QScopedPointer<QMediaRecorder> m_mediaRecorder;

    QString                     m_CamDescription;
    VideoSinkGrabber            m_VideoSinkGrabber;
    int                         m_LastFrameNum{ 0 };
    QSize                       m_DesiredFrameSize;
    bool                        m_WantCamInput[ eMaxAppModule ];
};


