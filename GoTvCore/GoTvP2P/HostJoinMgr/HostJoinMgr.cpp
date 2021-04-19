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
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

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
void HostJoinMgr::announceHostJoinOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState )
{
	LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinXferState state %d start\n", userHostOfferState );
	lockClientList();
	std::vector<HostJoinCallbackInterface *>::iterator iter;
	for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
	{
		HostJoinCallbackInterface * client = *iter;
		client->callbackHostJoinOfferState( hostOnlineId, userHostOfferState );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinXferState state %d done\n", userHostOfferState );
}

//============================================================================
void HostJoinMgr::announceHostJoinOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId )
{
    LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinOnlineState state %d start\n", onlineState );
    lockClientList();
    std::vector<HostJoinCallbackInterface *>::iterator iter;
    for( iter = m_HostJoinClients.begin();	iter != m_HostJoinClients.end(); ++iter )
    {
        HostJoinCallbackInterface * client = *iter;
        client->callbackHostJoinOnlineState( hostOnlineId, onlineState, connectionId );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "HostJoinMgr::announceHostJoinOnlineState state %d done\n", onlineState );
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
void HostJoinMgr::onHostJoinedByUser( VxSktBase * sktBase, VxNetIdent * netIdent, VxGUID sessionId, EPluginType pluginType, EHostType hostType )
{
    // TODO IMPLEMENT
    vx_assert( false );
}
