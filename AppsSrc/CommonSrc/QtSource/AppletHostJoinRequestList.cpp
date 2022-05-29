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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletHostJoinRequestList.h"
#include "AppletMgr.h"
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

	//connect( &m_HostJoinMgr, SIGNAL( signalHostJoinRequested( GuiHostJoin * ) ), this, SLOT( slotHostJoinRequested( GuiHostJoin *) ) );
	//connect( &m_HostJoinMgr, SIGNAL( signalHostJoinUpdated( GuiHostJoin* ) ), this, SLOT( slotlHostJoinUpdated( GuiHostJoin * ) ) );
	//connect( &m_HostJoinMgr, SIGNAL( signalHostJoinRemoved( VxGUID&, EHostType ) ), this, SLOT( slotHostJoinRemoved( VxGUID&, EHostType ) ) );
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
	std::map<GroupieId, GuiHostJoin*>& joinList = m_HostJoinMgr.getHostJoinList();
	for( auto& iter : joinList )
	{
		GuiHostJoin* guiHostJoin = iter.second;
		if( guiHostJoin->getUser() && !guiHostJoin->getUser()->isIgnored() && m_JoinState == guiHostJoin->getJoinState() )
		{
			updateHostJoinRequest( guiHostJoin );
		}
	}
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinRequested( GuiHostJoin* guiHostJoin )
{
	updateHostJoinRequest( guiHostJoin );
}

//============================================================================
void AppletHostJoinRequestList::slotlHostJoinUpdated( GuiHostJoin* guiHostJoin )
{
	updateHostJoinRequest( guiHostJoin );
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinRemoved( GroupieId& groupieId )
{
	ui.m_HostJoinRequestList->removeHostJoinRequest( groupieId );
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinOfferStateChange( GroupieId& groupieId, EJoinState hostOfferState )
{
	updateJoinList();
}

//============================================================================
void AppletHostJoinRequestList::slotHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline )
{
	updateJoinList();
}

//============================================================================
void AppletHostJoinRequestList::updateHostJoinRequest( GuiHostJoin* guiHostJoin )
{
	vx_assert( guiHostJoin );
	if( guiHostJoin->getJoinState() == eJoinStateJoinRequested )
	{
		ui.m_HostJoinRequestList->addOrUpdateHostRequest( guiHostJoin );
	}
	else
	{
		ui.m_HostJoinRequestList->removeHostJoinRequest( guiHostJoin->getGroupieId() );
	}
}

//============================================================================
void AppletHostJoinRequestList::slotAcceptButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	if( joinSession && joinSession->getHostJoin() )
	{
		m_HostJoinMgr.joinAccepted( joinSession->getHostJoin() );
	}
}

//============================================================================
void AppletHostJoinRequestList::slotRejectButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	m_HostJoinMgr.joinRejected( joinSession->getHostJoin() );
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