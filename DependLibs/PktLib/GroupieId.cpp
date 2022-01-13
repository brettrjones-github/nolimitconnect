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
    , m_HostOnlineId( hostOnlineId )
    , m_HostType( hostType )
{
}

//============================================================================
GroupieId::GroupieId( const GroupieId& rhs )
    : m_GroupieOnlineId(rhs.m_GroupieOnlineId)
    , m_HostOnlineId(rhs.m_HostOnlineId)
    , m_HostType( rhs.m_HostType )
{
}

//============================================================================
GroupieId& GroupieId::operator =( const GroupieId& rhs )
{
	if( this != &rhs )
	{
        m_GroupieOnlineId         = rhs.m_GroupieOnlineId;
        m_HostOnlineId            = rhs.m_HostOnlineId;
        m_HostType                = rhs.m_HostType;
	}

	return *this;
}

//============================================================================
bool GroupieId::operator == ( const GroupieId& rhs ) const
{
    return ( m_GroupieOnlineId == rhs.m_GroupieOnlineId ) && ( m_HostOnlineId == rhs.m_HostOnlineId ) && ( m_HostType == rhs.m_HostType );
}

//============================================================================
bool GroupieId::operator != ( const GroupieId& rhs ) const
{
    return !(*this == rhs);
}

//============================================================================
bool GroupieId::operator < ( const GroupieId& rhs ) const
{
    if( m_HostOnlineId < rhs.m_HostOnlineId )
        return true;
    if( m_HostOnlineId > rhs.m_HostOnlineId )
        return false;

    if( m_GroupieOnlineId < rhs.m_GroupieOnlineId )
        return true;
    return false;
}

//============================================================================
bool GroupieId::operator <= ( const GroupieId& rhs ) const
{
    if( *this == rhs )
    {
        return true;
    }

    if( m_HostOnlineId < rhs.m_HostOnlineId )
        return true;
    if( m_HostOnlineId > rhs.m_HostOnlineId )
        return false;

    if( m_GroupieOnlineId < rhs.m_GroupieOnlineId )
        return true;

    return false;
}

//============================================================================
bool GroupieId::operator > ( const GroupieId& rhs ) const
{
    if( m_HostOnlineId > rhs.m_HostOnlineId )
        return true;
    if( m_HostOnlineId < rhs.m_HostOnlineId )
        return false;

    if( m_GroupieOnlineId > rhs.m_GroupieOnlineId )
        return true;

    return false;
}

//============================================================================
bool GroupieId::operator >= ( const GroupieId& rhs ) const
{
    if( *this == rhs )
    {
        return true;
    }

    if( m_HostOnlineId > rhs.m_HostOnlineId )
        return true;
    if( m_HostOnlineId < rhs.m_HostOnlineId )
        return false;

    if( m_GroupieOnlineId > rhs.m_GroupieOnlineId )
        return true;

    return false;
}

//============================================================================
bool GroupieId::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_GroupieOnlineId );
    result &= blob.setValue( m_HostOnlineId );
    return result;
}

//============================================================================
bool GroupieId::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_GroupieOnlineId );
    result &= blob.getValue( m_HostOnlineId );
    return result;
}

// returns 0 if equal else -1 if less or 1 if greater
//============================================================================
int GroupieId::compareTo( GroupieId& groupieId )
{
    int result = 0;
    if( m_HostOnlineId > groupieId.m_HostOnlineId )
    {
        result = 1;
    }
    else if( m_HostOnlineId < groupieId.m_HostOnlineId )
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
    desc += " host id ";
    desc += m_HostOnlineId.toOnlineIdString();
    desc += " host type ";
    desc += DescribeHostType( m_HostType );
    return desc;
}
