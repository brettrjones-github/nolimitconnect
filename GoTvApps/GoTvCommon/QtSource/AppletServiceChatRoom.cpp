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
#include "AppletServiceChatRoom.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletServiceChatRoom::AppletServiceChatRoom( AppCommon& app, QWidget * parent )
: AppletServiceBase( OBJNAME_APPLET_SERVICE_CHAT_ROOM, app, parent )
{
    setupServiceBaseApplet( eAppletServiceChatRoom, ePluginTypeChatRoom );

    m_MyApp.activityStateChange( this, true );
}

//============================================================================
AppletServiceChatRoom::~AppletServiceChatRoom()
{
    m_MyApp.activityStateChange( this, false );
}
