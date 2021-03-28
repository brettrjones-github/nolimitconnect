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
#include "AppletDownloads.h"
#include "ActivityYesNoMsgBox.h"

#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"	
#include "GuiParams.h"

#include "FileXferWidget.h"
#include "GuiFileXferSession.h"
#include "MyIcons.h"
#include "ActivityDownloadItemMenu.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
AppletDownloads::AppletDownloads( AppCommon& app,  QWidget* parent )
: AppletPeerBase( OBJNAME_ACTIVITY_DOWNLOADS, app, parent ) 
{
    setAppletType( eAppletDownloads );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    //connectBarWidgets();

    connect( ui.m_FileItemList, SIGNAL(itemClicked(QListWidgetItem *)),		                this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));
    connect( ui.m_FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),	            this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));

	connect( this, SIGNAL(signalToGuiStartDownload(GuiFileXferSession *)),					this, SLOT(slotToGuiStartDownload(GuiFileXferSession *)) );
	connect( this, SIGNAL(signalToGuiFileXferState(VxGUID,EXferState,int,int)),			this, SLOT(slotToGuiFileXferState(VxGUID,EXferState,int,int)) );
	connect( this, SIGNAL(signalToGuiFileDownloadComplete(VxGUID,QString,EXferError)),	this, SLOT(slotToGuiFileDownloadComplete(VxGUID,QString,EXferError)) );
	m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
	checkDiskSpace();
    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletDownloads::~AppletDownloads()
{
	m_MyApp.wantToGuiFileXferCallbacks( this, this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletDownloads::showEvent( QShowEvent * ev )
{
    AppletPeerBase::showEvent( ev );
}

//============================================================================
void AppletDownloads::hideEvent( QHideEvent * ev )
{
    AppletPeerBase::hideEvent( ev );
}

//============================================================================
void AppletDownloads::slotHomeButtonClicked( void )
{
	hide();
}

// override default behavior of closing dialog when back button is clicked
//============================================================================
void AppletDownloads::onBackButtonClicked( void )
{
    hide();
}

////============================================================================
//void AppletDownloads::slotItemClicked(QListWidgetItem * item)
//{
//	GuiFileXferSession * poSession = (GuiFileXferSession *)item->data( Qt::UserRole + 1).toULongLong();
//	if( poSession )
//	{
//		ActivityDownloadItemMenu oDlg( m_MyApp, poSession, this );
//		if( QDialog::Rejected == oDlg.exec() )
//		{
//			m_FromGui.fromGuiCancelDownload( poSession->getLclSessionId() );
//			ui.m_FileItemList->removeItemWidget( item );
//			delete poSession;
//		}
//	}
//}

//============================================================================
FileXferWidget * AppletDownloads::sessionToWidget( GuiFileXferSession * poSession )
{
	FileXferWidget * item = new FileXferWidget(ui.m_FileItemList);
    item->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    item->setFileItemInfo( poSession );

	connect( item, SIGNAL(signalFileXferItemClicked(QListWidgetItem *)),	this, SLOT(slotFileXferItemClicked(QListWidgetItem *)) );
	connect( item, SIGNAL(signalFileIconButtonClicked(QListWidgetItem *)),	this, SLOT(slotFileIconButtonClicked(QListWidgetItem *)) );
	connect( item, SIGNAL(signalCancelButtonClicked(QListWidgetItem *)),	this, SLOT(slotCancelButtonClicked(QListWidgetItem *)) );
	connect( item, SIGNAL(signalPlayButtonClicked(QListWidgetItem *)),		this, SLOT(slotPlayButtonClicked(QListWidgetItem *)) );
	connect( item, SIGNAL(signalLibraryButtonClicked(QListWidgetItem *)),	this, SLOT(slotLibraryButtonClicked(QListWidgetItem *)) );
	connect( item, SIGNAL(signalFileShareButtonClicked(QListWidgetItem *)),	this, SLOT(slotFileShareButtonClicked(QListWidgetItem *)) );
	connect( item, SIGNAL(signalShredButtonClicked(QListWidgetItem *)),		this, SLOT(slotShredButtonClicked(QListWidgetItem *)) );

	updateListEntryWidget( item, poSession );

	return item;
}

//============================================================================
void AppletDownloads::updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * poSession )
{
	poSession->setWidget( item );
	item->updateWidgetFromInfo();
}

//============================================================================
GuiFileXferSession * AppletDownloads::widgetToSession( FileXferWidget * item )
{
	return item->getFileItemInfo();
}

//============================================================================
GuiFileXferSession * AppletDownloads::findSession( VxGUID lclSessionId )
{
	int iCnt = ui.m_FileItemList->count();
	for( int iRow = 0; iRow < iCnt; iRow++ )
	{
		QListWidgetItem * item =  ui.m_FileItemList->item( iRow );
		GuiFileXferSession * poCurInfo = (GuiFileXferSession *)item->data( Qt::UserRole + 1).toULongLong();
		if( poCurInfo->getLclSessionId() == lclSessionId )
		{
			return poCurInfo;
		}
	}

	return nullptr;
}

//============================================================================
FileXferWidget * AppletDownloads::findListEntryWidget( VxGUID lclSessionId )
{
	int iCnt = ui.m_FileItemList->count();
	for( int iRow = 0; iRow < iCnt; iRow++ )
	{
		QListWidgetItem * item =  ui.m_FileItemList->item( iRow );
		if( item )
		{
			GuiFileXferSession * poCurInfo = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
			if( poCurInfo && ( poCurInfo->getLclSessionId() == lclSessionId ) )
			{
				return (FileXferWidget *)item;
			}
		}
	}

	return nullptr;
}

//============================================================================
FileXferWidget * AppletDownloads::addDownload( GuiFileXferSession * poSession )
{
	FileXferWidget * item = findListEntryWidget( poSession->getLclSessionId() );
	if( item )
	{
		GuiFileXferSession * poCurInfo = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
		delete poCurInfo;
        item->QListWidgetItem::setData( Qt::UserRole + 1, QVariant( (qint64)poSession ) );
		updateListEntryWidget( item, poSession );
	}
	else
	{
		item = sessionToWidget( poSession );
        item->QListWidgetItem::setData( Qt::UserRole + 1, QVariant( (qint64)poSession ) );

		ui.m_FileItemList->addItem( item );
		ui.m_FileItemList->setItemWidget( item, item );
	}

	return item;
}

//============================================================================
bool AppletDownloads::isXferInProgress( VxGUID lclSessionId )
{
	if( findSession( lclSessionId ) )
	{
		return true;
	}

	return false;
}

//============================================================================
void AppletDownloads::slotToGuiStartDownload( GuiFileXferSession * poSession )
{
	GuiFileXferSession * newSession = new GuiFileXferSession( *poSession );
	newSession->setXferDirection( eXferDirectionRx );

	FileXferWidget * item = addDownload( newSession );
	item->setXferState( eXferStateInDownloadXfer, 0, 0 );
}

//============================================================================
void AppletDownloads::slotToGuiFileXferState(	VxGUID		lclSessionId, 
												EXferState		eXferState, 
												int				param1, 
												int				param2  )
{
	FileXferWidget * item = findListEntryWidget( lclSessionId );
	if( item )
	{
		item->setXferState( eXferState, param1, param2 );
	}
}

//============================================================================
void AppletDownloads::slotToGuiFileDownloadComplete(	VxGUID lclSessionId, QString newFileName, EXferError xferError )
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
void AppletDownloads::toGuiStartDownload( void * userData, GuiFileXferSession * xferSession )
{
	Q_UNUSED( userData );
	emit signalToGuiStartDownload( xferSession );
}

//============================================================================
void AppletDownloads::toGuiFileXferState( void * userData, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 )
{
	Q_UNUSED( userData );
	VxGUID myLclSession( lclSessionId );
	emit signalToGuiFileXferState( myLclSession, eXferState, param1, param2 );
}

//============================================================================
void AppletDownloads::toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError )
{
	Q_UNUSED( userData );
	VxGUID myLclSession( lclSession );
	emit signalToGuiFileDownloadComplete( myLclSession, newFileName, xferError );
}

//============================================================================
void AppletDownloads::slotFileXferItemClicked(QListWidgetItem * item)
{
}

//============================================================================
void AppletDownloads::slotFileIconButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{

	}	
}

//============================================================================
void AppletDownloads::slotCancelButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		if( eXferStateCompletedDownload != xferSession->getXferState() )
		{
			xferSession->setXferState( eXferStateUserCanceledDownload, 0, 0 );
			((FileXferWidget *)item)->setXferState( eXferStateUserCanceledDownload, 0, 0 );
			m_Engine.fromGuiCancelDownload( xferSession->getLclSessionId() );
		}
	}

	ui.m_FileItemList->removeItemWidget( item );
}

//============================================================================
void AppletDownloads::slotPlayButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		this->playFile( xferSession->getFullFileName() );
	}
}

//============================================================================
void AppletDownloads::slotLibraryButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
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
void AppletDownloads::slotFileShareButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
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
void AppletDownloads::slotShredButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		QString fileName = xferSession->getFullFileName();
		if( confirmDeleteFile( true ) )
		{
			ui.m_FileItemList->removeItemWidget( item );
			m_Engine.fromGuiDeleteFile( fileName.toUtf8().constData(), true );
		}
	}	
}

//============================================================================
bool AppletDownloads::confirmDeleteFile( bool shredFile )
{
	bool acceptAction = true;
	bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
	if( false == isConfirmDisabled )
	{
		QString title = shredFile ?  "Confirm Shred File" : "Confirm Delete File";
		QString bodyText = "";
		if( shredFile )
		{
			bodyText = "Are You Sure You Want To Write Random Data Into The File Then Delete From The Device?";
		}
		else
		{
			bodyText = "Are You Sure To Delete The File From The Device?";
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
