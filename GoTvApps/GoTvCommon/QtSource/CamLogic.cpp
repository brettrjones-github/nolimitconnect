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

Q_DECLARE_METATYPE( QCameraInfo )

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

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
    if( !m_isCapturingImage )
    {
        m_isCapturingImage = true;
        m_imageCapture->capture();
    }
}

//============================================================================
//void CamLogic::initCamLogic( void ) //: ui(new Ui::Camera)
//{
    //ui->setupUi(this);

    //CamLogic devices:

    //QActionGroup *videoDevicesGroup = new QActionGroup( this );
    //videoDevicesGroup->setExclusive( true );
    //const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    //for( const QCameraInfo &cameraInfo : availableCameras ) {
    //    QAction *videoDeviceAction = new QAction( cameraInfo.description(), videoDevicesGroup );
    //    videoDeviceAction->setCheckable( true );
    //    videoDeviceAction->setData( QVariant::fromValue( cameraInfo ) );
    //    if( cameraInfo == QCameraInfo::defaultCamera() )
    //        videoDeviceAction->setChecked( true );

    //    //ui->menuDevices->addAction(videoDeviceAction);
    //}

    //connect( videoDevicesGroup, &QActionGroup::triggered, this, &CamLogic::updateCameraDevice );
    ////connect(ui->captureWidget, &QTabWidget::currentChanged, this, &CamLogic::updateCaptureMode);

    //setCamera( QCameraInfo::defaultCamera() );
//}

//============================================================================
// set application is exiting.. returt true if cam is busy with capture
bool CamLogic::setAppIsExiting( bool isExiting )
{
    m_applicationExiting = isExiting;
    return m_isCapturingImage;
}

//============================================================================
void CamLogic::setViewfinder( QCameraViewfinder *viewfinder )
{
    //m_ViewFinder = viewfinder;
    //if( m_ViewFinder && !m_camera.isNull() )
    //{
    //    m_camera->setViewfinder( viewfinder );
    //}
}

//============================================================================
bool CamLogic::assureCamInitiated( void )
{
    if( !m_CamInitiated )
    {
        setCamera( QCameraInfo::defaultCamera() );
    }

    return  !m_camera.isNull() && m_camera->isAvailable();
}

//============================================================================
void CamLogic::setCamera( const QCameraInfo &cameraInfo )
{
    m_camera.reset( new QCamera( cameraInfo ) );

    connect( m_camera.data(), &QCamera::stateChanged, this, &CamLogic::updateCameraState );
    connect( m_camera.data(), QOverload<QCamera::Error>::of( &QCamera::error ), this, &CamLogic::displayCameraError );

    m_imageCapture.reset( new QCameraImageCapture( m_camera.data() ) );
    m_imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToBuffer );

    //connect(ui->exposureCompensation, &QAbstractSlider::valueChanged, this, &CamLogic::setExposureCompensation);
    //m_camera->setViewfinder(ui->viewfinder);

    updateCameraState( m_camera->state() );
    updateLockStatus( m_camera->lockStatus(), QCamera::UserRequest );

    connect( m_imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &CamLogic::readyForCapture );
    connect( m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &CamLogic::processCapturedImage );
    connect( m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &CamLogic::imageSaved );
    connect( m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of( &QCameraImageCapture::error ),
             this, &CamLogic::displayCaptureError );

    connect( m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of( &QCamera::lockStatusChanged ),
             this, &CamLogic::updateLockStatus );

    //ui->captureWidget->setTabEnabled(0, (m_camera->isCaptureModeSupported(QCamera::CaptureStillImage)));
    //ui->captureWidget->setTabEnabled(1, (m_camera->isCaptureModeSupported(QCamera::CaptureVideo)));

    updateCaptureMode(0);

    QCameraImageCapture *imagecapture = m_imageCapture.data();
    QImageEncoderSettings settings = imagecapture->encodingSettings();
    settings.setResolution( QSize( 320, 240 ) );
    m_imageCapture->setEncodingSettings( settings );
    m_CamInitiated = true;
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

void CamLogic::updateRecordTime()
{
    //QString str = QString( "Recorded %1 sec" ).arg( m_mediaRecorder->duration() / 1000 );
    //ui->statusbar->showMessage(str);
}

// from fourcc.org 
#define FOURCC_RGB		0x32424752

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
    switch( m_camera->lockStatus() ) {
    case QCamera::Searching:
    case QCamera::Locked:
        m_camera->unlock();
        break;
    case QCamera::Unlocked:
        m_camera->searchAndLock();
    }
}

//============================================================================
void CamLogic::updateLockStatus( QCamera::LockStatus status, QCamera::LockChangeReason reason )
{
    //QColor indicationColor = Qt::black;

    //switch (status) {
    //case QCamera::Searching:
    //    indicationColor = Qt::yellow;
    //    ui->statusbar->showMessage(tr("Focusing..."));
    //    ui->lockButton->setText(tr("Focusing..."));
    //    break;
    //case QCamera::Locked:
    //    indicationColor = Qt::darkGreen;
    //    ui->lockButton->setText(tr("Unlock"));
    //    ui->statusbar->showMessage(tr("Focused"), 2000);
    //    break;
    //case QCamera::Unlocked:
    //    indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
    //    ui->lockButton->setText(tr("Focus"));
    //    if (reason == QCamera::LockFailed)
    //        ui->statusbar->showMessage(tr("Focus Failed"), 2000);
    //}

    //QPalette palette = ui->lockButton->palette();
    //palette.setColor(QPalette::ButtonText, indicationColor);
    //ui->lockButton->setPalette(palette);
}

//============================================================================
void CamLogic::displayCaptureError( int id, const QCameraImageCapture::Error error, const QString &errorString )
{
    Q_UNUSED( id );
    Q_UNUSED( error );
    QMessageBox::warning( this, tr( "Image Capture Error" ), errorString );
    m_isCapturingImage = false;
}

//============================================================================
void CamLogic::startCamera()
{
    if( assureCamInitiated() )
    {
        m_CamIsStarted = true;
        m_camera->start();
        if( !m_ViewFinder )
        {
            m_ViewFinder = new QCameraViewfinder();
            m_ViewFinder->show();
            m_camera->setViewfinder( m_ViewFinder );
            m_ViewFinder->hide();
        }

        m_SnapshotTimer->start();
    }
}

//============================================================================
void CamLogic::stopCamera()
{
    m_SnapshotTimer->stop();
    m_CamIsStarted = false;
    if( !m_camera.isNull() )
    {
        m_camera->stop();
    }
}

//============================================================================
void CamLogic::updateCaptureMode(int)
{
    //int tabIndex = ui->captureWidget->currentIndex();
    //QCamera::CaptureModes captureMode = tabIndex == 0 ? QCamera::CaptureStillImage : QCamera::CaptureVideo;
    QCamera::CaptureModes captureMode = QCamera::CaptureStillImage;

    if( m_camera && m_camera->isCaptureModeSupported( captureMode ) )
    {
        m_camera->setCaptureMode( captureMode );
    }
}

//============================================================================
void CamLogic::updateCameraState( QCamera::State state )
{
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

void CamLogic::setExposureCompensation( int index )
{
    m_camera->exposure()->setExposureCompensation( index*0.5 );
}

void CamLogic::displayRecorderError()
{
    //QMessageBox::warning( this, tr( "Capture Error" ), m_mediaRecorder->errorString() );
}

void CamLogic::displayCameraError()
{
    QMessageBox::warning( this, tr( "CamLogic Error" ), m_camera->errorString() );
}

void CamLogic::updateCameraDevice( QAction *action )
{
    setCamera( qvariant_cast< QCameraInfo >( action->data() ) );
}

void CamLogic::displayViewfinder()
{
    //ui->stackedWidget->setCurrentIndex(0);
}

void CamLogic::displayCapturedImage()
{
    //ui->stackedWidget->setCurrentIndex(1);
}

void CamLogic::readyForCapture( bool ready )
{
    //ui->takeImageButton->setEnabled(ready);
}

void CamLogic::imageSaved( int id, const QString &fileName )
{
    Q_UNUSED( id );
    //ui->statusbar->showMessage(tr("Captured \"%1\"").arg(QDir::toNativeSeparators(fileName)));

    m_isCapturingImage = false;
    // if (m_applicationExiting)
    //     close();
}

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
