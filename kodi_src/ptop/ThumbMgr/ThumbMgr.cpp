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
#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginBase.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <time.h>

//============================================================================
ThumbMgr::ThumbMgr( P2PEngine& engine, const char * dbName, const char * dbStateName )
: AssetBaseMgr( engine, dbName, dbStateName,  eAssetMgrTypeThumb )
, m_ThumbInfoDb( dynamic_cast<ThumbInfoDb&>(AssetBaseMgr::getAssetInfoDb()) )
, m_ThumbInfoList( getAssetBaseInfoList() )
{
}

//============================================================================
AssetBaseInfo * ThumbMgr::createAssetInfo( EAssetType assetType, const char * fileName, uint64_t assetLen )
{
    if( eAssetTypeThumbnail == assetType )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::createAssetInfo creating thumbnail asset of asset type %s", DescribeAssetType( assetType ) );
    }

    return new ThumbInfo( fileName, assetLen );
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
	LogMsg( LOG_INFO, "ThumbMgr::announceAssetXferState state %d done", assetSendState );
}

//============================================================================
void ThumbMgr::announceThumbAdded( ThumbInfo& thumbInfo )
{
    lockClientList();
    std::vector<ThumbCallbackInterface *>::iterator iter;
    for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
    {
        ThumbCallbackInterface * client = *iter;
        client->callbackThumbAdded( &thumbInfo );
    }

    unlockClientList();
}

//============================================================================
void ThumbMgr::announceThumbUpdated( ThumbInfo& thumbInfo )
{
    lockClientList();
    std::vector<ThumbCallbackInterface *>::iterator iter;
    for( iter = m_ThumbClients.begin();	iter != m_ThumbClients.end(); ++iter )
    {
        ThumbCallbackInterface * client = *iter;
        client->callbackThumbUpdated( &thumbInfo );
    }

    unlockClientList();
}

//============================================================================
ThumbInfo* ThumbMgr::lookupThumbInfo( VxGUID& thumbId, int64_t thumbModifiedTime )
{
    m_ThumbInfoMutex.lock();
    for( AssetBaseInfo* thumbInfo : m_ThumbInfoList )
    {
        if( thumbInfo->getThumbId() == thumbId && ( 0 == thumbModifiedTime || thumbModifiedTime <= thumbInfo->getInfoModifiedTime() ) )
        {
            m_ThumbInfoMutex.unlock();
            return dynamic_cast<ThumbInfo*>(thumbInfo);
        }
    }

    m_ThumbInfoMutex.unlock();
    return nullptr;
}

//============================================================================
bool ThumbMgr::fromGuiThumbCreated( ThumbInfo& thumbInfo )
{
    if( AssetBaseMgr::addAsset( thumbInfo ) )
    {
        if( saveToDatabase( thumbInfo ) )
        {
            announceThumbAdded( thumbInfo );
            return true;
        }
    }

    return false;
}

//============================================================================
bool ThumbMgr::fromGuiThumbUpdated( ThumbInfo& thumbInfo )
{
    if( AssetBaseMgr::updateAsset( thumbInfo ) )
    {
        if( saveToDatabase( thumbInfo ) )
        {
            announceThumbUpdated( thumbInfo );
            return true;
        }
    }

    return false;
}

//============================================================================
void ThumbMgr::queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EHostType hostType )
{
    if( eHostTypeUnknown != hostType && netIdent->hasThumbId( hostType ) )
    {
        queryThumbIfNeeded( sktBase, netIdent, hostType, netIdent->getThumbId( hostType ), netIdent->getThumbModifiedTime( hostType ) );
    }   
}

//============================================================================
void ThumbMgr::queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    if( eHostTypeUnknown != hostType && netIdent->hasThumbId( hostType ) )
    {
        queryThumbIfNeeded( sktBase, netIdent, hostType, netIdent->getThumbId( hostType ), netIdent->getThumbModifiedTime( hostType ) );
    }    
}

//============================================================================
void ThumbMgr::queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EHostType hostType, VxGUID &thumbId, int64_t thumbModifiedTime )
{
    if( !lookupThumbInfo( thumbId, thumbModifiedTime ) )
    {
        EPluginType pluginType = HostTypeToHostPlugin( hostType );
        if( pluginType != ePluginTypeInvalid )
        {
            queryThumbIfNeeded( sktBase, netIdent, pluginType, thumbId, thumbModifiedTime );
        }
    }
}

//============================================================================
void ThumbMgr::queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType, VxGUID &thumbId, int64_t thumbModifiedTime )
{
    if( !lookupThumbInfo( thumbId, thumbModifiedTime ) )
    {
        // TODO implement
        vx_assert( false );
    }
}

//============================================================================
bool ThumbMgr::saveToDatabase( ThumbInfo& thumbInfo )
{
    lockResources();

    bool result = m_ThumbInfoDb.saveToDatabase( thumbInfo );

    unlockResources();
    return result;
}

//============================================================================
void ThumbMgr::onPktThumbGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
bool ThumbMgr::requestPluginThumb( VxNetIdent* netIdent, EPluginType pluginType, VxGUID& thumbId )
{

    if( !netIdent || ePluginTypeInvalid == pluginType )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb invalid param " );
        vx_assert( false );
        return false;
    }

    if( IsHostPluginType( pluginType ) )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb You must request thumb using Client plugin instead of Host plugin %s ", DescribePluginType( pluginType ) );
        vx_assert( false );
        return false;
    }

    PluginBase* plugin = m_Engine.getPluginMgr().getPlugin( pluginType );
    if( plugin )
    {
        return plugin->getThumbXferMgr().requestPluginThumb( netIdent, thumbId );
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb invalid plugin " );
        vx_assert( false );
        return false;
    }

    return false;
}