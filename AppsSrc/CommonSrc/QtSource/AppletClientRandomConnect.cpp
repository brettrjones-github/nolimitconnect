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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletClientRandomConnect.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

//============================================================================
AppletClientRandomConnect::AppletClientRandomConnect( AppCommon& app, QWidget* parent )
: AppletClientBase( OBJNAME_APPLET_CLIENT_RANDOM_PERSON, app, parent )
{
    setAppletType( eAppletClientRandomConnect );
    ui.setupUi( this );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

    connectBarWidgets();

    connect( ui.m_SerchButton, SIGNAL( clicked() ), this, SLOT( slotSimulateShakeClicked() ) );
    connect( this, SIGNAL( signalSearchComplete() ), this, SLOT( slotSearchComplete() ) );
    connect( this, SIGNAL( signalSearchResult( VxNetIdent* ) ), this, SLOT( slotSearchResult( VxNetIdent* ) ) );
    connect( &app,
             SIGNAL( signalRandomConnectStatus( ERandomConnectStatus, QString ) ),
             this,
             SLOT( slotRandomConnectStatus( ERandomConnectStatus, QString ) ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletClientRandomConnect::~AppletClientRandomConnect()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletClientRandomConnect::showEvent( QShowEvent* ev )
{
    ActivityBase::showEvent( ev );
    m_MyApp.wantToGuiActivityCallbacks( this, true );
}

//============================================================================
void AppletClientRandomConnect::hideEvent( QHideEvent* ev )
{
    m_MyApp.wantToGuiActivityCallbacks( this, false );
    ActivityBase::hideEvent( ev );
}

//============================================================================
void AppletClientRandomConnect::toGuiScanResultSuccess( EScanType eScanType, GuiUser * netIdent )
{
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
void AppletClientRandomConnect::toGuiClientScanSearchComplete( EScanType eScanType )
{
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
void AppletClientRandomConnect::slotHomeButtonClicked( void )
{
}

//============================================================================
void AppletClientRandomConnect::slotSimulateShakeClicked( void )
{
    m_MyApp.playSound( eSndDefSending );
    ui.m_FriendListWidget->clear();
    m_MyApp.toGuiUserMessage( QObject::tr( "Contacting Network Host" ).toUtf8().constData() );
    m_FromGui.fromGuiStartScan( eScanTypeRandomConnect, 0, 0 );
}

//============================================================================
void AppletClientRandomConnect::slotSearchComplete( void )
{
    m_MyApp.toGuiUserMessage( QObject::tr( "Search Complete" ).toUtf8().constData() );
}

//============================================================================
void AppletClientRandomConnect::slotSearchResult( GuiUser * netIdent )
{
    m_MyApp.playSound( eSndDefUserBellMessage );
    QString foundMatch = QObject::tr( "Search Complete " );
    foundMatch += netIdent->getOnlineName().c_str();
    m_MyApp.toGuiUserMessage( foundMatch.toUtf8().constData(), netIdent->getOnlineName().c_str() );
    ui.m_FriendListWidget->updateFriend( netIdent, false );
}

//============================================================================
void AppletClientRandomConnect::slotRandomConnectStatus( ERandomConnectStatus eHostStatus, QString strMsg )
{
    m_MyApp.toGuiUserMessage( strMsg.toUtf8().constData() );
}
