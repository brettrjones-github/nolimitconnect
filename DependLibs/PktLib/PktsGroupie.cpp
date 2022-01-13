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

#include "PktsGroupie.h"

#include <CoreLib/VxDebug.h>

#include <string.h>

//============================================================================
PktGroupieInfoReq::PktGroupieInfoReq()
{
    setPktType( PKT_TYPE_GROUPIE_INFO_REQ );
    setPktLength( sizeof( PktGroupieInfoReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktGroupieInfoReply::PktGroupieInfoReply()
{
    setPktType( PKT_TYPE_GROUPIE_INFO_REPLY );
    setPktLength( sizeof( PktGroupieInfoReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktGroupieInfoReply::calcPktLen( void )
{
    uint16_t pktLen = ( uint16_t )sizeof( PktGroupieInfoReply ) - sizeof( PktBlobEntry );
    pktLen += getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktGroupieInfoReply::setGroupieUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetWrite();
    bool result = m_BlobEntry.setValue( lastModifiedTime );
    result &= m_BlobEntry.setValue( groupieUrl );
    result &= m_BlobEntry.setValue( groupieTitle );
    result &= m_BlobEntry.setValue( groupieDesc );
    return result;
}

//============================================================================
bool PktGroupieInfoReply::getGroupieUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetRead();
    bool result = m_BlobEntry.getValue( lastModifiedTime );
    result &= m_BlobEntry.getValue( groupieUrl );
    result &= m_BlobEntry.getValue( groupieTitle );
    result &= m_BlobEntry.getValue( groupieDesc );
    return result;
}

//============================================================================
//============================================================================
PktGroupieAnnounceReq::PktGroupieAnnounceReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_GROUPIE_ANN_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktGroupieAnnounceReq ) ) );
    // LogMsg( LOG_DEBUG, "PktGroupieReq size %d %d", sizeof( PktGroupieReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktGroupieAnnounceReq::setGroupieInfo( std::string& inviteUrl, std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime )
{
    bool result = lastModifiedTime && !inviteUrl.empty() && !hostTitle.empty() && !hostDesc.empty();
    if( result )
    {
        m_BlobEntry.resetWrite();
        result &= m_BlobEntry.setValue( lastModifiedTime );
        result &= m_BlobEntry.setValue( inviteUrl );
        result &= m_BlobEntry.setValue( hostTitle );
        result &= m_BlobEntry.setValue( hostDesc );
        calcPktLen();
    }

    return result;
}

//============================================================================
bool PktGroupieAnnounceReq::getGroupieInfo( std::string& inviteUrl, std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetRead();
    bool result = m_BlobEntry.getValue( lastModifiedTime );
    result &= m_BlobEntry.getValue( inviteUrl );
    result &= m_BlobEntry.getValue( hostTitle );
    result &= m_BlobEntry.getValue( hostDesc );  
    
    return result && lastModifiedTime && !inviteUrl.empty() && !hostTitle.empty() && !hostDesc.empty();
}

//============================================================================
void PktGroupieAnnounceReq::calcPktLen()
{
    uint16_t pktLen = ( uint16_t )sizeof( PktGroupieAnnounceReq ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );

    // LogMsg( LOG_DEBUG, "PktGroupieReq calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktGroupieAnnounceReq* PktGroupieAnnounceReq::makeGroupieAnnCopy( void )
{
    vx_assert( getPktLength() );
    vx_assert( ( getPktLength() & 0x0f ) == 0 );
    vx_assert( PKT_TYPE_GROUPIE_ANN_REQ == getPktType() );
    char* pTemp = new char[getPktLength()];
    vx_assert( pTemp );
    memcpy( pTemp, this, getPktLength() );
    return ( PktGroupieAnnounceReq* )pTemp;
}

//============================================================================
PktGroupieAnnounceReply::PktGroupieAnnounceReply()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_GROUPIE_ANN_REPLY );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktGroupieAnnounceReply ) ) );
    // LogMsg( LOG_DEBUG, "PktGroupieReq size %d %d", sizeof( PktGroupieReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktGroupieSearchReq::PktGroupieSearchReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_GROUPIE_SEARCH_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktGroupieSearchReq ) ) );
    // LogMsg( LOG_DEBUG, "PktGroupieReq size %d %d", sizeof( PktGroupieReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktGroupieSearchReply::PktGroupieSearchReply()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_GROUPIE_SEARCH_REPLY );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktGroupieSearchReply ) ) );
    // LogMsg( LOG_DEBUG, "PktGroupieReq size %d %d", sizeof( PktGroupieReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktGroupieSearchReply::calcPktLen()
{
    uint16_t pktLen = ( uint16_t )sizeof( PktGroupieSearchReply ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );

    // LogMsg( LOG_DEBUG, "PktGroupieReq calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktGroupieMoreReq::PktGroupieMoreReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_GROUPIE_MORE_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktGroupieMoreReq ) ) );
    // LogMsg( LOG_DEBUG, "PktGroupieReq size %d %d", sizeof( PktGroupieReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == (getPktLength() & 0x0f) );
}

//============================================================================
PktGroupieMoreReply::PktGroupieMoreReply()
    : VxPktHdr()
    , m_BlobEntry()
{
    setPktType( PKT_TYPE_GROUPIE_MORE_REPLY );
    setPktLength( sizeof( PktGroupieMoreReply ) );
    // LogMsg( LOG_DEBUG, "PktGroupieMoreReply size %d %d", sizeof( PktGroupieReply ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktGroupieMoreReply::calcPktLen()
{
    uint16_t pktLen = ( uint16_t)sizeof( PktGroupieMoreReply ) - sizeof(PktBlobEntry);
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) ); 

    LogMsg( LOG_DEBUG, "PktGroupieMoreReply calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == (getPktLength() & 0x0f) );
}
