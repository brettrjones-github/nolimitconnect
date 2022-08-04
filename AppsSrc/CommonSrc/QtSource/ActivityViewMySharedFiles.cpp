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

#include "ActivityViewMySharedFiles.h"
#include "ActivityBrowseFiles.h"

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
ActivityViewMySharedFiles::ActivityViewMySharedFiles(	AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_VIEW_MY_SHARED_FILES, app, parent, eAppletMessengerFrame, true )
, m_bFetchInProgress( false )
, m_WidgetClickEventFixTimer( new QTimer( this ) )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "My Shared Files" ) );
    connectBarWidgets();
	m_WidgetClickEventFixTimer->setInterval( 10 );
	connect( m_WidgetClickEventFixTimer, SIGNAL(timeout()), this, SLOT(slotRequestFileList()) );

    connect(ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotHomeButtonClicked()));
    connect(ui.FileItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotListItemClicked(QListWidgetItem *)));

	connect( this, SIGNAL(signalToGuiFileList(VxMyFileInfo&)), this, SLOT(slotToGuiFileList(VxMyFileInfo&)) );
    connect(ui.FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotListItemClicked(QListWidgetItem *)));

	connect( ui.m_AddFilesButton,		SIGNAL(clicked()),								this, SLOT(slotAddFilesButtonClicked()) );
	connect( ui.m_FileFilterComboBox,	SIGNAL(signalApplyFileFilter(unsigned char)),	this, SLOT(slotApplyFileFilter(unsigned char)));

	setActionEnable( false );
	m_MyApp.wantToGuiFileXferCallbacks( this, true );
	slotRequestSharedFileList( ui.m_FileFilterComboBox->getCurrentFileFilterMask() );
}

//============================================================================
ActivityViewMySharedFiles::~ActivityViewMySharedFiles()
{
	clearFileList();
}

//============================================================================
void ActivityViewMySharedFiles::showEvent( QShowEvent * ev )
{
	Q_UNUSED( ev );
	ActivityBase::showEvent( ev );
	m_MyApp.wantToGuiFileXferCallbacks( this, true );
}

//============================================================================
void ActivityViewMySharedFiles::hideEvent( QHideEvent * ev )
{
	Q_UNUSED( ev );
	m_MyApp.wantToGuiFileXferCallbacks( this, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
void ActivityViewMySharedFiles::toGuiFileList( VxMyFileInfo& fileInfo )
{
	if( fileInfo.getFullFileName().isEmpty() )
	{
		setActionEnable( true );
		//statusMsg( "List Get Completed" );
	}
	else
	{
		addFile( fileInfo, fileInfo.getIsShared() );
	}
}

//============================================================================
void ActivityViewMySharedFiles::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText(strTitle);
}

//============================================================================
FileShareItemWidget * ActivityViewMySharedFiles::fileToWidget( VxMyFileInfo&	fileInfo, bool isShared )
{
	FileShareItemWidget * item = new FileShareItemWidget(ui.FileItemList);
	item->setSizeHint( QSize( (int)(GuiParams::getGuiScale() * 200), GuiParams::getFileListEntryHeight() ) );

	FileItemInfo * poItemInfo = new FileItemInfo( fileInfo );
	bool isInLibrary = m_Engine.fromGuiGetIsFileInLibrary( fileInfo.getFullFileName().toUtf8().constData() );
	poItemInfo->setIsShared( isShared );
	poItemInfo->setIsInLibrary( isInLibrary );
	item->setFileItemInfo( poItemInfo );

	connect(	item, 
				SIGNAL(signalFileIconClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListFileIconClicked(QListWidgetItem*)) );

	connect(	item, 
				SIGNAL(signalPlayButtonClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListPlayIconClicked(QListWidgetItem*)) );

	connect(	item, 
				SIGNAL(signalLibraryButtonClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListLibraryIconClicked(QListWidgetItem*)) );

	connect(	item, 
				SIGNAL(signalFileShareButtonClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListShareFileIconClicked(QListWidgetItem*)) );

	connect(	item, 
				SIGNAL(signalShredButtonClicked(QListWidgetItem*)), 
				this, 
				SLOT(slotListShredIconClicked(QListWidgetItem*)) );

	item->updateWidgetFromInfo();

	return item;
}

//============================================================================
void ActivityViewMySharedFiles::addFile(	VxMyFileInfo&	fileInfo,
											bool			isShared )
{
	FileShareItemWidget * item = fileToWidget( fileInfo, isShared );
	if( item )
	{
		//LogMsg( LOG_INFO, "ActivityViewMySharedFiles::addFile: adding widget\n");
		ui.FileItemList->addItem( (QListWidgetItem *)item );
		ui.FileItemList->setItemWidget( (QListWidgetItem *)item, (QWidget *)item );
	}
}

//============================================================================
void ActivityViewMySharedFiles::slotHomeButtonClicked( void )
{
	accept();
}

//============================================================================
void ActivityViewMySharedFiles::slotAddFilesButtonClicked( void )
{
	ActivityBrowseFiles dlg( m_MyApp, eFileFilterAll, this );
	dlg.exec();
	clearFileList();
	//statusMsg( "Requesting Library File List " );
	slotRequestSharedFileList( ui.m_FileFilterComboBox->getCurrentFileFilterMask() );
}

//============================================================================
void ActivityViewMySharedFiles::slotListItemClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		poInfo->getMyFileInfo().playFile( m_MyApp );
	}
}

//============================================================================
void ActivityViewMySharedFiles::slotListPlayIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		poInfo->getMyFileInfo().playFile( m_MyApp );
	}
}

//============================================================================
void ActivityViewMySharedFiles::slotListFileIconClicked( QListWidgetItem * item )
{
	slotListItemClicked( item ); 
}

//============================================================================
void ActivityViewMySharedFiles::slotListShareFileIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
			if( false == m_bFetchInProgress )
			{
				std::string strDir = poInfo->getFullFileName().toUtf8().constData();
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
			if( poInfo->getIsShared() )
			{
				m_MyApp.toGuiStatusMessage( "File is now Shared" );
			}
			else
			{
				m_MyApp.toGuiStatusMessage( "File is now NOT Shared" );
			}

            m_Engine.fromGuiSetFileIsShared( poInfo->getFullFileName().toUtf8().constData(), poInfo->getIsShared() );
		}
	}	
}

//============================================================================
void ActivityViewMySharedFiles::slotListLibraryIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
			if( false == m_bFetchInProgress )
			{
				std::string strDir = poInfo->getFullFileName().toUtf8().constData();
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
			poInfo->toggleIsInLibrary();
			((FileShareItemWidget*)item)->updateWidgetFromInfo();
			if( poInfo->getIsInLibrary() )
			{
				m_MyApp.toGuiStatusMessage( "File is now in Library" );
			}
			else
			{
				m_MyApp.toGuiStatusMessage( "File is now NOT in Library" );
			}

			m_Engine.fromGuiAddFileToLibrary( poInfo->getFullFileName().toUtf8().constData(), poInfo->getIsInLibrary() );
		}
	}	
}

//============================================================================
void ActivityViewMySharedFiles::slotListShredIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
		}
		else
		{
			// shred file
			QString fileName = poInfo->getFullFileName();
			if( confirmDeleteFile( fileName, true ) )
			{
				ui.FileItemList->removeItemWidget( item );
				m_Engine.fromGuiDeleteFile( fileName.toUtf8().constData(), true );
			}
		}
	}	
}


//============================================================================
void ActivityViewMySharedFiles::setActionEnable( bool enable )
{
	m_bFetchInProgress = enable ? false : true;
	if( false == m_bFetchInProgress )
	{
		m_bFetchInProgress = false;
	}

	LogMsg( LOG_INFO, "Fetch In Progress %d\n", m_bFetchInProgress );
	ui.m_FileFilterComboBox->setEnabled( enable );
}

//============================================================================
void ActivityViewMySharedFiles::slotApplyFileFilter( unsigned char fileFilter )
{
	slotRequestSharedFileList( fileFilter );
}

//============================================================================
void ActivityViewMySharedFiles::slotRequestSharedFileList( uint8_t fileTypeFilter )
{
	m_WidgetClickEventFixTimer->stop();
	clearFileList();
	m_Engine.getFromGuiInterface().fromGuiGetSharedFiles( fileTypeFilter );
}

//============================================================================
void ActivityViewMySharedFiles::clearFileList( void )
{
	for(int i = 0; i < ui.FileItemList->count(); ++i)
	{
		QListWidgetItem* item = ui.FileItemList->item(i);
		delete ((FileShareItemWidget *)item);
	}
	
	ui.FileItemList->clear();
}


//============================================================================
void ActivityViewMySharedFiles::fromListWidgetRequestFileList( void )
{
	// there is a problem in that if we clear the list in a click event from a widget in the list
	// then causes crash so emit timed event to let the click event finish
	m_WidgetClickEventFixTimer->start();
}
