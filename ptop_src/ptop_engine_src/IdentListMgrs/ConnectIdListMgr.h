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
#include <PktLib/ConnectId.h>

#include <vector>
#include <set>
#include <map>

// maintains a list of online users
class ConnectIdListCallbackInterface;
class P2PEngine;
class PktAnnounce;
class VxNetIdent;
class VxSktBase;

class ConnectIdListMgr : public IdentListMgrBase
{
public:
    ConnectIdListMgr() = delete;
    ConnectIdListMgr( P2PEngine& engine );
    virtual ~ConnectIdListMgr() = default;

    bool                        isOnline( VxGUID& onlineId );
    bool                        isHosted( VxGUID& onlineId );
    bool                        isOnline( GroupieId& groupieId );

    void                        userJoinedHost( VxGUID& sktConnectId, GroupieId& groupieId );
    void                        userLeftHost( VxGUID& sktConnectId, GroupieId& groupieId );
    void                        onUserOffline( VxGUID& onlineId );

    bool                        getConnections( HostedId& hostId, std::set<ConnectId>& retConnectIdSet, std::set<ConnectId>& relayConnectIdSet );

    VxSktBase*                  findHostConnection( GroupieId& groupieId, bool tryPeerFirst = false );
    VxSktBase*                  findRelayMemberConnection( VxGUID& onlineId );
    VxSktBase*                  findPeerConnection( VxGUID& onlineId );

    VxSktBase*                  findAnyOnlineConnection( VxGUID& onlineId );
    VxSktBase*                  findBestOnlineConnection( VxGUID& onlineId );

    virtual bool                findConnectionId( GroupieId& groupieId, VxGUID& retSktConnectId );
    virtual bool                findRelayConnectionId( VxGUID& onlineId, VxGUID& retSktConnectId );
    VxSktBase*                  findSktBase( VxGUID& connectId );

    void                        addConnection( VxGUID& sktConnectId, GroupieId& groupieId, bool relayed );
    void                        removeConnection( VxGUID& sktConnectId, GroupieId& groupieId );
    void                        addConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason );
    void                        removeConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason );

    void                        disconnectIfIsOnlyUser( GroupieId& groupieId );

    virtual void                onConnectionLost( VxGUID& sktConnectId );
    virtual void                onGroupUserAnnounce( PktAnnounce* pktAnn, VxSktBase* sktBase, VxNetIdent* netIdent, bool relayed );
    void                        onGroupRelayedUserAnnounce( PktAnnounce* pktAnn, VxSktBase* sktBase, VxNetIdent* netIdent );

    void                        wantConnectIdListCallback( ConnectIdListCallbackInterface* client, bool enable );

protected:
    virtual void                onUserOnlineStatusChange( VxGUID& onlineId, bool isOnline );
    virtual void                onUserOnlineStatusChange( ConnectId& connectId, bool isOnline );
    virtual void                onUserRelayStatusChange( ConnectId& connectId, bool isRelayed );

    virtual void                onNewConnection( ConnectId& connectId );
    virtual void                onLostConnection( ConnectId& connectId );

    void                        announceOnlineStatus( VxGUID& onlineId, bool isOnline );
    void                        announceOnlineStatus( ConnectId& connectId, bool isConnected );
    void                        announceRelayStatus( ConnectId& connectId, bool isRelayed );

    void                        announceConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason, bool enableReason );
    void                        announceConnectionLost( VxGUID& sktConnectId );

    void						lockClientList( void ) { m_ClientCallbackMutex.lock(); }
    void						unlockClientList( void ) { m_ClientCallbackMutex.unlock(); }

    std::set<ConnectId>         m_ConnectIdList;
    std::set<ConnectId>         m_RelayedIdList;
    std::map<VxGUID, std::set<EConnectReason>>      m_ConnectReasonList;
    std::vector<ConnectIdListCallbackInterface*>    m_CallbackClients;
    VxMutex						m_ClientCallbackMutex;
};

