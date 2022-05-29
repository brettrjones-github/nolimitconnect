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
// http://www.nolimitconnect.org
//============================================================================

#include "ConnectReqInfo.h"

#include <CoreLib/VxGlobals.h>

#include <memory.h>

//============================================================================
ConnectReqInfo::ConnectReqInfo( EConnectReason eConnectReason )
: m_eConnectReason( eConnectReason )
{
}

//============================================================================
ConnectReqInfo::ConnectReqInfo( VxConnectInfo& connectInfo, EConnectReason eConnectReason )
: m_eConnectReason( eConnectReason )
{
	m_ConnectInfo = connectInfo;
}

//============================================================================
ConnectReqInfo::ConnectReqInfo( const ConnectReqInfo &rhs )
{
	*this = rhs;
}

//============================================================================
ConnectReqInfo& ConnectReqInfo::operator =( const ConnectReqInfo& rhs )
{
	if( this != &rhs )
	{
		memcpy( this, &rhs, sizeof( ConnectReqInfo ) );
	}

	return *this;
}

//============================================================================
void ConnectReqInfo::setConnectReason( EConnectReason eConnectReason )
{
	m_eConnectReason = eConnectReason;
}

//============================================================================
EConnectReason ConnectReqInfo::getConnectReason( void )
{
	return m_eConnectReason;
}

//============================================================================
void ConnectReqInfo::setTimeLastConnectAttemptMs( uint64_t u32TimeSec )
{
	m_ConnectInfo.setTimeLastConnectAttemptMs( u32TimeSec );	
}

//============================================================================
uint64_t ConnectReqInfo::getTimeLastConnectAttemptMs( void )
{
	return m_ConnectInfo.getTimeLastConnectAttemptMs();	
}

//============================================================================
bool ConnectReqInfo::isTooSoonToAttemptConnectAgain( void )
{
	return m_ConnectInfo.isTooSoonToAttemptConnectAgain();
}


