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

#include "CamLogic.h"
#include "AppCommon.h"

#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
Q_DECLARE_METATYPE( QCameraInfo )
#else
# include <QMediaDevices>
# include <QVideoWidget>
# include <QAudioInput>
# include <QAudioOutput>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

namespace
{
    // from fourcc.org
    #define FOURCC_RGB		0x32424752
};

//============================================================================
CamLogic::CamLogic( AppCommon& myApp )
    : QWidget(&myApp)
    , m_MyApp( myApp )
    , m_SnapshotTimer( new QTimer( this ))
    , m_VideoSinkGrabber( this )
{
    // connect( m_SnapshotTimer, SIGNAL( timeout() ), this, SLOT( slotTakeSnapshot() ) );
    // m_SnapshotTimer->setInterval( CAM_SNAPSHOT_INTERVAL_MS );
    m_VideoSinkGrabber.setFps( 15 );
    connect( &m_VideoSinkGrabber, SIGNAL( signalSinkFrameAvailable( QImage& ) ), this, SLOT( slotSinkFrameAvailable( QImage& ) ) );
}

//============================================================================
void CamLogic::toGuiWantVideoCapture( bool wantVidCapture )
{
    cameraEnable( wantVidCapture );
}

//============================================================================
bool CamLogic::isCamAvailable( void )
{
    if( !GuiHelpers::checkUserPermission("android.permission.CAMERA"))
    {
        QMessageBox( QMessageBox::Information, QObject::tr("Camera Permission"), QObject::tr("Cannot use camera without user permission"), QMessageBox::Ok);
        return false;
    }

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
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    /*
    if( QCamera::Status::ActiveStatus != m_CamStatus || QCamera::State::ActiveState != m_CamState || QMultimedia::Available != m_imageCapture->availability() )
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
                LogMsg( LOG_DEBUG, "Cam Error %d Status %s State %s capture available %d error %d", m_camera->error(),
                    GuiParams::describeCamStatus( m_CamStatus ).toUtf8().constData(),
                    GuiParams::describeCamState( m_CamState ).toUtf8().constData(), m_imageCapture->availability(), m_imageCapture->error() );
            }
        }

        return;
    }
    */

    if( !m_imageCapture->isReadyForCapture() )
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
#else
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

#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
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
        setCamera( QMediaDevices::defaultVideoInput() );
#endif // QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    }

    return  !m_camera.isNull() && m_camera->isAvailable();
}

#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
//============================================================================
void CamLogic::setCamera( const QCameraDevice& cameraDevice )
{
    bool isStarted = m_CamIsStarted;
    cameraEnable( false );
    m_camera.reset( new QCamera( cameraDevice ) );
    m_captureSession.setCamera( m_camera.data() );

    connect( m_camera.data(), &QCamera::activeChanged, this, &CamLogic::updateCameraActive );
    connect( m_camera.data(), &QCamera::errorOccurred, this, &CamLogic::displayCameraError );

    m_mediaRecorder.reset( new QMediaRecorder );
    m_captureSession.setRecorder( m_mediaRecorder.data() );
    connect( m_mediaRecorder.data(), &QMediaRecorder::recorderStateChanged, this, &CamLogic::updateRecorderState );


    m_imageCapture = new QImageCapture( m_mediaRecorder.data() );
    m_captureSession.setImageCapture( m_imageCapture );

    connect( m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this, &CamLogic::updateRecordTime );
    connect( m_mediaRecorder.data(), &QMediaRecorder::errorChanged, this, &CamLogic::displayRecorderError );

    // HACK ALERT
    // some devices require the video output to be set even if invisible.. some do not
    // set video output to an invisible widget or we will never get the capture ready event
    m_captureSession.setVideoOutput(&m_VideoSinkGrabber);

    updateCameraActive( m_camera->isActive() );
    updateRecorderState( m_mediaRecorder->recorderState() );

    connect( m_imageCapture, &QImageCapture::readyForCaptureChanged, this, &CamLogic::readyForCapture );
    connect( m_imageCapture, &QImageCapture::imageCaptured, this, &CamLogic::processCapturedImage );
    connect( m_imageCapture, &QImageCapture::imageSaved, this, &CamLogic::imageSaved );
    connect( m_imageCapture, &QImageCapture::errorOccurred, this, &CamLogic::displayCaptureError );

    readyForCapture( m_imageCapture->isReadyForCapture() );

    selectVideoFormat( cameraDevice );

    updateCaptureMode( 0 );

    if( m_captureSession.audioInput() )
    {
        m_captureSession.audioInput()->setMuted(true);
    }

    if( m_captureSession.audioOutput() )
    {
        m_captureSession.audioOutput()->setMuted(true);
    }

    //m_imageCapture->setFileFormat( boxValue( ui->imageCodecBox ).value<QImageCapture::FileFormat>() );
    //m_imageCapture->setQuality( QImageCapture::HighQuality );
    //m_imageCapture->setResolution( GuiParams::getSnapshotDesiredSize() );

    m_CamDescription = cameraDevice.description();
    emit signalCameraDescription( m_CamDescription );

    m_CamInitiated = true;
    if( isStarted )
    {
        cameraEnable( true );
    }
}

#else
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
    settings.setResolution( GuiParams::getSnapshotSize() );
    m_imageCapture->setEncodingSettings( settings );
    m_CamInitiated = true;
}
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

//============================================================================
void CamLogic::selectVideoFormat( const QCameraDevice& cameraDevice )
{
    if( m_camera->cameraFormat().isNull() ) 
    {
        // Setting default settings.
        // The biggest resolution and the max framerate
        QSize targetSize( GuiParams::getSnapshotDesiredSize() );
        auto formats = cameraDevice.videoFormats();
        if( !formats.isEmpty() ) 
        {
            auto defaultFormat = formats.first();
            bool defaultFormatInvalid = true;
            LogMsg( LOG_VERBOSE, "Default camera resolution w %d h %d min fps %3.1f max fps %3.1f", defaultFormat.resolution().width(),
                defaultFormat.resolution().height(), defaultFormat.minFrameRate(), defaultFormat.maxFrameRate() );
            if( defaultFormat.resolution().width() >= targetSize.width() && defaultFormat.resolution().height() >= targetSize.height() )
            {
                defaultFormatInvalid = false;
            }

            int formatNum = 0;
            int defaultFormatNum = 0;
            for( const auto& format : formats ) 
            {
                formatNum++;
                LogMsg( LOG_VERBOSE, "Format %d camera resolution w %d h %d min fps %3.1f max fps %3.1f", formatNum, format.resolution().width(),
                    format.resolution().height(), format.minFrameRate(), format.maxFrameRate() );
                if( format.resolution().width() >= targetSize.width() && format.resolution().height() >= targetSize.height() )
                {
                    if( defaultFormatInvalid || ( format.resolution().width() - targetSize.width() < defaultFormat.resolution().width() - targetSize.width() )
                        || ( format.resolution().height() - targetSize.height() < defaultFormat.resolution().height() - targetSize.height() ) )
                    {
                        LogMsg( LOG_VERBOSE, "Found better camera resolution %d w %d h %d min fps %3.1f max fps %3.1f", formatNum, format.resolution().width(),
                            format.resolution().height(), format.minFrameRate(), format.maxFrameRate() );
                        defaultFormat = format;
                        if( defaultFormat.resolution().width() >= targetSize.width() && defaultFormat.resolution().height() >= targetSize.height() )
                        {
                            defaultFormatInvalid = false;
                            defaultFormatNum = formatNum;
                        }
                    }
                }
            }

            float desiredFps = 1000 / ( CAM_SNAPSHOT_INTERVAL_MS / 2 ); // request frame rate toughly twice as fast a snapshot interval
            LogMsg( LOG_VERBOSE, "Seting Format %d resolution w %d h %d min fps %3.1f max fps %3.1f desired fps %3.1f", formatNum, defaultFormat.resolution().width(),
                defaultFormat.resolution().height(), defaultFormat.minFrameRate(), defaultFormat.maxFrameRate(), desiredFps);

            m_camera->setCameraFormat( defaultFormat );

            if( desiredFps >= defaultFormat.minFrameRate() && desiredFps <= defaultFormat.maxFrameRate() )
            {
                m_mediaRecorder->setVideoFrameRate( desiredFps );
            }
            else if( desiredFps >= defaultFormat.maxFrameRate() )
            {
                m_mediaRecorder->setVideoFrameRate( defaultFormat.maxFrameRate() );
            }
            else if( desiredFps <= defaultFormat.minFrameRate()  )
            {
                m_mediaRecorder->setVideoFrameRate( defaultFormat.minFrameRate() );
            }
        }
    }
}

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
    QSize desiredSize = GuiParams::getSnapshotDesiredSize();
    if( !img.isNull() && img.format() == QImage::Format_RGB888 && img.size() == desiredSize )
    {
        uint32_t imageLen = img.bytesPerLine() * img.height();
        m_MyApp.getEngine().fromGuiVideoData( FOURCC_RGB, (uint8_t *)img.bits(), img.width(), img.height(), imageLen, m_MyApp.getCamCaptureRotation() );
    }
    else
    {
        QImage scaledImage = img.size() == desiredSize ? img : img.scaled( desiredSize );
        if( !scaledImage.isNull() && scaledImage.format() == QImage::Format_RGB888 )
        {
            uint32_t imageLen = scaledImage.bytesPerLine() * scaledImage.height();
            m_MyApp.getEngine().fromGuiVideoData( FOURCC_RGB, scaledImage.bits(), scaledImage.width(), scaledImage.height(), imageLen, m_MyApp.getCamCaptureRotation() );
        }
        else if( !scaledImage.isNull() )
        {
            QImage toSendImage = scaledImage.convertToFormat( QImage::Format_RGB888 );
            uint32_t imageLen = toSendImage.bytesPerLine() * toSendImage.height();
            m_MyApp.getEngine().fromGuiVideoData( FOURCC_RGB, toSendImage.bits(), toSendImage.width(), toSendImage.height(), imageLen, m_MyApp.getCamCaptureRotation() );
        }
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

#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
void CamLogic::updateCameraActive( bool active )
{
    if( active ) 
    {
        LogMsg( LOG_VERBOSE, "updateCameraActive is active " );
    }
    else 
    {
        LogMsg( LOG_VERBOSE, "updateCameraActive is NOT active " );
    }
}

//============================================================================
void CamLogic::updateRecorderState( QMediaRecorder::RecorderState state )
{
    switch( state ) 
    {
    case QMediaRecorder::StoppedState:
        LogMsg( LOG_VERBOSE, "QMediaRecorder stopped " );
        break;
    case QMediaRecorder::PausedState:
        LogMsg( LOG_VERBOSE, "QMediaRecorder paused " );
        break;
    case QMediaRecorder::RecordingState:
        LogMsg( LOG_VERBOSE, "QMediaRecorder recording " );
        break;
    }
}

//============================================================================
void CamLogic::displayCaptureError( int id, const QImageCapture::Error error, const QString& errorString )
{
    Q_UNUSED( id );
    //m_ReadyForCapture = false;
    //m_isCapturingImage = false;

    //LogModule(eLogVideo, LOG_VERBOSE, "displayCaptureError %d %s", error, errorString.toUtf8().constData());
    LogMsg( LOG_VERBOSE, "displayCaptureError %d %s", error, errorString.toUtf8().constData() );
    if( QImageCapture::NotReadyError != error )
    {
        QMessageBox::warning( this, QObject::tr( "Image Capture Error" ), errorString );
    }
}
#else
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
        m_VideoSinkGrabber.enableGrab( true );
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
        m_VideoSinkGrabber.enableGrab( false );
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
    LogMsg( LOG_ERROR, "CamLogic::readyForCapture ? %d", ready );
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

//============================================================================
void CamLogic::nextCamera( void )
{
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    bool foundDevice = false;
    bool setNewDevice = false;
    if( availableCameras.size() > 1 )
    {
        for( const QCameraDevice& cameraDevice : availableCameras )
        {
            if( cameraDevice.description() == m_CamDescription )
            {
                foundDevice = true;
            }

            if( foundDevice && !( m_CamDescription == cameraDevice.description() ) )
            {
                setCamera( cameraDevice );
                setNewDevice = true;
                return;
            }
        }

        if( !foundDevice || !setNewDevice )
        {
            setCamera( availableCameras.front() );
        }
    }
}

//============================================================================
void CamLogic::slotSinkFrameAvailable( QImage& frame )
{
    static int frameNum = 0;
    frameNum++;
    processCapturedImage( frameNum, frame );
}
