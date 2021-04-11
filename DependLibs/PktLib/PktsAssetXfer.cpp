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
#include <GoTvCore/GoTvP2P/AssetMgr/AssetInfo.h>
#include <GoTvCore/GoTvP2P/AssetBase/AssetBaseInfo.h>

#include <CoreLib/VxChop.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
// PktAssetSendReq
//============================================================================
PktAssetSendReq::PktAssetSendReq()
: m_AssetType( (uint16_t)eAssetTypeUnknown ) 
, m_u32Error(0)
, m_s64AssetLen(0)
, m_s64AssetOffs(0) 
, m_CreationTime(0)
, m_AssetNameLen(0)
, m_AssetTagLen(0)
, m_u32Res1(0)
, m_u32Res2(0)
, m_u32Res3(0)
, m_u32Res4(0)
{ 
	setPktType( PKT_TYPE_ASSET_SEND_REQ ); 
	m_AssetNameAndTag[0] = 0;
}

//============================================================================
void PktAssetSendReq::fillPktFromAsset( AssetBaseInfo& assetInfo )
{
	setAssetType( (uint16_t)assetInfo.getAssetType() );
	setCreatorId( assetInfo.getCreatorId() );
	setHistoryId( assetInfo.getHistoryId() );
	setUniqueId( assetInfo.getAssetUniqueId() );
	setAssetHashId( assetInfo.getAssetHashId() );
	setAssetLen( assetInfo.getAssetLength() );
	setCreationTime( (uint32_t) assetInfo.getCreationTime() );
	setAssetNameAndTag( assetInfo.getAssetName().c_str(), assetInfo.getAssetTag().c_str() );
}

//============================================================================
void PktAssetSendReq::fillAssetFromPkt( AssetBaseInfo& assetInfo )
{
	assetInfo.setAssetType( (EAssetType)getAssetType() );
	assetInfo.setAssetUniqueId( getUniqueId() );
	assetInfo.setCreatorId( getCreatorId() );
	assetInfo.setHistoryId( getHistoryId() );
	assetInfo.setAssetHashId( getAssetHashId() );
	assetInfo.setAssetLength( getAssetLen() );
	assetInfo.setCreationTime( (time_t)getCreationTime() );
	std::string assetName = getAssetName();
	assetInfo.setAssetName( assetName );
	if( getAssetTagLen() )
	{
		assetInfo.setAssetTag(  &m_AssetNameAndTag[ getAssetNameLen() ] );
	}
}

//============================================================================
std::string PktAssetSendReq::getAssetName()
{
	return m_AssetNameAndTag;
}

//============================================================================
std::string PktAssetSendReq::getAssetTag()
{
	std::string strTag = "";
	if( getAssetTagLen() )
	{
		strTag = &m_AssetNameAndTag[ getAssetNameLen() ];
	}

	return strTag;
}

//============================================================================
void PktAssetSendReq::setAssetNameAndTag( const char * pAssetName, const char * pAssetTag )
{
	int nameLen = (int)strlen( pAssetName );
	if( nameLen > 4095 )
	{
		nameLen = 4095;
		strncpy( m_AssetNameAndTag, pAssetName, nameLen );
		m_AssetNameAndTag[ nameLen ] = 0;
	}
	else
	{
		strcpy( m_AssetNameAndTag, pAssetName );	
	}

	nameLen += 1;
	setAssetNameLen( (uint16_t)nameLen );

	int tagLen = 0;
	if( pAssetTag )
	{
		tagLen = (int)strlen( pAssetTag );
		if( tagLen > 0 )
		{
			if( tagLen > 4095 )
			{
				tagLen = 4095;
				strncpy( &m_AssetNameAndTag[nameLen], pAssetTag, tagLen );
				m_AssetNameAndTag[ nameLen = tagLen ] = 0;
			}
			else
			{
				strcpy( &m_AssetNameAndTag[nameLen], pAssetTag );	
			}

			tagLen += 1;
			setAssetTagLen( (uint16_t)tagLen );
		}
	}

	uint16_t u16PktLen = ( uint16_t )( ( sizeof( PktAssetSendReq ) - sizeof( m_AssetNameAndTag ) ) + nameLen + tagLen );
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
PktAssetSendReply::PktAssetSendReply()
: m_u8RequiresFileXfer( 0 )
, m_u8Res( 0 )
, m_u16Res( 0 )
, m_s64AssetOffs( 0 )
, m_u32Error(0) 
, m_u32Res1( 0 )
, m_u32Res2(0)
{ 
	setPktType( PKT_TYPE_ASSET_SEND_REPLY ); 
	setPktLength( sizeof( PktAssetSendReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetChunkReq::PktAssetChunkReq()
: m_u16Res(0)
, m_u16AssetChunkLen(0)
, m_u32Error(0)
, m_u32Res1(0) 
{
	setPktType( PKT_TYPE_ASSET_CHUNK_REQ );
	setPktLength( emptyLength() );
}

//============================================================================
uint16_t PktAssetChunkReq::emptyLength( void )
{ 
	return (uint16_t)(sizeof( PktAssetChunkReq) - PKT_TYPE_ASSET_MAX_DATA_LEN); 
}

//============================================================================
void PktAssetChunkReq::setChunkLen( uint16_t u16ChunkLen ) 
{ 
	m_u16AssetChunkLen = htons( u16ChunkLen ); 
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLength() + u16ChunkLen ) );
}

//============================================================================
uint16_t PktAssetChunkReq::getChunkLen( void ) 
{ 
	return htons( m_u16AssetChunkLen ); 
}

//============================================================================
PktAssetChunkReply::PktAssetChunkReply()
: m_u16Res(0)
, m_u16AssetChunkLen(0)
, m_u32Error(0) 
{
	setPktType( PKT_TYPE_ASSET_CHUNK_REPLY );
	setPktLength( (uint16_t)sizeof( PktAssetChunkReply ) );
}

//============================================================================
PktAssetSendCompleteReq::PktAssetSendCompleteReq()
: m_u32Error( 0 )
, m_u32Res1( 0 )
{
	setPktType(  PKT_TYPE_ASSET_SEND_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktAssetSendCompleteReq ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );}

//============================================================================
PktAssetSendCompleteReply::PktAssetSendCompleteReply()
: m_u32Error( 0 )
, m_u32Res1( 0 )
{
	setPktType( PKT_TYPE_ASSET_SEND_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktAssetSendCompleteReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktAssetXferErr::PktAssetXferErr()
: m_u16Err(0xffff) 
, m_u16Res1(0)
, m_u32ResP1(0)
, m_u32ResP2(0)
, m_u32ResP3(0)
, m_u32Res1(0) 
, m_u32Res2(0)
{
	setPktType( PKT_TYPE_ASSET_XFER_ERR ); 
	setPktLength( sizeof( PktAssetXferErr ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
const char * PktAssetXferErr::describeError( void )
{
	switch( m_u16Err )
	{
	case PKT_REQ_STATUS_OK:
		return "200 Ok";
	case PKT_REQ_STATUS_CREATED:
		return "201 Created";
	case PKT_REQ_STATUS_ACCEPTED:
		return "202 Accepted";
	case PKT_REQ_ERR_NO_CONTENT:
		return "204 No Content";
	case PKT_REQ_ERR_MOVED_PERM:
		return "301 Moved Permanently";
	case PKT_REQ_ERR_MOVED_TEMP:
		return "301 Moved Temporarily";
	case PKT_REQ_ERR_NOT_MODIFIED:
		return "304 Not Modified";
	case PKT_REQ_ERR_BAD_REQUEST:
		return "400 Bad Request";
	case PKT_REQ_ERR_UNAUTHORIZED:
		return "401 Unauthorized";
	case PKT_REQ_ERR_FORBIDDEN:
		return "403 Forbidden";
	case PKT_REQ_ERR_NOT_FOUND:
		return "404 Not Found";
	case PKT_REQ_ERR_INTERNAL_SERVER_ERR:
		return "500 Internal Server Error";
	case PKT_REQ_ERR_NOT_IMPLEMENTED:
		return "501 Not Implemented";
	case PKT_REQ_ERR_BAD_GATEWAY:
		return "502 Bad Gateway";
	case PKT_REQ_ERR_SERVICE_UNAVAIL:
		return "503 Service Unavailable";
		//custom statuses
	case PKT_REQ_ERR_BANDWITH_LIMIT:
		return "600 Refused because of Bandwidth limit";
	case PKT_REQ_ERR_CONNECT_LIMIT:
		return "601 Refused because of Connection limit";
	case PKT_REQ_ERR_SERVICE_DISABLED:
		return "602	Refused because Service was disabled";
	case PKT_REQ_ERR_ALL_THREADS_BUSY:
		return "602	Refused because all threads are busy";
	default:
		return "Unknown Err";
	}
}


