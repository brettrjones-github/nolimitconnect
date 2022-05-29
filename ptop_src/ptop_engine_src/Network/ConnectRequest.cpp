//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include "ConnectRequest.h"

#include <CoreLib/VxGlobals.h>

#include <memory.h>

//============================================================================
ConnectRequest::ConnectRequest( EConnectReason eConnectReason )
: m_eConnectReason( eConnectReason )
{
}

//============================================================================
ConnectRequest::ConnectRequest( VxConnectInfo& connectInfo, EConnectReason eConnectReason )
: m_eConnectReason( eConnectReason )
{
	m_ConnectInfo = connectInfo;
}

//============================================================================
ConnectRequest::ConnectRequest( const ConnectRequest &rhs )
{
	*this = rhs;
}

//============================================================================
ConnectRequest& ConnectRequest::operator =( const ConnectRequest& rhs )
{
	if( this != &rhs )
	{
		memcpy( this, &rhs, sizeof( ConnectRequest ) );
	}

	return *this;
}

//============================================================================
void ConnectRequest::setConnectReason( EConnectReason eConnectReason )
{
	m_eConnectReason = eConnectReason;
}

//============================================================================
EConnectReason ConnectRequest::getConnectReason( void )
{
	return m_eConnectReason;
}

//============================================================================
void ConnectRequest::setTimeLastConnectAttemptMs( uint64_t u32TimeSec )
{
	m_ConnectInfo.setTimeLastConnectAttemptMs( u32TimeSec );	
}

//============================================================================
uint64_t ConnectRequest::getTimeLastConnectAttemptMs( void )
{
	return m_ConnectInfo.getTimeLastConnectAttemptMs();	
}

//============================================================================
bool ConnectRequest::isTooSoonToAttemptConnectAgain( void )
{
	return m_ConnectInfo.isTooSoonToAttemptConnectAgain();
}


