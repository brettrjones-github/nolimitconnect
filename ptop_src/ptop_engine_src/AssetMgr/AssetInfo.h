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

#include <ptop_src/ptop_engine_src/AssetBase/AssetBaseInfo.h>

class FileInfo;

class AssetInfo : public AssetBaseInfo
{
public:
	AssetInfo();
    AssetInfo( enum EAssetType assetType );
	AssetInfo( const AssetInfo& rhs );
    AssetInfo( const AssetBaseInfo& rhs );
    AssetInfo( FileInfo& rhs );
    AssetInfo( enum EAssetType assetType, std::string fileName );
    AssetInfo( enum EAssetType assetType, std::string fileName, VxGUID& assetId );
    AssetInfo( enum EAssetType assetType, std::string fileName, uint64_t fileLen );
    AssetInfo( enum EAssetType assetType, std::string fileName, uint64_t fileLen, VxGUID& assetId );

	AssetInfo&					operator=( const AssetInfo& rhs ); 

};
