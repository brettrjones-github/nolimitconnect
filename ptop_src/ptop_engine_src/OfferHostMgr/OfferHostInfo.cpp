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
// http://www.nolimitconnect.com
//============================================================================

#include <config_appcorelibs.h>
#include "OfferHostInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
OfferHostInfo::OfferHostInfo()
    : OfferBaseInfo()
{ 
}

//============================================================================
OfferHostInfo::OfferHostInfo( const OfferHostInfo& rhs )
{
	*this = rhs;
}

//============================================================================
OfferHostInfo::OfferHostInfo( const OfferBaseInfo& rhs )
{
    *((OfferBaseInfo *)this) = rhs;
}

//============================================================================
OfferHostInfo::OfferHostInfo( const std::string& fileName )
    : OfferBaseInfo(fileName)
{ 
}

//============================================================================
OfferHostInfo::OfferHostInfo( const char * fileName, uint64_t fileLen, uint16_t assetType )
: OfferBaseInfo( fileName, fileLen, assetType )
{
}

//============================================================================
OfferHostInfo& OfferHostInfo::operator=( const OfferHostInfo& rhs ) 
{	
	if( this != &rhs )
	{
        *( (OfferBaseInfo*)this ) = rhs;
	}

	return *this;
}
