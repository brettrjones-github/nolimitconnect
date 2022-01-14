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

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxGUID.h>

class PktBlobEntry;

#pragma pack(push)
#pragma pack(1)

class GroupieId
{
public:
	GroupieId() = default;
    GroupieId( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );
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

    void						setHostOnlineId( VxGUID& onlineId )                 { m_HostOnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                             { return m_HostOnlineId; }

    void						setHostType( EHostType hostType )                   { m_HostType = hostType; }
    EHostType                   getHostType( void )                                 { return m_HostType; }

    // returns 0 if equal else -1 if less or 1 if greater
    int							compareTo( GroupieId& guid );
    // returns true if guids are same value
    bool						isEqualTo( const GroupieId& guid );
    // get a description of the plugin id
    std::string                 describeGroupieId( void ) const;

    bool                        isValid( void )                                     { return eHostTypeUnknown != m_HostType && m_GroupieOnlineId.isVxGUIDValid() && m_HostOnlineId.isVxGUIDValid();  }

protected:
	//=== vars ===//
    VxGUID					    m_GroupieOnlineId;
    VxGUID					    m_HostOnlineId;
    EHostType                   m_HostType{ eHostTypeUnknown };
};

#pragma pack(pop)
