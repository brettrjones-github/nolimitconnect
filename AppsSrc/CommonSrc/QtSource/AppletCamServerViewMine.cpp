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
// http://www.nolimitconnect.org
//============================================================================

#include "AppletCamServerViewMine.h"

#include "AppCommon.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

//============================================================================
AppletCamServerViewMine::AppletCamServerViewMine( AppCommon& app, QWidget* parent )
: AppletCamClient( app, parent )
{
}

//============================================================================
AppletCamServerViewMine::~AppletCamServerViewMine()
{
	m_MyApp.wantToGuiActivityCallbacks( this, false );
	m_MyApp.activityStateChange( this, false );
}

//============================================================================
void AppletCamServerViewMine::setAppletType( EApplet applet )
{
    AppletCamClient::setAppletType( eAppletCamServerViewMine );
}
