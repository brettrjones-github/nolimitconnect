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

#include <PktLib/PktsHostInvite.h>
#include <CoreLib/VxGUIDList.h>

class VxNetIdent;
class ConnectionMgr;

class PluginBaseHostClient : public PluginBase, public IConnectRequestCallback
{
public:

    PluginBaseHostClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginBaseHostClient() override = default;

    //=== hosting ===//
    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable ) override;

    virtual bool				fromGuiRequestPluginThumb( VxNetIdent* netIdent, VxGUID& thumbId ) override;
    virtual bool                ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId ) override;

    virtual bool                sendLeaveHost( GroupieId& groupieId );
    virtual bool                sendUnJoinHost( GroupieId& groupieId );
    virtual bool                queryUserListFromHost( GroupieId& groupieId );

    virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;
    virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;
    virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;
    virtual void                onGroupRelayedUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ) override;

protected:
    //=== callback overrides ==//
    virtual bool                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return true;  };
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus, 
                                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return false; };
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus, 
                                                      EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual bool                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };

    virtual void                buildHostAnnounce( PluginSetting& pluginSetting ) {};
    virtual void				sendHostAnnounce( void ) {};
    virtual void				sendLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl );
    virtual void				sendUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl );


    virtual void				onContactWentOnline         ( VxNetIdent * netIdent, VxSktBase * sktBase ) override {};
    virtual void				onContactWentOffline        ( VxNetIdent *	netIdent, VxSktBase * sktBase ) override {};
    virtual void				replaceConnection           ( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override {};
    virtual void				onConnectionLost            ( VxSktBase * sktBase ) override {};

    virtual void				onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    //=== vars ===//
    ConnectionMgr&              m_ConnectionMgr; 
    HostClientMgr               m_HostClientMgr;
    VxMutex                     m_ClientMutex;
    VxGUIDList                  m_JoinedHosts;

    bool                        m_SendAnnounceEnabled{ false };
    bool                        m_PktHostInviteIsValid{ false };
    PktHostInviteAnnounceReq    m_PktHostInviteAnnounceReq;
    VxMutex                     m_AnnMutex;
};
