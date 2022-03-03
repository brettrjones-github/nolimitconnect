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

#include "PktsFileInfo.h"

#include <CoreLib/VxDebug.h>

#include <string.h>

//============================================================================
PktFileInfoInfoReq::PktFileInfoInfoReq()
{
    setPktType( PKT_TYPE_FILE_INFO_INFO_REQ );
    setPktLength( sizeof( PktFileInfoInfoReq ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktFileInfoInfoReply::PktFileInfoInfoReply()
{
    setPktType( PKT_TYPE_FILE_INFO_INFO_REPLY );
    setPktLength( sizeof( PktFileInfoInfoReply ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktFileInfoInfoReply::calcPktLen( void )
{
    uint16_t pktLen = ( uint16_t )sizeof( PktFileInfoInfoReply ) - sizeof( PktBlobEntry );
    pktLen += getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktFileInfoInfoReply::setFileInfoUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetWrite();
    bool result = m_BlobEntry.setValue( lastModifiedTime );
    result &= m_BlobEntry.setValue( groupieUrl );
    result &= m_BlobEntry.setValue( groupieTitle );
    result &= m_BlobEntry.setValue( groupieDesc );
    return result;
}

//============================================================================
bool PktFileInfoInfoReply::getFileInfoUrlAndTitleAndDescription( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
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
PktFileInfoAnnounceReq::PktFileInfoAnnounceReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_FILE_INFO_ANN_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktFileInfoAnnounceReq ) ) );
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktFileInfoAnnounceReq::setFileInfoInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    bool result = lastModifiedTime && !groupieUrl.empty() && !groupieTitle.empty() && !groupieDesc.empty();
    if( result )
    {
        m_BlobEntry.resetWrite();
        result &= m_BlobEntry.setValue( lastModifiedTime );
        result &= m_BlobEntry.setValue( groupieUrl );
        result &= m_BlobEntry.setValue( groupieTitle );
        result &= m_BlobEntry.setValue( groupieDesc );
        calcPktLen();
    }

    return result;
}

//============================================================================
bool PktFileInfoAnnounceReq::getFileInfoInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    m_BlobEntry.resetRead();
    bool result = m_BlobEntry.getValue( lastModifiedTime );
    result &= m_BlobEntry.getValue( groupieUrl );
    result &= m_BlobEntry.getValue( groupieTitle );
    result &= m_BlobEntry.getValue( groupieDesc );
    
    return result && lastModifiedTime && !groupieUrl.empty() && !groupieTitle.empty() && !groupieDesc.empty();
}

//============================================================================
void PktFileInfoAnnounceReq::calcPktLen()
{
    uint16_t pktLen = ( uint16_t )sizeof( PktFileInfoAnnounceReq ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );

    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktFileInfoAnnounceReq* PktFileInfoAnnounceReq::makeFileInfoAnnCopy( void )
{
    vx_assert( getPktLength() );
    vx_assert( ( getPktLength() & 0x0f ) == 0 );
    vx_assert( PKT_TYPE_FILE_INFO_ANN_REQ == getPktType() );
    char* pTemp = new char[getPktLength()];
    vx_assert( pTemp );
    memcpy( pTemp, this, getPktLength() );
    return ( PktFileInfoAnnounceReq* )pTemp;
}

//============================================================================
PktFileInfoAnnounceReply::PktFileInfoAnnounceReply()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_FILE_INFO_ANN_REPLY );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktFileInfoAnnounceReply ) ) );

    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktFileInfoSearchReq::PktFileInfoSearchReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_FILE_INFO_SEARCH_REQ );
    calcPktLen();

    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktFileInfoSearchReq::setSearchText( std::string& searchText )
{
    getBlobEntry().resetWrite();
    if( !searchText.empty() )
    {
        bool result = getBlobEntry().setValue( searchText );
        calcPktLen();
        return result;
    }

    return false;
}

//============================================================================
bool PktFileInfoSearchReq::getSearchText( std::string& searchText )
{
    getBlobEntry().resetRead();
    return getBlobEntry().getValue( searchText );
}

//============================================================================
void PktFileInfoSearchReq::calcPktLen( void )
{
    uint16_t pktLen = ( uint16_t )sizeof( PktFileInfoSearchReq ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );
}

//============================================================================
PktFileInfoSearchReply::PktFileInfoSearchReply()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_FILE_INFO_SEARCH_REPLY );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktFileInfoSearchReply ) ) );
    // LogMsg( LOG_DEBUG, "PktFileInfoReq size %d %d", sizeof( PktFileInfoReq ), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
bool PktFileInfoSearchReply::addFileInfoInfo( std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    bool result{ false };
    int requiredSpace = groupieUrl.length() + groupieTitle.length() + groupieDesc.length() + 3 * sizeof( uint32_t );
    if( m_BlobEntry.haveRoom( requiredSpace ) )
    {
        result = m_BlobEntry.setValue( lastModifiedTime );
        result &= m_BlobEntry.setValue( groupieUrl );
        result &= m_BlobEntry.setValue( groupieTitle );
        result &= m_BlobEntry.setValue( groupieDesc );
    }

    if( result )
    {
        incrementFileInfoCount();
    }

    calcPktLen();
    return result;
}

//============================================================================
void PktFileInfoSearchReply::calcPktLen()
{
    uint16_t pktLen = ( uint16_t )sizeof( PktFileInfoSearchReply ) - sizeof( PktBlobEntry );
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) );

    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
PktFileInfoMoreReq::PktFileInfoMoreReq()
    : VxPktHdr()
{
    setPktType( PKT_TYPE_FILE_INFO_MORE_REQ );
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktFileInfoMoreReq ) ) );

    vx_assert( 0 == (getPktLength() & 0x0f) );
}

//============================================================================
PktFileInfoMoreReply::PktFileInfoMoreReply()
    : VxPktHdr()
    , m_BlobEntry()
{
    setPktType( PKT_TYPE_FILE_INFO_MORE_REPLY );
    setPktLength( sizeof( PktFileInfoMoreReply ) );

    vx_assert( 0 == ( getPktLength() & 0x0f ) );
}

//============================================================================
void PktFileInfoMoreReply::calcPktLen()
{
    uint16_t pktLen = ( uint16_t)sizeof( PktFileInfoMoreReply ) - sizeof(PktBlobEntry);
    uint16_t blobLen = getBlobEntry().getTotalBlobLen();
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( pktLen + blobLen ) ); 

    // LogMsg( LOG_DEBUG, "PktFileInfoMoreReply calcPktLen blob %d len %d %d", blobLen, getPktLength(), (getPktLength() & 0x0f) ); 
    vx_assert( 0 == (getPktLength() & 0x0f) );
}
