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


#include "AppletHostSelect.h"
#include "GuiHostJoinMgr.h"
#include "GuiHostJoin.h"
#include "GuiHostJoinSession.h"

#include "AppCommon.h"
#include "AppSettings.h"
#include "AppletMgr.h"
#include "GuiHelpers.h"	
#include "GuiParams.h"

#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletHostSelect::AppletHostSelect( AppCommon& app,  QWidget* parent )
: AppletBase( OBJNAME_APPLET_HOST_SELECT, app, parent )
, m_HostJoinMgr( app.getHostJoinMgr() )
{
    setAppletType( eAppletHostSelect );
    ui.setupUi( getContentItemsFrame() );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
	GuiHelpers::fillJoinRequest( ui.m_RequestStateComboBox );
	ui.m_CreateInviteButton->setFixedSize( eButtonSizeSmall );
	ui.m_CreateInviteButton->setIcon( eMyIconInviteCreate );

	connect( &m_HostJoinMgr, SIGNAL( signalHostJoinOfferStateChange( GroupieId&, EJoinState ) ), this, SLOT( slotHostJoinOfferStateChange( GroupieId&, EJoinState ) ) );
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
	std::map<GroupieId, GuiHostJoin*>& joinList = m_HostJoinMgr.getHostJoinList();
	for( auto& iter : joinList )
	{
		GuiHostJoin* user = iter.second;
		if( !user->getUser()->isIgnored() && m_JoinState == user->getJoinState() )
		{
			updateHostJoinRequest( user );
		}
	}
}

//============================================================================
void AppletHostSelect::slotHostJoinRequested( GuiHostJoin* guiHostJoin )
{
	updateHostJoinRequest( guiHostJoin );
}

//============================================================================
void AppletHostSelect::slotlHostJoinUpdated( GuiHostJoin* guiHostJoin )
{
	updateHostJoinRequest( guiHostJoin );
}

//============================================================================
void AppletHostSelect::slotHostJoinRemoved( GroupieId& groupieId )
{
	ui.m_HostJoinRequestList->removeHostJoinRequest( groupieId );
}

//============================================================================
void AppletHostSelect::slotHostJoinOfferStateChange( GroupieId& groupieId, EJoinState hostOfferState )
{
	updateJoinList();
}

//============================================================================
void AppletHostSelect::slotHostJoinOnlineStatus( GuiHostJoin* user, bool isOnline )
{
	updateJoinList();
}

//============================================================================
void AppletHostSelect::updateHostJoinRequest( GuiHostJoin* guiHostJoin )
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
void AppletHostSelect::slotAcceptButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	if( joinSession && joinSession->getHostJoin() )
	{
		m_HostJoinMgr.joinAccepted( joinSession->getHostJoin() );
	}
}

//============================================================================
void AppletHostSelect::slotRejectButtonClicked( GuiHostJoinSession* joinSession, HostJoinRequestListItem* joinItem )
{
	m_HostJoinMgr.joinRejected( joinSession->getHostJoin() );
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
