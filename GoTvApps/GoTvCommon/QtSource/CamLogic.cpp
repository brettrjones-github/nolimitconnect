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
// http://www.nolimitconnect.com
//============================================================================

#include <app_precompiled_hdr.h>
#include "CamLogic.h"
#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
Q_DECLARE_METATYPE( QCameraInfo )
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

namespace
{
    // from fourcc.org
    #define FOURCC_RGB		0x32424752
};

//============================================================================
CamLogic::CamLogic( AppCommon& myApp )
    : QWidget(nullptr)
    , m_MyApp( myApp )
    , m_SnapshotTimer( new QTimer( this ))
{
    connect( m_SnapshotTimer, SIGNAL( timeout() ), this, SLOT( slotTakeSnapshot() ) );
    m_SnapshotTimer->setInterval( 60 );
}

//============================================================================
void CamLogic::toGuiWantVideoCapture( bool wantVidCapture )
{
    cameraEnable( wantVidCapture );
}

//============================================================================
bool CamLogic::isCamAvailable( void )
{
    assureCamInitiated();
    return !m_camera.isNull() && m_camera->isAvailable();
}

//============================================================================
bool CamLogic::isCamCaptureRunning( void )
{
    assureCamInitiated();
    return m_CamIsStarted && !m_camera.isNull() && m_camera->isAvailable();
}

//============================================================================
void CamLogic::cameraEnable( bool wantVidCapture )
{
    if( getAppIsExiting() )
    {
        if( m_CamIsStarted )
        {
            stopCamera();
        }

        return;
    }

    if( wantVidCapture != m_CamIsStarted )
    {
        if( wantVidCapture )
        {
            startCamera();
        }
        else
        {
            stopCamera();
        }
    }
}

//============================================================================
void CamLogic::slotTakeSnapshot( void )
{
    //LogModule(eLogVideo, LOG_DEBUG, "Cam Error %d Status %s", m_camera->error(), GuiParams::describeCamStatus(m_CamStatus).toUtf8().constData());
    static int notReadyCnt = 0;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if( QCamera::Status::ActiveStatus != m_CamStatus || QCamera::State::ActiveState != m_CamState || QMultimedia::Available != m_imageCapture->availability())
    {
        // camera system not ready
        notReadyCnt++;
        if( notReadyCnt > 2 )
        {
            static int64_t lastMsgTime = 0;
            int64_t elapsedNow = m_MyApp.elapsedSeconds();
            if( elapsedNow != lastMsgTime )
            {
                lastMsgTime = elapsedNow;
                //LogModule(eLogVideo, LOG_DEBUG, "Cam Status %s", GuiParams::describeCamStatus(m_CamStatus).toUtf8().constData());
                LogMsg(LOG_DEBUG, "Cam Error %d Status %s State %s capture available %d error %d", m_camera->error(),
                       GuiParams::describeCamStatus(m_CamStatus).toUtf8().constData(),
                       GuiParams::describeCamState(m_CamState).toUtf8().constData(), m_imageCapture->availability(), m_imageCapture->error());
            }
        }

        return;
    }

    if( !m_imageCapture->isReadyForCapture())
    {
        // Calling capture() while readyForCapture is false is not permitted and results in an error.
        return;
    }

    notReadyCnt = 0;
    if( !m_isCapturingImage )
    {
        m_isCapturingImage = true;
        m_imageCapture->capture();
    }

#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

//============================================================================
// set application is exiting.. returt true if cam is busy with capture
bool CamLogic::setAppIsExiting( bool isExiting )
{
    m_applicationExiting = isExiting;
    return m_isCapturingImage;
}

//============================================================================
bool CamLogic::assureCamInitiated( void )
{
    if( !m_CamInitiated )
    {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        setCamera( QCameraInfo::defaultCamera() );
#else
        m_camera.reset(new QCamera());
#endif  QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }

    return  !m_camera.isNull() && m_camera->isAvailable();
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
//============================================================================
void CamLogic::setCamera( const QCameraInfo &cameraInfo )
{
    m_camera.reset( new QCamera( cameraInfo ) );

    connect( m_camera.data(), &QCamera::stateChanged, this, &CamLogic::cameraStateChanged );
    connect( m_camera.data(), &QCamera::statusChanged, this, &CamLogic::cameraStatusChanged );
    connect( m_camera.data(), QOverload<QCamera::Error>::of( &QCamera::error ), this, &CamLogic::displayCameraError );

    m_imageCapture.reset( new QCameraImageCapture( m_camera.data() ) );
    m_imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToBuffer );

    cameraStateChanged( m_camera->state() );
    updateLockStatus( m_camera->lockStatus(), QCamera::UserRequest );

    connect( m_imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &CamLogic::readyForCapture );
    connect( m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &CamLogic::processCapturedImage );
    connect( m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &CamLogic::imageSaved );
    connect( m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of( &QCameraImageCapture::error ),
             this, &CamLogic::displayCaptureError );

    connect( m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of( &QCamera::lockStatusChanged ),
             this, &CamLogic::updateLockStatus );

    updateCaptureMode(0);

    QCameraImageCapture *imagecapture = m_imageCapture.data();
    QImageEncoderSettings settings = imagecapture->encodingSettings();
    settings.setResolution( QSize( 320, 240 ) );
    m_imageCapture->setEncodingSettings( settings );
    m_CamInitiated = true;
}
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
void CamLogic::keyPressEvent( QKeyEvent * event )
{
    if( event->isAutoRepeat() )
        return;

    switch( event->key() ) {
    case Qt::Key_CameraFocus:
        //displayViewfinder();
        //m_camera->searchAndLock();
        event->accept();
        break;
    case Qt::Key_Camera:
        //if( m_camera->captureMode() == QCamera::CaptureStillImage ) 
        //{
        //    slotTakeSnapshot();
        //}
        event->accept();
        break;
    default:
        QWidget::keyPressEvent( event );
    }
}

//============================================================================
void CamLogic::keyReleaseEvent( QKeyEvent *event )
{
    if( event->isAutoRepeat() )
        return;

    switch( event->key() ) {
    case Qt::Key_CameraFocus:
        //m_camera->unlock();
        break;
    default:
        QWidget::keyReleaseEvent( event );
    }
}

//============================================================================
void CamLogic::updateRecordTime()
{
    //QString str = QString( "Recorded %1 sec" ).arg( m_mediaRecorder->duration() / 1000 );
    //ui->statusbar->showMessage(str);
}

//============================================================================
void CamLogic::processCapturedImage( int requestId, const QImage& img )
{
    Q_UNUSED( requestId );
    if( img.isNull() )
    {
        LogMsg( LOG_ERROR, "processCapturedImage null image " );
        return;
    }

    // LogMsg( LOG_DEBUG, "processCapturedImage x%d y%d ", img.width(), img.height() );

    QImage toSendImage;
    if( img.format() == QImage::Format_RGB888 )
    {
        toSendImage = img;
    }
    else
    {
        toSendImage = img.convertToFormat( QImage::Format_RGB888 );
    }

    if( !toSendImage.isNull() )
    {
        uint32_t imageLen = toSendImage.bytesPerLine() * toSendImage.height();
        m_MyApp.getEngine().fromGuiVideoData( FOURCC_RGB, toSendImage.bits(), toSendImage.width(), toSendImage.height(), imageLen, m_MyApp.getCamCaptureRotation() );
    }

    m_isCapturingImage = false;
}

//============================================================================
void CamLogic::configureCaptureSettings()
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    switch( m_camera->captureMode() ) {
    case QCamera::CaptureStillImage:
        configureImageSettings();
        break;
    case QCamera::CaptureVideo:
        configureVideoSettings();
        break;
    default:
        break;
    }
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

//============================================================================
void CamLogic::configureVideoSettings()
{
    //VideoSettings settingsDialog( m_mediaRecorder.data() );
    //settingsDialog.setWindowFlags( settingsDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint );

    //settingsDialog.setAudioSettings( m_audioSettings );
    //settingsDialog.setVideoSettings( m_videoSettings );
    //settingsDialog.setFormat( m_videoContainerFormat );

    //if( settingsDialog.exec() ) {
    //    m_audioSettings = settingsDialog.audioSettings();
    //    m_videoSettings = settingsDialog.videoSettings();
    //    m_videoContainerFormat = settingsDialog.format();

    //    m_mediaRecorder->setEncodingSettings(
    //        m_audioSettings,
    //        m_videoSettings,
    //        m_videoContainerFormat );

    //    m_camera->unload();
    //    m_camera->start();
    //}
}

//============================================================================
void CamLogic::configureImageSettings()
{
    //ImageSettings settingsDialog( m_imageCapture.data() );
    //settingsDialog.setWindowFlags( settingsDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint );

    //settingsDialog.setImageSettings( m_imageSettings );

    //if( settingsDialog.exec() ) 
    //{
    //    m_imageSettings = settingsDialog.imageSettings();
    //    m_imageCapture->setEncodingSettings( m_imageSettings );
    //}
}

//============================================================================
void CamLogic::record()
{
    //m_mediaRecorder->record();
    //updateRecordTime();
}

//============================================================================
void CamLogic::pause()
{
//    m_mediaRecorder->pause();
}

//============================================================================
void CamLogic::stop()
{
//    m_mediaRecorder->stop();
}

//============================================================================
void CamLogic::setMuted( bool muted )
{
//    m_mediaRecorder->setMuted( muted );
}

//============================================================================
void CamLogic::toggleLock()
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    switch( m_camera->lockStatus() ) {
    case QCamera::Searching:
    case QCamera::Locked:
        m_camera->unlock();
        break;
    case QCamera::Unlocked:
        m_camera->searchAndLock();
    }
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
//============================================================================
void CamLogic::displayCaptureError( int id, const QCameraImageCapture::Error error, const QString &errorString )
{
    Q_UNUSED( id );
    //m_ReadyForCapture = false;
    //m_isCapturingImage = false;

    //LogModule(eLogVideo, LOG_VERBOSE, "displayCaptureError %d %s", error, errorString.toUtf8().constData());
    LogMsg(LOG_VERBOSE, "displayCaptureError %d %s", error, errorString.toUtf8().constData());
    if( QCameraImageCapture::NotReadyError != error )
    {
        QMessageBox::warning( this, QObject::tr( "Image Capture Error" ), errorString );
    }
}
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
void CamLogic::startCamera()
{
    if( assureCamInitiated() && !m_camera.isNull() && !m_CamIsStarted )
    {
        m_CamIsStarted = true;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if( !m_ViewFinder )
        {
            m_ViewFinder = new QCameraViewfinder();
            m_ViewFinder->show();
            m_camera->setViewfinder( m_ViewFinder );
            m_ViewFinder->hide();
        }
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)

        m_camera->start();
        m_SnapshotTimer->start();
    }
    else if( m_camera.isNull() )
    {
        QMessageBox::warning( this, QObject::tr( "CamLogic Error" ), QObject::tr( "Camera is null" ) );
    }
}

//============================================================================
void CamLogic::stopCamera()
{
    if( m_CamIsStarted )
    {
        m_SnapshotTimer->stop();
        m_CamIsStarted = false;
        if( !m_camera.isNull() )
        {
            m_camera->stop();
        }
    }
}

//============================================================================
void CamLogic::updateCaptureMode(int)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QCamera::CaptureModes captureMode = QCamera::CaptureStillImage;

    if( m_camera && m_camera->isCaptureModeSupported( captureMode ) )
    {
        m_camera->setCaptureMode( captureMode );
    }
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
//============================================================================
void CamLogic::cameraStateChanged( QCamera::State camState )
{
    m_CamState = camState;
    //LogModule(eLogVideo, LOG_VERBOSE, "camState %s", GuiParams::describeCamState( camState ).toUtf8().constData());
    LogMsg(LOG_VERBOSE, "camState %s", GuiParams::describeCamState( camState ).toUtf8().constData());
    //switch (state) {
    //case QCamera::ActiveState:
    //    ui->actionStartCamera->setEnabled(false);
    //    ui->actionStopCamera->setEnabled(true);
    //    ui->captureWidget->setEnabled(true);
    //    ui->actionSettings->setEnabled(true);
    //    break;
    //case QCamera::UnloadedState:
    //case QCamera::LoadedState:
    //    ui->actionStartCamera->setEnabled(true);
    //    ui->actionStopCamera->setEnabled(false);
    //    ui->captureWidget->setEnabled(false);
    //    ui->actionSettings->setEnabled(false);
    //}
}

//============================================================================
void CamLogic::cameraStatusChanged( QCamera::Status camStatus )
{
    m_CamStatus = camStatus;
    //LogModule(eLogVideo, LOG_VERBOSE, "camStatus %s", GuiParams::describeCamStatus( camStatus ).toUtf8().constData());
    LogMsg(LOG_VERBOSE, "camStatus %s", GuiParams::describeCamStatus( camStatus ).toUtf8().constData());
    //switch (state) {
    //case QCamera::ActiveState:
    //    ui->actionStartCamera->setEnabled(false);
    //    ui->actionStopCamera->setEnabled(true);
    //    ui->captureWidget->setEnabled(true);
    //    ui->actionSettings->setEnabled(true);
    //    break;
    //case QCamera::UnloadedState:
    //case QCamera::LoadedState:
    //    ui->actionStartCamera->setEnabled(true);
    //    ui->actionStopCamera->setEnabled(false);
    //    ui->captureWidget->setEnabled(false);
    //    ui->actionSettings->setEnabled(false);
    //}
}

//============================================================================
void CamLogic::updateRecorderState( QMediaRecorder::State state )
{
    //switch (state) {
    //case QMediaRecorder::StoppedState:
    //    ui->recordButton->setEnabled(true);
    //    ui->pauseButton->setEnabled(true);
    //    ui->stopButton->setEnabled(false);
    //    break;
    //case QMediaRecorder::PausedState:
    //    ui->recordButton->setEnabled(true);
    //    ui->pauseButton->setEnabled(false);
    //    ui->stopButton->setEnabled(true);
    //    break;
    //case QMediaRecorder::RecordingState:
    //    ui->recordButton->setEnabled(false);
    //    ui->pauseButton->setEnabled(true);
    //    ui->stopButton->setEnabled(true);
    //    break;
    //}
}
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
void CamLogic::setExposureCompensation( int index )
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    m_camera->exposure()->setExposureCompensation( index*0.5 );
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

//============================================================================
void CamLogic::displayRecorderError()
{
    //QMessageBox::warning( this, tr( "Capture Error" ), m_mediaRecorder->errorString() );
}

//============================================================================
void CamLogic::displayCameraError()
{
    //LogModule(eLogVideo, LOG_ERROR, "CamLogic Error %d %s", m_camera->error(), m_camera->errorString().toUtf8().constData() );
    LogMsg(LOG_ERROR, "CamLogic Error %d %s", m_camera->error(), m_camera->errorString().toUtf8().constData() );

    // QMessageBox::warning( this, tr( "CamLogic Error" ), m_camera->errorString() );
}

//============================================================================
void CamLogic::updateCameraDevice( QAction *action )
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    setCamera( qvariant_cast< QCameraInfo >( action->data() ) );
#endif // #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

//============================================================================
void CamLogic::displayViewfinder()
{
    //ui->stackedWidget->setCurrentIndex(0);
}

//============================================================================
void CamLogic::displayCapturedImage()
{
    //ui->stackedWidget->setCurrentIndex(1);
}

//============================================================================
void CamLogic::readyForCapture( bool ready )
{
    m_ReadyForCapture = ready;
}

//============================================================================
void CamLogic::imageSaved( int id, const QString &fileName )
{
    Q_UNUSED( id );
    //ui->statusbar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    m_isCapturingImage = false;
    // if (m_applicationExiting)
    //     close();
}

//============================================================================
void CamLogic::closeEvent( QCloseEvent *event )
{
    if( m_isCapturingImage ) {
        //setEnabled(false);
        m_applicationExiting = true;
        event->ignore();
    }
    else {
        event->accept();
    }
}
