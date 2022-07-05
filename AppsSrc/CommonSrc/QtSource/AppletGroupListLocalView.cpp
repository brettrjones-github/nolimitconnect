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
// http://www.nolimitconnect.org
//============================================================================

#include "AppCommon.h"
#include "AppGlobals.h"
#include "AppletPopupMenu.h"
#include "AppletGroupListLocalView.h"
#include "ActivityMessageBox.h"
#include "GuiHostSession.h"
#include "GuiParams.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>

namespace
{
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletGroupListLocalView::AppletGroupListLocalView(	AppCommon&		    app, 
												    QWidget *			parent )
: AppletClientBase( OBJNAME_APPLET_GROUP_LIST_LOCAL_VIEW, app, parent )
, m_CloseAppletTimer( new QTimer( this ) )
{
    setAppletType( eAppletGroupListLocalView );
    setHostType( eHostTypeGroup );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    setSearchType( eSearchGroupHost );

    connectBarWidgets();

    connect( this,					    SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );
    connect( ui.m_RefreshButton,	    SIGNAL(clicked()),		                    this, SLOT(slotRefreshGroupList()) );
    connect( this,					    SIGNAL(signalSearchComplete()),				this, SLOT(slotSearchComplete()) );
    connect( this,					    SIGNAL(signalSearchResult(VxNetIdent*)),	this, SLOT(slotSearchResult(VxNetIdent*)) ); 

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( &m_MyApp, SIGNAL(signalHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )),
        this, SLOT(slotHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )),
        this, SLOT(slotHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )) );


    connect( ui.m_HostedListWidget,      SIGNAL(signalIconButtonClicked(GuiHostSession*,GuiHostedListItem*)),  this, SLOT(slotIconButtonClicked(GuiHostSession*,GuiHostedListItem*)) );
    connect( ui.m_HostedListWidget,      SIGNAL(signalMenuButtonClicked(GuiHostSession*,GuiHostedListItem*)),  this, SLOT(slotMenuButtonClicked(GuiHostSession*,GuiHostedListItem*)) );
    connect( ui.m_HostedListWidget,      SIGNAL(signalJoinButtonClicked(GuiHostSession*,GuiHostedListItem*)),  this, SLOT(slotJoinButtonClicked(GuiHostSession*,GuiHostedListItem*)) );

    setStatusLabel( QObject::tr( "Groups Announced To Network Host" ) );
    m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, true );
    slotRefreshGroupList();

    if( eFriendStateIgnore == m_Engine.getPluginPermission( ePluginTypeHostNetwork ) )
    {
        okMessageBox( QObject::tr( "Service Unavailable" ), QObject::tr( "Network Host Service Not Enabled. You view groups announced to this device because the service is not enabled" ) );
        connect( m_CloseAppletTimer, SIGNAL(timeout()), this, SLOT(onCancelButClick()) );
        m_CloseAppletTimer->setSingleShot( true );
        m_CloseAppletTimer->start( 1000 );
    }
    else if( eFriendStateIgnore == m_Engine.isDirectConnectReady() )
    {
        okMessageBox( QObject::tr( "Service Disabled" ), QObject::tr( "Network Host Service is not enabled because it requires an open port." ) );
        connect( m_CloseAppletTimer, SIGNAL(timeout()), this, SLOT(onCancelButClick()) );
        m_CloseAppletTimer->setSingleShot( true );
        m_CloseAppletTimer->start( 1000 );
    }
}

//============================================================================
AppletGroupListLocalView::~AppletGroupListLocalView()
{
    m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, false );
}

//============================================================================
void AppletGroupListLocalView::setStatusLabel( QString strMsg )
{
    ui.m_StatusLabel->setText( strMsg );
}

//============================================================================
void AppletGroupListLocalView::setInfoLabel( QString strMsg )
{
    ui.m_InfoLabel->setText( strMsg );
    ui.m_InfoLabel->update();
}

//============================================================================
void AppletGroupListLocalView::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
void AppletGroupListLocalView::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletGroupListLocalView::slotHomeButtonClicked( void )
{
}

//============================================================================
void AppletGroupListLocalView::slotRefreshGroupList( void )
{
    clearPluginSettingToList();
    clearStatus();
    m_SearchSessionId.initializeWithNewVxGUID();
    m_MyApp.getFromGuiInterface().fromGuiSendAnnouncedList( eHostTypeGroup, m_SearchSessionId );
}

//============================================================================
void AppletGroupListLocalView::slotSearchComplete( void )
{
    //ui.m_SearchsParamWidget->slotSearchComplete();
}

//============================================================================
void AppletGroupListLocalView::slotInfoMsg( const QString& text )
{
    setStatusLabel( text ); // Adds the message to the widget                                                                                              //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletGroupListLocalView::slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString text )
{
    setInfoLabel( GuiParams::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletGroupListLocalView::slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString text )
{
    setInfoLabel( GuiParams::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletGroupListLocalView::slotHostSearchStatus( EHostType hostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg )
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
void AppletGroupListLocalView::toGuiInfoMsg( char * infoMsg )
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
void AppletGroupListLocalView::infoMsg( const char* errMsg, ... )
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
void AppletGroupListLocalView::clearPluginSettingToList( void )
{
    ui.m_HostedListWidget->clearHostList();
}

//============================================================================
void AppletGroupListLocalView::clearStatus( void )
{
    setInfoLabel( "" );
    setStatusLabel( "" );
}

//============================================================================
void AppletGroupListLocalView::slotIconButtonClicked( GuiHostSession* hostSession, GuiHostedListItem* hostItem )
{

}

//============================================================================
void AppletGroupListLocalView::slotMenuButtonClicked( GuiHostSession* hostSession, GuiHostedListItem* hostItem )
{

}

//============================================================================
void AppletGroupListLocalView::slotJoinButtonClicked( GuiHostSession* hostSession, GuiHostedListItem* hostItem )
{
    onJointButtonClicked( hostSession );
}


//============================================================================
void AppletGroupListLocalView::callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
    LogMsg( LOG_DEBUG, "AppletGroupListLocalView::callbackGuiHostedListSearchResult host %s title %s", DescribeHostType( hostedId.getHostType() ), guiHosted->getHostTitle().c_str() );

    QString strMsg = QObject::tr( "Match found: " );
    strMsg += guiHosted->getHostTitle().c_str();
    setInfoLabel( strMsg );
    updateHostedList( hostedId, guiHosted, sessionId );
}

//============================================================================
void AppletGroupListLocalView::updateHostedList( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
    ui.m_HostedListWidget->updateHostedList( hostedId, guiHosted, sessionId );
}

//============================================================================   
void AppletGroupListLocalView::onCancelButClick( void )
{
    onBackButtonClicked();
}
