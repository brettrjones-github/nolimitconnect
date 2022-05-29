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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletGetStarted.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletGetStarted::AppletGetStarted( AppCommon& app, QWidget * parent )
: AppletBase( OBJNAME_APPLET_GET_STARTED, app, parent )
{
	ui.setupUi( getContentItemsFrame() );
    setAppletType( eAppletGetStarted );
	setTitleBarText( DescribeApplet( m_EAppletType ) );
	connect( this, SIGNAL( signalBackButtonClicked() ), this, SLOT( closeApplet() ) );

	m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletGetStarted::~AppletGetStarted()
{
    m_MyApp.activityStateChange( this, false );
}