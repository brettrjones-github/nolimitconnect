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
// http://www.nolimitconnect.org
//============================================================================

#include "ThumbMgr.h"
#include "ThumbInfo.h"
#include "ThumbInfoDb.h"
#include "ThumbCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <GuiInterface/IToGui.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsFileList.h>

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTime.h>

#include <algorithm>
#include <time.h>

//============================================================================
ThumbMgr::ThumbMgr( P2PEngine& engine, const char* dbName, const char* dbStateName )
: AssetBaseMgr( engine, dbName, dbStateName,  eAssetMgrTypeThumb )
, m_ThumbInfoDb( dynamic_cast<ThumbInfoDb&>(AssetBaseMgr::getAssetInfoDb()) )
, m_ThumbInfoList( getAssetBaseInfoList() )
{
}

//============================================================================
AssetBaseInfo* ThumbMgr::createAssetInfo( EAssetType assetType, const char* fileName, uint64_t assetLen )
{
    if( eAssetTypeThumbnail == assetType )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::createAssetInfo creating thumbnail asset of asset type %s", DescribeAssetType( assetType ) );
    }

    ThumbInfo* assetInfo = new ThumbInfo( fileName, assetLen );
    assetInfo->assureHasCreatorId();
    return assetInfo;
}

//============================================================================
AssetBaseInfo* ThumbMgr::createAssetInfo( EAssetType assetType, const char* fileName, uint64_t assetLen, VxGUID& assetId )
{
    if( eAssetTypeThumbnail == assetType )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::createAssetInfo creating thumbnail asset of asset type %s", DescribeAssetType( assetType ) );
    }

    ThumbInfo* assetInfo = new ThumbInfo( fileName, assetLen, assetId );
    assetInfo->assureHasCreatorId();
    return assetInfo;
}

//============================================================================
AssetBaseInfo* ThumbMgr::createAssetInfo( AssetBaseInfo& assetInfo )
{
    ThumbInfo* assetInfoNew = new ThumbInfo( assetInfo );
    assetInfoNew->assureHasCreatorId();
    return assetInfoNew;
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
bool ThumbMgr::addAsset( AssetBaseInfo& assetInfo, AssetBaseInfo*& retCreatedAsset )
{
    bool result = AssetBaseMgr::addAsset( assetInfo, retCreatedAsset );
    if( result )
    {
        announceAssetAdded( retCreatedAsset );
    }

    return result;
}

//============================================================================
void ThumbMgr::announceAssetAdded( AssetBaseInfo* assetInfo )
{
    if( !assetInfo || !assetInfo->isValidThumbnail() )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceThumbAdded invalid thumbnil" );
        vx_assert( false );
    }

    AssetBaseMgr::announceAssetAdded( assetInfo );
    ThumbInfo* thumbInfo = dynamic_cast<ThumbInfo*>( assetInfo );
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
void ThumbMgr::announceAssetUpdated( AssetBaseInfo* assetInfo )
{
    if( !assetInfo || !assetInfo->isValidThumbnail() )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceAssetUpdated invalid thumbnil" );
        vx_assert( false );
    }

    AssetBaseMgr::announceAssetUpdated( assetInfo );
    ThumbInfo* thumbInfo = dynamic_cast<ThumbInfo*>( assetInfo );
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
void ThumbMgr::announceAssetRemoved( AssetBaseInfo* assetInfo )
{
    AssetBaseMgr::announceAssetRemoved( assetInfo );
    ThumbInfo* thumbInfo = dynamic_cast<ThumbInfo*>( assetInfo );
    if( thumbInfo && thumbInfo->isThumbAsset() )
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

	LogMsg( LOG_INFO, "ThumbMgr::announceAssetXferState state %d start", assetSendState );
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
    if( !thumbInfo.isValidThumbnail() )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::announceThumbUpdated invalid thumbnil" );
        vx_assert( false );
    }

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
        if( thumbInfo->getThumbId() == thumbId && ( isEmoticonThumbnail( thumbId ) || 0 == thumbModifiedTime || thumbModifiedTime <= thumbInfo->getInfoModifiedTime() ) )
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
    AssetBaseInfo* existingAsset = findAsset( thumbInfo.getAssetUniqueId() );
    if( existingAsset )
    {
        *existingAsset = thumbInfo;
        updateDatabase( existingAsset );
        announceAssetUpdated( existingAsset );
        return true;
    }

    // thumbInfo will be destroyed.. only use the object created by addAsset
    AssetBaseInfo* createdThumbInfo = nullptr;
    if( AssetBaseMgr::addAsset( thumbInfo, createdThumbInfo ) && createdThumbInfo )
    {
        ThumbInfo* newThumbInfo = dynamic_cast<ThumbInfo*>(createdThumbInfo);
        if( newThumbInfo )
        {
            if( saveToDatabase( *newThumbInfo ) )
            {
                announceThumbAdded( *newThumbInfo );
                return true;
            }
            else
            {
                LogMsg( LOG_ERROR, "ThumbMgr::fromGuiThumbCreated failed save to db" );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "ThumbMgr::fromGuiThumbCreated failed cast to ThumbInfo" );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::fromGuiThumbCreated failed add asset" );
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

//===========================================================================
std::string ThumbMgr::fromGuiGetThumbFile( VxGUID& thumbId )
{
    std::string fileName;
    m_ThumbInfoMutex.lock();
    for( AssetBaseInfo* thumbInfo : m_ThumbInfoList )
    {
        if( thumbInfo->getThumbId() == thumbId )
        {
            fileName = thumbInfo->getAssetName();
            break;
        }
    }

    m_ThumbInfoMutex.unlock();
    return fileName;
}

//============================================================================
void ThumbMgr::queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EHostType hostType )
{
    if( eHostTypeUnknown != hostType && netIdent->hasThumbId( hostType ) )
    {
        queryThumbIfNeeded( sktBase, netIdent, hostType, netIdent->getThumbId( hostType ), netIdent->getHostOrThumbModifiedTime( hostType ) );
    }   
}

//============================================================================
void ThumbMgr::queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType )
{
    EHostType hostType = PluginTypeToHostType( pluginType );
    if( eHostTypeUnknown != hostType && netIdent->hasThumbId( hostType ) )
    {
        queryThumbIfNeeded( sktBase, netIdent, hostType, netIdent->getThumbId( hostType ), netIdent->getHostOrThumbModifiedTime( hostType ) );
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
        ptopEngineRequestPluginThumb( sktBase, netIdent, pluginType, thumbId );
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
void ThumbMgr::onPktThumbGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void ThumbMgr::onPktThumbXferErr( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
bool ThumbMgr::fromGuiRequestPluginThumb( VxNetIdent* netIdent, EPluginType pluginType, VxGUID& thumbId )
{
    if( !netIdent || ePluginTypeInvalid == pluginType || !thumbId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb invalid param " );
        vx_assert( false );
        return false;
    }

    if( isEmoticonThumbnail( thumbId ) )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb emoticon thumbnails must be generated by gui " );
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
        return plugin->fromGuiRequestPluginThumb( netIdent, thumbId );
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb invalid plugin " );
        vx_assert( false );
        return false;
    }
}

//============================================================================
bool ThumbMgr::ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType, VxGUID& thumbId )
{
    if( !netIdent || ePluginTypeInvalid == pluginType || !thumbId.isVxGUIDValid() )
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
        return plugin->ptopEngineRequestPluginThumb( sktBase, netIdent, thumbId );
    }
    else
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestPluginThumb invalid plugin " );
        vx_assert( false );
        return false;
    }

    return false;
}

//============================================================================
bool ThumbMgr::requestThumbs( VxSktBase* sktBase, VxNetIdent* netIdent )
{
    if( !sktBase || !netIdent || netIdent->isIgnored() )
    {
        LogMsg( LOG_ERROR, "ThumbMgr::requestThumbs invalid param " );
        return false;
    }

    for( int i = eHostTypeNetwork; i < eMaxHostType; i++ )
    {
        EHostType hostType = ( EHostType )i;
        if( netIdent->hasThumbId( hostType ) )
        {
            VxGUID thumbId = netIdent->getHostThumbId( hostType, false );
            int64_t thumbTimestamp = netIdent->getHostOrThumbModifiedTime( hostType );
            if( thumbId.isVxGUIDValid() && thumbTimestamp && !isThumbUpToDate( thumbId, thumbTimestamp ) )
            {
                EPluginType pluginType = HostTypeToClientPlugin( hostType );
                ptopEngineRequestPluginThumb( sktBase, netIdent, pluginType, thumbId );
            }
        }
    }
   
    return true;
}

//============================================================================
bool ThumbMgr::isThumbUpToDate( VxGUID& thumbId, int64_t thumbModifiedTime )
{
    bool isEmoteThumb = isEmoticonThumbnail( thumbId );

    m_ThumbInfoMutex.lock();
    for( AssetBaseInfo* thumbInfo : m_ThumbInfoList )
    {
        if( thumbInfo->getThumbId() == thumbId && ( isEmoteThumb || thumbModifiedTime <= thumbInfo->getInfoModifiedTime() ) &&
               ( isEmoteThumb || thumbInfo->isValidFile() ) )
        {
            m_ThumbInfoMutex.unlock();
            return true;
        }
    }

    m_ThumbInfoMutex.unlock();
    return false;
}

//============================================================================
uint64_t ThumbMgr::fromGuiClearCache( ECacheType cacheType )
{
    uint64_t cacheDeletedAmt{ 0 };
    if( eCacheTypeThumbnail == cacheType )
    {
        // delete every thumbnail not in use by my identity or plugin
        std::vector<VxGUID> inUseList;
        for( int i = eHostTypeNetwork; i < eMaxHostType; i++ )
        {
            VxGUID thumbId = m_Engine.getMyPktAnnounce().getHostThumbId( ( EHostType )i, false );
            if( thumbId.isVxGUIDValid() )
            {
                inUseList.push_back( thumbId );
            }
        }

        AssetBaseInfoDb& assetDb = getAssetInfoDb();
        m_ThumbInfoMutex.lock();
        for( auto iter = m_ThumbInfoList.begin(); iter != m_ThumbInfoList.end(); )
        {
            AssetBaseInfo* assetInfo = ( *iter );
            if( inUseList.end() == std::find( inUseList.begin(), inUseList.end(), assetInfo->getAssetUniqueId() ) )
            {
                assetDb.removeAsset( assetInfo );
                cacheDeletedAmt += VxFileUtil::fileExists( assetInfo->getAssetName().c_str() );
                VxFileUtil::deleteFile( assetInfo->getAssetName().c_str() );
                iter = m_ThumbInfoList.erase( iter );
            }
            else
            {
                ++iter;
            }
        }

        m_ThumbInfoMutex.unlock();

        // TODO clean up any not in database but the file exists?
    }

    return cacheDeletedAmt;
}
