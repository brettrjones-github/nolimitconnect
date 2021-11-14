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
#include "AppletFriendListClient.h"
#include "ActivityMessageBox.h"
#include "GuiHostSession.h"
#include "GuiParams.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

namespace
{
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletFriendListClient::AppletFriendListClient(	AppCommon&		    app, 
												QWidget *			parent )
: AppletClientBase( OBJNAME_APPLET_FRIEND_LIST_CLIENT, app, parent )
{
    setAppletType( eAppletFriendListClient );
    setHostType( eHostTypeGroup );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    // GuiHelpers::fillFriendListType( )
    setSearchType( eSearchGroupHost );

    connectBarWidgets();

    connect( this,					    SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );
    //connect( ui.m_SearchsParamWidget,	SIGNAL(signalSearchState(bool)),		    this, SLOT(slotStartSearchState(bool)) );
    connect( this,					    SIGNAL(signalSearchComplete()),				this, SLOT(slotSearchComplete()) );
    connect( this,					    SIGNAL(signalSearchResult(VxNetIdent*)),	this, SLOT(slotSearchResult(VxNetIdent*)) ); 

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( &m_MyApp, SIGNAL(signalHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )),
        this, SLOT(slotHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )),
        this, SLOT(slotHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )) );

    connect( &m_MyApp, SIGNAL(signalHostSearchResult( EHostType, VxGUID, VxNetIdent, PluginSetting  )),
        this, SLOT(slotHostSearchResult( EHostType, VxGUID, VxNetIdent, PluginSetting  )) );

    connect( ui.m_FriendListWidget,      SIGNAL( signalIconButtonClicked( GuiHostSession*, FriendListItem* ) ),  this, SLOT( slotIconButtonClicked( GuiHostSession*, FriendListItem* ) ) );
    connect( ui.m_FriendListWidget,      SIGNAL( signalMenuButtonClicked( GuiHostSession*, FriendListItem* ) ),  this, SLOT( slotMenuButtonClicked( GuiHostSession*, FriendListItem* ) ) );
    connect( ui.m_FriendListWidget,      SIGNAL( signalJoinButtonClicked( GuiHostSession*, FriendListItem* ) ),  this, SLOT( slotJoinButtonClicked( GuiHostSession*, FriendListItem* ) ) );

    // setStatusLabel( QObject::tr( "Search For Group Host To Join" ) );
    std::string lastHostSearchText;
    m_MyApp.getAppSettings().getLastHostSearchText( getSearchType(), lastHostSearchText ); 
    if( !lastHostSearchText.empty() )
    {
        //ui.m_SearchsParamWidget->getSearchTextEdit()->setText( lastHostSearchText.c_str() );
    }

    onShowFriendList();
}

//============================================================================
void AppletFriendListClient::setStatusLabel( QString strMsg )
{
    ui.m_StatusLabel->setText( strMsg );
}

//============================================================================
void AppletFriendListClient::setInfoLabel( QString strMsg )
{
    ui.m_InfoLabel->setText( strMsg );
    ui.m_InfoLabel->update();
}

//============================================================================
void AppletFriendListClient::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void AppletFriendListClient::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletFriendListClient::slotHomeButtonClicked( void )
{
}

//============================================================================
void AppletFriendListClient::slotStartSearchState(bool startSearch)
{
    //if( startSearch && !m_SearchStarted )
    //{
    //    m_SearchStarted = true;
    //    clearPluginSettingToList();
    //    clearStatus();

    //    QString strSearch = getSearchText();
    //    if( 3 > strSearch.length() )
    //    {
    //        //ui.m_SearchsParamWidget->slotSearchCancel();
    //        ActivityMessageBox errMsgBox( m_MyApp, this, LOG_ERROR, QObject::tr( "Search must have at least 3 characters" ) );
    //        errMsgBox.exec();
    //    }
    //    else
    //    {
    //        //if( ui.m_SearchsParamWidget->toSearchParams( m_SearchParams ) )
    //        {
    //            m_SearchParams.setHostType( getHostType() );
    //            m_SearchParams.setSearchType( getSearchType() );
    //            m_SearchParams.createNewSessionId();
    //            m_SearchParams.updateSearchStartTime();
    //            setStatusLabel( QObject::tr( "Search Started" ) );
    //            m_FromGui.fromGuiSearchHost( getHostType(), m_SearchParams, true );
    //            if( !m_SearchParams.getSearchText().empty() )
    //            {
    //                m_MyApp.getAppSettings().setLastHostSearchText( m_SearchParams.getSearchType(), m_SearchParams.getSearchText() );
    //                m_MyApp.getAppSettings().setLastHostSearchAgeType( m_SearchParams.getSearchType(), m_SearchParams.getAgeType() );
    //                m_MyApp.getAppSettings().setLastHostSearchGender( m_SearchParams.getSearchType(), m_SearchParams.getGender() );
    //                m_MyApp.getAppSettings().setLastHostSearchLanguage( m_SearchParams.getSearchType(), m_SearchParams.getLanguage() );
    //                m_MyApp.getAppSettings().setLastHostSearchContentRating( m_SearchParams.getSearchType(), m_SearchParams.getContentRating() );
    //            }
    //        }
    //        //else
    //        //{
    //        //    setStatusLabel( QObject::tr( "Search Params Invalid" ) );
    //        //}
    //    }
    //}
    //else if( !startSearch && m_SearchStarted )
    //{
    //    m_SearchStarted = false;
    //    m_FromGui.fromGuiSearchHost( getHostType(), m_SearchParams, false );
    //    setStatusLabel( QObject::tr( "Search Stopped" ) );
    //}
}

//============================================================================
void AppletFriendListClient::slotSearchComplete( void )
{
    //ui.m_SearchsParamWidget->slotSearchComplete();
}

//============================================================================
void AppletFriendListClient::slotInfoMsg( const QString& text )
{
    setStatusLabel( text ); // Adds the message to the widget                                                                                              //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletFriendListClient::slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString text )
{
    setInfoLabel( GuiParams::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletFriendListClient::slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString text )
{
    setInfoLabel( GuiParams::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletFriendListClient::slotHostSearchStatus( EHostType hostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg )
{
    if( hostStatus == eHostSearchCompleted )
    {
        m_SearchStarted = false;
        setStatusLabel( strMsg );
        //ui.m_SearchsParamWidget->slotSearchComplete();
    }
    else
    {
        setInfoLabel( strMsg );
    }
}

//============================================================================
void AppletFriendListClient::slotHostSearchResult( EHostType hostType, VxGUID sessionId, VxNetIdent hostIdent, PluginSetting pluginSetting )
{
    LogMsg( LOG_DEBUG, "slotHostSearchResult host %s ident %s plugin %s", DescribeHostType( hostType ), hostIdent.getOnlineName(), 
        DescribePluginType( pluginSetting.getPluginType() ) );

    QString strMsg = QObject::tr( "Match found: " );
    strMsg += hostIdent.getOnlineName();
    setInfoLabel( strMsg );
    addPluginSettingToList( hostType, sessionId, hostIdent, pluginSetting );
}

//============================================================================
void AppletFriendListClient::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    infoStr.remove(QRegularExpression("[\\n\\r]"));
#else
    infoStr.remove(QRegExp("[\\n\\r]"));
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletFriendListClient::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}

//============================================================================
void AppletFriendListClient::addPluginSettingToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    //ui.m_FriendListWidget->addHostAndSettingsToList( hostType, sessionId, hostIdent, pluginSetting );
}

//============================================================================
void AppletFriendListClient::clearPluginSettingToList( void )
{
    //ui.m_FriendListWidget->clearFriendList();
}

//============================================================================
void AppletFriendListClient::clearStatus( void )
{
    setInfoLabel( "" );
    setStatusLabel( "" );
}

////============================================================================
//void AppletFriendListClient::slotIconButtonClicked( GuiHostSession* hostSession, FriendListItem* hostItem )
//{
//
//}
//
////============================================================================
//void AppletFriendListClient::slotMenuButtonClicked( GuiHostSession* hostSession, FriendListItem* hostItem )
//{
//
//}
//
////============================================================================
//void AppletFriendListClient::slotJoinButtonClicked( GuiHostSession* hostSession, FriendListItem* hostItem )
//{
//    onJointButtonClicked( hostSession );
//}

//============================================================================
void AppletFriendListClient::onShowFriendList( void )
{
    std::vector<std::pair<VxGUID, int64_t>> friendList;
    FriendListMgr& friendMgr = m_Engine.getFriendListMgr();
    // make a copy to avoid pausing engine and to avoid threading issues
    friendMgr.lockList();
    friendList = friendMgr.getIdentList();
    friendMgr.unlockList();
    
    // for each see if we already have that ident as gui user else request it


}

//============================================================================
void AppletFriendListClient::onShowIgnoreList( void )
{
    std::vector<std::pair<VxGUID, int64_t>> ignoreList;
    IgnoreListMgr& ignoreMgr = m_Engine.getIgnoreListMgr();
    // make a copy to avoid pausing engine and to avoid threading issues
    ignoreMgr.lockList();
    ignoreList = ignoreMgr.getIdentList();
    ignoreMgr.unlockList();

    // for each see if we already have that ident as gui user else request it
}