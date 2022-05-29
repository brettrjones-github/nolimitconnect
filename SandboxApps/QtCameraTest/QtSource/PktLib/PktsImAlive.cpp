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
// http://www.nolimitconnect.org
//============================================================================

#include "PktsImAlive.h"

#include <CoreLib/IsBigEndianCpu.h>

//============================================================================
PktImAliveReq::PktImAliveReq()
: m_Res(0)
{
	setPktType( PKT_TYPE_IM_ALIVE_REQ );
	setPktLength( sizeof( PktImAliveReq ) );
}

////============================================================================
//void PktImAliveReq::setTimestamp( uint64_t timeStamp )
//{
//	m_Timestamp = htonU64( timeStamp );
//}
//
////============================================================================
//uint64_t PktImAliveReq::getTimestamp( void )
//{
//	return ntohU64( m_Timestamp );
//}

//============================================================================
PktImAliveReply::PktImAliveReply()
: m_Res(0)
{
	setPktType( PKT_TYPE_IM_ALIVE_REPLY );
	setPktLength( sizeof( PktImAliveReq ) );
}

////============================================================================
//void PktImAliveReply::setTimestamp( uint64_t timeStamp )
//{
//	m_Timestamp = htonU64( timeStamp );
//}
//
////============================================================================
//uint64_t PktImAliveReply::getTimestamp( void )
//{
//	return ntohU64( m_Timestamp );
//}
