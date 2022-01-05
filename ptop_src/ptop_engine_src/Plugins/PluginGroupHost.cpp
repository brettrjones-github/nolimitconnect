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

#include "PluginGroupHost.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif
 

//============================================================================
PluginGroupHost::PluginGroupHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
    : PluginBaseHostService( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeHostGroup );
    setHostType( eHostTypeGroup );
}

//============================================================================
void PluginGroupHost::pluginStartup( void )
{
    PluginBaseHostService::pluginStartup();
}

//============================================================================
EMembershipState PluginGroupHost::getMembershipState( VxNetIdent* netIdent )
{
    if( eFriendStateIgnore == getPluginPermission() )
    {
        return eMembershipStateJoinDenied;
    }

    return m_HostServerMgr.getMembershipState( netIdent, getHostType() );
}
