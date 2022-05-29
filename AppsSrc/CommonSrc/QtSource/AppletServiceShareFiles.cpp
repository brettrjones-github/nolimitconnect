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

#include "AppletServiceShareFiles.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletServiceShareFiles::AppletServiceShareFiles( AppCommon& app, QWidget * parent )
: AppletServiceBase( OBJNAME_APPLET_SERVICE_SHARE_FILES, app, parent )
{
    setupServiceBaseApplet( eAppletServiceShareFiles, ePluginTypeFileShareServer );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletServiceShareFiles::~AppletServiceShareFiles()
{
    m_MyApp.activityStateChange( this, false );
}
