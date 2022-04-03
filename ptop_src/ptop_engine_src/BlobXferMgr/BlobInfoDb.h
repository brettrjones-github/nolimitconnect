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
// http://www.nolimitconnect.com
//============================================================================

#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseInfoDb.h>

class BlobInfo;
class BlobMgr;

class BlobInfoDb : public AssetBaseInfoDb
{
public:
	BlobInfoDb( AssetBaseMgr& mgr, const char * dbName );
	virtual ~BlobInfoDb() = default;

protected:
    virtual AssetBaseInfo *     createAssetInfo( enum EAssetType assetType, const char * fileName, uint64_t fileLen ) override;
    virtual AssetBaseInfo *     createAssetInfo( AssetBaseInfo& assetInfo ) override;

};


