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
// http://www.nolimitconnect.com
//============================================================================

#include "PktTypes.h"
#include "VxCommon.h"

#pragma pack(push)
#pragma pack(1)

class PktSessionStartReq : public VxPktHdr
{
public:
	PktSessionStartReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

private:
	//=== vars ===//
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktSessionStartReply : public VxPktHdr
{
public:
    PktSessionStartReply();

    void						setOfferResponse( enum EOfferResponse eResponse ){ m_u16Response = (uint16_t)eResponse; }
	EOfferResponse				getOfferResponse( void )					{ return (EOfferResponse)m_u16Response; }

private:
	//=== vars ===//
    uint16_t					m_u16Response{ 0 };
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktSessionStopReq : public VxPktHdr
{
public:
	PktSessionStopReq();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

private:
	//=== vars ===//
	uint32_t					m_u32Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

class PktSessionStopReply : public VxPktHdr
{
public:
	PktSessionStopReply();

	void						setLclSessionId( VxGUID& lclId )			{ m_LclSessionId = lclId; }
	VxGUID&						getLclSessionId( void )						{ return m_LclSessionId; }
	void						setRmtSessionId( VxGUID& rmtId )			{ m_RmtSessionId = rmtId; }
	VxGUID&						getRmtSessionId( void )						{ return m_RmtSessionId; }

    void						setOfferResponse( enum EOfferResponse eResponse ){ m_u16Response = (uint16_t)eResponse; }
	EOfferResponse				getOfferResponse( void )					{ return (EOfferResponse)m_u16Response; }

private:
	//=== vars ===//
	uint16_t					m_u16Response{ 0 };		
	uint16_t					m_u16Res1{ 0 };
	uint32_t					m_u32Res2{ 0 };
	VxGUID						m_LclSessionId;
	VxGUID						m_RmtSessionId;
};

#pragma pack(pop)

