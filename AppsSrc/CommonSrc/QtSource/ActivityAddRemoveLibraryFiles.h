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
#include "ToGuiFileXferInterface.h"
#include "ui_ActivityAddRemoveLibraryFiles.h"

#include <CoreLib/VxDefs.h>
#include <CoreLib/VxGUID.h>

class AppCommon;
class FileShareItemWidget;
class FileItemInfo;
class FromGuiInterface;
class FromEngineInterface;
class P2PEngine;
class QTimer;

class ActivityAddRemoveLibraryFiles : public ActivityBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:

    ActivityAddRemoveLibraryFiles( AppCommon& app, QWidget* parent = nullptr );
    virtual ~ActivityAddRemoveLibraryFiles();

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

	void						setTitle( QString strTitle );

	FileShareItemWidget*		fileToWidget( FileInfo& fileInfo );
	void						updateListEntryWidget( FileShareItemWidget* item );

protected slots:
    void						slotHomeButtonClicked( void ) override;
	void						slotUpDirectoryClicked( void );
	void						slotBrowseButtonClicked( void );

	void						slotListItemClicked( QListWidgetItem* item );
	void						slotListFileIconClicked( QListWidgetItem* item );
	void						slotListLockIconClicked( QListWidgetItem* item );
	void						slotRequestFileList( void );
	
protected:
	virtual void				callbackToGuiFileList( FileInfo& fileInfo ) override;
	virtual void				callbackToGuiFileListCompleted( void ) override;

	void						fromListWidgetRequestFileList( void );
	void						setCurrentBrowseDir( QString browseDir );
	void						setActionEnable( bool enable );
	void						addFile( FileInfo& fileInfo );

	void						updateListEntryWidget( FileShareItemWidget* item, FileItemInfo* poSession );
	void						clearFileList( void );



	//=== vars ===//
	Ui::ShareFilesDialog		ui;

	std::string					m_strCurBrowseDirectory;
	bool						m_bFetchInProgress;
	QTimer *					m_WidgetClickEventFixTimer;
};


