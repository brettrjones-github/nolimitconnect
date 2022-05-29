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

#include <config_appcorelibs.h>
#include "BaseSessionInfo.h"

#include <CoreLib/VxDebug.h>
#include <PktLib/VxCommon.h>

//============================================================================
BaseSessionInfo::BaseSessionInfo()
{ 
}

//============================================================================
BaseSessionInfo::BaseSessionInfo( EPluginType pluginType, VxGUID onlineId, VxGUID sessionId, VxGUID connectionId )
    : m_PluginType( pluginType )
    , m_OnlineId( onlineId )
    , m_SessionId( sessionId )
    , m_ConnectionId( connectionId )
{ 
}

//============================================================================
BaseSessionInfo::BaseSessionInfo( const BaseSessionInfo& rhs )
    : m_PluginType( rhs.m_PluginType )
    , m_OnlineId( rhs.m_OnlineId )
    , m_SessionId( rhs.m_SessionId )
    , m_ConnectionId( rhs.m_ConnectionId )
    , m_OnlineState( rhs.m_OnlineState )
{
}

//============================================================================
BaseSessionInfo& BaseSessionInfo::operator=( const BaseSessionInfo& rhs )
{
    if( this != &rhs )
    {
        m_PluginType = rhs.m_PluginType;
        m_OnlineId = rhs.m_OnlineId;
        m_SessionId = rhs.m_SessionId;
        m_ConnectionId = rhs.m_ConnectionId;
        m_OnlineState = rhs.m_OnlineState;
    }

    return *this;
}

//============================================================================
bool BaseSessionInfo::operator==( const BaseSessionInfo& rhs )
{
    return m_PluginType == rhs.m_PluginType &&
            m_OnlineId == rhs.m_OnlineId &&
            m_SessionId == rhs.m_SessionId &&
            m_ConnectionId == rhs.m_ConnectionId;
}
