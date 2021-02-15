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

#include <GoTvInterface/IDefs.h>

#include <CoreLib/VxGUID.h>

class PktBlobEntry;

class PluginId
{
public:
	PluginId() = default;
    PluginId( VxGUID& onlineId, EPluginType pluginType );
    PluginId( const PluginId& rhs );
	virtual ~PluginId() = default;
	PluginId&				    operator =( const PluginId& rhs );
    bool						operator == ( const PluginId& rhs ) const;
    bool						operator != ( const PluginId& rhs ) const;
    bool						operator < ( const PluginId& rhs ) const;
    bool						operator <= ( const PluginId& rhs ) const;
    bool						operator > ( const PluginId& rhs ) const;
    bool						operator >= ( const PluginId& rhs ) const;

    virtual bool                addToBlob( PktBlobEntry& blob );
    virtual bool                extractFromBlob( PktBlobEntry& blob );

    void						setOnlineId( VxGUID& onlineId )             { m_OnlineId = onlineId; }
    VxGUID&					    getOnlineId( void )                         { return m_OnlineId; }

    void						setPluginType( EPluginType gender )         { m_PluginType = gender; }
    EPluginType&				getPluginType( void )                       { return m_PluginType; }

    // returns 0 if equal else -1 if less or 1 if greater
    int							compareTo( PluginId& guid );
    // returns true if guids are same value
    bool						isEqualTo( const PluginId& guid );

protected:
	//=== vars ===//
    VxGUID					    m_OnlineId;
    EPluginType					m_PluginType{ ePluginTypeInvalid };
};


