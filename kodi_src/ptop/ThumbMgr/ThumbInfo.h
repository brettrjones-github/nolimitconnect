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

class ThumbInfo : public AssetBaseInfo
{
public:
	ThumbInfo();
	ThumbInfo( const ThumbInfo& rhs );
    ThumbInfo( const AssetBaseInfo& rhs );
    ThumbInfo( VxGUID& onlineId, int64_t modifiedTime = 0 );
    ThumbInfo( VxGUID& onlineId, VxGUID& assetId, int64_t modifiedTime = 0 );
	ThumbInfo( const std::string& fileName );
    ThumbInfo( const char * fileName, uint64_t fileLen );

	ThumbInfo&				    operator=( const ThumbInfo& rhs ); 

    /// thumb objects use the asset id and ther is no other assciated thumb to this thumb fil
    virtual VxGUID&				getThumbId( void ) override                             { return m_UniqueId; }

public:
	//=== vars ===//

};
