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

#include "HostJoinMgr.h"
#include "HostJoinInfo.h"
#include "HostJoinInfoDb.h"
#include "HostJoinCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/BaseInfo/BaseSessionInfo.h>

#include <PktLib/PktAnnounce.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

//============================================================================
HostJoinMgr::HostJoinMgr( P2PEngine& engine, const char * dbName, const char * dbJoinedLastName )
: m_Engine( engine )
, m_HostJoinInfoDb( engine, *this, dbName )
, m_HostJoinedLastDb( engine, *this, dbJoinedLastName )
{
}

//============================================================================
void HostJoinMgr::fromGuiUserLoggedOn( void )
{
    // dont call HostBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbJoinFileName = VxGetSettingsDirectory();
        std::string dbJoinedLastName = VxGetSettingsDirectory();
        dbJoinFileName += m_HostJoinInfoDb.getDatabaseName();
        dbJoinedLastName += m_HostJoinedLastDb.getDatabaseName();

        lockResources();
        m_HostJoinInfoDb.dbShutdown();
        m_HostJoinInfoDb.dbStartup( USER_HOST_JOIN_DB_VERSION, dbJoinFileName );

        m_HostJoinedLastDb.dbShutdown();
        m_HostJoinedLastDb.dbStartup( JOINED_LAST_DB_VERSION, dbJoinedLastName );      

        clearHostJoinInfoList();
        m_HostJoinInfoDb.getAllHostJoins( m_HostJoinInfoList );
        for( auto iter = m_HostJoinInfoList.begin(); iter != m_HostJoinInfoList.end(); ++iter )
        {
            HostJoinInfo* hostJoinInfo = iter->second;
            if( !hostJoinInfo->getNetIdent() )
            {
                VxNetIdent* netIdent = m_Engine.getBigListMgr().findBigListInfo( hostJoinInfo->getOnlineId() );
                if( netIdent )
                {
                    hostJoinInfo->setNetIdent( netIdent );
                }
                else if( hostJoinInfo->getOnlineId() == m_Engine.getMyOnlineId() && m_Engine.getMyNetIdent()->isValidNetIdent() )
                {
                    // is myself
                    hostJoinInfo->setNetIdent( m_Engine.getMyNetIdent() );
                }
            }
        }

        m_HostJoinListInitialized = true;
        unlockResources();
    }
}

//============================================================================
void HostJoinMgr::addHostJoinMgrClient( HostJoinCallbackInterface * client, bool enable )
{
    lockClientList();
    std::vector<HostJoinCallbackInterface*>::iterator iter;
    for( iter = m_HostJoinClients.begin(); iter != m_HostJoinClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_HostJoinClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_HostJoinClients.push_back( client );
    }

    unlockClientList();
}

//============================================================================
void HostJoinMgr::announceHostJoinRequested( HostJoinInfo * hostInfo )
{
    HostJoinInfo * userHostInfo = dynamic_cast<HostJoinInfo *>(hostInfo);
    if( userHostInfo )
    {
	    LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinRequested start" );
	
	    lockClientList();
	    std::vector<HostJoinCallbackInterface *>::iterator iter;
	    for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
	    {
		    HostJoinCallbackInterface * client = *iter;
		    client->callbackHostJoinRequested( userHostInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinRequested done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostJoinRequested dynamic_cast failed" );
    }
}

//============================================================================
void HostJoinMgr::announceHostJoinUpdated( HostJoinInfo * hostInfo )
{
    HostJoinInfo * userHostInfo = dynamic_cast<HostJoinInfo *>(hostInfo);
    if( userHostInfo )
    {
        lockClientList();
        std::vector<HostJoinCallbackInterface *>::iterator iter;
        for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
        {
            HostJoinCallbackInterface * client = *iter;
            client->callbackHostJoinUpdated( userHostInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostJoinUpdated dynamic_cast failed" );
    }
}

//============================================================================
void HostJoinMgr::announceHostJoinRemoved( GroupieId& groupieId )
{
    removeFromDatabase( groupieId, false );
	lockClientList();
	std::vector<HostJoinCallbackInterface *>::iterator iter;
	for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
	{
		HostJoinCallbackInterface * client = *iter;
		client->callbackHostJoinRemoved( groupieId );
	}

	unlockClientList();
}

//============================================================================
void HostJoinMgr::clearHostJoinInfoList( void )
{
    for( auto iter = m_HostJoinInfoList.begin(); iter != m_HostJoinInfoList.end(); ++iter )
    {
        delete iter->second;
    }

    m_HostJoinInfoList.clear();
}

//============================================================================
void HostJoinMgr::onHostJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    GroupieId groupieId( netIdent->getMyOnlineId(), m_Engine.getMyOnlineId(), sessionInfo.getHostType() );
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( !joinInfo )
    {
        joinInfo = new HostJoinInfo();
        joinInfo->setGroupieId( groupieId );
        wasAdded = true;
    }

    joinInfo->fillBaseInfo( netIdent, PluginTypeToHostType( sessionInfo.getPluginType() ) );
    joinInfo->setPluginType( sessionInfo.getPluginType() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );
    joinInfo->setNetIdent( netIdent );
    int64_t timeNowMs = GetTimeStampMs();
    joinInfo->setThumbId( netIdent->getThumbId( PluginTypeToHostType( sessionInfo.getPluginType() ) ) );
    joinInfo->setJoinState( eJoinStateJoinRequested );
    joinInfo->setUserUrl( netIdent->getMyOnlineUrl() );
    joinInfo->setFriendState( netIdent->getMyFriendshipToHim() );

    joinInfo->setConnectionId( sktBase->getConnectionId() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );

    joinInfo->setInfoModifiedTime( timeNowMs );
    joinInfo->setLastConnectTime( timeNowMs );
    joinInfo->setLastJoinTime( timeNowMs );
    if( wasAdded )
    {
        m_HostJoinInfoList[joinInfo->getGroupieId()] = joinInfo;
    }
    
    saveToDatabase( joinInfo, true );
    unlockResources();

    if( wasAdded )
    {
        announceHostJoinRequested( joinInfo );
    }
    else
    {
        announceHostJoinUpdated( joinInfo );
    }
}

//============================================================================
void HostJoinMgr::onHostJoinedByUser( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    GroupieId groupieId( netIdent->getMyOnlineId(), m_Engine.getMyOnlineId(), sessionInfo.getHostType() );
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( !joinInfo )
    {
        joinInfo = new HostJoinInfo();
        joinInfo->setGroupieId( groupieId );
        wasAdded = true;
    }

    joinInfo->fillBaseInfo( netIdent, PluginTypeToHostType( sessionInfo.getPluginType() ) );
    joinInfo->setPluginType( sessionInfo.getPluginType() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );
    joinInfo->setNetIdent( netIdent );
    int64_t timeNowMs = GetTimeStampMs();
    joinInfo->setThumbId( netIdent->getThumbId( PluginTypeToHostType( sessionInfo.getPluginType() ) ) );
    joinInfo->setJoinState( eJoinStateJoinAccepted );
    joinInfo->setUserUrl( netIdent->getMyOnlineUrl() );
    joinInfo->setFriendState( netIdent->getMyFriendshipToHim() );

    joinInfo->setConnectionId( sktBase->getConnectionId() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );

    joinInfo->setInfoModifiedTime( timeNowMs );
    joinInfo->setLastConnectTime( timeNowMs );
    joinInfo->setLastJoinTime( timeNowMs );

    unlockResources();

    saveToDatabase( joinInfo );

    m_Engine.getThumbMgr().queryThumbIfNeeded( sktBase, netIdent, sessionInfo.getPluginType() );

    if( wasAdded )
    {
        announceHostJoinRequested( joinInfo );
    }
    else
    {
        announceHostJoinUpdated( joinInfo );
    }
}

//============================================================================
HostJoinInfo* HostJoinMgr::findUserJoinInfo( GroupieId& groupieId )
{
    HostJoinInfo* joinFoundInfo = nullptr;
    auto iter = m_HostJoinInfoList.find( groupieId );

    if( iter != m_HostJoinInfoList.end() )
    {
        return iter->second;
    }

    return nullptr;
}

//============================================================================
bool HostJoinMgr::saveToDatabase( HostJoinInfo* joinInfo, bool resourcesLocked )
{
    if( joinInfo->getOnlineId() == m_Engine.getMyOnlineId() )
    {
        // do not add ourself to database. If we joined then we are the admin
        // and we may join another host at the same time
        return true;
    }

    if( !resourcesLocked )
    {
        lockResources();
    }

    bool result = m_HostJoinInfoDb.addHostJoin( joinInfo );
    if( !resourcesLocked )
    {
        unlockResources();
    }

    return result;
}

//============================================================================
void HostJoinMgr::removeFromDatabase( GroupieId& groupieId, bool resourcesLocked )
{
    if( !resourcesLocked )
    {
        lockResources();
    }

    m_HostJoinInfoDb.removeHostJoin( groupieId );
    if( !resourcesLocked )
    {
        unlockResources();
    }
}

//============================================================================
void HostJoinMgr::fromGuiGetJoinedStateList( EPluginType pluginType, EJoinState joinState, std::vector<HostJoinInfo*>& hostJoinList )
{
    // NOTE: assumes resources have been locked
    for( auto iter = m_HostJoinInfoList.begin(); iter != m_HostJoinInfoList.end(); ++iter )
    {
        if( iter->second->getPluginType() == pluginType && iter->second->getJoinState() == joinState )
        {
            hostJoinList.push_back( iter->second );
        }
    }
}

//============================================================================
int HostJoinMgr::fromGuiGetJoinedListCount( EPluginType pluginType )
{
    int joinedCnt = 0;
    lockResources();

    for( auto iter = m_HostJoinInfoList.begin(); iter != m_HostJoinInfoList.end(); ++iter)
    {
        if( iter->second->getPluginType() == pluginType )
        {
            joinedCnt++;
        }
    }

    unlockResources();
    return joinedCnt;
}

//============================================================================
EJoinState HostJoinMgr::fromGuiQueryJoinState( EHostType hostType, VxNetIdent& netIdent )
{
    EJoinState hostJoinState = eJoinStateNone;
    GroupieId groupieId( netIdent.getMyOnlineId(), m_Engine.getMyOnlineId(), hostType );

    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( joinInfo )
    {
        hostJoinState = joinInfo->getJoinState();
    }
    else if( netIdent.getMyOnlineId() == m_Engine.getMyOnlineId() )
    {
        // if we are host we can always join our own hosted servers
        hostJoinState = eJoinStateJoinAccepted;
    }

    unlockResources();
    return hostJoinState;
}


//============================================================================
EMembershipState HostJoinMgr::fromGuiQueryMembership( EHostType hostType, VxNetIdent& netIdent )
{
    EMembershipState membershipState{ eMembershipStateNone };
    GroupieId groupieId( netIdent.getMyOnlineId(), m_Engine.getMyOnlineId(), hostType );
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( joinInfo )
    {
        EJoinState hostJoinState = joinInfo->getJoinState();
        switch( hostJoinState )
        {
        case eJoinStateNone:
        case eJoinStateSending:
        case eJoinStateSendFail:
        case eJoinStateSendAcked:
        case eJoinStateJoinRequested:
            return eMembershipStateCanBeRequested;

        case eJoinStateJoinAccepted:
            return eMembershipStateJoined;

        case eJoinStateJoinDenied:
            return eMembershipStateJoinDenied;
        }
    }
    else if( netIdent.getMyOnlineId() == m_Engine.getMyOnlineId() )
    {
        // if we are host we can always join our own hosted servers
        membershipState = eMembershipStateCanBeRequested;
    }

    unlockResources();
    return membershipState;
}

//============================================================================
void HostJoinMgr::onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId )
{
    // TODO BRJ handle disconnect
}

//============================================================================
void HostJoinMgr::changeJoinState( GroupieId& groupieId, EJoinState joinState )
{
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( joinInfo && joinInfo->setJoinState( joinState ) )
    {
        if( groupieId.getGroupieOnlineId() != m_Engine.getMyOnlineId() )
        {
            saveToDatabase( joinInfo, true );
        }

        announceHostJoinUpdated( joinInfo );
    }

    unlockResources();
}

//============================================================================
void HostJoinMgr::fromGuiListAction( EListAction listAction )
{
    
}