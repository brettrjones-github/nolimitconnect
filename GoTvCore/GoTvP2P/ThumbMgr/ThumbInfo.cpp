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
#include "ThumbInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
ThumbInfo::ThumbInfo()
: AssetBaseInfo()
{ 
}

//============================================================================
ThumbInfo::ThumbInfo( const ThumbInfo& rhs )
: AssetBaseInfo( rhs )
{
}

//============================================================================
ThumbInfo::ThumbInfo( const AssetBaseInfo& rhs )
: AssetBaseInfo( rhs )
{
}

//============================================================================
ThumbInfo::ThumbInfo( VxGUID& onlineId, int64_t modifiedTime )
: AssetBaseInfo( onlineId, modifiedTime )
{

}

//============================================================================
ThumbInfo::ThumbInfo( VxGUID& onlineId, VxGUID& assetId, int64_t modifiedTime )
: AssetBaseInfo( onlineId, assetId, modifiedTime )
{

}

//============================================================================
ThumbInfo::ThumbInfo( const std::string& fileName )
    : AssetBaseInfo( fileName )
{ 
}

//============================================================================
ThumbInfo::ThumbInfo( const char * fileName, uint64_t fileLen, uint16_t assetType )
: AssetBaseInfo( fileName, fileLen, assetType )
{
}

//============================================================================
ThumbInfo& ThumbInfo::operator=( const ThumbInfo& rhs ) 
{	
	if( this != &rhs )
	{
        *( (AssetBaseInfo*)this ) = rhs;
	}

	return *this;
}
