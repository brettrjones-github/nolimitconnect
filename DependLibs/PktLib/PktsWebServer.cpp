//============================================================================
// Copyright (C) 2014 Brett R. Jones 
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

#include "PktsWebServer.h"

//============================================================================
PktMyPicSendReq::PktMyPicSendReq()
: m_u32Res1(0)
, m_u32Res2(0)
{
	setPktLength( sizeof(PktMyPicSendReq) );		
	setPktType( PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ ); 
}

//============================================================================
PktMyPicSendReply::PktMyPicSendReply()
: m_u32TotalDataLen( 0 )
, m_u16Error(0)
, m_u16Res1(0)
{
	setPktLength( sizeof(PktMyPicSendReply) );		
	setPktType( PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY ); 
}

//============================================================================
PktWebServerPicChunkTx::PktWebServerPicChunkTx()
{ 
	init();
}

//============================================================================
void PktWebServerPicChunkTx::init( void )
{
	setPktLength( 0 ); 
	setPktType( PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX ); 
	m_u64TotalDataLen = 0;
	m_u32ThisDataLen = 0;
	m_u64FileOffset = 0;
	m_u32Res = 0;
}

//============================================================================
void PktWebServerPicChunkTx::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof(PktWebServerPicChunkTx) + getThisDataLen() ) );
}

//============================================================================
PktWebServerPicChunkAck::PktWebServerPicChunkAck()
: m_u64TotalDataLen(0)
, m_u64FileOffset(0)
, m_u32ThisDataLen(0)
, m_u32Res1(0)
{ 
	setPktLength( sizeof(PktWebServerPicChunkAck) );		
	setPktType( PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK ); 
}

//============================================================================
PktWebServerGetChunkTx::PktWebServerGetChunkTx()
{ 
	init();
}

//============================================================================
void PktWebServerGetChunkTx::init( void )
{
	setPktLength( 0 ); 
	setPktType( PKT_TYPE_WEB_SERVER_GET_CHUNK_TX ); 
	m_u64TotalDataLen = 0;
	m_u32ThisDataLen = 0;
	m_u64FileOffset = 0;
	m_u32Res1 = 0;
}

//============================================================================
void PktWebServerGetChunkTx::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof(PktWebServerGetChunkTx) + getThisDataLen() ) );
}

//============================================================================
PktWebServerGetChunkAck::PktWebServerGetChunkAck()
: m_u64TotalDataLen(0)
, m_u64FileOffset(0)
, m_u32ThisDataLen(0)
, m_u32Res1(0)
{ 
	setPktLength( sizeof(PktWebServerGetChunkAck) );		
	setPktType( PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK ); 
}

//============================================================================
PktWebServerPutChunkTx::PktWebServerPutChunkTx()
{ 
	init();
}

//============================================================================
void PktWebServerPutChunkTx::init( void )
{
	setPktLength( 0 ); 
	setPktType( PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX ); 
	m_u64TotalDataLen = 0;
	m_u32ThisDataLen = 0;
	m_u64FileOffset = 0;
	m_u32Res1 = 0;
}

//============================================================================
void PktWebServerPutChunkTx::calcPktLen( void )
{
	setPktLength( ROUND_TO_16BYTE_BOUNDRY( sizeof(PktWebServerPutChunkTx) + getThisDataLen() ) );
}

//============================================================================
PktWebServerPutChunkAck::PktWebServerPutChunkAck()
: m_u64TotalDataLen(0)
, m_u64FileOffset(0)
, m_u32ThisDataLen(0)
, m_u32Res1(0)
{ 
	setPktLength( sizeof(PktWebServerPutChunkAck) );		
	setPktType( PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK ); 
}
