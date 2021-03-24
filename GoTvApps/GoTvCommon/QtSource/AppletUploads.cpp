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

#include "AppletUploads.h"
#include "ActivityYesNoMsgBox.h"

#include "FileXferWidget.h"
#include "GuiFileXferSession.h"
#include "MyIcons.h"
#include "GuiHelpers.h"	
#include "GuiParams.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxFileInfo.h>

//============================================================================
AppletUploads::AppletUploads( AppCommon& app, QWidget *	parent )
: AppletPeerBase( OBJNAME_ACTIVITY_UPLOADS, app, parent )
{
    setAppletType( eAppletPeerViewSharedFiles );
    ui.setupUi( parent );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

	connect( &m_MyApp,				SIGNAL(signalStatusMsg(QString)),		ui.m_TitleBarWidget,	SLOT(slotTitleStatusBarMsg(QString)) );
	connect( ui.m_TitleBarWidget,	SIGNAL(signalBackButtonClicked()),		this,	                SLOT(slotHomeButtonClicked()) );

    connectBarWidgets();

    connect(ui.m_FileItemList,	SIGNAL(itemClicked(QListWidgetItem *)),         this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));
    connect(ui.m_FileItemList, SIGNAL(itemDoubleClicked(QListWidgetItem *)),    this, SLOT(slotFileXferItemClicked(QListWidgetItem *)));

	connect( this, SIGNAL(signalToGuiStartUpload(GuiFileXferSession *)),			this, SLOT(slotToGuiStartUpload(GuiFileXferSession *)) );
	connect( this, SIGNAL(signalToGuiFileXferState(VxGuidQt,EXferState,int,int)),	this, SLOT(slotToGuiFileXferState(VxGuidQt,EXferState,int,int)) );
	connect( this, SIGNAL(signalToGuiFileUploadComplete(VxGuidQt,int)),				this, SLOT(slotToGuiFileUploadComplete(VxGuidQt,int)) );
	m_MyApp.wantToGuiFileXferCallbacks( this, this, true );
	checkDiskSpace();
}

//============================================================================
AppletUploads::~AppletUploads()
{
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );
}

//============================================================================
void AppletUploads::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
}

//============================================================================
void AppletUploads::hideEvent( QHideEvent * ev )
{
	ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletUploads::slotHomeButtonClicked( void )
{
	hide();
}

//============================================================================
FileXferWidget * AppletUploads::sessionToWidget( GuiFileXferSession * poSession )
{
	FileXferWidget * item = new FileXferWidget(ui.m_FileItemList);
    item->setSizeHint( QSize( ( int )( GuiParams::getGuiScale() * 200 ),
        ( int )( 62 * GuiParams::getGuiScale() ) ) );

    item->QListWidgetItem::setData( Qt::UserRole + 1, QVariant((quint64)poSession) );

    connect( item, SIGNAL(signalFileXferItemClicked(QListWidgetItem*)), this, SLOT(slotFileXferItemClicked(QListWidgetItem*)));
    connect( item, SIGNAL(signalCancelButtonClicked(QListWidgetItem*)), this, SLOT(slotCancelButtonClicked(QListWidgetItem*)));

	updateListEntryWidget( item, poSession );
	return item;
}

//============================================================================
void AppletUploads::updateListEntryWidget( FileXferWidget * item, GuiFileXferSession * poSession )
{
	poSession->setWidget( item );
	item->updateWidgetFromInfo();
}

//============================================================================
GuiFileXferSession * AppletUploads::widgetToSession( FileXferWidget * item )
{
	return (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1 ).toULongLong();
}

//============================================================================
GuiFileXferSession * AppletUploads::findSession( VxGuidQt lclSessionId )
{
	int iCnt = ui.m_FileItemList->count();
	for( int iRow = 0; iRow < iCnt; iRow++ )
	{
		QListWidgetItem * item =  ui.m_FileItemList->item( iRow );
		GuiFileXferSession * poCurInfo = (GuiFileXferSession *)item->data( Qt::UserRole ).toULongLong();
		if( poCurInfo->getLclSessionId() == lclSessionId )
		{
			return poCurInfo;
		}
	}

	return NULL;
}

//============================================================================
FileXferWidget * AppletUploads::findListEntryWidget( VxGuidQt lclSessionId )
{
	int iCnt = ui.m_FileItemList->count();
	for( int iRow = 0; iRow < iCnt; iRow++ )
	{
		QListWidgetItem * item =  ui.m_FileItemList->item( iRow );
		GuiFileXferSession * poCurInfo = (GuiFileXferSession *)item->data( Qt::UserRole + 1 ).toULongLong();
		if( poCurInfo->getLclSessionId() == lclSessionId )
		{
			return (FileXferWidget *)item;
		}
	}

	return NULL;
}

//============================================================================
FileXferWidget * AppletUploads::addUpload( GuiFileXferSession * poSession )
{
	FileXferWidget * item = sessionToWidget( poSession );

	ui.m_FileItemList->addItem( item );
	ui.m_FileItemList->setItemWidget( item, item );
	return item;
}

//============================================================================
bool AppletUploads::isUploadInProgress( VxGuidQt fileInstance )
{
	if( findSession( fileInstance ) )
	{
		return true;
	}

	return false;
}

//============================================================================
void AppletUploads::slotToGuiStartUpload( GuiFileXferSession * poSession )
{
	GuiFileXferSession * newSession = new GuiFileXferSession( *poSession );
	newSession->setXferDirection( eXferDirectionTx );

	FileXferWidget * item = addUpload( newSession );
	item->setXferState( eXferStateInUploadXfer, 0, 0 );
}

//============================================================================
void AppletUploads::slotToGuiFileXferState(	VxGuidQt		lclSessionId, 
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
void AppletUploads::slotToGuiFileUploadComplete(	VxGuidQt		lclSessionId, 
													int				xferError )
{
	FileXferWidget * item = findListEntryWidget( lclSessionId );
	if( item )
	{
		item->setXferState( eXferStateCompletedUpload, xferError, xferError );
	}
}

//============================================================================
void AppletUploads::toGuiStartUpload( void * userData, GuiFileXferSession * xferSession )
{
	Q_UNUSED( userData );
	emit signalToGuiStartUpload( xferSession );
}


//============================================================================
void AppletUploads::toGuiFileXferState( void * userData, VxGUID& lclSessionId, EXferState eXferState, int param1, int param2 )
{
	Q_UNUSED( userData );
	VxGuidQt myLclSession( lclSessionId );
	emit signalToGuiFileXferState( myLclSession, eXferState, param1, param2 );
}

//============================================================================
void AppletUploads::toGuiFileUploadComplete( void * userData, VxGUID& lclSession, EXferError xferError )
{
	Q_UNUSED( userData );
	VxGuidQt myLclSession( lclSession );
	emit signalToGuiFileUploadComplete( myLclSession, (int)xferError );
}

//============================================================================
void AppletUploads::slotFileXferItemClicked(QListWidgetItem * item)
{
}

//============================================================================
void AppletUploads::slotFileIconButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{

	}	
}

//============================================================================
void AppletUploads::slotCancelButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		if( eXferStateCompletedUpload != xferSession->getXferState() )
		{
			xferSession->setXferState( eXferStateUserCanceledUpload, 0, 0 );
			((FileXferWidget *)item)->setXferState( eXferStateUserCanceledUpload, 0, 0 );
			m_Engine.fromGuiCancelUpload( xferSession->getLclSessionId() );
		}
	}

	ui.m_FileItemList->removeItemWidget( item );
}

//============================================================================
void AppletUploads::slotPlayButtonClicked( QListWidgetItem * item )
{
	GuiFileXferSession * xferSession = (GuiFileXferSession *)item->QListWidgetItem::data( Qt::UserRole + 1).toULongLong();
	if( xferSession )
	{
		this->playFile( xferSession->getFullFileName() );
	}
}

//============================================================================
void AppletUploads::slotLibraryButtonClicked( QListWidgetItem * item )
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
void AppletUploads::slotFileShareButtonClicked( QListWidgetItem * item )
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
void AppletUploads::slotShredButtonClicked( QListWidgetItem * item )
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
bool AppletUploads::confirmDeleteFile( bool shredFile )
{
	bool acceptAction = true;
	bool isConfirmDisabled = m_MyApp.getAppSettings().getIsConfirmDeleteDisabled();
	if( false == isConfirmDisabled )
	{
		QString title = shredFile ?  QObject::tr("Confirm Shred File") :  QObject::tr("Confirm Delete File");
		QString bodyText = "";
		if( shredFile )
		{
			bodyText =  QObject::tr("Are You Sure You Want To Write Random Data Into The File Then Delete From The Device?");
		}
		else
		{
			bodyText =  QObject::tr("Are You Sure To Delete The File From The Device?");
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
