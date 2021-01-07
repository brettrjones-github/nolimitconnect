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

#include <GoTvCore/GoTvP2P/NetworkMonitor/NetStatusAccum.h>

#include <CoreLib/VxMutex.h>
#include <CoreLib/VxGUID.h>
#include <PktLib/PktAnnounce.h>

class P2PEngine;
class BigListMgr;
class VxSktBase;

class ConnectionMgr : public NetAvailStatusCallbackInterface
{
public:
    ConnectionMgr() = delete;
    ConnectionMgr( P2PEngine& engine );
    virtual ~ConnectionMgr() = default;

    bool                        onSktConnectedWithPktAnn( VxSktBase* sktBase, BigListInfo* bigListInfo );
    void                        onSktDisconnected( VxSktBase* sktBase );

    void                        setHostOnlineId( EHostType hostType, VxGUID& hostOnlineId );
    bool                        getHostOnlineId( EHostType hostType, VxGUID& retHostOnlineId );
    
    void                        applyHostUrl( EHostType hostType, std::string& hostUrl );

protected:
    virtual void				callbackInternetStatusChanged( EInternetStatus internetStatus ) override;
    virtual void				callbackNetAvailStatusChanged( ENetAvailStatus netAvalilStatus ) override;

    ConnectedInfo*              getOrAddConnectedInfo( BigListInfo* bigListInfo ) { return m_AllList.getOrAddConnectedInfo( bigListInfo ); }
    VxGUID&				        getMyOnlineId( void )   { return m_MyOnlineId; }
    PktAnnounce&				getMyPktAnn( void )     { return m_MyPktAnn; }

    void                        onInternetAvailable( void );
    void                        onNoLimitNetworkAvailable( void );

    //=== vars ===//
    P2PEngine&					m_Engine;
    BigListMgr&					m_BigListMgr;
    VxMutex						m_ConnectionMutex;
    ConnectedListAll            m_AllList;
    VxGUID                      m_MyOnlineId;
    PktAnnounce                 m_MyPktAnn;
    EInternetStatus             m_InternetStatus{ eInternetNoInternet };
    ENetAvailStatus             m_NetAvailStatus{ eNetAvailNoInternet };
    std::map<EHostType, VxGUID> m_HostIdList;
};

