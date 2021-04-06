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

#include <GoTvCore/GoTvP2P/AssetBase/AssetBaseInfo.h>

class BlobInfo : public AssetBaseInfo
{
public:
	BlobInfo();
	BlobInfo( const BlobInfo& rhs );
	BlobInfo( const std::string& fileName );
    BlobInfo( const char * fileName, uint64_t fileLen, uint16_t assetType );

	BlobInfo&				    operator=( const BlobInfo& rhs ); 
};
