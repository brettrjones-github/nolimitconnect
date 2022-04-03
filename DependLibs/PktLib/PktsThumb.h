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
// http://www.nolimitconnect.com
//============================================================================
#pragma once

#include "VxPktHdr.h"
#include <CoreLib/IsBigEndianCpu.h>

#pragma pack(push)
#pragma pack(1)

class PktThumbBaseReq : public VxPktHdr
{
public:
    PktThumbBaseReq() = default;

    void						setThumbId( VxGUID& thumbId )   { setGuidToNetOrder( thumbId, m_ThumbId ); };
    VxGUID						getThumbId( void )              { return getGuidInHostOrder( m_ThumbId ); };

private:
	//=== vars ===//
	VxGUID						m_ThumbId;
};

class PktThumbSettingReq : public PktThumbBaseReq
{
public:
    PktThumbSettingReq();

    //=== vars ===//
    uint32_t					m_u32Res2 = 0;
    uint32_t					m_u32Res3 = 0;
};

class PktThumbAvatarReq : public PktThumbBaseReq
{
public:
    PktThumbAvatarReq();

    //=== vars ===//
    uint32_t					m_u32Res2 = 0;
    uint32_t					m_u32Res3 = 0;
};

class PktThumbFileReq : public PktThumbBaseReq
{
public:
    PktThumbFileReq();

    //=== vars ===//
    uint32_t					m_u32Res2 = 0;
    uint32_t				    m_u32Res3 = 0;
};


#define MAX_THUMB_PIC_LEN		(240*240*4)

class PktThumbBaseReply : public VxPktHdr
{
public:
    PktThumbBaseReply() = default;

    static PktThumbBaseReply *	allocateNewWithMaximumSizeAThumbCanBe( void );

    void                        calcThumbPktLen( void );

    void						setThumbId( VxGUID& thumbId )   { setGuidToNetOrder( thumbId, m_ThumbId ); };
    VxGUID						getThumbId( void )              { return getGuidInHostOrder( m_ThumbId ); };
    void						setTimeStampMs( int64_t timeStamp );
    int64_t					    getTimeStampMs( void );

    void						setWidth( uint16_t width );
    uint16_t					getWidth( void );
    void						setHeight( uint16_t height );
    uint16_t					getHeight( void );

    void						setTotalDataLen( uint32_t len );
    uint32_t					getTotalDataLen( void );
    void						setThisDataLen( uint32_t len );
    uint32_t					getThisDataLen( void );

    void						setPktSeqNum( uint32_t seqNum );
    uint32_t					getPktSeqNum( void );
    void						setTotalPktsInSeq( uint32_t seqCnt );
    uint32_t					getTotalPktsInSeq( void );

    uint8_t *					getDataPayload( void ) { return m_au8Data; }

private:
    //=== vars ===//
    VxGUID						m_ThumbId;
    int64_t						m_s64TimeStampMs = 0;
    uint16_t					m_u16Width = 0;
    uint16_t					m_u16Height = 0;
    uint32_t					m_u32TotalDataLen = 0;
    uint32_t					m_u32ThisDataLen = 0;
    uint32_t					m_u32PktSeqNum = 0;
    uint32_t					m_u32TotalPktsInSeq = 0;
    uint32_t					m_u32Res1 = 0;
    uint32_t					m_u32Res2 = 0;
    uint32_t					m_u32Res3 = 0;
    uint8_t						m_au8Data[ 4 ];
};

class PktThumbSettingReply : public PktThumbBaseReply
{
public:
    PktThumbSettingReply();

    //=== vars ===//
};

class PktThumbAvatarReply : public PktThumbBaseReply
{
public:
    PktThumbAvatarReply();

    //=== vars ===//
};

class PktThumbFileReply : public PktThumbBaseReply
{
public:
    PktThumbFileReply();

    //=== vars ===//
};

#define MAX_THUMB_CHUNK_LEN		(MAX_PKT_LEN - ( sizeof(VxPktHdr) + 64 ))

class PktThumbPicChunkBaseReq : public VxPktHdr
{
public:
    PktThumbPicChunkBaseReq() = default;

	void						calcPktLen( void );

    void						setThumbId( VxGUID& thumbId )           { setGuidToNetOrder( thumbId, m_ThumbId ); };
    VxGUID						getThumbId( void )                      { return getGuidInHostOrder( m_ThumbId ); };

	void						setThisDataLen( uint32_t len )		    { m_u32ThisDataLen = htonl( len ); }
	uint32_t					getThisDataLen( void )				    { return ntohl( m_u32ThisDataLen ); }
	void						setPktSeqNum( uint32_t seqNum )		    { m_u32PktSeqNum = htonl( seqNum ); }
	uint32_t					getPktSeqNum( void )				    { return ntohl( m_u32PktSeqNum ); }
	void						setTotalPktsInSeq( uint32_t seqCnt )	{ m_u32TotalPktsInSeq = htonl( seqCnt ); }
	uint32_t					getTotalPktsInSeq( void )			    { return ntohl( m_u32TotalPktsInSeq ); }

	uint8_t *					getDataPayload( void )				    { return m_au8Data; }

private:
	//=== vars ===//
    VxGUID						m_ThumbId;
    uint32_t					m_u32ThisDataLen = 0;
	uint32_t					m_u32PktSeqNum = 0;
	uint32_t					m_u32TotalPktsInSeq = 0;
	uint32_t					m_u32Res = 0;
	uint8_t						m_au8Data[ MAX_THUMB_CHUNK_LEN + 16 ];
};

class PktThumbPicChunkBaseReply : public VxPktHdr
{
public:
    PktThumbPicChunkBaseReply() = default;

    void						setThumbId( VxGUID& thumbId )           { setGuidToNetOrder( thumbId, m_ThumbId ); };
    VxGUID						getThumbId( void )                      { return getGuidInHostOrder( m_ThumbId ); };

private:
    //=== vars ===//
    VxGUID						m_ThumbId;
    uint32_t					m_u32Res2 = 0;
    uint32_t					m_u32Res3 = 0;
};

class PktThumbSettingChunkReq : public PktThumbPicChunkBaseReq
{
public:
    PktThumbSettingChunkReq();

    //=== vars ===//
};

class PktThumbSettingChunkReply : public PktThumbPicChunkBaseReply
{
public:
    PktThumbSettingChunkReply();

    //=== vars ===//
};

class PktThumbAvatarChunkReq : public PktThumbPicChunkBaseReq
{
public:
    PktThumbAvatarChunkReq();

    //=== vars ===//
};

class PktThumbAvatarChunkReply : public PktThumbPicChunkBaseReply
{
public:
    PktThumbAvatarChunkReply();

    //=== vars ===//
};

class PktThumbFileChunkReq : public PktThumbPicChunkBaseReq
{
public:
    PktThumbFileChunkReq();

    //=== vars ===//
};

class PktThumbFileChunkReply : public PktThumbPicChunkBaseReply
{
public:
    PktThumbFileChunkReply();

    //=== vars ===//
};

#pragma pack(pop)
