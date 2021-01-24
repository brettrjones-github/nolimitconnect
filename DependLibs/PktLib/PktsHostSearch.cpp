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
    m_au8Data[0] = 0;
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostSearchReq ) - sizeof(m_au8Data) + 1) ); 
    vx_assert( 0 == getPktLength() & 0x0f );
}

//============================================================================
void PktHostSearchReq::setSearchText( const char * msg )
{
    vx_assert( msg );
    uint16_t msgLen = ( uint16_t)strlen( msg );
    vx_assert( MAX_HOST_SEARCH_MSG_LEN > msgLen );
    strcpy( (char *)m_au8Data, msg );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostSearchReq ) - sizeof(m_au8Data) + msgLen + 1) ); 
    m_StrLen = htons( msgLen );
    vx_assert( 0 == getPktLength() & 0x0f );
}

//============================================================================
const char * PktHostSearchReq::getSearchText( void )
{
    return (const char *)m_au8Data;
}

//============================================================================
PktHostSearchReply::PktHostSearchReply()
{
    setPktType( PKT_TYPE_HOST_SEARCH_REPLY );
    setPktLength( sizeof(PktHostSearchReply) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}
