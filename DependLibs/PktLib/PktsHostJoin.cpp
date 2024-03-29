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

#include "PktsHostJoin.h"

#include <CoreLib/VxDebug.h>

#include <string.h>


//============================================================================
PktHostOfferReq::PktHostOfferReq()
{
    setPktType( PKT_TYPE_HOST_OFFER_REQ );
    m_au8Data[0] = 0;
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostOfferReq ) - sizeof(m_au8Data) + 1) ); 
    vx_assert( ( 0 == getPktLength() ) & 0x0f );
}

//============================================================================
void PktHostOfferReq::setOfferMsg( const char* msg )
{
    vx_assert( msg );
    uint16_t msgLen = ( uint16_t)strlen( msg );
    vx_assert( MAX_HOST_OFFER_MSG_LEN > msgLen );
    strcpy( (char *)m_au8Data, msg );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostOfferReq ) - sizeof(m_au8Data) + msgLen + 1) ); 
    m_StrLen = htons( msgLen );
    vx_assert( ( 0 == getPktLength() ) & 0x0f );
}

//============================================================================
const char* PktHostOfferReq::getOfferMsg( void )
{
    return (const char*)m_au8Data;
}

//============================================================================
//============================================================================
PktHostOfferReply::PktHostOfferReply()
{
    setPktType( PKT_TYPE_HOST_OFFER_REPLY );
    m_au8Data[0] = 0;
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostOfferReply ) - sizeof(m_au8Data) + 1) ); 
}

//============================================================================
void PktHostOfferReply::setOfferMsg( const char* msg )
{
    vx_assert( msg );
    uint16_t msgLen = ( uint16_t)strlen( msg );
    vx_assert( MAX_HOST_OFFER_MSG_LEN > msgLen );
    strcpy( (char *)m_au8Data, msg );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostOfferReq ) - sizeof(m_au8Data) + msgLen + 1) ); 
    m_StrLen = htons( msgLen );
}

//============================================================================
const char* PktHostOfferReply::getOfferMsg( void )
{
    return (const char*)m_au8Data;
}

//============================================================================
//============================================================================
PktHostJoinReq::PktHostJoinReq()
{
    setPktType( PKT_TYPE_HOST_JOIN_REQ );
    setPktLength( sizeof(PktHostJoinReq) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostJoinReply::PktHostJoinReply()
{
    setPktType( PKT_TYPE_HOST_JOIN_REPLY );
    setPktLength( sizeof(PktHostJoinReply) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostLeaveReq::PktHostLeaveReq()
{
    setPktType( PKT_TYPE_HOST_LEAVE_REQ );
    setPktLength( sizeof( PktHostLeaveReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostLeaveReply::PktHostLeaveReply()
{
    setPktType( PKT_TYPE_HOST_LEAVE_REPLY );
    setPktLength( sizeof( PktHostLeaveReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostUnJoinReq::PktHostUnJoinReq()
{
    setPktType( PKT_TYPE_HOST_UNJOIN_REQ );
    setPktLength( sizeof( PktHostUnJoinReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostUnJoinReply::PktHostUnJoinReply()
{
    setPktType( PKT_TYPE_HOST_UNJOIN_REPLY );
    setPktLength( sizeof( PktHostUnJoinReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}
