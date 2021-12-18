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
// http://www.nolimitconnect.com
//============================================================================

#include "AppletBase.h"
#include "AppCommon.h"
#include "AppSettings.h"
#include "MyIcons.h"
#include "AppCommon.h"
#include "GuiHelpers.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>

//============================================================================
AppletBase::AppletBase( const char * ObjName, AppCommon& app, QWidget * parent )
: ActivityBase( ObjName, app, parent )
, m_ActivityCallbacksEnabled( false )
, m_IsPlaying( false )
, m_SliderIsPressed( false )
{
}

//============================================================================
void AppletBase::setAssetInfo( AssetBaseInfo& assetBaseInfo )
{
    AssetInfo assetInfo( assetBaseInfo );
    m_AssetInfo = assetInfo;
}

//============================================================================
bool AppletBase::isAssetInfoSet( void )
{
    return m_AssetInfo.getAssetUniqueId().isVxGUIDValid();
}

//============================================================================
void AppletBase::slotToGuiAssetAction( EAssetAction assetAction, int pos0to100000 )
{
}
