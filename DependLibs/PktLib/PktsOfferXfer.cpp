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
#include "PktsOfferXfer.h"
#include <GoTvCore/GoTvP2P/OfferBase/OfferBaseInfo.h>

#include <GuiInterface/IDefs.h>
#include <CoreLib/VxChop.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
// PktOfferSendReq
//============================================================================
PktOfferSendReq::PktOfferSendReq()
: m_OfferType( (uint16_t)eOfferTypeUnknown ) 
, m_u32Error(0)
, m_s64OfferLen(0)
, m_s64OfferOffs(0) 
, m_CreationTime(0)
, m_OfferNameLen(0)
, m_OfferTagLen(0)
, m_u32Res1(0)
, m_u32Res2(0)
, m_u32Res3(0)
, m_u32Res4(0)
{ 
	setPktType( PKT_TYPE_OFFER_SEND_REQ ); 
	m_OfferNameAndTag[0] = 0;
}

//============================================================================
void PktOfferSendReq::fillPktFromOffer( OfferBaseInfo& assetInfo )
{
	setOfferType( (uint16_t)assetInfo.getOfferType() );
	setCreatorId( assetInfo.getCreatorId() );
	setHistoryId( assetInfo.getHistoryId() );
	setOfferId( assetInfo.getOfferId() );
	setOfferHashId( assetInfo.getOfferHashId() );
	setOfferLen( assetInfo.getOfferLength() );
	setCreationTime( (uint32_t) assetInfo.getCreationTime() );
	setOfferNameAndTag( assetInfo.getOfferName().c_str(), assetInfo.getOfferTag().c_str() );
}

//============================================================================
void PktOfferSendReq::fillOfferFromPkt( OfferBaseInfo& assetInfo )
{
	assetInfo.setOfferType( (EOfferType)getOfferType() );
	assetInfo.setOfferId( getOfferId() );
	assetInfo.setCreatorId( getCreatorId() );
	assetInfo.setHistoryId( getHistoryId() );
	assetInfo.setOfferHashId( getOfferHashId() );
	assetInfo.setOfferLength( getOfferLen() );
	assetInfo.setCreationTime( (time_t)getCreationTime() );
	std::string assetName = getOfferName();
	assetInfo.setOfferName( assetName );
	if( getOfferTagLen() )
	{
		assetInfo.setOfferTag(  &m_OfferNameAndTag[ getOfferNameLen() ] );
	}
}

//============================================================================
std::string PktOfferSendReq::getOfferName()
{
	return m_OfferNameAndTag;
}

//============================================================================
std::string PktOfferSendReq::getOfferTag()
{
	std::string strTag = "";
	if( getOfferTagLen() )
	{
		strTag = &m_OfferNameAndTag[ getOfferNameLen() ];
	}

	return strTag;
}

//============================================================================
void PktOfferSendReq::setOfferNameAndTag( const char * pOfferName, const char * pOfferTag )
{
	int nameLen = (int)strlen( pOfferName );
	if( nameLen > 4095 )
	{
		nameLen = 4095;
		strncpy( m_OfferNameAndTag, pOfferName, nameLen );
		m_OfferNameAndTag[ nameLen ] = 0;
	}
	else
	{
		strcpy( m_OfferNameAndTag, pOfferName );	
	}

	nameLen += 1;
	setOfferNameLen( (uint16_t)nameLen );

	int tagLen = 0;
	if( pOfferTag )
	{
		tagLen = (int)strlen( pOfferTag );
		if( tagLen > 0 )
		{
			if( tagLen > 4095 )
			{
				tagLen = 4095;
				strncpy( &m_OfferNameAndTag[nameLen], pOfferTag, tagLen );
				m_OfferNameAndTag[ nameLen = tagLen ] = 0;
			}
			else
			{
				strcpy( &m_OfferNameAndTag[nameLen], pOfferTag );	
			}

			tagLen += 1;
			setOfferTagLen( (uint16_t)tagLen );
		}
	}

	uint16_t u16PktLen = ( uint16_t )( ( sizeof( PktOfferSendReq ) - sizeof( m_OfferNameAndTag ) ) + nameLen + tagLen );
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
PktOfferSendReply::PktOfferSendReply()
: m_u8RequiresFileXfer( 0 )
, m_u8Res( 0 )
, m_u16Res( 0 )
, m_s64OfferOffs( 0 )
, m_u32Error(0) 
, m_u32Res1( 0 )
, m_u32Res2(0)
{ 
	setPktType( PKT_TYPE_OFFER_SEND_REPLY ); 
	setPktLength( sizeof( PktOfferSendReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktOfferChunkReq::PktOfferChunkReq()
: m_u16Res(0)
, m_u16OfferChunkLen(0)
, m_u32Error(0)
, m_u32Res1(0) 
{
	setPktType( PKT_TYPE_OFFER_CHUNK_REQ );
	setPktLength( emptyLength() );
}

//============================================================================
uint16_t PktOfferChunkReq::emptyLength( void )
{ 
	return (uint16_t)(sizeof( PktOfferChunkReq) - PKT_TYPE_OFFER_MAX_DATA_LEN); 
}

//============================================================================
void PktOfferChunkReq::setChunkLen( uint16_t u16ChunkLen ) 
{ 
	m_u16OfferChunkLen = htons( u16ChunkLen ); 
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLength() + u16ChunkLen ) );
}

//============================================================================
uint16_t PktOfferChunkReq::getChunkLen( void ) 
{ 
	return htons( m_u16OfferChunkLen ); 
}

//============================================================================
PktOfferChunkReply::PktOfferChunkReply()
: m_u16Res(0)
, m_u16OfferChunkLen(0)
, m_u32Error(0) 
{
	setPktType( PKT_TYPE_OFFER_CHUNK_REPLY );
	setPktLength( (uint16_t)sizeof( PktOfferChunkReply ) );
}

//============================================================================
PktOfferSendCompleteReq::PktOfferSendCompleteReq()
: m_u32Error( 0 )
, m_u32Res1( 0 )
{
	setPktType(  PKT_TYPE_OFFER_SEND_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktOfferSendCompleteReq ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );}

//============================================================================
PktOfferSendCompleteReply::PktOfferSendCompleteReply()
: m_u32Error( 0 )
, m_u32Res1( 0 )
{
	setPktType( PKT_TYPE_OFFER_SEND_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktOfferSendCompleteReply ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktOfferXferErr::PktOfferXferErr()
: m_u16Err(0xffff) 
, m_u16Res1(0)
, m_u32ResP1(0)
, m_u32ResP2(0)
, m_u32ResP3(0)
, m_u32Res1(0) 
, m_u32Res2(0)
{
	setPktType( PKT_TYPE_OFFER_XFER_ERR ); 
	setPktLength( sizeof( PktOfferXferErr ) );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
const char * PktOfferXferErr::describeError( void )
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


