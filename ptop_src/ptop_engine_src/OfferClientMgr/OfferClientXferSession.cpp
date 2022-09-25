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
#include "OfferClientXferSession.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <GuiInterface/IToGui.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
OfferClientXferSession::OfferClientXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr )
: OfferBaseXferSession( engine, offerMgr )
{

}

//============================================================================
OfferClientXferSession::OfferClientXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxSktBase* sktBase, VxNetIdent* netIdent )
: OfferBaseXferSession( engine, offerMgr, sktBase, netIdent )
{

}

//============================================================================
OfferClientXferSession::OfferClientXferSession( P2PEngine& engine, OfferBaseMgr& offerMgr, VxGUID& lclSessionId, VxSktBase* sktBase, VxNetIdent* netIdent )
: OfferBaseXferSession( engine, offerMgr, lclSessionId, sktBase, netIdent )
{

}
