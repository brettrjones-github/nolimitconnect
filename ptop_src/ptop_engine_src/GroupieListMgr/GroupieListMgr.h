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

#include "GroupieInfo.h"
#include "GroupieListDb.h"

#include <CoreLib/VxMutex.h>

class P2PEngine;
class VxGUID;
class VxNetIdent;
class VxPktHdr;
class VxPtopUrl;
class GroupieListCallbackInterface;
class PluginBase;
class PluginBaseHostService;
class BaseSessionInfo;

class GroupieListMgr : public IConnectRequestCallback
{
    const int HOSTED_LIST_DB_VERSION = 1;
public:
    GroupieListMgr() = delete;
    GroupieListMgr( P2PEngine& engine );
    virtual ~GroupieListMgr() = default;

    RCODE                       groupieListMgrStartup( std::string& dbFileName );
    RCODE                       groupieListMgrShutdown( void );

    void                        lockList( void )    { m_GroupieInfoMutex.lock(); }
    void                        unlockList( void )  { m_GroupieInfoMutex.unlock(); }

    void                        addGroupieListMgrClient( GroupieListCallbackInterface* client, bool enable );

    bool                        setGroupieUrlAndTitleAndDescription( GroupieId& groupieId, std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );
    bool                        getGroupieUrlAndTitleAndDescription( GroupieId& groupieId, std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime );

    bool                        fromGuiQueryMyGroupieInfo( EHostType hostType, std::vector<GroupieInfo>& groupieInfoList );
    bool                        fromGuiQueryGroupieInfoList( EHostType hostType, std::vector<GroupieInfo>& groupieInfoList, VxGUID& hostIdIfNullThenAll );

    // returns true if retGroupieInfo was filled
    bool                        updateGroupieInfo( EHostType hostType, GroupieInfo& groupieInfo, VxNetIdent* netIdent, VxSktBase* sktBase, GroupieInfo* retGroupieInfo = nullptr );
    void                        updateGroupie( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& hosted, int64_t timestampMs = 0 );

    void                        updateGroupieList( VxNetIdent* netIdent, VxSktBase* sktBase );
    void                        groupieSearchResult( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, GroupieInfo& groupieInfo );
    void                        groupieSearchCompleted( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr );

    virtual void				onPktGroupieInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin );
    virtual void				onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin );

    virtual void				onPktGroupieAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin );
    virtual void				onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin );
    virtual void				onPktGroupieSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin );
    virtual void				onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin );
    virtual void				onPktGroupieMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin );
    virtual void				onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin );

    void                        onGroupieAnnounceAdded( EHostType hostType, GroupieInfo& groupieInfo, VxNetIdent* netIdent, VxSktBase* sktBase );
    void                        onGroupieAnnounceUpdated( EHostType hostType, GroupieInfo& groupieInfo, VxNetIdent* netIdent, VxSktBase* sktBase );

    void                        onHostJoinedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostLeftByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );

protected:
    virtual bool                onUrlActionQueryIdSuccess( VxGUID& sessionId, std::string& url, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onUrlActionQueryIdFail( VxGUID& sessionId, std::string& url, ERunTestStatus testStatus,
                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};

    virtual bool                onContactHandshaking( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override { return true; };
    virtual void                onHandshakeTimeout( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};
    virtual void                onContactSessionDone( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    /// returns false if one time use and packet has been sent. Connect Manager will disconnect if nobody else needs the connection
    virtual bool                onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override;
    virtual void                onConnectRequestFail( VxGUID& sessionId, VxGUID& onlineId, EConnectStatus connectStatus,
                                        EConnectReason connectReason = eConnectReasonUnknown, ECommErr commErr = eCommErrNone ) override {};
    virtual void                onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBased, VxGUID& onlineId, EConnectReason connectReason = eConnectReasonUnknown ) override {};

    void						removeGroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );
    void						clearGroupieInfoList( void );

    void                        updateAndRequestInfoIfNeeded( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& nodeUrl, VxNetIdent* netIdent, VxSktBase* sktBase );

    bool                        updateIsFavorite( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, bool isFavorite );
    bool                        updateLastConnected( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastConnectedTime );
    bool                        updateLastJoined( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastJoinedTime );
    bool                        updateGroupieUrlAndTitleAndDescription( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& title, std::string& description, int64_t lastDescUpdateTime, VxNetIdent* netIdent = nullptr );

    bool                        requestGroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, VxNetIdent* netIdent, VxSktBase* sktBase );

    void                        announceGroupieInfoUpdated( GroupieInfo* groupieInfo );
    void                        announceGroupieInfoSearchResult( GroupieInfo* groupieInfo, VxGUID& sessionId );
    void                        announceGroupieInfoRemoved( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType );

    void						addToListInJoinedTimestampOrder( std::vector<GroupieInfo>& groupieInfoList, GroupieInfo& groupieInfo );

    void						lockClientList( void )          { m_GroupieInfoListClientMutex.lock(); }
    void						unlockClientList( void )        { m_GroupieInfoListClientMutex.unlock(); }

    void                        logCommError( ECommErr commErr, const char* desc, VxSktBase* sktBase, VxNetIdent* netIdent );
    void                        updateFromGroupieSearchBlob( EHostType hostType, VxGUID& hostOnlineId, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, PktBlobEntry& blobEntry, int hostInfoCount );
    bool                        requestMoreGroupiesFromHost( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& nextGroupieOnlineId, PluginBase* plugin );

    void                        connectToGroupieIfPossible( GroupieInfo& groupieInfo, EConnectReason connectReason );

    P2PEngine&                  m_Engine;
    VxMutex                     m_GroupieInfoMutex;
    GroupieListDb               m_GroupieInfoListDb;

    std::vector<GroupieInfo>    m_GroupieInfoList;

    std::vector<GroupieListCallbackInterface*> m_GroupieInfoListClients;
    VxMutex						m_GroupieInfoListClientMutex;

    EHostType                   m_SearchHostType{ eHostTypeUnknown };
    VxGUID                      m_SearchSessionId;
    VxGUID                      m_SearchSpecificOnlineId;
};

