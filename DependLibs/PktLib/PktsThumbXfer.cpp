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

#include "PktTypes.h"
#include "PktsThumbXfer.h"
#include <GoTvCore/GoTvP2P/ThumbMgr/ThumbInfo.h>

#include <CoreLib/VxChop.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
// PktThumbSendReq
//============================================================================
PktThumbSendReq::PktThumbSendReq()
{ 
	setPktType( PKT_TYPE_THUMB_SEND_REQ );
    setPktLength( sizeof(PktThumbSendReq) );
	m_ThumbNameAndTag[0] = 0;
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktThumbSendReq::fillPktFromThumb( ThumbInfo& hostInfo )
{
	setThumbType( (uint16_t)hostInfo.getAssetType() );
    setAttributeFlags( hostInfo.getAttributeFlags() );
    setCreatorId( hostInfo.getCreatorId() );
	setHistoryId( hostInfo.getHistoryId() );
	setUniqueId( hostInfo.getAssetUniqueId() );
    setThumbId( hostInfo.getThumbId() );
	setThumbHashId( hostInfo.getAssetHashId() );
	setThumbLen( hostInfo.getAssetLength() );
	setCreationTime( (uint64_t) hostInfo.getCreationTime() );
    setModifiedTime( (uint64_t) hostInfo.getModifiedTime() );
    setAccessedTime( (uint64_t) hostInfo.getAccessedTime() );
	setThumbNameAndTag( hostInfo.getAssetName().c_str(), hostInfo.getAssetTag().c_str() );
}

//============================================================================
void PktThumbSendReq::fillThumbFromPkt( ThumbInfo& hostInfo )
{
	hostInfo.setAssetType( (EAssetType)getThumbType() );
    hostInfo.setAttributeFlags( getAttributeFlags() );
	hostInfo.setAssetUniqueId( getUniqueId() );
	hostInfo.setCreatorId( getCreatorId() );
	hostInfo.setHistoryId( getHistoryId() );
    hostInfo.setThumbId( getThumbId() );
	hostInfo.setAssetHashId( getThumbHashId() );
	hostInfo.setAssetLength( getThumbLen() );
	hostInfo.setCreationTime( (uint64_t)getCreationTime() );
    hostInfo.setModifiedTime( (uint64_t)getCreationTime() );
    hostInfo.setAccessedTime( (uint64_t)getAccessedTime() );
	std::string assetName = getThumbName();
	hostInfo.setAssetName( assetName );
	if( getThumbTagLen() )
	{
		hostInfo.setAssetTag(  &m_ThumbNameAndTag[ getThumbNameLen() ] );
	}
}

//============================================================================
std::string PktThumbSendReq::getThumbName()
{
	return m_ThumbNameAndTag;
}

//============================================================================
std::string PktThumbSendReq::getThumbTag()
{
	std::string strTag = "";
	if( getThumbTagLen() )
	{
		strTag = &m_ThumbNameAndTag[ getThumbNameLen() ];
	}

	return strTag;
}

//============================================================================
void PktThumbSendReq::setThumbNameAndTag( const char * pThumbName, const char * pThumbTag )
{
	int nameLen = (int)strlen( pThumbName );
	if( nameLen > 4095 )
	{
		nameLen = 4095;
		strncpy( m_ThumbNameAndTag, pThumbName, nameLen );
		m_ThumbNameAndTag[ nameLen ] = 0;
	}
	else
	{
		strcpy( m_ThumbNameAndTag, pThumbName );	
	}

	nameLen += 1;
	setThumbNameLen( (uint16_t)nameLen );

	int tagLen = 0;
	if( pThumbTag )
	{
		tagLen = (int)strlen( pThumbTag );
		if( tagLen > 0 )
		{
			if( tagLen > 4095 )
			{
				tagLen = 4095;
				strncpy( &m_ThumbNameAndTag[nameLen], pThumbTag, tagLen );
				m_ThumbNameAndTag[ nameLen = tagLen ] = 0;
			}
			else
			{
				strcpy( &m_ThumbNameAndTag[nameLen], pThumbTag );	
			}

			tagLen += 1;
			setThumbTagLen( (uint16_t)tagLen );
		}
	}

	uint16_t u16PktLen = ( uint16_t )( ( sizeof( PktThumbSendReq ) - sizeof( m_ThumbNameAndTag ) ) + nameLen + tagLen );
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
PktThumbSendReply::PktThumbSendReply()
{ 
	setPktType( PKT_TYPE_THUMB_SEND_REPLY );
	setPktLength( sizeof( PktThumbSendReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktThumbChunkReq::PktThumbChunkReq()
{
	setPktType( PKT_TYPE_THUMB_CHUNK_REQ );
	setPktLength( emptyLength() );
}

//============================================================================
uint16_t PktThumbChunkReq::emptyLength( void )
{ 
	return (uint16_t)(sizeof( PktThumbChunkReq) - PKT_TYPE_THUMB_MAX_DATA_LEN);
}

//============================================================================
void PktThumbChunkReq::setChunkLen( uint16_t u16ChunkLen ) 
{ 
	m_u16ThumbChunkLen = htons( u16ChunkLen ); 
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLength() + u16ChunkLen ) );
}

//============================================================================
uint16_t PktThumbChunkReq::getChunkLen( void ) 
{ 
	return htons( m_u16ThumbChunkLen ); 
}

//============================================================================
PktThumbChunkReply::PktThumbChunkReply()
: m_u16Res(0)
, m_u16ThumbChunkLen(0)
, m_u32Error(0) 
{
	setPktType( PKT_TYPE_THUMB_CHUNK_REPLY );
	setPktLength( (uint16_t)sizeof( PktThumbChunkReply ) );
}

//============================================================================
PktThumbSendCompleteReq::PktThumbSendCompleteReq()
{
	setPktType(  PKT_TYPE_THUMB_SEND_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktThumbSendCompleteReq ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );}

//============================================================================
PktThumbSendCompleteReply::PktThumbSendCompleteReply()
{
	setPktType( PKT_TYPE_THUMB_SEND_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktThumbSendCompleteReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktThumbXferErr::PktThumbXferErr()
{
	setPktType( PKT_TYPE_THUMB_XFER_ERR );
	setPktLength( sizeof( PktThumbXferErr ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
const char * PktThumbXferErr::describeError( uint16_t error )
{
	switch( error )
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


