//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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
#include "PktsHostInfo.h"

#include <CoreLib/VxDebug.h>

//============================================================================
PktHostInfoReq::PktHostInfoReq()
{
    setPktType( PKT_TYPE_HOST_INFO_REQ );
    setPktLength( sizeof( PktHostInfoReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostInfoReply::PktHostInfoReply()
{
    setPktType( PKT_TYPE_HOST_INFO_REPLY );
    setPktLength( sizeof( PktHostInfoReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktHostInfoReply::calcPktLen( void )
{
    uint16_t pktLen = ( uint16_t)sizeof( PktHostInfoReply ) - sizeof(PktBlobEntry);
    pktLen += getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktHostInfoReply::setHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetWrite();
    bool result = m_BlobEntry.setValue( lastModifiedTime );
    result &= m_BlobEntry.setValue( hostTitle );
    result &= m_BlobEntry.setValue( hostDesc );
    return result;
}

//============================================================================
bool PktHostInfoReply::getHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetRead();
    bool result = m_BlobEntry.getValue( lastModifiedTime );
    result &= m_BlobEntry.getValue( hostTitle );
    result &= m_BlobEntry.getValue( hostDesc );
    return result;
}
