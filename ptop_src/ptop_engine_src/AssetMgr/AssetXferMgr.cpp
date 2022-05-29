//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include <config_appcorelibs.h>
#include "AssetXferMgr.h"
#include "AssetInfo.h"
#include "AssetMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "AssetTxSession.h"
#include "AssetRxSession.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <PktLib/PktsAssetXfer.h>
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
AssetXferMgr::AssetXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName )
: AssetBaseXferMgr( engine, assetMgr, xferInterface, "AssetXferDb.db3", "AssetXferWorkerThrd" )
{
}

//============================================================================
void AssetXferMgr::onPktAssetGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseGetReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseGetReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseChunkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseChunkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void AssetXferMgr::onPktAssetXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_INFO, "AssetXferMgr::onPktAssetXferErr");
    AssetBaseXferMgr::onPktAssetBaseXferErr( sktBase, pktHdr, netIdent );
}

