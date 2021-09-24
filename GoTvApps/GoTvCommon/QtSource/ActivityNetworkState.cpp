//============================================================================
// Copyright (C) 2016 Brett R. Jones
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
#include "ActivityNetworkState.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <CoreLib/IsBigEndianCpu.h>

#include <QDesktopServices>
#include <QUrl>

namespace
{
	const int					MAX_NET_HELP_LINES			= 8;	
}

//============================================================================
ActivityNetworkState::ActivityNetworkState( AppCommon& app, QWidget * parent )
: ActivityBase( OBJNAME_ACTIVITY_NETWORK_STATE, app, parent, eAppletMessengerFrame, true )
{
	ui.setupUi(this);
	ui.m_TitleBarWidget->setTitleBarText( QObject::tr( "Network State" ) );

    connectBarWidgets();

	setupActivityNetworkState();

	connect( ui.m_TitleBarWidget,	SIGNAL(signalBackButtonClicked()),						this, SLOT(reject()) );
	connect( ui.gotoWebsiteButton,	SIGNAL(clicked()),										this, SLOT(gotoWebsite()) );
	connect( &app,					SIGNAL(signalNetworkStateChanged(ENetworkStateType)),	this, SLOT(slotNetworkStateChanged(ENetworkStateType)) );
	slotNetworkStateChanged( app.getNetworkState() );
}

//============================================================================
void ActivityNetworkState::setupActivityNetworkState( void )
{
}

//============================================================================
void  ActivityNetworkState::gotoWebsite( void )
{
	QDesktopServices::openUrl( QUrl("http://www.nolimitconnect.com/") );
}

//============================================================================
void ActivityNetworkState::slotNetworkStateChanged( ENetworkStateType eNetworkState )
{
    VxNetIdent myIdent;
	m_FromGui.fromGuiQueryMyIdent( &myIdent );
	std::string strOnlineIp = myIdent.getOnlineIpAddress().toStdString();
	uint16_t tcpPort = myIdent.getMyOnlinePort();
	ui.m_IPAddressValueLabel->setText( strOnlineIp.c_str() );
	ui.m_TcpPortValueLabel->setText( QString("%1").arg( tcpPort ) );
	ui.m_MyNodeUrlValueLabel->setText( QString("My Node Url: http://%1:%2").arg( strOnlineIp.c_str() ).arg(tcpPort) );

	bool networkConnected = true;
	switch( eNetworkState )
	{
	case eNetworkStateTypeUnknown:
	case eNetworkStateTypeLost:
	case eNetworkStateTypeNoInternetConnection:
		showNoInternetHelp();
		networkConnected = false;
		break;

	case eNetworkStateTypeAvail:
	case eNetworkStateTypeTestConnection:
	case eNetworkStateTypeAnnounce:
		showDiscoverNetworkHelp();
		break;

	case eNetworkStateTypeGetRelayList:
	case eNetworkStateTypeRelaySearch:
		showRelaySearchHelp();
		break;

	case eNetworkStateTypeOnlineDirect:
		showDirectConnectMessage();
		break;

	case eNetworkStateTypeOnlineThroughRelay:
		showRelayConnectedHelp();
		break;

	case eNetworkStateTypeFailedResolveHostNetwork:
        showHostNetworkError();
		break;

    case eNetworkStateTypeFailedResolveHostGroup:
        showHostGroupError();
        break;

    case eNetworkStateTypeFailedResolveHostGroupList:
        showHostGroupListError();
        break;

    case eMaxNetworkStateType:
    default:
        break;
	}

	if( networkConnected )
	{
		ui.m_InternetConnectedStateLabel->setText( QObject::tr("YES") );
	}

	ui.m_P2PWebNetStateValueLabel->setText( DescribeNetworkState( eNetworkState ) );
}

//============================================================================
void ActivityNetworkState::showNoInternetHelp( void )
{
	clearHelpLines();
	setHelpLine( 0, QObject::tr("No internet yet available") );
}

//============================================================================
void ActivityNetworkState::showDiscoverNetworkHelp( void )
{
	clearHelpLines();
	setHelpLine( 0, QObject::tr("Internet connected and NoLimitConnect ") );
	setHelpLine( 1, QObject::tr("is testing the network.") );
}

//============================================================================
void ActivityNetworkState::showRelaySearchHelp( void )
{
	setHelpLine( 0, QObject::tr("Internet connected but incoming TCP Port is blocked. ") );
	setHelpLine( 1, QObject::tr("This may be because UPNP is disable on the router or ") );
	setHelpLine( 2, QObject::tr("your device has a firewall or anti-virus program that ") );
	setHelpLine( 3, QObject::tr("blocks the TCP Port used by NoLimitConnect.  ") );
	setHelpLine( 4, QObject::tr("NoLimitConnect is searching for another node to act as  ") );
	setHelpLine( 5, QObject::tr("relay/proxy for this device. Check Website for more details ") );
}

//============================================================================
void ActivityNetworkState::showDirectConnectMessage( void )
{
	setHelpLine( 0, QObject::tr("Internet is connected and TCP Port ") ); 
	setHelpLine( 1, QObject::tr("can accept incoming connections. ") );
	setHelpLine( 2, QObject::tr("You have the best possible connection!") );
}

//============================================================================
void ActivityNetworkState::showRelayConnectedHelp( void )
{
	setHelpLine( 0, QObject::tr("NoLimitConnect connected but incoming TCP Port is blocked. ") );
	setHelpLine( 1, QObject::tr("This may be because UPNP is disable on the router or ") );
	setHelpLine( 2, QObject::tr("your device has a firewall or anti-virus program that ") );
	setHelpLine( 3, QObject::tr("blocks the TCP Port used by NoLimitConnect.  ") );
	setHelpLine( 4, QObject::tr("NoLimitConnect requires relay service for this device which results in slow " ) );
	setHelpLine( 6, QObject::tr("data transfer and unstable connectivity." ) );
	setHelpLine( 7, QObject::tr("Visit NoLimitConnect Website for help on how to direct connect.") );
}

//============================================================================
void ActivityNetworkState::showHostNetworkError( void )
{
    setHelpLine( 0, QObject::tr("NoLimitConnect connected but could not contact the Network Host. ") );
    setHelpLine( 1, QObject::tr("Check your network setting and Network Host url.") );
    setHelpLine( 2, QObject::tr("Visit NoLimitConnect  Website for details.") );
}

//============================================================================
void ActivityNetworkState::showHostGroupError( void )
{
    setHelpLine( 0, QObject::tr("NoLimitConnect connected but could not contact the Group Host. ") );
    setHelpLine( 1, QObject::tr("Check your network setting and Group Host url.") );
    setHelpLine( 2, QObject::tr("Visit NoLimitConnect  Website for details.") );
}

//============================================================================
void ActivityNetworkState::showHostGroupListError( void )
{
    setHelpLine( 0, QObject::tr("NoLimitConnect connected but could not contact the Group List Host. ") );
	setHelpLine( 1, QObject::tr("Check your network setting and Network Host url.") );
	setHelpLine( 2, QObject::tr("Visit NoLimitConnect  Website for details.") );
}

//============================================================================
void ActivityNetworkState::clearHelpLines()
{
	QString emptyString = "";
	for( int i = 0; i < MAX_NET_HELP_LINES; i++ )
	{
		setHelpLine( i, emptyString );
	}
}

//============================================================================
void ActivityNetworkState::setHelpLine( int lineIdx, QString helpText )
{
	switch( lineIdx )
	{
	case 0:
		ui.m_NetStatLine1->setText( helpText );
		break;

	case 1:
		ui.m_NetStatLine2->setText( helpText );
		break;

	case 2:
		ui.m_NetStatLine3->setText( helpText );
		break;

	case 3:
		ui.m_NetStatLine4->setText( helpText );
		break;

	case 4:
		ui.m_NetStatLine5->setText( helpText );
		break;

	case 5:
		ui.m_NetStatLine6->setText( helpText );
		break;

	case 6:
		ui.m_NetStatLine7->setText( helpText );
		break;

	case 7:
		ui.m_NetStatLine8->setText( helpText );
		break;

	}
}

