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

#include "AppletSettingsHostNetwork.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletSettingsHostNetwork::AppletSettingsHostNetwork( AppCommon& app, QWidget * parent )
: AppletServiceBaseSettings( OBJNAME_APPLET_SETTINGS_HOST_NETWORK, app, parent )
{
    setAppletType( eAppletSettingsHostNetwork );
    setTitleBarText( DescribeApplet( m_EAppletType ) );
    setupApplet();
    loadFromSettings();
    ui.m_PermissionWidget->setPluginType( ePluginTypeHostNetwork );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
void AppletSettingsHostNetwork::setupApplet()
{
}

//============================================================================
void AppletSettingsHostNetwork::loadFromSettings()
{

}

//============================================================================
void AppletSettingsHostNetwork::saveToSettings()
{

}
