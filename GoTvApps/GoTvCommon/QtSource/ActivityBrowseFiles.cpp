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
// http://www.nolimitconnect.com
//============================================================================
#include <app_precompiled_hdr.h>
#include "ActivityBrowseFiles.h"
#include "ActivityYesNoMsgBox.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include "FileShareItemWidget.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxGlobals.h>

#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QFileDialog>

//============================================================================
ActivityBrowseFiles::ActivityBrowseFiles( AppCommon& app,  EFileFilterType fileFilter, ActivityBase * parent, bool isSelectAFileMode )
: ActivityBase( OBJNAME_ACTIVITY_BROWSE_FILES, app, parent, eActivityBrowseFiles, true )
, m_bFetchInProgress( false )
, m_WidgetClickEventFixTimer( new QTimer( this ) )
, m_IsSelectAFileMode( isSelectAFileMode )
, m_FileWasSelected( false )
, m_SelectedFileType( 0 )
, m_SelectedFileName( "" )
, m_SelectedFileLen( 0 )
, m_SelectedFileIsShared( false )
, m_SelectedFileIsInLibrary( false )
, m_eFileFilterType( fileFilter )
, m_FileFilterMask( VXFILE_TYPE_ALLNOTEXE )
{
	ui.setupUi( this );
    setFileFilter( m_eFileFilterType );

	setTitleBarText( QObject::tr("Browse Device Files") );
	ui.m_DoubleTapInstructionLabel->setVisible( m_IsSelectAFileMode );
    connectBarWidgets();
	ui.m_UpDirectoryButton->setIcon( eMyIconMoveUpDirNormal );
	m_WidgetClickEventFixTimer->setInterval( 10 );
	connect( m_WidgetClickEventFixTimer, SIGNAL(timeout()), this, SLOT(slotRequestFileList()) );


    connect( ui.FileItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotListItemClicked(QListWidgetItem *)));
    connect( ui.FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotListItemDoubleClicked(QListWidgetItem *)));
	connect( this, SIGNAL(signalToGuiFileList(VxMyFileInfo&)), this, SLOT(slotToGuiFileList(VxMyFileInfo&)) );

    connect( ui.m_UpDirectoryLabel, SIGNAL(clicked()), this, SLOT(slotUpDirectoryClicked()));

    connect( ui.m_UpDirectoryButton, SIGNAL(clicked()), this, SLOT(slotUpDirectoryClicked()));
    connect( ui.m_UpDirWidget, SIGNAL(clicked()), this, SLOT(slotUpDirectoryClicked()));
	connect( ui.m_BrowseButton, SIGNAL(clicked()), this, SLOT(slotBrowseButtonClicked()));

	setDefaultCurrentDir( m_eFileFilterType );
	
	m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
	setActionEnable( false );
	setFileFilter( m_eFileFilterType );
	connect( ui.m_FileFilterComboBox, SIGNAL(signalApplyFileFilter(unsigned char)), this,  SLOT(slotApplyFileFilter(unsigned char)) );
	slotRequestFileList();
}

//============================================================================
ActivityBrowseFiles::~ActivityBrowseFiles()
{
	clearFileList();
}
    
//========================================================================
void ActivityBrowseFiles::setDefaultCurrentDir( EFileFilterType eFileFilterType )
{
    std::string strLastDir = "";
	m_MyApp.getAppSettings().getLastBrowseDir( eFileFilterType, strLastDir );
	if( 0 == strLastDir.length() )
	{
		strLastDir = getDefaultDir( eFileFilterType );
	}
    
	if( 0 == strLastDir.length() )
	{
		strLastDir = VxGetDownloadsDirectory();
	}
	   	
	setCurrentBrowseDir( strLastDir.c_str() );
}

//========================================================================
std::string ActivityBrowseFiles::getDefaultDir( int eFileFilterType )
{
	std::string defaultDir = "";

	switch( eFileFilterType )
	{
	case eFileFilterPhoto:
    case eFileFilterPhotoOnly:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
			QStringList paths = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
			QString picturesLocation = paths[0];
#else
			QString picturesLocation = QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
#endif //QT_5_OR_GREATER
			defaultDir = picturesLocation.toStdString();
			if( 0 != defaultDir.length() )
			{
				VxFileUtil::makeForwardSlashPath( defaultDir );
				defaultDir += "/";
			}
		}
    	
    	break;
    	
	case eFileFilterAudio:
    case eFileFilterAudioOnly:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
			QStringList paths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
			QString musicLocation = paths[0];
#else
			QString musicLocation = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
#endif //QT_5_OR_GREATER
			defaultDir = musicLocation.toStdString();
			if( 0 != defaultDir.length() )
			{
				VxFileUtil::makeForwardSlashPath( defaultDir );
				defaultDir += "/";
			}
		}
    	
    	break;
    	
	case eFileFilterVideo:
    case eFileFilterVideoOnly:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
			QStringList paths = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
			QString moviesLocation = paths[0];
#else
			QString moviesLocation = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
#endif //QT_5_OR_GREATER
			defaultDir = moviesLocation.toStdString();
			if( 0 != defaultDir.length() )
			{
				VxFileUtil::makeForwardSlashPath( defaultDir );
				defaultDir += "/";
			}
		}

		break;

	case eFileFilterDocuments:
		{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
			QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
			QString docsLocation = paths[0];
#else
			QString docsLocation = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#endif //QT_5_OR_GREATER
			defaultDir = docsLocation.toStdString();
			if( 0 != defaultDir.length() )
			{
				VxFileUtil::makeForwardSlashPath( defaultDir );
				defaultDir += "/";
			}
		}
    	
    	break;
    	
    default:
		break;
	}
	
	if( 0 == defaultDir.length() )
	{
		defaultDir = VxGetDownloadsDirectory();
	}
	
	return defaultDir;
}

//============================================================================
void ActivityBrowseFiles::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
	slotRequestFileList();
}

//============================================================================
void ActivityBrowseFiles::hideEvent( QHideEvent * ev )
{
	m_MyApp.wantToGuiFileXferCallbacks( this, this, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
void ActivityBrowseFiles::toGuiFileList( void * userData, VxMyFileInfo& fileInfo )
{
	emit signalToGuiFileList( fileInfo );
}

//============================================================================
void ActivityBrowseFiles::slotToGuiFileList( VxMyFileInfo& fileInfo )
{
	if( fileInfo.getFullFileName().isEmpty() )
	{
		setActionEnable( true );
		//statusMsg( "List Get Completed" );
	}
	else
	{
		addFile( fileInfo, fileInfo.getIsShared(), fileInfo.getIsInLibrary() );
	}
}

//============================================================================
void ActivityBrowseFiles::setCurrentBrowseDir( QString browseDir )
{
	if( !browseDir.isEmpty() )
	{
		m_CurBrowseDirectory = browseDir.toUtf8().constData();
		m_MyApp.getAppSettings().setLastBrowseDir( m_eFileFilterType, m_CurBrowseDirectory );
		ui.m_CurDirLabel->setTextBreakAnywhere( m_CurBrowseDirectory.c_str(), 4 );
	}
}

//============================================================================
void ActivityBrowseFiles::setFileFilter( EFileFilterType eFileFilter )
{
    ui.m_FileFilterComboBox->setFileFilter( eFileFilter );
	m_FileFilterMask = ui.m_FileFilterComboBox->getMaskFromFileFilterType( eFileFilter );
	//if( m_eFileFilterType != eFileFilter )
	{
		m_eFileFilterType = eFileFilter;
		setDefaultCurrentDir( eFileFilter );
	}
}

//============================================================================
FileShareItemWidget * ActivityBrowseFiles::fileToWidget( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary )
{
	FileShareItemWidget * item = new FileShareItemWidget(ui.FileItemList);
	item->setSizeHint(QSize( (int)(GuiParams::getGuiScale() * 200),
                             (int)(62 * GuiParams::getGuiScale()) ) );

	FileItemInfo * poItemInfo = new FileItemInfo( fileInfo, 0, isShared, isInLibrary );
	poItemInfo->setIsShared( isShared );
	item->setFileItemInfo( poItemInfo );

	connect(	item, SIGNAL(signalFileShareItemClicked(QListWidgetItem*)), 
				this, SLOT(slotListItemClicked(QListWidgetItem*)) );

	connect(	item, SIGNAL(signalFileIconClicked(QListWidgetItem*)), 
				this, SLOT(slotListFileIconClicked(QListWidgetItem*)) );

	connect(	item, SIGNAL(signalPlayButtonClicked(QListWidgetItem*)), 
				this, SLOT(slotListPlayIconClicked(QListWidgetItem*)) );

	connect(	item, SIGNAL(signalLibraryButtonClicked(QListWidgetItem*)), 
				this, SLOT(slotListLibraryIconClicked(QListWidgetItem*)) );

	connect(	item, SIGNAL(signalFileShareButtonClicked(QListWidgetItem*)), 
				this, SLOT(slotListShareFileIconClicked(QListWidgetItem*)) );

	connect(	item, SIGNAL(signalShredButtonClicked(QListWidgetItem*)), 
				this, SLOT(slotListShredIconClicked(QListWidgetItem*)) );

	item->updateWidgetFromInfo();

	return item;
}

//============================================================================
void ActivityBrowseFiles::addFile(		VxMyFileInfo&	fileInfo,
										bool			isShared,
										bool			isInLibrary )
{
	FileShareItemWidget * item = fileToWidget( fileInfo, isShared, isInLibrary );
	if( item )
	{
		if( fileInfo.isDirectory() )
		{
			if( 0 == ui.FileItemList->count() )
			{
				LogMsg( LOG_INFO, "add directory %s\n", fileInfo.getJustFileName().toUtf8().constData() );
				ui.FileItemList->addItem( item );
			}
			else
			{
				LogMsg( LOG_INFO, "insert 0 directory %s\n", fileInfo.getJustFileName().toUtf8().constData() );
				ui.FileItemList->insertItem( 0, (QListWidgetItem *)item );
			}
		}
		else
		{
			bool itemInserted = false;
			QString justFileName = fileInfo.getJustFileName();
			for(int i = 0; i < ui.FileItemList->count(); i++ )
			{
				QListWidgetItem* itemInList = ui.FileItemList->item(i);
				FileItemInfo * poInfo = ((FileShareItemWidget *)itemInList)->getFileItemInfo();
				if( poInfo->isDirectory() )
				{
					continue;
				}

				if( poInfo->getJustFileName() > justFileName )
				{
					itemInserted = true;
					LogMsg( LOG_INFO, "insert %d file %s\n", i, fileInfo.getJustFileName().toUtf8().constData() );
					ui.FileItemList->insertItem( i, (QListWidgetItem *)item );
					break;
				}
			}


			if( false == itemInserted )
			{
				LogMsg( LOG_INFO, "add file %s\n", fileInfo.getJustFileName().toUtf8().constData() );
				ui.FileItemList->addItem( item );
			}
		}

		ui.FileItemList->setItemWidget( (QListWidgetItem *)item, (QWidget *)item );
	}
}

//============================================================================
void ActivityBrowseFiles::slotHomeButtonClicked( void )
{
	hide();
}

//============================================================================
void ActivityBrowseFiles::slotUpDirectoryClicked( void )
{
	if( m_CurBrowseDirectory.length() > 1 )
	{
		// move up one directory
		char * pBuf = new char[ m_CurBrowseDirectory.length() + 1 ];
		strcpy( pBuf, m_CurBrowseDirectory.c_str() );
		if( '/' == pBuf[ m_CurBrowseDirectory.length() - 1 ] )
		{
			pBuf[ m_CurBrowseDirectory.length() - 1 ] = 0;
		}

		char * pTemp = strrchr( pBuf, '/' );
		if( pTemp )
		{
			pTemp++;
			pTemp[0] = 0;
		}

		m_CurBrowseDirectory = pBuf;
		VxFileUtil::assureTrailingDirectorySlash( m_CurBrowseDirectory );
        delete[] pBuf;
		setActionEnable( false );
		slotRequestFileList();
	}
}

//============================================================================
void ActivityBrowseFiles::slotBrowseButtonClicked( void )
{
	QString selectedDir = "";
	QString curDir = m_CurBrowseDirectory.c_str();
	QFileDialog dialog( (QWidget *)this->parent(), QObject::tr("Open Directory"), curDir );

#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
	dialog.setFileMode(QFileDialog::Directory);
#else
	dialog.setFileMode(QFileDialog::DirectoryOnly);
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
	dialog.setOptions( QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
	dialog.setDirectory( curDir );
	QStringList fileNames;
	if (dialog.exec())
	{
		fileNames = dialog.selectedFiles();
		if( fileNames.size() )
		{
			selectedDir = fileNames[0];
		}
	}

	if( false == selectedDir.isEmpty() )
	{
		selectedDir.replace( "\\", "/" );
		if( '/' != selectedDir.at(selectedDir.length() - 1) )
		{
			selectedDir.append( "/" );
		}
	}

	//QString strDir = GuiHelpers::browseForDirectory( m_CurBrowseDirectory.c_str(), this );
	if( !selectedDir.isEmpty() )
	{
		setCurrentBrowseDir( selectedDir );
		setActionEnable( false );
		slotRequestFileList();
	}
}

//============================================================================
void ActivityBrowseFiles::slotListItemClicked( QListWidgetItem * item )
{
	if( 1000 > m_ClickToFastTimer.elapsedMs() )
	{
		// in windows if user clicks to fast on some files the app will crash.. don't know why
		return;
	}

	FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
	if( poInfo )
	{
		if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
		{
            if( ! poInfo->getFullFileName().isEmpty() )
			{
                m_CurBrowseDirectory =  poInfo->getFullFileName().toUtf8().constData();
				VxFileUtil::assureTrailingDirectorySlash( m_CurBrowseDirectory );
				m_WidgetClickEventFixTimer->start(10);
			}
		}
		else
		{
			// is file
			if( m_IsSelectAFileMode )
			{
				// dont know why but get single click then double click so just send on to double click
				slotListItemDoubleClicked( item );
				return;
			}

			m_ClickToFastTimer.startTimer();
			if( poInfo->shouldOpenFile() )
			{
				this->playFile( poInfo->getFullFileName() );
			}
		}
	}
}

//============================================================================
void ActivityBrowseFiles::slotListItemDoubleClicked( QListWidgetItem * item )
{
	if( 1000 > m_ClickToFastTimer.elapsedMs() )
	{
		// in windows if user clicks to fast on some files the app will crash.. don't know why
		return;
	}

	if( m_IsSelectAFileMode )
	{
		FileItemInfo * poInfo = ((FileShareItemWidget *)item)->getFileItemInfo();
		if( poInfo )
		{
			if( VXFILE_TYPE_DIRECTORY == poInfo->getFileType() )
			{
				slotListItemClicked( item ); 
			}
			else
			{
				// is file
				m_ClickToFastTimer.startTimer();
				if( poInfo->shouldOpenFile() )
				{
					m_FileWasSelected			= true;
					m_SelectedFileType			= poInfo->getFileType();
					m_SelectedFileName			= poInfo->getFullFileName().toUtf8().constData();
					m_SelectedFileLen			= poInfo->getFileLength();
					m_SelectedFileIsShared		= poInfo->getIsShared();
					m_SelectedFileIsInLibrary	= poInfo->getIsInLibrary();
					accept();
				}
			}
		}
	}
	else
	{
		slotListItemClicked( item );
	}
}

//============================================================================
void ActivityBrowseFiles::slotListFileIconClicked( QListWidgetItem * item )
{
	slotListItemClicked( item ); 
}

//============================================================================
void ActivityBrowseFiles::slotListShareFileIconClicked( QListWidgetItem * item )
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
					m_CurBrowseDirectory = strDir;
					VxFileUtil::assureTrailingDirectorySlash( m_CurBrowseDirectory );
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
            m_Engine.fromGuiSetFileIsShared( poInfo->getFullFileName().toUtf8().constData(), poInfo->getIsShared() );
		}
	}	
}

//============================================================================
void ActivityBrowseFiles::slotListLibraryIconClicked( QListWidgetItem * item )
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
					m_CurBrowseDirectory = strDir;
					VxFileUtil::assureTrailingDirectorySlash( m_CurBrowseDirectory );
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
			m_Engine.fromGuiAddFileToLibrary(	poInfo->getFullFileName().toUtf8().constData(), 
												poInfo->getIsInLibrary(),
												poInfo->getFileHashId().getHashData() );
		}
	}	
}

//============================================================================
void ActivityBrowseFiles::slotListPlayIconClicked( QListWidgetItem * item )
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
					m_CurBrowseDirectory = strDir;
					VxFileUtil::assureTrailingDirectorySlash( m_CurBrowseDirectory );
					setActionEnable( false );
					fromListWidgetRequestFileList();
				}
			}
		}
		else
		{
			// play file
			this->playFile( poInfo->getFullFileName() );
		}
	}	
}

//============================================================================
void ActivityBrowseFiles::slotListShredIconClicked( QListWidgetItem * item )
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
					m_CurBrowseDirectory = strDir;
					VxFileUtil::assureTrailingDirectorySlash( m_CurBrowseDirectory );
					setActionEnable( false );
					fromListWidgetRequestFileList();
				}
			}
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
void ActivityBrowseFiles::setActionEnable( bool enable )
{
	m_bFetchInProgress = enable ? false : true;

	LogMsg( LOG_INFO, "Fetch In Progress %d\n", m_bFetchInProgress );
	ui.m_UpDirectoryButton->setEnabled( enable );
}

//============================================================================
void ActivityBrowseFiles::fromListWidgetRequestFileList( void )
{
	// there is a problem in that if we clear the list in a click event from a widget in the list
	// then causes crash so emit timed event to let the click event finish
	m_WidgetClickEventFixTimer->start();
}

//============================================================================
void ActivityBrowseFiles::slotApplyFileFilter( unsigned char fileMask )
{
	m_eFileFilterType = ui.m_FileFilterComboBox->getCurrentFileFilterType();
	setDefaultCurrentDir( ui.m_FileFilterComboBox->getCurrentFileFilterType() );
	m_FileFilterMask = fileMask;
	slotRequestFileList();
}

//============================================================================
void ActivityBrowseFiles::slotRequestFileList( void )
{
	m_WidgetClickEventFixTimer->stop();
	clearFileList();
	ui.m_CurDirLabel->setText( m_CurBrowseDirectory.c_str() );
	m_MyApp.getAppSettings().setLastBrowseDir( m_eFileFilterType, m_CurBrowseDirectory );
	m_Engine.getFromGuiInterface().fromGuiBrowseFiles( m_CurBrowseDirectory.c_str(), true, m_FileFilterMask | VXFILE_TYPE_DIRECTORY );
}

//============================================================================
void ActivityBrowseFiles::clearFileList( void )
{
	for(int i = 0; i < ui.FileItemList->count(); ++i)
	{
		QListWidgetItem* item = ui.FileItemList->item(i);
		delete ((FileShareItemWidget *)item);
	}
	
	ui.FileItemList->clear();
}
