//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "ConnectedInfo.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>

#include <NetLib/VxSktConnectSimple.h>
#include <NetLib/VxSktBase.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

#include <algorithm>

namespace
{
    const uint64_t HOST_ACTION_TIMEOUT_MS = 30000;
    const uint64_t HOST_CONNECT_ATTEMPT_TIMEOUT_MS = 60000;
}

//============================================================================
ConnectedInfo::ConnectedInfo( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
ConnectedInfo::ConnectedInfo( const ConnectedInfo& rhs )
    : m_Engine( rhs.m_Engine )
{
    *this = rhs;
}

//============================================================================
ConnectedInfo& ConnectedInfo::operator=( const ConnectedInfo& rhs )
{
    if( this != &rhs )
    {
        m_PluginType = rhs.m_PluginType;
        m_OnlineId = rhs.m_OnlineId;
    }

    return *this;
}

//============================================================================
bool ConnectedInfo::operator==( const ConnectedInfo& rhs )
{
    return  m_PluginType == rhs.m_PluginType &&
        m_OnlineId == rhs.m_OnlineId;
}
