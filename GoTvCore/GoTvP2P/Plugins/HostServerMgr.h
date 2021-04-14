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
// http://www.nolimitconnect.com
//============================================================================

#include "HostServerSearchMgr.h"
#include <GoTvCore/GoTvP2P/User/UserList.h>

#include <PktLib/PktHostAnnounce.h>
#include <PktLib/PluginId.h>

class HostServerMgr : public HostServerSearchMgr
{
public:
    HostServerMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostServerMgr() = default;

    virtual void                removeSession( VxGUID& sessionId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                sendHostAnnounceToNetworkHost( VxGUID& sessionId, PktHostAnnounce& hostAnnounce, EConnectReason connectReason );

    virtual void				onUserJoined( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId );

protected:
    virtual void                onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason ) override;
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;

    virtual void                onClientJoined( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual bool                addClient( VxSktBase * sktBase, VxNetIdent * netIdent );
    virtual bool                removeClient( VxGUID& onlineId );

    virtual void                addAnnounceSession( VxGUID& sessionId, PktHostAnnounce* hostAnn );
    virtual void                removeAnnounceSession( VxGUID& sessionId );

    VxMutex                     m_ServerMutex;
    VxGUIDList                  m_ClientList;
    std::map<VxGUID, PktHostAnnounce*> m_AnnList;
    VxMutex                     m_AnnListMutex;
};

