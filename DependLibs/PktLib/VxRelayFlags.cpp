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
#include "VxRelayFlags.h"
#include "PktBlobEntry.h"

#define RC_PROXY_FLAG_REQUIRES_PROXY			0x01	// user requires proxy to connect to him
#define RC_PROXY_FLAG_HAS_PROXY					0x02	// user has at least one proxy that may be used to connect to him
#define RC_PROXY_FLAG_IS_MY_PREFERED_PROXY		0x04	// user is a proxy for others
#define RC_PROXY_FLAG_USE_ENCRYPTION			0x08	// user wants encrypted traffic

//============================================================================
VxRelayFlags::VxRelayFlags( const VxRelayFlags& rhs )
: m_u8RelayFlags( rhs.m_u8RelayFlags )
{
}

//============================================================================
VxRelayFlags& VxRelayFlags::operator =( const VxRelayFlags& rhs )
{
    if( this != &rhs )
    {
        m_u8RelayFlags = rhs.m_u8RelayFlags;
    }

    return *this;
}

//============================================================================
bool VxRelayFlags::addToBlob( PktBlobEntry& blob )
{
    return blob.setValue( m_u8RelayFlags );
}

//============================================================================
bool VxRelayFlags::extractFromBlob( PktBlobEntry& blob )
{
    return blob.getValue( m_u8RelayFlags );
}

//============================================================================
void VxRelayFlags::setRelayFlags( uint8_t u8RelayFlags )
{
	m_u8RelayFlags = u8RelayFlags;
}

//============================================================================
uint8_t VxRelayFlags::getRelayFlags( void )
{
	return m_u8RelayFlags;
}

//============================================================================

bool VxRelayFlags::hasRelay( void )						
{ 
	return (RC_PROXY_FLAG_HAS_PROXY & m_u8RelayFlags)?1:0; 
}

//============================================================================
void	VxRelayFlags::setHasRelay( bool bHasRelay )				
{
	if( bHasRelay )
		m_u8RelayFlags |= RC_PROXY_FLAG_HAS_PROXY;
	else
		m_u8RelayFlags &= ~RC_PROXY_FLAG_HAS_PROXY;
}

//============================================================================
bool	VxRelayFlags::requiresRelay( void )					
{ 
	return (RC_PROXY_FLAG_REQUIRES_PROXY & m_u8RelayFlags) ? true : false; 
}

//============================================================================
void	VxRelayFlags::setRequiresRelay( bool bRequiresRelay )				
{
	if( bRequiresRelay )
		m_u8RelayFlags |= RC_PROXY_FLAG_REQUIRES_PROXY;
	else
		m_u8RelayFlags &= ~RC_PROXY_FLAG_REQUIRES_PROXY;
}

//============================================================================
bool	VxRelayFlags::isMyPreferedRelay( void )					
{ 
	return (RC_PROXY_FLAG_IS_MY_PREFERED_PROXY & m_u8RelayFlags)?1:0; 
}

//============================================================================
void	VxRelayFlags::setIsMyPreferedRelay( bool bRequiresRelay )				
{
	if( bRequiresRelay )
		m_u8RelayFlags |= RC_PROXY_FLAG_IS_MY_PREFERED_PROXY;
	else
		m_u8RelayFlags &= ~RC_PROXY_FLAG_IS_MY_PREFERED_PROXY;
}