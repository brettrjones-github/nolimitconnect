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
        for( auto* joinInfo : m_HostJoinInfoList )
        {
            if( !joinInfo->getNetIdent() )
            {
                VxNetIdent* netIdent = m_Engine.getBigListMgr().findBigListInfo( joinInfo->getOnlineId() );
                if( netIdent )
                {
                    joinInfo->setNetIdent( netIdent );
                }
                else if( joinInfo->getOnlineId() == m_Engine.getMyOnlineId() && m_Engine.getMyNetIdent()->isIdentValid() )
                {
                    // is myself
                    joinInfo->setNetIdent( m_Engine.getMyNetIdent() );
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
void HostJoinMgr::announceHostJoinRemoved( VxGUID& hostOnlineId, EPluginType pluginType )
{
    removeFromDatabase( hostOnlineId, pluginType, false );
	lockClientList();
	std::vector<HostJoinCallbackInterface *>::iterator iter;
	for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
	{
		HostJoinCallbackInterface * client = *iter;
		client->callbackHostJoinRemoved( hostOnlineId, pluginType );
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
void HostJoinMgr::onHostJoinRequestedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    bool wasAdded = false;
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( netIdent->getMyOnlineId(), sessionInfo.getPluginType() );
    if( !joinInfo )
    {
        joinInfo = new HostJoinInfo();
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
        m_HostJoinInfoList.push_back( joinInfo );
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
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( netIdent->getMyOnlineId(), sessionInfo.getPluginType() );
    if( !joinInfo )
    {
        joinInfo = new HostJoinInfo();
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
bool HostJoinMgr::saveToDatabase( HostJoinInfo* joinInfo, bool resourcesLocked )
{
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
void HostJoinMgr::removeFromDatabase( VxGUID& hostOnlineId, EPluginType pluginType, bool resourcesLocked )
{
    if( !resourcesLocked )
    {
        lockResources();
    }

    m_HostJoinInfoDb.removeHostJoin( hostOnlineId, pluginType );
    if( !resourcesLocked )
    {
        unlockResources();
    }
}

//============================================================================
void HostJoinMgr::fromGuiGetJoinedStateList( EPluginType pluginType, EJoinState joinState, std::vector<HostJoinInfo*>& hostJoinList )
{
    // NOTE: assumes resources have been locked
    HostJoinInfo* joinFoundInfo = nullptr;
    for( auto* joinInfo : m_HostJoinInfoList )
    {
        if( joinInfo->getPluginType() == pluginType && joinInfo->getJoinState() == joinState )
        {
            hostJoinList.push_back(joinInfo);
        }
    }
}

//============================================================================
int HostJoinMgr::fromGuiGetJoinedListCount( EPluginType pluginType )
{
    int joinedCnt = 0;
    lockResources();

    HostJoinInfo* joinFoundInfo = nullptr;
    for( auto joinInfo : m_HostJoinInfoList )
    {
        if( joinInfo->getPluginType() == pluginType )
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

    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( netIdent.getMyOnlineId(), HostTypeToHostPlugin( hostType ) );
    if( joinInfo )
    {
        hostJoinState = joinInfo->getJoinState();
    }
    else if( netIdent.getMyOnlineId() == m_Engine.getMyOnlineId() )
    {
        // if we are host we can always join our own hosted servers

        // commented out temporarily for development
        // hostJoinState = eJoinStateJoinAccepted;
    }

    unlockResources();
    return hostJoinState;
}

//============================================================================
void HostJoinMgr::onConnectionLost( VxSktBase* sktBase, VxGUID& connectionId, VxGUID& peerOnlineId )
{
    // TODO BRJ handle disconnect
}

//============================================================================
void HostJoinMgr::changeJoinState( VxGUID& onlineId, EPluginType pluginType, EJoinState joinState )
{
    lockResources();
    HostJoinInfo* joinInfo = findUserJoinInfo( onlineId, pluginType );
    if( joinInfo && joinInfo->setJoinState( joinState ) )
    {
        if( onlineId != m_Engine.getMyOnlineId() )
        {
            saveToDatabase( joinInfo, true );
        }

        announceHostJoinUpdated( joinInfo );
    }

    unlockResources();
}