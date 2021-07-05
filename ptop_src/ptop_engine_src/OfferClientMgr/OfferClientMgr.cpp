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

#include "OfferClientMgr.h"
#include "OfferClientInfo.h"
#include "OfferClientInfoDb.h"
#include "OfferClientCallbackInterface.h"

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
OfferClientMgr::OfferClientMgr( P2PEngine& engine, const char * dbName, const char * stateDbName )
: OfferBaseMgr( engine, dbName, stateDbName, eOfferMgrTypeOfferClient )
, m_OfferClientInfoDb( dynamic_cast<OfferClientInfoDb&>(OfferBaseMgr::getOfferInfoDb()) )
, m_OfferClientInfoList( getOfferBaseInfoList() )
{
}

//============================================================================
OfferBaseInfo * OfferClientMgr::createOfferInfo( const char * assetName, uint64_t assetLen, uint16_t assetType )
{
    return new OfferClientInfo( assetName, assetLen, assetType );
}

//============================================================================
OfferBaseInfo * OfferClientMgr::createOfferInfo( OfferBaseInfo& assetInfo )
{
    return new OfferClientInfo( assetInfo );
}

//============================================================================
void OfferClientMgr::fromGuiUserLoggedOn( void )
{
    // dont call OfferBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_OfferClientInfoDb.getDatabaseName(); 
        lockResources();
        m_OfferClientInfoDb.dbShutdown();
        m_OfferClientInfoDb.dbStartup( THUMB_DB_VERSION, dbFileName );

        clearOfferInfoList();
        m_OfferClientInfoDb.getAllOffers( m_OfferClientInfoList );

        m_OfferClientListInitialized = true;
        unlockResources();
    }
}

//============================================================================
void OfferClientMgr::addOfferClientMgrClient( OfferClientCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_OfferClientClients.push_back( client );
    }
    else
    {
        std::vector<OfferClientCallbackInterface *>::iterator iter;
        for( iter = m_OfferClientClients.begin(); iter != m_OfferClientClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_OfferClientClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void OfferClientMgr::announceOfferAdded( OfferBaseInfo * assetInfo )
{
    OfferBaseMgr::announceOfferAdded( assetInfo );
    OfferClientInfo * thumbInfo = dynamic_cast<OfferClientInfo *>( assetInfo );
    if( thumbInfo )
    {
	    LogMsg( LOG_INFO, "OfferClientMgr::announceOfferClientAdded start" );
	
	    lockClientList();
	    std::vector<OfferClientCallbackInterface *>::iterator iter;
	    for( iter = m_OfferClientClients.begin();	iter != m_OfferClientClients.end(); ++iter )
	    {
		    OfferClientCallbackInterface * client = *iter;
		    client->callbackOfferClientAdded( thumbInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "OfferClientMgr::announceOfferClientAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferClientMgr::announceOfferAdded dynamic_cast failed" );
    }
}

//============================================================================
void OfferClientMgr::announceOfferUpdated( OfferBaseInfo * assetInfo )
{
    OfferBaseMgr::announceOfferUpdated( assetInfo );
    OfferClientInfo * thumbInfo = dynamic_cast<OfferClientInfo *>( assetInfo );
    if( thumbInfo )
    {
        lockClientList();
        std::vector<OfferClientCallbackInterface *>::iterator iter;
        for( iter = m_OfferClientClients.begin();	iter != m_OfferClientClients.end(); ++iter )
        {
            OfferClientCallbackInterface * client = *iter;
            client->callbackOfferClientUpdated( thumbInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferClientMgr::announceOfferRemoved dynamic_cast failed" );
    }
}

//============================================================================
void OfferClientMgr::announceOfferRemoved( OfferBaseInfo * assetInfo )
{
    OfferBaseMgr::announceOfferRemoved( assetInfo );
    OfferClientInfo * thumbInfo = dynamic_cast<OfferClientInfo *>( assetInfo );
    if( thumbInfo )
    {
	    lockClientList();
	    std::vector<OfferClientCallbackInterface *>::iterator iter;
	    for( iter = m_OfferClientClients.begin();	iter != m_OfferClientClients.end(); ++iter )
	    {
		    OfferClientCallbackInterface * client = *iter;
		    client->callbackOfferClientRemoved( thumbInfo->getOfferId() );
	    }

	    unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "OfferClientMgr::announceOfferRemoved dynamic_cast failed" );
    }
}

//============================================================================
void OfferClientMgr::announceOfferXferState( VxGUID& assetOfferId, EOfferSendState assetSendState, int param )
{
    OfferBaseMgr::announceOfferXferState( assetOfferId, assetSendState, param );

	LogMsg( LOG_INFO, "OfferClientMgr::announceOfferXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<OfferClientCallbackInterface *>::iterator iter;
	for( iter = m_OfferClientClients.begin();	iter != m_OfferClientClients.end(); ++iter )
	{
		OfferClientCallbackInterface * client = *iter;
		client->callbackOfferSendState( assetOfferId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "OfferClientMgr::announceOfferXferState state %d done\n", assetSendState );
}

//============================================================================
bool OfferClientMgr::fromGuiOfferClientCreated( OfferClientInfo& thumbInfo )
{
    return addOffer( thumbInfo );
}

//============================================================================
bool OfferClientMgr::fromGuiOfferClientUpdated( OfferClientInfo& thumbInfo )
{
    return updateOffer( thumbInfo );
}