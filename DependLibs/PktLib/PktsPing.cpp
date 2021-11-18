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
// http://www.nolimitconnect.com
//============================================================================

#include "PktsPing.h"

#include <CoreLib/IsBigEndianCpu.h>

//============================================================================
PktPingReq::PktPingReq()
{
	setPktType( PKT_TYPE_PING_REQ );
	setPktLength( sizeof( PktPingReq ) );
}

//============================================================================
void PktPingReq::setTimestamp( uint64_t timeStamp )
{
	m_Timestamp = htonU64( timeStamp );
}

//============================================================================
uint64_t PktPingReq::getTimestamp( void )
{
	return ntohU64( m_Timestamp );
}

//============================================================================
PktPingReply::PktPingReply()
{
	setPktType( PKT_TYPE_PING_REPLY );
	setPktLength( sizeof( PktPingReq ) );
}

//============================================================================
void PktPingReply::setTimestamp( uint64_t timeStamp )
{
	m_Timestamp = htonU64( timeStamp );
}

//============================================================================
uint64_t PktPingReply::getTimestamp( void )
{
	return ntohU64( m_Timestamp );
}
