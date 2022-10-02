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
// http://www.nolimitconnect.org
//============================================================================

#include "VxCommon.h"
#include "PktTypes.h"

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxSha1Hash.h>

#define PKT_PLUGIN_OFFER_MAX_MSG_LEN 512

#pragma pack(push)
#pragma pack(1)

class PktPluginOfferReq : public VxPktHdr
{
public:
	PktPluginOfferReq();

	void						setLclSessionId( VxGUID& lclId )		{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )					{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )		{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )					{ return m_RmtSessionId; }

	void						setFileHashId( VxSha1Hash& id )			{ m_FileHashId = id; }
	void						setFileHashId( uint8_t * id )			{ m_FileHashId.setHashData( id ); }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHashId; }

	void						setMessages( const char* pMsg1, const char* fileName = NULL );
	int							getOfferMsgLen( void )					{ return ntohs( m_u16Msg1Len ); }
	const char*				getOfferMsg( void );
	int							getFileNameLen( void )					{ return ntohs( m_u16FileNameLen ); }
	const char*				getFileName( void );

private:
	//=== vars ===//
	uint16_t					m_u16Msg1Len;		// length of message string
	uint16_t					m_u16FileNameLen;	// length of file name
	uint8_t						m_u8Version;		// version
	uint8_t						m_u8Reserved;		// reserved
	uint16_t					m_u16Res;
	uint32_t					m_u32Res1; 
	uint32_t					m_u32Res2; 
	uint32_t					m_u32Res3; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_FileHashId;
	char						m_as8Args[ (PKT_PLUGIN_OFFER_MAX_MSG_LEN * 2) + 18 ]; //message or argument
};

class PktPluginOfferReply : public VxPktHdr
{
public:
	PktPluginOfferReply();

	void						calcPktLen( void );

	void						setLclSessionId( VxGUID& lclId )		{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )					{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )		{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )					{ return m_RmtSessionId; }
	void						setFileHashId( VxSha1Hash& id )			{ m_FileHashId = id; }
	VxSha1Hash&					getFileHashId( void )					{ return m_FileHashId; }

	void						setOfferResponse( EOfferResponse eResponse )	{ m_u8Response = (uint8_t)eResponse; }
	EOfferResponse				getOfferResponse( void )						{ return (EOfferResponse)m_u8Response; }

private:
	uint8_t						m_u8Version;		// version
	uint8_t						m_u8Reserved;		// reserved
	uint8_t						m_u8Response;		// response
	uint8_t						m_u8Res2;			// reserved2
	uint32_t					m_u32Error;			// error if any
	uint64_t					m_u64Arg2;			// arg2
	uint64_t					m_u64Arg3;			// third arg if can be binary
	uint32_t					m_u32Res1; 
	uint32_t					m_u32Res2; 
	uint32_t					m_u32Res3; 
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
	VxSha1Hash					m_FileHashId;
	char						m_as8Arg1[ (PKT_PLUGIN_OFFER_MAX_MSG_LEN + 1) ]; //message or argument
};

#pragma pack(pop)




