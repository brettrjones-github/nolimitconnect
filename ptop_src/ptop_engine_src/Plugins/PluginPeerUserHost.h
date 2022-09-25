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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginBaseHostService.h"

#include <ptop_src/ptop_engine_src/Connections/IConnectRequest.h>

#include <PktLib/PktsHostInvite.h>
#include <CoreLib/VxMutex.h>

class PluginPeerUserHost : public PluginBaseHostService, public IConnectRequestCallback
{
public:
    PluginPeerUserHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
    virtual ~PluginPeerUserHost() override = default;

    virtual void				pluginStartup( void ) override;

protected:
    /// return true if have use for this connection
    //=== callback overrides ==//
    virtual bool                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus, 
                                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return false; };
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, 
                                                      EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};
    virtual bool                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    void                        buildHostGroupAnnounce( PluginSetting& pluginSetting );
    void                        sendHostGroupAnnounce( void );

    //=== vars ===//
};

