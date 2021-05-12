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

#include <PktLib/PktsThumbXfer.h>

//============================================================================
PktBaseGetReq* ThumbXferMgr::createPktBaseGetReq( void )
{
    return new PktThumbGetReq();
}

//============================================================================
PktBaseGetReply* ThumbXferMgr::createPktBaseGetReply( void )
{
    return new PktThumbGetReply();
}

//============================================================================
PktBaseSendReq* ThumbXferMgr::createPktBaseSendReq( void )
{
    return new PktThumbSendReq();
}

//============================================================================
PktBaseSendReply* ThumbXferMgr::createPktBaseSendReply( void )
{
    return new PktThumbSendReply();
}

//============================================================================
PktBaseChunkReq* ThumbXferMgr::createPktBaseChunkReq( void )
{
    return new PktThumbChunkReq();
}

//============================================================================
PktBaseChunkReply* ThumbXferMgr::createPktBaseChunkReply( void )
{
    return new PktThumbChunkReply();
}

//============================================================================
PktBaseGetCompleteReq* ThumbXferMgr::createPktBaseGetCompleteReq( void )
{
    return new PktThumbGetCompleteReq();
}

//============================================================================
PktBaseGetCompleteReply* ThumbXferMgr::createPktBaseGetCompleteReply( void )
{
    return new PktThumbGetCompleteReply();
}

//============================================================================
PktBaseSendCompleteReq* ThumbXferMgr::createPktBaseSendCompleteReq( void )
{
    return new PktThumbSendCompleteReq();
}

//============================================================================
PktBaseSendCompleteReply* ThumbXferMgr::createPktBaseSendCompleteReply( void )
{
    return new PktThumbSendCompleteReply();
}

//============================================================================
PktBaseXferErr* ThumbXferMgr::createPktBaseXferErr( void )
{
    return new PktThumbXferErr();
}

//============================================================================

PktBaseListReq* ThumbXferMgr::createPktBaseListReq( void )
{
    return new PktThumbListReq();
}

//============================================================================
PktBaseListReply* ThumbXferMgr::createPktBaseListReply( void )
{
    return new PktThumbListReply();
}

