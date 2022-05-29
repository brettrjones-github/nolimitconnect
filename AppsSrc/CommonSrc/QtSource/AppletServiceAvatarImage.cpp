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

#include "AppletServiceAvatarImage.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletServiceAvatarImage::AppletServiceAvatarImage( AppCommon& app, QWidget * parent )
: AppletServiceBase( OBJNAME_APPLET_SERVICE_AVATAR_IMAGE, app, parent )
{
    setupServiceBaseApplet( eAppletServiceAvatarImage, ePluginTypeHostPeerUser );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletServiceAvatarImage::~AppletServiceAvatarImage()
{
    m_MyApp.activityStateChange( this, false );
}
