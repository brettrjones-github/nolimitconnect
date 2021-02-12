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
: AppletClientBase( OBJNAME_APPLET_CHAT_ROOM_JOIN_SEARCH, app, parent )
{
    setAppletType( eAppletChatRoomJoinSearch );
    setHostType( eHostTypeChatRoom );
	ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );

    connectBarWidgets();

	connect( this,					SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );

	connect( ui.m_SearchsParamWidget,	SIGNAL(signalSearchState(bool)),		this, SLOT(slotStartSearchState(bool)) );
    connect( this,					SIGNAL(signalSearchComplete()),				this, SLOT(slotSearchComplete()) );
    connect( this,					SIGNAL(signalSearchResult(VxNetIdent*)),	this, SLOT(slotSearchResult(VxNetIdent*)) ); 

    setStatusLabel( QObject::tr( "Search For Chat Room To Join" ) );
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

	//if( eScanType == getScanType() )
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

	//if( eScanType == getScanType() )
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
void AppletChatRoomJoinSearch::slotStartSearchState(bool startSearch)
{
    if( startSearch && !m_SearchStarted )
    {
        m_SearchStarted = true;
        ui.m_FriendListWidget->clear();
        QString strSearch = getSearchText();
        if( 3 > strSearch.length() )
        {
            ui.m_SearchsParamWidget->slotSearchCancel();
            ActivityMessageBox errMsgBox( m_MyApp, this, LOG_ERROR, QObject::tr( "Search must have at least 3 characters" ) );
            errMsgBox.exec();
        }
        else
        {
            ui.m_SearchsParamWidget->toSearchParams( m_SearchParams );
            m_SearchParams.setSearchType(eSearchChatRoomHost);
            setStatusLabel( QObject::tr( "Search Started" ) );
            m_FromGui.fromGuiSearchHost( getHostType(), m_SearchParams, true );
        }

    }
    else if( !startSearch && m_SearchStarted )
    {
        m_SearchStarted = false;
        m_FromGui.fromGuiSearchHost( getHostType(), m_SearchParams, false );
        setStatusLabel( QObject::tr( "Search Stopped" ) );
    }
}

//============================================================================
void AppletChatRoomJoinSearch::slotSearchComplete( void )
{
    ui.m_SearchsParamWidget->slotSearchComplete();
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

