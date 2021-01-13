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

#include "PluginRandomConnectClient.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginRandomConnectClient::PluginRandomConnectClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBaseHostClient( engine, pluginMgr, myIdent )
{
    setPluginType( ePluginTypeRandomConnectClient );
}
