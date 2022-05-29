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

#include "config_gotvapps.h"

#include <QTimer>
#include <QKeyEvent>
#include <QCameraInfo>
#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QMediaMetaData>

#include <QMessageBox>
#include <QPalette>

#include <QtWidgets>
#include <QCameraImageCapture>
#include <QMediaService>


class AppCommon;

class CamLogic : public QWidget
{
    Q_OBJECT

public:
    CamLogic( AppCommon& myApp );
    virtual ~CamLogic() = default;

    void                        cameraEnable( bool wantVidCapture );

    // set application is exiting.. returt true if cam is busy with capture
    bool                        setAppIsExiting( bool isExiting );
    bool                        getAppIsExiting( void )                             { return m_applicationExiting; }


    void                        setViewfinder( QCameraViewfinder *viewfinder );


    void						setCamSourceId( uint32_t camId )                        { m_CamId = camId; }
    uint32_t					getCamSourceId( void )                                  { return m_CamId; }

    void						setCamShowPreview( bool showPreview )                   { m_ShowPreview = showPreview; };
    bool						getCamShowPreview( void )                               { return m_ShowPreview; }

    void						setCamRotation( uint32_t camId, uint32_t camRotation )  { m_CamRotation = camRotation; };
    uint32_t					getCamRotation( uint32_t camId )                        { return m_CamRotation; }

    void						setVidFeedRotation( uint32_t feedRotation )             { m_FeedRotation = feedRotation; };
    uint32_t					getVidFeedRotation( void )                              { return m_FeedRotation; }

    void                        toGuiWantVideoCapture( bool wantVidCapture );

public slots:
    void setCamera( const QCameraInfo &cameraInfo );

    void startCamera();
    void stopCamera();

    void record();
    void pause();
    void stop();
    void setMuted( bool );

    void toggleLock();
    void displayCaptureError( int, QCameraImageCapture::Error, const QString &errorString );

    void configureCaptureSettings();
    void configureVideoSettings();
    void configureImageSettings();

    void displayRecorderError();
    void displayCameraError();

    void updateCameraDevice( QAction *action );

    void updateCameraState( QCamera::State );
    void updateCaptureMode( int );
    void updateRecorderState( QMediaRecorder::State state );
    void setExposureCompensation( int index );

    void updateRecordTime();

    void processCapturedImage( int requestId, const QImage &img );
    void updateLockStatus( QCamera::LockStatus, QCamera::LockChangeReason );

    void displayViewfinder();
    void displayCapturedImage();

    void readyForCapture( bool ready );
    void imageSaved( int id, const QString &fileName );

    void slotTakeSnapshot( void );

protected:
    void keyPressEvent( QKeyEvent *event ) override;
    void keyReleaseEvent( QKeyEvent *event ) override;
    void closeEvent( QCloseEvent *event ) override;

private:
    AppCommon&                  m_MyApp;
    uint32_t                    m_CamId{ 0 };
    bool                        m_ShowPreview;
    uint32_t                    m_CamRotation{ 0 };
    uint32_t                    m_FeedRotation{ 0 };

    QTimer *                    m_SnapshotTimer{ nullptr };

    QScopedPointer<QCamera>     m_camera;
    QCameraViewfinder *         m_ViewFinder{ nullptr };
    QScopedPointer<QCameraImageCapture> m_imageCapture;

    QImageEncoderSettings       m_imageSettings;
    QAudioEncoderSettings       m_audioSettings;
    QVideoEncoderSettings       m_videoSettings;
    QString                     m_videoContainerFormat;
    bool                        m_isCapturingImage = false;
    bool                        m_applicationExiting = false;
    bool                        m_CamIsStarted = false;

};


