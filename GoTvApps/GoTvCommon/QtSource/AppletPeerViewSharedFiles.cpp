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

#include <app_precompiled_hdr.h>
#include "AppCommon.h"
#include "AppSettings.h"

#include "AppletPeerViewSharedFiles.h"

#include "AppletDownloads.h"
#include "ActivityMessageBox.h"
#include "ActivityYesNoMsgBox.h"

#include "FileXferWidget.h"
#include "MyIcons.h"
#include "PopupMenu.h"
#include "AppGlobals.h"
#include "GuiFileXferSession.h"
#include "FileListReplySession.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <PktLib/VxSearchDefs.h>
#include <NetLib/VxFileXferInfo.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
AppletPeerViewSharedFiles::AppletPeerViewSharedFiles( AppCommon& app, QWidget *	parent )
: AppletPeerBase( OBJNAME_ACTIVITY_TO_FRIEND_VIEW_SHARED_FILES, app, parent )
, m_u8FileFilter( VXFILE_TYPE_ALLNOTEXE )
{
	m_LclSessionId.initializeWithNewVxGUID();
    setPluginType( ePluginTypeVideoPhone );
    setAppletType( eAppletPeerViewSharedFiles );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    connectBarWidgets();

    connect(ui.FileItemList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));
    connect(ui.FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotItemClicked(QListWidgetItem *)));
	connect( ui.m_FileFilterComboBox, SIGNAL(signalApplyFileFilter(unsigned char)),			this,  SLOT(slotApplyFileFilter(unsigned char)) );

	connect( this, SIGNAL(signalToGuiFileListReply(FileListReplySession*)),					this, SLOT(slotToGuiFileListReply(FileListReplySession*)) );
	connect( this, SIGNAL(signalToGuiFileXferState(VxGUID, EXferState, int, int)),		this, SLOT(slotToGuiFileXferState(VxGUID, EXferState, int, int)) );

	connect( this, SIGNAL(signalToGuiStartDownload(GuiFileXferSession *)),					this, SLOT(slotToGuiStartDownload(GuiFileXferSession *)) );
	connect( this, SIGNAL(signalToGuiFileDownloadComplete(VxGUID,QString,EXferError)),	this, SLOT(slotToGuiFileDownloadComplete(VxGUID,QString,EXferError)) );

	m_MyApp.wantToGuiFileXferCallbacks( this, this, true );

    /*
    ui.m_TitleBarWidget->setTitleBarText( strTitle + "'s Files" );
    statusMsg( "Requesting File List " );

    QString strTitle( netIdent->getOnlineName() );
	m_FromGui.fromGuiMakePluginOffer( ePluginTypeFileServer, netIdent->getMyOnlineId(), 0, "View Files", 0, 0, m_LclSessionId );
    */

	// plugin control
	//"ViewDirectory"
	//"DownloadFile"
	checkDiskSpace();
}

//============================================================================
void AppletPeerViewSharedFiles::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
}

//============================================================================
void AppletPeerViewSharedFiles::hideEvent( QHideEvent * ev )
{
	m_MyApp.wantToGuiFileXferCallbacks( this, this, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletPeerViewSharedFiles::toGuiFileListReply( void * userData, FileListReplySession * replySession )
{
	Q_UNUSED( userData );
	emit signalToGuiFileListReply( replySession );
}

//============================================================================
void AppletPeerViewSharedFiles::slotToGuiFileListReply( FileListReplySession* session ) 
{
	addFile( session->getIdent(), session->getFileInfo() );
}

//============================================================================
void AppletPeerViewSharedFiles::toGuiFileXferState( void * userData, VxGUID& lclSession, EXferState eXferState, int param1, int param2 )
{
	Q_UNUSED( userData );
	emit signalToGuiFileXferState( lclSession, eXferState, param1, param2 );
}

//============================================================================
void AppletPeerViewSharedFiles::slotToGuiFileXferState( VxGUID lclSessionId, EXferState eXferState, int param1, int param2 )
{
	LogMsg( LOG_INFO, "Got Update File Download\n");
	FileXferWidget * item = findListEntryWidget( lclSessionId );
	if( item )
	{
		item->setXferState( eXferState, param1, param2 );
	}
}

//============================================================================
void AppletPeerViewSharedFiles::toGuiStartDownload( void * userData, GuiFileXferSession * xferSession )
{
	Q_UNUSED( userData );
	emit signalToGuiStartDownload( xferSession );
}

//============================================================================
GuiFileXferSession * AppletPeerViewSharedFiles::findSession( VxGUID lclSessionId )
{
	int iCnt = ui.FileItemList->count();
	for( int iRow = 0; iRow < iCnt; iRow++ )
	{
		QListWidgetItem * item =  ui.FileItemList->item( iRow );
		GuiFileXferSession * poCurInfo = (GuiFileXferSession *)item->data( Qt::UserRole + 1).toULongLong();
		if( poCurInfo->getLclSessionId() == lclSessionId )
		{
			return poCurInfo;
		}
	}

	return NULL;
}

//============================================================================
void AppletPeerViewSharedFiles::slotToGuiStartDownload( GuiFileXferSession * xferSessionIn )
{
	GuiFileXferSession * xferSession = findSession( xferSessionIn->getLclSessionId() );
	if( xferSession )
	{
		xferSession->setXferState( eXferStateInDownloadXfer, 0, 0 );
		FileXferWidget * item = findListEntryWidget( xferSessionIn->getLclSessionId() );
		if( item )
		{
			item->updateWidgetFromInfo();
		}
	}
}

//============================================================================
void AppletPeerViewSharedFiles::toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError )
{
	Q_UNUSED( userData );
	VxGUID myLclSessionId( lclSession );
	emit signalToGuiFileDownloadComplete( myLclSessionId, newFileName, xferError );
}

//============================================================================
void AppletPeerViewSharedFiles::slotToGuiFileDownloadComplete( VxGUID lclSessionId, QString newFileName, EXferError xferError )
{
	GuiFileXferSession * xferSession = findSession( lclSessionId );
	if( xferSession )
	{
		if( !newFileName.isEmpty() )
		{
			xferSession->setFullFileName( newFileName );
		}

		xferSession->setXferState( eXferStateCompletedDownload, xferError, 0 );
		FileXferWidget * item = findListEntryWidget( lclSessionId );
		if( item )
		{
			item->updateWidgetFromInfo();
		}
	}
}

//============================================================================
void AppletPeerViewSharedFiles::statusMsg( QString strMsg )
{
	//LogMsg( LOG_INFO, strMsg.toStdString().c_str() );
	ui.m_StatusMsgLabel->setText( strMsg );
}

//============================================================================
void AppletPeerViewSharedFiles::slotApplyFileFilter( unsigned char fileTypeMask )
{
	m_u8FileFilter = fileTypeMask;
	int iIdx = 0;
	FileXferWidget * poWidget;
	while( iIdx < ui.FileItemList->count() )
	{
		poWidget = (FileXferWidget *)ui.FileItemList->item(iIdx);
		if( poWidget )
		{
			GuiFileXferSession * poFileInfo = (GuiFileXferSession *)poWidget->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
			updateListEntryWidget( poWidget, poFileInfo );
		}

		iIdx++;
	}
}

//============================================================================
FileXferWidget * AppletPeerViewSharedFiles::fileToWidget( GuiUser * netIdent, VxMyFileInfo& fileInfo )
{
	FileXferWidget * item = new FileXferWidget(ui.FileItemList);
    item->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );
    VxGUID lclSessionId;
    lclSessionId.initializeWithNewVxGUID();
	GuiFileXferSession * xferSession = new GuiFileXferSession(	ePluginTypeFileServer, 
																netIdent, 
                                                                lclSessionId,
                                                                fileInfo );
	xferSession->setXferDirection( eXferDirectionRx );
	xferSession->setWidget( item );
    item->QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)xferSession) );

    connect( item, SIGNAL(signalFileXferItemClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
    connect( item, SIGNAL(signalFileIconButtonClicked(QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));

	connect( item, SIGNAL(signalCancelButtonClicked(QListWidgetItem*)),		this, SLOT(slotCancelButtonClicked(QListWidgetItem*)) );
	connect( item, SIGNAL(signalPlayButtonClicked(QListWidgetItem*)),		this, SLOT(slotPlayButtonClicked(QListWidgetItem*)) );
	connect( item, SIGNAL(signalLibraryButtonClicked(QListWidgetItem*)),	this, SLOT(slotLibraryButtonClicked(QListWidgetItem*)) );
	connect( item, SIGNAL(signalFileShareButtonClicked(QListWidgetItem*)),	this, SLOT(slotFileShareButtonClicked(QListWidgetItem*)) );
	connect( item, SIGNAL(signalShredButtonClicked(QListWidgetItem*)),		this, SLOT(slotShredButtonClicked(QListWidgetItem*)) );

	updateListEntryWidget( item, xferSession );

	return item;
}

//============================================================================
void AppletPeerViewSharedFiles::updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * xferSession )
{
	if( ( 0 == item )
		|| ( 0 == xferSession ) )
	{
		return;
	}

	item->updateWidgetFromInfo();
	if( 0 == ( xferSession->getFileType() & m_u8FileFilter ) )
	{
		item->setVisible( false );
	}
	else
	{
		item->setVisible( true );
	}

	int percent = m_FromGui.fromGuiGetFileDownloadState( xferSession->getFileHashId().getHashData() );
	if( percent < 0 )
	{
		item->setFileIconButtonEnabled( true );
		item->setFileProgressBarValue( 0 );
	}
	else
	{
		item->setFileIconButtonEnabled( false );
		item->setFileProgressBarValue( percent );
	}
}

//============================================================================
GuiFileXferSession * AppletPeerViewSharedFiles::widgetToFileItemInfo( FileXferWidget * item )
{
	return (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
FileXferWidget * AppletPeerViewSharedFiles::findListEntryWidget( VxGUID lclSessionId )
{
	int iIdx = 0;
	FileXferWidget * poWidget;
	while( iIdx < ui.FileItemList->count() )
	{
		poWidget = (FileXferWidget *)ui.FileItemList->item(iIdx);
		if( poWidget )
		{
			GuiFileXferSession * poFileInfo = (GuiFileXferSession *)poWidget->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
			if( poFileInfo && ( poFileInfo->getLclSessionId() == lclSessionId ) )
			{
				return poWidget;
			}
		}

		iIdx++;
	}

	return NULL;
}

//============================================================================
void AppletPeerViewSharedFiles::addFile( GuiUser * netIdent, VxMyFileInfo& fileInfo )
{
    if( fileInfo.getFileLength()
        && !fileInfo.getFullFileName().isEmpty() )
	{
        FileXferWidget * item = fileToWidget( netIdent, fileInfo );
		if( item )
		{
			//LogMsg( LOG_INFO, "AppletPeerViewSharedFiles::addFile: adding widget\n");
			ui.FileItemList->addItem( item );
			ui.FileItemList->setItemWidget( item, item );
		}
	}
	else
	{
		statusMsg( "File List Complete" );
	}
}

//============================================================================
void AppletPeerViewSharedFiles::slotHomeButtonClicked( void )
{
	accept();
}

//============================================================================
void AppletPeerViewSharedFiles::slotItemClicked(QListWidgetItem * item)
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->data(Qt::UserRole + 1).toLongLong();
	if( xferSession )
	{
		if( -1 == m_FromGui.fromGuiGetFileDownloadState( xferSession->getFileHashId().getHashData() ) )
		{
			promptForDownload( xferSession );
		}
	}
}

//============================================================================
void AppletPeerViewSharedFiles::slotCancelButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->data(Qt::UserRole + 1).toLongLong();
	if( xferSession )
	{
		switch( xferSession->getXferState() )
		{
		case eXferStateDownloadNotStarted:
			beginDownload( xferSession, item );
			break;

		case eXferStateCompletedUpload:
		case eXferStateCompletedDownload:
			removeDownload( xferSession, item );
			break;
		case eXferStateWaitingOfferResponse:
		case eXferStateInUploadQue:
		case eXferStateBeginUpload:
		case eXferStateInUploadXfer:
		case eXferStateUserCanceledUpload:
		case eXferStateUploadOfferRejected:
		case eXferStateUploadError:
			cancelUpload( xferSession, item );
			break;

		case eXferStateInDownloadQue:
		case eXferStateBeginDownload:
		case eXferStateInDownloadXfer:
		case eXferStateUserCanceledDownload:
		case eXferStateDownloadError:
			cancelDownload( xferSession, item );
		default:
			break;
		}
	}
}

//============================================================================
void AppletPeerViewSharedFiles::beginDownload( GuiFileXferSession * xferSession, QListWidgetItem * item  )
{
	if(	-1 != m_FromGui.fromGuiGetFileDownloadState( xferSession->getFileHashId().getHashData() ) )
	{
		ActivityMessageBox errMsgBox( m_MyApp, this, LOG_INFO, "File is already downloading" );
		errMsgBox.exec();
	}
	else
	{
		if( false == xferSession->getLclSessionId().isVxGUIDValid() )
		{
			xferSession->getLclSessionId().initializeWithNewVxGUID();
		}

		EXferError xferError = (EXferError)m_FromGui.fromGuiPluginControl(	ePluginTypeFileServer, 
																			m_Friend->getMyOnlineId(), 
																			"DownloadFile", 
																			xferSession->getFullFileName().toUtf8().constData(),
																			0,
																			xferSession->getLclSessionId(),
																			xferSession->getFileHashId().getHashData() );
		if( eXferErrorNone == xferError )
		{
			// make copy.. downloads will delete
			//GuiFileXferSession * poNewInfo = new GuiFileXferSession();
			//*poNewInfo = *m_SelectedFileInfo;
			//m_MyApp.getAppletDownloads()->slotUpdateDownload( poNewInfo );
		}
		else 
		{
			ActivityMessageBox errMsgBox( m_MyApp, this, LOG_INFO, GuiParams::describeEXferError( xferError ) );
			errMsgBox.exec();
		}
	}
}

//============================================================================
void AppletPeerViewSharedFiles::removeDownload( GuiFileXferSession * xferSession, QListWidgetItem * item  )
{
	ui.FileItemList->removeItemWidget( item );
}

//============================================================================
void AppletPeerViewSharedFiles::cancelDownload( GuiFileXferSession * xferSession, QListWidgetItem * item )
{
	xferSession->setXferState( eXferStateUserCanceledDownload, 0, 0 );
	((FileXferWidget *)item)->setXferState( eXferStateUserCanceledDownload, 0, 0 );
	m_Engine.fromGuiCancelDownload( xferSession->getLclSessionId() );

	removeDownload( xferSession, item );
}

//============================================================================
void AppletPeerViewSharedFiles::cancelUpload( GuiFileXferSession * xferSession, QListWidgetItem * item )
{
	xferSession->setXferState( eXferStateUserCanceledUpload, 0, 0 );
	((FileXferWidget *)item)->setXferState( eXferStateUserCanceledUpload, 0, 0 );
	m_Engine.fromGuiCancelUpload( xferSession->getLclSessionId() );

	removeDownload( xferSession, item );
}

//============================================================================
void AppletPeerViewSharedFiles::slotPlayButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->data(Qt::UserRole + 1).toLongLong();
	if( xferSession )
	{
		this->playFile( xferSession->getFullFileName() );
	}
}

//============================================================================
void AppletPeerViewSharedFiles::slotLibraryButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->data(Qt::UserRole + 1).toLongLong();
	if( xferSession )
	{
		bool inLibary = xferSession->getIsInLibrary();
		inLibary = !inLibary;
		xferSession->setIsInLibrary( inLibary );
		m_Engine.fromGuiAddFileToLibrary( xferSession->getFullFileName().toUtf8().constData(), inLibary, xferSession->getFileHashId().getHashData() );
		((FileXferWidget *)item)->updateWidgetFromInfo();
	}
}

//============================================================================
void AppletPeerViewSharedFiles::slotFileShareButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->data(Qt::UserRole + 1).toLongLong();
	if( xferSession )
	{
		bool isShared = xferSession->getIsShared();
		isShared = !isShared;
		xferSession->setIsShared( isShared );
		m_Engine.fromGuiSetFileIsShared( xferSession->getFullFileName().toUtf8().constData(), isShared, xferSession->getFileHashId().getHashData() );
		((FileXferWidget *)item)->updateWidgetFromInfo();
	}
}

//============================================================================
void AppletPeerViewSharedFiles::slotShredButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->data(Qt::UserRole + 1).toLongLong();
	if( xferSession )
	{
		QString fileName = xferSession->getFullFileName();
		if( confirmDeleteFile( true ) )
		{
			removeDownload( xferSession, item );
			m_Engine.fromGuiDeleteFile( fileName.toUtf8().constData(), true );
		}
	}
}

//============================================================================
bool AppletPeerViewSharedFiles::confirmDeleteFile( bool shredFile )
{
	bool acceptAction = true;
	bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
	if( false == isConfirmDisabled )
	{
		QString title = shredFile ?  QObject::tr( "Confirm Shred File" ) : QObject::tr( "Confirm Delete File" );
		QString bodyText = "";
		if( shredFile )
		{
			bodyText = QObject::tr( "Are You Sure You Want To Write Random Data Into The File Then Delete From The Device?" );
		}
		else
		{
			bodyText = QObject::tr( "Are You Sure To Delete The File From The Device?" );
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
void AppletPeerViewSharedFiles::promptForDownload( GuiFileXferSession * poInfo )
{
	m_SelectedFileInfo = poInfo;
	PopupMenu popupMenu( m_MyApp, this );
    popupMenu.setTitleBarWidget( this->getTitleBarWidget() );
    popupMenu.setBottomBarWidget( this->getBottomBarWidget() );
    popupMenu.setTitle( QObject::tr( "Download A File" ) );
    popupMenu.addMenuItem( 1, getMyIcons().getIcon(eMyIconFileDownload), QObject::tr( "Download A File" ) );
    popupMenu.addMenuItem( 2, getMyIcons().getIcon(getMyIcons().getFileIcon(poInfo->getFileType())), poInfo->getJustFileName() );
    connect( &popupMenu, SIGNAL(menuItemClicked(int, PopupMenu *, ActivityBase *)), this, SLOT(slotDownloadFileSelected(int, PopupMenu *, ActivityBase *)));

    popupMenu.exec();
}

//============================================================================
void AppletPeerViewSharedFiles::slotDownloadFileSelected( int iMenuId, QWidget * popupMenu )
{
	if( m_SelectedFileInfo )
	{
		if(	-1 != m_FromGui.fromGuiGetFileDownloadState( m_SelectedFileInfo->getFileHashId().getHashData() ) )
		{
			ActivityMessageBox errMsgBox( m_MyApp, this, LOG_INFO, "File is already downloading" );
			errMsgBox.exec();
		}
		else
		{
			if( false == m_SelectedFileInfo->getLclSessionId().isVxGUIDValid() )
			{
				m_SelectedFileInfo->getLclSessionId().initializeWithNewVxGUID();
			}

			EXferError xferError = (EXferError)m_FromGui.fromGuiPluginControl(	ePluginTypeFileServer, 
																				m_Friend->getMyOnlineId(), 
																				"DownloadFile", 
																				m_SelectedFileInfo->getFullFileName().toUtf8().constData(),
																				0,
																				m_SelectedFileInfo->getLclSessionId(),
																				m_SelectedFileInfo->getFileHashId().getHashData() );

			if( eXferErrorNone == xferError )
			{
				// make copy.. downloads will delete
				//GuiFileXferSession * poNewInfo = new GuiFileXferSession();
				//*poNewInfo = *m_SelectedFileInfo;
				//m_MyApp.getAppletDownloads()->slotUpdateDownload( poNewInfo );
			}
			else 
			{
				ActivityMessageBox errMsgBox( m_MyApp, this, LOG_INFO, GuiParams::describeEXferError( xferError ) );
				errMsgBox.exec();
			}
		}
	}
}

//============================================================================
void AppletPeerViewSharedFiles::clearFileList( void )
{
	ui.FileItemList->clear();
}

//============================================================================
void AppletPeerViewSharedFiles::moveUpOneFolder( void )
{
	if( m_strCurrentDirectory.length() )
	{
		char * pBuf = new char[ m_strCurrentDirectory.length() + 1 ];
		strcpy( pBuf, m_strCurrentDirectory.c_str() );
		char * pTemp = strrchr( pBuf, '/' );
		if( pTemp )
		{
			pTemp[0] = 0;
		}
		m_strCurrentDirectory = pBuf;
		delete pBuf;
	}
}

