//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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
// http://www.gotvptop.net
//============================================================================

#include "PktLogReq.h"

//============================================================================
PktLogReq::PktLogReq()
: m_u16Res(0)
, m_u32Enable(0)
{
	setPktType( PKT_TYPE_LOG_REQ );
	setPktLength( sizeof( PktLogReq ) );
}

//============================================================================
void PktLogReq::setEnable( bool enable )
{
	m_u32Enable = 	enable ? htonl( 1 ) : 0;
}

//============================================================================
bool PktLogReq::isEnable( void )
{
	return m_u32Enable ? true : false;
}
