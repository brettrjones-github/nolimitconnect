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

#include "HostClientSearchMgr.h"

#include <PktLib/PluginIdList.h>
#include <PktLib/SearchParams.h>
#include <PktLib/GroupieId.h>

#include <set>

class BaseSessionInfo;
class PktHostSearchReply;

class HostClientMgr : public HostClientSearchMgr
{
public:
    HostClientMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, PluginBase& pluginBase );
	virtual ~HostClientMgr() = default;

    virtual void                removeSession( VxGUID& sessionId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                sendHostSearchToNetworkHost( VxGUID& sessionId, SearchParams& searchParams, EConnectReason connectReason );

    virtual void                onPktHostJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void                onPktHostLeaveReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void                onPktHostUnJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void                onPktHostSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;


    virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual void                onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual void                onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual void                onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );

    virtual void                onGroupRelayedUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent );

protected:
    virtual bool                onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason ) override;
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;

    virtual void                startHostDetailSession( PktHostSearchReply* hostReply, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual bool                stopHostSearch( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId );

    void                        addPluginRxSession( VxGUID& sessionId, PluginIdList& pluginIdList );
    void                        removePluginRxSession( VxGUID& sessionId );

    VxMutex                     m_ClientMutex;
    VxMutex                     m_ServerListMutex;
    std::set<GroupieId>         m_ServerList;
    std::map<VxGUID, PluginIdList> m_PluginRxList;
    VxMutex                     m_PluginRxListMutex;
};

