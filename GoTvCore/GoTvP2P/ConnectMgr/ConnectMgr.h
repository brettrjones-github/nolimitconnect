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

#include "ConnectInfoDb.h"

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class ConnectInfo;
class ConnectCallbackInterface;
class P2PEngine;

class ConnectMgr 
{
    const int USER_HOST_DB_VERSION = 1;
public:
	ConnectMgr( P2PEngine& engine, const char * dbName, const char * stateDbName );
	virtual ~ConnectMgr() = default;

    void                        fromGuiUserLoggedOn( void );

    void                        addConnectMgrClient( ConnectCallbackInterface * client, bool enable );

    virtual void				announceConnectAdded( ConnectInfo * userHostInfo );
    virtual void				announceConnectUpdated( ConnectInfo * userHostInfo );
    virtual void				announceConnectRemoved( VxGUID& hostOnlineId );
    virtual void				announceConnectOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState );
    virtual void				announceConnectOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId );

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

protected:
    void						clearConnectInfoList( void );

    void						lockClientList( void )						{ m_ConnectClientMutex.lock(); }
    void						unlockClientList( void )					{ m_ConnectClientMutex.unlock(); }

    P2PEngine&					m_Engine;
    ConnectInfoDb              m_ConnectInfoDb;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };
 
    std::vector<ConnectInfo*>	m_ConnectInfoList;
    VxMutex						m_ConnectInfoMutex;
    bool                        m_ConnectListInitialized{ false };

    std::vector<ConnectCallbackInterface *> m_ConnectClients;
    VxMutex						m_ConnectClientMutex;
};

