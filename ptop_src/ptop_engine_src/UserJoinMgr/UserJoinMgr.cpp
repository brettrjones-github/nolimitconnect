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
#include <ptop_src/ptop_engine_src/BaseInfo/BaseSessionInfo.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

//============================================================================
UserJoinMgr::UserJoinMgr( P2PEngine& engine, const char * dbName, const char * dbStateName )
: m_Engine( engine )
, m_UserJoinInfoDb( engine, *this, dbName )
{
}

//============================================================================
void UserJoinMgr::fromGuiUserLoggedOn( void )
{
    // dont call HostBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_UserJoinInfoDb.getDatabaseName(); 
        lockResources();
        m_UserJoinInfoDb.dbShutdown();
        m_UserJoinInfoDb.dbStartup( USER_JOIN_DB_VERSION, dbFileName );

        clearUserJoinInfoList();
        m_UserJoinInfoDb.getAllUserJoins( m_UserJoinInfoList );

        m_UserJoinListInitialized = true;
        unlockResources();
    }
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
void UserJoinMgr::announceUserJoinRequested( UserJoinInfo* joinInfo )
{
    UserJoinInfo* userHostInfo = dynamic_cast<UserJoinInfo*>(joinInfo);
    if( userHostInfo )
    {
        LogMsg( LOG_INFO, "UserJoinMgr::announceUserJoinRequested start" );

        lockClientList();
        std::vector<UserJoinCallbackInterface*>::iterator iter;
        for( iter = m_UserJoinClients.begin(); iter != m_UserJoinClients.end(); ++iter )
        {
            UserJoinCallbackInterface* client = *iter;
            client->callbackUserJoinRequested( userHostInfo );
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
void UserJoinMgr::announceUserJoinUpdated( UserJoinInfo * joinInfo )
{
    UserJoinInfo * userHostInfo = dynamic_cast<UserJoinInfo *>( joinInfo );
    if( userHostInfo )
    {
        lockClientList();
        std::vector<UserJoinCallbackInterface *>::iterator iter;
        for( iter = m_UserJoinClients.begin();	iter != m_UserJoinClients.end(); ++iter )
        {
            UserJoinCallbackInterface * client = *iter;
            client->callbackUserJoinUpdated( userHostInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "UserJoinMgr::announceUserJoinRemoved dynamic_cast failed" );
    }
}

//============================================================================
void UserJoinMgr::announceUserJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType )
{
	lockClientList();
	std::vector<UserJoinCallbackInterface *>::iterator iter;
	for( iter = m_UserJoinClients.begin();	iter != m_UserJoinClients.end(); ++iter )
	{
		UserJoinCallbackInterface * client = *iter;
		client->callbackUserJoinRemoved( hostOnlineId, pluginType );
	}

	unlockClientList();
}

//============================================================================
void UserJoinMgr::clearUserJoinInfoList( void )
{
    for( auto iter = m_UserJoinInfoList.begin(); iter != m_UserJoinInfoList.end(); ++iter )
    {
        delete (*iter);
    }

    m_UserJoinInfoList.clear();
}

//============================================================================
void UserJoinMgr::onUserJoinedHost( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    lockResources();
    UserJoinInfo* joinInfo = findUserJoinInfo( netIdent->getMyOnlineId(), sessionInfo.getPluginType() );
    if( !joinInfo )
    {
        joinInfo = new UserJoinInfo();
        joinInfo->fillBaseInfo( netIdent, PluginTypeToHostType( sessionInfo.getPluginType() ) );
        joinInfo->setPluginType( sessionInfo.getPluginType() );
        wasAdded = true;
    }

    joinInfo->setNetIdent( netIdent );
    int64_t timeNowMs = GetTimeStampMs();
    joinInfo->setThumbId( netIdent->getThumbId( PluginTypeToHostType( sessionInfo.getPluginType() ) ) );
    joinInfo->setJoinState( eJoinStateJoinAccepted );
    joinInfo->setHostUrl( netIdent->getMyOnlineUrl() );

    joinInfo->setConnectionId( sktBase->getConnectionId() );
    joinInfo->setSessionId( sessionInfo.getSessionId() );

    joinInfo->setInfoModifiedTime( timeNowMs );
    joinInfo->setLastConnectTime( timeNowMs );
    joinInfo->setLastJoinTime( timeNowMs );

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
UserJoinInfo* UserJoinMgr::findUserJoinInfo( VxGUID& hostOnlineId, EPluginType pluginType )
{
    UserJoinInfo* joinFoundInfo = nullptr;
    for( auto joinInfo : m_UserJoinInfoList )
    {
        if( joinInfo->getOnlineId() == hostOnlineId && joinInfo->getPluginType() == pluginType )
        {
            joinFoundInfo = joinInfo;
            break;
        }
    }

    return joinFoundInfo;
}

//============================================================================
bool UserJoinMgr::saveToDatabase( UserJoinInfo* joinInfo, bool isLocked )
{
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
void UserJoinMgr::onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId )
{
    // TODO BRJ handle disconnect
}