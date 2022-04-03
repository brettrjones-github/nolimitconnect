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
class PluginBase;
class VxGUID;
class VxNetIdent;
class VxPktHdr;
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

    void                        lockList( void )    { m_HostedInfoMutex.lock(); }
    void                        unlockList( void )  { m_HostedInfoMutex.unlock(); }

    void                        wantHostedListCallback( HostedListCallbackInterface* client, bool enable );

    bool                        fromGuiQueryMyHostedInfo( enum EHostType hostType, std::vector<HostedInfo>& hostedInfoList );
    bool                        fromGuiQueryHostedInfoList(enum  EHostType hostType, std::vector<HostedInfo>& hostedInfoList, VxGUID& hostIdIfNullThenAll );
    bool                        fromGuiQueryHostListFromNetworkHost( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& hostIdIfNullThenAll );
    virtual bool				fromGuiQueryGroupiesFromHosted( VxPtopUrl& hostedUrl, EHostType hostType, VxGUID& onlineIdIfNullThenAll );

    // returns true if retHostedInfo was filled
    bool                        updateHostInfo( enum EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase, HostedInfo* retHostedInfo = nullptr );
    void                        updateHosted( enum EHostType hostType, VxGUID& hostGuid, std::string& hosted, int64_t timestampMs = 0 );

    void                        updateHostedList( VxNetIdent* netIdent, VxSktBase* sktBase );
    void                        hostSearchResult( enum EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, HostedInfo& hostedInfo );
    void                        hostSearchCompleted( enum EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr );

    void                        onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin );

    void                        onHostInviteAnnounceAdded( enum EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase );
    void                        onHostInviteAnnounceUpdated( enum EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase );

protected:
    virtual bool                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, enum ERunTestStatus testStatus,
                                        enum EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    virtual bool                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, enum EConnectStatus connectStatus,
                                        enum EConnectReason connectReason = eConnectReasonUnknown, enum ECommErr commErr = eCommErrNone ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBased, VxGUID& onlineId, enum EConnectReason connectReason = eConnectReasonUnknown ) override {};

    void						removeClosedPortIdent( VxGUID& onlineId );
    void						removeHostedInfo( EHostType hostType, VxGUID& onlineId );
    void						clearHostedInfoList( void );

    void                        updateAndRequestInfoIfNeeded( enum EHostType hostType, VxGUID& onlineId, std::string& nodeUrl, VxNetIdent* netIdent, VxSktBase* sktBase );

    bool                        updateIsFavorite( enum EHostType hostType, VxGUID& onlineId, bool isFavorite );
    bool                        updateLastConnected( enum EHostType hostType, VxGUID& onlineId, int64_t lastConnectedTime );
    bool                        updateLastJoined( enum EHostType hostType, VxGUID& onlineId, int64_t lastJoinedTime );
    bool                        updateHostTitleAndDescription( enum EHostType hostType, VxGUID& onlineId, std::string& title, std::string& description, int64_t lastDescUpdateTime, VxNetIdent* netIdent = nullptr );

    bool                        requestHostedInfo( enum EHostType hostType, VxGUID& onlineId, VxNetIdent* netIdent, VxSktBase* sktBase );

    void                        announceHostInfoUpdated( HostedInfo* hostedInfo );
    void                        announceHostInfoSearchResult( HostedInfo* hostedInfo, VxGUID& sessionId );
    void                        announceHostInfoSearchComplete( EHostType hostType, VxGUID& sessionId );
    void                        announceHostInfoRemoved( enum EHostType hostType, VxGUID& onlineId );

    void						addToListInJoinedTimestampOrder( std::vector<HostedInfo>& hostedInfoList, HostedInfo& hostedInfo );

    void						lockClientList( void )          { m_HostedInfoListClientMutex.lock(); }
    void						unlockClientList( void )        { m_HostedInfoListClientMutex.unlock(); }

    P2PEngine&                  m_Engine;
    VxMutex                     m_HostedInfoMutex;
    HostedListDb                m_HostedInfoListDb;

    std::vector<HostedInfo>     m_HostedInfoList;

    std::vector<HostedListCallbackInterface*> m_HostedInfoListClients;
    VxMutex						m_HostedInfoListClientMutex;

    EHostType                   m_SearchHostType{ eHostTypeUnknown };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_SearchSpecificOnlineId;
};

