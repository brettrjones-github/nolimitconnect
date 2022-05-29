#pragma once
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

#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseInfoDb.h>

class OfferHostMgr;

class OfferHostInfoDb : public OfferBaseInfoDb
{
public:
	OfferHostInfoDb( OfferBaseMgr& mgr, const char * dbname );
	virtual ~OfferHostInfoDb();

protected:
    virtual OfferBaseInfo *     createOfferInfo( const char * fileName, uint64_t fileLen, uint16_t fileType ) override;
    virtual OfferBaseInfo *     createOfferInfo( OfferBaseInfo& assetInfo ) override;
};

