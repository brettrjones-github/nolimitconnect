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

#include "HostJoinInfoDb.h"
#include "HostJoinedLastDb.h"

#include <CoreLib/VxMutex.h>
#include <map>

class BaseSessionInfo;
class HostJoinInfo;
class HostJoinCallbackInterface;
class P2PEngine;
class VxSktBase;
class VxNetIdent;
class GroupieId;

class HostJoinMgr 
{
    const int USER_HOST_JOIN_DB_VERSION = 1;
    const int JOINED_LAST_DB_VERSION = 1;
public:
	HostJoinMgr( P2PEngine& engine, const char* dbName, const char* dbJoinedLastName );
	virtual ~HostJoinMgr() = default;

    void                        fromGuiUserLoggedOn( void );
    virtual int					fromGuiGetJoinedListCount( enum EPluginType pluginType );
    virtual EJoinState	        fromGuiQueryJoinState( enum EHostType hostType, VxNetIdent& netIdent );
    EMembershipState            fromGuiQueryMembership( enum EHostType hostType, VxNetIdent& netIdent );
    void                        fromGuiGetJoinedStateList(enum  EPluginType pluginType, enum EJoinState joinState, std::vector<HostJoinInfo*>& hostJoinList );
    void                        fromGuiListAction( EListAction listAction );

    void                        wantHostJoinMgrCallbacks( HostJoinCallbackInterface * client, bool enable );

    virtual void				announceHostJoinRequested( HostJoinInfo* userHostInfo );
    virtual void				announceHostJoinUpdated( HostJoinInfo* userHostInfo );
    virtual void				announceHostUnJoin( GroupieId& groupieId );
    virtual void				announceHostJoinRemoved( GroupieId& groupieId );

    VxMutex&					getHostJoinInfoListMutex( void )                    { return m_HostJoinInfoListMutex; }
    void						lockHostJoinInfoList( void )						{ m_HostJoinInfoListMutex.lock(); }
    void						unlockHostJoinInfoList( void )						{ m_HostJoinInfoListMutex.unlock(); }

    void                        onHostJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostUnJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostJoinedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostLeftByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostUnJoinedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId );

    HostJoinInfo*               findUserJoinInfo( GroupieId& groupieId );

    void                        changeJoinState( GroupieId& groupieId, enum EJoinState joinState );
    EJoinState                  getHostJoinState( GroupieId& groupieId );
    bool                        isUserJoinedToRelayHost( VxGUID& onlineId );

protected:
    void						clearHostJoinInfoList( void );

    void						lockClientList( void )						{ m_HostJoinClientMutex.lock(); }
    void						unlockClientList( void )					{ m_HostJoinClientMutex.unlock(); }

    bool                        saveToDatabase( HostJoinInfo* joinInfo, bool resourcesLocked = false );
    void                        removeFromDatabase( GroupieId& groupieId, bool resourcesLocked );

    P2PEngine&					m_Engine;
    HostJoinInfoDb              m_HostJoinInfoDb;
    HostJoinedLastDb            m_HostJoinedLastDb;
    bool						m_Initialized{ false };
 
    std::map<GroupieId, HostJoinInfo*>	m_HostJoinInfoList;
    VxMutex						m_HostJoinInfoListMutex;
    bool                        m_HostJoinListInitialized{ false };

    std::vector<HostJoinCallbackInterface *> m_HostJoinClients;
    VxMutex						m_HostJoinClientMutex;
};

