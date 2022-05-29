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
// http://www.nolimitconnect.org
//============================================================================

#include "PktTcpPunch.h"

//============================================================================
PktTcpPunch::PktTcpPunch()
: m_ConnectInfo()
, m_u32Res1( 0 )
, m_u32Res2( 0 )
, m_u32Res3( 0 )
, m_u32Res4( 0 )
, m_u32Res5( 0 )
, m_u32Res6( 0 )
{
	setPktLength( sizeof(PktTcpPunch) );		
	setPktType( PKT_TYPE_TCP_PUNCH ); 
};
