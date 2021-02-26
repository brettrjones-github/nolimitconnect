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
// http://www.nolimitconnect.com
//============================================================================

#include <app_precompiled_hdr.h>
#include "FileXferWidget.h"
#include "AppCommon.h"
#include "MyIcons.h"

#include "GuiFileXferSession.h"
#include "VxMyFileInfo.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

//============================================================================
FileXferWidget::FileXferWidget(QWidget *parent)
: QWidget(parent)
, m_MyApp( GetAppInstance() )
{
	ui.setupUi(this);
	connect( ui.m_FileIconButton,		SIGNAL(clicked()),						this, SLOT(slotFileIconButtonClicked()) );
	connect( ui.m_FileCancelButton,		SIGNAL(clicked()),						this, SLOT(slotCancelButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalPlayButtonClicked()),		this, SLOT(slotPlayButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalLibraryButtonClicked()),	this, SLOT(slotLibraryButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalFileShareButtonClicked()), this, SLOT(slotFileShareButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalShredButtonClicked()),		this, SLOT(slotShredButtonClicked()) );
}

//============================================================================
FileXferWidget::~FileXferWidget()
{
	GuiFileXferSession * fileItemInfo = (GuiFileXferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
	delete fileItemInfo;
}

//============================================================================
MyIcons&  FileXferWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void FileXferWidget::mousePressEvent(QMouseEvent * event)
{
	QWidget::mousePressEvent(event);
	emit signalFileXferItemClicked( this );
}

//============================================================================
void FileXferWidget::setXferState( EXferState xferState, int param1, int param2 )
{
	GuiFileXferSession * sessionInfo = getFileItemInfo();
	if( sessionInfo )
	{
		sessionInfo->setXferState( xferState, param1, param2 );
		updateWidgetFromInfo();
	}
}

//============================================================================
void FileXferWidget::slotFileIconButtonClicked( void )
{
	emit signalFileIconButtonClicked( this );
}

//============================================================================
void FileXferWidget::slotCancelButtonClicked( void )
{
	emit signalCancelButtonClicked( this );
}

//============================================================================
void FileXferWidget::slotPlayButtonClicked( void )
{
	emit signalPlayButtonClicked( this );
}

//============================================================================
void FileXferWidget::slotLibraryButtonClicked( void )
{
	emit signalLibraryButtonClicked( this );
}

//============================================================================
void FileXferWidget::slotFileShareButtonClicked( void )
{
	emit signalLibraryButtonClicked( this );
}

//============================================================================
void FileXferWidget::slotShredButtonClicked( void )
{
	emit signalShredButtonClicked( this );
}

//============================================================================
void FileXferWidget::resizeEvent(QResizeEvent* resizeEvent)
{
	QWidget::resizeEvent(resizeEvent);
	updateWidgetFromInfo();
}

//============================================================================
bool FileXferWidget::getIsShared( void )
{
	bool isShared = false;
	GuiFileXferSession * poInfo = getFileItemInfo();
	if( poInfo )
	{
		isShared = poInfo->getIsShared();
	}

	return isShared;
}

//============================================================================
void FileXferWidget::setIsShared( bool isShared )
{
	GuiFileXferSession * sessionInfo = getFileItemInfo();
	if( sessionInfo )
	{
		sessionInfo->setIsShared( isShared );
	}

	updateWidgetFromInfo();
}

//============================================================================
bool FileXferWidget::getIsInLibrary( void )
{
	bool isInLibrary = false;
	GuiFileXferSession * sessionInfo = getFileItemInfo();
	if( sessionInfo )
	{
		isInLibrary = sessionInfo->getIsInLibrary();
	}

	return isInLibrary;
}

//============================================================================
void FileXferWidget::setIsInLibrary( bool isInLibrary )
{
	GuiFileXferSession * sessionInfo = getFileItemInfo();
	if( sessionInfo )
	{
		sessionInfo->setIsInLibrary( isInLibrary );
	}

	updateWidgetFromInfo();
}

//============================================================================
void FileXferWidget::setFileItemInfo( GuiFileXferSession * fileItemInfo )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)fileItemInfo) );
}

//============================================================================
GuiFileXferSession * FileXferWidget::getFileItemInfo( void )
{
	return (GuiFileXferSession *)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void FileXferWidget::setFileIconButtonEnabled( bool enable )
{
	ui.m_FileIconButton->setEnabled( enable );
}

//============================================================================
void FileXferWidget::setFileProgressBarValue( int val )
{
	ui.m_FileProgressBar->setValue( val );
}

//============================================================================
void FileXferWidget::updateWidgetFromInfo( void )
{
	GuiFileXferSession * poSession= getFileItemInfo();
	if( 0 == poSession )
	{
		return;
	}

	ui.m_FileIconButton->setIcon( getMyIcons().getFileIcon( poSession->getFileType() ) );
	if( VXFILE_TYPE_DIRECTORY == poSession->getFileType() )
	{
		ui.m_FileIconButton->setIcon( eMyIconFileBrowseNormal );
		ui.m_FileSizeLabel->setVisible( false );
		ui.m_FileNameLabel->setText( poSession->getFilePath() );
		ui.m_FilePathLabel->setVisible( false );
		ui.m_FileStatus->setVisible( false );
		ui.m_FileActionBar->setVisible( false );
		ui.m_FileProgressBar->setVisible( false );
		ui.m_FileCancelButton->setVisible( false );
	}
	else
	{
		bool enableAccept		= false;
		bool enablePlay			= false;
		bool enableProgress		= false;

		ui.m_FileNameLabel->setText( poSession->getJustFileName() );
		ui.m_FilePathLabel->setText( poSession->getFilePath() );
		ui.m_FileSizeLabel->setText( poSession->describeFileLength() );
		ui.m_FilePathLabel->setTextBreakAnywhere( poSession->getFilePath(), 3 );
		ui.m_FileStatus->setText( poSession->describeXferState() );
		switch( poSession->getXferState() )
		{
		case eXferStateInDownloadXfer:
		case eXferStateInUploadXfer:
			enableProgress = true;
			ui.m_FileProgressBar->setValue( poSession->getXferStateParam1() );
			break;

		case eXferStateUploadError:
		case eXferStateDownloadError:
			ui.m_FileStatus->setText( GuiParams::describeEXferError( (EXferError)poSession->getXferStateParam2() ) );
			ui.m_FileStatus->setStyleSheet("color: rgb(244, 0, 0);\n");
			break;

		case eXferStateCompletedUpload:
		case eXferStateCompletedDownload:
			if( 0 != poSession->getXferStateParam2() )
			{
				ui.m_FileStatus->setText( GuiParams::describeEXferError( (EXferError)poSession->getXferStateParam2() ) );
				ui.m_FileStatus->setStyleSheet("color: rgb(244, 0, 0);\n");
			}
			else
			{
				enableAccept = true;
				enablePlay = true;
			}

			break;

		case eXferStateUserCanceledUpload:
		case eXferStateUserCanceledDownload:
		case eXferStateUploadOfferRejected:
			enableAccept = true;
			break;

		case eXferStateBeginUpload:
		case eXferStateBeginDownload:
			enableProgress = true;
			break;

		case eXferStateUnknown:
		case eXferStateWaitingOfferResponse:
		case eXferStateUploadNotStarted:
		case eXferStateInUploadQue:
		case eXferStateDownloadNotStarted:
		case eXferStateInDownloadQue:
		default:
			break;
		}

		if( enableAccept )
		{
			ui.m_FileCancelButton->setIcon( eMyIconAcceptNormal );
		}
		else
		{
			if( eXferStateDownloadNotStarted == poSession->getXferState() )
			{
				ui.m_FileCancelButton->setIcons( eMyIconFileDownloadNormal );
			}
			else if( eXferStateUploadNotStarted == poSession->getXferState() )
			{
				ui.m_FileCancelButton->setIcons( eMyIconSendFileNormal );
			}
			else
			{
				if( eXferDirectionRx ==  poSession->getXferDirection() )
				{
					ui.m_FileCancelButton->setIcons( eMyIconFileDownloadCancel );
				}
				else
				{
					ui.m_FileCancelButton->setIcons( eMyIconSendFileCancel );
				}
			}
		}

		if( enablePlay )
		{
			ui.m_FileActionBar->setVisible( true );
		}
		else
		{
			ui.m_FileActionBar->setVisible( false );
		}

		if( enableProgress )
		{
			ui.m_FileProgressBar->setVisible( true );
		}
		else
		{
			ui.m_FileProgressBar->setVisible( false );
		}
	}

	ui.m_FileActionBar->setIsShared(  poSession->getIsShared() );
	ui.m_FileActionBar->setIsInLibrary(  poSession->getIsInLibrary() );
}
