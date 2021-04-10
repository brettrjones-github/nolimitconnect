//============================================================================
// Copyright (C) 2015 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "ThumbMgr.h"
#include "ThumbInfo.h"
#include "ThumbInfoDb.h"
#include "ThumbCallbackInterface.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

//============================================================================
ThumbMgr::ThumbMgr( P2PEngine& engine, const char * dbName )
: AssetBaseMgr( engine, dbName, eAssetMgrTypeThumb )
, m_ThumbInfoDb( dynamic_cast<ThumbInfoDb&>(AssetBaseMgr::getAssetInfoDb()) )
, m_ThumbInfoList( getAssetBaseInfoList() )
{
}

//============================================================================
AssetBaseInfo * ThumbMgr::createAssetInfo( const char * assetName, uint64_t assetLen, uint16_t assetType )
{
    return new ThumbInfo( assetName, assetLen, assetType );
}

//============================================================================
AssetBaseInfo * ThumbMgr::createAssetInfo( AssetBaseInfo& assetInfo )
{
    return new ThumbInfo( assetInfo );
}

//============================================================================
void ThumbMgr::fromGuiUserLoggedOn( void )
{
    // dont call AssetBaseMgr::fromGuiUserLoggedOn because we never generate sha hash for thumbnails
    if( !m_Initialized )
    {
        m_Initialized = true;
        // user specific directory should be set
        std::string dbFileName = VxGetSettingsDirectory();
        dbFileName += m_ThumbInfoDb.getDatabaseName(); 
        lockResources();
        m_ThumbInfoDb.dbShutdown();
        m_ThumbInfoDb.dbStartup( THUMB_DB_VERSION, dbFileName );

        clearAssetInfoList();
        m_ThumbInfoDb.getAllAssets( m_ThumbInfoList );

        m_ThumbListInitialized = true;
        unlockResources();
    }
}

//============================================================================
void ThumbMgr::addThumbMgrClient( ThumbCallbackInterface * client, bool enable )
{
    lockClientList();
    if( enable )
    {
        m_ThumbClients.push_back( client );
    }
    else
    {
        std::vector<ThumbCallbackInterface *>::iterator iter;
        for( iter = m_ThumbClients.begin(); iter != m_ThumbClients.end(); ++iter )
        {
            if( *iter == client )
            {
                m_ThumbClients.erase( iter );
                break;
            }
        }
    }

    unlockClientList();
}

//============================================================================
void ThumbMgr::announceAssetAdded( AssetBaseInfo * assetInfo )
{
    AssetBaseMgr::announceAssetAdded( assetInfo );
    ThumbInfo * thumbInfo = dynamic_cast<ThumbInfo *>( assetInfo );
    if( thumbInfo )
    {
	    LogMsg( LOG_INFO, "ThumbMgr::announceThumbAdded start" );
	
	    lockClientList();
	    std::vector<ThumbCallbackInterface *>::iterator iter;
	    for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
	    {
		    ThumbCallbackInterface * client = *iter;
		    client->callbackThumbAdded( thumbInfo );
	    }

	    unlockClientList();
	    LogMsg( LOG_INFO, "ThumbMgr::announceThumbAdded done" );
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceAssetAdded dynamic_cast failed" );
    }
}

//============================================================================
void ThumbMgr::announceAssetUpdated( AssetBaseInfo * assetInfo )
{
    AssetBaseMgr::announceAssetUpdated( assetInfo );
    ThumbInfo * thumbInfo = dynamic_cast<ThumbInfo *>( assetInfo );
    if( thumbInfo )
    {
        lockClientList();
        std::vector<ThumbCallbackInterface *>::iterator iter;
        for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
        {
            ThumbCallbackInterface * client = *iter;
            client->callbackThumbUpdated( thumbInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceAssetRemoved dynamic_cast failed" );
    }
}

//============================================================================
void ThumbMgr::announceAssetRemoved( AssetBaseInfo * assetInfo )
{
    AssetBaseMgr::announceAssetRemoved( assetInfo );
    ThumbInfo * thumbInfo = dynamic_cast<ThumbInfo *>( assetInfo );
    if( thumbInfo )
    {
	    lockClientList();
	    std::vector<ThumbCallbackInterface *>::iterator iter;
	    for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
	    {
		    ThumbCallbackInterface * client = *iter;
		    client->callbackThumbRemoved( thumbInfo->getThumbId() );
	    }

	    unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceAssetRemoved dynamic_cast failed" );
    }
}

//============================================================================
void ThumbMgr::announceAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param )
{
    AssetBaseMgr::announceAssetXferState( assetUniqueId, assetSendState, param );

	LogMsg( LOG_INFO, "ThumbMgr::announceAssetXferState state %d start\n", assetSendState );
	lockClientList();
	std::vector<ThumbCallbackInterface *>::iterator iter;
	for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
	{
		ThumbCallbackInterface * client = *iter;
		client->callbackAssetSendState( assetUniqueId, assetSendState, param );
	}

	unlockClientList();
	LogMsg( LOG_INFO, "ThumbMgr::announceAssetXferState state %d done\n", assetSendState );
}

//============================================================================
bool ThumbMgr::fromGuiThumbCreated( ThumbInfo& thumbInfo )
{
    return addAsset( thumbInfo );
}

//============================================================================
bool ThumbMgr::fromGuiThumbUpdated( ThumbInfo& thumbInfo )
{
    return updateAsset( thumbInfo );
}