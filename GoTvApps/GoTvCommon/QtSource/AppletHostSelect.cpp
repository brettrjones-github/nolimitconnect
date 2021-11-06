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
#include "AppletHostSelect.h"
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
AppletHostSelect::AppletHostSelect( AppCommon& app,  QWidget* parent, int launchParam )
: AppletBase( OBJNAME_APPLET_HOST_SELECT, app, parent )
, m_HostJoinMgr( app.getHostJoinMgr() )
{
    setAppletType( eAppletHostSelect );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	GuiHelpers::fillJoinRequest( ui.m_RequestStateComboBox );
	ui.m_CreateInviteButton->setFixedSize( eButtonSizeSmall );
	ui.m_CreateInviteButton->setIcon( eMyIconInviteCreate );

	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinRequested( GuiHostJoin * ) ), this, SLOT( slotHostJoinRequested( GuiHostJoin *) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinUpdated( GuiHostJoin* ) ), this, SLOT( slotlHostJoinUpdated( GuiHostJoin * ) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinRemoved( VxGUID&, EHostType ) ), this, SLOT( slotHostJoinRemoved( VxGUID&, EHostType ) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinOfferStateChange( VxGUID&, EHostType, EJoinState ) ), this, SLOT( slotHostJoinOfferStateChange( VxGUID&, EHostType, EJoinState ) ) );
	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinOnlineStatus( GuiHostJoin *, bool ) ), this, SLOT( slotHostJoinOnlineStatus( GuiHostJoin*, bool ) ) );

	connect( ui.m_HostJoinRequestList, SIGNAL( signalAcceptButtonClicked( GuiHostJoinSession *, HostJoinRequestListItem * ) ), this, SLOT( slotAcceptButtonClicked( GuiHostJoinSession*, HostJoinRequestListItem* ) ) );
	connect( ui.m_HostJoinRequestList, SIGNAL( signalRejectButtonClicked( GuiHostJoinSession*, HostJoinRequestListItem* ) ), this, SLOT( slotRejectButtonClicked( GuiHostJoinSession*, HostJoinRequestListItem* ) ) );

	connect( ui.m_RequestStateComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotJoinComboBoxSelectionChange( int ) ) );
	connect( ui.m_CreateInviteButton, SIGNAL( clicked() ), this, SLOT( slotCreateInviteButtonClicked() ) );

	updateJoinList();

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletHostSelect::~AppletHostSelect()
{
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletHostSelect::updateJoinList( void )
{
	ui.m_HostJoinRequestList->clear();
	std::map<VxGUID, GuiHostJoin*>& joinList = m_HostJoinMgr.getHostJoinList();
	for( auto& iter : joinList )
	{
		GuiHostJoin* user = iter.second;
		if( !user->isIgnored() && m_JoinState == user->getJoinState( m_HostType ) )
		{
			updateHostJoinRequest( user );
		}
	}
}

//============================================================================
void AppletHostSelect::slotHostJoinRequested( GuiHostJoin* user )
{
	updateHostJoinRequest( user );
}

//============================================================================
void AppletHostSelect::slotlHostJoinUpdated( GuiHostJoin* user )
{
	updateHostJoinRequest( user );
}

//============================================================================
void AppletHostSelect::slotHostJoinRemoved( VxGUID& onlineId, EHostType hostType )
{
	ui.m_HostJoinRequestList->removeHostJoinRequest( onlineId, hostType );
}

//============================================================================
void AppletHostSelect::slotHostJoinOfferStateChange( VxGUID& userOnlineId, EHostType hostType, EJoinState hostOfferState )
{
	updateJoinList();
}

//============================================================================
void AppletHostSelect::slotHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline )
{
	updateJoinList();
}

//============================================================================
void AppletHostSelect::updateHostJoinRequest( GuiHostJoin* user )
{
	vx_assert( user );
	VxGUID& onlineId = user->getMyOnlineId();
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
void AppletHostSelect::slotAcceptButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	if( joinSession && joinSession->getUserIdent() )
	{
		m_HostJoinMgr.joinAccepted( joinSession->getUserIdent(), joinSession->getHostType() );
	}
}

//============================================================================
void AppletHostSelect::slotRejectButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	m_HostJoinMgr.joinRejected( joinSession->getUserIdent(), joinSession->getHostType() );
}

//============================================================================
void AppletHostSelect::slotJoinComboBoxSelectionChange( int comboIdx )
{
	m_JoinState = GuiHelpers::comboIdxToJoinState( comboIdx );
	updateJoinList();
}

//============================================================================
void AppletHostSelect::slotCreateInviteButtonClicked( void )
{
	m_MyApp.getAppletMgr().launchApplet( eAppletInviteCreate, this );
}
