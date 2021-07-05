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
// http://www.nolimitconnect.com
//============================================================================

#include <config_gotvcore.h>

#include "OfferClientXferSession.h"

class OfferClientTxSession : public OfferClientXferSession
{
public:
	OfferClientTxSession( P2PEngine& engine, OfferBaseMgr& offerMgr );
	OfferClientTxSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase * sktBase, VxNetIdent * netIdent );
	OfferClientTxSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase * sktBase, VxNetIdent * netIdent );
};
