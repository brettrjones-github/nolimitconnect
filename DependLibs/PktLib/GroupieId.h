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
// http://www.nolimitconnect.com
//============================================================================

#include "HostedId.h"

class PktBlobEntry;

#pragma pack(push)
#pragma pack(1)

class GroupieId
{
public:
	GroupieId() = default;
    GroupieId( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );
    GroupieId( VxGUID& groupieOnlineId, HostedId& hostedId );
    GroupieId( const GroupieId& rhs );
    //do not use virtuals because this object is packed in packets
	GroupieId&				    operator =( const GroupieId& rhs );
    bool						operator == ( const GroupieId& rhs ) const;
    bool						operator != ( const GroupieId& rhs ) const;
    bool						operator < ( const GroupieId& rhs ) const;
    bool						operator <= ( const GroupieId& rhs ) const;
    bool						operator > ( const GroupieId& rhs ) const;
    bool						operator >= ( const GroupieId& rhs ) const;

    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    void						setGroupieOnlineId( VxGUID& onlineId )             { m_GroupieOnlineId = onlineId; }
    VxGUID&					    getGroupieOnlineId( void )                         { return m_GroupieOnlineId; }

    void				        setHostedId( HostedId& hostedId )                   { m_HostedId = hostedId; }
    HostedId&                   getHostedId( void )                                 { return m_HostedId; }
    void				        setHostedOnlineId( VxGUID& onlineId )               { m_HostedId.setOnlineId( onlineId ); }
    VxGUID&                     getHostedOnlineId( void )                           { return m_HostedId.getOnlineId(); }
    void			            setHostType( EHostType hostType )                   { m_HostedId.setHostType( hostType ); }
    EHostType	                getHostType( void )                                 { return m_HostedId.getHostType(); }

    // returns 0 if equal else -1 if less or 1 if greater
    int							compareTo( GroupieId& guid );
    // returns true if guids are same value
    bool						isEqualTo( const GroupieId& guid );
    // get a description of the plugin id
    std::string                 describeGroupieId( void ) const;

    bool                        isValid( void )                                 { return m_GroupieOnlineId.isVxGUIDValid() && m_HostedId.isValid();  }
    void                        clear( void )                                   { m_GroupieOnlineId.clearVxGUID(); m_HostedId.clear(); }

protected:
	//=== vars ===//
    VxGUID					    m_GroupieOnlineId;
    HostedId					m_HostedId;
};

#pragma pack(pop)
