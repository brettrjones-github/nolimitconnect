//============================================================================
// Copyright (C) 2010 Brett R. Jones
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
// http://www.nolimitconnect.com
//============================================================================

#include "PktTypes.h"
#include "PktsAssetXfer.h"

#include <CoreLib/VxDebug.h>

//============================================================================
// PktAssetSendReq
//============================================================================
PktAssetGetReq::PktAssetGetReq()
    : PktBaseGetReq() 
{ 
    setPktType( PKT_TYPE_ASSET_GET_REQ ); 
    setPktLength( sizeof( PktAssetGetReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetGetReply::PktAssetGetReply()
    : PktBaseGetReply()
{ 
    setPktType( PKT_TYPE_ASSET_GET_REPLY ); 
    setPktLength( sizeof( PktAssetGetReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
// PktAssetSendReq
//============================================================================
PktAssetSendReq::PktAssetSendReq()
: PktBaseSendReq() 
{ 
	setPktType( PKT_TYPE_ASSET_SEND_REQ ); 
    setPktLength( sizeof( PktAssetSendReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetSendReply::PktAssetSendReply()
: PktBaseSendReply()
{ 
	setPktType( PKT_TYPE_ASSET_SEND_REPLY ); 
	setPktLength( sizeof( PktAssetSendReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetChunkReq::PktAssetChunkReq()
: PktBaseChunkReq()
{
	setPktType( PKT_TYPE_ASSET_CHUNK_REQ );
	setPktLength( emptyLength() );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetChunkReply::PktAssetChunkReply()
: PktBaseChunkReply()
{
	setPktType( PKT_TYPE_ASSET_CHUNK_REPLY );
	setPktLength( (uint16_t)sizeof( PktAssetChunkReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetGetCompleteReq::PktAssetGetCompleteReq()
    : PktBaseGetCompleteReq()
{
    setPktType(  PKT_TYPE_ASSET_GET_COMPLETE_REQ );
    setPktLength( (uint16_t)sizeof( PktAssetGetCompleteReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );}

//============================================================================
PktAssetGetCompleteReply::PktAssetGetCompleteReply()
    : PktBaseGetCompleteReply()
{
    setPktType( PKT_TYPE_ASSET_GET_COMPLETE_REPLY );
    setPktLength( (uint16_t)sizeof( PktAssetGetCompleteReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetSendCompleteReq::PktAssetSendCompleteReq()
: PktBaseSendCompleteReq()
{
	setPktType(  PKT_TYPE_ASSET_SEND_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktAssetSendCompleteReq ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );}

//============================================================================
PktAssetSendCompleteReply::PktAssetSendCompleteReply()
: PktBaseSendCompleteReply()
{
	setPktType( PKT_TYPE_ASSET_SEND_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktAssetSendCompleteReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetXferErr::PktAssetXferErr()
: PktBaseXferErr() 
{
	setPktType( PKT_TYPE_ASSET_XFER_ERR ); 
	setPktLength( sizeof( PktAssetXferErr ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetFindReq:: PktAssetFindReq()
    : PktBaseFindReq() 
{
    setPktType( PKT_TYPE_ASSET_FIND_REQ ); 
    setPktLength( sizeof(  PktAssetFindReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetListReq:: PktAssetListReq()
    : PktBaseListReq() 
{
    setPktType( PKT_TYPE_ASSET_LIST_REQ ); 
    setPktLength( sizeof(  PktAssetListReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetListReply::PktAssetListReply()
    : PktBaseListReply( )
{
    setPktType( PKT_TYPE_ASSET_LIST_REPLY ); 
    setPktLength( sizeof(  PktAssetListReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}
