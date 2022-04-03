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

#include "HostUrlInfo.h"
#include "HostUrlListDb.h"

#include <CoreLib/VxMutex.h>

class P2PEngine;
class VxGUID;
class VxNetIdent;

class HostUrlListMgr : public IConnectRequestCallback
{
    const int HOST_URL_LIST_DB_VERSION = 1;
public:
    HostUrlListMgr() = delete;
    HostUrlListMgr( P2PEngine& engine );
    ~HostUrlListMgr() = default;

    RCODE                       hostUrlListMgrStartup( std::string& dbFileName );
    RCODE                       hostUrlListMgrShutdown( void );

    void                        lockList( void )    { m_HostUrlsMutex.lock(); }
    void                        unlockList( void )  { m_HostUrlsMutex.unlock(); }

    void                        updateHostUrl( enum EHostType hostType, VxGUID& hostGuid, std::string& hostUrl, int64_t timestampMs = 0 );
    bool                        getHostUrls( enum EHostType hostType, std::vector<HostUrlInfo>& retHostUrls );

    void                        requestIdentity( std::string& url );
    void                        updateHostUrls( VxNetIdent* netIdent, int64_t timestampMs = 0 );

protected:
    virtual bool                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus,
                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    virtual bool                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus,
                                        enum EConnectReason connectReason = eConnectReasonUnknown, enum ECommErr commErr = eCommErrNone ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBased, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override {};

    void						removeClosedPortIdent( VxGUID& onlineId );

    P2PEngine&                  m_Engine;
    VxMutex                     m_HostUrlsMutex;
    HostUrlListDb               m_HostUrlListDb;

    std::vector<HostUrlInfo>    m_HostUrlsList;
};

