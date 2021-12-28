//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include "AppletTestHostClient.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "GuiHelpers.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxPtopUrl.h>

//============================================================================
AppletTestHostClient::AppletTestHostClient( AppCommon& app, QWidget * parent )
: AppletTestBase( OBJNAME_APPLET_TEST_HOST_CLIENT, app, parent )
{
	ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletTestHostClient );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	fillHostList( eHostTypeNetwork, ui.m_NetworkHostComboBox );
	GuiHelpers::fillHostType( ui.m_HostTypeComboBox, true );
	
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );
	connect( ui.m_NetworkHostComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotNetworkHostComboBoxSelectionChange( int ) ) );
	connect( ui.m_HostTypeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHostTypeComboBoxSelectionChange( int ) ) );
	connect( ui.m_HostUrlComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHostUrlSelectionChange( int ) ) );
	connect( ui.m_QueryButton, SIGNAL( clicked() ), this, SLOT( slotQueryButtonClicked() ) );
	connect( ui.m_GetNetworkHostIdentityButton, SIGNAL( clicked() ), this, SLOT( slotNetworkHostIdentityButtonClicked() ) );
	connect( ui.m_GetNetHostListButton, SIGNAL( clicked() ), this, SLOT( slotQueryHostListFromNetworkHostButtonClicked() ) );
	connect( ui.m_JoinHostButton, SIGNAL( clicked() ), this, SLOT( slotJoinHostButtonClicked() ) );

    if( ui.m_NetworkHostComboBox->count() )
	{
		slotNetworkHostComboBoxSelectionChange( 0 );
	}

	if( ui.m_HostTypeComboBox->count() )
	{
		slotHostTypeComboBoxSelectionChange( 0 );
	}

	m_MyApp.activityStateChange( this, true );
	m_MyApp.getUserMgr().wantGuiUserMgrGuiUserUpdateCallbacks( this, true );
}

//============================================================================
AppletTestHostClient::~AppletTestHostClient()
{
	m_MyApp.getUserMgr().wantGuiUserMgrGuiUserUpdateCallbacks( this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletTestHostClient::callbackOnUserAdded( GuiUser* guiUser )
{
	if( m_NetworkHostOnlineId == guiUser->getMyOnlineId() )
	{
		ui.m_NetworkHostIdentWidget->updateIdentity( guiUser );
	}
}

//============================================================================
void AppletTestHostClient::callbackOnUserUpdated( GuiUser* guiUser )
{
	if( m_NetworkHostOnlineId == guiUser->getMyOnlineId() )
	{
		ui.m_NetworkHostIdentWidget->updateIdentity( guiUser );
	}
}

//============================================================================
void AppletTestHostClient::slotNetworkHostComboBoxSelectionChange( int comboIdx )
{
}

//============================================================================
void AppletTestHostClient::slotHostTypeComboBoxSelectionChange( int comboIdx )
{
	EHostType hostType = GuiHelpers::comboIdxToHostType( comboIdx );
	if( hostType != eHostTypeUnknown )
	{
		updateHostType( hostType );
	}
}

//============================================================================
void AppletTestHostClient::slotHostUrlSelectionChange( int comboIdx )
{


}

//============================================================================
void AppletTestHostClient::slotQueryButtonClicked( void )
{
	
}

//============================================================================
void AppletTestHostClient::updateHostType( EHostType hostType )
{
	if( hostType == eHostTypeUnknown )
	{
		return;
	}

	m_HostType = hostType;

}

//============================================================================
void AppletTestHostClient::fillHostList( EHostType hostType, QComboBox* comboBox )
{
	comboBox->clear();

	std::string defaultUrlStr = m_MyApp.getFromGuiInterface().fromGuiQueryDefaultUrl( hostType );
	if( !defaultUrlStr.empty() )
	{
		QString defaultUrl;
		VxNetIdent netIdent;
		if( m_MyApp.getFromGuiInterface().fromGuiQueryIdentity( defaultUrlStr, netIdent, false ) )
		{
			defaultUrl = netIdent.getOnlineName();
			defaultUrl += " - ";
			defaultUrl += defaultUrlStr.c_str();
		}
		else
		{
			defaultUrl = defaultUrlStr.c_str();
		}

		comboBox->addItem( defaultUrl );
	}
}

//============================================================================
void AppletTestHostClient::slotNetworkHostIdentityButtonClicked( void )
{
	QString url = ui.m_NetworkHostComboBox->currentText();
	if( !url.isEmpty() )
	{
		VxPtopUrl ptopUrl( url.toUtf8().constData() );
		if( ptopUrl.isValid() )
		{
			m_NetworkHostOnlineId = ptopUrl.getOnlineId();
			VxNetIdent netIdent;
			if( m_MyApp.getEngine().fromGuiQueryIdentity( ptopUrl.getUrl(), netIdent, true ) )
			{
				ui.m_NetworkHostIdentWidget->updateIdentity( &netIdent );
			}
		}
		else
		{
			okMessageBox( QObject::tr( "Invalid URL" ), QObject::tr( "Invalid URL" ) );
		}
	}
}

//============================================================================
void AppletTestHostClient::slotQueryHostListFromNetworkHostButtonClicked( void )
{

}

//============================================================================
void AppletTestHostClient::slotJoinHostButtonClicked( void )
{

}
