//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include <config_gotvcore.h>
#include "BaseInfo.h"

//============================================================================
BaseInfo::BaseInfo()
{ 
}

//============================================================================
BaseInfo::BaseInfo( const BaseInfo& rhs )
    :
{
	*this = rhs;
}

//============================================================================
BaseInfo::BaseInfo( const AssetBaseInfo& rhs )
{
    *((AssetBaseInfo *)this) = rhs;
}

//============================================================================
BaseInfo::BaseInfo( const std::string& fileName )
    : AssetBaseInfo(fileName)
{ 
}

//============================================================================
BaseInfo::BaseInfo( const char * fileName, uint64_t fileLen, uint16_t assetType )
: AssetBaseInfo( fileName, fileLen, assetType )
{
}

//============================================================================
BaseInfo& BaseInfo::operator=( const BaseInfo& rhs ) 
{	
	if( this != &rhs )
	{
        *( (AssetBaseInfo*)this ) = rhs;
	}

	return *this;
}
