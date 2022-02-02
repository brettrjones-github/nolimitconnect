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

#include "GroupieId.h"
#include "PktBlobEntry.h"

//============================================================================
GroupieId::GroupieId( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
    : m_GroupieOnlineId( groupieOnlineId )
    , m_HostedId( hostOnlineId, hostType )
{
}

//============================================================================
GroupieId::GroupieId( VxGUID& groupieOnlineId, HostedId& hostedId )
    : m_GroupieOnlineId( groupieOnlineId )
    , m_HostedId( hostedId )
{
}

//============================================================================
GroupieId::GroupieId( const GroupieId& rhs )
    : m_GroupieOnlineId( rhs.m_GroupieOnlineId )
    , m_HostedId( rhs.m_HostedId )
{
}

//============================================================================
GroupieId& GroupieId::operator =( const GroupieId& rhs )
{
	if( this != &rhs )
	{
        m_GroupieOnlineId         = rhs.m_GroupieOnlineId;
        m_HostedId                = rhs.m_HostedId;
	}

	return *this;
}

//============================================================================
bool GroupieId::operator == ( const GroupieId& rhs ) const
{
    return m_GroupieOnlineId == rhs.m_GroupieOnlineId &&  m_HostedId == rhs.m_HostedId;
}

//============================================================================
bool GroupieId::operator != ( const GroupieId& rhs ) const
{
    return !(*this == rhs);
}

//============================================================================
bool GroupieId::operator < ( const GroupieId& rhs ) const
{
    return m_GroupieOnlineId < rhs.m_GroupieOnlineId || ( m_GroupieOnlineId == rhs.m_GroupieOnlineId && m_HostedId < rhs.m_HostedId );
}

//============================================================================
bool GroupieId::operator <= ( const GroupieId& rhs ) const
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
bool GroupieId::operator > ( const GroupieId& rhs ) const
{
    return m_GroupieOnlineId > rhs.m_GroupieOnlineId || ( m_GroupieOnlineId == rhs.m_GroupieOnlineId && m_HostedId > rhs.m_HostedId );
}

//============================================================================
bool GroupieId::operator >= ( const GroupieId& rhs ) const
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
bool GroupieId::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_GroupieOnlineId );
    result &= m_HostedId.addToBlob( blob );
    return result;
}

//============================================================================
bool GroupieId::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_GroupieOnlineId );
    result &= m_HostedId.extractFromBlob( blob );
    return result;
}

// returns 0 if equal else -1 if less or 1 if greater
//============================================================================
int GroupieId::compareTo( GroupieId& groupieId )
{
    int result = 0;
    if( *this > groupieId )
    {
        result = 1;
    }
    else if( *this < groupieId )
    {
        result = -1;
    }

    if( 0 == result )
    {
        result = m_GroupieOnlineId.compareTo( groupieId.getGroupieOnlineId() );
    }

    return result;
}

// returns true if guids are same value
//============================================================================
bool GroupieId::isEqualTo( const GroupieId& groupieId )
{
    return *this == groupieId;
}

// get a description of the plugin id
//============================================================================
std::string GroupieId::describeGroupieId( void ) const
{
    std::string desc = "user id ";
    desc += m_GroupieOnlineId.toOnlineIdString();
    desc += " ";
    desc += m_HostedId.describeHostedId();
    return desc;
}
