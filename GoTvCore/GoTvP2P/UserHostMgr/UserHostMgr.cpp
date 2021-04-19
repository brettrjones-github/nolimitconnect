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

#include "UserHostMgr.h"
#include "UserHostInfo.h"
#include "UserHostInfoDb.h"
#include "UserHostCallbackInterface.h"

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
UserHostMgr::UserHostMgr( P2PEngine& engine, const char * dbName, const char * dbStateName )
: m_Engine( engine )
, m_UserHostInfoDb( engine, *this, dbName )
{
}

//============================================================================
void UserHostMgr::fromGuiUserLoggedOn( void )
{
    // dont call HostBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_UserHostInfoDb.getDatabaseName(); 
        lockResources();
        m_UserHostInfoDb.dbShutdown();
        m_UserHostInfoDb.dbStartup( USER_HOST_DB_VERSION, dbFileName );

        clearUserHostInfoList();
        m_UserHostInfoDb.getAllUserHosts( m_UserHostInfoList );

        m_UserHostListInitialized = true;
        unlockResources();
    }
}

//============================================================================
void UserHostMgr::addUserHostMgrClient( UserHostCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_UserHostClients.push_back( client );
    }
    else
    {
        std::vector<UserHostCallbackInterface *>::iterator iter;
        for( iter = m_UserHostClients.begin(); iter != m_UserHostClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_UserHostClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void UserHostMgr::announceUserHostAdded( UserHostInfo * assetInfo )
{
    UserHostInfo * userHostInfo = dynamic_cast<UserHostInfo *>( assetInfo );
    if( userHostInfo )
    {
	    LogMsg( LOG_INFO, "UserHostMgr::announceUserHostAdded start" );
	
	    lockClientList();
	    std::vector<UserHostCallbackInterface *>::iterator iter;
	    for( iter = m_UserHostClients.begin();	iter != m_UserHostClients.end(); ++iter )
	    {
		    UserHostCallbackInterface * client = *iter;
		    client->callbackUserHostAdded( userHostInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "UserHostMgr::announceUserHostAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "UserHostMgr::announceUserHostAdded dynamic_cast failed" );
    }
}

//============================================================================
void UserHostMgr::announceUserHostUpdated( UserHostInfo * assetInfo )
{
    UserHostInfo * userHostInfo = dynamic_cast<UserHostInfo *>( assetInfo );
    if( userHostInfo )
    {
        lockClientList();
        std::vector<UserHostCallbackInterface *>::iterator iter;
        for( iter = m_UserHostClients.begin();	iter != m_UserHostClients.end(); ++iter )
        {
            UserHostCallbackInterface * client = *iter;
            client->callbackUserHostUpdated( userHostInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "UserHostMgr::announceUserHostRemoved dynamic_cast failed" );
    }
}

//============================================================================
void UserHostMgr::announceUserHostRemoved( VxGUID& hostOnlineId )
{
	lockClientList();
	std::vector<UserHostCallbackInterface *>::iterator iter;
	for( iter = m_UserHostClients.begin();	iter != m_UserHostClients.end(); ++iter )
	{
		UserHostCallbackInterface * client = *iter;
		client->callbackUserHostRemoved( hostOnlineId );
	}

	unlockClientList();
}

//============================================================================
void UserHostMgr::announceUserHostOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState )
{
	LogMsg( LOG_INFO, "UserHostMgr::announceUserHostXferState state %d start\n", userHostOfferState );
	lockClientList();
	std::vector<UserHostCallbackInterface *>::iterator iter;
	for( iter = m_UserHostClients.begin();	iter != m_UserHostClients.end(); ++iter )
	{
		UserHostCallbackInterface * client = *iter;
		client->callbackUserHostOfferState( hostOnlineId, userHostOfferState );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "UserHostMgr::announceUserHostXferState state %d done\n", userHostOfferState );
}

//============================================================================
void UserHostMgr::announceUserHostOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId )
{
    LogMsg( LOG_INFO, "UserHostMgr::announceUserHostOnlineState state %d start\n", onlineState );
    lockClientList();
    std::vector<UserHostCallbackInterface *>::iterator iter;
    for( iter = m_UserHostClients.begin();	iter != m_UserHostClients.end(); ++iter )
    {
        UserHostCallbackInterface * client = *iter;
        client->callbackUserHostOnlineState( hostOnlineId, onlineState, connectionId );
    }

    unlockClientList();
    LogMsg( LOG_INFO, "UserHostMgr::announceUserHostOnlineState state %d done\n", onlineState );
}

//============================================================================
void UserHostMgr::clearUserHostInfoList( void )
{
    for( auto iter = m_UserHostInfoList.begin(); iter != m_UserHostInfoList.end(); ++iter )
    {
        delete (*iter);
    }

    m_UserHostInfoList.clear();
}

//============================================================================
void UserHostMgr::onUserJoinedHost( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EPluginType pluginType, EHostType hostType )
{
    // TODO IMPLEMENT
    vx_assert( false );
}
