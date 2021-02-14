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

#include "PktsHostSearch.h"

#include <CoreLib/VxDebug.h>

#include <string.h>


//============================================================================
PktHostSearchReq::PktHostSearchReq()
{
    setPktType( PKT_TYPE_HOST_SEARCH_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostSearchReq ) ) ); 
    vx_assert( 0 == getPktLength() & 0x0f );
}

//============================================================================
void PktHostSearchReq::calculateLength()
{
    uint16_t pktLen = ( uint16_t)sizeof( PktHostSearchReq ) - sizeof(PktBlobEntry);
    pktLen += getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) ); 
    vx_assert( 0 == getPktLength() & 0x0f );
}

//============================================================================
PktHostSearchReply::PktHostSearchReply()
{
    setPktType( PKT_TYPE_HOST_SEARCH_REPLY );
    setPktLength( sizeof(PktHostSearchReply) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktHostSearchReply::setTotalMatches( uint16_t matchCnt )
{
    m_TotalMatches = htons( matchCnt );
}

//============================================================================
uint16_t PktHostSearchReply::getTotalMatches( void ) const
{
    return ntohs( m_TotalMatches );
}

//============================================================================
void PktHostSearchReply::setTotalBlobLen( uint16_t totalBlobLen )
{
    m_TotalBlobLen = htons( totalBlobLen );
}

//============================================================================
uint16_t PktHostSearchReply::getTotalBlobLen( void ) const
{
    return ntohs( m_TotalBlobLen );
}

//============================================================================
uint16_t PktHostSearchReply::getRemainingBlobStorageLen( void ) const
{
    return m_PktBlob.getRemainingStorageLen();
}