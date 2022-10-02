//============================================================================
// Copyright (C) 2017 Brett R. Jones
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

#include "AppletPlayerBase.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppCommon.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

//============================================================================
AppletPlayerBase::AppletPlayerBase( const char* ObjName, AppCommon& app, QWidget* parent )
: AppletBase( ObjName, app, parent )
, m_ActivityCallbacksEnabled( false )
, m_IsPlaying( false )
, m_SliderIsPressed( false )
{
	initAppletPlayerBase();
}

//============================================================================
void AppletPlayerBase::initAppletPlayerBase( void )
{
}
