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

#include <config_appcorelibs.h>
#include "BlobInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
BlobInfo::BlobInfo()
: AssetBaseInfo()
{ 
}

//============================================================================
BlobInfo::BlobInfo( const BlobInfo& rhs )
{
	*((AssetBaseInfo *)this) = rhs;
}

//============================================================================
BlobInfo::BlobInfo( const AssetBaseInfo& rhs )
{
    *((AssetBaseInfo *)this) = rhs;
}

//============================================================================
BlobInfo::BlobInfo( EAssetType assetType, const std::string& fileName )
: AssetBaseInfo( assetType, fileName )
{ 
}

//============================================================================
BlobInfo::BlobInfo( EAssetType assetType, const std::string& fileName, VxGUID& assetId )
	: AssetBaseInfo( assetType, fileName, assetId )
{
}

//============================================================================
BlobInfo::BlobInfo( EAssetType assetType, const char* fileName, uint64_t fileLen )
: AssetBaseInfo( assetType, fileName, fileLen )
{
}

//============================================================================
BlobInfo::BlobInfo( EAssetType assetType, const char* fileName, uint64_t fileLen, VxGUID& assetId )
	: AssetBaseInfo( assetType, fileName, fileLen, assetId )
{
}

//============================================================================
BlobInfo& BlobInfo::operator=( const BlobInfo& rhs ) 
{	
	if( this != &rhs )
	{
        *((AssetBaseInfo *)this) = rhs;
	}

	return *this;
}
