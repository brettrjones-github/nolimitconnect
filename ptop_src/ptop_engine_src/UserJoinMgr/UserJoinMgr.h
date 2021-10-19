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

#include "UserJoinInfoDb.h"

#include <CoreLib/VxMutex.h>

class BaseSessionInfo;
class UserJoinInfo;
class UserJoinCallbackInterface;
class P2PEngine;
class VxSktBase;
class VxNetIdent;

class UserJoinMgr 
{
    const int USER_JOIN_DB_VERSION = 1;
public:
	UserJoinMgr( P2PEngine& engine, const char * dbName, const char * dbStateName );
	virtual ~UserJoinMgr() = default;

    void                        fromGuiUserLoggedOn( void );

    void                        addUserJoinMgrClient( UserJoinCallbackInterface * client, bool enable );

    void                        onHostJoinedByUser( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo );
    void                        onUserJoinedHost( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId );

    virtual void				announceUserJoinRequested( UserJoinInfo* userHostInfo );
    virtual void				announceUserJoinUpdated( UserJoinInfo * userJoinInfo );
    virtual void				announceUserJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType );

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

    UserJoinInfo*               findUserJoinInfo( VxGUID& hostOnlineId, EPluginType pluginType );

protected:
    void						clearUserJoinInfoList( void );

    void						lockClientList( void )						{ m_UserJoinClientMutex.lock(); }
    void						unlockClientList( void )					{ m_UserJoinClientMutex.unlock(); }

    bool                        saveToDatabase( UserJoinInfo* joinInfo, bool isLocked = false );
    void                        removeFromDatabase( VxGUID& hostOnlineId, EPluginType pluginType, bool resourcesLocked );

    P2PEngine&					m_Engine;
    UserJoinInfoDb              m_UserJoinInfoDb;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };
 
    std::vector<UserJoinInfo*>	m_UserJoinInfoList;
    VxMutex						m_UserJoinInfoMutex;
    bool                        m_UserJoinListInitialized{ false };

    std::vector<UserJoinCallbackInterface *> m_UserJoinClients;
    VxMutex						m_UserJoinClientMutex;
};

