#pragma once
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

#include "GroupieId.h"

class PktBlobEntry;

#pragma pack(push)
#pragma pack(1)

class ConnectId
{
public:
	ConnectId() = default;
    ConnectId( VxGUID& socketId );
    ConnectId( VxGUID& socketId, GroupieId& groupieId );
    ConnectId( VxGUID& socketId, VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );
    ConnectId( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );
    ConnectId( VxGUID& groupieOnlineId, HostedId& hostedId );
    ConnectId( const ConnectId& rhs );
    //do not use virtuals because this object is packed in packets
	ConnectId&				    operator =( const ConnectId& rhs );
    bool						operator == ( const ConnectId& rhs ) const;
    bool						operator != ( const ConnectId& rhs ) const;
    bool						operator < ( const ConnectId& rhs ) const;
    bool						operator <= ( const ConnectId& rhs ) const;
    bool						operator > ( const ConnectId& rhs ) const;
    bool						operator >= ( const ConnectId& rhs ) const;

    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    void						setSocketId( VxGUID& socketId )                     { m_SocketId = socketId; }
    VxGUID&                     getSocketId( void )                                 { return m_SocketId; }

    void						setGroupieId( GroupieId& groupieId )                { m_GroupieId = groupieId; }
    GroupieId&                  getGroupieId( void )                                { return m_GroupieId; }

    void						setGroupieOnlineId( VxGUID& onlineId )              { m_GroupieId.setGroupieOnlineId( onlineId ); }
    VxGUID&					    getGroupieOnlineId( void )                          { return m_GroupieId.getGroupieOnlineId(); }

    void				        setHostedId( HostedId& hostedId )                   { m_GroupieId.setHostedId( hostedId ); }
    HostedId&                   getHostedId( void )                                 { return m_GroupieId.getHostedId(); }
    void				        setHostedOnlineId( VxGUID& onlineId )               { m_GroupieId.setHostedOnlineId( onlineId ); }
    VxGUID&                     getHostedOnlineId( void )                           { return m_GroupieId.getHostedOnlineId(); }
    void			            setHostType( EHostType hostType )                   { m_GroupieId.setHostType( hostType ); }
    EHostType	                getHostType( void )                                 { return m_GroupieId.getHostType(); }

    // returns 0 if equal else -1 if less or 1 if greater
    int							compareTo( ConnectId& guid );
    // returns true if guids are same value
    bool						isEqualTo( const ConnectId& guid );
    // get a description of the plugin id
    std::string                 describeConnectId( void ) const;

    bool                        isValid( void )                                 { return m_SocketId.isVxGUIDValid() && m_GroupieId.isValid();  }
    void                        clear( void )                                   { m_SocketId.clearVxGUID(); m_GroupieId.clear(); }

protected:
	//=== vars ===//
    VxGUID					    m_SocketId;
    GroupieId					m_GroupieId;
};

#pragma pack(pop)
