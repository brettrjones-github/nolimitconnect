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

#include "ConnectMgr.h"
#include "ConnectInfo.h"
#include "ConnectInfoDb.h"
#include "ConnectCallbackInterface.h"

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
ConnectMgr::ConnectMgr( P2PEngine& engine, const char * dbName, const char * stateDbName )
: m_Engine( engine )
, m_ConnectInfoDb( engine, *this, dbName )
{
}

//============================================================================
void ConnectMgr::fromGuiUserLoggedOn( void )
{
    // dont call HostBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_ConnectInfoDb.getDatabaseName(); 
        lockResources();
        m_ConnectInfoDb.dbShutdown();
        m_ConnectInfoDb.dbStartup( CONNECT_MGR_DB_VERSION, dbFileName );

        clearConnectInfoList();
        m_ConnectInfoDb.getAllConnects( m_ConnectInfoList );

        m_ConnectListInitialized = true;
        unlockResources();
    }
}

//============================================================================
bool ConnectMgr::isConnectedToHost( EHostType hostType, VxUrl& hostUrl, VxSktBase*& sktBase )
{
    bool isConnected = false;
    if( hostUrl.getOnlineId() == m_Engine.getMyOnlineId() )
    {
        // we are connecting to ourself
        sktBase = m_Engine.getSktLoopback();
        return true;
    }
    else
    {
        // lookup connection

    }

    return isConnected;
}

//============================================================================
void ConnectMgr::addConnectMgrClient( ConnectCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_ConnectClients.push_back( client );
    }
    else
    {
        std::vector<ConnectCallbackInterface *>::iterator iter;
        for( iter = m_ConnectClients.begin(); iter != m_ConnectClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_ConnectClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void ConnectMgr::announceConnectAdded( ConnectInfo * assetInfo )
{
    ConnectInfo * userHostInfo = dynamic_cast<ConnectInfo *>( assetInfo );
    if( userHostInfo )
    {
	    LogMsg( LOG_INFO, "ConnectMgr::announceConnectAdded start" );
	
	    lockClientList();
	    std::vector<ConnectCallbackInterface *>::iterator iter;
	    for( iter = m_ConnectClients.begin();	iter != m_ConnectClients.end(); ++iter )
	    {
		    ConnectCallbackInterface * client = *iter;
		    client->callbackConnectAdded( userHostInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "ConnectMgr::announceConnectAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "ConnectMgr::announceConnectAdded dynamic_cast failed" );
    }
}

//============================================================================
void ConnectMgr::announceConnectUpdated( ConnectInfo * assetInfo )
{
    ConnectInfo * userHostInfo = dynamic_cast<ConnectInfo *>( assetInfo );
    if( userHostInfo )
    {
        lockClientList();
        std::vector<ConnectCallbackInterface *>::iterator iter;
        for( iter = m_ConnectClients.begin();	iter != m_ConnectClients.end(); ++iter )
        {
            ConnectCallbackInterface * client = *iter;
            client->callbackConnectUpdated( userHostInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "ConnectMgr::announceConnectRemoved dynamic_cast failed" );
    }
}

//============================================================================
void ConnectMgr::announceConnectRemoved( VxGUID& hostOnlineId )
{
	lockClientList();
	std::vector<ConnectCallbackInterface *>::iterator iter;
	for( iter = m_ConnectClients.begin();	iter != m_ConnectClients.end(); ++iter )
	{
		ConnectCallbackInterface * client = *iter;
		client->callbackConnectRemoved( hostOnlineId );
	}

	unlockClientList();
}

//============================================================================
void ConnectMgr::announceConnectOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState )
{
	LogMsg( LOG_INFO, "ConnectMgr::announceConnectXferState state %d start\n", userHostOfferState );
	lockClientList();
	std::vector<ConnectCallbackInterface *>::iterator iter;
	for( iter = m_ConnectClients.begin();	iter != m_ConnectClients.end(); ++iter )
	{
		ConnectCallbackInterface * client = *iter;
		client->callbackConnectOfferState( hostOnlineId, userHostOfferState );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "ConnectMgr::announceConnectXferState state %d done\n", userHostOfferState );
}

//============================================================================
void ConnectMgr::announceConnectOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId )
{
    LogMsg( LOG_INFO, "ConnectMgr::announceConnectOnlineState state %d start\n", onlineState );
    lockClientList();
    std::vector<ConnectCallbackInterface *>::iterator iter;
    for( iter = m_ConnectClients.begin();	iter != m_ConnectClients.end(); ++iter )
    {
        ConnectCallbackInterface * client = *iter;
        client->callbackConnectOnlineState( hostOnlineId, onlineState, connectionId );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "ConnectMgr::announceConnectOnlineState state %d done\n", onlineState );
}

//============================================================================
void ConnectMgr::clearConnectInfoList( void )
{
    for( auto iter = m_ConnectInfoList.begin(); iter != m_ConnectInfoList.end(); ++iter )
    {
        delete (*iter);
    }

    m_ConnectInfoList.clear();
}
