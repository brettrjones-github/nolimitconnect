#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "AppletPeerBase.h"
#include "ui_AppletPeerViewSharedFiles.h"
#include "ToGuiFileXferInterface.h"

#include <PktLib/VxCommon.h>

#include <QString>
#include <QDialog>

class FileXferWidget;
class GuiFileXferSession;
class FileListReplySession;
class GuiUser;
class VxSha1Hash;
class VxMyFileInfo;

class AppletPeerViewSharedFiles : public AppletPeerBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:
	AppletPeerViewSharedFiles( AppCommon& myApp, QWidget * parent = nullptr );
	virtual ~AppletPeerViewSharedFiles() override = default;

public:
    void						addFile( GuiUser * netIdent, VxMyFileInfo& fileInfo  );

private slots:
    void						slotHomeButtonClicked( void ) override;
	void						statusMsg( QString strMsg );
	void						slotApplyFileFilter( unsigned char fileTypeMask );
	void						slotDownloadFileSelected( int iMenuId, QWidget * popupMenu );

	void						slotItemClicked( QListWidgetItem * item );
	void						slotCancelButtonClicked( QListWidgetItem * item );
	void						slotPlayButtonClicked( QListWidgetItem * item );
	void						slotLibraryButtonClicked( QListWidgetItem * item );
	void						slotFileShareButtonClicked( QListWidgetItem * item );
	void						slotShredButtonClicked( QListWidgetItem * item );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;

    virtual void				toGuiFileListReply(	FileListReplySession * replySession ) override;
    virtual void				toGuiFileXferState( EPluginType pluginType, VxGUID& lclSession, EXferState eXferState, int param1, int param2 ) override;
    virtual void				toGuiStartDownload( GuiFileXferSession * xferSession ) override;
    virtual void				toGuiFileDownloadComplete( EPluginType pluginType, VxGUID& lclSession, QString newFileName, EXferError xferError ) override;

	FileXferWidget *			fileToWidget( GuiUser * netIdent, VxMyFileInfo& fileInfo );
	void						updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * xferSession );
	GuiFileXferSession *		widgetToFileItemInfo( FileXferWidget * item );

	FileXferWidget *			findListEntryWidget( VxGUID fileInstance );
	GuiFileXferSession *		findSession( VxGUID lclSessionId );

	void						clearFileList( void );
	void						moveUpOneFolder( void );
	void						promptForDownload( GuiFileXferSession * poInfo );
	void						beginDownload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	void						cancelDownload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	void						cancelUpload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	void						removeDownload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	bool						confirmDeleteFile( bool shredFile );

	//=== vars ===//
	Ui::AppletPeerViewSharedFilesUi	ui;
    GuiUser *				    m_Friend{ nullptr };
	int							m_iMenuItemHeight{ 34 };
	std::string					m_strCurrentDirectory;
	GuiFileXferSession *		m_SelectedFileInfo{ nullptr };
	uint8_t						m_u8FileFilter{ 0 };
	VxGUID						m_LclSessionId;
};
