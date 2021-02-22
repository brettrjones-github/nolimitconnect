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
#include "GuiHelpers.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

#include <QString>

namespace
{
    const int MAX_LOG_EDIT_BLOCK_CNT = 1000;
    const int MAX_INFO_MSG_SIZE = 2048;
}

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

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( &m_MyApp, SIGNAL(signalHostAnnounceStatus( EHostType, VxGUID, EHostAnnounceStatus, QString )),
        this, SLOT(slotHostAnnounceStatus( EHostType, VxGUID, EHostAnnounceStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )),
        this, SLOT(slotHostJoinStatus( EHostType, VxGUID&, EHostJoinStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )),
        this, SLOT(slotHostSearchStatus( EHostType, VxGUID&, EHostSearchStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchResult( EHostType, VxGUID, VxNetIdent &, PluginSetting & )),
        this, SLOT(slotHostSearchResult( EHostType, VxGUID&, VxNetIdent &, PluginSetting & )) );

    setStatusLabel( QObject::tr( "Search For Chat Room To Join" ) );
    std::string lastHostSearchText;
    m_MyApp.getAppSettings().getLastHostSearchText( lastHostSearchText ); 
    if( !lastHostSearchText.empty() )
    {
        ui.m_SearchsParamWidget->getSearchTextEdit()->setText( lastHostSearchText.c_str() );
    }
}

//============================================================================
void AppletChatRoomJoinSearch::setStatusLabel( QString strMsg )
{
	ui.m_StatusLabel->setText( strMsg );
}

//============================================================================
void AppletChatRoomJoinSearch::setInfoLabel( QString strMsg )
{
    ui.m_InfoLabel->setText( strMsg );
}

//============================================================================
void AppletChatRoomJoinSearch::toGuiScanResultSuccess( void * callbackData, EScanType eScanType, VxNetIdent * netIdent )
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
            if( ui.m_SearchsParamWidget->toSearchParams( m_SearchParams ) )
            {
                m_SearchParams.setHostType(getHostType());
                m_SearchParams.setSearchType(eSearchChatRoomHost);
                m_SearchParams.createNewSessionId();
                m_SearchParams.updateSearchStartTime();
                setStatusLabel( QObject::tr( "Search Started" ) );
                m_FromGui.fromGuiSearchHost( getHostType(), m_SearchParams, true );
                if( !m_SearchParams.getSearchText().empty() )
                {
                    m_MyApp.getAppSettings().setLastHostSearchText( m_SearchParams.getSearchText() );
                }
            }
            else
            {
                setStatusLabel( QObject::tr( "Search Params Invalid" ) );
            }
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

//============================================================================
void AppletChatRoomJoinSearch::slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString text )
{
   // getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    //getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    setInfoLabel( GuiHelpers::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletChatRoomJoinSearch::slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString text )
{
    //getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    //getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    setInfoLabel( GuiHelpers::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletChatRoomJoinSearch::slotHostSearchStatus( EHostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg )
{
    //getInfoEdit()->appendPlainText( strMsg ); // Adds the message to the widget
    //getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
    setInfoLabel( GuiHelpers::describeStatus(hostStatus) + strMsg);
}

//============================================================================
void AppletChatRoomJoinSearch::slotHostSearchResult( EHostType hostType, VxGUID sessionId, VxNetIdent &hostIdent, PluginSetting &pluginSetting )
{
    LogMsg( LOG_DEBUG, "slotHostSearchResult host %s ident %s plugin %s", DescribeHostType( hostType ), hostIdent.getOnlineName(), 
        DescribePluginType2( pluginSetting.getPluginType() ) );
}

//============================================================================
void AppletChatRoomJoinSearch::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );
    infoStr.remove( QRegExp( "[\\n\\r]" ) );
    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletChatRoomJoinSearch::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}
