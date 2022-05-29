//============================================================================
// Copyright (C) 2021 Brett R. Jones
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

#include <config_appcorelibs.h>
#include "OfferHostXferMgr.h"
#include "OfferHostInfo.h"
#include "OfferHostMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "OfferHostTxSession.h"
#include "OfferHostRxSession.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseMgr.h>
#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseInfo.h>
#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseRxSession.h>
#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseTxSession.h>

#include <PktLib/PktsOfferXfer.h>
#include <PktLib/VxCommon.h>
#include <NetLib/VxSktBase.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/AppErr.h>
#include <CoreLib/VxFileUtil.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//============================================================================
OfferHostXferMgr::OfferHostXferMgr( P2PEngine& engine, OfferHostMgr offerMgr, PluginMessenger& plugin, PluginSessionMgr& pluginSessionMgr, const char * stateDbName )
: OfferBaseXferMgr( engine, offerMgr, plugin, pluginSessionMgr, stateDbName, eOfferMgrTypeOfferHost )
{
}
