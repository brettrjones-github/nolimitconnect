#pragma once
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
// http://www.nolimitconnect.com
//============================================================================

#include "PktVoiceReq.h"
#include "PktVoiceReply.h"

#pragma pack(push)
#pragma pack(1)

class PktVideoFeedReq : public VxPktHdr
{
public:
	PktVideoFeedReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

private:
	//=== vars ===//
	float						m_f32MotionDetectLevel;
	uint32_t							m_u32Res2;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

enum EFeedStatus
{
	eFeedStatusUnknown		= 0,
	eFeedStatusOnline		= 1,
	eFeedStatusOffline		= 2,
	eFeedStatusBusy			= 3,

	eMaxFeedStatus
};

class PktVideoFeedStatus : public VxPktHdr
{
public:
	PktVideoFeedStatus();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }
	void						setFeedStatus( EFeedStatus feedStatus )		{ m_u8Status = (uint8_t)feedStatus; }
	EFeedStatus					getFeedStatus( void )						{ return (EFeedStatus)m_u8Status; }

	//=== vars ===//
	float						m_f32MotionDetectLevel;
	uint8_t							m_u8Status;

private:
	uint8_t							m_u8Res1;
	uint16_t							m_u16Res1;
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

#ifdef _MSC_VER
# pragma warning(disable: 4200)
#endif //_MSC_VER

#define MAX_VIDEO_JPG_PIC_LEN		(320*240*3)

class PktVideoFeedPic : public VxPktHdr
{
public:
	PktVideoFeedPic();

static PktVideoFeedPic *		allocateNewWithMaximumSizeAJpgCanBe( void );

	void						calcPktLen( void );

	void						setWidth( uint16_t width );
	uint16_t					getWidth( void );
	void						setHeight( uint16_t height );
	uint16_t					getHeight( void );
	void						setBitsPerPixel( uint8_t bitsPerPixel );
	uint8_t						getBitsPerPixel( void );
	void						setPicType( uint8_t picType );
	uint8_t						getPicType( void );

	void						setMotionDetect( int32_t motion0to100000 );
	int32_t						getMotionDetect( void );
	void						setTimeStampMs( int64_t timeStamp );
	int64_t					    getTimeStampMs( void );

	void						setTotalDataLen( uint32_t len );
	uint32_t					getTotalDataLen( void );
	void						setThisDataLen( uint32_t len );
	uint32_t					getThisDataLen( void );

	void						setPktSeqNum( uint32_t seqNum );
	uint32_t					getPktSeqNum( void );
	void						setTotalPktsInSeq( uint32_t seqCnt );
	uint32_t					getTotalPktsInSeq( void );

	uint8_t *					getDataPayload( void )			{ return m_au8Data; }

private:
	//=== vars ===//
	uint16_t					m_u16Width;
	uint16_t					m_u16Height;
	uint8_t						m_u8BitsPerPixel;
	uint8_t						m_u8PicType;
	uint16_t					m_u16Res;
	int32_t						m_s32Motion0to100000;
	int64_t						m_s64TimeStampMs;
	uint32_t					m_u32TotalDataLen;
	uint32_t					m_u32ThisDataLen;
	uint32_t					m_u32PktSeqNum;
	uint32_t					m_u32TotalPktsInSeq;
	uint32_t					m_u32Res1;
	uint32_t					m_u32Res2;
	uint32_t					m_u32Res3;
	uint32_t					m_u32Res4;
	uint8_t						m_au8Data[0];
};


#define MAX_PIC_CHUNK_LEN		(MAX_PKT_LEN - ( sizeof(VxPktHdr) + 32 ))

class PktVideoFeedPicChunk : public VxPktHdr
{
public:
	PktVideoFeedPicChunk();

	void						calcPktLen( void );

	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl( len ); }
	uint32_t							getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }
	void						setPktSeqNum( uint32_t seqNum )			{ m_u32PktSeqNum = htonl( seqNum ); }
	uint32_t							getPktSeqNum( void )				{ return ntohl( m_u32PktSeqNum ); }
	void						setTotalPktsInSeq( uint32_t seqCnt )		{ m_u32TotalPktsInSeq = htonl( seqCnt ); }
	uint32_t							getTotalPktsInSeq( void )			{ return ntohl( m_u32TotalPktsInSeq ); }

	uint8_t *						getDataPayload( void )				{ return m_au8Data; }

private:
	//=== vars ===//
	uint32_t							m_u32ThisDataLen;
	uint32_t							m_u32PktSeqNum;
	uint32_t							m_u32TotalPktsInSeq;
	uint32_t							m_u32Res;
	uint8_t							m_au8Data[ MAX_PIC_CHUNK_LEN + 16 ];
};

class PktVideoFeedPicAck : public VxPktHdr
{
public:
	PktVideoFeedPicAck();

private:
	//=== vars ===//
	uint32_t							m_u32Res2;
	uint32_t							m_u32Res3;
};

#pragma pack(pop)
