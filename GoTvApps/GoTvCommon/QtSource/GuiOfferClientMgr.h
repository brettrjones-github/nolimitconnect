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

#include <ptop_src/ptop_engine_src/OfferClientMgr/OfferClientCallbackInterface.h>
#include "GuiOfferMgrBase.h"

class GuiOfferClientMgr : public GuiOfferMgrBase, public OfferClientCallbackInterface
{
    Q_OBJECT
public:
    GuiOfferClientMgr() = delete;
    GuiOfferClientMgr( AppCommon& myApp )
        : GuiOfferMgrBase( myApp )
    {
    }

    virtual ~GuiOfferClientMgr() override = default;

    virtual void                onAppCommonCreated( void ) override;
    virtual void                onMessengerReady( bool ready ) override { }
    virtual bool                isMessengerReady( void ) override;
    virtual void                onSystemReady( bool ready ) { }
};
