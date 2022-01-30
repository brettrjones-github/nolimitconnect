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

#include "UserJoinMgr.h"
#include "UserJoinInfo.h"
#include "UserJoinInfoDb.h"
#include "UserJoinCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/BaseInfo/BaseSessionInfo.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>
#include <CoreLib/VxPtopUrl.h>

//============================================================================
UserJoinMgr::UserJoinMgr( P2PEngine& engine, const char * dbName, const char * dbJoinedLastName )
: m_Engine( engine )
, m_UserJoinInfoDb( engine, *this, dbName )
, m_UserJoinedLastDb( engine, *this, dbJoinedLastName )
{
}

//============================================================================
void UserJoinMgr::fromGuiUserLoggedOn( void )
{
    LogModule( eLogStartup, LOG_VERBOSE, "UserJoinMgr::fromGuiUserLoggedOn start" );
    // dont call HostBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_UserJoinInfoDb.getDatabaseName(); 

        std::string dbLastJoinedFileName = VxGetSettingsDirectory();
        dbLastJoinedFileName += m_UserJoinedLastDb.getDatabaseName();

        lockResources();
        m_UserJoinInfoDb.dbShutdown();
        m_UserJoinInfoDb.dbStartup( USER_JOIN_DB_VERSION, dbFileName );

        m_UserJoinedLastDb.dbShutdown();
        m_UserJoinedLastDb.dbStartup( USER_JOINED_LAST_DB_VERSION, dbLastJoinedFileName );

        clearUserJoinInfoList();
        m_UserJoinInfoDb.getAllUserJoins( m_UserJoinInfoList );
        for( auto iter = m_UserJoinInfoList.begin();  iter != m_UserJoinInfoList.end(); ++iter )
        {
            UserJoinInfo* userJoinInfo = iter->second;
            if( !userJoinInfo->getNetIdent() )
            {
                VxNetIdent* netIdent = m_Engine.getBigListMgr().findBigListInfo( userJoinInfo->getOnlineId() );
                if( netIdent )
                {
                    userJoinInfo->setNetIdent( netIdent );
                }
                else if( userJoinInfo->getOnlineId() == m_Engine.getMyOnlineId() && m_Engine.getMyNetIdent()->isValidNetIdent() )
                {
                    // is myself
                    userJoinInfo->setNetIdent( m_Engine.getMyNetIdent() );
                }
            }
        }

        m_UserJoinListInitialized = true;
        unlockResources();
    }

    LogModule( eLogStartup, LOG_VERBOSE, "UserJoinMgr::fromGuiUserLoggedOn done" );
}

//============================================================================
void UserJoinMgr::addUserJoinMgrClient( UserJoinCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_UserJoinClients.push_back( client );
    }
    else
    {
        std::vector<UserJoinCallbackInterface *>::iterator iter;
        for( iter = m_UserJoinClients.begin(); iter != m_UserJoinClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_UserJoinClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void UserJoinMgr::announceUserJoinRequested( UserJoinInfo* userJoinInfo )
{
    if( userJoinInfo )
    {
        LogMsg( LOG_INFO, "UserJoinMgr::announceUserJoinRequested start" );

        lockClientList();
        std::vector<UserJoinCallbackInterface*>::iterator iter;
        for( iter = m_UserJoinClients.begin(); iter != m_UserJoinClients.end(); ++iter )
        {
            UserJoinCallbackInterface* client = *iter;
            client->callbackUserJoinAdded( userJoinInfo );
        }

        unlockClientList();
        LogMsg( LOG_INFO, "UserJoinMgr::announceUserJoinRequested done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "UserJoinMgr::announceUserJoinRequested dynamic_cast failed" );
    }
}

//============================================================================
void UserJoinMgr::announceUserJoinUpdated( UserJoinInfo * userJoinInfo )
{
    if( userJoinInfo )
    {
        lockClientList();
        std::vector<UserJoinCallbackInterface *>::iterator iter;
        for( iter = m_UserJoinClients.begin();	iter != m_UserJoinClients.end(); ++iter )
        {
            UserJoinCallbackInterface * client = *iter;
            client->callbackUserJoinUpdated( userJoinInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "UserJoinMgr::announceUserJoinRemoved null" );
    }
}

//============================================================================
void UserJoinMgr::announceUserUnJoinUpdated( UserJoinInfo* userJoinInfo )
{
    if( userJoinInfo )
    {
        lockClientList();
        std::vector<UserJoinCallbackInterface*>::iterator iter;
        for( iter = m_UserJoinClients.begin(); iter != m_UserJoinClients.end(); ++iter )
        {
            UserJoinCallbackInterface* client = *iter;
            client->callbackUserUnJoinUpdated( userJoinInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "UserJoinMgr::announceUserUnJoinUpdated null" );
    }
}

//============================================================================
void UserJoinMgr::announceUserJoinRemoved( GroupieId& groupieId )
{
    removeFromDatabase( groupieId, false );

	lockClientList();
	std::vector<UserJoinCallbackInterface *>::iterator iter;
	for( iter = m_UserJoinClients.begin();	iter != m_UserJoinClients.end(); ++iter )
	{
		UserJoinCallbackInterface * client = *iter;
		client->callbackUserJoinRemoved( groupieId );
	}

	unlockClientList();
}

//============================================================================
void UserJoinMgr::clearUserJoinInfoList( void )
{
    for( auto iter = m_UserJoinInfoList.begin(); iter != m_UserJoinInfoList.end(); ++iter )
    {
        delete iter->second;
    }

    m_UserJoinInfoList.clear();
}

//============================================================================
void UserJoinMgr::onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    lockResources();
    UserJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( !joinInfo )
    {
        joinInfo = new UserJoinInfo();
        joinInfo->setGroupieId( groupieId );
        joinInfo->fillBaseInfo( netIdent, PluginTypeToHostType( sessionInfo.getPluginType() ) );
        joinInfo->setPluginType( sessionInfo.getPluginType() );
        wasAdded = true;
    }

    joinInfo->setNetIdent( netIdent );
    int64_t timeNowMs = GetTimeStampMs();
    joinInfo->setThumbId( netIdent->getThumbId( PluginTypeToHostType( sessionInfo.getPluginType() ) ) );
    joinInfo->setJoinState( eJoinStateJoinGranted );
    joinInfo->setHostUrl( netIdent->getMyOnlineUrl() );

    joinInfo->setConnectionId( sktBase->getConnectionId() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );

    joinInfo->setInfoModifiedTime( timeNowMs );
    joinInfo->setLastConnectTime( timeNowMs );
    joinInfo->setLastJoinTime( timeNowMs );
    if( wasAdded )
    {
        m_UserJoinInfoList[groupieId] = joinInfo;
    }

    saveToDatabase( joinInfo, true );

    unlockResources();

    m_Engine.getThumbMgr().queryThumbIfNeeded( sktBase, netIdent, sessionInfo.getPluginType() );

    if( wasAdded )
    {
        announceUserJoinRequested( joinInfo );
    }
    else
    {
        announceUserJoinUpdated( joinInfo );
    } 
}

//============================================================================
void UserJoinMgr::onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    lockResources();
    UserJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( !joinInfo )
    {
        joinInfo = new UserJoinInfo();
        joinInfo->setGroupieId( groupieId );
        joinInfo->fillBaseInfo( netIdent, PluginTypeToHostType( sessionInfo.getPluginType() ) );
        joinInfo->setPluginType( sessionInfo.getPluginType() );
        wasAdded = true;
    }

    joinInfo->setNetIdent( netIdent );
    int64_t timeNowMs = GetTimeStampMs();
    joinInfo->setThumbId( netIdent->getThumbId( PluginTypeToHostType( sessionInfo.getPluginType() ) ) );
    joinInfo->setJoinState( eJoinStateJoinLeaveHost );
    joinInfo->setHostUrl( netIdent->getMyOnlineUrl() );

    joinInfo->setConnectionId( sktBase->getConnectionId() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );

    joinInfo->setInfoModifiedTime( timeNowMs );
    joinInfo->setLastConnectTime( timeNowMs );
    joinInfo->setLastJoinTime( timeNowMs );
    if( wasAdded )
    {
        m_UserJoinInfoList[groupieId] = joinInfo;
    }

    saveToDatabase( joinInfo, true );

    unlockResources();

    if( wasAdded )
    {
        announceUserJoinRequested( joinInfo );
    }
    else
    {
        announceUserJoinUpdated( joinInfo );
    }
}

//============================================================================
void UserJoinMgr::onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    lockResources();
    UserJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( joinInfo )
    {


        joinInfo->setNetIdent( netIdent );
        int64_t timeNowMs = GetTimeStampMs();
        joinInfo->setThumbId( netIdent->getThumbId( PluginTypeToHostType( sessionInfo.getPluginType() ) ) );
        joinInfo->setJoinState( eJoinStateJoinGranted );
        joinInfo->setHostUrl( netIdent->getMyOnlineUrl() );

        joinInfo->setConnectionId( sktBase->getConnectionId() );
        joinInfo->setSessionId( sessionInfo.getSessionId() );

        joinInfo->setInfoModifiedTime( timeNowMs );
        joinInfo->setLastConnectTime( timeNowMs );
        joinInfo->setLastJoinTime( 0 );


        saveToDatabase( joinInfo, true );

        unlockResources();

        m_Engine.getThumbMgr().queryThumbIfNeeded( sktBase, netIdent, sessionInfo.getPluginType() );

        announceUserUnJoinUpdated( joinInfo );
    }
}

//============================================================================
UserJoinInfo* UserJoinMgr::findUserJoinInfo( GroupieId& groupieId )
{
    auto iter = m_UserJoinInfoList.find( groupieId );
    if( iter != m_UserJoinInfoList.end() )
    {
        return iter->second;
    }

    return nullptr;
}

//============================================================================
bool UserJoinMgr::saveToDatabase( UserJoinInfo* joinInfo, bool isLocked )
{
    bool result{ false };
    if( joinInfo->getOnlineId() == m_Engine.getMyOnlineId() )
    {
        // do not add ourself to database. If we joined then we are the admin
        // and we may join another host at the same time
        return true;
    }

    if( !isLocked )
    {
        lockResources();
    }

    VxPtopUrl ptopUrl( joinInfo->getHostUrl() );
    if( ptopUrl.isValid() )
    {
        result = m_UserJoinInfoDb.addUserJoin( joinInfo );

        result &= m_UserJoinedLastDb.setJoinedLast( joinInfo->getHostType(), ptopUrl.getOnlineId(), joinInfo->getLastJoinTime(), joinInfo->getHostUrl() );

        result &= m_UserJoinedLastDb.setJoinedLastHostType( joinInfo->getHostType() );
    }

    if( !isLocked )
    {
        unlockResources();
    }

    return result;
}

//============================================================================
void UserJoinMgr::removeFromDatabase( GroupieId& groupieId, bool resourcesLocked )
{
    if( !resourcesLocked )
    {
        lockResources();
    }

    m_UserJoinInfoDb.removeUserJoin( groupieId );
    if( !resourcesLocked )
    {
        unlockResources();
    }
}

//============================================================================
bool UserJoinMgr::saveToJoinedLastDatabase( UserJoinInfo* joinInfo, bool isLocked )
{
    if( joinInfo->getOnlineId() == m_Engine.getMyOnlineId() )
    {
        // do not add ourself to database. If we joined then we are the admin
        // and we may join another host at the same time
        return true;
    }

    if( !isLocked )
    {
        lockResources();
    }

    bool result = m_UserJoinInfoDb.addUserJoin( joinInfo );

    if( !isLocked )
    {
        unlockResources();
    }

    return result;
}

//============================================================================
void UserJoinMgr::removeFromJoinedLastDatabase( GroupieId& groupieId, bool resourcesLocked )
{
    if( !resourcesLocked )
    {
        lockResources();
    }

    m_UserJoinInfoDb.removeUserJoin( groupieId );
    if( !resourcesLocked )
    {
        unlockResources();
    }
}

//============================================================================
void UserJoinMgr::onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId )
{
    // TODO BRJ handle disconnect
}

//============================================================================
bool UserJoinMgr::getLastJoinedHostUrl( EHostType hostType, std::string& retHostUrl )
{
    VxGUID onlineId;
    int64_t lastJoinMs;
    bool result = m_UserJoinedLastDb.getJoinedLast( hostType, onlineId, lastJoinMs, retHostUrl );
    if( result )
    {
        VxPtopUrl ptopUrl( retHostUrl );
        result = ptopUrl.isValid();
        if( !result )
        {
            LogMsg( LOG_ERROR, "UserJoinMgr::getLastJoinedHostUrl invalid url for host type %d", hostType );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "UserJoinMgr::getLastJoinedHostUrl no last joined host url for host type %d", hostType );
    }

    return result;
}

//============================================================================
void UserJoinMgr::changeJoinState( GroupieId& groupieId, EJoinState joinState )
{
    lockResources();
    UserJoinInfo* joinInfo = findUserJoinInfo( groupieId );
    if( joinInfo && joinInfo->setJoinState( joinState ) )
    {
        if( groupieId.getGroupieOnlineId() != m_Engine.getMyOnlineId() )
        {
            saveToDatabase( joinInfo, true );
        }

        announceUserJoinUpdated( joinInfo );
    }

    unlockResources();
}
