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

//============================================================================
AppletTestHostClient::AppletTestHostClient( AppCommon& app, QWidget * parent )
: AppletTestBase( OBJNAME_APPLET_TEST_HOST_CLIENT, app, parent )
{
	ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletTestHostClient );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	GuiHelpers::fillHostType( ui.m_HostTypeComboBox, true );
	
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );
	connect( ui.m_HostTypeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHostTypeComboBoxSelectionChange( int ) ) );
	connect( ui.m_HostUrlComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( slotHostUrlSelectionChange( int ) ) );
	connect( ui.m_QueryButton, SIGNAL( clicked() ), this, SLOT( slotQueryButtonClicked() ) );

	if( ui.m_HostTypeComboBox->count() )
	{
		slotHostTypeComboBoxSelectionChange( 0 );
	}

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletTestHostClient::~AppletTestHostClient()
{
    m_MyApp.activityStateChange( this, false );
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

	ui.m_HostUrlComboBox->clear();
	std::string defaultUrlStr = m_MyApp.getFromGuiInterface().fromGuiQueryDefaultUrl( hostType );
	if( !defaultUrlStr.empty() )
	{
		QString defaultUrl;
		VxGUID onlineId;
		std::string userName = m_MyApp.getFromGuiInterface().fromGuiQueryUrlUserName( defaultUrlStr, onlineId );
		if( !userName.empty() )
		{
			defaultUrl = userName.c_str();
			defaultUrl += " - ";
			defaultUrl += defaultUrlStr.c_str();
		}
		else
		{
			defaultUrl = defaultUrlStr.c_str();
		}

		ui.m_HostUrlComboBox->addItem( defaultUrl );
	}

}