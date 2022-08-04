//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
#include "FileActionBarWidget.h"
#include "AppCommon.h"

//============================================================================
FileActionBarWidget::FileActionBarWidget(QWidget *parent)
: QFrame(parent)
, m_MyApp( GetAppInstance() )
{
	ui.setupUi(this);
	
	ui.m_PlayFileButton->setIcons( eMyIconPlayNormal );
	ui.m_FileInLibraryButton->setIcons( eMyIconLibraryNormal );
	ui.m_FileShareButton->setIcons( eMyIconShareFilesNormal );
	ui.m_ShredFileButton->setIcons( eMyIconShredderNormal );

	ui.m_PlayFileButton->setFixedSize( eButtonSizeSmall );
	ui.m_FileInLibraryButton->setFixedSize( eButtonSizeSmall );
	ui.m_FileShareButton->setFixedSize( eButtonSizeSmall );
	ui.m_ShredFileButton->setFixedSize( eButtonSizeSmall );

	connect( ui.m_PlayFileButton,		SIGNAL(clicked()), this, SLOT(slotPlayButtonClicked()) );
	connect( ui.m_FileInLibraryButton,	SIGNAL(clicked()), this, SLOT(slotLibraryButtonClicked()) );
	connect( ui.m_FileShareButton,		SIGNAL(clicked()), this, SLOT(slotFileShareButtonClicked()) );
	connect( ui.m_ShredFileButton,		SIGNAL(clicked()), this, SLOT(slotShredButtonClicked()) );
}

//============================================================================
void FileActionBarWidget::resizeEvent( QResizeEvent * ev )
{
	QFrame::resizeEvent( ev );
    //int buttonHeight = geometry().height() - 4;
	//QSize buttonSize( buttonHeight, buttonHeight );
	//ui.m_PlayFileButton->setFixedSizeAbsolute( buttonSize );
	//ui.m_FileInLibraryButton->setFixedSizeAbsolute( buttonSize );
	//ui.m_FileShareButton->setFixedSizeAbsolute( buttonSize );
	//ui.m_ShredFileButton->setFixedSizeAbsolute( buttonSize );

	//ui.m_PlayFileButton->setIconSize( buttonSize );
	//ui.m_FileInLibraryButton->setIconSize( buttonSize );
	//ui.m_FileShareButton->setIconSize( buttonSize );
	//ui.m_ShredFileButton->setIconSize( buttonSize );
}

//============================================================================
void FileActionBarWidget::slotPlayButtonClicked( void )
{
	emit signalPlayButtonClicked();
}

//============================================================================
void FileActionBarWidget::slotLibraryButtonClicked( void )
{
	emit signalLibraryButtonClicked();
}

//============================================================================
void FileActionBarWidget::slotFileShareButtonClicked( void )
{
	emit signalFileShareButtonClicked();
}

//============================================================================
void FileActionBarWidget::slotShredButtonClicked( void )
{
	emit signalShredButtonClicked();
}

//============================================================================
void FileActionBarWidget::setIsInLibrary( bool isInLibrary )
{
	if( isInLibrary )
	{
		ui.m_FileInLibraryButton->setIcons( eMyIconLibraryNormal );
	}
	else
	{
		ui.m_FileInLibraryButton->setIcons( eMyIconLibraryCancel );
	}
}

//============================================================================
void FileActionBarWidget::setIsShared( bool isShared )
{
	if( isShared )
	{
		ui.m_FileShareButton->setIcons( eMyIconServiceShareFiles );
	}
	else
	{
		ui.m_FileShareButton->setIcons( eMyIconShareFilesDisabled );
	}
}
