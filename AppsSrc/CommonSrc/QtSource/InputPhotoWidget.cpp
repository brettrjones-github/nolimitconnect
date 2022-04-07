//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "InputPhotoWidget.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "ChatEntryWidget.h"
#include "GuiParams.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>

#include <QDebug>

#include <time.h>

//============================================================================
InputPhotoWidget::InputPhotoWidget( QWidget * parent )
: InputBaseWidget( GetAppInstance(), parent )
{
	m_AssetInfo.setAssetType( eAssetTypePhoto );
	qDebug() << "InputPhotoWidget::InputPhotoWidget ";

	ui.setupUi( this );
    ui.m_SnapShotButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_RotateCamButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_SelectCamButton->setSquareButtonSize( eButtonSizeTiny );
    ui.m_CancelPhotoButton->setSquareButtonSize( eButtonSizeTiny );
	ui.m_BackButton->setSquareButtonSize( eButtonSizeTiny );

	ui.m_SnapShotButton->setIcons( eMyIconCameraNormal );
	ui.m_RotateCamButton->setIcons( eMyIconCamRotateNormal );
	ui.m_SelectCamButton->setIcons( eMyIconCamSelectNormal );
	ui.m_CancelPhotoButton->setIcons( eMyIconCameraCancel );
	// TODO.. add camera selection
	ui.m_SelectCamButton->setEnabled( false );

    ui.m_BackButton->setIcons( eMyIconBack );

	connect( ui.m_SnapShotButton,				SIGNAL(clicked()),							this, SLOT(slotSnapShotButtonClicked()) );
	connect( ui.m_RotateCamButton,				SIGNAL(clicked()),							this, SLOT(slotRotateCamButtonClicked()) );
	connect( ui.m_SelectCamButton,				SIGNAL(clicked()),							this, SLOT(slotSelectCamButtonClicked()) );
	connect( ui.m_CancelPhotoButton,			SIGNAL(clicked()),							this, SLOT(slotCancelPhotoButtonClicked()) );
	connect( ui.m_VidWidget->getVideoScreen(),	SIGNAL(signalPlayVideoFrame( QPixmap,int)),	this, SLOT(slotVideoFrameBitmap( QPixmap,int)) );
    connect( ui.m_BackButton, SIGNAL( clicked() ), this, SLOT( slotExitPhotoWidget() ) );
}

//============================================================================
void InputPhotoWidget::slotSnapShotButtonClicked( void )
{
	if( m_PicImage.isNull() )
	{
		QMessageBox::warning(this, tr("Photo Snapshot Error"), tr("Photo Unavailable" ) );
	}
	else
	{
		m_AssetInfo.generateNewUniqueId();
		generateFileName( eAssetTypePhoto, m_AssetInfo.getAssetUniqueId() );
		m_AssetInfo.setAssetName( m_FileName );
		if( false == m_PicImage.save( m_FileName.c_str() ) )
		{
			QString msgText = tr("Failed to write photo to file ") + m_FileName.c_str();
			QMessageBox::warning(this, tr("Error Writing Photo To File"), msgText);
		}
		else
		{
			uint64_t fileLen = VxFileUtil::getFileLen( m_FileName.c_str() );
			if( 1000 > fileLen )
			{
				// not long enough to be a photo
				VxFileUtil::deleteFile( m_FileName.c_str() );
				QMessageBox::warning(this, tr("Photo Snapshot Error"), tr("Photo File Not Found" ) );
			}
			else
			{
				m_AssetInfo.setAssetLength( fileLen );
				m_TimeRecStart		= GetTimeStampMs();
				m_TimeRecCurrent	= m_TimeRecStart;
				m_AssetInfo.setCreationTime( m_TimeRecCurrent );
				m_MyApp.getEngine().fromGuiAssetAction( m_IsPersonalRecorder ? eAssetActionAddToAssetMgr : eAssetActionAddAssetAndSend,  m_AssetInfo );
			}
		}
	}

	emit signalInputCompleted();
}

//============================================================================
void InputPhotoWidget::slotRotateCamButtonClicked( void )
{
	uint32_t camId = m_MyApp.getAppSettings().getCamSourceId();
	int camRotation = m_MyApp.getAppSettings().getCamRotation( camId );
	camRotation += 90;
	if( camRotation >= 360 )
	{
		camRotation = 0;
	}

	m_MyApp.getAppSettings().setCamRotation( camId, camRotation );
	m_MyApp.setCamCaptureRotation( camRotation );
}

//============================================================================
void InputPhotoWidget::slotSelectCamButtonClicked( void )
{
}

//============================================================================
void InputPhotoWidget::slotCancelPhotoButtonClicked( void )
{
	emit signalInputCompleted();
}

//============================================================================
void InputPhotoWidget::hideEvent(QHideEvent * hideEvent)
{
	InputBaseWidget::hideEvent(hideEvent);
	if( ( 0 != m_MyIdent ) && ( false == VxIsAppShuttingDown() ) )
	{
		m_MyApp.getEngine().fromGuiWantMediaInput( m_AssetInfo.getHistoryId(), eMediaInputVideoJpgBig, false );
	}
}

//============================================================================
void InputPhotoWidget::showEvent(QShowEvent * showEvent)
{
	InputBaseWidget::showEvent(showEvent);
	if( ( 0 != m_MyIdent ) && ( false == VxIsAppShuttingDown() ) )
	{
		ui.m_VidWidget->setVideoFeedId( m_AssetInfo.getCreatorId() );
		m_MyApp.getEngine().fromGuiWantMediaInput( m_AssetInfo.getHistoryId(), eMediaInputVideoJpgBig, true );
	}
}

//============================================================================
void InputPhotoWidget::playVideoFrame( VxGUID& onlineId, unsigned char * pu8Jpg, unsigned long u32JpgLen, int motion0To100000 )
{
	ui.m_VidWidget->playVideoFrame( onlineId, pu8Jpg, u32JpgLen, motion0To100000 );
}

//============================================================================
void InputPhotoWidget::slotVideoFrameBitmap( QPixmap picBitmap, int iRotate )
{
	m_PicImage = picBitmap;
}

//============================================================================
void InputPhotoWidget::slotExitPhotoWidget( void )
{
    this->setVisible( false );
    m_ChatEntryWidget->setEntryMode( eAssetTypeUnknown );
    m_ChatEntryWidget->setVisible( true );
}
