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

#include "AssetMgr.h"
#include "AssetInfo.h"

//============================================================================
AssetMgr::AssetMgr( P2PEngine& engine, const char * dbName, const char * dbStateName )
: AssetBaseMgr( engine, dbName, dbStateName, eAssetMgrTypeAssets )
{
}

//============================================================================
AssetBaseInfo * AssetMgr::createAssetInfo( EAssetType assetType, const char * assetName, uint64_t assetLen )
{
    AssetInfo* assetInfo = new AssetInfo( assetType, assetName, assetLen );
    assetInfo->assureHasCreatorId();
    return assetInfo;
}

//============================================================================
AssetBaseInfo* AssetMgr::createAssetInfo( EAssetType assetType, const char* assetName, uint64_t assetLen, VxGUID& assetId )
{
    AssetInfo* assetInfo = new AssetInfo( assetType, assetName, assetLen, assetId );
    assetInfo->assureHasCreatorId();
    return assetInfo;
}

//============================================================================
AssetBaseInfo * AssetMgr::createAssetInfo( AssetBaseInfo& assetInfo )
{
    AssetInfo* assetInfoNew = new AssetInfo( assetInfo );
    assetInfoNew->assureHasCreatorId();
    return assetInfoNew;
}