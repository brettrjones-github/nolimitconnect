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

#include "HostServerSearchMgr.h"
#include <ptop_src/ptop_engine_src/User/UserList.h>

#include <PktLib/PktsHostInvite.h>
#include <PktLib/PluginId.h>

class HostServerMgr : public HostServerSearchMgr
{
public:
    HostServerMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, PluginBase& pluginBase );
	virtual ~HostServerMgr() = default;

    virtual void				fromGuiListAction( EListAction listAction ) override;

    virtual void                removeSession( VxGUID& sessionId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                sendHostAnnounceToNetworkHost( VxGUID& sessionId, PktHostInviteAnnounceReq& hostAnnounce, EConnectReason connectReason );

    virtual EJoinState	        getJoinState( VxNetIdent* netIdent, EHostType hostType ) override;
    virtual EMembershipState	getMembershipState( VxNetIdent* netIdent, EHostType hostType ) override;

    virtual void				onJoinRequested( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType );
    virtual void				onUserJoined( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType );
    virtual void				onUserLeftHost( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType );
    virtual void				onUserUnJoined( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType );

    virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );

    virtual void                onGroupDirectUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );

protected:
    virtual bool                onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason ) override;
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;

    virtual void                onClientJoined( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual bool                addClient( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual bool                removeClient( VxGUID& onlineId );

    virtual void                addAnnounceSession( VxGUID& sessionId, PktHostInviteAnnounceReq* hostAnn );
    virtual void                removeAnnounceSession( VxGUID& sessionId );

    VxMutex                     m_ServerMutex;
    VxGUIDList                  m_ClientList;
    std::map<VxGUID, PktHostInviteAnnounceReq*> m_AnnList;
    VxMutex                     m_AnnListMutex;
};

