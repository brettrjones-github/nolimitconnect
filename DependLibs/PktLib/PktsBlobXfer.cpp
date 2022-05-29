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

#include "PktTypes.h"
#include "PktsBlobXfer.h"
#include <ptop_src/ptop_engine_src/BlobXferMgr/BlobInfo.h>

#include <CoreLib/VxChop.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
// PktBlobSendReq
//============================================================================
PktBlobSendReq::PktBlobSendReq()
{ 
	setPktType( PKT_TYPE_BLOB_SEND_REQ );
    setPktLength( sizeof(PktBlobSendReq) );
	m_BlobNameAndTag[0] = 0;
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktBlobSendReq::fillPktFromBlob( BlobInfo& hostInfo )
{
	setBlobType( (uint16_t)hostInfo.getAssetType() );
    setAttributeFlags( hostInfo.getAttributeFlags() );
    setCreatorId( hostInfo.getCreatorId() );
	setHistoryId( hostInfo.getHistoryId() );
	setUniqueId( hostInfo.getAssetUniqueId() );
    setThumbId( hostInfo.getThumbId() );
	setBlobHashId( hostInfo.getAssetHashId() );
	setBlobLen( hostInfo.getAssetLength() );
	setCreationTime( (uint64_t) hostInfo.getCreationTime() );
    setModifiedTime( (uint64_t) hostInfo.getModifiedTime() );
    setAccessedTime( (uint64_t) hostInfo.getAccessedTime() );
	setBlobNameAndTag( hostInfo.getAssetName().c_str(), hostInfo.getAssetTag().c_str() );
}

//============================================================================
void PktBlobSendReq::fillBlobFromPkt( BlobInfo& hostInfo )
{
	hostInfo.setAssetType( (EAssetType)getBlobType() );
    hostInfo.setAttributeFlags( getAttributeFlags() );
	hostInfo.setAssetUniqueId( getUniqueId() );
	hostInfo.setCreatorId( getCreatorId() );
	hostInfo.setHistoryId( getHistoryId() );
    hostInfo.setThumbId( getThumbId() );
	hostInfo.setAssetHashId( getBlobHashId() );
	hostInfo.setAssetLength( getBlobLen() );
	hostInfo.setCreationTime( (uint64_t)getCreationTime() );
    hostInfo.setModifiedTime( (uint64_t)getCreationTime() );
    hostInfo.setAccessedTime( (uint64_t)getAccessedTime() );
	std::string assetName = getBlobName();
	hostInfo.setAssetName( assetName );
	if( getBlobTagLen() )
	{
		hostInfo.setAssetTag(  &m_BlobNameAndTag[ getBlobNameLen() ] );
	}
}

//============================================================================
std::string PktBlobSendReq::getBlobName()
{
	return m_BlobNameAndTag;
}

//============================================================================
std::string PktBlobSendReq::getBlobTag()
{
	std::string strTag = "";
	if( getBlobTagLen() )
	{
		strTag = &m_BlobNameAndTag[ getBlobNameLen() ];
	}

	return strTag;
}

//============================================================================
void PktBlobSendReq::setBlobNameAndTag( const char * pBlobName, const char * pBlobTag )
{
	int nameLen = (int)strlen( pBlobName );
	if( nameLen > 4095 )
	{
		nameLen = 4095;
		strncpy( m_BlobNameAndTag, pBlobName, nameLen );
		m_BlobNameAndTag[ nameLen ] = 0;
	}
	else
	{
		strcpy( m_BlobNameAndTag, pBlobName );	
	}

	nameLen += 1;
	setBlobNameLen( (uint16_t)nameLen );

	int tagLen = 0;
	if( pBlobTag )
	{
		tagLen = (int)strlen( pBlobTag );
		if( tagLen > 0 )
		{
			if( tagLen > 4095 )
			{
				tagLen = 4095;
				strncpy( &m_BlobNameAndTag[nameLen], pBlobTag, tagLen );
				m_BlobNameAndTag[ nameLen = tagLen ] = 0;
			}
			else
			{
				strcpy( &m_BlobNameAndTag[nameLen], pBlobTag );	
			}

			tagLen += 1;
			setBlobTagLen( (uint16_t)tagLen );
		}
	}

	uint16_t u16PktLen = ( uint16_t )( ( sizeof( PktBlobSendReq ) - sizeof( m_BlobNameAndTag ) ) + nameLen + tagLen );
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
PktBlobSendReply::PktBlobSendReply()
{ 
	setPktType( PKT_TYPE_BLOB_SEND_REPLY );
	setPktLength( sizeof( PktBlobSendReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktBlobChunkReq::PktBlobChunkReq()
{
	setPktType( PKT_TYPE_BLOB_CHUNK_REQ );
	setPktLength( emptyLength() );
}

//============================================================================
uint16_t PktBlobChunkReq::emptyLength( void )
{ 
	return (uint16_t)(sizeof( PktBlobChunkReq) - PKT_TYPE_BLOB_MAX_DATA_LEN);
}

//============================================================================
void PktBlobChunkReq::setChunkLen( uint16_t u16ChunkLen ) 
{ 
	m_u16BlobChunkLen = htons( u16ChunkLen ); 
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLength() + u16ChunkLen ) );
}

//============================================================================
uint16_t PktBlobChunkReq::getChunkLen( void ) 
{ 
	return htons( m_u16BlobChunkLen ); 
}

//============================================================================
PktBlobChunkReply::PktBlobChunkReply()
: m_u16Res(0)
, m_u16BlobChunkLen(0)
, m_u32Error(0) 
{
	setPktType( PKT_TYPE_BLOB_CHUNK_REPLY );
	setPktLength( (uint16_t)sizeof( PktBlobChunkReply ) );
}

//============================================================================
PktBlobSendCompleteReq::PktBlobSendCompleteReq()
{
	setPktType(  PKT_TYPE_BLOB_SEND_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktBlobSendCompleteReq ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );}

//============================================================================
PktBlobSendCompleteReply::PktBlobSendCompleteReply()
{
	setPktType( PKT_TYPE_BLOB_SEND_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktBlobSendCompleteReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktBlobXferErr::PktBlobXferErr()
{
	setPktType( PKT_TYPE_BLOB_XFER_ERR );
	setPktLength( sizeof( PktBlobXferErr ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
const char * PktBlobXferErr::describeError( uint16_t error )
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


