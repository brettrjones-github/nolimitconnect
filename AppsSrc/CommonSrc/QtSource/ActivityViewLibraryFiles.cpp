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

#include "AppCommon.h"	
#include "AppSettings.h"

#include "ActivityViewLibraryFiles.h"
#include "AppletDownloads.h"
#include "ActivityBrowseFiles.h"
#include "ActivityYesNoMsgBox.h"

#include "FileShareItemWidget.h"
#include "MyIcons.h"
#include "AppletPopupMenu.h"
#include "AppGlobals.h"
#include "FileItemInfo.h"
#include "FileActionMenu.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
ActivityViewLibraryFiles::ActivityViewLibraryFiles(	AppCommon& app, QWidget * parent, bool isSelectAFileMode )
: ActivityBase( OBJNAME_ACTIVITY_VIEW_LIBRARY_FILES, app, parent, eAppletMessengerFrame, true )
, m_ePluginType( ePluginTypeInvalid )
, m_IsSelectAFileMode( isSelectAFileMode )
, m_FileWasSelected( false )
, m_SelectedFileType( 0 )
, m_SelectedFileName( "" )
, m_SelectedFileLen( 0 )
, m_SelectedFileIsShared( false )
, m_SelectedFileIsInLibrary( false )
, m_eFileFilterType( eFileFilterAll )
, m_FileFilterMask( VXFILE_TYPE_ALLNOTEXE )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "My Library Of Files" ) );
	ui.m_DoubleTapInstructionLabel->setVisible( m_IsSelectAFileMode );

    connect(ui.m_TitleBarWidget, SIGNAL(signalBackButtonClicked()), this, SLOT(slotHomeButtonClicked()));
    connect( ui.m_FileItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotListItemClicked(QListWidgetItem *)));
    connect( ui.m_FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotListItemDoubleClicked(QListWidgetItem *)));

	connect( this, SIGNAL(signalToGuiFileList(VxMyFileInfo&)), this, SLOT(slotToGuiFileList(VxMyFileInfo&)) );
	connect( ui.m_AddFilesButton, SIGNAL(clicked()), this, SLOT(slotAddFilesButtonClicked()) );

	setFileFilter( m_eFileFilterType );
	connect( ui.m_FileFilterComboBox, SIGNAL(signalApplyFileFilter(unsigned char)), this,  SLOT(slotApplyFileFilter(unsigned char)) );
	statusMsg( "Requesting Library File List " );
	m_MyApp.wantToGuiFileXferCallbacks( this, true );
	slotApplyFileFilter( ui.m_FileFilterComboBox->getCurrentFileFilterMask() );
    connectBarWidgets();
}

//============================================================================
void ActivityViewLibraryFiles::statusMsg( QString strMsg )
{
	//LogMsg( LOG_INFO, strMsg.toStdString().c_str() );
	ui.m_StatusMsgLabel->setText( strMsg );
}

//============================================================================
void ActivityViewLibraryFiles::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
	m_MyApp.setIsLibraryActivityActive( true );
	m_MyApp.wantToGuiFileXferCallbacks( this, true );
	slotRequestFileList();
}

//============================================================================
void ActivityViewLibraryFiles::hideEvent( QHideEvent * ev )
{
	m_MyApp.wantToGuiFileXferCallbacks( this, false );
	ActivityBase::hideEvent( ev );
	m_MyApp.setIsLibraryActivityActive( false );
}

//============================================================================
void ActivityViewLibraryFiles::toGuiFileList( VxMyFileInfo& fileInfo )
{
	if( fileInfo.getFullFileName().isEmpty() )
	{
		//setActionEnable( true );
		statusMsg( "List Get Completed" );
	}
	else
	{
		addFile( fileInfo, fileInfo.getIsShared(), fileInfo.getIsInLibrary() );
	}
}

//============================================================================
void ActivityViewLibraryFiles::setFileFilter( EFileFilterType eFileFilter )
{
	m_eFileFilterType = eFileFilter;
	m_FileFilterMask = ui.m_FileFilterComboBox->getMaskFromFileFilterType( m_eFileFilterType );
	ui.m_FileFilterComboBox->setFileFilter( eFileFilter );
}

//============================================================================
void ActivityViewLibraryFiles::slotApplyFileFilter( unsigned char fileTypeMask )
{
	m_FileFilterMask = fileTypeMask;
	slotRequestFileList();
}

//============================================================================
void ActivityViewLibraryFiles::slotRequestFileList( void )
{
	clearFileList();
	m_FromGui.fromGuiGetFileLibraryList( m_FileFilterMask );
}

//============================================================================
FileShareItemWidget * ActivityViewLibraryFiles::fileToWidget( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary )
{
	FileShareItemWidget * item = new FileShareItemWidget(ui.m_FileItemList);
    item->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

	FileItemInfo * poItemInfo = new FileItemInfo( fileInfo, 0, isShared, isInLibrary );
    item->QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)poItemInfo) );
    connect( item, SIGNAL(signalFileShareItemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));

	connect(	item, 
		SIGNAL(signalFileShareItemClicked(QListWidgetItem*)), 
		this, 
		SLOT(slotListItemClicked(QListWidgetItem*)) );

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
void ActivityViewLibraryFiles::slotListFileIconClicked( QListWidgetItem * item )
{
	slotListItemClicked( item ); 
}

//============================================================================
void ActivityViewLibraryFiles::slotListShareFileIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
		}
		else
		{
			// is file
			poInfo->toggleIsShared();
			((FileShareItemWidget*)item)->updateWidgetFromInfo();
			m_Engine.fromGuiSetFileIsShared( poInfo->getFullFileName().toUtf8().constData(), poInfo->getIsShared() );
		}
	}	
}

//============================================================================
void ActivityViewLibraryFiles::slotListLibraryIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
		}
		else
		{
			// is file
			poInfo->toggleIsInLibrary();
			((FileShareItemWidget*)item)->updateWidgetFromInfo();
			m_Engine.fromGuiAddFileToLibrary(	poInfo->getFullFileName().toUtf8().constData(), 
				poInfo->getIsInLibrary(),
				poInfo->getFileHashId().getHashData() );
		}
	}	
}

//============================================================================
void ActivityViewLibraryFiles::slotListPlayIconClicked( QListWidgetItem * item )
{
	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
		}
		else
		{
			// play file
			this->playFile( poInfo->getFullFileName() );
		}
	}	
}

//============================================================================
void ActivityViewLibraryFiles::slotListShredIconClicked( QListWidgetItem * item )
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
				ui.m_FileItemList->removeItemWidget( item );
				m_Engine.fromGuiDeleteFile( fileName.toUtf8().constData(), true );
			}
		}
	}	
}

//============================================================================
bool ActivityViewLibraryFiles::confirmDeleteFile( QString fileName, bool shredFile )
{
	bool acceptAction = true;
	bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
	if( false == isConfirmDisabled )
	{
		QString title = shredFile ?  "Confirm Shred File" : "Confirm Delete File";
		QString bodyText = "";
		if( shredFile )
		{
			bodyText = "Are You Sure You Want To Write Random Data Into The File Then Delete From The Device?";
		}
		else
		{
			bodyText = "Are You Sure To Delete The File From The Device?";
		}

		ActivityYesNoMsgBox dlg( m_MyApp, &m_MyApp, title, bodyText );
		dlg.makeNeverShowAgainVisible( true );
		if( false == (QDialog::Accepted == dlg.exec()))
		{
			acceptAction = false;
		}

		if( dlg.wasNeverShowAgainChecked() )
		{
			m_MyApp.getAppSettings().setIsConfirmDeleteDisabled( true );
		}
	}

	return acceptAction;
}

//============================================================================
//!	get friend from QListWidgetItem data
FileItemInfo * ActivityViewLibraryFiles::widgetToFileItemInfo( FileShareItemWidget * item )
{
	return (FileItemInfo *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
}

//============================================================================
FileShareItemWidget * ActivityViewLibraryFiles::findListEntryWidget( VxMyFileInfo& fileInfo )
{
	int iIdx = 0;
	FileShareItemWidget * poWidget;
	while( iIdx < ui.m_FileItemList->count() )
	{
		poWidget = (FileShareItemWidget *)ui.m_FileItemList->item(iIdx);
		if( poWidget )
		{
			FileItemInfo * poFileInfo = (FileItemInfo *)poWidget->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
			if( poFileInfo && ( poFileInfo->getFullFileName() == fileInfo.getFullFileName() ) )
			{
				return poWidget;
			}
		}

		iIdx++;
	}

	return NULL;
}

//============================================================================
void ActivityViewLibraryFiles::slotAddFilesButtonClicked( void )
{
	ActivityBrowseFiles dlg( m_MyApp, eFileFilterAll, this );
	dlg.exec();
	clearFileList();
	statusMsg( "Requesting Library File List " );
	m_FromGui.fromGuiGetFileLibraryList( m_FileFilterMask );
}

//============================================================================
void ActivityViewLibraryFiles::addFile(	VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary  )
{
	FileShareItemWidget * item = fileToWidget( fileInfo, isShared, isInLibrary );
	if( item )
	{
		//LogMsg( LOG_INFO, "ActivityViewLibraryFiles::addFile: adding widget\n");
		ui.m_FileItemList->addItem( item );
		ui.m_FileItemList->setItemWidget( item, item );
	}
}

//============================================================================
void ActivityViewLibraryFiles::slotHomeButtonClicked( void )
{
	close();
}

//============================================================================
//! user selected menu item
void ActivityViewLibraryFiles::slotListItemClicked(QListWidgetItem * item)
{
	FileItemInfo * poInfo = (FileItemInfo *)item->data(Qt::UserRole + 1).toLongLong();
	if( poInfo )
	{
		VxMyFileInfo& fileInfo =	poInfo->getMyFileInfo();
		if( m_IsSelectAFileMode )
		{
			m_FileWasSelected			= true;
			m_SelectedFileType			= fileInfo.getFileType();
			m_SelectedFileName			= fileInfo.getFullFileName().toUtf8().constData();
			m_SelectedFileLen			= fileInfo.getFileLength();
			m_SelectedFileIsShared		= poInfo->getIsShared();
			m_SelectedFileIsInLibrary	= poInfo->getIsInLibrary();
			accept();
		}
		else
		{
			//FileActionMenu fileActionMenuDialog(	m_MyApp, 
			//										this, 
			//										poInfo->getMyFileInfo(),
			//										poInfo->getIsShared(),
			//										poInfo->getIsInLibrary() );
			//fileActionMenuDialog.exec();
			playFile( fileInfo.getFullFileName() );
		}
	}
}

//============================================================================
//! user double clicked menu item
void ActivityViewLibraryFiles::slotListItemDoubleClicked(QListWidgetItem * item)
{
	slotListItemClicked( item );
}

//============================================================================
void ActivityViewLibraryFiles::clearFileList( void )
{
	ui.m_FileItemList->clear();
}
