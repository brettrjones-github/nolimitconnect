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
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "AppletGroupHost.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletGroupHost::AppletGroupHost( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_GROUP_HOST, app, parent )
{
	m_EAppletType = eAppletGroupHost;
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletGroupHost::~AppletGroupHost()
{
}