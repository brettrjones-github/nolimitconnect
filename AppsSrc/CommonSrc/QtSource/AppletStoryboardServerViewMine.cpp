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

#include "AppletStoryboardServerViewMine.h"

#include "AppCommon.h"

//============================================================================
AppletStoryboardServerViewMine::AppletStoryboardServerViewMine( AppCommon& app, QWidget* parent )
    : AppletStoryboardClient( app, parent )
{
}

//============================================================================
AppletStoryboardServerViewMine::~AppletStoryboardServerViewMine()
{
    m_MyApp.activityStateChange( this, false );
    m_MyApp.wantToGuiActivityCallbacks( this, false );
}

//============================================================================
void AppletStoryboardServerViewMine::setAppletType( EApplet applet )
{
    AppletStoryboardClient::setAppletType( eAppletStoryboardServerViewMine );
}
