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

#include "HostJoinInfoDb.h"

#include <CoreLib/VxMutex.h>

class BaseSessionInfo;
class HostJoinInfo;
class HostJoinCallbackInterface;
class P2PEngine;
class VxSktBase;
class VxNetIdent;

class HostJoinMgr 
{
    const int USER_HOST_DB_VERSION = 1;
public:
	HostJoinMgr( P2PEngine& engine, const char * dbName, const char * dbStateName );
	virtual ~HostJoinMgr() = default;

    void                        fromGuiUserLoggedOn( void );
    virtual int					fromGuiGetJoinedListCount( EPluginType pluginType );
    virtual EJoinState	        fromGuiQueryJoinState( EHostType hostType, VxNetIdent& netIdent );
    EMembershipState            fromGuiQueryMembership( EHostType hostType, VxNetIdent& netIdent );
    void                        fromGuiGetJoinedStateList( EPluginType pluginType, EJoinState joinState, std::vector<HostJoinInfo*>& hostJoinList );

    void                        addHostJoinMgrClient( HostJoinCallbackInterface * client, bool enable );

    virtual void				announceHostJoinRequested( HostJoinInfo* userHostInfo );
    virtual void				announceHostJoinUpdated( HostJoinInfo* userHostInfo );
    virtual void				announceHostJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType );

    VxMutex&					getResourceMutex( void )					{ return m_ResourceMutex; }
    void						lockResources( void )						{ m_ResourceMutex.lock(); }
    void						unlockResources( void )						{ m_ResourceMutex.unlock(); }

    void                        onHostJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    void                        onHostJoinedByUser( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo );
    virtual void                onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId );

    HostJoinInfo*               findUserJoinInfo( VxGUID& hostOnlineId, EPluginType pluginType ); 

    void                        changeJoinState( VxGUID& onlineId, EPluginType pluginType, EJoinState joinState );

protected:
    void						clearHostJoinInfoList( void );

    void						lockClientList( void )						{ m_HostJoinClientMutex.lock(); }
    void						unlockClientList( void )					{ m_HostJoinClientMutex.unlock(); }

    bool                        saveToDatabase( HostJoinInfo* joinInfo, bool resourcesLocked = false );
    void                        removeFromDatabase( VxGUID& hostOnlineId, EPluginType pluginType, bool resourcesLocked );

    P2PEngine&					m_Engine;
    HostJoinInfoDb              m_HostJoinInfoDb;
    VxMutex						m_ResourceMutex;
    bool						m_Initialized{ false };
 
    std::vector<HostJoinInfo*>	m_HostJoinInfoList;
    VxMutex						m_HostJoinInfoMutex;
    bool                        m_HostJoinListInitialized{ false };

    std::vector<HostJoinCallbackInterface *> m_HostJoinClients;
    VxMutex						m_HostJoinClientMutex;
};

