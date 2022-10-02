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

#include "VxConnectId.h"
#include "PktBlobEntry.h"

#include <NetLib/VxSktUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>

#include <memory.h>

//============================================================================
VxConnectId::VxConnectId( const VxConnectId &rhs )
: VxGUID(rhs)
, m_IPv6OnlineIp( rhs.m_IPv6OnlineIp )
, m_IPv4OnlineIp( rhs.m_IPv4OnlineIp )
, m_u16OnlinePort( rhs.m_u16OnlinePort )
{
}

//============================================================================
bool VxConnectId::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( *( (VxGUID *)this ) );
    result &= m_IPv6OnlineIp.addToBlob( blob );
    result &= m_IPv4OnlineIp.addToBlob( blob );
    result &= blob.setValue( m_u16OnlinePort );
	result &= blob.setValue( m_u16ResConnectId1 );
    return result;
}

//============================================================================
bool VxConnectId::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( *( (VxGUID *)this ) );
    result &= m_IPv6OnlineIp.extractFromBlob( blob );
    result &= m_IPv4OnlineIp.extractFromBlob( blob );
    result &= blob.getValue( m_u16OnlinePort );
	result &= blob.getValue( m_u16ResConnectId1 );
    return result;
}

//============================================================================
VxConnectId& VxConnectId::operator =( const VxConnectId &rhs )
{
	if( this != &rhs )
	{
        *((VxGUID*)this) = *((VxGUID*)&rhs);
        m_IPv6OnlineIp = rhs.m_IPv6OnlineIp;
        m_IPv4OnlineIp = rhs.m_IPv4OnlineIp;
        m_u16OnlinePort = rhs.m_u16OnlinePort;
		m_u16ResConnectId1 = rhs.m_u16ResConnectId1;
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
bool VxConnectId::setIpAddress( const char* pIp )		
{
    bool changed = false;
	InetAddress oAddr;
	oAddr.setIp( pIp );
	if( oAddr.isValid() && oAddr.isIPv6() )
	{
		m_IPv6OnlineIp = oAddr; 
	}
	else if( oAddr.isValid() )
	{
        changed = m_IPv4OnlineIp.setIp( oAddr.getIPv4AddressInNetOrder() );
	}
	else
	{
		vx_assert( false );
	}

    return changed;
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
bool VxConnectId::isIpAddressValid( void )
{
	return m_IPv4OnlineIp.isValid() || m_IPv6OnlineIp.isValid();
}

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
