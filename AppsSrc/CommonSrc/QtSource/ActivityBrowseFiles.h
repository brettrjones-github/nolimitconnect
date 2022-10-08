#pragma once
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

#include "ActivityBase.h"
#include "ui_ActivityBrowseFiles.h"
#include "FileItemInfo.h"
#include "ToGuiFileXferInterface.h"

#include <CoreLib/VxTimer.h>

class AppCommon;
class FileShareItemWidget;
class FromGuiInterface;
class FromEngineInterface;
class P2PEngine;
class QTimer;

class ActivityBrowseFiles : public ActivityBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:

	ActivityBrowseFiles( AppCommon& app, EFileFilterType fileFilter, ActivityBase* parent = nullptr, bool isSelectAFileMode = false );
	virtual ~ActivityBrowseFiles();

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public:
	void						setFileFilter( EFileFilterType eFileFilter );

	FileShareItemWidget*		fileToWidget( FileInfo& fileInfo );
	void						updateListEntryWidget( FileShareItemWidget* item );
	bool						getWasFileSelected( void )						{ return m_FileWasSelected; }
	uint8_t						getSelectedFileType( void )						{ return m_SelectedFileType; }
	QString						getSelectedFileName( void )						{ return m_SelectedFileName; }
	uint64_t					getSelectedFileLen( void )						{ return m_SelectedFileLen; }
	bool						getSelectedFileIsShared( void )					{ return m_SelectedFileIsShared; }
	bool						getSelectedFileIsInLibrary( void )				{ return m_SelectedFileIsInLibrary; }

protected slots:
    void						slotHomeButtonClicked( void ) override;
	void						slotUpDirectoryClicked( void );
	void						slotBrowseButtonClicked( void );

	void						slotRequestFileList( void );
	void						slotApplyFileFilter( unsigned char fileMask );

	void						slotListItemClicked( QListWidgetItem* item );
	void						slotListItemDoubleClicked( QListWidgetItem* item );
	void						slotListFileIconClicked( QListWidgetItem* item );
	void						slotListShareFileIconClicked( QListWidgetItem* item );
	void						slotListLibraryIconClicked( QListWidgetItem* item );
	void						slotListPlayIconClicked( QListWidgetItem* item );
	void						slotListShredIconClicked( QListWidgetItem* item );
	void						slotAddAllButtonClicked( void );
	
protected:
    virtual void				showEvent( QShowEvent* ev ) override;
    virtual void				hideEvent( QHideEvent* ev ) override;

	virtual void				callbackToGuiFileList( FileInfo& fileInfo ) override;
	virtual void				callbackToGuiFileListCompleted( void ) override;

	void						fromListWidgetRequestFileList( void );
	void						setCurrentBrowseDir( QString browseDir );
	void						setActionEnable( bool enable );
	void						addFile( FileInfo& fileInfo );

	void						updateListEntryWidget( FileShareItemWidget* item, FileItemInfo* poSession );
	void						clearFileList( void );
	void						setDefaultCurrentDir( EFileFilterType eFileFilterType );
	std::string					getDefaultDir( int eFileFilterType );

	Ui::BrowseFilesWidget		ui;

	std::string					m_CurBrowseDirectory;
	bool						m_bFetchInProgress;
	QTimer *					m_WidgetClickEventFixTimer;
	bool						m_IsSelectAFileMode;
	bool						m_FileWasSelected;
	uint8_t						m_SelectedFileType;
	QString						m_SelectedFileName;
	uint64_t					m_SelectedFileLen;
	bool						m_SelectedFileIsShared;
	bool						m_SelectedFileIsInLibrary;

	EFileFilterType				m_eFileFilterType;
	uint8_t						m_FileFilterMask;
	VxTimer						m_ClickToFastTimer;
};


