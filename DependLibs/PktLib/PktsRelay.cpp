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
// http://www.nolimitconnect.com
//============================================================================

#include "PktsRelay.h"

#include <CoreLib/IsBigEndianCpu.h>

//============================================================================
PktRelayServiceReq::PktRelayServiceReq()
{
	setPktLength( sizeof(PktRelayServiceReq) );		
	setPktType( PKT_TYPE_RELAY_SERVICE_REQ ); 
};

//============================================================================
PktRelayServiceReply::PktRelayServiceReply()
{
	setPktLength( sizeof(PktRelayServiceReply) );		
	setPktType( PKT_TYPE_RELAY_SERVICE_REPLY ); 
};

//============================================================================
PktRelayConnectToUserReq::PktRelayConnectToUserReq()
{
	setPktLength( sizeof(PktRelayConnectToUserReq) );		
	setPktType( PKT_TYPE_RELAY_CONNECT_TO_USER_REQ ); 
};

//============================================================================
PktRelayConnectToUserReply::PktRelayConnectToUserReply()
{
	setPktLength( sizeof(PktRelayConnectToUserReply) );		
	setPktType( PKT_TYPE_RELAY_CONNECT_TO_USER_REPLY ); 
};

//============================================================================
PktRelayUserDisconnect::PktRelayUserDisconnect()
{
	setPktLength( sizeof(PktRelayUserDisconnect) );		
	setPktType( PKT_TYPE_RELAY_USER_DISCONNECT ); 
};

//============================================================================
PktRelaySessionReq::PktRelaySessionReq()
{
	setPktType( PKT_TYPE_RELAY_SESSION_REQ ); 
	setPktLength( sizeof( PktRelaySessionReq ) );
}

//============================================================================
PktRelaySessionReply::PktRelaySessionReply()
{
	setPktType( PKT_TYPE_RELAY_SESSION_REPLY ); 
	setPktLength( sizeof( PktRelaySessionReply ) );
}

//============================================================================
PktRelayConnectReq::PktRelayConnectReq()
{
	setPktType( PKT_TYPE_RELAY_CONNECT_REQ ); 
	setPktLength( sizeof( PktRelayConnectReq ) );
}

//============================================================================
PktRelayConnectReply::PktRelayConnectReply()
{
	setPktType( PKT_TYPE_RELAY_CONNECT_REPLY ); 
	setPktLength( sizeof( PktRelayConnectReply ) );
}

//============================================================================
PktRelayTestReq::PktRelayTestReq()
{
	setPktType( PKT_TYPE_RELAY_TEST_REQ ); 
	setPktLength( sizeof( PktRelayTestReq ) );
}
//============================================================================
void PktRelayTestReq::setTimeStampMs( int64_t timeStamp )
{
	m_TimeStampMs = htonU64( timeStamp );
}

//============================================================================
int64_t PktRelayTestReq::getTimeStampMs( void )
{
	return ntohU64( m_TimeStampMs );
}

//============================================================================
PktRelayTestReply::PktRelayTestReply()
{
	setPktType( PKT_TYPE_RELAY_TEST_REPLY ); 
	setPktLength( sizeof( PktRelayTestReply ) );
}

//============================================================================
void PktRelayTestReply::setTimeStampMs( int64_t timeStamp )
{
	m_TimeStampMs = htonU64( timeStamp );
}

//============================================================================
int64_t PktRelayTestReply::getTimeStampMs( void )
{
	return ntohU64( m_TimeStampMs );
}

//============================================================================
void PktRelayTestReply::setMaxRelayUsers( uint32_t maxUsers )
{
	m_MaxRelayUsers = htonl( maxUsers );
}

//============================================================================
uint32_t PktRelayTestReply::getMaxRelayUsers( void )
{
	return ntohl( m_MaxRelayUsers );
}

//============================================================================
void PktRelayTestReply::setCurRelayUsers( uint32_t curUsers )
{
	m_CurRelayUsers = htonl( curUsers );
}

//============================================================================
uint32_t PktRelayTestReply::getCurRelayUsers( void )
{
	return ntohl( m_CurRelayUsers );
}
