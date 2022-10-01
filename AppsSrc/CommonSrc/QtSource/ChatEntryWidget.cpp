//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "AppCommon.h"
#include "AppSettings.h"
#include "ChatEntryWidget.h"

#include <CoreLib/VxFileUtil.h>

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <time.h>

//============================================================================
ChatEntryWidget::ChatEntryWidget( QWidget*parent, EAssetType inputMode )
: QWidget( parent )
, m_InputMode( inputMode )
, m_MyIdent( 0 )
, m_HisIdent( 0 )
{
	qDebug() << "ChatEntryWidget input mode " << inputMode;
	ui.setupUi(this);

	connect( ui.m_InputAllWidget,		SIGNAL(signalUserInputButtonClicked()),		this, SIGNAL(signalUserInputButtonClicked()) );

	connect( ui.m_InputAllWidget,		SIGNAL(signalAllTextButtonClicked()),		this, SLOT(slotAllTextButtonClicked()) );
	connect( ui.m_InputAllWidget,		SIGNAL(signalAllEmoteButtonClicked()),		this, SLOT(slotAllEmoteButtonClicked()) );
	connect( ui.m_InputAllWidget,		SIGNAL(signalAllVideoButtonClicked()),		this, SLOT(slotAllVideoButtonClicked()) );
	connect( ui.m_InputAllWidget,		SIGNAL(signalAllCameraButtonClicked()),		this, SLOT(slotAllCameraButtonClicked()) );
	connect( ui.m_InputAllWidget,		SIGNAL(signalAllGalleryButtonClicked()),	this, SLOT(slotAllGalleryButtonClicked()) );
	connect( ui.m_InputAllWidget,		SIGNAL(signalAllMicButtonPressed()),		this, SLOT(slotAllMicButtonPressed()) );
	connect( ui.m_InputAllWidget,		SIGNAL(signalAllMicButtonReleased()),		this, SLOT(slotAllMicButtonReleased()) );

	connect( ui.m_InputPhotoWidget,		SIGNAL(signalInputCompleted()),				this, SLOT(slotInputCompleted()) );
	connect( ui.m_InputTextWidget,		SIGNAL(signalInputCompleted()),				this, SLOT(slotInputCompleted()) );
	connect( ui.m_InputFaceWidget,		SIGNAL(signalInputCompleted()),				this, SLOT(slotInputCompleted()) );
	connect( ui.m_InputVideoWidget,		SIGNAL(signalInputCompleted()),				this, SLOT(slotInputCompleted()) );
	connect( ui.m_InputVoiceWidget,		SIGNAL(signalInputCompleted()),				this, SLOT(slotInputCompleted()) );

	setEntryMode( m_InputMode );
}

//============================================================================
void ChatEntryWidget::setAppModule( EAppModule appModule )
{
	m_AppModule = appModule;

	ui.m_InputAllWidget->setAppModule( appModule );
	ui.m_InputFaceWidget->setAppModule( appModule );
	ui.m_InputPhotoWidget->setAppModule( appModule );
	ui.m_InputTextWidget->setAppModule( appModule );
	ui.m_InputVideoWidget->setAppModule( appModule );
	ui.m_InputVoiceWidget->setAppModule( appModule );
}

//============================================================================
void ChatEntryWidget::setIdents( GuiUser * myIdent, GuiUser * hisIdent )
{
	m_MyIdent		= myIdent;
	m_HisIdent		= hisIdent;

	ui.m_InputAllWidget->setIdents( myIdent, hisIdent );
	ui.m_InputFaceWidget->setIdents( myIdent, hisIdent );
	ui.m_InputPhotoWidget->setIdents( myIdent, hisIdent );
	ui.m_InputTextWidget->setIdents( myIdent, hisIdent );
	ui.m_InputVideoWidget->setIdents( myIdent, hisIdent );
	ui.m_InputVoiceWidget->setIdents( myIdent, hisIdent );
}

//============================================================================
void ChatEntryWidget::setCanSend( bool canSend )
{
	if( false == canSend )
	{
		setEntryMode( eAssetTypeUnknown );
	}

	ui.m_InputAllWidget->setCanSend( canSend );
	ui.m_InputFaceWidget->setCanSend( canSend );
	ui.m_InputPhotoWidget->setCanSend( canSend );
	ui.m_InputTextWidget->setCanSend( canSend );
	ui.m_InputVideoWidget->setCanSend( canSend );
	ui.m_InputVoiceWidget->setCanSend( canSend );
}

//============================================================================
void ChatEntryWidget::callbackGuiPlayMotionVideoFrame( VxGUID& feedOnlineId, QImage& vidFrame, int motion0To100000 )
{
	switch( m_InputMode )
	{
	case eAssetTypeVideo:
		ui.m_InputVideoWidget->callbackGuiPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
		break;
	case eAssetTypePhoto:
		ui.m_InputPhotoWidget->callbackGuiPlayMotionVideoFrame( feedOnlineId, vidFrame, motion0To100000 );
		break;
	default:
		break;
	}
}

//============================================================================
void ChatEntryWidget::slotAllTextButtonClicked( void )
{
	setEntryMode( eAssetTypeChatText );	
}

//============================================================================
void ChatEntryWidget::slotAllEmoteButtonClicked( void )
{
	setEntryMode( eAssetTypeChatFace );	
}

//============================================================================
void ChatEntryWidget::slotAllVideoButtonClicked( void )
{
	setEntryMode( eAssetTypeVideo );	
}

//============================================================================
void ChatEntryWidget::slotAllCameraButtonClicked( void )
{
	setEntryMode( eAssetTypePhoto );	
}

#define SUPPORTED_IMAGE_FILES "Image files (*.bmp *.jpg *.jpeg *.png)"

//============================================================================
void ChatEntryWidget::slotAllGalleryButtonClicked( void )
{
	if( ( 0 == m_MyIdent ) 
		||  ( 0 == m_HisIdent ) )
	{
		return;
	}

	AppCommon& myApp = GetAppInstance();
	QString startPath = QDir::current().path();
	std::string lastGalleryPath;
	myApp.getAppSettings().getLastGalleryDir( lastGalleryPath );
	if( ( 0 != lastGalleryPath.length() )
		&& ( VxFileUtil::directoryExists( lastGalleryPath.c_str() ) ) )
	{
		startPath = lastGalleryPath.c_str();
	}

	// Get a filename from the file dialog.
	QString filename = QFileDialog::getOpenFileName(	this,
		QObject::tr("Open Image"),
		startPath,
		SUPPORTED_IMAGE_FILES);

	if( filename.length() > 0 )
	{
		QPixmap oBitmap;
		if( false == oBitmap.load(filename) )
		{
			QString msgText = QObject::tr("Failed To Read Image File ") + filename;
			QMessageBox::critical( this, QObject::tr("Error Reading Image"), msgText );
		}
		else
		{
			std::string justFileName;
			VxFileUtil::seperatePathAndFile( filename.toUtf8().constData(), lastGalleryPath, justFileName );
			if( ( 0 != lastGalleryPath.length() )
				&& ( VxFileUtil::directoryExists( lastGalleryPath.c_str() ) ) )
			{
				myApp.getAppSettings().setLastGalleryDir( lastGalleryPath );
			}

			AssetInfo assetInfo;
			assetInfo.setAssetType( eAssetTypePhoto );
			assetInfo.setAssetLength( VxFileUtil::getFileLen( filename.toUtf8().constData() ) );
			if( 0 == assetInfo.getAssetLength() )
			{
				LogMsg( LOG_ERROR, "InputBaseWidget::photo file has zero len %s\n", filename.toUtf8().constData() );
				return;
			}

			assetInfo.setAssetName( filename.toUtf8().constData() );
			assetInfo.setCreatorId( m_MyIdent->getMyOnlineId() );
			assetInfo.setHistoryId( m_HisIdent->getMyOnlineId() );
			assetInfo.generateNewUniqueId();
			assetInfo.setCreationTime( GetTimeStampMs() );
			GetAppInstance().getEngine().fromGuiAssetAction( eAssetActionAddToAssetMgr,  assetInfo );
		}
	}	
}

//============================================================================
void ChatEntryWidget::slotAllMicButtonPressed( void )
{
	setEntryMode( eAssetTypeAudio );	
}

//============================================================================
void ChatEntryWidget::slotInputCompleted( void )
{
	setEntryMode( eAssetTypeUnknown );	
    setVisible( true );
}

//============================================================================
void ChatEntryWidget::slotAllMicButtonReleased( void )
{
	//cancelInput();
}

//============================================================================
void ChatEntryWidget::setEntryMode( EAssetType inputMode )
{
	switch( inputMode )
	{
	case eAssetTypeChatText:
		ui.m_InputAllWidget->setVisible( false );
		ui.m_InputFaceWidget->setVisible( false );
		ui.m_InputPhotoWidget->setVisible( false );
		ui.m_InputVoiceWidget->setVisible( false );
		ui.m_InputVideoWidget->setVisible( false );
		ui.m_InputTextWidget->setVisible( true );
		break;

	case eAssetTypeChatFace:
		ui.m_InputAllWidget->setVisible( false );
		ui.m_InputTextWidget->setVisible( false );
		ui.m_InputPhotoWidget->setVisible( false );
		ui.m_InputVoiceWidget->setVisible( false );
		ui.m_InputVideoWidget->setVisible( false );
		ui.m_InputFaceWidget->setVisible( true );
		break;

	case eAssetTypePhoto:
		ui.m_InputAllWidget->setVisible( false );
		ui.m_InputTextWidget->setVisible( false );
		ui.m_InputFaceWidget->setVisible( false );
		ui.m_InputVoiceWidget->setVisible( false );
		ui.m_InputVideoWidget->setVisible( false );
		ui.m_InputPhotoWidget->setVisible( true );
		break;

	case eAssetTypeAudio:
		ui.m_InputAllWidget->setVisible( false );
		ui.m_InputTextWidget->setVisible( false );
		ui.m_InputFaceWidget->setVisible( false );
		ui.m_InputPhotoWidget->setVisible( false );
		ui.m_InputVideoWidget->setVisible( false );
		ui.m_InputVoiceWidget->setVisible( true );
		break;

	case eAssetTypeVideo:
		ui.m_InputAllWidget->setVisible( false );
		ui.m_InputTextWidget->setVisible( false );
		ui.m_InputFaceWidget->setVisible( false );
		ui.m_InputPhotoWidget->setVisible( false );
		ui.m_InputVoiceWidget->setVisible( false );
		ui.m_InputVideoWidget->setVisible( true );
		break;

	default:

		if( eAssetTypeVideo == m_InputMode )
		{
			AppCommon& myApp = GetAppInstance();
			QString useMsg = QObject::tr( "Use camcorder button on left to start and stop recording video." );
			myApp.toGuiUserMessage( useMsg.toUtf8().constData() );
			//QMessageBox::information( this, QObject::tr( "Information" ), QObject::tr( "Use camcorder button on left to start and stop recording video." ), QObject::tr( "OK" ) );
		}


		ui.m_InputTextWidget->setVisible( false );
		ui.m_InputFaceWidget->setVisible( false );
		ui.m_InputPhotoWidget->setVisible( false );
		ui.m_InputVoiceWidget->setVisible( false );
		ui.m_InputVideoWidget->setVisible( false );
		ui.m_InputAllWidget->setVisible( true );
		break;
	}

	m_InputMode = inputMode;
}

//============================================================================
void ChatEntryWidget::setIsPersonalRecorder( bool isPersonal )
{
	ui.m_InputAllWidget->setIsPersonalRecorder( isPersonal );
	ui.m_InputFaceWidget->setIsPersonalRecorder( isPersonal );
	ui.m_InputPhotoWidget->setIsPersonalRecorder( isPersonal );
	ui.m_InputTextWidget->setIsPersonalRecorder( isPersonal );
	ui.m_InputVideoWidget->setIsPersonalRecorder( isPersonal );
	ui.m_InputVoiceWidget->setIsPersonalRecorder( isPersonal );
}
