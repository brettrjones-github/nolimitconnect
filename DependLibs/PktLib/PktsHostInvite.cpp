//============================================================================
// Copyright (C) 2022 Brett R. Jones 
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

#include "PktsHostInvite.h"

#include <CoreLib/VxDebug.h>

#include <string.h>

//============================================================================
PktHostInviteAnnounceReq::PktHostInviteAnnounceReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_HOST_INVITE_ANN_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostInviteAnnounceReq ) ) );
    // LogMsg( LOG_DEBUG, "PktHostInviteReq size %d %d", sizeof( PktHostInviteReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktHostInviteAnnounceReq::calcPktLen()
{
    uint16_t pktLen = ( uint16_t )sizeof( PktHostInviteAnnounceReq ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );

    // LogMsg( LOG_DEBUG, "PktHostInviteReq calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostInviteAnnounceReply::PktHostInviteAnnounceReply()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_HOST_INVITE_ANN_REPLY );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostInviteAnnounceReply ) ) );
    // LogMsg( LOG_DEBUG, "PktHostInviteReq size %d %d", sizeof( PktHostInviteReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostInviteSearchReq::PktHostInviteSearchReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_HOST_INVITE_SEARCH_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostInviteSearchReq ) ) );
    // LogMsg( LOG_DEBUG, "PktHostInviteReq size %d %d", sizeof( PktHostInviteReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostInviteSearchReply::PktHostInviteSearchReply()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_HOST_INVITE_SEARCH_REPLY );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostInviteSearchReply ) ) );
    // LogMsg( LOG_DEBUG, "PktHostInviteReq size %d %d", sizeof( PktHostInviteReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktHostInviteSearchReply::calcPktLen()
{
    uint16_t pktLen = ( uint16_t )sizeof( PktHostInviteSearchReply ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );

    // LogMsg( LOG_DEBUG, "PktHostInviteReq calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktHostInviteMoreReq::PktHostInviteMoreReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_HOST_INVITE_MORE_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktHostInviteMoreReq ) ) );
    // LogMsg( LOG_DEBUG, "PktHostInviteReq size %d %d", sizeof( PktHostInviteReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == (getPktLength() & 0x0f) );
}

//============================================================================
PktHostInviteMoreReply::PktHostInviteMoreReply()
    : VxPktHdr()
    , m_BlobEntry()
{
    setPktType( PKT_TYPE_HOST_INVITE_MORE_REPLY );
    setPktLength( sizeof( PktHostInviteMoreReply ) );
    // LogMsg( LOG_DEBUG, "PktHostInviteMoreReply size %d %d", sizeof( PktHostInviteReply ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktHostInviteMoreReply::calcPktLen()
{
    uint16_t pktLen = ( uint16_t)sizeof( PktHostInviteMoreReply ) - sizeof(PktBlobEntry);
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) ); 

    LogMsg( LOG_DEBUG, "PktHostInviteReply calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == (getPktLength() & 0x0f) );
}
