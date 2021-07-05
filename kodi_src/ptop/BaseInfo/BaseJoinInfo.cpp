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

#include <config_gotvcore.h>
#include "BaseJoinInfo.h"

//============================================================================
BaseJoinInfo::BaseJoinInfo()
    : BaseInfo()
{ 
}

//============================================================================
BaseJoinInfo::BaseJoinInfo( const BaseJoinInfo& rhs )
    : BaseInfo( rhs )
    , m_PluginType( rhs.m_PluginType )
    , m_JoinState( rhs.m_JoinState )
    , m_LastConnectMs( rhs.m_LastConnectMs )
    , m_LastJoinMs( rhs.m_LastJoinMs )
{
}

//============================================================================
BaseJoinInfo& BaseJoinInfo::operator=( const BaseJoinInfo& rhs )
{
    if( this != &rhs )
    {
        BaseInfo::operator=( rhs );
        m_PluginType = rhs.m_PluginType;
        m_JoinState = rhs.m_JoinState;
        m_LastConnectMs = rhs.m_LastConnectMs;
        m_LastJoinMs = rhs.m_LastJoinMs;
    }

    return *this;
}
