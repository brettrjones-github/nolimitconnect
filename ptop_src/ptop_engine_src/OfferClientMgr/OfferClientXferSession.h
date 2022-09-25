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

#include <config_appcorelibs.h>

#include "OfferClientInfo.h"
#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseXferSession.h>

#include <NetLib/VxFileXferInfo.h>

#include <vector>

class VxSktBase;
class VxNetIdent;
class P2PEngine;

class OfferClientXferSession : public OfferBaseXferSession
{
public:
	OfferClientXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr );
	OfferClientXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase* sktBase, VxNetIdent* netIdent );
	OfferClientXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent );
	virtual ~OfferClientXferSession() = default;

};
