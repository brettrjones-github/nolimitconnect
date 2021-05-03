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

#include "PluginBaseHostClient.h"

#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>

class PluginConnectionTestClient : public PluginBaseHostClient
{
public:
    PluginConnectionTestClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginConnectionTestClient() override = default;
    PluginConnectionTestClient() = delete; // don't allow default constructor
    PluginConnectionTestClient( const PluginConnectionTestClient& ) = delete; // don't allow copy constructor

    virtual RCODE				handleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

    void						testIsMyPortOpen( void );

protected:
    RCODE						internalHandleHttpConnection( VxSktBase * sktBase, NetServiceHdr& netServiceHdr );

    virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )	{};
    virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )	{};
    virtual void				onConnectionLost( VxSktBase * sktBase )	{};

    //=== vars ===//
    NetServicesMgr&				m_NetServicesMgr;
};
