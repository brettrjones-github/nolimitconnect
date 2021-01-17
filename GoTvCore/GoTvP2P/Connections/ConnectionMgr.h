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
#pragma once

#include "ConnectedListAll.h"
#include "IConnectRequest.h"

#include <GoTvCore/GoTvP2P/NetworkMonitor/NetStatusAccum.h>
#include <GoTvCore/GoTvP2P/NetworkTest/RunUrlAction.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxGUIDList.h>
#include <PktLib/PktAnnounce.h>

class P2PEngine;
class BigListMgr;
class VxSktBase;

class ConnectionMgr : public NetAvailStatusCallbackInterface, public UrlActionResultInterface
{
public:
    ConnectionMgr() = delete;
    ConnectionMgr( P2PEngine& engine );
    virtual ~ConnectionMgr() = default;

    /// set default url from network settings
    void                        applyDefaultHostUrl( EHostType hostType, std::string& hostUrl );
    /// get default url for given host type
    std::string                 getDefaultHostUrl( EHostType hostType );

    void                        setDefaultHostOnlineId( EHostType hostType, VxGUID& hostOnlineId );
    bool                        getDefaultHostOnlineId( EHostType hostType, VxGUID& retHostOnlineId );

    EHostJoinStatus             lookupOrQueryId( std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason = eConnectReasonUnknown );

    EConnectStatus              requestConnection( std::string url, VxGUID onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, 
                                                   EConnectReason connectReason = eConnectReasonUnknown );
    void                        doneWithConnection( VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason = eConnectReasonUnknown );

    bool                        onSktConnectedWithPktAnn( VxSktBase* sktBase, BigListInfo* bigListInfo );
    void                        onSktDisconnected( VxSktBase* sktBase );

protected:
    virtual void				callbackInternetStatusChanged( EInternetStatus internetStatus ) override;
    virtual void				callbackNetAvailStatusChanged( ENetAvailStatus netAvalilStatus ) override;

    virtual void                callbackActionStatus( UrlActionInfo& actionInfo, ERunTestStatus eStatus, std::string statusMsg ) override {};
    virtual void                callbackActionFailed( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError = eNetCmdErrorUnknown ) override;

    virtual void                callbackPingSuccess( UrlActionInfo& actionInfo, std::string myIp ) override {};
    virtual void                callbackConnectionTestSuccess( UrlActionInfo& actionInfo, bool canDirectConnect, std::string myIp ) override {};
    virtual void                callbackQueryIdSuccess( UrlActionInfo& actionInfo, VxGUID onlineId ) override;

    ConnectedInfo*              getOrAddConnectedInfo( BigListInfo* bigListInfo ) { return m_AllList.getOrAddConnectedInfo( bigListInfo ); }
    VxGUID&				        getMyOnlineId( void )   { return m_MyOnlineId; }
    PktAnnounce&				getMyPktAnn( void )     { return m_MyPktAnn; }

    void                        onInternetAvailable( void );
    void                        onNoLimitNetworkAvailable( void );
    void                        resetDefaultHostUrl( EHostType hostType );

    /// keep a cache of urls to online id to avoid time consuming query host id
    void                        updateUrlCache( std::string& hostUrl, VxGUID& onlineId );
    bool                        urlCacheOnlineIdLookup( std::string& hostUrl, VxGUID& onlineId );
    EConnectStatus              attemptConnection( std::string url, VxGUID onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason );


    //=== vars ===//
    P2PEngine&					m_Engine;
    BigListMgr&					m_BigListMgr;
    VxMutex						m_ConnectionMutex;
    ConnectedListAll            m_AllList;
    VxGUID                      m_MyOnlineId;
    PktAnnounce                 m_MyPktAnn;
    EInternetStatus             m_InternetStatus{ eInternetNoInternet };
    ENetAvailStatus             m_NetAvailStatus{ eNetAvailNoInternet };
    std::map<EHostType, VxGUID>         m_DefaultHostIdList;
    std::map<EHostType, std::string>    m_DefaultHostUrlList;
    std::map<EHostType, std::string>    m_DefaultHostRequiresOnlineId;
    std::map<EHostType, ERunTestStatus> m_DefaultHostQueryIdFailed;

    /// keep a cache of urls to online id to avoid time consuming query host id
    std::map<std::string, VxGUID>       m_UrlCache;
    std::map<VxGUID, std::pair<IConnectRequestCallback*, EConnectReason>>   m_ConnectRequests;
};

