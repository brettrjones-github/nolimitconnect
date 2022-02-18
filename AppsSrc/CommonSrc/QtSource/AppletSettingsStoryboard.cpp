//============================================================================
// Copyright (C) 2018 Brett R. Jones
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

#include "AppletSettingsStoryboard.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletSettingsStoryboard::AppletSettingsStoryboard( AppCommon& app, QWidget * parent )
: AppletServiceBaseSettings( OBJNAME_APPLET_SETTINGS_SHARE_STORYBOARD, app, parent )
{
	ui.setupUi( getContentItemsFrame() );
	setAppletType( eAppletSettingsStoryboard );
	setPluginType( ePluginTypeStoryboard );
	setTitleBarText( DescribeApplet( m_EAppletType ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletSettingsStoryboard::~AppletSettingsStoryboard()
{
    m_MyApp.activityStateChange( this, false );
}
