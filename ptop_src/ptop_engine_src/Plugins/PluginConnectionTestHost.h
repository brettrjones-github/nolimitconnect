#pragma once
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

#include "PluginBaseHostService.h"

#include <ptop_src/ptop_engine_src/Connections/IConnectRequest.h>

class NetServicesMgr;

class PluginConnectionTestHost : public PluginBaseHostService
{
public:

    PluginConnectionTestHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginConnectionTestHost() override = default;

    void                        testIsMyPortOpen( void );
    RCODE                       handleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr ) override;
    RCODE                       internalHandleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

protected:
    NetServicesMgr&				m_NetServicesMgr;
};
