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
// http://www.nolimitconnect.com
//============================================================================

#include "ActivityBase.h"
#include "ui_ActivityToFriendViewSharedFiles.h"
#include "ToGuiFileXferInterface.h"

#include <PktLib/VxCommon.h>

#include <QString>
#include <QDialog>

class FileXferWidget;
class GuiFileXferSession;
class FileListReplySession;
class VxNetIdent;
class VxSha1Hash;
class VxMyFileInfo;

class ActivityToFriendViewSharedFiles : public ActivityBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:
	ActivityToFriendViewSharedFiles(	AppCommon&	myApp, 
										VxNetIdent *	netIdent, 
										QWidget *		parent = NULL );
	virtual ~ActivityToFriendViewSharedFiles() override = default;

    // overrides required for dialogs with there own title bar and bottom bar widgets
    virtual TitleBarWidget *	getTitleBarWidget( void ) override { return ui.m_TitleBarWidget; }
    virtual BottomBarWidget *	getBottomBarWidget( void ) override { return ui.m_BottomBarWidget; }

public:
    void						addFile( VxNetIdent * netIdent, VxMyFileInfo& fileInfo  );

signals:
	void						signalToGuiFileListReply( FileListReplySession* replySession );
	void						signalToGuiFileXferState( VxGuidQt lclSessionId, EXferState eXferState, int param1, int param2 );
	void						signalToGuiStartDownload( GuiFileXferSession * xferSession );
	void						signalToGuiFileDownloadComplete( VxGuidQt lclSessionId, QString newFileName, EXferError xferError );

private slots:
	void						slotToGuiFileListReply( FileListReplySession* replySession );
	void						slotToGuiFileXferState( VxGuidQt lclSessionId, EXferState eXferState, int param1, int param2 );
	void						slotToGuiStartDownload( GuiFileXferSession * xferSession );
	void						slotToGuiFileDownloadComplete( VxGuidQt lclSessionId, QString newFileName, EXferError xferError );

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

    virtual void				toGuiFileListReply(	void * userData, FileListReplySession * replySession ) override;
    virtual void				toGuiFileXferState( void * userData, VxGUID& lclSession, EXferState eXferState, int param1, int param2 ) override;
    virtual void				toGuiStartDownload( void * userData, GuiFileXferSession * xferSession ) override;
    virtual void				toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError ) override;

	FileXferWidget *			fileToWidget( VxNetIdent *      netIdent,
                                              VxMyFileInfo&     fileInfo );
	void						updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * xferSession );
	GuiFileXferSession *		widgetToFileItemInfo( FileXferWidget * item );

	FileXferWidget *			findListEntryWidget( VxGuidQt fileInstance );
	GuiFileXferSession *		findSession( VxGuidQt lclSessionId );

	void						clearFileList( void );
	void						moveUpOneFolder( void );
	void						promptForDownload( GuiFileXferSession * poInfo );
	void						beginDownload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	void						cancelDownload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	void						cancelUpload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	void						removeDownload( GuiFileXferSession * xferSession, QListWidgetItem * item );
	bool						confirmDeleteFile( bool shredFile );

	//=== vars ===//
	Ui::ViewFriendsSharedFilesDialog	ui;
	VxNetIdent *				m_Friend;
	int							m_iMenuItemHeight;
	std::string					m_strCurrentDirectory;
	GuiFileXferSession *		m_SelectedFileInfo;
	uint8_t							m_u8FileFilter;
	VxGUID						m_LclSessionId;
};
