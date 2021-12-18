//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "AppletChatRoomJoin.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

namespace
{
    const int MAX_LOG_EDIT_BLOCK_CNT = 1000;
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletChatRoomJoin::AppletChatRoomJoin( AppCommon& app, QWidget * parent )
: AppletClientBase( OBJNAME_APPLET_CHAT_ROOM_JOIN, app, parent )
{
    ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletChatRoomJoin );
    setHostType( eHostTypeChatRoom );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    setupApplet();

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletChatRoomJoin::~AppletChatRoomJoin()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletChatRoomJoin::setupApplet( void )
{
    getInfoEdit()->setMaximumBlockCount( MAX_LOG_EDIT_BLOCK_CNT );
    getInfoEdit()->setReadOnly( true );

    connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );
    connect( ui.m_JoinDefaultButton, SIGNAL( clicked() ), this, SLOT( slotJoinDefaultChatRoom() ) );
    connect( ui.m_CopyResultToClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyResultToClipboardClicked() ) );

    connect( this, SIGNAL( signalLogMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );
    connect( this, SIGNAL( signalInfoMsg( const QString& ) ), this, SLOT( slotInfoMsg( const QString& ) ) );

    connect( &m_MyApp, SIGNAL(signalHostAnnounceStatus( EHostType, VxGUID, EHostAnnounceStatus, QString )),
        this, SLOT(slotHostAnnounceStatus( EHostType, VxGUID, EHostAnnounceStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )),
        this, SLOT(slotHostJoinStatus( EHostType, VxGUID, EHostJoinStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )),
        this, SLOT(slotHostSearchStatus( EHostType, VxGUID, EHostSearchStatus, QString )) );
    connect( &m_MyApp, SIGNAL(signalHostSearchResult( EHostType, VxGUID, VxNetIdent , PluginSetting  )),
        this, SLOT(slotHostSearchResult( EHostType, VxGUID, VxNetIdent , PluginSetting  )) );
}

//============================================================================
void AppletChatRoomJoin::slotJoinDefaultChatRoom( void )
{
    VxGUID::generateNewVxGUID( m_JoinSessionId );
    m_Engine.fromGuiJoinHost( getHostType(), m_JoinSessionId );
}

//============================================================================
void AppletChatRoomJoin::slotCopyResultToClipboardClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText( getInfoEdit()->toPlainText() );
}

//============================================================================
void AppletChatRoomJoin::toGuiInfoMsg( char * infoMsg )
{
    QString infoStr( infoMsg );
#if QT_VERSION > QT_VERSION_CHECK(6,0,0)
    infoStr.remove(QRegularExpression("[\\n\\r]"));
#else
    infoStr.remove( QRegExp( "[\\n\\r]" ) );
#endif // QT_VERSION > QT_VERSION_CHECK(6,0,0)
    emit signalInfoMsg( infoStr );
}

//============================================================================
void AppletChatRoomJoin::slotInfoMsg( const QString& text )
{
    getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom                                                                                                  //m_LogFile.write( text ); // Logs to file
}

//============================================================================
void AppletChatRoomJoin::slotHostAnnounceStatus( EHostType hostType, VxGUID sessionId, EHostAnnounceStatus hostStatus, QString text )
{
    getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
}

//============================================================================
void AppletChatRoomJoin::slotHostJoinStatus( EHostType hostType, VxGUID sessionId, EHostJoinStatus hostStatus, QString text )
{
    getInfoEdit()->appendPlainText( text ); // Adds the message to the widget
    getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
}

//============================================================================
void AppletChatRoomJoin::slotHostSearchStatus( EHostType hostType, VxGUID sessionId, EHostSearchStatus hostStatus, QString strMsg )
{
    getInfoEdit()->appendPlainText( strMsg ); // Adds the message to the widget
    getInfoEdit()->verticalScrollBar()->setValue( getInfoEdit()->verticalScrollBar()->maximum() ); // Scrolls to the bottom
}

//============================================================================
void AppletChatRoomJoin::slotHostSearchResult( EHostType hostType, VxGUID sessionId, VxNetIdent hostIdent, PluginSetting pluginSetting )
{
    LogMsg( LOG_DEBUG, "slotHostSearchResult host %s ident %s plugin %s", DescribeHostType( hostType ), hostIdent.getOnlineName(), 
           DescribePluginType( pluginSetting.getPluginType() ) );
}

//============================================================================
void AppletChatRoomJoin::infoMsg( const char* errMsg, ... )
{
    char as8Buf[ MAX_INFO_MSG_SIZE ];
    va_list argList;
    va_start( argList, errMsg );
    vsnprintf( as8Buf, sizeof( as8Buf ), errMsg, argList );
    as8Buf[ sizeof( as8Buf ) - 1 ] = 0;
    va_end( argList );

    toGuiInfoMsg( as8Buf );
}
