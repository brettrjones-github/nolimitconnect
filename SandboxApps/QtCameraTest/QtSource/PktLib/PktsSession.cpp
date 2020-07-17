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

#include "PktsSession.h"

//============================================================================
PktSessionStartReq::PktSessionStartReq()
: m_u32Res1(0)
, m_u32Res2(0)
{
	setPktLength( sizeof( PktSessionStartReq ) ); 
	setPktType(  PKT_TYPE_SESSION_START_REQ );
}

//============================================================================
PktSessionStartReply::PktSessionStartReply()
: m_u16Response(0)
, m_u16Res1(0)
, m_u32Res2(0)
{ 
	setPktLength( sizeof( PktSessionStartReply ) ); 
	setPktType(  PKT_TYPE_SESSION_START_REPLY );
}

//============================================================================
PktSessionStopReq::PktSessionStopReq()
	: m_u32Res1(0)
	, m_u32Res2(0)
{
	setPktLength( sizeof( PktSessionStopReq ) ); 
	setPktType(  PKT_TYPE_SESSION_STOP_REQ );
}

//============================================================================
PktSessionStopReply::PktSessionStopReply()
	: m_u16Response(0)
	, m_u16Res1(0)
	, m_u32Res2(0)
{ 
	setPktLength( sizeof( PktSessionStopReply ) ); 
	setPktType(  PKT_TYPE_SESSION_STOP_REPLY );
}

