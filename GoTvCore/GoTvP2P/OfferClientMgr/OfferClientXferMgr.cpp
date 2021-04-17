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
// http://www.nolimitconnect.com
//============================================================================

#include <config_gotvcore.h>
#include "OfferClientXferMgr.h"
#include "OfferClientInfo.h"
#include "OfferClientMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "OfferClientTxSession.h"
#include "OfferClientRxSession.h"

#include <GuiInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/OfferBase/OfferBaseMgr.h>

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
OfferClientXferMgr::OfferClientXferMgr( P2PEngine& engine, OfferClientMgr& offerMgr, PluginMessenger& plugin, PluginSessionMgr& pluginSessionMgr, const char * stateDbName )
    : OfferBaseXferMgr( engine, offerMgr, plugin, pluginSessionMgr, stateDbName, eOfferMgrTypeOfferClient )
{
}
