#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones 
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

#include "PktTypes.h"
#include <CoreLib/IsBigEndianCpu.h>

#ifdef _MSC_VER
# pragma warning(disable: 4200) //nonstandard extension used : zero-sized array in struct/union
#endif

#pragma pack(push)
#pragma pack(1)
class PktMyPicSendReq : public VxPktHdr
{
public:
	PktMyPicSendReq();

private:
	//=== vars ===//
    uint32_t					m_u32Res1;
    uint32_t					m_u32Res2;
};

class PktMyPicSendReply : public VxPktHdr
{
public:
	PktMyPicSendReply();

	void						setTotalDataLen( uint32_t len )			{ m_u32TotalDataLen = htonl(len); }
    uint32_t					getTotalDataLen( void )                 { return htonl( m_u32TotalDataLen ); }
    void						setError( uint16_t errCode )			{ m_u16Error = htons(errCode); }
    uint16_t					getError( void )                        { return ntohs( m_u16Error ); }

private:
	//=== vars ===//
    uint32_t					m_u32TotalDataLen;
    uint16_t					m_u16Error;
    uint16_t					m_u16Res1;
};

#define MAX_WEB_SERVER_CHUNK_LEN 8192 + 4096
#define PIC_ERR_ACCESS_DENIED		1
#define PIC_ERR_FILE_NOT_FOUND		2
#define PIC_ERR_FILE_OPEN_ERROR		3
#define PIC_ERR_FILE_READ_ERROR		4

class PktWebServerPicChunkTx : public VxPktHdr
{
public:
	PktWebServerPicChunkTx();

	void						init( void );
	void						calcPktLen( void );
	void						setTotalDataLen( uint64_t len )			{ m_u64TotalDataLen = htonU64(len); }
	uint64_t							getTotalDataLen( void )				{ return ntohU64( m_u64TotalDataLen ); }
	void						setFileOffset( uint64_t offs )			{ m_u64FileOffset = htonU64(offs); }
	uint64_t							getFileOffset( void )				{ return ntohU64( m_u64FileOffset ); }
	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl(len); }
	uint32_t							getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }
	unsigned char *				getPayloadBuffer( void )			{ return m_au8Data; }
private:
	//=== vars ===//
    uint64_t					m_u64TotalDataLen;
    uint64_t					m_u64FileOffset;
    uint32_t					m_u32ThisDataLen;
    uint32_t					m_u32Res;
	unsigned char				m_au8Data[0];
};

class PktWebServerPicChunkAck : public VxPktHdr
{
public:
	PktWebServerPicChunkAck();

	void						setTotalDataLen( uint64_t len )			{ m_u64TotalDataLen = htonU64(len); }
    uint64_t					getTotalDataLen( void )				{ return ntohU64( m_u64TotalDataLen ); }
	void						setFileOffset( uint64_t offs )			{ m_u64FileOffset = htonU64(offs); }
    uint64_t					getFileOffset( void )				{ return ntohU64( m_u64FileOffset ); }
	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl(len); }
    uint32_t					getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }

private:
	//=== vars ===//
	uint64_t							m_u64TotalDataLen;
	uint64_t							m_u64FileOffset;
	uint32_t							m_u32ThisDataLen;
	uint32_t							m_u32Res1;
};

class PktWebServerGetChunkTx : public VxPktHdr
{
public:
	PktWebServerGetChunkTx();

	void						init( void );
	void						calcPktLen( void );
	void						setTotalDataLen( uint64_t len )			{ m_u64TotalDataLen = htonU64(len); }
	uint64_t							getTotalDataLen( void )				{ return ntohU64( m_u64TotalDataLen ); }
	void						setFileOffset( uint64_t offs )			{ m_u64FileOffset = htonU64(offs); }
	uint64_t							getFileOffset( void )				{ return ntohU64( m_u64FileOffset ); }
	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl(len); }
	uint32_t							getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }
	unsigned char *				getDataPayload( void )				{ return m_au8Data; }

private:
	//=== vars ===//
	uint64_t							m_u64TotalDataLen;
	uint64_t							m_u64FileOffset;
	uint32_t							m_u32ThisDataLen;
	uint32_t							m_u32Res1;
	unsigned char				m_au8Data[0];
};

class PktWebServerGetChunkAck : public VxPktHdr
{
public:
	PktWebServerGetChunkAck();
	void						setTotalDataLen( uint64_t len )			{ m_u64TotalDataLen = htonU64(len); }
	uint64_t							getTotalDataLen( void )				{ return ntohU64( m_u64TotalDataLen ); }
	void						setFileOffset( uint64_t offs )			{ m_u64FileOffset = htonU64(offs); }
	uint64_t							getFileOffset( void )				{ return ntohU64( m_u64FileOffset ); }
	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl(len); }
	uint32_t							getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }

private:
	//=== vars ===//
	uint64_t							m_u64TotalDataLen;
	uint64_t							m_u64FileOffset;
	uint32_t							m_u32ThisDataLen;
	uint32_t							m_u32Res1;
};

class PktWebServerPutChunkTx : public VxPktHdr
{
public:
	PktWebServerPutChunkTx();

	void						init( void );
	void						calcPktLen( void );
	void						setTotalDataLen( uint64_t len )			{ m_u64TotalDataLen = htonU64(len); }
	uint64_t							getTotalDataLen( void )				{ return ntohU64( m_u64TotalDataLen ); }
	void						setFileOffset( uint64_t offs )			{ m_u64FileOffset = htonU64(offs); }
	uint64_t							getFileOffset( void )				{ return ntohU64( m_u64FileOffset ); }
	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl(len); }
	uint32_t							getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }
	unsigned char *				getDataPayload( void )				{ return m_au8Data; }

private:
	//=== vars ===//
	uint64_t							m_u64TotalDataLen;
	uint64_t							m_u64FileOffset;
	uint32_t							m_u32ThisDataLen;
	uint32_t							m_u32Res1;
	unsigned char				m_au8Data[0];

};

class PktWebServerPutChunkAck : public VxPktHdr
{
public:
	PktWebServerPutChunkAck();

	void						setTotalDataLen( uint64_t len )			{ m_u64TotalDataLen = htonU64(len); }
	uint64_t							getTotalDataLen( void )				{ return ntohU64( m_u64TotalDataLen ); }
	void						setFileOffset( uint64_t offs )			{ m_u64FileOffset = htonU64(offs); }
	uint64_t							getFileOffset( void )				{ return ntohU64( m_u64FileOffset ); }
	void						setThisDataLen( uint32_t len )			{ m_u32ThisDataLen = htonl(len); }
	uint32_t							getThisDataLen( void )				{ return ntohl( m_u32ThisDataLen ); }

private:
	//=== vars ===//
	uint64_t							m_u64TotalDataLen;
	uint64_t							m_u64FileOffset;
	uint32_t							m_u32ThisDataLen;
	uint32_t							m_u32Res1;
};

#pragma pack(pop)

