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

#include "PluginBase.h"

#include <GoTvCore/GoTvP2P/Connections/IConnectRequest.h>

#include <CoreLib/VxGUIDList.h>

class VxNetIdent;
class ConnectionMgr;

class PluginBaseHostClient : public PluginBase, public IConnectRequestCallback
{
public:

    PluginBaseHostClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
	virtual ~PluginBaseHostClient() override = default;

    //=== hosting ===//
    virtual void				fromGuiJoinHost( EHostType hostType, const char * ptopUrl ) override;

protected:
    //=== callback overrides ==//
    virtual void                onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return false; };
    virtual void                onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactDisconnected( VxSktBase* sktBased, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    //=== vars ===//
    ConnectionMgr&              m_ConnectionMgr; 
    VxMutex                     m_ClientMutex;
    VxGUIDList                  m_JoinedHosts;

};