//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "PluginBaseHostClient.h"
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

//============================================================================
PluginBaseHostClient::PluginBaseHostClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
    : PluginBase( engine, pluginMgr, myIdent )
    , m_ConnectionMgr(engine.getConnectionMgr())
{
}

//============================================================================
void PluginBaseHostClient::fromGuiJoinHost( EHostType hostType, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        VxGUID hostGuid;
        //EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( hostType, url.c_str(), hostGuid, this);
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
    }
}
