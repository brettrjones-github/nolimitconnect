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

#include "AppletGroupJoin.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"
#include "GuiHostedListSession.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletGroupJoin::AppletGroupJoin( AppCommon& app, QWidget * parent )
: AppletJoinBase( OBJNAME_APPLET_GROUP_JOIN, app, parent )
{
	ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletGroupJoin );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	setHostType( eHostTypeGroup );
	ui.m_GuiHostedListWidget->setIsHostView( false );
	ui.m_GuiGroupieListWidget->setIsHostView( false );

	// so is actually destroyed
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( closeApplet() ) );
	connect( ui.m_ChooseHostButton, SIGNAL( clicked() ), this, SLOT( slotChooseHostModeButtonClick() ) );

	connect( ui.m_GuiHostedListWidget, SIGNAL( signalIconButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ), this, SLOT( slotIconButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalMenuButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ), this, SLOT( slotMenuButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalJoinButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ), this, SLOT( slotJoinButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalConnectButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ), this, SLOT( slotConnectButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalKickButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ), this, SLOT( slotKickButtonClicked( GuiHostedListSession*, GuiHostedListItem* ) ) );

	m_MyApp.activityStateChange( this, true );
	m_MyApp.getUserMgr().wantGuiUserUpdateCallbacks( this, true );
	m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, true );
	m_MyApp.getGroupieListMgr().wantGroupieListCallbacks( this, true );
	m_MyApp.getHostJoinMgr().wantHostJoinCallbacks( this, true );
	m_MyApp.getUserJoinMgr().wantUserJoinCallbacks( this, true );

	setupGuiMode( false );
	queryHostedList();

	m_JoinedHostSession.initializeWithNewVxGUID();
	m_MyApp.getFromGuiInterface().fromGuiJoinLastJoinedHost( getHostType(), m_JoinedHostSession );
}

//============================================================================
AppletGroupJoin::~AppletGroupJoin()
{
	m_MyApp.getUserMgr().wantGuiUserUpdateCallbacks( this, false );
	m_MyApp.getGroupieListMgr().wantGroupieListCallbacks( this, false );
	m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, false );
	m_MyApp.getHostJoinMgr().wantHostJoinCallbacks( this, false );
	m_MyApp.getUserJoinMgr().wantUserJoinCallbacks( this, false );
	m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletGroupJoin::slotChooseHostModeButtonClick( void )
{
	setupGuiMode( false );
}

//============================================================================
void AppletGroupJoin::setupGuiMode( bool userListMode )
{
	if( userListMode )
	{
		setListLabel( QObject::tr( "User List" ) );
		ui.m_ChooseHostButton->setVisible( true );
		ui.m_HostAdminLabel->setVisible( true );
		ui.m_HostedIdentWidget->setVisible( true );
		// TODO probably can remove m_HostedPluginWidget completely
		ui.m_HostedPluginWidget->setVisible( true );

		ui.m_GuiGroupieListWidget->setVisible( false );
		ui.m_GuiHostedListWidget->setVisible( true );
	}
	else
	{
		setListLabel( QObject::tr( "Group Host List" ) );
		ui.m_ChooseHostButton->setVisible( false );
		ui.m_HostAdminLabel->setVisible( false );
		ui.m_HostedIdentWidget->setVisible( false );
		ui.m_HostedPluginWidget->setVisible( false );

		ui.m_GuiHostedListWidget->setVisible( true );
		ui.m_GuiGroupieListWidget->setVisible( false );
	}
}

//============================================================================
void AppletGroupJoin::setStatusMsg( QString statusMsg )
{
	ui.m_StatusLabel->setText( statusMsg );
}

//============================================================================
void AppletGroupJoin::setListLabel( QString labelText )
{
	ui.m_HostOrListViewLabel->setText( labelText );
}

//============================================================================
void AppletGroupJoin::queryHostedList( void )
{
	setStatusMsg( QObject::tr("Fetching host list from network host") );
	setupGuiMode( false );
	ui.m_GuiHostedListWidget->clear();
	ui.m_GuiGroupieListWidget->clear();

	AppletJoinBase::queryHostedList();
}

//============================================================================
void AppletGroupJoin::callbackOnUserAdded( GuiUser* guiUser )
{
}

//============================================================================
void AppletGroupJoin::callbackOnUserUpdated( GuiUser* guiUser )
{
}

//============================================================================
void AppletGroupJoin::callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
	if( hostedId.getHostType() == m_HostType && guiHosted )
	{
		ui.m_GuiHostedListWidget->updateHostedList( hostedId, guiHosted, sessionId );
	}
}

//============================================================================
void AppletGroupJoin::callbackGuiHostedListSearchComplete( EHostType hostType, VxGUID& sessionId )
{
	setStatusMsg( QObject::tr( "Host list from network host completed" ) );
}

//============================================================================
void AppletGroupJoin::callbackGuiGroupieListSearchResult( GroupieId& groupieId, GuiGroupie* guiGroupie, VxGUID& sessionId )
{
	if( groupieId.getHostType() == m_HostType && guiGroupie )
	{
		ui.m_GuiGroupieListWidget->updateGroupieList( groupieId, guiGroupie, sessionId );
	}
}

//============================================================================
void AppletGroupJoin::callbackGuiGroupieListSearchComplete( EHostType hostType, VxGUID& sessionId )
{
	setStatusMsg( QObject::tr( "User list from host completed" ) );
}

//============================================================================
void AppletGroupJoin::updateHostedIdent( GuiHosted* guiHosted )
{
	ui.m_HostedIdentWidget->updateIdentity( guiHosted->getUser() );
	ui.m_HostedPluginWidget->updateHosted( guiHosted );
}

//============================================================================
void AppletGroupJoin::slotIconButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::slotIconButtonClicked" );
}

//============================================================================
void AppletGroupJoin::slotMenuButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::slotMenuButtonClicked" );
}

//============================================================================
void AppletGroupJoin::slotJoinButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::slotJoinButtonClicked" );
	std::string ptopUrl = hostSession->getHostUrl();
	m_MyApp.getFromGuiInterface().fromGuiJoinHost( hostSession->getHostType(), hostSession->getSessionId(), ptopUrl );
}

//============================================================================
void AppletGroupJoin::slotConnectButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::slotConnectButtonClicked" );
	std::string ptopUrl = hostSession->getHostUrl();
	GroupieId groupieId( m_MyApp.getMyOnlineId(), hostSession->getHostedId() );
	GuiUserJoin* userJoin = m_MyApp.getUserJoinMgr().getUserJoin( groupieId );
	EJoinState joinState{ eJoinStateNone };
	if( userJoin )
	{
		joinState = userJoin->getJoinState();
	}

	if( joinState == eJoinStateJoinGranted )
	{
		m_MyApp.getFromGuiInterface().fromGuiLeaveHost( hostSession->getHostType(), hostSession->getSessionId(), ptopUrl );
	}
	else
	{
		m_MyApp.getFromGuiInterface().fromGuiJoinHost( hostSession->getHostType(), hostSession->getSessionId(), ptopUrl );
	}
}

//============================================================================
void AppletGroupJoin::slotKickButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::slotKickButtonClicked" );
	if( yesNoMessageBox( QObject::tr( "Revoke Membership" ), QObject::tr( "Are You Sure You Want To Revoke Membership?" ) ) )
	{
		std::string ptopUrl = hostSession->getHostUrl();
		m_MyApp.getFromGuiInterface().fromGuiUnJoinHost( hostSession->getHostType(), hostSession->getSessionId(), ptopUrl );
	}
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinRequested" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinRequested( groupieId, guiHostJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinGranted" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinGranted( groupieId, guiHostJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostUnJoinGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostUnJoinGranted" );
	ui.m_GuiHostedListWidget->callbackGuiHostUnJoinGranted( groupieId, guiHostJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinDenied( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinDenied" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinDenied( groupieId, guiHostJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinLeaveHost( GroupieId& groupieId )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinLeaveHost" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinLeaveHost( groupieId );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostUnJoin( GroupieId& groupieId )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostUnJoin" );
	ui.m_GuiHostedListWidget->callbackGuiHostUnJoin( groupieId );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinRemoved( GroupieId& groupieId )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinRemoved" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinRemoved( groupieId );
}

//============================================================================
//============================================================================

//============================================================================
void AppletGroupJoin::callbackGuiUserJoinRequested( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinRequested" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinRequested( groupieId, guiUserJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiUserJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinGranted" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinGranted( groupieId, guiUserJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiUserUnJoinGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserUnJoinGranted" );
	ui.m_GuiHostedListWidget->callbackGuiUserUnJoinGranted( groupieId, guiUserJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiUserJoinDenied( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinDenied" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinDenied( groupieId, guiUserJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiUserJoinLeaveHost( GroupieId& groupieId )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinLeaveHost" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinLeaveHost( groupieId );
}

//============================================================================
void AppletGroupJoin::callbackGuiUserJoinRemoved( GroupieId& groupieId )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinRemoved" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinLeaveHost( groupieId );
}
