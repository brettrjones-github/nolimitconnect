//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "AppletInviteAccept.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiParams.h"
#include "MyIcons.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/Invite.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxUrl.h>

#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QScrollBar>
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QDesktopWidget>
#endif // QT_VERSION < QT_VERSION_CHECK(6,0,0)

namespace
{
    const int MAX_INFO_MSG_SIZE = 2048;
}

//============================================================================
AppletInviteAccept::AppletInviteAccept( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_INVITE_ACCEPT, app, parent )
{
    setAppletType( eAppletInviteAccept );
    ui.setupUi( getContentItemsFrame() );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
    ui.m_IdentWidget->setIdentWidgetSize( eButtonSizeSmall );
    //ui.m_IdentWidget->setVisible( false );
    ui.m_CopyInviteFromClipboardButton->setFixedSize( eButtonSizeSmall );
    ui.m_CopyInviteFromClipboardButton->setIcon( eMyIconEditPaste );
    ui.m_AcceptInviteButton->setFixedSize( eButtonSizeMedium );
    ui.m_AcceptInviteButton->setIcon( eMyIconInviteAccept );
    ui.m_RejectInviteButton->setFixedSize( eButtonSizeMedium );
    ui.m_RejectInviteButton->setIcon( eMyIconCancelNormal );

    connect( ui.m_CopyInviteFromClipboardButton, SIGNAL( clicked() ), this, SLOT( slotCopyInviteFromClipboardButtonClicked() ) );
    connect( ui.m_AcceptInviteButton, SIGNAL( clicked() ), this, SLOT( slotAcceptInviteButtonClicked() ) );
    connect( ui.m_RejectInviteButton, SIGNAL( clicked() ), this, SLOT( slotRejectInviteButtonClicked() ) );

    // Log is seperate now VxAddLogHandler( this );
    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletInviteAccept::~AppletInviteAccept()
{
    // Log is seperate now VxRemoveLogHandler( this );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletInviteAccept::toGuiInfoMsg( char * infoMsg )
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
void AppletInviteAccept::slotCopyInviteFromClipboardButtonClicked( void )
{
    QClipboard * clipboard = QApplication::clipboard();
    if( clipboard->text().isEmpty() )
    {
        okMessageBox( QObject::tr( "Clipboard Is Empty" ), QObject::tr( "Cannot Paste Empty Clipboard" ) );
        return;
    }
    
    Invite invite;
    if( !invite.setInviteText( clipboard->text().toUtf8().constData() ) )
    {
        okMessageBox( QObject::tr( "Clipboard Contained Invalid Invite Text" ), QObject::tr( "Clipboard Has Text That Cannot Be Parsed Into An Invite" ) );
        return;
    }

    ui.m_InviteTextEdit->setPlainText( clipboard->text().toUtf8().constData() );
    updateInvite();
}

//============================================================================
void AppletInviteAccept::slotAcceptInviteButtonClicked( void )
{

}

//============================================================================
void AppletInviteAccept::slotRejectInviteButtonClicked( void )
{
    close();
}

//============================================================================
void AppletInviteAccept::infoMsg( const char* errMsg, ... )
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
void AppletInviteAccept::updateInvite( void )
{
    QString inviteText = getInviteEdit()->toPlainText();
    if( inviteText.isEmpty() )
    {
        return;
    }

    Invite invite;
    if( invite.setInviteText( inviteText.toUtf8().constData() ) )
    {
        ui.m_PersonUrlEdit->setText( invite.getInviteUrl( eHostTypePeerUser ).c_str() );
        ui.m_GroupUrlEdit->setText( invite.getInviteUrl( eHostTypeGroup ).c_str() );
    }
}