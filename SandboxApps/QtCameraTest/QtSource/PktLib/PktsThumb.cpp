//============================================================================
// Copyright (C) 2010 Brett R. Jones 
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

#include "PktsThumb.h"
#include "PktTypes.h"

#include <memory.h>



//============================================================================
PktThumbBaseReply * PktThumbBaseReply::allocateNewWithMaximumSizeAThumbCanBe( void )
{
    char * newPkt = new char[ sizeof( PktThumbBaseReply ) + 16 + MAX_THUMB_PIC_LEN ];
    PktThumbBaseReply lclPkt;
    memcpy( newPkt, &lclPkt, sizeof( PktThumbBaseReply ) );
    return ( PktThumbBaseReply * )newPkt;
}

//============================================================================
void PktThumbBaseReply::calcThumbPktLen( void )
{
    setPktLength( ROUND_TO_16BYTE_BOUNDRY( ( sizeof( PktThumbBaseReply ) - 4 ) + htonl( m_u32ThisDataLen ) ) );
}

//============================================================================
PktThumbSettingReq::PktThumbSettingReq()
{
    setPktLength( sizeof( PktThumbSettingReq ) );
    setPktType( PKT_TYPE_THUMB_SETTING_REQ );
}

//============================================================================
PktThumbAvatarReq::PktThumbAvatarReq()
{
    setPktLength( sizeof( PktThumbAvatarReq ) );
    setPktType( PKT_TYPE_THUMB_AVATAR_REQ );
}

//============================================================================
PktThumbFileReq::PktThumbFileReq()
{
    setPktLength( sizeof( PktThumbFileReq ) );
    setPktType( PKT_TYPE_THUMB_FILE_REQ );
}

void	        PktThumbBaseReply::setWidth( uint16_t width )			    { m_u16Width = htons( width ); }
uint16_t		PktThumbBaseReply::getWidth( void )				            { return ntohs( m_u16Width ); }
void	        PktThumbBaseReply::setHeight( uint16_t height )		        { m_u16Height = htons( height ); }
uint16_t		PktThumbBaseReply::getHeight( void )				        { return ntohs( m_u16Height ); }
void	        PktThumbBaseReply::setTimeStampMs( int64_t timeStamp )	    { m_s64TimeStampMs = htonU64( timeStamp ); }
int64_t		    PktThumbBaseReply::getTimeStampMs( void )			        { return ntohU64( m_s64TimeStampMs ); }
void	        PktThumbBaseReply::setTotalDataLen( uint32_t len )		    { m_u32TotalDataLen = htonl( len ); }
uint32_t		PktThumbBaseReply::getTotalDataLen( void )		            { return ntohl( m_u32TotalDataLen ); }
void	        PktThumbBaseReply::setThisDataLen( uint32_t len )		    { m_u32ThisDataLen = htonl( len ); }
uint32_t		PktThumbBaseReply::getThisDataLen( void )			        { return ntohl( m_u32ThisDataLen ); }
void	        PktThumbBaseReply::setPktSeqNum( uint32_t seqNum )		    { m_u32PktSeqNum = htonl( seqNum ); }
uint32_t		PktThumbBaseReply::getPktSeqNum( void )			            { return ntohl( m_u32PktSeqNum ); }
void	        PktThumbBaseReply::setTotalPktsInSeq( uint32_t seqCnt )     { m_u32TotalPktsInSeq = htonl( seqCnt ); }
uint32_t		PktThumbBaseReply::getTotalPktsInSeq( void )		        { return ntohl( m_u32TotalPktsInSeq ); }


//============================================================================
PktThumbSettingReply::PktThumbSettingReply()
{
    setPktLength( sizeof( PktThumbSettingReply ) );
    setPktType( PKT_TYPE_THUMB_SETTING_REPLY );
}

//============================================================================
PktThumbAvatarReply::PktThumbAvatarReply()
{
    setPktLength( sizeof( PktThumbAvatarReply ) );
    setPktType( PKT_TYPE_THUMB_AVATAR_REQ );
}

//============================================================================
PktThumbFileReply::PktThumbFileReply()
{
    setPktLength( sizeof( PktThumbFileReply ) );
    setPktType( PKT_TYPE_THUMB_FILE_REQ );
}

//============================================================================
void PktThumbPicChunkBaseReq::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof( PktThumbPicChunkBaseReq ) - sizeof( m_au8Data ) + getThisDataLen() ) );
}

//============================================================================
PktThumbSettingChunkReq::PktThumbSettingChunkReq()
{ 
    calcPktLen();
	setPktType( PKT_TYPE_THUMB_SETTING_CHUNK_REQ );
}

//============================================================================
PktThumbSettingChunkReply::PktThumbSettingChunkReply()
{
    setPktLength( sizeof( PktThumbSettingChunkReply ) );
    setPktType( PKT_TYPE_THUMB_SETTING_CHUNK_REPLY );
}

//============================================================================
PktThumbAvatarChunkReq::PktThumbAvatarChunkReq()
{
    calcPktLen();
    setPktType( PKT_TYPE_THUMB_AVATAR_CHUNK_REQ );
}

//============================================================================
PktThumbAvatarChunkReply::PktThumbAvatarChunkReply()
{
    setPktLength( sizeof( PktThumbAvatarChunkReply ) );
    setPktType( PKT_TYPE_THUMB_AVATAR_CHUNK_REPLY );
}

//============================================================================
PktThumbFileChunkReq::PktThumbFileChunkReq()
{
    calcPktLen();
    setPktType( PKT_TYPE_THUMB_FILE_CHUNK_REQ );
}

//============================================================================
PktThumbFileChunkReply::PktThumbFileChunkReply()
{
    setPktLength( sizeof( PktThumbFileChunkReply ) );
    setPktType( PKT_TYPE_THUMB_FILE_CHUNK_REPLY );
}
