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

#include <GoTvCore/GoTvP2P/Connections/IConnectRequest.h>

#include <CoreLib/VxMutex.h>

#include <PktLib/PktHostAnnounce.h>

class PluginChatRoomHost : public PluginBaseHostService, public IConnectRequestCallback
{
public:
    PluginChatRoomHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
	virtual ~PluginChatRoomHost() override = default;

    virtual void				pluginStartup( void ) override;

    virtual bool                setPluginSetting( PluginSetting& pluginSetting ) override;
    virtual void				onThreadOncePer15Minutes( void ) override;

protected:
    virtual	void				onPluginSettingChange( PluginSetting& pluginSetting ) override;
    /// return true if have use for this connection
    virtual bool                onContactConnected( EConnectRequestType hostConnectType, VxSktBase* sktBase ) override;
    virtual void                onContactDisconnected( EConnectRequestType hostConnectType, VxSktBase* sktBase ) override;

    void                        buildHostChatRoomAnnounce( PluginSetting& pluginSetting );
    void                        sendHostChatRoomAnnounce( void );

    bool                        m_SendAnnounceEnabled{ false };
    bool                        m_HostAnnounceBuilt{ false };
    PktHostAnnounce             m_PktHostAnnounce;
    VxMutex                     m_AnnMutex;
};
