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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginBase.h"
#include <PktLib/HostedId.h>

class PluginBaseService : public PluginBase
{
public:
    PluginBaseService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginBaseService() override = default;

    EHostType                   getHostType( void ) override        { return m_HostType; }
    HostedId&                   getHostedId( void )                 { return m_HostedId; }

    void                        broadcastToClients( VxPktHdr* pktHdr, VxGUID& requestorOnlineId, VxSktBase* sktBaseRequester = nullptr );

protected:
    EConnectReason              getHostAnnounceConnectReason( void );
    EConnectReason              getHostJoinConnectReason( void );
    EConnectReason              getHostSearchConnectReason( void );

    //=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    HostedId                    m_HostedId;
};
