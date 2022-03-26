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

#include "AppletPeerBase.h"
#include "ToGuiFileXferInterface.h"
#include "ui_AppletUploads.h"

#include <PktLib/VxCommon.h>

class GuiFileXferSession;
class FileXferWidget;

class AppletUploads : public AppletPeerBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:
	AppletUploads(	AppCommon&	app, 
						QWidget *		parent = NULL );
	virtual ~AppletUploads() override;

public:
	FileXferWidget *			addDownload( GuiFileXferSession * poFileInfo );

	bool						isUploadInProgress( VxGUID fileInstance );

	GuiFileXferSession *		findSession( VxGUID fileInstance );
	FileXferWidget *			findListEntryWidget( VxGUID fileInstance );
	FileXferWidget *			addUpload( GuiFileXferSession * poSession );

private slots:
    void						slotHomeButtonClicked( void ) override;
	void						slotFileXferItemClicked( QListWidgetItem * item );

	void						slotFileIconButtonClicked( QListWidgetItem * item );
	void						slotCancelButtonClicked( QListWidgetItem * item );
	void						slotPlayButtonClicked( QListWidgetItem * item );
	void						slotLibraryButtonClicked( QListWidgetItem * item );
	void						slotFileShareButtonClicked( QListWidgetItem * item );
	void						slotShredButtonClicked( QListWidgetItem * item );

protected:
    virtual void				showEvent( QShowEvent * ev ) override;
    virtual void				hideEvent( QHideEvent * ev ) override;

    // override default behavior of closing dialog when back button is clicked
    void                        onBackButtonClicked( void ) override;

    virtual void				toGuiStartUpload( GuiFileXferSession * xferSession ) override;
    virtual void				toGuiFileXferState( EPluginType plguinType, VxGUID& lclSession, EXferState eXferState, int param1, int param2 ) override;
    virtual void				toGuiFileUploadComplete( EPluginType plguinType, VxGUID& lclSession, EXferError xferError ) override;

	FileXferWidget *			sessionToWidget( GuiFileXferSession * poSession );
	void						updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * poSession );
	GuiFileXferSession *		widgetToSession( FileXferWidget * item );
	bool						confirmDeleteFile( bool shredFile );

	//=== vars ===//
	Ui::AppletUploadsUi			ui;
};

