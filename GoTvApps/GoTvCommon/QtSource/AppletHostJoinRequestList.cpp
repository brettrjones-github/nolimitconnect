//============================================================================
// Copyright (C) 2021 Brett R. Jones
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
#include "AppletHostJoinRequestList.h"
#include "GuiHostJoinMgr.h"
#include "GuiHostJoin.h"
#include "GuiHostJoinSession.h"

#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"	
#include "GuiParams.h"

#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletHostJoinRequestList::AppletHostJoinRequestList( AppCommon& app,  QWidget* parent )
: AppletBase( OBJNAME_APPLET_HOST_JOIN_REQUEST_LIST, app, parent )
, m_HostJoinMgr( app.getHostJoinMgr() )
{
    setAppletType( eAppletHostJoinRequestList );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	GuiHelpers::fillJoinRequest( ui.m_RequestStateComboBox );
	ui.m_InviteCreateButton->setFixedSize( eButtonSizeSmall );
	ui.m_InviteCreateButton->setIcon( eMyIconInviteCreate );
	ui.m_InviteAcceptButton->setFixedSize( eButtonSizeSmall );
	ui.m_InviteAcceptButton->setIcon( eMyIconInvite );

	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinRequested( GuiHostJoin * ) ), this, SLOT( slotHostJoinRequested( GuiHostJoin *) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinUpdated( GuiHostJoin* ) ), this, SLOT( slotlHostJoinUpdated( GuiHostJoin * ) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinRemoved( VxGUID&, EHostType ) ), this, SLOT( slotHostJoinRemoved( VxGUID&, EHostType ) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinOfferStateChange( VxGUID&, EHostType, EJoinState ) ), this, SLOT( slotHostJoinOfferStateChange( VxGUID&, EHostType, EJoinState ) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinOnlineStatus( GuiHostJoin *, bool ) ), this, SLOT( slotHostJoinOnlineStatus( GuiHostJoin*, bool ) ) );

	connect( ui.m_HostJoinRequestList, SIGNAL( signalAcceptButtonClicked( GuiHostJoinSession *, HostJoinRequestListItem * ) ), this, SLOT( slotAcceptButtonClicked( GuiHostJoinSession*, HostJoinRequestListItem* ) ) );
	connect( ui.m_HostJoinRequestList, SIGNAL( signalRejectButtonClicked( GuiHostJoinSession*, HostJoinRequestListItem* ) ), this, SLOT( slotRejectButtonClicked( GuiHostJoinSession*, HostJoinRequestListItem* ) ) );

	connect( ui.m_RequestStateComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotJoinComboBoxSelectionChange( int ) ) );
	connect( ui.m_InviteCreateButton, SIGNAL( clicked() ), this, SLOT( slotInviteCreateButtonClicked() ) );
	connect( ui.m_InviteAcceptButton, SIGNAL( clicked() ), this, SLOT( slotInviteAcceptButtonClicked() ) );

	updateJoinList();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletHostJoinRequestList::~AppletHostJoinRequestList()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletHostJoinRequestList::updateJoinList( void )
{
	ui.m_HostJoinRequestList->clear();
	std::map<VxGUID, GuiHostJoin*>& joinList = m_HostJoinMgr.getHostJoinList();
	for( auto& iter : joinList )
	{
		GuiHostJoin* user = iter.second;
		if( !user->getUser()->isIgnored() && m_JoinState == user->getJoinState( m_HostType ) )
		{
			updateHostJoinRequest( user );
		}
	}
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinRequested( GuiHostJoin* user )
{
	updateHostJoinRequest( user );
}

//============================================================================
void AppletHostJoinRequestList::slotlHostJoinUpdated( GuiHostJoin* user )
{
	updateHostJoinRequest( user );
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinRemoved( VxGUID& onlineId, EHostType hostType )
{
	ui.m_HostJoinRequestList->removeHostJoinRequest( onlineId, hostType );
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinOfferStateChange( VxGUID& userOnlineId, EHostType hostType, EJoinState hostOfferState )
{
	updateJoinList();
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline )
{
	updateJoinList();
}

//============================================================================
void AppletHostJoinRequestList::updateHostJoinRequest( GuiHostJoin* user )
{
	vx_assert( user );
	VxGUID& onlineId = user->getUser()->getMyOnlineId();
	std::vector<EHostType> hostRequests;
	user->getRequestStateHosts( m_JoinState, hostRequests );
	for( int hostEnum = eHostTypeUnknown; hostEnum < eMaxHostType; hostEnum++ )
	{
		EHostType hostType = (EHostType)hostEnum;
		if( std::find( hostRequests.begin(), hostRequests.end(), hostType ) != hostRequests.end() )
		{
			ui.m_HostJoinRequestList->addOrUpdateHostRequest( hostType, user );
		}
		else
		{
			ui.m_HostJoinRequestList->removeHostJoinRequest( onlineId, hostType );
		}
	}
}

//============================================================================
void AppletHostJoinRequestList::slotAcceptButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	if( joinSession && joinSession->getUserIdent() )
	{
		m_HostJoinMgr.joinAccepted( joinSession->getUserIdent(), joinSession->getHostType() );
	}
}

//============================================================================
void AppletHostJoinRequestList::slotRejectButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	m_HostJoinMgr.joinRejected( joinSession->getUserIdent(), joinSession->getHostType() );
}

//============================================================================
void AppletHostJoinRequestList::slotJoinComboBoxSelectionChange( int comboIdx )
{
	m_JoinState = GuiHelpers::comboIdxToJoinState( comboIdx );
	updateJoinList();
}

//============================================================================
void AppletHostJoinRequestList::slotInviteCreateButtonClicked( void )
{
	m_MyApp.getAppletMgr().launchApplet( eAppletInviteCreate, this );
}

//============================================================================
void AppletHostJoinRequestList::slotInviteAcceptButtonClicked( void )
{
	m_MyApp.getAppletMgr().launchApplet( eAppletInviteAccept, this );
}