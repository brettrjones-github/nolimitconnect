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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

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
void UserJoinMgr::announceUserJoinAdded( UserJoinInfo * joinInfo )
{
    UserJoinInfo * userHostInfo = dynamic_cast<UserJoinInfo *>( joinInfo );
    if( userHostInfo )
    {
	    LogMsg( LOG_INFO, "UserJoinMgr::announceUserJoinAdded start" );
	
	    lockClientList();
	    std::vector<UserJoinCallbackInterface *>::iterator iter;
	    for( iter = m_UserJoinClients.begin();	iter != m_UserJoinClients.end(); ++iter )
	    {
		    UserJoinCallbackInterface * client = *iter;
		    client->callbackUserJoinAdded( userHostInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "UserJoinMgr::announceUserJoinAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "UserJoinMgr::announceUserJoinAdded dynamic_cast failed" );
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
void UserJoinMgr::announceUserJoinRemoved( VxGUID& hostOnlineId, EHostType hostType )
{
	lockClientList();
	std::vector<UserJoinCallbackInterface *>::iterator iter;
	for( iter = m_UserJoinClients.begin();	iter != m_UserJoinClients.end(); ++iter )
	{
		UserJoinCallbackInterface * client = *iter;
		client->callbackUserJoinRemoved( hostOnlineId, hostType );
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
void UserJoinMgr::onUserJoinedHost( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EPluginType pluginType, EHostType hostType )
{
    bool wasAdded = false;
    lockResources();
    UserJoinInfo* joinInfo = findUserJoinInfo( netIdent->getMyOnlineId(), pluginType );
    if( !joinInfo )
    {
        joinInfo = new UserJoinInfo();
        joinInfo->fillBaseInfo( netIdent, hostType );
        joinInfo->setPluginType( pluginType );
        joinInfo->setHostType( hostType );
        wasAdded = true;
    }

    int64_t timeNowMs = GetTimeStampMs();
    joinInfo->setThumbId( netIdent->getThumbId( hostType ) );
    joinInfo->setJoinState( eJoinStateJoinAccepted );
    joinInfo->setHostUrl( netIdent->getMyOnlineUrl() );

    joinInfo->setConnectionId( sktBase->getConnectionId() );
    joinInfo->setSessionId( sessionId );

    joinInfo->setInfoModifiedTime( timeNowMs );
    joinInfo->setLastConnectTime( timeNowMs );
    joinInfo->setLastJoinTime( timeNowMs );

    unlockResources();

    m_Engine.getThumbMgr().queryThumbIfNeeded( sktBase, netIdent, pluginType );

    if( wasAdded )
    {
        announceUserJoinAdded( joinInfo );
    }
    else
    {
        announceUserJoinUpdated( joinInfo );
    }

    saveToDatabase( joinInfo );
}

//============================================================================
UserJoinInfo* UserJoinMgr::findUserJoinInfo( VxGUID& hostOnlineId, EHostType hostType )
{
    UserJoinInfo* joinFoundInfo = nullptr;
    for( auto joinInfo : m_UserJoinInfoList )
    {
        if( joinInfo->getOnlineId() == hostOnlineId && joinInfo->getHostType() == hostType )
        {
            joinFoundInfo = joinInfo;
            break;
        }
    }

    return joinFoundInfo;
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
bool UserJoinMgr::saveToDatabase( UserJoinInfo* joinInfo )
{
    lockResources();

    bool result = m_UserJoinInfoDb.addUserJoin( joinInfo );

    unlockResources();
    return result;
}