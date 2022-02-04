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

#include "IdentListMgrBase.h"
#include <PktLib/GroupieId.h>

#include <map>
#include <set>

// maintains a list of online users
class P2PEngine;
class VxSktBase;

class OnlineSktList : public IdentListMgrBase
{
public:
    OnlineSktList() = delete;
    OnlineSktList( P2PEngine& engine );
    virtual ~OnlineSktList() = default;

    bool                        isOnline( VxGUID& onlineId );
    bool                        isOnline( GroupieId& groupieId );

    void                        userJoinedHost( VxGUID& sktConnectId, GroupieId& groupieId );
    void                        userLeftHost( VxGUID& sktConnectId, GroupieId& groupieId );

    bool                        getConnections( HostedId& hostId, std::set<std::pair<VxGUID,VxGUID>>& retConnectIdSet );

    VxSktBase*                  findHostConnection( GroupieId& groupieId, bool tryPeerFirst = false );
    VxSktBase*                  findPeerConnection( VxGUID& onlineId );
    virtual bool                findConnectionId( GroupieId& groupieId, VxGUID& retSktConnectId );
    VxSktBase*                  findSktBase( VxGUID& connectId );

    virtual void                addConnection( VxGUID& sktConnectId, GroupieId& groupieId );
    virtual void                removeConnection( VxGUID& sktConnectId, GroupieId& groupieId );
    void                        disconnectIfIsOnlyUser( GroupieId& groupieId );

    virtual void                onConnectionLost( VxGUID& sktConnectId );

    virtual void                onUserOnlineStatusChange( GroupieId& groupieId, bool isOnline ) {};
    virtual void                onUserOnlineStatusChange( VxGUID& onlineId, bool isOnline ) {};

protected:

    std::map< VxGUID, std::set<GroupieId>> m_OnlineSktList;
};

