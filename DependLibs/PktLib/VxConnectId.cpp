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
// http://www.gotvptop.com
//============================================================================

#include "VxConnectId.h"

#include <NetLib/VxSktUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>

#include <memory.h>

//============================================================================
VxConnectId::VxConnectId()
    : m_IPv6OnlineIp()
    , m_IPv4OnlineIp()
    , m_u16OnlinePort(0)
{
}

//============================================================================
VxConnectId::VxConnectId( const VxConnectId &rhs )
: VxGUID()
{
	*this = rhs;
}

//============================================================================
VxConnectId& VxConnectId::operator =( const VxConnectId &rhs )
{
	if( this != &rhs )
	{
		// we can get away with memcpy because no virtual functions
		memcpy( this, &rhs, sizeof( VxConnectId ) );
	}
	return *this;
}

//============================================================================
bool VxConnectId::operator ==( const VxConnectId &rhs )  const
{
	return ( 0 == memcmp( this, &rhs, sizeof( VxConnectId )) );
}

//============================================================================
bool VxConnectId::operator !=( const VxConnectId &rhs )  const
{
	return ( 0 != memcmp( this, &rhs, sizeof( VxConnectId )) );
}

//============================================================================
void VxConnectId::setIpAddress( const char * pIp )		
{
	InetAddress oAddr;
	oAddr.setIp( pIp );
	if( oAddr.isValid() && oAddr.isIPv6() )
	{
		m_IPv6OnlineIp = oAddr; 
	}
	else if( oAddr.isValid() )
	{
		m_IPv4OnlineIp.setIp( oAddr.getIPv4AddressInNetOrder() );
	}
	else
	{
		vx_assert( false );
	}
};

//============================================================================
void VxConnectId::getIpAddress( std::string& retString )
{
	if( m_IPv6OnlineIp.isValid() && VxCanConnectUsingIPv6() )
	{
		retString = m_IPv6OnlineIp.toStdString();
	}

	retString = m_IPv4OnlineIp.toStdString(); 
};

//============================================================================
void VxConnectId::getIPv4( std::string& retString )	
{ 
	retString = m_IPv4OnlineIp.toStdString(); 
};

//============================================================================
void VxConnectId::getIPv6( std::string& retString )
{
	retString = m_IPv6OnlineIp.toStdString(); 
}

//============================================================================
void VxConnectId::clear( void )
{
	clearVxGUID();
	m_u16OnlinePort = 0;
	m_IPv4OnlineIp.setToInvalid();
	m_IPv6OnlineIp.setToInvalid();
}
