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

#include "UserHostInfoDb.h"

#include <CoreLib/VxThread.h>
#include <CoreLib/VxSemaphore.h>
#include <CoreLib/VxMutex.h>

class UserHostInfo;
class UserHostCallbackInterface;
class P2PEngine;

class UserHostMgr 
{
    const int USER_HOST_DB_VERSION = 1;
public:
	UserHostMgr( P2PEngine& engine, const char * dbName, const char * dbStateName );
	virtual ~UserHostMgr() = default;

    void                        fromGuiUserLoggedOn( void );

    void                        addUserHostMgrClient( UserHostCallbackInterface * client, bool enable );

    virtual void				announceUserHostAdded( UserHostInfo * userHostInfo );
    virtual void				announceUserHostUpdated( UserHostInfo * userHostInfo );
    virtual void				announceUserHostRemoved( VxGUID& hostOnlineId );
    virtual void				announceUserHostOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState );
    virtual void				announceUserHostOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId );

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

protected:
    void						clearUserHostInfoList( void );

    void						lockClientList( void )						{ m_UserHostClientMutex.lock(); }
    void						unlockClientList( void )					{ m_UserHostClientMutex.unlock(); }

    P2PEngine&					m_Engine;
    UserHostInfoDb              m_UserHostInfoDb;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };
 
    std::vector<UserHostInfo*>	m_UserHostInfoList;
    VxMutex						m_UserHostInfoMutex;
    bool                        m_UserHostListInitialized{ false };

    std::vector<UserHostCallbackInterface *> m_UserHostClients;
    VxMutex						m_UserHostClientMutex;
};

