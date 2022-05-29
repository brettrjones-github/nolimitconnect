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
// http://www.nolimitconnect.org
//============================================================================

#include "RcMulticast.h"
#include <ptop_src/ptop_engine_src/NetworkMonitor/NetStatusAccum.h>

#include <CoreLib/VxGUID.h>

#include <vector>

class BigListInfo;

class NearbyMgr : public IMulticastListenCallback, public NetAvailStatusCallbackInterface, public RcMulticast
{
public:
    const int                   MIN_TIME_MULTICAST_CONNECT_ATTEMPT = 60000;

    NearbyMgr() = delete;
    NearbyMgr( P2PEngine& engine, NetworkMgr& networkMgr );
    virtual ~NearbyMgr() = default;

    void                        lockList( void ) { m_ListMutex.lock(); }
    void                        unlockList( void ) { m_ListMutex.unlock(); }

    void						networkMgrStartup( void );
    void						networkMgrShutdown( void );

    virtual bool                fromGuiNearbyBroadcastEnable( bool enable );
    virtual void				fromGuiNetworkAvailable( const char* lclIp, bool isCellularNetwork = false );
    virtual void				fromGuiNetworkLost( void );

    virtual void				callbackInternetStatusChanged( EInternetStatus internetStatus ) override;
    virtual void				callbackNetAvailStatusChanged( ENetAvailStatus netAvalilStatus ) override;

    bool                        isNearby( VxGUID& onlineId );
    virtual void                updateIdent( VxGUID& onlineId, int64_t timestamp );
    virtual void                removeIdent( VxGUID& onlineId );

    std::vector<std::pair<VxGUID, int64_t>>& getIdentList()         { return m_NearbyIdentList; };

    virtual	void				handleMulticastSktCallback( VxSktBase* sktBase );
    void						handleTcpLanConnectSuccess( BigListInfo* bigListInfo, VxSktBase* skt, bool isNewConnection, EConnectReason connectReason );

    virtual void                onNearbyUserUpdated( VxNetIdent* netIdent, int64_t timestamp = 0);
    void                        onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId );

protected:
    void						setBroadcastPort( uint16_t u16Port );
    bool						setBroadcastEnable( bool enable );

    virtual void				multicastPktAnnounceAvail( VxSktBase* skt, PktAnnounce* pktAnnounce ) override;

    VxMutex                     m_ListMutex;
    std::vector<std::pair<VxGUID, int64_t>> m_NearbyIdentList;

    VxMutex                     m_MulticastIdentMutex;
    std::map<VxGUID, VxNetIdent> m_MulticastIdentList;
};

