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

#include "OfferClientInfoDb.h"
#include "OfferClientMgr.h"
#include "OfferClientInfo.h"

//============================================================================
OfferClientInfoDb::OfferClientInfoDb( OfferBaseMgr& hostListMgr, const char* dbname )
: OfferBaseInfoDb( hostListMgr, dbname )
{
}

//============================================================================
OfferClientInfoDb::~OfferClientInfoDb()
{
}

//============================================================================
OfferBaseInfo* OfferClientInfoDb::createOfferInfo( std::string assetName, uint64_t assetLen, uint16_t assetType )
{
    return new OfferClientInfo( assetName, assetLen, assetType );
}

//============================================================================
OfferBaseInfo* OfferClientInfoDb::createOfferInfo( OfferBaseInfo& assetInfo )
{
    return new OfferClientInfo( assetInfo );
}