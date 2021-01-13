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

class PluginGroupClient : public PluginBaseHostClient, public IConnectRequestCallback
{
public:

    PluginGroupClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
	virtual ~PluginGroupClient() override = default;

    virtual void				pluginStartup( void ) override;

    virtual bool                setPluginSetting( PluginSetting& pluginSetting ) override;
    virtual void				onThreadOncePer15Minutes( void ) override;

protected:
    virtual	void				onPluginSettingChange( PluginSetting& pluginSetting ) override;
    /// return true if have use for this connection
    virtual bool                onContactConnected( EConnectRequestType hostConnectType, VxSktBase* sktBase ) override;
    virtual void                onContactDisconnected( EConnectRequestType hostConnectType, VxSktBase* sktBase ) override;

    virtual void				onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase ) override {};
    virtual void				onConnectionLost( VxSktBase * sktBase ) override {};
    virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override {};

    void                        buildHostGroupAnnounce( PluginSetting& pluginSetting );
    void                        sendHostGroupAnnounce( void );

    bool                        m_SendAnnounceEnabled{ false };
    bool                        m_HostAnnounceBuilt{ false };
    PktHostAnnounce             m_PktHostAnnounce;
    VxMutex                     m_AnnMutex;
};
