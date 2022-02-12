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
#include "ConnectReqInfo.h"
#include "HandshakeList.h"

#include <ptop_src/ptop_engine_src/Network/NetworkDefs.h>
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetStatusAccum.h>
#include <ptop_src/ptop_engine_src/NetworkTest/RunUrlAction.h>

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>
#include <PktLib/PktAnnounce.h>

class P2PEngine;
class BigListMgr;
class VxSktBase;
class VxPeerMgr;
class HandshakeInfo;

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

    EHostAnnounceStatus         lookupOrQueryAnnounceId( EHostType hostType, VxGUID& sessionId, std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason = eConnectReasonUnknown );
    EHostJoinStatus             lookupOrQueryJoinId( VxGUID& sessionId, std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason = eConnectReasonUnknown );
    EHostSearchStatus           lookupOrQuerySearchId( VxGUID& sessionId, std::string hostUrl, VxGUID& hostGuid, IConnectRequestCallback* callback, EConnectReason connectReason = eConnectReasonUnknown );

    EConnectStatus              requestConnection( VxGUID& sessionId, std::string url, VxGUID onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, 
                                                   EConnectReason connectReason = eConnectReasonUnknown );
    void                        doneWithConnection( VxGUID sessionId, VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason = eConnectReasonUnknown );

    void                        onSktConnectedWithPktAnn( VxSktBase* sktBase, BigListInfo* bigListInfo );
    void                        onSktDisconnected( VxSktBase* sktBase );

    void						doNetConnectionsThread( void );
    void						doStayConnectedThread( void );

    void                        lockConnectionList( void )      { m_ConnectionMutex.lock(); }
    void                        unlockConnectionList( void )    { m_ConnectionMutex.unlock(); }

protected:
    virtual void				callbackInternetStatusChanged( EInternetStatus internetStatus ) override;
    virtual void				callbackNetAvailStatusChanged( ENetAvailStatus netAvalilStatus ) override;

    virtual void                callbackActionStatus( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError, std::string statusMsg ) override {};
    virtual void                callbackActionFailed( UrlActionInfo& actionInfo, ERunTestStatus eStatus, ENetCmdError netCmdError ) override;

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
    EConnectStatus              attemptConnection( VxGUID& sessionId, std::string url, VxGUID& onlineId, IConnectRequestCallback* callback, VxSktBase*& retSktBase, EConnectReason connectReason );

    //=== connection low level ===/
    EConnectStatus              directConnectTo(    std::string                 url,
                                                    VxGUID&                     onlineId,
                                                    IConnectRequestCallback*    callback,
                                                    VxSktBase*&                 retSktBase,
                                                    VxGUID                      sessionId,
                                                    EConnectReason              connectReason,
                                                    int					        iConnectTimeoutMs = DIRECT_CONNECT_TIMEOUT );  // how long to attempt connect

    EConnectStatus				directConnectTo(	VxConnectInfo&			    connectInfo,		 
                                                    VxSktBase *&			    ppoRetSkt,		
                                                    VxGUID                      sessionId,
                                                    int						    iConnectTimeout = DIRECT_CONNECT_TIMEOUT,	 
                                                    bool					    useLanIp = false,
                                                    bool					    useUdpIp = false,
                                                    IConnectRequestCallback*    callback = nullptr,                                                
                                                    EConnectReason              connectReason = eConnectReasonUnknown );	 

    EConnectStatus              directConnectTo(    std::string                 ipAddr,
                                                    uint16_t                    port,
                                                    VxGUID                      onlineId,
                                                    VxSktBase*&                 retSktBase,
                                                    IConnectRequestCallback*    callback,
                                                    VxGUID                      sessionId,
                                                    EConnectReason              connectReason,
                                                    int					        iConnectTimeoutMs );

    void                        addConnectRequestToQue( VxConnectInfo& connectInfo, EConnectReason connectReason, bool addToHeadOfQue, bool replaceExisting );
    void                        addConnectRequestToQue( ConnectReqInfo& connectRequest, bool addToHeadOfQue, bool replaceExisting );
    bool                        connectToContact(   VxConnectInfo&		connectInfo,
                                                    VxSktBase *&		ppoRetSkt,
                                                    VxGUID&             sessionId,
                                                    bool&				retIsNewConnection );
    bool                        connectUsingTcp( VxConnectInfo& connectInfo, VxSktBase *& ppoRetSkt, VxGUID& sessionId );
    bool                        tryIPv6Connect( VxConnectInfo& connectInfo, VxSktBase *& ppoRetSkt );

    bool                        sendMyPktAnnounce(  VxGUID&				destinationId,
                                                    VxSktBase *			sktBase,
                                                    bool				requestAnnReply,
                                                    bool				requestReverseConnection,
                                                    bool				requestSTUN );
    bool                        txPacket( VxGUID&				destinationId,
                                          VxSktBase *			sktBase,
                                          VxPktHdr *			poPkt );

    void                        handleConnectSuccess( BigListInfo * bigListInfo, VxSktBase * skt, bool isNewConnection, EConnectReason connectReason );
    void                        closeConnection( ESktCloseReason closeReason, VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo );

    bool                        doConnectRequest( ConnectReqInfo& connectRequest, bool ignoreToSoonToConnectAgain );

    void                        setQueryIdFailedCount( EHostType hostType, int failedCount );
    int                         getQueryIdFailedCount( EHostType hostType );

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
    std::map<EHostType, std::pair<ERunTestStatus, int>> m_DefaultHostQueryIdFailed;

    /// keep a cache of urls to online id to avoid time consuming query host id
    std::map<std::string, VxGUID>       m_UrlCache;
    //std::map<VxGUID, std::pair<IConnectRequestCallback*, EConnectReason>>   m_ConnectRequests;

    HandshakeList               m_HandshakeList;
    VxMutex                     m_HandshakeMutex;

    /// low level connect vars
    VxThread					m_NetConnectThread;
    VxThread					m_StayConnectedThread;
    PktAnnounce&				m_PktAnn;
    VxPeerMgr&					m_PeerMgr;

    VxMutex						m_NetConnectorMutex;
    VxSemaphore					m_WaitForConnectWorkSemaphore;
    std::vector<ConnectReqInfo>	m_IdentsToConnectList;
};

