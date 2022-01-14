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

class HostedId
{
public:
	HostedId() = default;
    HostedId( VxGUID& onlineId, EHostType hostType );
    HostedId( const HostedId& rhs );
    //do not use virtuals because this object is packed in packets
	HostedId&				    operator =( const HostedId& rhs );
    bool						operator == ( const HostedId& rhs ) const;
    bool						operator != ( const HostedId& rhs ) const;
    bool						operator < ( const HostedId& rhs ) const;
    bool						operator <= ( const HostedId& rhs ) const;
    bool						operator > ( const HostedId& rhs ) const;
    bool						operator >= ( const HostedId& rhs ) const;

    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

    void						setOnlineId( VxGUID& onlineId )             { m_OnlineId = onlineId; }
    VxGUID&					    getOnlineId( void )                         { return m_OnlineId; }

    void						setHostType( EHostType hostType )           { m_HostType = (uint8_t)hostType; }
    EHostType  				    getHostType( void ) const                   { return (EHostType)m_HostType; }

    // returns 0 if equal else -1 if less or 1 if greater
    int							compareTo( HostedId& guid );
    // returns true if guids are same value
    bool						isEqualTo( const HostedId& guid );
    // get a description of the plugin id
    std::string                 describeHostedId( void ) const;

    bool                        isValid( void )                             { return eHostTypeUnknown != m_HostType && m_OnlineId.isVxGUIDValid(); }

protected:
	//=== vars ===//
    VxGUID					    m_OnlineId;
    uint8_t					    m_HostType{ 0 };
};

#pragma pack(pop)
