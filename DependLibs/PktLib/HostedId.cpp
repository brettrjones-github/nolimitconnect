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

#include "HostedId.h"
#include "PktBlobEntry.h"

//============================================================================
HostedId::HostedId( VxGUID& onlineId, EHostType hostType )
    : m_OnlineId(onlineId)
    , m_HostType(hostType)
{
}

//============================================================================
HostedId::HostedId( const HostedId& rhs )
    : m_OnlineId(rhs.m_OnlineId)
    , m_HostType(rhs.m_HostType)
{
}

//============================================================================
HostedId& HostedId::operator =( const HostedId& rhs )
{
	if( this != &rhs )
	{
        m_OnlineId            = rhs.m_OnlineId;
        m_HostType            = rhs.m_HostType;
	}

	return *this;
}

//============================================================================
bool HostedId::operator == ( const HostedId& rhs ) const
{
    return ( m_OnlineId == rhs.m_OnlineId ) && ( m_HostType == rhs.m_HostType );
}

//============================================================================
bool HostedId::operator != ( const HostedId& rhs ) const
{
    return !(*this == rhs);
}

//============================================================================
bool HostedId::operator < ( const HostedId& rhs ) const
{
    return m_OnlineId < rhs.m_OnlineId || ( m_OnlineId == rhs.m_OnlineId && m_HostType < rhs.m_HostType );
}

//============================================================================
bool HostedId::operator <= ( const HostedId& rhs ) const
{
    if( *this == rhs )
    {
        return true;
    }

    if( *this < rhs )
    {
        return true;
    }

    return false;
}

//============================================================================
bool HostedId::operator > ( const HostedId& rhs ) const
{
    return m_OnlineId > rhs.m_OnlineId || ( m_OnlineId == rhs.m_OnlineId && m_HostType > rhs.m_HostType );
}

//============================================================================
bool HostedId::operator >= ( const HostedId& rhs ) const
{
    if( *this == rhs )
    {
        return true;
    }

    if( *this > rhs )
    {
        return true;
    }

    return false;
}

//============================================================================
bool HostedId::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_OnlineId );
    result &= blob.setValue( m_HostType );
    return result;
}

//============================================================================
bool HostedId::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_OnlineId );
    result &= blob.getValue( m_HostType );
    return result;
}

// returns 0 if equal else -1 if less or 1 if greater
//============================================================================
int HostedId::compareTo( HostedId& pluginId )
{
    int result = 0;
    if( m_HostType > pluginId.m_HostType )
    {
        result = 1;
    }
    else if( m_HostType < pluginId.m_HostType )
    {
        result = -1;
    }

    if( 0 == result )
    {
        result = m_OnlineId.compareTo( pluginId.getOnlineId() );
    }

    return result;
}

// returns true if guids are same value
//============================================================================
bool HostedId::isEqualTo( const HostedId& pluginId )
{
    return *this == pluginId;
}

// get a description of the plugin id
//============================================================================
std::string HostedId::describeHostedId( void ) const
{
    std::string desc = DescribeHostType( getHostType() );
    desc += m_OnlineId.toOnlineIdString();
    return desc;
}
