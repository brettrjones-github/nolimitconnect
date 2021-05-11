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

#include "BlobInfo.h"
#include "BlobInfoDb.h"
#include "BlobMgr.h"

//============================================================================
BlobInfoDb::BlobInfoDb( AssetBaseMgr& hostListMgr, const char * dbName )
: AssetBaseInfoDb( hostListMgr, dbName )
{
}

//============================================================================
AssetBaseInfo * BlobInfoDb::createAssetInfo( EAssetType assetType, const char * assetName, uint64_t assetLen )
{
    return new BlobInfo( assetType, assetName, assetLen );
}

//============================================================================
AssetBaseInfo * BlobInfoDb::createAssetInfo( AssetBaseInfo& assetInfo )
{
    return new BlobInfo( assetInfo );
}