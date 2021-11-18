//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "PktsNearby.h"

#include <CoreLib/IsBigEndianCpu.h>

//============================================================================
PktNearbyReq::PktNearbyReq()
{
	setPktType( PKT_TYPE_NEARBY_REQ );
	setPktLength( sizeof( PktNearbyReq ) );
}

//============================================================================
void PktNearbyReq::setTimestamp( uint64_t timeStamp )
{
	m_Timestamp = htonU64( timeStamp );
}

//============================================================================
uint64_t PktNearbyReq::getTimestamp( void )
{
	return ntohU64( m_Timestamp );
}
