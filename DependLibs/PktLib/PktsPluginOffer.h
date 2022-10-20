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
#include "PktBlobEntry.h"

#pragma pack(push)
#pragma pack(1)

class PktPluginOfferReq : public VxPktHdr
{
public:
	PktPluginOfferReq();

	void						calcPktLen( void );

	void						setPluginType( EPluginType pluginType ) { m_PluginType = (uint8_t)pluginType; }
	EPluginType					getPluginType( void )					{ return (EPluginType)m_PluginType; }

	void						setLclSessionId( VxGUID& lclId )		{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )					{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )		{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )					{ return m_RmtSessionId; }

	PktBlobEntry&				getBlobEntry( void )					{ return m_BlobEntry; }

private:
	//=== vars ===//
	// VxPktHdr 40 bytes 
	uint8_t						m_u8Version{ 1 };		// version
	uint8_t						m_u8Response{ 0 };		// response
	uint8_t						m_PluginType{ 0 };		
	uint8_t						m_Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };			// reserved

	VxGUID						m_LclSessionId;  // 16 bytes
	VxGUID						m_RmtSessionId;  // 16 bytes

	PktBlobEntry                m_BlobEntry;	// size 14352
};

class PktPluginOfferReply : public VxPktHdr
{
public:
	PktPluginOfferReply();

	void						calcPktLen( void );

	void						setPluginType( EPluginType pluginType )			{ m_PluginType = (uint8_t)pluginType; }
	EPluginType					getPluginType( void )							{ return (EPluginType)m_PluginType; }

	void						setLclSessionId( VxGUID& lclId )				{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )							{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )				{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )							{ return m_RmtSessionId; }

	void						setOfferResponse( EOfferResponse eResponse )	{ m_u8Response = (uint8_t)eResponse; }
	EOfferResponse				getOfferResponse( void )						{ return (EOfferResponse)m_u8Response; }

	PktBlobEntry&				getBlobEntry( void )							{ return m_BlobEntry; }

private:
	uint8_t						m_u8Version{ 1 };		// version
	uint8_t						m_u8Response{ 0 };		// response
	uint8_t						m_PluginType{ 0 };
	uint8_t						m_Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };			// reserved

	VxGUID						m_LclSessionId;  // 16 bytes
	VxGUID						m_RmtSessionId;  // 16 bytes

	PktBlobEntry                m_BlobEntry;	// size 14352
};

#pragma pack(pop)




