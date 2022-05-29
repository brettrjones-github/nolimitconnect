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
// http://www.nolimitconnect.org
//============================================================================

#include "VxConnectInfo.h"
#include "VxNetIdentBase.h"

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>

#include <memory.h>
#include <stdio.h>

//============================================================================
VxNetIdentBase::VxNetIdentBase()
: VxConnectInfo()
{
}

//============================================================================
VxNetIdentBase::VxNetIdentBase( const VxNetIdentBase &rhs )
{
	*this = rhs;
}

//============================================================================
//! return true if identity matches
bool	VxNetIdentBase::isVxNetIdentMatch( const VxNetIdentBase& oOtherIdent ) const
{
	return ( *((VxGUID *)&oOtherIdent.m_DirectConnectId) == *((VxGUID *)&m_DirectConnectId));
}

//============================================================================
void VxNetIdentBase::useAsRelay( VxNetIdentBase * poRelayIdent )
{
	m_RelayConnectId = poRelayIdent->m_DirectConnectId;
}

//============================================================================
bool VxNetIdentBase::operator ==( const VxNetIdentBase &a ) const
{
	return this->isVxNetIdentMatch( a );
}

//============================================================================
void VxNetIdentBase::getProfileUri( std::string& retUri, const char * myIp, const char * webFileName )
{
	std::string onlineId;
	uint16_t u16Port;
	InetAddress addr;
	std::string ipAddr = addr.toStdString();
	if( hasRelay() && requiresRelay() )
	{
		u16Port = getRelayPort();
		addr = getRelayIpAddress();
		ipAddr = addr.toStdString();
	}
	else
	{
		u16Port = getOnlinePort();
		ipAddr = myIp;
	}

	char as8Port[ 20 ];
	sprintf( as8Port, "%d", u16Port );
	getMyOnlineId( onlineId );

	retUri = "http://";
	retUri += ipAddr;
	retUri += ":";
	retUri += as8Port;
	retUri += "/";
	retUri += onlineId;
	retUri += "/";
	retUri	+= webFileName;
}

//============================================================================
//! not equal operator
bool VxNetIdentBase::operator != ( const VxNetIdentBase &a ) const
{
	return !(this->isVxNetIdentMatch( a ));
}

//============================================================================
//! assignment operator
VxNetIdentBase& VxNetIdentBase::operator =( const VxNetIdentBase &rhs )
{
	// Check for self-assignment!
    if (this == &rhs)      // Same object?
		return *this;        // Yes, so skip assignment, and just return *this.
	// we can get away with memcpy because no virtual functions
    memcpy( this, &rhs, sizeof( VxNetIdentBase ) );
	return *this;
}

//============================================================================
void VxNetIdentBase::setDareCount( uint32_t dareCnt )
{
	m_u32DareCnt = htonl( dareCnt );
}

//============================================================================
uint32_t	VxNetIdentBase::getDareCount()
{
	return ntohl( m_u32DareCnt );
}

//============================================================================
void VxNetIdentBase::setRejectedTruthCount( uint16_t rejectedCnt )
{
	m_u16RejectedTruthsCnt = htons( rejectedCnt );
}

//============================================================================
uint16_t	VxNetIdentBase::getRejectedTruthCount()
{
	return ntohs( m_u16RejectedTruthsCnt );
}

//============================================================================
void VxNetIdentBase::setRejectedDareCount( uint16_t rejectedCnt )
{
	m_u16RejectedDaresCnt = htons( rejectedCnt );
}

//============================================================================
uint16_t	VxNetIdentBase::getRejectedDareCount()
{
	return ntohs( m_u16RejectedDaresCnt );
}

//============================================================================
void VxNetIdentBase::setTruthCount( uint32_t truthCnt )
{
	m_u32TruthCnt = htonl( truthCnt );
}

//============================================================================
uint32_t	VxNetIdentBase::getTruthCount( void )
{
	return ntohl( m_u32TruthCnt );
}
