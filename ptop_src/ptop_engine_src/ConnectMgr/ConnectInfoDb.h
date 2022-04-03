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

#include <GuiInterface/IDefs.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class P2PEngine;
class ConnectInfo;
class ConnectMgr;
class VxGUID;

class ConnectInfoDb : public DbBase
{
public:
   ConnectInfoDb( P2PEngine& engine,ConnectMgr& mgr, const char *dbName );
    virtual ~ConnectInfoDb() = default;

    void						lockConnectInfoDb( void ) { m_ConnectInfoDbMutex.lock(); }
    void						unlockConnectInfoDb( void ) { m_ConnectInfoDbMutex.unlock(); }

    void						addConnect(     enum EHostType  hostType,
                                                VxGUID&			hostOnlineId,  
                                                uint64_t		hostModTime,
                                                uint32_t        hostFlags,
                                                VxGUID&			thumbId,
                                                uint64_t		thumbModTime,
                                                VxGUID&			offerId,
                                                enum EOfferState offerState,
                                                uint64_t		offerModTime,
                                                std::string     hostUrl
                                                );

    void						addConnect(ConnectInfo * hostInfo );
    void						removeConnect( VxGUID& hostOnlineId );

    void						getAllConnects( std::vector<ConnectInfo*>& userHostList );
    void						purgeAllConnects( void ); 

protected:

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );
    void						insertConnectInTimeOrder(ConnectInfo * hostInfo, std::vector<ConnectInfo*>& assetList );

    P2PEngine&					m_Engine;
    ConnectMgr&				    m_ConnectMgr;
    VxMutex						m_ConnectInfoDbMutex;
};

