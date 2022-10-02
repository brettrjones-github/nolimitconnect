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

#include <ptop_src/ptop_engine_src/ConnectIdListMgr/ConnectIdListCallbackInterface.h>

#include <PktLib/GroupieId.h>

#include <CoreLib/VxMutex.h>

class BaseSessionInfo;
class P2PEngine;
class User;
class UserOnlineCallbackInterface;
class VxSktBase;
class VxNetIdent;

class UserOnlineMgr : public ConnectIdListCallbackInterface
{
public:
	UserOnlineMgr( P2PEngine& engine, const char* dbName, const char* dbStateName );
	virtual ~UserOnlineMgr() = default;

    void                        fromGuiUserLoggedOn( void );

    void                        addUserOnlineMgrClient( UserOnlineCallbackInterface * client, bool enable );

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

    void                        onUserOnline( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    bool                        onUserOnline( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent ); // return true if user was added
    void                        onHostJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostJoinedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostLeftByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onUserJoinedHost( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId );
    virtual void                onUserOffline( VxGUID& onlineId );

    User*                       findUser( VxGUID& onlineId );

protected:
    void				        callbackOnlineStatusChange( VxGUID& onlineId, bool isOnline ) override;
    void				        callbackConnectionStatusChange( ConnectId& connectId, bool isConnected ) override;
    void                        callbackRelayStatusChange( ConnectId& connectId, bool isRelayed ) override;

    void                        updateUserSession( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo, bool leftHost = false );
    void                        updateUserSession( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo, bool leftHost );

    virtual void				announceUserOnlineAdded( User* userJoinInfo, BaseSessionInfo& sessionInfo );
    virtual void				announceUserOnlineUpdated( User* userJoinInfo, BaseSessionInfo& sessionInfo );
    virtual void				announceUserOnlineRemoved( VxGUID& hostOnlineId, EHostType hostType );

    void				        announceUserOnlineState( User* userJoinInfo, bool isOnline );
    void                        announceUserSessionState( User* user, bool isInSession );

    void						lockClientList( void )						{ m_UserOnlineClientMutex.lock(); }
    void						unlockClientList( void )					{ m_UserOnlineClientMutex.unlock(); }

    P2PEngine&					m_Engine;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };
 
    std::vector<User*>	        m_UserOnlineList;
    VxMutex						m_UserOnlineMutex;
    bool                        m_UserOnlineListInitialized{ false };

    std::vector<UserOnlineCallbackInterface *> m_UserOnlineClients;
    VxMutex						m_UserOnlineClientMutex;
};

