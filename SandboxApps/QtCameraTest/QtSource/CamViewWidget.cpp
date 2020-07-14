//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "CamViewWidget.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTimeUtil.h>
#include <CoreLib/VxFileUtil.h>

#include <QDebug>
#include <QTimer>
#include <QFile>

#include <time.h>

namespace
{
	const int		MOTION_ALARM_EXPIRE_MS		= 5000;
	const int		MOTION_RECORD_EXPIRE_MS		= 30000;
}

//============================================================================
CamViewWidget::CamViewWidget(QWidget *parent)
: QWidget(parent)
, m_ThumbnailPreview( 0 )
, m_DisablePreview( false )
, m_DisableRecordControls( false )
, m_RecFriendName( "" )
, m_RecFilePath( "" )
, m_RecFileName( "" )
, m_IconToggle( false )
, m_IconToggleTimer( new QTimer( this ) )
, m_MotionAlarmOn( false )
, m_MotionAlarmDetected( false )
, m_MotionAlarmExpireTimer( new QTimer( this ) )
, m_MotionRecordOn( false )
, m_MotionRecordDetected( false )
, m_MotionRecordExpireTimer( new QTimer( this ) )
, m_InNormalRecord( false )
{
    setObjectName( "VxWidget" );
	ui.setupUi(this);
	showAllControls( false );
	m_ThumbnailPreview = new VxLabel( ui.m_VideoScreen );
    ui.m_VideoFrame->setThumbnailPreview( m_ThumbnailPreview );
    ui.m_VideoFrame->setVideoScreen( ui.m_VideoScreen );
    m_ThumbnailPreview->setImageFromFile( ":/AppRes/Resources/web_cam_buffering.png" );

	ui.m_CamSourceButton->setEnabled( false );

	ui.m_VidFilesButton->setEnabled( false );

	ui.m_MotionBar->setMaximum( 100000 );
	ui.m_MotionBar->setTextVisible(false);
	ui.m_MotionSensitivitySlider->setMaximum( 100000 );

	connect( ui.m_VidFilesButton,		SIGNAL(clicked()),						this, SLOT(slotVidFilesButtonClicked()) );
	connect( ui.m_PictureSnapshotButton,SIGNAL(clicked()),						this, SLOT(slotPictureSnapshotButton()) );
	connect( ui.m_MotionAlarmButton,	SIGNAL(clicked()),						this, SLOT(slotMotionAlarmButtonClicked()) );
	connect( ui.m_MotionRecordButton,	SIGNAL(clicked()),						this, SLOT(slotRecMotionButtonClicked()) );
	connect( ui.m_NormalRecordButton,	SIGNAL(clicked()),						this, SLOT(slotRecNormalButtonClicked()) );
	connect( this,						SIGNAL(signalVidFeedMotion(int)),		this, SLOT(slotVidFeedMotion(int)) );

	connect( m_IconToggleTimer,			SIGNAL(timeout()),			this, SLOT(slotIconToggleTimeout()) );
	connect( m_MotionAlarmExpireTimer,	SIGNAL(timeout()),			this, SLOT(slotMotionAlarmTimeout()) );
	connect( m_MotionRecordExpireTimer,	SIGNAL(timeout()),			this, SLOT(slotMotionRecordTimeout()) );

	connect( ui.m_VideoScreen,			SIGNAL(clicked()),			this, SLOT(slotWidgetClicked()) );	
	connect( m_ThumbnailPreview,		SIGNAL(clicked()),			this, SLOT(slotWidgetClicked()) );	
	connect( ui.m_CamPreviewButton,		SIGNAL(clicked()),			this, SLOT(slotCamPreviewButtonClicked()) );	
	connect( ui.m_CamRotateButton,		SIGNAL(clicked()),			this, SLOT(slotCamRotateButtonClicked()) );	
	connect( ui.m_ImageRotateButton,	SIGNAL(clicked()),			this, SLOT(slotFeedRotateButtonClicked()) );

    showUserMsgLabel( false );
	showOfflineImage();
	updateVidFeedImageRotation();
}

//============================================================================
VxLabel * CamViewWidget::getVideoScreen( void )
{
	return ui.m_VideoScreen;
}

//============================================================================
void CamViewWidget::setAspectRatio( float aspectRatio )
{
    ui.m_VideoFrame->setVideoScreenAspectRatio( aspectRatio );
}

//============================================================================
void CamViewWidget::setVideoFeedId( VxGUID& feedOnlineId )				
{ 
    m_VideoFeedId = feedOnlineId;
}

//============================================================================
void CamViewWidget::showOfflineImage( void )
{
	ui.m_VideoScreen->setImageFromFile( ":/AppRes/Resources/web_cam_buffering.png" );
}

//============================================================================
void CamViewWidget::showUserMsgLabel( bool showCtrls )
{
    ui.m_UserMsgLabel->setVisible( showCtrls );
}

//============================================================================
void CamViewWidget::showAllControls( bool showCtrls )
{
	showFeedControls( showCtrls );
	showRecordControls( showCtrls );
	showMotionSensitivityControls( showCtrls );
}

//============================================================================
void CamViewWidget::enableVidFilesButton( bool enable )
{
	ui.m_VidFilesButton->setEnabled( enable );
}

//============================================================================
void CamViewWidget::enableCamSourceControls( bool enable )
{
	ui.m_CamRotateButton->setEnabled( enable );
	ui.m_CamPreviewButton->setEnabled( enable );
}

//============================================================================
void CamViewWidget::enableCamFeedControls( bool enable )
{
	ui.m_ImageRotateButton->setEnabled( enable );
}

//============================================================================
void CamViewWidget::showFeedControls( bool showCtrls )
{
	ui.m_VidCtrlFrame->setVisible( showCtrls );
}

//============================================================================
bool CamViewWidget::isFeedControlsVisible( void )
{
	return ui.m_VidCtrlFrame->isVisible();
}

//============================================================================
void CamViewWidget::showRecordControls( bool showCtrls )
{
	ui.m_RecordCtrlFrame->setVisible( showCtrls );
	showMotionSensitivityControls( showCtrls );
}

//============================================================================
bool CamViewWidget::isRecordControlsVisible( void )
{
	return ui.m_RecordCtrlFrame->isVisible();
}

//============================================================================
void CamViewWidget::showMotionSensitivityControls( bool showCtrls )
{
	ui.m_RecordSensitivityFrame->setVisible( showCtrls );
}

//============================================================================
bool CamViewWidget::isMotionSensitivityControlsVisible( void )
{
	return ui.m_RecordSensitivityFrame->isVisible();
}

//============================================================================
void CamViewWidget::setVidImageRotation( int imageRotation )
{
	ui.m_VideoScreen->setVidImageRotation( imageRotation );
}

//============================================================================
int CamViewWidget::getVidImageRotation( void )
{
	return ui.m_VideoScreen->getVidImageRotation();
}

//============================================================================
void CamViewWidget::slotStatusMsg( QString userMsg )
{
	ui.m_UserMsgLabel->setText( userMsg );
}

//============================================================================
void CamViewWidget::slotWidgetClicked( void )
{
	emit clicked();
	bool visState = ! isFeedControlsVisible();
	showFeedControls( visState );
	if( !m_DisableRecordControls )
	{
		showRecordControls( visState );
	}
}

//============================================================================
void CamViewWidget::slotCamPreviewButtonClicked( void )
{
	//if( !m_DisablePreview )
	//{
	//	bool isVisible = m_AppSettings.getCamShowPreview();
	//	m_AppSettings.setCamShowPreview( !isVisible );
	//	updatePreviewVisibility();
	//	if( !isVisible && ! m_DisablePreview )
	//	{
	//		m_MyApp.toGuiUserMessage( "Cam Preview Enabled" );
	//	}
	//	else
	//	{
	//		m_MyApp.toGuiUserMessage( "Cam Preview Disabled" );
	//	}
	//}
}

//============================================================================
void CamViewWidget::updatePreviewVisibility( void )
{
	//bool showPreview = m_AppSettings.getCamShowPreview();
	//if( m_DisablePreview )
	//{
	//	showPreview = false;
	//}

	//m_ThumbnailPreview->setVisible( showPreview );
	//if( showPreview )
	//{
	//	ui.m_CamPreviewButton->setIcon( eMyIconCamPreviewCancelNormal );
	//	if( m_MyOnlineId != m_VideoFeedId )
	//	{
	//		m_Engine.fromGuiWantMediaInput( m_MyOnlineId, eMediaInputVideoJpgSmall, true );
	//	}
	//}
	//else
	//{
	//	ui.m_CamPreviewButton->setIcons( eMyIconCamPreviewNormal );
	//	if( m_MyOnlineId != m_VideoFeedId )
	//	{
	//		m_Engine.fromGuiWantMediaInput( m_MyOnlineId, eMediaInputVideoJpgSmall, false );
	//	}
	//}
}

//============================================================================
void CamViewWidget::slotFeedRotateButtonClicked( void )
{
	//int feedRotation = m_AppSettings.getVidFeedRotation();
	//feedRotation += 90;
	//if( feedRotation >= 360 )
	//{
	//	feedRotation = 0;
	//}

	//m_AppSettings.setVidFeedRotation( feedRotation );
	//updateVidFeedImageRotation();
	//m_MyApp.toGuiUserMessage( "Contact Feed Rotation %d", feedRotation );
}

//============================================================================
void CamViewWidget::updateVidFeedImageRotation( void )
{
	//int feedRotation = m_AppSettings.getVidFeedRotation();
	//setVidImageRotation( feedRotation );
}

//============================================================================
void CamViewWidget::slotCamRotateButtonClicked( void )
{
	//uint32_t camId = m_AppSettings.getCamSourceId();
	//int camRotation = m_AppSettings.getCamRotation( camId );
	//camRotation += 90;
	//if( camRotation >= 360 )
	//{
	//	camRotation = 0;
	//}

	//m_AppSettings.setCamRotation( camId, camRotation );
	//m_MyApp.setCamCaptureRotation( camRotation );
	//m_MyApp.toGuiUserMessage( "My Cam Rotation %d", camRotation );
}

//============================================================================
void CamViewWidget::disablePreview( bool disable )
{
	m_DisablePreview = disable;
	if( m_DisablePreview )
	{
		m_ThumbnailPreview->setVisible( false );
		ui.m_CamPreviewButton->setVisible( false );
	}
	else
	{
		ui.m_CamPreviewButton->setVisible( true );
		updatePreviewVisibility();
	}
}

//============================================================================
void CamViewWidget::setImageFromFile( QString fileName )
{
	ui.m_VideoScreen->setImageFromFile( fileName );
}

//============================================================================
void CamViewWidget::playVideoFrame( VxGUID&	feedOnlineId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
	if( feedOnlineId == m_VideoFeedId )
	{
		ui.m_VideoScreen->playVideoFrame( pu8Jpg, u32JpgLen, motion0To100000 );
		if( ui.m_RecordSensitivityFrame->isVisible() )
		{
			emit signalVidFeedMotion( motion0To100000 );
		}
	}
	else if( !m_DisablePreview
			&& m_ThumbnailPreview->isVisible() 
			&& ( feedOnlineId == m_MyOnlineId ) )
	{
		m_ThumbnailPreview->playVideoFrame( pu8Jpg, u32JpgLen, motion0To100000 );
	}
}

//============================================================================
int CamViewWidget::playVideoFrame( VxGUID& feedOnlineId, uint8_t * picBuf, uint32_t picBufLen, int picWidth, int picHeight )
{
    int behindFrameCnt = 0;
    //if( feedOnlineId == m_VideoFeedId )
    {
        behindFrameCnt = ui.m_VideoScreen->playVideoFrame( picBuf, picBufLen, picWidth, picHeight );
    }

    return behindFrameCnt;
}

//============================================================================
void CamViewWidget::showEvent( QShowEvent * ev )
{
	QWidget::showEvent( ev );
	updatePreviewVisibility();
	//if( m_VideoFeedId.isVxGUIDValid() )
	//{
	//	m_Engine.fromGuiWantMediaInput( m_VideoFeedId, eMediaInputVideoJpgSmall, true );
	//}
}

//============================================================================
void CamViewWidget::hideEvent( QHideEvent * ev )
{
	QWidget::hideEvent( ev );
	//if( m_VideoFeedId.isVxGUIDValid() )
	//{
	//	m_Engine.fromGuiWantMediaInput( m_VideoFeedId, eMediaInputVideoJpgSmall, false );
	//}
}

//============================================================================
void CamViewWidget::slotIconToggleTimeout( void )
{
	//ui.m_MotionAlarmButton->setNotifyEnabled( m_MotionAlarmDetected  );
	//ui.m_MotionRecordButton->setNotifyEnabled( m_MotionRecordDetected );
	//ui.m_NormalRecordButton->setNotifyEnabled( m_InNormalRecord );
}

//============================================================================
void CamViewWidget::slotMotionAlarmTimeout( void )
{
	m_MotionAlarmExpireTimer->stop();
	m_MotionAlarmDetected = false;
	//ui.m_MotionAlarmButton->setNotifyEnabled( false );
	//ui.m_MotionAlarmButton->setIcon( m_MotionAlarmOn ? eMyIconMotionAlarmRed : eMyIconMotionAlarmWhite );

	updateMotionBarColor();
}

//============================================================================
void CamViewWidget::slotMotionRecordTimeout( void )
{
	m_MotionRecordExpireTimer->stop();
	m_MotionRecordDetected = false;
	//ui.m_MotionRecordButton->setNotifyEnabled( false );
	//if( m_MotionRecordOn )
	//{
	//	m_Engine.fromGuiVideoRecord( eVideoRecordStatePauseRecording, m_VideoFeedId, m_RecFileName.toUtf8().constData() );
	//}

	updateMotionBarColor();
}

//============================================================================
void CamViewWidget::slotVidFeedMotion( int motion0To100000 )
{
	//qDebug() << "motion " << motion0To100000;
	ui.m_MotionBar->setValue( motion0To100000 );
	//if( motion0To100000 >= ui.m_MotionSensitivitySlider->value() )
	//{
	//	if( m_MotionRecordOn )
	//	{
	//		m_MotionRecordExpireTimer->start( MOTION_RECORD_EXPIRE_MS );
	//		if( !m_MotionRecordDetected )
	//		{
	//			m_MotionRecordDetected = true;
	//			m_Engine.fromGuiVideoRecord( eVideoRecordStateResumeRecording, m_VideoFeedId, m_RecFileName.toUtf8().constData() );
	//			m_MyApp.toGuiUserMessage( "Video Motion Record Resumed" );
	//			updateMotionBarColor();
	//			ui.m_MotionRecordButton->setNotifyEnabled( true );
	//		}
	//	}

	//	if( m_MotionAlarmOn )
	//	{
	//		m_MotionAlarmExpireTimer->start( MOTION_ALARM_EXPIRE_MS );
	//		if( !m_MotionAlarmDetected )
	//		{
	//			m_MotionAlarmDetected = true;
	//			playMotionAlarm();
	//			updateMotionBarColor();
	//			ui.m_MotionAlarmButton->setNotifyEnabled( true );
	//		}
	//	}
	//}
}

//============================================================================
void CamViewWidget::updateMotionBarColor( void )
{
	if( m_MotionAlarmDetected || m_MotionRecordDetected )
	{
		ui.m_MotionBar->setStyleSheet( "QProgressBar {background-color: #FF0000;}" );
	}
	else
	{
		ui.m_MotionBar->setStyleSheet( "QProgressBar {background-color: #00FF00;}" );
	}
}

//============================================================================
void CamViewWidget::playMotionAlarm( void )
{
	//m_MyApp.playSound( eSndDefAlarmPleasant );
}

//============================================================================
void CamViewWidget::slotVidFilesButtonClicked( void )
{
	//if( !m_MotionRecordOn
	//	&& !m_InNormalRecord )
	//{
	//	m_MyApp.launchLibraryActivity( VXFILE_TYPE_VIDEO );
	//}
	//else
	//{
	//	LogMsg( LOG_ERROR, "CamViewWidget::slotVidFilesButtonClicked dont launch Library Activity while recording\n" );
	//}
}

//============================================================================
void CamViewWidget::slotPictureSnapshotButton( void )
{
	QImage photoImage = getVideoScreen()->getLastVideoImage();
	if( photoImage.isNull() )
	{
		//m_MyApp.playSound( eSndDefIgnore );
		LogMsg( LOG_ERROR, "CamViewWidget::slotPictureSnapshotButton video image is null\n" );
		//m_MyApp.toGuiUserMessage( "No Image To Take A Snap Shot Of" );
	}
	else
	{
		//m_MyApp.playSound( eSndDefCameraClick );
		time_t timeNow = time( 0 );
		QString photoFileName = m_RecFilePath + m_RecFriendName + VxTimeUtil::getFileNameCompatibleDateAndTime( timeNow ).c_str();
		photoFileName += ".png";
		QFile photoFile(photoFileName);
		photoFile.open(QIODevice::WriteOnly);
		photoImage.save(&photoFile, "PNG");
		photoFile.close();
		uint64_t fileLen = VxFileUtil::getFileLen( photoFileName.toUtf8().constData() );
		if( 500 < fileLen )
		{
			//m_MyApp.getEngine().fromGuiAddFileToLibrary( photoFileName.toUtf8().constData(), true );
			//m_MyApp.toGuiUserMessage( "Snapshot added to library %s", photoFileName.toUtf8().constData() );
		}
		else
		{
			//m_MyApp.toGuiUserMessage( "ERROR: Snapshot file create failed" );
		}
	}
}

//============================================================================
void CamViewWidget::slotMotionAlarmButtonClicked( void )
{
	if( m_MotionAlarmOn )
	{
		m_MotionAlarmExpireTimer->stop();
		m_MotionAlarmDetected = false;
	}

	m_MotionAlarmOn = !m_MotionAlarmOn;
	//ui.m_MotionAlarmButton->setNotifyEnabled( false );
	//ui.m_MotionAlarmButton->setIcon( m_MotionAlarmOn ? eMyIconMotionAlarmRed : eMyIconMotionAlarmWhite );
	if( m_MotionAlarmOn )
	{
		showMotionSensitivityControls( true );
		//m_MyApp.toGuiUserMessage( "Motion Alarm Enabled" );
	}
	else
	{
		//m_MyApp.toGuiUserMessage( "Motion Alarm Disabled" );
		//if( false == m_MotionRecordOn )
		//{
		//	showMotionSensitivityControls( false );
		//}
	}
}

//============================================================================
void CamViewWidget::slotRecMotionButtonClicked( void )
{
	if( !m_InNormalRecord )
	{
		if( !m_RecFilePath.isEmpty() )
		{
			//ui.m_MotionRecordButton->setNotifyEnabled( false );
			if( m_MotionRecordOn )
			{
				//ui.m_MotionRecordButton->setIcon( eMyIconRecordMotionNormal );
				m_MotionRecordOn = false;
				//m_Engine.fromGuiVideoRecord( eVideoRecordStateStopRecording, m_VideoFeedId, m_RecFileName.toUtf8().constData() );
				uint64_t fileLen = VxFileUtil::getFileLen( m_RecFileName.toUtf8().constData() );
				if( 5000 > fileLen )
				{
					// not long enough to be a recording
					VxFileUtil::deleteFile( m_RecFileName.toUtf8().constData() );
					LogMsg( LOG_ERROR, "CamViewWidget::videoMotionRecord file %s has to short len %lld\n", m_RecFileName.toUtf8().constData(), fileLen );
					//m_MyApp.toGuiUserMessage( "ERROR: Motion video record file was too short" );
				}
				else
				{
					//m_MyApp.getEngine().fromGuiAddFileToLibrary( m_RecFileName.toUtf8().constData(), true );
					//m_MyApp.toGuiUserMessage( "Added motion video to library %s", m_RecFileName.toUtf8().constData() );
					enableVidFilesButton( true );
				}
			}
			else
			{
				enableVidFilesButton( false );
				m_MotionRecordDetected = false;
				m_MotionRecordOn = true;
				time_t timeNow = time( 0 );
				m_RecFileName = m_RecFilePath + m_RecFriendName + VxTimeUtil::getFileNameCompatibleDateAndTime( timeNow ).c_str();
				m_RecFileName += ".avi";
				//m_Engine.fromGuiVideoRecord( eVideoRecordStateStartRecordingInPausedState, m_VideoFeedId, m_RecFileName.toUtf8().constData() );
				//ui.m_MotionRecordButton->setNotifyEnabled( true );
				//m_MyApp.toGuiUserMessage( "Video Motion Record Started" );
			}
		}
	}
	else
	{
		//m_MyApp.toGuiUserMessage( "Already recording video without motion detect" );
	}

	//if( !m_RecFilePath.isEmpty() )
	//{
	//	if( m_MotionAlarmOn )
	//	{
	//		showMotionSensitivityControls( true );
	//	}
	//	else
	//	{
	//		if( false == m_MotionAlarmOn )
	//		{
	//			showMotionSensitivityControls( false );
	//		}
	//	}
	//}
}

//============================================================================
void CamViewWidget::slotRecNormalButtonClicked( void )
{
	//if( !m_MotionRecordOn )
	//{
	//	if( !m_RecFilePath.isEmpty() )
	//	{
	//		if( m_InNormalRecord )
	//		{
	//			ui.m_NormalRecordButton->setIcon( eMyIconCamcorderNormal );
	//			m_InNormalRecord = false;

	//			m_Engine.fromGuiVideoRecord( eVideoRecordStateStopRecording, m_VideoFeedId, m_RecFileName.toUtf8().constData() );
	//			uint64_t fileLen = VxFileUtil::getFileLen( m_RecFileName.toUtf8().constData() );
	//			if( 5000 > fileLen )
	//			{
	//				// not long enough to be a recording
	//				VxFileUtil::deleteFile( m_RecFileName.toUtf8().constData() );
	//				LogMsg( LOG_ERROR, "CamViewWidget::videoRecord file %s has to short len %lld\n", m_RecFileName.toUtf8().constData(), fileLen );
	//				m_MyApp.toGuiUserMessage( "ERROR: Video record file was too short" );
	//			}
	//			else
	//			{
	//				m_MyApp.getEngine().fromGuiAddFileToLibrary( m_RecFileName.toUtf8().constData(), true );
	//				m_MyApp.toGuiUserMessage( "Added Video Recording to library %s", m_RecFileName.toUtf8().constData() );
	//				enableVidFilesButton( true );
	//			}
	//		}
	//		else
	//		{
	//			enableVidFilesButton( false );
	//			m_InNormalRecord = true;
	//			time_t timeNow = time( 0 );
	//			m_RecFileName = m_RecFilePath + m_RecFriendName + VxTimeUtil::getFileNameCompatibleDateAndTime( timeNow ).c_str();
	//			m_RecFileName += ".avi";
	//			m_Engine.fromGuiVideoRecord( eVideoRecordStateStartRecording, m_VideoFeedId, m_RecFileName.toUtf8().constData() );
	//			ui.m_NormalRecordButton->setNotifyEnabled( true );
	//			m_MyApp.toGuiUserMessage( "Starting Video Record" );
	//		}
	//	}
	//}
}
