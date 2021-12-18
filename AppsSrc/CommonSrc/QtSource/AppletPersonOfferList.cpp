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


#include "AppletPersonOfferList.h"
#include "ActivityYesNoMsgBox.h"

#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"	
#include "GuiParams.h"

#include "FileXferWidget.h"
#include "GuiFileXferSession.h"
#include "MyIcons.h"
#include "ActivityDownloadItemMenu.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
AppletPersonOfferList::AppletPersonOfferList( AppCommon& app,  QWidget* parent )
: AppletPeerBase( OBJNAME_APPLET_PERSONS_OFFER_LIST, app, parent )
{
    setAppletType( eAppletPersonOfferList );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    //connectBarWidgets();

    /*
    connect( ui.m_FileItemList, SIGNAL(itemClicked(QListWidgetItem *)),		                this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));
    connect( ui.m_FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),	            this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));

	connect( this, SIGNAL(signalToGuiStartDownload(GuiFileXferSession *)),					this, SLOT(slotToGuiStartDownload(GuiFileXferSession *)) );
	connect( this, SIGNAL(signalToGuiFileXferState(VxGUID,EXferState,int,int)),			    this, SLOT(slotToGuiFileXferState(VxGUID,EXferState,int,int)) );
	connect( this, SIGNAL(signalToGuiFileDownloadComplete(VxGUID,QString,EXferError)),	    this, SLOT(slotToGuiFileDownloadComplete(VxGUID,QString,EXferError)) );

	m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
	checkDiskSpace();
    */
    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletPersonOfferList::~AppletPersonOfferList()
{
	m_MyApp.wantToGuiFileXferCallbacks( this, this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletPersonOfferList::showEvent( QShowEvent * ev )
{
    AppletPeerBase::showEvent( ev );
}

//============================================================================
void AppletPersonOfferList::hideEvent( QHideEvent * ev )
{
    AppletPeerBase::hideEvent( ev );
}

/*
//============================================================================
void AppletPersonOfferList::slotHomeButtonClicked( void )
{
	hide();
}

// override default behavior of closing dialog when back button is clicked
//============================================================================
void AppletPersonOfferList::onBackButtonClicked( void )
{
    hide();
}
*/

////============================================================================
//void AppletPersonOfferList::slotItemClicked(QListWidgetItem * item)
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
/*
//============================================================================
FileXferWidget * AppletPersonOfferList::sessionToWidget( GuiFileXferSession * poSession )
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

    return nullptr;
}
*/

/*
//============================================================================
void AppletPersonOfferList::updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * poSession )
{
	poSession->setWidget( item );
	item->updateWidgetFromInfo();
}

//============================================================================
GuiFileXferSession * AppletPersonOfferList::widgetToSession( FileXferWidget * item )
{
	//return item->getFileItemInfo();
}

//============================================================================
GuiFileXferSession * AppletPersonOfferList::findSession( VxGUID lclSessionId )
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
FileXferWidget * AppletPersonOfferList::findListEntryWidget( VxGUID lclSessionId )
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
FileXferWidget * AppletPersonOfferList::addDownload( GuiFileXferSession * poSession )
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
bool AppletPersonOfferList::isXferInProgress( VxGUID lclSessionId )
{
	if( findSession( lclSessionId ) )
	{
		return true;
	}

	return false;
}

//============================================================================
void AppletPersonOfferList::slotToGuiStartDownload( GuiFileXferSession * poSession )
{
	GuiFileXferSession * newSession = new GuiFileXferSession( *poSession );
	newSession->setXferDirection( eXferDirectionRx );

	FileXferWidget * item = addDownload( newSession );
	item->setXferState( eXferStateInDownloadXfer, 0, 0 );
}

//============================================================================
void AppletPersonOfferList::slotToGuiFileXferState(	VxGUID		lclSessionId, 
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
void AppletPersonOfferList::slotToGuiFileDownloadComplete(	VxGUID lclSessionId, QString newFileName, EXferError xferError )
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
void AppletPersonOfferList::toGuiStartDownload( void * userData, GuiFileXferSession * xferSession )
{
	Q_UNUSED( userData );
	emit signalToGuiStartDownload( xferSession );
}

//============================================================================
void AppletPersonOfferList::toGuiFileXferState( void * userData, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 )
{
	Q_UNUSED( userData );
	VxGUID myLclSession( lclSessionId );
	emit signalToGuiFileXferState( myLclSession, eXferState, param1, param2 );
}

//============================================================================
void AppletPersonOfferList::toGuiFileDownloadComplete( void * userData, VxGUID& lclSession, QString newFileName, EXferError xferError )
{
	Q_UNUSED( userData );
	VxGUID myLclSession( lclSession );
	emit signalToGuiFileDownloadComplete( myLclSession, newFileName, xferError );
}

//============================================================================
void AppletPersonOfferList::slotFileXferItemClicked(QListWidgetItem * item)
{
}

//============================================================================
void AppletPersonOfferList::slotFileIconButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{

	}	
}

//============================================================================
void AppletPersonOfferList::slotCancelButtonClicked( QListWidgetItem * item )
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
void AppletPersonOfferList::slotPlayButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		this->playFile( xferSession->getFullFileName() );
	}
}

//============================================================================
void AppletPersonOfferList::slotLibraryButtonClicked( QListWidgetItem * item )
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
void AppletPersonOfferList::slotFileShareButtonClicked( QListWidgetItem * item )
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
void AppletPersonOfferList::slotShredButtonClicked( QListWidgetItem * item )
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
bool AppletPersonOfferList::confirmDeleteFile( bool shredFile )
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
*/