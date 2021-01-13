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
#include <app_precompiled_hdr.h>
#include "AppletServiceRandomConnect.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletServiceRandomConnect::AppletServiceRandomConnect( AppCommon& app, QWidget * parent )
: AppletServiceBase( OBJNAME_APPLET_SERVICE_RANDOM_PERSON, app, parent )
{
    setupServiceBaseApplet( eAppletServiceRandomConnect, ePluginTypeRandomConnectClient );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletServiceRandomConnect::~AppletServiceRandomConnect()
{
    m_MyApp.activityStateChange( this, false );
}
