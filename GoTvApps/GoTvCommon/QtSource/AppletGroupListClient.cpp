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
#include "AppletGroupListClient.h"
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
AppletGroupListClient::AppletGroupListClient(	AppCommon&		    app, 
												QWidget *			parent )
: AppletClientBase( OBJNAME_APPLET_GROUP_LIST_CLIENT, app, parent )
{
    setAppletType( eAppletGroupListClient );
    setHostType( eHostTypeGroup );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    setSearchType( eSearchGroupHost );

    connectBarWidgets();

    connect( this,					    SIGNAL(finished(int)),						this, SLOT(slotHomeButtonClicked()) );
    connect( ui.m_SearchsParamWidget,	SIGNAL(signalSearchState(bool)),		    this, SLOT(slotStartSearchState(bool)) );
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

    connect( ui.m_HostListWidget,      SIGNAL( signalIconButtonClicked( GuiHostSession*, HostListItem* ) ),  this, SLOT( slotIconButtonClicked( GuiHostSession*, HostListItem* ) ) );
    connect( ui.m_HostListWidget,      SIGNAL( signalMenuButtonClicked( GuiHostSession*, HostListItem* ) ),  this, SLOT( slotMenuButtonClicked( GuiHostSession*, HostListItem* ) ) );
    connect( ui.m_HostListWidget,      SIGNAL( signalJoinButtonClicked( GuiHostSession*, HostListItem* ) ),  this, SLOT( slotJoinButtonClicked( GuiHostSession*, HostListItem* ) ) );
    connect( ui.m_HostListWidget,      SIGNAL( signalConnectButtonClicked( GuiHostSession*, HostListItem* ) ), this, SLOT( slotConnectButtonClicked( GuiHostSession*, HostListItem* ) ) );

    setStatusLabel( QObject::tr( "Fetch Group Host List" ) );
    std::string lastHostSearchText;
    m_MyApp.getAppSettings().getLastHostSearchText( getSearchType(), lastHostSearchText ); 
    if( !lastHostSearchText.empty() )
    {
        ui.m_SearchsParamWidget->getSearchTextEdit()->setText( lastHostSearchText.c_str() );
    }

    ui.m_SearchsParamWidget->setVisible( false );
    ui.m_SearchsParamWidget->setSearchListAll( true );
    slotStartSearchState( true );
}

//============================================================================
void AppletGroupListClient::setStatusLabel( QString strMsg )
{
    ui.m_StatusLabel->setText( strMsg );
}

//============================================================================
void AppletGroupListClient::setInfoLabel( QString strMsg )
{
    ui.m_InfoLabel->setText( strMsg );
    ui.m_InfoLabel->update();
}

//============================================================================
void AppletGroupListClient::showEvent( QShowEvent * ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, this, true );
}

//============================================================================
void AppletGroupListClient::hideEvent( QHideEvent * ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletGroupListClient::slotHomeButtonClicked( void )
{
}

//============================================================================
void AppletGroupListClient::slotStartSearchState(bool startSearch)
{
    if( startSearch && !m_SearchStarted )
    {
        m_SearchStarted = true;
        clearPluginSettingToList();
        clearStatus();

        QString strSearch = getSearchText();
        if( !ui.m_SearchsParamWidget->getSearchListAll() && 3 > strSearch.length() )
        {
            ui.m_SearchsParamWidget->slotSearchCancel();
            ActivityMessageBox errMsgBox( m_MyApp, this, LOG_ERROR, QObject::tr( "Search must have at least 3 characters" ) );
            errMsgBox.exec();
        }
        else
        {
            if( ui.m_SearchsParamWidget->toSearchParams( m_SearchParams ) )
            {
                m_SearchParams.setHostType( getHostType() );
                m_SearchParams.setSearchType( getSearchType() );
                m_SearchParams.createNewSessionId();
                m_SearchParams.updateSearchStartTime();
                setStatusLabel( QObject::tr( "Search Started" ) );
                if( !m_SearchParams.getSearchText().empty() )
                {
                    m_MyApp.getAppSettings().setLastHostSearchText( m_SearchParams.getSearchType(), m_SearchParams.getSearchText() );
                    m_MyApp.getAppSettings().setLastHostSearchAgeType( m_SearchParams.getSearchType(), m_SearchParams.getAgeType() );
                    m_MyApp.getAppSettings().setLastHostSearchGender( m_SearchParams.getSearchType(), m_SearchParams.getGender() );
                    m_MyApp.getAppSettings().setLastHostSearchLanguage( m_SearchParams.getSearchType(), m_SearchParams.getLanguage() );
                    m_MyApp.getAppSettings().setLastHostSearchContentRating( m_SearchParams.getSearchType(), m_SearchParams.getContentRating() );
                }

                m_FromGui.fromGuiSearchHost( getHostType(), m_SearchParams, true );
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
void AppletGroupListClient::slotSearchComplete( void )
{
    ui.m_SearchsParamWidget->slotSearchComplete();
}

//============================================================================
void AppletGroupListClient::slotInfoMsg( const QString& text )
{
    setStatusLabel( text ); // Adds the message to the widget                                                                                              //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletGroupListClient::slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString text )
{
    setInfoLabel( GuiParams::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletGroupListClient::slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString text )
{
    setInfoLabel( GuiParams::describeStatus(hostStatus) + text);
}

//============================================================================
void AppletGroupListClient::slotHostSearchStatus( EHostType hostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg )
{
    if( hostStatus == eHostSearchCompleted )
    {
        m_SearchStarted = false;
        setStatusLabel( strMsg );
        ui.m_SearchsParamWidget->slotSearchComplete();
    }
    else
    {
        setInfoLabel( strMsg );
    }
}

//============================================================================
void AppletGroupListClient::slotHostSearchResult( EHostType hostType, VxGUID sessionId, VxNetIdent hostIdent, PluginSetting pluginSetting )
{
    LogMsg( LOG_DEBUG, "slotHostSearchResult host %s ident %s plugin %s", DescribeHostType( hostType ), hostIdent.getOnlineName(), 
        DescribePluginType( pluginSetting.getPluginType() ) );

    QString strMsg = QObject::tr( "Match found: " );
    strMsg += hostIdent.getOnlineName();
    setInfoLabel( strMsg );
    addPluginSettingToList( hostType, sessionId, hostIdent, pluginSetting );
}

//============================================================================
void AppletGroupListClient::toGuiInfoMsg( char * infoMsg )
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
void AppletGroupListClient::infoMsg( const char* errMsg, ... )
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
void AppletGroupListClient::addPluginSettingToList( EHostType hostType, VxGUID& sessionId, VxNetIdent& hostIdent, PluginSetting& pluginSetting )
{
    ui.m_HostListWidget->addHostAndSettingsToList( hostType, sessionId, hostIdent, pluginSetting );
}

//============================================================================
void AppletGroupListClient::clearPluginSettingToList( void )
{
    ui.m_HostListWidget->clearHostList();
}

//============================================================================
void AppletGroupListClient::clearStatus( void )
{
    setInfoLabel( "" );
    setStatusLabel( "" );
}

//============================================================================
void AppletGroupListClient::slotIconButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem )
{

}

//============================================================================
void AppletGroupListClient::slotMenuButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem )
{

}

//============================================================================
void AppletGroupListClient::slotJoinButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem )
{
    onJointButtonClicked( hostSession );
}

//============================================================================
void AppletGroupListClient::slotConnectButtonClicked( GuiHostSession* hostSession, HostListItem* hostItem )
{
    onJointButtonClicked( hostSession );
}
