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
#include "VxGuidQt.h"
#include "ToGuiFileXferInterface.h"
#include "ui_AppletDownloads.h"

#include <PktLib/VxCommon.h>

class FileXferWidget;
class P2PEngine;
class GuiFileXferSession;

class AppletDownloads : public AppletPeerBase, public ToGuiFileXferInterface
{
	Q_OBJECT
public:
	AppletDownloads(	AppCommon&	app, 
						QWidget *		parent = NULL );
	virtual ~AppletDownloads() override;

	bool						isXferInProgress( VxGuidQt fileInstance );
	FileXferWidget *			addDownload( GuiFileXferSession * poSession );

signals:
	void						signalToGuiStartDownload( GuiFileXferSession * xferSession );
	void						signalToGuiFileXferState( VxGuidQt lclSession, EXferState eXferState, int param1, int param2 );
	void						signalToGuiFileDownloadComplete( VxGuidQt lclSession, QString newFileName, EXferError xferError );

private slots:
	void						slotToGuiStartDownload(	GuiFileXferSession * poSession );
	void						slotToGuiFileXferState( VxGuidQt lclSessionId, EXferState eXferState, int param1, int param2 );
	void						slotToGuiFileDownloadComplete( VxGuidQt lclSessionId, QString newFileName, EXferError xferError );

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

    virtual void				toGuiStartDownload( void * userData, GuiFileXferSession * xferSession ) override;
    virtual void				toGuiFileXferState( void * userData, VxGUID& lclSession, EXferState eXferState, int param1, int param2 ) override;
    virtual void				toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError ) override;

	FileXferWidget *			sessionToWidget( GuiFileXferSession * poSession );
	void						updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * poSession );
	GuiFileXferSession *		widgetToSession( FileXferWidget * item );
	GuiFileXferSession *		findSession( VxGuidQt lclSessionId );
	FileXferWidget *			findListEntryWidget( VxGuidQt lclSessionId );
	bool						confirmDeleteFile( bool shredFile );

	//=== vars ===//
	Ui::DownloadsDialog			ui;
};
