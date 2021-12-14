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
#include "HostClientMgr.h"

#include <ptop_src/ptop_engine_src/Connections/IConnectRequest.h>

#include <PktLib/PktHostAnnounce.h>
#include <CoreLib/VxGUIDList.h>

class VxNetIdent;
class ConnectionMgr;

class PluginBaseHostClient : public PluginBase, public IConnectRequestCallback
{
public:

    PluginBaseHostClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginBaseHostClient() override = default;

    //=== hosting ===//
    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl ) override;
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl ) override;
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable ) override;

    virtual bool				fromGuiRequestPluginThumb( VxNetIdent* netIdent, VxGUID& thumbId ) override;
    virtual bool                ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId ) override;

protected:
    //=== callback overrides ==//
    virtual void                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus, 
                                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return false; };
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, 
                                                      EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    virtual void                buildHostAnnounce( PluginSetting& pluginSetting ) {};
    virtual void				sendHostAnnounce( void ) {};

    virtual void				onContactWentOnline         ( VxNetIdent * netIdent, VxSktBase * sktBase ) override {};
    virtual void				onContactWentOffline        ( VxNetIdent *	netIdent, VxSktBase * sktBase ) override {};
    virtual void				replaceConnection           ( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override {};
    virtual void				onConnectionLost            ( VxSktBase * sktBase ) override {};

    //=== vars ===//
    ConnectionMgr&              m_ConnectionMgr; 
    HostClientMgr               m_HostClientMgr;
    VxMutex                     m_ClientMutex;
    VxGUIDList                  m_JoinedHosts;

    bool                        m_SendAnnounceEnabled{ false };
    bool                        m_HostAnnounceBuilt{ false };
    PktHostAnnounce             m_PktHostAnnounce;
    VxMutex                     m_AnnMutex;
};
