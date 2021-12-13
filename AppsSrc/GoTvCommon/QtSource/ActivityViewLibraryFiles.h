#pragma once
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

#include "ActivityBase.h"
#include "VxMyFileInfo.h"
#include "ToGuiFileXferInterface.h"

#include "ui_ActivityViewLibraryFiles.h"

#include <PktLib/VxCommon.h>

class FileShareItemWidget;
class VxNetIdent;
class VxSha1Hash;
class FileItemInfo;
class FileListReplySession;
class VxMyFileInfo;

class ActivityViewLibraryFiles : public ActivityBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:
	ActivityViewLibraryFiles( AppCommon& app, QWidget * parent = NULL, bool isSelectAFileMode = false );
	virtual ~ActivityViewLibraryFiles() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public:
	void						addFile( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary );
	void						setFileFilter( EFileFilterType eFileFilter );
	bool						getWasFileSelected( void )						{ return m_FileWasSelected; }
	uint8_t						getSelectedFileType( void )						{ return m_SelectedFileType; }
	QString						getSelectedFileName( void )						{ return m_SelectedFileName; }
	uint64_t					getSelectedFileLen( void )						{ return m_SelectedFileLen; }
	bool						getSelectedFileIsShared( void )					{ return m_SelectedFileIsShared; }
	bool						getSelectedFileIsInLibrary( void )				{ return m_SelectedFileIsInLibrary; }

signals:
	void						signalToGuiFileList( VxMyFileInfo& fileInfo );

private slots:
    void						slotHomeButtonClicked( void ) override;
	void						statusMsg( QString strMsg );
	void						slotRequestFileList( void );
	void						slotApplyFileFilter( unsigned char fileTypeMask );
	void						slotAddFilesButtonClicked( void );
	void						slotToGuiFileList( VxMyFileInfo& fileInfo );

	void						slotListItemClicked( QListWidgetItem * item );
	void						slotListItemDoubleClicked( QListWidgetItem * item );

	void						slotListFileIconClicked( QListWidgetItem * item );
	void						slotListPlayIconClicked( QListWidgetItem * item );
	void						slotListLibraryIconClicked( QListWidgetItem * item );
	void						slotListShareFileIconClicked( QListWidgetItem * item );
	void						slotListShredIconClicked( QListWidgetItem * item );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;
    virtual void				toGuiFileList( void * userData, VxMyFileInfo& fileInfo ) override;

	FileShareItemWidget *		fileToWidget( VxMyFileInfo& fileInfo, bool isShared, bool isInLibrary );
	FileItemInfo *				widgetToFileItemInfo( FileShareItemWidget * item );

	FileShareItemWidget *		findListEntryWidget( VxMyFileInfo& fileInfo );
	void						clearFileList( void );
	bool						confirmDeleteFile( QString fileName, bool shredFile );

	//=== vars ===//
	Ui::ViewLibraryFilesDialog	ui;
    EPluginType					m_ePluginType;
	bool						m_IsSelectAFileMode;
	bool						m_FileWasSelected;
	uint8_t						m_SelectedFileType;
	QString						m_SelectedFileName;
	uint64_t					m_SelectedFileLen;
	bool						m_SelectedFileIsShared;
	bool						m_SelectedFileIsInLibrary;
	EFileFilterType				m_eFileFilterType;
	uint8_t						m_FileFilterMask;
};
