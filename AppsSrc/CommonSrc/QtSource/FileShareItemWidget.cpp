//============================================================================
// Copyright (C) 2010 Brett R. Jones 
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


#include "FileItemInfo.h"
#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "GuiHelpers.h"
#include "AppCommon.h"

//============================================================================
FileShareItemWidget::FileShareItemWidget(QWidget*parent )
: QWidget(parent)
, m_MyApp( GetAppInstance() )
{
	ui.setupUi(this);

	ui.m_FileIconButton->setFixedSize( eButtonSizeMedium );

	connect( ui.m_FileIconButton,		SIGNAL(clicked()),						this, SLOT(slotFileIconButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalPlayButtonClicked()),		this, SLOT(slotPlayButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalLibraryButtonClicked()),	this, SLOT(slotLibraryButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalFileShareButtonClicked()), this, SLOT(slotFileShareButtonClicked()) );
	connect( ui.m_FileActionBar,		SIGNAL(signalShredButtonClicked()),		this, SLOT(slotShredButtonClicked()) );
}

//============================================================================
FileShareItemWidget::~FileShareItemWidget()
{
	FileItemInfo* poInfo = getFileItemInfo();
	delete poInfo;
}

//============================================================================
MyIcons&  FileShareItemWidget::getMyIcons( void )
{
	return m_MyApp.getMyIcons();
}

//============================================================================
void FileShareItemWidget::slotFileIconButtonClicked( void )
{
	emit signalFileIconClicked( this );
}

//============================================================================
void FileShareItemWidget::slotPlayButtonClicked( void )
{
	emit signalPlayButtonClicked( this );
}

//============================================================================
void FileShareItemWidget::slotLibraryButtonClicked( void )
{
	emit signalLibraryButtonClicked( this );
}

//============================================================================
void FileShareItemWidget::slotFileShareButtonClicked( void )
{
	emit signalFileShareButtonClicked( this );
}

//============================================================================
void FileShareItemWidget::slotShredButtonClicked( void )
{
	emit signalShredButtonClicked( this );
}

//============================================================================
void FileShareItemWidget::mousePressEvent(QMouseEvent * event)
{
	QWidget::mousePressEvent(event);
	emit signalFileShareItemClicked( this );
}

//============================================================================
void FileShareItemWidget::resizeEvent(QResizeEvent* resizeEvent)
{
	QWidget::resizeEvent(resizeEvent);
	updateWidgetFromInfo();
}

//============================================================================
bool FileShareItemWidget::getIsShared( void )
{
	bool isShared = false;
	FileItemInfo* poInfo = getFileItemInfo();
	if( poInfo )
	{
		isShared = poInfo->getIsSharedFile();
	}

	return isShared;
}

//============================================================================
void FileShareItemWidget::setIsSharedFile( bool isShared )
{
	FileItemInfo* poInfo = getFileItemInfo();
	if( poInfo )
	{
		poInfo->setIsSharedFile( isShared );
	}

	updateWidgetFromInfo();
}

//============================================================================
bool FileShareItemWidget::getIsInLibrary( void )
{
	bool isInLibrary = false;
	FileItemInfo* poInfo = getFileItemInfo();
	if( poInfo )
	{
		isInLibrary = poInfo->getIsInLibrary();
	}

	return isInLibrary;
}

//============================================================================
void FileShareItemWidget::setIsInLibrary( bool isInLibrary )
{
	FileItemInfo* poInfo = getFileItemInfo();
	if( poInfo )
	{
		poInfo->setIsInLibrary( isInLibrary );
	}

	updateWidgetFromInfo();
}

//============================================================================
void FileShareItemWidget::setFileItemInfo( FileItemInfo* fileItemInfo )
{
    QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)fileItemInfo) );
}

//============================================================================
FileItemInfo* FileShareItemWidget::getFileItemInfo( void )
{
	return (FileItemInfo*)QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
void FileShareItemWidget::updateWidgetFromInfo( void )
{
	FileItemInfo* poInfo = getFileItemInfo();
	if( 0 == poInfo )
	{
		return;
	}

	if( poInfo->isDirectory() )
	{
		ui.m_FileActionBar->setVisible( false );
		ui.m_FileIconButton->setIcon( eMyIconFileBrowseNormal );
		ui.m_FileNameLabel->setText( poInfo->getFullFileName() );
		ui.m_FileSizeLabel->setVisible( false );

		return;
	}

	ui.m_FileActionBar->setIsSharedFile(  poInfo->getIsSharedFile() );

	ui.m_FileActionBar->setIsInLibrary(  poInfo->getIsInLibrary() );

	ui.m_FileIconButton->setIcon( getMyIcons().getFileIcon( poInfo->getFileType() ) );

	ui.m_FileNameLabel->setText( poInfo->getJustFileName() );
	ui.m_FileSizeLabel->setText( poInfo->describeFileLength() );
}
