#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "videosettings.h"
#include "imagesettings.h"

#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>
#include <CoreLib/VxGlobals.h>

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


Q_DECLARE_METATYPE( QCameraInfo )

namespace GuiParams
{
    QSize camImageDefaultSize( 320, 240 );
    QSize& getCamImageDefaultSize() { return camImageDefaultSize;  }
}

//============================================================================
MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
    , ui(new Ui::MainWindowUi)
    , m_SnapshotTimer(new QTimer(this))
    , m_MyApp( GetAppInstance() )
    , m_CamLogic( m_MyApp.getCamLogic() )
{
    ui->setupUi(this);

    QActionGroup *videoDevicesGroup = new QActionGroup( this );
    videoDevicesGroup->setExclusive( true );
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for( const QCameraInfo &cameraInfo : availableCameras ) 
    {
        QAction *videoDeviceAction = new QAction( cameraInfo.description(), videoDevicesGroup );
        videoDeviceAction->setCheckable( true );
        videoDeviceAction->setData( QVariant::fromValue( cameraInfo ) );
        if( cameraInfo == QCameraInfo::defaultCamera() )
        {
            videoDeviceAction->setChecked( true );
        }

        ui->menuDevices->addAction( videoDeviceAction );
    }

    QObject::connect( videoDevicesGroup, SIGNAL( triggered( QAction * ) ), this, SLOT( updateCameraDevice( QAction * ) ) );
    QObject::connect( ui->captureWidget, SIGNAL( currentChanged(int) ), this, SLOT( updateCaptureMode( int ) ) );

    QObject::connect( ui->takeImageButton, SIGNAL( clicked() ), this, SLOT( takeImage() ) );
    QObject::connect( ui->recordButton, SIGNAL( clicked() ), this, SLOT( record() ) );
    QObject::connect( ui->stopButton, SIGNAL( clicked() ), this, SLOT( stop() ) );
    QObject::connect( ui->pauseButton, SIGNAL( clicked() ), this, SLOT( pause() ) );
    QObject::connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
    QObject::connect( ui->takeImageButton, SIGNAL( clicked() ), this, SLOT( takeImage() ) );
    QObject::connect( ui->lockButton, SIGNAL( clicked() ), this, SLOT( toggleLock() ) );
    QObject::connect( ui->muteButton, SIGNAL( toggled( bool ) ), this, SLOT( setMuted( bool ) ) );
    QObject::connect( ui->exposureCompensation, SIGNAL( valueChanged( int ) ), this, SLOT( setExposureCompensation( int ) ) );
    QObject::connect( ui->actionSettings, SIGNAL( triggered() ), this, SLOT( configureCaptureSettings() ) );
    QObject::connect( ui->actionStartCamera, SIGNAL( triggered() ), this, SLOT( startCamera() ) );
    QObject::connect( ui->actionStopCamera, SIGNAL( triggered() ), this, SLOT( stopCamera() ) );

    setCamera( QCameraInfo::defaultCamera() );
    /// Add/Remove callback from MediaProcessor when given media type is processed and available
    m_MyApp.getEngine().fromGuiWantMediaInput( eMediaInputVideoJpgSmall, this, this, true );
}

//============================================================================
MainWindow::~MainWindow()
{
    delete ui;
}

//============================================================================
void MainWindow::setCamera( const QCameraInfo &cameraInfo )
{
    m_CamLogic.setCamera( cameraInfo );



    //m_camera.reset( new QCamera( cameraInfo ) );
    //m_camera->load();

    //connect( m_camera.data(), &QCamera::stateChanged, this, &MainWindow::updateCameraState );
    //connect( m_camera.data(), QOverload<QCamera::Error>::of( &QCamera::error ), this, &MainWindow::displayCameraError );

    ////m_mediaRecorder.reset( new QMediaRecorder( m_camera.data() ) );
    ////connect( m_mediaRecorder.data(), &QMediaRecorder::stateChanged, this, &MainWindow::updateRecorderState );

    //m_imageCapture.reset( new QCameraImageCapture( m_camera.data() ) );
    //m_imageCapture->setCaptureDestination( QCameraImageCapture::CaptureToBuffer );

    //connect( m_mediaRecorder.data(), &QMediaRecorder::durationChanged, this, &MainWindow::updateRecordTime );
    //connect( m_mediaRecorder.data(), QOverload<QMediaRecorder::Error>::of( &QMediaRecorder::error ),
    //         this, &MainWindow::displayRecorderError );

    //m_mediaRecorder->setMetaData( QMediaMetaData::Title, QVariant( QLatin1String( "Test Title" ) ) );

    connect( ui->exposureCompensation, &QAbstractSlider::valueChanged, this, &MainWindow::setExposureCompensation );

    //m_camera->setViewfinder( ui->viewfinder );

    //updateCameraState( m_camera->state() );
    //updateLockStatus( m_camera->lockStatus(), QCamera::UserRequest );
    ////updateRecorderState( m_mediaRecorder->state() );

    //connect( m_imageCapture.data(), &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::readyForCapture );
    //connect( m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &MainWindow::processCapturedImage );
    //connect( m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &MainWindow::imageSaved );
    //connect( m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of( &QCameraImageCapture::error ),
    //         this, &MainWindow::displayCaptureError );

    //connect( m_camera.data(), QOverload<QCamera::LockStatus, QCamera::LockChangeReason>::of( &QCamera::lockStatusChanged ),
    //         this, &MainWindow::updateLockStatus );

    ui->captureWidget->setTabEnabled( 0, true );
    ui->captureWidget->setTabEnabled( 1, false );

    updateCaptureMode( 0 );

    m_CamLogic.startCamera();
}

//============================================================================
void MainWindow::slotTakeSnapshot( void )
{
    if( ui->captureWidget->isEnabled() )
    {
        takeImage();
    }
}

//============================================================================
void MainWindow::keyPressEvent( QKeyEvent * event )
{
    if( event->isAutoRepeat() )
        return;

    switch( event->key() ) {
    case Qt::Key_CameraFocus:
        //m_CamLogic.keyPressEvent( event );
        event->accept();
        break;
    case Qt::Key_Camera:
        //if( m_camera->captureMode() == QCamera::CaptureStillImage ) {
        //    takeImage();
        //}
        //else {
        //    //if( m_mediaRecorder->state() == QMediaRecorder::RecordingState )
        //    //    stop();
        //    //else
        //    //    record();
        //}
        event->accept();
        break;
    default:
        QMainWindow::keyPressEvent( event );
    }
}

//============================================================================
void MainWindow::keyReleaseEvent( QKeyEvent *event )
{
    if( event->isAutoRepeat() )
        return;

    switch( event->key() ) {
    case Qt::Key_CameraFocus:
        //m_camera->unlock();
        break;
    default:
        QMainWindow::keyReleaseEvent( event );
    }
}

//============================================================================
void MainWindow::updateRecordTime()
{
    //QString str = QString( "Recorded %1 sec" ).arg( m_mediaRecorder->duration() / 1000 );
    //ui->statusbar->showMessage( str );
}

//============================================================================
void MainWindow::processCapturedImage( int requestId, const QImage& img )
{
    //Q_UNUSED( requestId );
    //QImage scaledImage = img.scaled( ui->viewfinder->size(),
    //                                 Qt::KeepAspectRatio,
    //                                 Qt::SmoothTransformation );

    //ui->lastImagePreviewLabel->setPixmap( QPixmap::fromImage( scaledImage ) );

    //QImage scaledImagePreview = img.scaled( GuiParams::getCamImageDefaultSize(),
    //                                 Qt::KeepAspectRatio,
    //                                 Qt::SmoothTransformation );
    //static int cnt = 0;
    //cnt++;
    //static uint64_t lastSnapshotTime = 0;
    //if( !lastSnapshotTime )
    //{
    //    lastSnapshotTime = GetGmtTimeMs();
    //}

    //uint64_t timeNow = GetGmtTimeMs();
    //LogMsg( LOG_DEBUG, "processCapturedImage %d null ? %d size x%d y%d %lldms", cnt, scaledImagePreview.isNull(), scaledImagePreview.width(), 
    //            scaledImagePreview.width(), timeNow - lastSnapshotTime );
    //lastSnapshotTime = timeNow;

    //ui->m_CamPreview->showScaledImage( scaledImagePreview );

    //m_isCapturingImage = false;
    //// Display captured image for 4 seconds.
    //displayCapturedImage();
    //QTimer::singleShot( 4000, this, &MainWindow::displayViewfinder );
}

//============================================================================
void MainWindow::configureCaptureSettings()
{
    configureImageSettings();
}

//============================================================================
void MainWindow::configureVideoSettings()
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
void MainWindow::configureImageSettings()
{
    m_CamLogic.configureImageSettings();
    
    //ImageSettings settingsDialog( m_imageCapture.data() );
    //settingsDialog.setWindowFlags( settingsDialog.windowFlags() & ~Qt::WindowContextHelpButtonHint );

    //settingsDialog.setImageSettings( m_imageSettings );

    //if( settingsDialog.exec() ) {
    //    m_imageSettings = settingsDialog.imageSettings();
    //    m_imageCapture->setEncodingSettings( m_imageSettings );
    //}
}

//============================================================================
void MainWindow::record()
{
    //m_mediaRecorder->record();
    //updateRecordTime();
}

//============================================================================
void MainWindow::pause()
{
    //m_mediaRecorder->pause();
}

//============================================================================
void MainWindow::stop()
{
    //m_mediaRecorder->stop();
}

//============================================================================
void MainWindow::setMuted( bool muted )
{
    //m_mediaRecorder->setMuted( muted );
}

//============================================================================
void MainWindow::toggleLock()
{
    //switch( m_camera->lockStatus() ) {
    //case QCamera::Searching:
    //case QCamera::Locked:
    //    m_camera->unlock();
    //    break;
    //case QCamera::Unlocked:
    //    m_camera->searchAndLock();
    //}
}

//============================================================================
void MainWindow::updateLockStatus( QCamera::LockStatus status, QCamera::LockChangeReason reason )
{
    QColor indicationColor = Qt::black;

    switch( status ) {
    case QCamera::Searching:
        indicationColor = Qt::yellow;
        ui->statusbar->showMessage( tr( "Focusing..." ) );
        ui->lockButton->setText( tr( "Focusing..." ) );
        break;
    case QCamera::Locked:
        indicationColor = Qt::darkGreen;
        ui->lockButton->setText( tr( "Unlock" ) );
        ui->statusbar->showMessage( tr( "Focused" ), 2000 );
        break;
    case QCamera::Unlocked:
        indicationColor = reason == QCamera::LockFailed ? Qt::red : Qt::black;
        ui->lockButton->setText( tr( "Focus" ) );
        if( reason == QCamera::LockFailed )
            ui->statusbar->showMessage( tr( "Focus Failed" ), 2000 );
    }

    QPalette palette = ui->lockButton->palette();
    palette.setColor( QPalette::ButtonText, indicationColor );
    ui->lockButton->setPalette( palette );
}

//============================================================================
void MainWindow::takeImage()
{
    //if( !m_isCapturingImage )
    //{
    //    m_isCapturingImage = true;
    //    m_imageCapture->capture();
    //}
}

//============================================================================
void MainWindow::displayCaptureError( int id, const QCameraImageCapture::Error error, const QString &errorString )
{
    Q_UNUSED( id );
    Q_UNUSED( error );
    QMessageBox::warning( this, tr( "Image Capture Error" ), errorString );
    //m_isCapturingImage = false;
}

//============================================================================
void MainWindow::startCamera()
{
    m_CamLogic.startCamera();
    //m_camera->start();
    //m_SnapshotTimer->start();
}

//============================================================================
void MainWindow::stopCamera()
{
    m_CamLogic.stopCamera();

    //m_SnapshotTimer->stop();
    //m_camera->stop();
}

//============================================================================
void MainWindow::updateCaptureMode( int )
{
    //int tabIndex = ui->captureWidget->currentIndex();
    //QCamera::CaptureModes captureMode = tabIndex == 0 ? QCamera::CaptureStillImage : QCamera::CaptureVideo;

    //if( m_camera->isCaptureModeSupported( captureMode ) )
    //    m_camera->setCaptureMode( captureMode );
}

//============================================================================
void MainWindow::updateCameraState( QCamera::State state )
{
    switch( state ) {
    case QCamera::ActiveState:
        ui->actionStartCamera->setEnabled( false );
        ui->actionStopCamera->setEnabled( true );
        ui->captureWidget->setEnabled( true );
        ui->actionSettings->setEnabled( true );
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
        ui->actionStartCamera->setEnabled( true );
        ui->actionStopCamera->setEnabled( false );
        ui->captureWidget->setEnabled( false );
        ui->actionSettings->setEnabled( false );
    }
}

//============================================================================
void MainWindow::updateRecorderState( QMediaRecorder::State state )
{
    //switch( state ) {
    //case QMediaRecorder::StoppedState:
    //    ui->recordButton->setEnabled( true );
    //    ui->pauseButton->setEnabled( true );
    //    ui->stopButton->setEnabled( false );
    //    break;
    //case QMediaRecorder::PausedState:
    //    ui->recordButton->setEnabled( true );
    //    ui->pauseButton->setEnabled( false );
    //    ui->stopButton->setEnabled( true );
    //    break;
    //case QMediaRecorder::RecordingState:
    //    ui->recordButton->setEnabled( false );
    //    ui->pauseButton->setEnabled( true );
    //    ui->stopButton->setEnabled( true );
    //    break;
    //}
}

//============================================================================
void MainWindow::setExposureCompensation( int index )
{
    //m_camera->exposure()->setExposureCompensation( index*0.5 );
}

//============================================================================
void MainWindow::displayRecorderError()
{
    //QMessageBox::warning( this, tr( "Capture Error" ), m_mediaRecorder->errorString() );
}

//============================================================================
void MainWindow::displayCameraError()
{
    //QMessageBox::warning( this, tr( "Camera Error" ), m_camera->errorString() );
}

//============================================================================
void MainWindow::updateCameraDevice( QAction *action )
{
    setCamera( qvariant_cast< QCameraInfo >( action->data() ) );
}

//============================================================================
void MainWindow::displayViewfinder()
{
    ui->stackedWidget->setCurrentIndex( 0 );
}

//============================================================================
void MainWindow::displayCapturedImage()
{
    ui->stackedWidget->setCurrentIndex( 1 );
}

//============================================================================
void MainWindow::readyForCapture( bool ready )
{
    ui->takeImageButton->setEnabled( ready );
}

//============================================================================
void MainWindow::imageSaved( int id, const QString &fileName )
{
    Q_UNUSED( id );
    ui->statusbar->showMessage( tr( "Captured \"%1\"" ).arg( QDir::toNativeSeparators( fileName ) ) );

    //m_isCapturingImage = false;
    if( m_CamLogic.getAppIsExiting() )
    {
        close();
    }
}

//============================================================================
void MainWindow::closeEvent( QCloseEvent *event )
{
    m_MyApp.getEngine().fromGuiWantMediaInput( eMediaInputVideoJpgSmall, this, this, false );

    if( m_CamLogic.setAppIsExiting( true ) ) 
    {
        setEnabled( false );
        event->ignore();
    }
    else 
    {
        VxSetAppIsShuttingDown( true );
        event->accept();
    }
}

//============================================================================
void MainWindow::callbackVideoJpgSmall( void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
    if( jpgData && jpgDataLen )
    {
        CamViewWidget* camWidget = getCamViewWidget();
        if( camWidget )
        {
            camWidget->playVideoFrame( vidFeedId, jpgData, jpgDataLen, motion0to100000 );
        }
    }
}

//============================================================================
CamViewWidget* MainWindow::getCamViewWidget()
{
    return ui->m_CamPlaybackWidget;
}
