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
#include "ActivityDownloads.h"
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
ActivityDownloads::ActivityDownloads(	AppCommon&	app, 
										QWidget *		parent )
: ActivityBase( OBJNAME_ACTIVITY_DOWNLOADS, app, parent, eAppletMessenger, true ) 
{
	ui.setupUi( this );
	connect( &m_MyApp,				SIGNAL(signalStatusMsg(QString)),		ui.m_TitleBarWidget,	SLOT(slotTitleStatusBarMsg(QString)) );

	ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "Downloads" ) );
    connectBarWidgets();

	connect( ui.m_TitleBarWidget,					SIGNAL(signalBackButtonClicked()),				this, SLOT(slotHomeButtonClicked()) );
    connect( ui.m_FileItemList, SIGNAL(itemClicked(QListWidgetItem *)),		this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));
    connect( ui.m_FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),	this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));

	connect( this, SIGNAL(signalToGuiStartDownload(GuiFileXferSession *)),					this, SLOT(slotToGuiStartDownload(GuiFileXferSession *)) );
	connect( this, SIGNAL(signalToGuiFileXferState(VxGuidQt,EXferState,int,int)),			this, SLOT(slotToGuiFileXferState(VxGuidQt,EXferState,int,int)) );
	connect( this, SIGNAL(signalToGuiFileDownloadComplete(VxGuidQt,QString,EXferError)),	this, SLOT(slotToGuiFileDownloadComplete(VxGuidQt,QString,EXferError)) );
	m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
	checkDiskSpace();
}

//============================================================================
ActivityDownloads::~ActivityDownloads()
{
	m_MyApp.wantToGuiFileXferCallbacks( this, this, false );
}

//============================================================================
void ActivityDownloads::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
}

//============================================================================
void ActivityDownloads::hideEvent( QHideEvent * ev )
{
	ActivityBase::hideEvent( ev );
}

//============================================================================
void ActivityDownloads::slotHomeButtonClicked( void )
{
	hide();
}

////============================================================================
//void ActivityDownloads::slotItemClicked(QListWidgetItem * item)
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
FileXferWidget * ActivityDownloads::sessionToWidget( GuiFileXferSession * poSession )
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
void ActivityDownloads::updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * poSession )
{
	poSession->setWidget( item );
	item->updateWidgetFromInfo();
}

//============================================================================
GuiFileXferSession * ActivityDownloads::widgetToSession( FileXferWidget * item )
{
	return item->getFileItemInfo();
}

//============================================================================
GuiFileXferSession * ActivityDownloads::findSession( VxGuidQt lclSessionId )
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
FileXferWidget * ActivityDownloads::findListEntryWidget( VxGuidQt lclSessionId )
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
FileXferWidget * ActivityDownloads::addDownload( GuiFileXferSession * poSession )
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
bool ActivityDownloads::isXferInProgress( VxGuidQt lclSessionId )
{
	if( findSession( lclSessionId ) )
	{
		return true;
	}

	return false;
}

//============================================================================
void ActivityDownloads::slotToGuiStartDownload( GuiFileXferSession * poSession )
{
	GuiFileXferSession * newSession = new GuiFileXferSession( *poSession );
	newSession->setXferDirection( eXferDirectionRx );

	FileXferWidget * item = addDownload( newSession );
	item->setXferState( eXferStateInDownloadXfer, 0, 0 );
}

//============================================================================
void ActivityDownloads::slotToGuiFileXferState(	VxGuidQt		lclSessionId, 
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
void ActivityDownloads::slotToGuiFileDownloadComplete(	VxGuidQt lclSessionId, QString newFileName, EXferError xferError )
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
void ActivityDownloads::toGuiStartDownload( void * userData, GuiFileXferSession * xferSession )
{
	Q_UNUSED( userData );
	emit signalToGuiStartDownload( xferSession );
}

//============================================================================
void ActivityDownloads::toGuiFileXferState( void * userData, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 )
{
	Q_UNUSED( userData );
	VxGuidQt myLclSession( lclSessionId );
	emit signalToGuiFileXferState( myLclSession, eXferState, param1, param2 );
}

//============================================================================
void ActivityDownloads::toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError )
{
	Q_UNUSED( userData );
	VxGuidQt myLclSession( lclSession );
	emit signalToGuiFileDownloadComplete( myLclSession, newFileName, xferError );
}

//============================================================================
void ActivityDownloads::slotFileXferItemClicked(QListWidgetItem * item)
{
}

//============================================================================
void ActivityDownloads::slotFileIconButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{

	}	
}

//============================================================================
void ActivityDownloads::slotCancelButtonClicked( QListWidgetItem * item )
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
void ActivityDownloads::slotPlayButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		this->playFile( xferSession->getFullFileName() );
	}
}

//============================================================================
void ActivityDownloads::slotLibraryButtonClicked( QListWidgetItem * item )
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
void ActivityDownloads::slotFileShareButtonClicked( QListWidgetItem * item )
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
void ActivityDownloads::slotShredButtonClicked( QListWidgetItem * item )
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
bool ActivityDownloads::confirmDeleteFile( bool shredFile )
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
