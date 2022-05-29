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

#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "AppCommon.h"
#include "AppSettings.h"
#include "ActivityAddRemoveLibraryFiles.h"

#include "FileShareItemWidget.h"
#include "FileItemInfo.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxGlobals.h>

#include <QDesktopServices>
#include <QUrl>
#include <QTimer>

//============================================================================
ActivityAddRemoveLibraryFiles::ActivityAddRemoveLibraryFiles(	AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_ADD_REMOVE_LIB_FILES, app, parent, eAppletMessengerFrame, true )
, m_bFetchInProgress( false )
, m_WidgetClickEventFixTimer( new QTimer( this ) )
{
	ui.setupUi(this);
    connectBarWidgets();
	ui.m_UpDirectoryButton->setIcon( eMyIconUpOneDirectory );
	connect( m_WidgetClickEventFixTimer, SIGNAL(timeout()), this, SLOT(slotRequestFileList()) );

    connect(ui.ExitDialogButton, SIGNAL(clicked()), this, SLOT(slotHomeButtonClicked()) );

    connect(ui.FileItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotListItemClicked(QListWidgetItem *)) );

    connect( &m_MyApp, SIGNAL(signalFileList(const char *,uint8_t,int64_t,int,bool)), this, SLOT(slotFileList(const char *,uint8_t,int64_t,int,bool)) );
    connect(ui.FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotListItemClicked(QListWidgetItem *)) );
    connect(ui.m_UpDirectoryLabel, SIGNAL(clicked()), this, SLOT(slotUpDirectoryClicked()) );

    connect(ui.m_UpDirectoryButton, SIGNAL(clicked()), this, SLOT(slotUpDirectoryClicked()) );

    connect(ui.m_UpDirWidget, SIGNAL(clicked()), this, SLOT(slotUpDirectoryClicked()) );

    connect(ui.m_BrowseButton, SIGNAL(clicked()), this, SLOT(slotBrowseButtonClicked()) );

	std::string strLastDir;
    m_MyApp.getAppSettings().getLastBrowseShareDir( strLastDir );
	if( 0 == strLastDir.length() )
	{
		strLastDir = VxGetDownloadsDirectory();
	}

	setCurrentBrowseDir( strLastDir.c_str() );
	setActionEnable( false );
	slotRequestFileList();
}

//============================================================================
ActivityAddRemoveLibraryFiles::~ActivityAddRemoveLibraryFiles()
{
	clearFileList();
}

//============================================================================
void ActivityAddRemoveLibraryFiles::setTitle( QString strTitle )
{
	ui.TitleLabel->setText(strTitle);
}

//============================================================================
void ActivityAddRemoveLibraryFiles::setCurrentBrowseDir( QString browseDir )
{
	if( !browseDir.isEmpty() )
	{
		m_strCurBrowseDirectory = browseDir.toLatin1().constData();
        m_MyApp.getAppSettings().setLastBrowseShareDir( m_strCurBrowseDirectory );
		ui.m_CurDirLabel->setTextBreakAnywhere( m_strCurBrowseDirectory.c_str(), 4 );
	}
}

//============================================================================
FileShareItemWidget * ActivityAddRemoveLibraryFiles::fileToWidget( uint8_t u8FileType, const char * pFileName, uint64_t u64FileLen, VxGUID assetId, bool isShared )
{
	FileShareItemWidget * item = new FileShareItemWidget(ui.FileItemList);
	item->setSizeHint(QSize(200, GuiParams::getButtonSize().height() + 4));

	FileItemInfo * poItemInfo = new FileItemInfo( u8FileType, pFileName, u64FileLen, assetId );
	poItemInfo->setIsShared( isShared );
	item->setFileItemInfo( poItemInfo );

	connect(	item, 
				SIGNAL(signalFileShareItemClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListItemClicked(QListWidgetItem*)) );
	connect(	item, 
				SIGNAL(signalFileShareFileIconClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListFileIconClicked(QListWidgetItem*)) );
	connect(	item, 
				SIGNAL(signalFileShareLockClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListLockIconClicked(QListWidgetItem*)) );

	item->updateWidgetFromInfo();

	return item;
}

//============================================================================
void ActivityAddRemoveLibraryFiles::addFile(uint8_t			u8FileType,
										    uint64_t		u64FileLen, 
										    const char *	pFileName,
											VxGUID			assetId,
										    bool			isShared )
{
	FileShareItemWidget * item = fileToWidget( u8FileType, pFileName, u64FileLen, assetId, isShared );
	if( item )
	{
        LogMsg( LOG_INFO, "ActivityAddRemoveLibraryFiles::addFile: adding widget\n");
		ui.FileItemList->addItem( (QListWidgetItem *)item );
		ui.FileItemList->setItemWidget( (QListWidgetItem *)item, (QWidget *)item );
	}
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotHomeButtonClicked( void )
{
	hide();
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotUpDirectoryClicked( void )
{
	if( m_strCurBrowseDirectory.length() > 1 )
	{
		// move up one directory
		char * pBuf = new char[ m_strCurBrowseDirectory.length() + 1 ];
		strcpy( pBuf, m_strCurBrowseDirectory.c_str() );
		if( '/' == pBuf[ m_strCurBrowseDirectory.length() - 1 ] )
		{
			pBuf[ m_strCurBrowseDirectory.length() - 1 ] = 0;
		}

		char * pTemp = strrchr( pBuf, '/' );
		if( pTemp )
		{
			pTemp++;
			pTemp[0] = 0;
		}

		m_strCurBrowseDirectory = pBuf;
		VxFileUtil::assureTrailingDirectorySlash( m_strCurBrowseDirectory );
        delete[] pBuf;
		setActionEnable( false );
		slotRequestFileList();
	}
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotBrowseButtonClicked( void )
{
    QString strDir = GuiHelpers::browseForDirectory( m_strCurBrowseDirectory.c_str(), this );
	if( !strDir.isEmpty() )
	{
		setCurrentBrowseDir( strDir );
		setActionEnable( false );
		slotRequestFileList();
	}
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotListItemClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
        if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
            if( ! poInfo->getFullFileName().isEmpty() )
			{
                m_strCurBrowseDirectory =  poInfo->getFullFileName().toLatin1().constData();
				VxFileUtil::assureTrailingDirectorySlash( m_strCurBrowseDirectory );
				m_WidgetClickEventFixTimer->start(10);
			}
		}
		else
		{
			// is file
			if( poInfo->shouldOpenFile() )
			{
                QDesktopServices::openUrl(QUrl::fromLocalFile(poInfo->getFullFileName()));
			}
		}
	}
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotListFileIconClicked( QListWidgetItem * item )
{
	slotListItemClicked( item ); 
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotListLockIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
        if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
			if( false == m_bFetchInProgress )
			{
                std::string strDir = poInfo->getFullFileName().toLatin1().constData();
				if( strDir.length() )
				{
					m_strCurBrowseDirectory = strDir;
					VxFileUtil::assureTrailingDirectorySlash( m_strCurBrowseDirectory );
					setActionEnable( false );
					fromListWidgetRequestFileList();
				}
			}
		}
		else
		{
			// is file
			poInfo->toggleIsShared();
			((FileShareItemWidget*)item)->updateWidgetFromInfo();
            m_Engine.fromGuiSetFileIsShared( poInfo->getFullFileName().toLatin1().constData(), poInfo->getIsShared() );
		}
	}	
}

//============================================================================
void ActivityAddRemoveLibraryFiles::setActionEnable( bool enable )
{
	m_bFetchInProgress = enable ? false : true;
	if( false == m_bFetchInProgress )
	{
		m_bFetchInProgress = false;
	}

	LogMsg( LOG_DEBUG, "Fetch In Progress %d\n", m_bFetchInProgress );
	ui.m_UpDirectoryButton->setEnabled( enable );
}

//============================================================================
void ActivityAddRemoveLibraryFiles::fromListWidgetRequestFileList( void )
{
	// there is a problem in that if we clear the list in a click event from a widget in the list
	// then causes crash so emit timed event to let the click event finish
	m_WidgetClickEventFixTimer->start();
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotRequestFileList( void )
{
	m_WidgetClickEventFixTimer->stop();
	clearFileList();
	ui.m_CurDirLabel->setText( m_strCurBrowseDirectory.c_str() );
    m_MyApp.getAppSettings().setLastBrowseShareDir( m_strCurBrowseDirectory );
	m_Engine.getFromGuiInterface().fromGuiBrowseFiles( m_strCurBrowseDirectory.c_str(), true );
}

//============================================================================
void ActivityAddRemoveLibraryFiles::slotFileList( const char * fileName, uint8_t fileType, int64_t dataLen, VxGUID assetId, int done, bool isShared )
{
	if( done )
	{
		setActionEnable( true );
	}
	else
	{
		addFile( fileType, dataLen, fileName, assetId, isShared );
	}
}

//============================================================================
void ActivityAddRemoveLibraryFiles::clearFileList( void )
{
	for(int i = 0; i < ui.FileItemList->count(); ++i)
	{
		QListWidgetItem* item = ui.FileItemList->item(i);
		delete ((FileShareItemWidget *)item);
	}
	
	ui.FileItemList->clear();
}
