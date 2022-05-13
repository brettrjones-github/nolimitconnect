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

#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>

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
	ui.m_GuiHostedListWidget->setHostType( eHostTypeGroup );
	ui.m_GuiGroupieListWidget->setIsHostView( false );
	ui.m_GuiGroupieListWidget->setHostType( eHostTypeGroup );

	// so is actually destroyed
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( closeApplet() ) );
	connect( ui.m_ChooseHostButton, SIGNAL( clicked() ), this, SLOT( slotChooseHostModeButtonClick() ) );

	connect( ui.m_GuiHostedListWidget, SIGNAL( signalIconButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ),		this, SLOT( slotIconButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalMenuButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ),		this, SLOT( slotMenuButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalJoinButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ),		this, SLOT( slotJoinButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalConnectButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ),	this, SLOT( slotConnectButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalKickButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ),		this, SLOT( slotKickButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ) );
	connect( ui.m_GuiHostedListWidget, SIGNAL( signalFavoriteButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ), this, SLOT( slotFavoriteButtonClicked(GuiHostedListSession*,GuiHostedListItem*) ) );

	m_MyApp.activityStateChange( this, true );
	m_MyApp.getUserMgr().wantGuiUserUpdateCallbacks( this, true );
	m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, true );
	m_MyApp.getGroupieListMgr().wantGroupieListCallbacks( this, true );
	m_MyApp.getHostJoinMgr().wantHostJoinCallbacks( this, true );
	m_MyApp.getUserJoinMgr().wantUserJoinCallbacks( this, true );

	m_UserListMode = true;
	changeGuiMode( false );
	queryHostedList();

	m_JoinedHostSession.initializeWithNewVxGUID();
	// m_MyApp.getFromGuiInterface().fromGuiJoinLastJoinedHost( getHostType(), m_JoinedHostSession );
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
	changeGuiMode( false );
}

//============================================================================
void AppletGroupJoin::changeGuiMode( bool userListMode )
{
	// if( userListMode != m_UserListMode )
	{
		m_UserListMode = userListMode;
		if( userListMode )
		{
			setListLabel( QObject::tr( "User List" ) );
			ui.m_ChooseHostButton->setVisible( true );
			ui.m_HostAdminLabel->setVisible( true );
			ui.m_HostAdminIdentWidget->setVisible( true );

			ui.m_GuiHostedListWidget->setVisible( false );
			ui.m_GuiGroupieListWidget->setVisible( true );
		}
		else
		{
			setListLabel( QObject::tr( "Group Host List" ) );
			ui.m_ChooseHostButton->setVisible( false );
			ui.m_HostAdminLabel->setVisible( false );
			ui.m_HostAdminIdentWidget->setVisible( false );

			ui.m_GuiHostedListWidget->setVisible( true );
			ui.m_GuiGroupieListWidget->setVisible( false );
		}
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
	changeGuiMode( false );
	ui.m_GuiHostedListWidget->clear();
	ui.m_GuiGroupieListWidget->clear();

	AppletJoinBase::queryHostedList();
}

//============================================================================
void AppletGroupJoin::callbackOnUserAdded( GuiUser* guiUser )
{
	updateUser( guiUser );
}

//============================================================================
void AppletGroupJoin::callbackOnUserUpdated( GuiUser* guiUser )
{
	updateUser( guiUser );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
	if( hostedId.getHostType() == m_HostType && guiHosted )
	{
		ui.m_GuiHostedListWidget->updateHostedList( hostedId, guiHosted, sessionId );
		GroupieId groupieId( m_MyApp.getMyOnlineId(), hostedId );
		if( !m_UserListMode && m_MyApp.getUserJoinMgr().getUserJoinState( groupieId ) == eJoinStateJoinIsGranted  )
		{
			onJoinedHost( groupieId, guiHosted );
		}
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

	if( joinState == eJoinStateJoinIsGranted )
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
void AppletGroupJoin::slotFavoriteButtonClicked( GuiHostedListSession* hostSession, GuiHostedListItem* hostItem )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::slotFavoriteButtonClicked" );
	std::string ptopUrl = hostSession->getHostUrl();
	if( !ptopUrl.empty() )
	{
		if( yesNoMessageBox( QObject::tr( "Join Host On Startup?" ), QObject::tr( "Join This Host When Application Starts?" ) ) )
		{
			m_MyApp.getHostedListMgr().setJoinOnStartup( ptopUrl, true );
		}
		else
		{
			m_MyApp.getHostedListMgr().setJoinOnStartup( ptopUrl, false );
		}
	}
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinRequested( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinRequested" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinRequested( groupieId, guiHostJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinWasGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinGranted" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinWasGranted( groupieId, guiHostJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiHostJoinIsGranted( GroupieId& groupieId, GuiHostJoin* guiHostJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiHostJoinIsGranted" );
	ui.m_GuiHostedListWidget->callbackGuiHostJoinIsGranted( groupieId, guiHostJoin );
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
void AppletGroupJoin::callbackGuiUserJoinWasGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinWasGranted" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinWasGranted( groupieId, guiUserJoin );
}

//============================================================================
void AppletGroupJoin::callbackGuiUserJoinIsGranted( GroupieId& groupieId, GuiUserJoin* guiUserJoin )
{
	LogMsg( LOG_VERBOSE, "AppletGroupJoin::callbackGuiUserJoinGranted" );
	ui.m_GuiHostedListWidget->callbackGuiUserJoinIsGranted( groupieId, guiUserJoin );
	ui.m_GuiGroupieListWidget->callbackGuiUserJoinIsGranted( groupieId, guiUserJoin );
	GuiHosted* guiHosted = ui.m_GuiHostedListWidget->findGuiHostedByHostId( groupieId.getHostedId() );
	onJoinedHost( groupieId, guiHosted );
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

//============================================================================
void AppletGroupJoin::updateUser( GuiUser* guiUser )
{
	ui.m_GuiHostedListWidget->updateUser( guiUser );
	ui.m_GuiGroupieListWidget->updateUser( guiUser );
}

//============================================================================
void AppletGroupJoin::onJoinedHost( GroupieId& groupieId, GuiHosted* guiHosted )
{
	if( !m_UserListMode )
	{
		changeGuiMode( true );
		// find and fill in the host admin
		ui.m_HostAdminIdentWidget->updateHosted( guiHosted );
		m_MyApp.getEngine().getUserJoinMgr().queryUserListFromHost( groupieId );
	}
}