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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BaseInfo/BaseSessionInfo.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

//============================================================================
HostJoinMgr::HostJoinMgr( P2PEngine& engine, const char * dbName, const char * dbStateName )
: m_Engine( engine )
, m_HostJoinInfoDb( engine, *this, dbName )
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
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_HostJoinInfoDb.getDatabaseName(); 
        lockResources();
        m_HostJoinInfoDb.dbShutdown();
        m_HostJoinInfoDb.dbStartup( USER_HOST_DB_VERSION, dbFileName );

        clearHostJoinInfoList();
        m_HostJoinInfoDb.getAllHostJoins( m_HostJoinInfoList );

        m_HostJoinListInitialized = true;
        unlockResources();
    }
}

//============================================================================
void HostJoinMgr::addHostJoinMgrClient( HostJoinCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_HostJoinClients.push_back( client );
    }
    else
    {
        std::vector<HostJoinCallbackInterface *>::iterator iter;
        for( iter = m_HostJoinClients.begin(); iter != m_HostJoinClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_HostJoinClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void HostJoinMgr::announceHostJoinAdded( HostJoinInfo * assetInfo )
{
    HostJoinInfo * userHostInfo = dynamic_cast<HostJoinInfo *>( assetInfo );
    if( userHostInfo )
    {
	    LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinAdded start" );
	
	    lockClientList();
	    std::vector<HostJoinCallbackInterface *>::iterator iter;
	    for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
	    {
		    HostJoinCallbackInterface * client = *iter;
		    client->callbackHostJoinAdded( userHostInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostJoinAdded dynamic_cast failed" );
    }
}

//============================================================================
void HostJoinMgr::announceHostJoinUpdated( HostJoinInfo * assetInfo )
{
    HostJoinInfo * userHostInfo = dynamic_cast<HostJoinInfo *>( assetInfo );
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
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostJoinRemoved dynamic_cast failed" );
    }
}

//============================================================================
void HostJoinMgr::announceHostJoinRemoved( VxGUID& hostOnlineId )
{
	lockClientList();
	std::vector<HostJoinCallbackInterface *>::iterator iter;
	for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
	{
		HostJoinCallbackInterface * client = *iter;
		client->callbackHostJoinRemoved( hostOnlineId );
	}

	unlockClientList();
}

//============================================================================
void HostJoinMgr::clearHostJoinInfoList( void )
{
    for( auto iter = m_HostJoinInfoList.begin(); iter != m_HostJoinInfoList.end(); ++iter )
    {
        delete (*iter);
    }

    m_HostJoinInfoList.clear();
}

//============================================================================
void HostJoinMgr::onHostJoinedByUser( VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( netIdent->getMyOnlineId(), sessionInfo.getPluginType() );
    if( !joinInfo )
    {
        joinInfo = new HostJoinInfo();
        joinInfo->fillBaseInfo( netIdent, PluginTypeToHostType( sessionInfo.getPluginType() ) );
        joinInfo->setPluginType( sessionInfo.getPluginType() );
        joinInfo->setSessionId( sessionInfo.getSessionId() );
        wasAdded = true;
    }

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
        announceHostJoinAdded( joinInfo );
    }
    else
    {
        announceHostJoinUpdated( joinInfo );
    }

    // saveToDatabase( joinInfo );
}

//============================================================================
HostJoinInfo* HostJoinMgr::findUserJoinInfo( VxGUID& hostOnlineId, EPluginType pluginType )
{
    HostJoinInfo* joinFoundInfo = nullptr;
    for( auto joinInfo : m_HostJoinInfoList )
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
bool HostJoinMgr::saveToDatabase( HostJoinInfo* joinInfo )
{
    lockResources();

    bool result = m_HostJoinInfoDb.addHostJoin( joinInfo );

    unlockResources();
    return result;
}