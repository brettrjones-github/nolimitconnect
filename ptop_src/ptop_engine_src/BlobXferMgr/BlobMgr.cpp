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

#include "BlobMgr.h"
#include "BlobInfo.h"
#include "BlobInfoDb.h"

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
BlobMgr::BlobMgr( P2PEngine& engine, const char * dbName, const char * stateDbName )
: AssetBaseMgr( engine, dbName, stateDbName, eAssetMgrTypeBlob )
{
}

//============================================================================
AssetBaseInfo * BlobMgr::createAssetInfo( EAssetType assetType, const char * assetName, uint64_t assetLen )
{
    BlobInfo* assetInfo = new BlobInfo( assetType, assetName, assetLen );
    assetInfo->assureHasCreatorId();
    return assetInfo;
}

//============================================================================
AssetBaseInfo* BlobMgr::createAssetInfo( EAssetType assetType, const char* assetName, uint64_t assetLen, VxGUID& assetId )
{
    BlobInfo* assetInfo = new BlobInfo( assetType, assetName, assetLen, assetId );
    assetInfo->assureHasCreatorId();
    return assetInfo;
}

//============================================================================
AssetBaseInfo * BlobMgr::createAssetInfo( AssetBaseInfo& assetInfo )
{
    BlobInfo* assetInfoNew = new BlobInfo( assetInfo );
    assetInfoNew->assureHasCreatorId();
    return assetInfoNew;
}