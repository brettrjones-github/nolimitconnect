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

#include <config_gotvcore.h>

#include "AssetInfoDb.h"
#include "AssetMgr.h"
#include "AssetInfo.h"

//============================================================================
AssetInfoDb::AssetInfoDb( AssetBaseMgr& assetInfoMgr, const char * dbName )
: AssetBaseInfoDb( assetInfoMgr, dbName )
{
}

//============================================================================
AssetBaseInfo * AssetInfoDb::createAssetInfo( const char * assetName, uint64_t assetLen, uint16_t assetType )
{
    return new AssetInfo( assetName, assetLen, assetType );
}

//============================================================================
AssetBaseInfo * AssetInfoDb::createAssetInfo( AssetBaseInfo& assetInfo )
{
    return new AssetInfo( assetInfo );
}