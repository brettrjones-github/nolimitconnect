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
// http://www.nolimitconnect.org
//============================================================================

#include "OfferHostMgr.h"
#include "OfferHostInfo.h"
#include "OfferHostInfoDb.h"
#include "OfferHostCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

//============================================================================
OfferHostMgr::OfferHostMgr( P2PEngine& engine, const char * dbName, const char * stateDbName )
: OfferBaseMgr( engine, dbName, stateDbName, eOfferMgrTypeOfferHost )
, m_OfferHostInfoDb( dynamic_cast<OfferHostInfoDb&>(OfferBaseMgr::getOfferInfoDb()) )
, m_OfferHostInfoList( getOfferBaseInfoList() )
{
}

//============================================================================
OfferBaseInfo * OfferHostMgr::createOfferInfo( const char * assetName, uint64_t assetLen, uint16_t assetType )
{
    return new OfferHostInfo( assetName, assetLen, assetType );
}

//============================================================================
OfferBaseInfo * OfferHostMgr::createOfferInfo( OfferBaseInfo& assetInfo )
{
    return new OfferHostInfo( assetInfo );
}

//============================================================================
void OfferHostMgr::fromGuiUserLoggedOn( void )
{
    // dont call OfferBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_OfferHostInfoDb.getDatabaseName(); 
        lockResources();
        m_OfferHostInfoDb.dbShutdown();
        m_OfferHostInfoDb.dbStartup( THUMB_DB_VERSION, dbFileName );

        clearOfferInfoList();
        m_OfferHostInfoDb.getAllOffers( m_OfferHostInfoList );

        m_OfferHostListInitialized = true;
        unlockResources();
    }
}

//============================================================================
void OfferHostMgr::addOfferHostMgrClient( OfferHostCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_OfferHostClients.push_back( client );
    }
    else
    {
        std::vector<OfferHostCallbackInterface *>::iterator iter;
        for( iter = m_OfferHostClients.begin(); iter != m_OfferHostClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_OfferHostClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void OfferHostMgr::announceOfferAdded( OfferBaseInfo * assetInfo )
{
    OfferBaseMgr::announceOfferAdded( assetInfo );
    OfferHostInfo * thumbInfo = dynamic_cast<OfferHostInfo *>( assetInfo );
    if( thumbInfo )
    {
	    LogMsg( LOG_INFO, "OfferHostMgr::announceOfferHostAdded start" );
	
	    lockClientList();
	    std::vector<OfferHostCallbackInterface *>::iterator iter;
	    for( iter = m_OfferHostClients.begin();	iter != m_OfferHostClients.end(); ++iter )
	    {
		    OfferHostCallbackInterface * client = *iter;
		    client->callbackOfferHostAdded( thumbInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "OfferHostMgr::announceOfferHostAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferHostMgr::announceOfferAdded dynamic_cast failed" );
    }
}

//============================================================================
void OfferHostMgr::announceOfferUpdated( OfferBaseInfo * assetInfo )
{
    OfferBaseMgr::announceOfferUpdated( assetInfo );
    OfferHostInfo * thumbInfo = dynamic_cast<OfferHostInfo *>( assetInfo );
    if( thumbInfo )
    {
        lockClientList();
        std::vector<OfferHostCallbackInterface *>::iterator iter;
        for( iter = m_OfferHostClients.begin();	iter != m_OfferHostClients.end(); ++iter )
        {
            OfferHostCallbackInterface * client = *iter;
            client->callbackOfferHostUpdated( thumbInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferHostMgr::announceOfferRemoved dynamic_cast failed" );
    }
}

//============================================================================
void OfferHostMgr::announceOfferRemoved( OfferBaseInfo * assetInfo )
{
    OfferBaseMgr::announceOfferRemoved( assetInfo );
    OfferHostInfo * thumbInfo = dynamic_cast<OfferHostInfo *>( assetInfo );
    if( thumbInfo )
    {
	    lockClientList();
	    std::vector<OfferHostCallbackInterface *>::iterator iter;
	    for( iter = m_OfferHostClients.begin();	iter != m_OfferHostClients.end(); ++iter )
	    {
		    OfferHostCallbackInterface * client = *iter;
		    client->callbackOfferHostRemoved( thumbInfo->getOfferId() );
	    }

	    unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferHostMgr::announceOfferRemoved dynamic_cast failed" );
    }
}

//============================================================================
void OfferHostMgr::announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
    OfferBaseMgr::announceOfferXferState( assetOfferId, assetSendState, param );

	LogMsg( LOG_INFO, "OfferHostMgr::announceOfferXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<OfferHostCallbackInterface *>::iterator iter;
	for( iter = m_OfferHostClients.begin();	iter != m_OfferHostClients.end(); ++iter )
	{
		OfferHostCallbackInterface * client = *iter;
		client->callbackOfferSendState( assetOfferId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "OfferHostMgr::announceOfferXferState state %d done\n", assetSendState );
}

//============================================================================
bool OfferHostMgr::fromGuiOfferHostCreated( OfferHostInfo& thumbInfo )
{
    return addOffer( thumbInfo );
}

//============================================================================
bool OfferHostMgr::fromGuiOfferHostUpdated( OfferHostInfo& thumbInfo )
{
    return updateOffer( thumbInfo );
}