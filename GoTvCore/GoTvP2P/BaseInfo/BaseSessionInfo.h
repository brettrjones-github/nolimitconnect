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

class VxNetIdent;

class BaseSessionInfo
{
public:
    BaseSessionInfo();
    BaseSessionInfo( EPluginType pluginType, VxGUID& onlineId, VxGUID& sessionId, VxGUID& connectionId );
	BaseSessionInfo( const BaseSessionInfo& rhs );
    virtual ~BaseSessionInfo() = default;

	BaseSessionInfo&			operator=( const BaseSessionInfo& rhs ); 
    bool			            operator==( const BaseSessionInfo& rhs ); 

    virtual void				setPluginType( EPluginType hostType )           { m_PluginType = hostType; }
    virtual EPluginType			getPluginType( void )                           { return m_PluginType; }

    virtual void				setOnlineId( VxGUID onlineId )                  { m_OnlineId = onlineId; }
    virtual void				setOnlineId( const char * onlineId )            { m_OnlineId.fromVxGUIDHexString( onlineId ); }
    virtual VxGUID&				getOnlineId( void )                             { return m_OnlineId; }

    virtual void				setSessionId( VxGUID& sessionId )               { m_SessionId = sessionId; }
    virtual void				setSessionId( const char * sessionId )          { m_SessionId.fromVxGUIDHexString( sessionId ); }
    virtual VxGUID&				getSessionId( void )                            { return m_SessionId; }

    virtual void				setConnectionId( VxGUID& connectionId )         { m_ConnectionId = connectionId; }
    virtual void				setConnectionId( const char * connectionId )    { m_ConnectionId.fromVxGUIDHexString( connectionId ); }
    virtual VxGUID&				getConnectionId( void )                         { return m_ConnectionId; }

    virtual void				setOnlineState( EOnlineState onlineState )      { m_OnlineState = onlineState; }
    virtual EOnlineState	    getOnlineState( void )                          { return m_OnlineState; }

public:
	//=== vars ===//
    EPluginType                 m_PluginType{ ePluginTypeInvalid };
    VxGUID						m_OnlineId; 
    VxGUID						m_SessionId; 
    VxGUID						m_ConnectionId; 
    EOnlineState                m_OnlineState{ eOnlineStateUnknown };
};
