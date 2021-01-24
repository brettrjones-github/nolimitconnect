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
#include "HostServerMgr.h"

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
    //=== callback overrides ==//
    virtual void                onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return false; };
    virtual void                onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactDisconnected( VxSktBase* sktBased, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    
    virtual void				onPktHostJoinReq                ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktHostSearchReq              ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktHostOfferReq               ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;
    virtual void				onPktHostOfferReply             ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent ) override;

    void                        buildHostChatRoomAnnounce( PluginSetting& pluginSetting );
    void                        sendHostChatRoomAnnounce( void );

    //=== vars ===//
    HostServerMgr               m_HostServerMgr;
    bool                        m_SendAnnounceEnabled{ false };
    bool                        m_HostAnnounceBuilt{ false };
    PktHostAnnounce             m_PktHostAnnounce;
    VxMutex                     m_AnnMutex;
};
