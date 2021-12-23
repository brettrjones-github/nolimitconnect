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

#include "AppletTestHostService.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletTestHostService::AppletTestHostService( AppCommon& app, QWidget * parent )
: AppletTestBase( OBJNAME_APPLET_TEST_HOST_SERVICE, app, parent )
{
	ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletTestHostService );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( close() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletTestHostService::~AppletTestHostService()
{
    m_MyApp.activityStateChange( this, false );
}