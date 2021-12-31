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

#include <ptop_src/ptop_engine_src/Connections/IConnectRequest.h>

#include "HostedInfo.h"
#include "HostedListDb.h"

#include <CoreLib/VxMutex.h>

class P2PEngine;
class VxGUID;
class VxNetIdent;
class VxPtopUrl;
class HostedListCallbackInterface;

class HostedListMgr : public IConnectRequestCallback
{
    const int HOSTED_LIST_DB_VERSION = 1;
public:
    HostedListMgr() = delete;
    HostedListMgr( P2PEngine& engine );
    virtual ~HostedListMgr() = default;

    RCODE                       hostedListMgrStartup( std::string& dbFileName );
    RCODE                       hostedListMgrShutdown( void );

    void                        lockList( void )    { m_HostedMutex.lock(); }
    void                        unlockList( void )  { m_HostedMutex.unlock(); }

    void                        addHostedListMgrClient( HostedListCallbackInterface* client, bool enable );

    bool                        fromGuiQueryMyHosted( EHostType hostType );
    bool                        fromGuiQueryHosts( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& hostIdIfNullThenAll );

    void                        updateHosted( EHostType hostType, VxGUID& hostGuid, std::string& hosted, int64_t timestampMs = 0 );
    bool                        getHosteds( EHostType hostType, std::vector<HostedInfo>& retHosteds );

    void                        requestIdentity( std::string& url );
    void                        updateHosteds( VxNetIdent* netIdent );

protected:
    virtual void                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus,
                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    virtual void                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus,
                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBased, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    void						removeClosedPortIdent( VxGUID& onlineId );

    P2PEngine&                  m_Engine;
    VxMutex                     m_HostedMutex;
    HostedListDb                m_HostedListDb;

    std::vector<HostedInfo>     m_HostedList;
};

