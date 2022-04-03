#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones 
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

#include "VxPktHdr.h"

#pragma pack(push)
#pragma pack(1)

class PktMultiSessionReq : public VxPktHdr
{
public:
	PktMultiSessionReq();

	void						        setMSessionAction( uint32_t action )		{ m_u32Action = htonl( action ); }
	uint32_t							getMSessionAction( void )			        { return ntohl( m_u32Action ); }
	void						        setMSessionParam( uint32_t param )		    { m_u32ActionParam = htonl( param ); }
	uint32_t							getMSessionParam( void )			        { return ntohl( m_u32ActionParam ); }

private:
	//=== vars ===//
	uint32_t							m_u32Action;
	uint32_t							m_u32ActionParam;
	uint32_t							m_u32Res3;
	uint32_t							m_u32Res4;
	uint32_t							m_u32Res5;
	uint32_t							m_u32Res6;
};

class PktMultiSessionReply : public VxPktHdr
{
public:
	PktMultiSessionReply();

	void						        setMSessionAction( uint32_t action )		{ m_u32Action = htonl( action ); }
	uint32_t							getMSessionAction( void )			        { return ntohl( m_u32Action ); }
	void						        setMSessionParam( uint32_t param )		    { m_u32ActionParam = htonl( param ); }
	uint32_t							getMSessionParam( void )			        { return ntohl( m_u32ActionParam ); }

private:
	//=== vars ===//
	uint32_t							m_u32Action;
	uint32_t							m_u32ActionParam;
	uint32_t							m_u32Res3;
	uint32_t							m_u32Res4;
	uint32_t							m_u32Res5;
	uint32_t							m_u32Res6;
};

#pragma pack(pop)

