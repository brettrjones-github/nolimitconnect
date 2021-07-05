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

#include "AssetBaseXferMgr.h"

#include <PktLib/PktsBaseXfer.h>

//============================================================================
PktBaseGetReq* AssetBaseXferMgr::createPktBaseGetReq( void )
{
    return new PktBaseGetReq();
}

//============================================================================
PktBaseGetReply* AssetBaseXferMgr::createPktBaseGetReply( void )
{
    return new PktBaseGetReply();
}

//============================================================================
PktBaseSendReq* AssetBaseXferMgr::createPktBaseSendReq( void )
{
    return new PktBaseSendReq();
}

//============================================================================
PktBaseSendReply* AssetBaseXferMgr::createPktBaseSendReply( void )
{
    return new PktBaseSendReply();
}

//============================================================================
PktBaseChunkReq* AssetBaseXferMgr::createPktBaseChunkReq( void )
{
    return new PktBaseChunkReq();
}

//============================================================================
PktBaseChunkReply* AssetBaseXferMgr::createPktBaseChunkReply( void )
{
    return new PktBaseChunkReply();
}

//============================================================================
PktBaseGetCompleteReq* AssetBaseXferMgr::createPktBaseGetCompleteReq( void )
{
    return new PktBaseGetCompleteReq();
}

//============================================================================
PktBaseGetCompleteReply* AssetBaseXferMgr::createPktBaseGetCompleteReply( void )
{
    return new PktBaseGetCompleteReply();
}

//============================================================================
PktBaseSendCompleteReq* AssetBaseXferMgr::createPktBaseSendCompleteReq( void )
{
    return new PktBaseSendCompleteReq();
}

//============================================================================
PktBaseSendCompleteReply* AssetBaseXferMgr::createPktBaseSendCompleteReply( void )
{
    return new PktBaseSendCompleteReply();
}

//============================================================================
PktBaseXferErr* AssetBaseXferMgr::createPktBaseXferErr( void )
{
    return new PktBaseXferErr();
}

//============================================================================

PktBaseListReq* AssetBaseXferMgr::createPktBaseListReq( void )
{
    return new PktBaseListReq();
}

//============================================================================
PktBaseListReply* AssetBaseXferMgr::createPktBaseListReply( void )
{
    return new PktBaseListReply();
}

