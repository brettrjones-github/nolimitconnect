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

#include <GoTvCore/GoTvP2P/Connections/IConnectRequest.h>

#include <CoreLib/VxMutex.h>

#include <PktLib/PktHostAnnounce.h>

class PluginGroupClient : public PluginBaseHostClient
{
public:

    PluginGroupClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
	virtual ~PluginGroupClient() override = default;

    virtual void				pluginStartup( void ) override;

    virtual bool                setPluginSetting( PluginSetting& pluginSetting ) override;
    virtual void				onThreadOncePer15Minutes( void ) override;

protected:
    virtual	void				onPluginSettingChange( PluginSetting& pluginSetting ) override;
    //=== callback overrides ==//
    virtual void                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus, 
                                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return false; };
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, 
                                                      EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase ) override {};
    virtual void				onConnectionLost( VxSktBase * sktBase ) override {};
    virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override {};
};
