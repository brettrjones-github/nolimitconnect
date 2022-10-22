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

#include "OfferBaseInfo.h"

#include <PktLib/PktBlobEntry.h>

//============================================================================
OfferBaseInfo::OfferBaseInfo( std::string fileName )
	: AssetInfo( VxFileNameToAssetType( fileName ), fileName )
{
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( std::string fileName, uint64_t assetLen, uint16_t assetType )
	: AssetInfo( VxFileNameToAssetType( fileName ), fileName, assetLen )
{
}

//============================================================================
OfferBaseInfo::OfferBaseInfo( FileInfo& fileInfo )
	: AssetInfo( fileInfo )
{
}

//============================================================================
bool OfferBaseInfo::addToBlob( PktBlobEntry& blob )
{
	blob.resetWrite();
	if( AssetInfo::addToBlob( blob ) )
	{
		bool result = blob.setValue( m_OfferMsg );
		result &= blob.getValue( m_UniqueId );
		return result;
	}

	return false;
}

//============================================================================
bool OfferBaseInfo::extractFromBlob( PktBlobEntry& blob )
{
	blob.resetRead();
	if( AssetInfo::extractFromBlob( blob ) )
	{
		bool result = blob.getValue( m_OfferMsg );
		result &= blob.getValue( m_AssetTag );
		return result;
	}

	return false;
}
