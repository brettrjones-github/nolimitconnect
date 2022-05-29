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

#include "ConnectId.h"
#include "PktBlobEntry.h"

//============================================================================
ConnectId::ConnectId( VxGUID& socketId )
    : m_SocketId( socketId )
    , m_GroupieId()
{
}

//============================================================================
ConnectId::ConnectId( VxGUID& socketId, GroupieId& groupieId )
    : m_SocketId( socketId )
    , m_GroupieId( groupieId )
{
}

//============================================================================
ConnectId::ConnectId( VxGUID& socketId, VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
    : m_SocketId( socketId )
    , m_GroupieId( groupieOnlineId, hostOnlineId, hostType )
{
}

//============================================================================
ConnectId::ConnectId( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
    : m_SocketId()
    , m_GroupieId( groupieOnlineId, hostOnlineId, hostType )
{
}

//============================================================================
ConnectId::ConnectId( VxGUID& groupieOnlineId, HostedId& hostedId )
    : m_SocketId()
    , m_GroupieId( groupieOnlineId, hostedId )
{
}

//============================================================================
ConnectId::ConnectId( const ConnectId& rhs )
    : m_SocketId( rhs.m_SocketId )
    , m_GroupieId( rhs.m_GroupieId )
{
}

//============================================================================
ConnectId& ConnectId::operator =( const ConnectId& rhs )
{
	if( this != &rhs )
	{
        m_SocketId                  = rhs.m_SocketId;
        m_GroupieId                 = rhs.m_GroupieId;
	}

	return *this;
}

//============================================================================
bool ConnectId::operator == ( const ConnectId& rhs ) const
{
    return m_SocketId == rhs.m_SocketId &&  m_GroupieId == rhs.m_GroupieId;
}

//============================================================================
bool ConnectId::operator != ( const ConnectId& rhs ) const
{
    return !(*this == rhs);
}

//============================================================================
bool ConnectId::operator < ( const ConnectId& rhs ) const
{
    return m_SocketId < rhs.m_SocketId || ( m_SocketId == rhs.m_SocketId && m_GroupieId < rhs.m_GroupieId );
}

//============================================================================
bool ConnectId::operator <= ( const ConnectId& rhs ) const
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
bool ConnectId::operator > ( const ConnectId& rhs ) const
{
    return m_SocketId > rhs.m_SocketId || ( m_SocketId == rhs.m_SocketId && m_GroupieId > rhs.m_GroupieId );
}

//============================================================================
bool ConnectId::operator >= ( const ConnectId& rhs ) const
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
bool ConnectId::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_SocketId );
    result &= m_GroupieId.addToBlob( blob );
    return result;
}

//============================================================================
bool ConnectId::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_SocketId );
    result &= m_GroupieId.extractFromBlob( blob );
    return result;
}

// returns 0 if equal else -1 if less or 1 if greater
//============================================================================
int ConnectId::compareTo( ConnectId& connectId )
{
    int result = 0;
    if( *this > connectId )
    {
        result = 1;
    }
    else if( *this < connectId )
    {
        result = -1;
    }

    if( 0 == result )
    {
        result = m_SocketId.compareTo( connectId.getSocketId() );
    }

    return result;
}

// returns true if guids are same value
//============================================================================
bool ConnectId::isEqualTo( const ConnectId& groupieId )
{
    return *this == groupieId;
}

// get a description of the plugin id
//============================================================================
std::string ConnectId::describeConnectId( void ) const
{
    std::string desc = "skt id ";
    desc += m_SocketId.toOnlineIdString();
    desc += " ";
    desc += m_GroupieId.describeGroupieId();
    return desc;
}
