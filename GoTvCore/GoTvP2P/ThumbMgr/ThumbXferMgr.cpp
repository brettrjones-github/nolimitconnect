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

#include "ThumbXferMgr.h"
#include "ThumbInfo.h"
#include "ThumbMgr.h"

#include "../Plugins/PluginBase.h"
#include "../Plugins/PluginMgr.h"
#include "../Plugins/PluginMessenger.h"
#include "ThumbTxSession.h"
#include "ThumbRxSession.h"

#include <GuiInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/AssetBase/AssetBaseMgr.h>
#include <GoTvCore/GoTvP2P/AssetMgr/AssetMgr.h>

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
ThumbXferMgr::ThumbXferMgr( P2PEngine& engine, AssetBaseMgr& assetMgr, BaseXferInterface& xferInterface, const char * stateDbName, const char * workThreadName )
    : AssetBaseXferMgr( engine, assetMgr, xferInterface, stateDbName, workThreadName )
{
}

//============================================================================
void ThumbXferMgr::onPktThumbGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{

}

//============================================================================
void ThumbXferMgr::onPktThumbGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{

}

//============================================================================
void ThumbXferMgr::onPktThumbSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseChunkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseChunkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseGetCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseGetCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendCompleteReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    AssetBaseXferMgr::onPktAssetBaseSendCompleteReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void ThumbXferMgr::onPktThumbXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "ThumbXferMgr::onPktThumbXferErr");
    AssetBaseXferMgr::onPktAssetBaseXferErr( sktBase, pktHdr, netIdent );
}

//============================================================================
bool ThumbXferMgr::requestPluginThumb( VxNetIdent* netIdent, VxGUID& thumbId )
{
    if( !netIdent || !thumbId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ThumbXferMgr::requestPluginThumb invalid param" );
        vx_assert( false );
        return false;
    }


    ThumbInfo thumbInfo( netIdent->getMyOnlineId(), thumbId, eAssetTypeThumbnail );

    return fromGuiRequestAssetBase( netIdent, thumbInfo );
}