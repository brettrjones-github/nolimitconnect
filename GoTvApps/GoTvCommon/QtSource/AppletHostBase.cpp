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
#include "AppletHostBase.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "GuiHostSession.h"

#include <CoreLib/VxDebug.h>

//============================================================================
AppletHostBase::AppletHostBase( const char * objName, AppCommon& app, QWidget * parent )
    : AppletBase( objName, app, parent )
{
}

//============================================================================
void AppletHostBase::onJointButtonClicked( GuiHostSession* hostSession )
{
    if( hostSession )
    {
        VxGUID::generateNewVxGUID( m_JoinSessionId );
        m_JoinHostType = hostSession->getHostType();
        m_JoinHostUrl = hostSession->getHostUrl();
        m_Engine.fromGuiJoinHost( m_JoinHostType, m_JoinSessionId, m_JoinHostUrl.c_str() );
    }
    else
    {
        LogMsg( LOG_ERROR, "AppletHostBase::onJointButtonClicked null hostSession" );
    }
}

