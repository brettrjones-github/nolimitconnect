//============================================================================
// Copyright (C) 2009 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "P2PEngine.h"
#include "P2PConnectList.h"

#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <NetLib/VxSktBase.h>
#include <PktLib/PktsRelay.h>
#include <PktLib/PktTcpPunch.h>

#include <CoreLib/VxParse.h>

//============================================================================
void P2PEngine::onPktRelayUserDisconnect( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogMsg( LOG_VERBOSE, "P2PEngine::onPktRelayUserDisconnect not used" );
}
