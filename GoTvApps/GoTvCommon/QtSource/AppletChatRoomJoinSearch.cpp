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
#include "PopupMenu.h"
#include "AppletChatRoomJoinSearch.h"
#include "ActivityMessageBox.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

#include <QString>

//============================================================================
AppletChatRoomJoinSearch::AppletChatRoomJoinSearch(	AppCommon&		    app, 
													QWidget *			parent )
: AppletBase( OBJNAME_APPLET_CHAT_ROOM_JOIN_SEARCH, app, parent, true )
, m_eScanType( eScanTypeChatRoomJoinSearch )
{
    setAppletType( eAppletChatRoomJoinSearch );
	ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    connectBarWidgets();

	connect( this,					SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );

	connect( ui.StartSearchButton,	SIGNAL(clicked()),							this, SLOT(slotStartSearchClicked()) );
	connect( ui.StopSearchButton,	SIGNAL(clicked()),							this, SLOT(slotStopSearchClicked()) );
    connect( this,					SIGNAL(signalSearchComplete()),				this, SLOT(slotSearchComplete()) );
    connect( this,					SIGNAL(signalSearchResult(VxNetIdent*)),	this, SLOT(slotSearchResult(VxNetIdent*)) ); 

	if( eScanTypePeopleSearch == getScanType() )
	{
        setStatusLabel( QObject::tr( "Search For Chat Room To Join" ) );
		ui.SearchLabel->setText( QObject::tr( "Search for name (at least 3 characters)" ) );
	}
}

//============================================================================
void AppletChatRoomJoinSearch::setStatusLabel( QString strMsg )
{
	ui.ScanStatusLabel->setText( strMsg );
}

//============================================================================
void AppletChatRoomJoinSearch::toGuiSearchResultSuccess( void * callbackData, EScanType eScanType, VxNetIdent * netIdent )
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
void AppletChatRoomJoinSearch::toGuiClientScanSearchComplete( void * callbackData, EScanType eScanType )
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
void AppletChatRoomJoinSearch::showEvent( QShowEvent * ev )
{
	ActivityBase::showEvent( ev );
	m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void AppletChatRoomJoinSearch::hideEvent( QHideEvent * ev )
{
	m_MyApp.wantToGuiActivityCallbacks( this, this, false );
	ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletChatRoomJoinSearch::slotHomeButtonClicked( void )
{
}

//============================================================================
void AppletChatRoomJoinSearch::slotStartSearchClicked()
{
	ui.m_FriendListWidget->clear();
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
void AppletChatRoomJoinSearch::slotStopSearchClicked()
{
	m_FromGui.fromGuiStopScan( m_eScanType );
	setStatusLabel( QObject::tr( "Search Stopped" ) );
}

//============================================================================
void AppletChatRoomJoinSearch::slotSearchComplete()
{
	setStatusLabel( QObject::tr( "Search Complete" ) );
}

////============================================================================
//void AppletChatRoomJoinSearch::slotFriendClicked( VxNetIdent * netIdent )
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
void AppletChatRoomJoinSearch::slotSearchResult( VxNetIdent * netIdent )
{
	setStatusLabel( QString("Found Match %1").arg( netIdent->getOnlineName() ) );
	ui.m_FriendListWidget->updateFriend( netIdent, false );
}

