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
#include "AppGlobals.h"
#include "ActivityScanPeopleSearch.h"
#include "ActivityMessageBox.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

#include <QString>

//============================================================================
ActivityScanPeopleSearch::ActivityScanPeopleSearch(	AppCommon&		    app, 
													EScanType			eSearchType,
													QWidget *			parent )
: ActivityBase( OBJNAME_ACTIVITY_PEOPLE_SEARCH, app, parent, eAppletSearchPersons, true )
, m_eScanType( eSearchType )
{
	ui.setupUi(this);

    connectBarWidgets();

    connect( ui.m_TitleBarWidget,	SIGNAL(signalBackButtonClicked()),			this, SLOT(close()) );
	connect( this,					SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );

	connect( ui.StartSearchButton,	SIGNAL(clicked()),							this, SLOT(slotStartSearchClicked()) );
	connect( ui.StopSearchButton,	SIGNAL(clicked()),							this, SLOT(slotStopSearchClicked()) );
    connect( this,					SIGNAL(signalSearchComplete()),				this, SLOT(slotSearchComplete()) );
    connect( this,					SIGNAL(signalSearchResult(VxNetIdent*)),	this, SLOT(slotSearchResult(VxNetIdent*)) ); 

	if( eScanTypePeopleSearch == getScanType() )
	{
		setTitle( QObject::tr( "Search for people by name" ) );
		ui.SearchLabel->setText( QObject::tr( "Search for name (at least 3 characters)" ) );
	}
	else
	{
		setTitle( QObject::tr( "Search for people by mood message" ) );
		ui.SearchLabel->setText( QObject::tr( "Search for mood message (at least 3 characters)" ) );
	}
}

//============================================================================
void ActivityScanPeopleSearch::setTitle( QString strTitle )
{
	ui.m_TitleBarWidget->setTitleBarText( strTitle );
}

//============================================================================
void ActivityScanPeopleSearch::setStatusLabel( QString strMsg )
{
	ui.ScanStatusLabel->setText( strMsg );
}

//============================================================================
void ActivityScanPeopleSearch::toGuiScanResultSuccess( void * callbackData, EScanType eScanType, GuiUser * netIdent )
{
	Q_UNUSED( callbackData );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( eScanType == getScanType() )
	{
		emit signalSearchResult( netIdent );
	}
};

//============================================================================
void ActivityScanPeopleSearch::toGuiClientScanSearchComplete( void * callbackData, EScanType eScanType )
{
	Q_UNUSED( callbackData );
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	if( eScanType == getScanType() )
	{
		emit signalSearchComplete();
	}
}

//============================================================================
void ActivityScanPeopleSearch::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void ActivityScanPeopleSearch::hideEvent( QHideEvent * ev )
{
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
void ActivityScanPeopleSearch::slotHomeButtonClicked( void )
{
}

//============================================================================
void ActivityScanPeopleSearch::slotStartSearchClicked()
{
	ui.friendListWidget->clear();
	QString strSearch = ui.searchEdit->text();
	if( 3 > strSearch.length() )
	{
		ActivityMessageBox errMsgBox( m_MyApp, this, LOG_ERROR, QObject::tr( "Search must have at least 3 characters" ) );
		errMsgBox.exec();
	}
	else
	{
		setStatusLabel( QObject::tr( "Search Started" ) );
		m_FromGui.fromGuiStartScan( m_eScanType, 0, 0, strSearch.toStdString().c_str() );
	}
}

//============================================================================
void ActivityScanPeopleSearch::slotStopSearchClicked()
{
	m_FromGui.fromGuiStopScan( m_eScanType );
	setStatusLabel( QObject::tr( "Search Stopped" ) );
}

//============================================================================
void ActivityScanPeopleSearch::slotSearchComplete()
{
	setStatusLabel( QObject::tr( "Search Complete" ) );
}

////============================================================================
//void ActivityScanPeopleSearch::slotFriendClicked( VxNetIdent * netIdent )
//{
//	PopupMenu oPopupMenu( (QWidget *)this->parent() );
//	if( false == connect( &oPopupMenu, SIGNAL(menuItemClicked(int, PopupMenu *, ActivityBase *)), &oPopupMenu, SLOT(onFriendActionSelected(int)) ) )
//	{
//		LogMsg( LOG_ERROR, "FriendListWidget::findListEntryWidget failed connect\n" );
//	}
//
//	oPopupMenu.showFriendMenu( netIdent );
//}

//============================================================================
void ActivityScanPeopleSearch::slotSearchResult( GuiUser * netIdent )
{
	setStatusLabel( QString("Found Match %1").arg( netIdent->getOnlineName() ) );
	ui.friendListWidget->updateFriend( netIdent, false );
}

