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

#include "AssetXferMgr.h"

#include <PktLib/PktsAssetXfer.h>

//============================================================================
PktBaseGetReq* AssetXferMgr::createPktBaseGetReq( void )
{
    return new PktAssetGetReq();
}

//============================================================================
PktBaseGetReply* AssetXferMgr::createPktBaseGetReply( void )
{
    return new PktAssetGetReply();
}

//============================================================================
PktBaseSendReq* AssetXferMgr::createPktBaseSendReq( void )
{
    return new PktAssetSendReq();
}

//============================================================================
PktBaseSendReply* AssetXferMgr::createPktBaseSendReply( void )
{
    return new PktAssetSendReply();
}

//============================================================================
PktBaseChunkReq* AssetXferMgr::createPktBaseChunkReq( void )
{
    return new PktAssetChunkReq();
}

//============================================================================
PktBaseChunkReply* AssetXferMgr::createPktBaseChunkReply( void )
{
    return new PktAssetChunkReply();
}

//============================================================================
PktBaseGetCompleteReq* AssetXferMgr::createPktBaseGetCompleteReq( void )
{
    return new PktAssetGetCompleteReq();
}

//============================================================================
PktBaseGetCompleteReply* AssetXferMgr::createPktBaseGetCompleteReply( void )
{
    return new PktAssetGetCompleteReply();
}

//============================================================================
PktBaseSendCompleteReq* AssetXferMgr::createPktBaseSendCompleteReq( void )
{
    return new PktAssetSendCompleteReq();
}

//============================================================================
PktBaseSendCompleteReply* AssetXferMgr::createPktBaseSendCompleteReply( void )
{
    return new PktAssetSendCompleteReply();
}

//============================================================================
PktBaseXferErr* AssetXferMgr::createPktBaseXferErr( void )
{
    return new PktAssetXferErr();
}
