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
// http://www.nolimitconnect.com
//============================================================================

#include "ActivityBase.h"
#include "ui_ActivityViewMySharedFiles.h"
#include "ToGuiFileXferInterface.h"

#include <CoreLib/VxDefs.h>

class AppCommon;
class FileShareItemWidget;
class FileItemInfo;
class FromGuiInterface;
class FromEngineInterface;
class P2PEngine;
class QTimer;
class VxMyFileInfo;

class ActivityViewMySharedFiles : public ActivityBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:

	ActivityViewMySharedFiles( AppCommon& app, QWidget * parent = NULL );
	virtual ~ActivityViewMySharedFiles() override;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public:
	void						setTitle( QString strTitle );

	FileShareItemWidget *		fileToWidget( VxMyFileInfo&	fileInfo, bool isShared );
	void						updateListEntryWidget( FileShareItemWidget * item );

protected slots:
    void						slotHomeButtonClicked( void ) override;
	void						slotAddFilesButtonClicked( void );

	void						slotListItemClicked( QListWidgetItem * item );

	void						slotListFileIconClicked( QListWidgetItem * item );
	void						slotListPlayIconClicked( QListWidgetItem * item );
	void						slotListLibraryIconClicked( QListWidgetItem * item );
	void						slotListShareFileIconClicked( QListWidgetItem * item );
	void						slotListShredIconClicked( QListWidgetItem * item );

	void						slotRequestSharedFileList( uint8_t fileTypeFilter );
	void						slotApplyFileFilter( unsigned char fileFilter );
	
protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;
    virtual void				toGuiFileList( VxMyFileInfo& fileInfo ) override;

	void						setActionEnable( bool enable );
	void						addFile(	VxMyFileInfo&	fileInfo,
											bool			isShared );

	void						updateListEntryWidget( FileShareItemWidget * item, FileItemInfo * poSession );
	void						clearFileList( void );
	void						fromListWidgetRequestFileList( void );

	Ui::ViewMySharedFilesDialog	ui;

	std::string					m_strCurBrowseDirectory;
	bool						m_bFetchInProgress;
	QTimer *					m_WidgetClickEventFixTimer;
};


