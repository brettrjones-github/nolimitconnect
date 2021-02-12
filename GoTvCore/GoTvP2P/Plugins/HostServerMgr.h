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

#include <PktLib/PktHostAnnounce.h>

class HostServerMgr : public HostServerSearchMgr
{
public:
    HostServerMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostServerMgr() = default;

    virtual void                sendHostAnnounceToNetworkHost( VxGUID& sessionId, PktHostAnnounce& hostAnnounce, EConnectReason connectReason );

protected:
    virtual void                onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason ) override;
    virtual void                onContactDisconnected(VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;

    virtual void                onClientJoined( VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual bool                addClient( VxSktBase * sktBase, VxNetIdent * netIdent );
    virtual bool                removeClient( VxGUID& onlineId );

    VxMutex                     m_ServerMutex;
    VxGUIDList                  m_ClientList;
    PktHostAnnounce             m_PktHostAnnounce;
};

