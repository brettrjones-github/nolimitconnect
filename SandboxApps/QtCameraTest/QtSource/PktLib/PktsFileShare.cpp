//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include "PktTypes.h"
#include "PktsFileShare.h"

#include <CoreLib/VxChop.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <string.h>

//============================================================================
PktFileGetReq::PktFileGetReq()
: m_s64StartOffs(0)
, m_s64EndOffs(0)
, m_u32Res1(0)
, m_u32Res2(0)
{
   setPktType( PKT_TYPE_FILE_GET_REQ );
}

//============================================================================
void PktFileGetReq::setFileName( std::string &csName )
{
	if( csName.size() )
	{
		strcpy( (char *)m_FileName, csName.c_str() );
	}
	else
	{
		m_FileName[ 0 ] = 0; 
	}

	uint16_t u16PktLen = ( uint16_t )((sizeof( PktFileGetReq ) - sizeof( m_FileName )) + strlen( m_FileName ) + 1);
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
void PktFileGetReq::getFileName( std::string &csName )
{
	csName = m_FileName;
}

//============================================================================
PktFileGetReply::PktFileGetReply()
: m_s64StartOffs(0)
, m_s64EndOffs(0)
, m_u32Error(0) 
, m_u32Res2(0)
{
	setPktType( PKT_TYPE_FILE_GET_REPLY );
}

//============================================================================
void PktFileGetReply::getFileName( std::string &csName )
{
	csName = m_FileName;
}

//============================================================================
void PktFileGetReply::setFileName( std::string &csName )
{
    if( csName.size() )
    {
		strcpy( (char *)m_FileName, csName.c_str() );
    }
    else
    {
        m_FileName[ 0 ] = 0;
    }

	uint16_t u16PktLen = ( uint16_t)(( sizeof( PktFileGetReply ) - sizeof( m_FileName )) + strlen( m_FileName ) + 1);
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
uint16_t PktFileGetReply::getemptyLen( void )
{
    return (uint16_t)(sizeof( PktFileGetReq )-VX_MAX_PATH);
}

//============================================================================
// PktFileSendReq
//============================================================================
PktFileSendReq::PktFileSendReq()
: m_u16State(1) 
, m_u8XferCmd(0)
, m_u8XferOption(0)
, m_u8FileType(0)
, m_u8Res1(0)
, m_u16Res1(0)
, m_s64FileOffs(0) 
, m_s64FileLen(0)
, m_u32Error(0) 
, m_u32Res2(0)
{ 
	setPktType( PKT_TYPE_FILE_SEND_REQ ); 
	setFileName( "" );
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}


//============================================================================
void PktFileSendReq::setFileName( const char * pFileName )
{
	strcpy( (char *)m_FileName, pFileName );
	uint16_t u16PktLen = (uint16_t)((sizeof( PktFileSendReq ) - sizeof( m_FileName )) + strlen( m_FileName ) + 1);
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}

//============================================================================
PktFileSendReply::PktFileSendReply()
: m_u16State(1) 
, m_u8XferCmd(0)
, m_u8XferOption(0)
, m_u32Res(0)
, m_s64FileOffs(0) 
, m_s64FileLen(0)
, m_u32Error(0) 
, m_u32Res2(0)
{ 
	setPktType( PKT_TYPE_FILE_SEND_REPLY ); 
	setPktLength( sizeof( PktFileSendReq ) );
	m_FileName[0] = 0;
	vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktFileSendReply::setFileName( const char * pFileName )
{
	strcpy( (char *)m_FileName, pFileName );
	uint16_t u16PktLen = (uint16_t)((sizeof( PktFileSendReply ) - sizeof( m_FileName )) + strlen( m_FileName ) + 1);
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( u16PktLen ) );
}


//============================================================================
// PktFileFindReq
//============================================================================
PktFileFindReq::PktFileFindReq()
: m_u16FileFlags(0) 
, m_u16SizeLimitType(0) 
, m_u32ResP1(0)
, m_s64FileLen(0)
, m_u32Res1(0) 
, m_u32Res2(0)
{ 
	setPktLength( getEmptyLen() ); 
	setPktType(  PKT_TYPE_FILE_FIND_REQ ); 
	m_MatchName[ 0 ] = 0;
}

//============================================================================
void PktFileFindReq::SetMatchName( std::string &csName )
{
    if( ( PKT_SHARE_FIND_FILE_MATCHNAME_MAX_LEN - 1) > csName.size() )
	{
		strcpy( m_MatchName, (const char *)csName.c_str() );
		setPktLength( ROUND_TO_16BYTE_BOUNDRY( getEmptyLen()  + csName.size() + 1 ) );
	}
	else
	{
		m_MatchName[ 0 ] = 0; 
		setPktLength( getEmptyLen() );
	}
}

//============================================================================
void PktFileFindReq::GetMatchName( std::string &csName )
{
	if( getEmptyLen() >= getPktLength() )
	{
		vx_assert( false );
		csName = "";
	}
	else
	{
		char as8Buf[ PKT_SHARE_FIND_FILE_MATCHNAME_MAX_LEN ];
		VxUnchopStr( (unsigned char *)m_MatchName, as8Buf );
		csName = as8Buf;
	}
}

//============================================================================
PktFileChunkReq::PktFileChunkReq()
: m_u16Res(0)
, m_u16FileChunkLen(0)
, m_u32Error(0)
, m_u32Res1(0) 
{
	setPktType( PKT_TYPE_FILE_CHUNK_REQ );
	setPktLength( emptyLength() );
}

//============================================================================
uint16_t PktFileChunkReq::emptyLength( void )
{ 
	return (uint16_t)(sizeof( PktFileChunkReq) - PKT_TYPE_FILE_MAX_DATA_LEN); 
}

//============================================================================
void PktFileChunkReq::setChunkLen( uint16_t u16ChunkLen ) 
{ 
	m_u16FileChunkLen = htons( u16ChunkLen ); 
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( emptyLength() + u16ChunkLen ) );
}

//============================================================================
uint16_t PktFileChunkReq::getChunkLen( void ) 
{ 
	return htons( m_u16FileChunkLen ); 
}

//============================================================================
PktFileChunkReply::PktFileChunkReply()
: m_u16Res(0)
, m_u16FileChunkLen(0)
, m_u32Error(0) 
{
	setPktType( PKT_TYPE_FILE_CHUNK_REPLY );
	setPktLength( (uint16_t)sizeof( PktFileChunkReply ) );
}

//============================================================================
PktFileSendCompleteReq::PktFileSendCompleteReq()
: m_u32Error(0)
{
	setPktType(  PKT_TYPE_FILE_SEND_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktFileSendCompleteReq ) );
}

//============================================================================
PktFileSendCompleteReply::PktFileSendCompleteReply()
: m_u32Error(0)
{
	setPktType( PKT_TYPE_FILE_SEND_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktFileSendCompleteReply ) );
}

//============================================================================
PktFileGetCompleteReq::PktFileGetCompleteReq()
: m_u32Error(0)
{
	setPktType( PKT_TYPE_FILE_GET_COMPLETE_REQ );
	setPktLength( (uint16_t)sizeof( PktFileGetCompleteReq ) );
}

//============================================================================
PktFileGetCompleteReply::PktFileGetCompleteReply()
: m_u32Error(0)
{
	setPktType( PKT_TYPE_FILE_GET_COMPLETE_REPLY );
	setPktLength( (uint16_t)sizeof( PktFileGetCompleteReply ) );
}

//============================================================================
PktFileShareErr::PktFileShareErr()
: m_u16Err(0xffff) 
, m_u16Res1(0)
, m_u32ResP1(0)
, m_u32ResP2(0)
, m_u32ResP3(0)
, m_u32Res1(0) 
, m_u32Res2(0)
{
	setPktType( PKT_TYPE_FILE_SHARE_ERR ); 
	setPktLength( sizeof( PktFileShareErr ) );
}

//============================================================================
const char * PktFileShareErr::describeError( void )
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


