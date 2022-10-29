#pragma once
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

#include "ThumbInfoDb.h"

#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseMgr.h>
#include <GuiInterface/IDefs.h>

#include <CoreLib/VxMutex.h>

class PktFileListReply;

class ThumbInfo;
class ThumbHistoryMgr;
class ThumbCallbackInterface;
class VxNetIdent;
class VxPktHdr;
class VxSktBase;
class BigListInfo;

class ThumbMgr : public AssetBaseMgr
{
    const int THUMB_DB_VERSION = 1;
public:
	ThumbMgr( P2PEngine& engine, const char* dbName, const char* dbStateName );
	virtual ~ThumbMgr() = default;

    void                        addThumbMgrClient( ThumbCallbackInterface * client, bool enable );

    void                        fromGuiUserLoggedOn( void ) override;
    bool				        fromGuiThumbCreated( ThumbInfo& thumbInfo );
    bool				        fromGuiThumbUpdated( ThumbInfo& thumbInfo );
    virtual bool			    fromGuiRequestPluginThumb( VxNetIdent* netIdent, EPluginType pluginType, VxGUID& thumbId );
    std::string				    fromGuiGetThumbFile( VxGUID& thumbId );
    virtual uint64_t			fromGuiClearCache( ECacheType cacheType );

    virtual void				announceAssetAdded( AssetBaseInfo* assetInfo ) override;
    virtual void				announceAssetUpdated( AssetBaseInfo* assetInfo ) override;
    virtual void				announceAssetRemoved( AssetBaseInfo* assetInfo ) override;
    virtual void				announceAssetXferState( VxGUID& assetUniqueId, EAssetSendState assetSendState, int param ) override;

    virtual void				queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EHostType hostType );
    virtual void				queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType );
    virtual void				queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EHostType hostType, VxGUID& thumbId, int64_t thumbModifiedTime );
    virtual void				queryThumbIfNeeded( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType, VxGUID& thumbId, int64_t thumbModifiedTime );

    // packet handlers
    virtual void				onPktThumbGetReq            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbGetReply          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbChunkReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbChunkReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbGetCompleteReq    ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbGetCompleteReply  ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendCompleteReq   ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendCompleteReply ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbXferErr           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual bool                ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, EPluginType pluginType, VxGUID& thumbId );
    virtual bool                requestThumbs( VxSktBase* sktBase, VxNetIdent* netIdent );

protected:
    virtual bool				addAsset( AssetBaseInfo& assetInfo, AssetBaseInfo*& retCreatedAsset ) override;

    virtual AssetBaseInfo*      createAssetInfo( EAssetType assetType, const char* fileName, uint64_t fileLen ) override;
    virtual AssetBaseInfo*      createAssetInfo( EAssetType assetType, const char* fileName, uint64_t fileLen, VxGUID& assetId ) override;
    virtual AssetBaseInfo*      createAssetInfo( AssetBaseInfo& assetInfo ) override;
    ThumbInfo*                  lookupThumbInfo( VxGUID& thumbId, int64_t thumbModifiedTime = 0 );
    void				        announceThumbAdded( ThumbInfo& thumbInfo );
    void				        announceThumbUpdated( ThumbInfo& thumbInfo );
    bool                        saveToDatabase( ThumbInfo& thumbInfo );
    bool                        isThumbUpToDate( VxGUID& thumbId, int64_t thumbModifiedTime );

    ThumbInfoDb&                m_ThumbInfoDb;
    std::vector<AssetBaseInfo*>&	m_ThumbInfoList;
    VxMutex						m_ThumbInfoMutex;
    bool                        m_ThumbListInitialized{ false };

    std::vector<ThumbCallbackInterface *> m_ThumbClients;
    VxMutex						m_ThumbClientMutex;
};

