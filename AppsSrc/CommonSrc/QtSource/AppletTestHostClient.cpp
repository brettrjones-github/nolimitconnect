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
// http://www.nolimitconnect.org
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

	GuiHelpers::fillHostType( ui.m_HostTypeComboBox, true );
	fillHostList( eHostTypeNetwork, ui.m_NetworkHostComboBox );
	setNetworkHostIdFromNetHostComboBoxSelection();
	
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( closeApplet() ) );
	connect( ui.m_NetworkHostComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotNetworkHostComboBoxSelectionChange( int ) ) );
	connect( ui.m_HostTypeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHostTypeComboBoxSelectionChange( int ) ) );
	connect( ui.m_HostListUrlComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHostListUrlSelectionChange( int ) ) );

	connect( ui.m_GetNetworkHostIdentityButton, SIGNAL( clicked() ), this, SLOT( slotNetworkHostIdentityButtonClicked() ) );
	connect( ui.m_GetNetHostListButton, SIGNAL( clicked() ), this, SLOT( slotQueryHostListFromNetworkHostButtonClicked() ) );
	connect( ui.m_QueryHostedUsersButton, SIGNAL( clicked() ), this, SLOT( slotQueryGroupiesButtonClicked() ) );

	m_MyApp.activityStateChange( this, true );
	m_MyApp.getUserMgr().wantGuiUserUpdateCallbacks( this, true );
	m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, true );

	if( ui.m_NetworkHostComboBox->count() )
	{
		slotNetworkHostComboBoxSelectionChange( 0 );
	}

	if( ui.m_HostTypeComboBox->count() )
	{
		slotHostTypeComboBoxSelectionChange( 0 );
	}
}

//============================================================================
AppletTestHostClient::~AppletTestHostClient()
{
	m_MyApp.getUserMgr().wantGuiUserUpdateCallbacks( this, false );
	m_MyApp.getHostedListMgr().wantHostedListCallbacks( this, false );
    m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletTestHostClient::setNetworkHostIdFromNetHostComboBoxSelection( void )
{
	std::string curNetHostUrl = ui.m_NetworkHostComboBox->currentText().toUtf8().constData();
	if( !curNetHostUrl.empty() )
	{
		VxPtopUrl hostUrl( curNetHostUrl );
		if( hostUrl.isValid() )
		{
			m_NetworkHostOnlineId = hostUrl.getOnlineId();
			GuiUser* netHostUser = m_MyApp.getUserMgr().getOrQueryUser( m_NetworkHostOnlineId );
			if( netHostUser )
			{
				ui.m_NetworkHostIdentWidget->updateIdentity( netHostUser );
			}
		}
	}
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
	setNetworkHostIdFromNetHostComboBoxSelection();
	slotQueryHostListFromNetworkHostButtonClicked();
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
	std::string networkHostUrl = ui.m_NetworkHostComboBox->currentText().toUtf8().constData();
	// strip the host name
	size_t foundOffs = networkHostUrl.find( "ptop:" );
	if( foundOffs == std::string::npos )
	{
		// not found 
		return;
	}

	ui.m_HostListUrlComboBox->clear();
	ui.m_GuiHostedListWidget->clear();
	ui.m_GuiGroupieListWidget->clear();
	std::string netHostUrlStr = networkHostUrl.substr( foundOffs, networkHostUrl.length() );
	if( !netHostUrlStr.empty() )
	{
		EHostType hostType = GuiHelpers::comboIdxToHostType( ui.m_HostTypeComboBox->currentIndex() );
		VxPtopUrl hostUrl( netHostUrlStr );
		if( hostUrl.isValid() && hostType != eHostTypeUnknown )
		{
			VxGUID nullGuid;
			m_MyApp.getFromGuiInterface().fromGuiQueryHostListFromNetworkHost( hostUrl, hostType, nullGuid );
		}
		else
		{
			okMessageBox( "Invalid Url", "Invalid Network Host Url" );
		}
	}
}

//============================================================================
void AppletTestHostClient::callbackGuiHostedListSearchResult( HostedId& hostedId, GuiHosted* guiHosted, VxGUID& sessionId )
{
	if( hostedId.getHostType() == m_HostType && guiHosted )
	{
		if( updateHostedUrlComboBox( guiHosted->getHostInviteUrl() ) )
		{
			updateHostedIdent( guiHosted );
		}

		ui.m_GuiHostedListWidget->updateHostedList( hostedId, guiHosted, sessionId );
	}
}

//============================================================================
void AppletTestHostClient::callbackGuiGroupieListSearchResult( GroupieId& groupieId, GuiGroupie* guiGroupie, VxGUID& sessionId )
{
	if( groupieId.getHostType() == m_HostType && guiGroupie )
	{
		ui.m_GuiGroupieListWidget->updateGroupieList( groupieId, guiGroupie, sessionId );
	}
}

//============================================================================
bool AppletTestHostClient::updateHostedUrlComboBox( std::string& hostUrl )
{
	bool result = 0 == ui.m_HostListUrlComboBox->count();
	QString url = hostUrl.c_str();
	if( !url.isEmpty() )
	{
		ui.m_HostListUrlComboBox->addItem( url );
	}
	
	return result && !url.isEmpty();
}

//============================================================================
void AppletTestHostClient::updateHostedIdent( GuiHosted* guiHosted )
{
	ui.m_HostedIdentWidget->updateIdentity( guiHosted->getUser() );
	ui.m_HostedPluginWidget->updateHosted( guiHosted );
}

//============================================================================
void AppletTestHostClient::slotHostListUrlSelectionChange( int comboIdx )
{
	slotQueryGroupiesButtonClicked();
}

//============================================================================
void AppletTestHostClient::slotQueryGroupiesButtonClicked( void )
{
	std::string hostedListUrl = ui.m_HostListUrlComboBox->currentText().toUtf8().constData();
	if( !hostedListUrl.empty() )
	{
		ui.m_GuiGroupieListWidget->clear();
		VxPtopUrl hostUrl( hostedListUrl );
		EHostType hostType = hostUrl.getHostType();
		if( hostUrl.isValid() && hostType != eHostTypeUnknown )
		{
			VxGUID nullGuid;
			m_MyApp.getFromGuiInterface().fromGuiQueryGroupiesFromHosted( hostUrl, hostType, nullGuid );
		}
		else
		{
			okMessageBox( "Invalid Url", "Invalid Groupie Host Url" );
		}
	}
}
