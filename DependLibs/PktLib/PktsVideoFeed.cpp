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

#include "PktsVideoFeed.h"
#include "PktTypes.h"

#include <memory.h>

//============================================================================
PktVideoFeedReq::PktVideoFeedReq()
: m_f32MotionDetectLevel(0.0f)
, m_u32Res2( 0 )
, m_LclSessionId()
, m_RmtSessionId()
{ 
	setPktLength( sizeof( PktVideoFeedReq ) ); 
	setPktType( PKT_TYPE_VIDEO_FEED_REQ ); 
}

//============================================================================
PktVideoFeedStatus::PktVideoFeedStatus()
: m_f32MotionDetectLevel(0.0f)
, m_u8Status(0)
, m_u16Res1(0)
, m_LclSessionId()
, m_RmtSessionId()
{ 
	setPktLength( sizeof( PktVideoFeedStatus ) ); 
	setPktType( PKT_TYPE_VIDEO_FEED_STATUS ); 
}

//============================================================================
PktVideoFeedPic::PktVideoFeedPic()
: m_u16Width(0)
, m_u16Height(0)
, m_u8BitsPerPixel(0)
, m_u8PicType(0)
, m_u16Res(0)
, m_s32Motion0to100000(0)
, m_s64TimeStampMs(0)
, m_u32TotalDataLen(0)
, m_u32ThisDataLen(0)
, m_u32PktSeqNum(1)
, m_u32TotalPktsInSeq(1)
, m_u32Res1(0)
, m_u32Res2(0)
, m_u32Res3(0)
, m_u32Res4(0)
{ 
	setPktType( PKT_TYPE_VIDEO_FEED_PIC ); 
}

//============================================================================
PktVideoFeedPic * PktVideoFeedPic::allocateNewWithMaximumSizeAJpgCanBe( void )
{
	char * newPkt =  new char[ sizeof( PktVideoFeedPic ) + 16 + MAX_VIDEO_JPG_PIC_LEN ];
	PktVideoFeedPic lclPkt;
	memcpy( newPkt, &lclPkt, sizeof( PktVideoFeedPic ) );
	return (PktVideoFeedPic *)newPkt;
}

//============================================================================
void PktVideoFeedPic::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof(PktVideoFeedPic) + htonl( m_u32ThisDataLen ) ) );
}

void	        PktVideoFeedPic::setWidth( uint16_t width )			    { m_u16Width = htons( width ); }
uint16_t		PktVideoFeedPic::getWidth( void )				        { return ntohs( m_u16Width ); }
void	        PktVideoFeedPic::setHeight( uint16_t height )		    { m_u16Height = htons( height ); }
uint16_t		PktVideoFeedPic::getHeight( void )				        { return ntohs( m_u16Height ); }
void	        PktVideoFeedPic::setBitsPerPixel( uint8_t bitsPerPixel )	{ m_u8BitsPerPixel = bitsPerPixel; }
uint8_t		    PktVideoFeedPic::getBitsPerPixel( void )		        { return m_u8BitsPerPixel; }
void	        PktVideoFeedPic::setPicType( uint8_t picType )		    { m_u8PicType = picType; }
uint8_t		    PktVideoFeedPic::getPicType( void )				        { return m_u8PicType; }
//void	PktVideoFeedPic::setRotate( uint16_t rotate )		    { m_u16Rotate = ntohs( rotate ); }
//uint16_t		PktVideoFeedPic::getRotate( void )				{ return ntohs( m_u16Rotate ); }
void	        PktVideoFeedPic::setMotionDetect( int32_t percent )	    { m_s32Motion0to100000 = percent; }
int32_t		    PktVideoFeedPic::getMotionDetect( void )		        { return m_s32Motion0to100000; }
void	        PktVideoFeedPic::setTimeStampMs( int64_t timeStamp )	{ m_s64TimeStampMs = htonU64( timeStamp ); }
int64_t		    PktVideoFeedPic::getTimeStampMs( void )			        { return ntohU64( m_s64TimeStampMs ); }
void	        PktVideoFeedPic::setTotalDataLen( uint32_t len )		{ m_u32TotalDataLen = htonl( len ); }
uint32_t		PktVideoFeedPic::getTotalDataLen( void )		        { return ntohl( m_u32TotalDataLen ); }
void	        PktVideoFeedPic::setThisDataLen( uint32_t len )		    { m_u32ThisDataLen = htonl( len ); }
uint32_t		PktVideoFeedPic::getThisDataLen( void )			        { return ntohl( m_u32ThisDataLen ); }
void	        PktVideoFeedPic::setPktSeqNum( uint32_t seqNum )		{ m_u32PktSeqNum = htonl( seqNum ); }
uint32_t		PktVideoFeedPic::getPktSeqNum( void )			        { return ntohl( m_u32PktSeqNum ); }
void	        PktVideoFeedPic::setTotalPktsInSeq( uint32_t seqCnt )   { m_u32TotalPktsInSeq = htonl( seqCnt ); }
uint32_t		PktVideoFeedPic::getTotalPktsInSeq( void )		        { return ntohl( m_u32TotalPktsInSeq ); }


//============================================================================
PktVideoFeedPicChunk::PktVideoFeedPicChunk()
	: m_u32ThisDataLen(0)
	, m_u32PktSeqNum(1)
	, m_u32TotalPktsInSeq(1)
	, m_u32Res(0)
{ 
	setPktType( PKT_TYPE_VIDEO_FEED_PIC_CHUNK ); 
}

//============================================================================
void PktVideoFeedPicChunk::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof(PktVideoFeedPicChunk) - sizeof( m_au8Data ) + getThisDataLen() ) );
}

//============================================================================
PktVideoFeedPicAck::PktVideoFeedPicAck()
: m_u32Res2(0)
, m_u32Res3(1)
{ 
	setPktLength( sizeof( PktVideoFeedPicAck ) ); 
	setPktType( PKT_TYPE_VIDEO_FEED_PIC_ACK ); 
}
