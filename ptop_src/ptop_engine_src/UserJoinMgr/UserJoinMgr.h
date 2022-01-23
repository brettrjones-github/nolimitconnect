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
#include "UserJoinedLastDb.h"

#include <CoreLib/VxMutex.h>
#include <PktLib/GroupieId.h>
#include <map>

// client side manager of user join to host service states

class BaseSessionInfo;
class UserJoinInfo;
class UserJoinCallbackInterface;
class P2PEngine;
class VxSktBase;
class VxNetIdent;

class UserJoinMgr 
{
    const int USER_JOIN_DB_VERSION = 1;
    const int USER_JOINED_LAST_DB_VERSION = 1;
public:
	UserJoinMgr( P2PEngine& engine, const char * dbName, const char * dbJoinedLastName );
	virtual ~UserJoinMgr() = default;

    void                        fromGuiUserLoggedOn( void );
    bool                        getLastJoinedHostUrl( EHostType hostType, std::string& retHostUrl );

    void                        addUserJoinMgrClient( UserJoinCallbackInterface * client, bool enable );

    void                        onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );

    virtual void                onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId );

    virtual void				announceUserJoinRequested( UserJoinInfo* userHostInfo );
    virtual void				announceUserJoinUpdated( UserJoinInfo * userJoinInfo );
    virtual void				announceUserJoinRemoved( GroupieId& groupieId );

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

    UserJoinInfo*               findUserJoinInfo( GroupieId& groupieId );

protected:
    void						clearUserJoinInfoList( void );

    void						lockClientList( void )						{ m_UserJoinClientMutex.lock(); }
    void						unlockClientList( void )					{ m_UserJoinClientMutex.unlock(); }

    bool                        saveToDatabase( UserJoinInfo* joinInfo, bool isLocked = false );
    void                        removeFromDatabase( GroupieId& groupieId, bool isLocked );

    bool                        saveToJoinedLastDatabase( UserJoinInfo* joinInfo, bool isLocked = false );
    void                        removeFromJoinedLastDatabase( GroupieId& groupieId, bool isLocked );

    P2PEngine&					m_Engine;
    UserJoinInfoDb              m_UserJoinInfoDb;
    UserJoinedLastDb            m_UserJoinedLastDb;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };
 
    std::map<GroupieId, UserJoinInfo*>	m_UserJoinInfoList;
    VxMutex						m_UserJoinInfoMutex;
    bool                        m_UserJoinListInitialized{ false };

    std::vector<UserJoinCallbackInterface *> m_UserJoinClients;
    VxMutex						m_UserJoinClientMutex;
};

