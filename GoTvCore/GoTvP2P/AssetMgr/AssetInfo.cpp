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

#include "AssetInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
AssetInfo::AssetInfo()
: AssetBaseInfo()
{ 
}

//============================================================================
AssetInfo::AssetInfo( const AssetInfo& rhs )
{
	*this = rhs;
}

//============================================================================
AssetInfo::AssetInfo( const std::string& fileName )
: AssetBaseInfo( fileName )
{ 
}

//============================================================================
AssetInfo::AssetInfo( const char * fileName, uint64_t fileLen, uint16_t assetType )
: AssetBaseInfo( fileName, fileLen, assetType )
{
}

//============================================================================
AssetInfo& AssetInfo::operator=( const AssetInfo& rhs ) 
{	
	if( this != &rhs )
	{
        *( (AssetBaseInfo*)this) =  *( (AssetBaseInfo*)&rhs);
	}

	return *this;
}
