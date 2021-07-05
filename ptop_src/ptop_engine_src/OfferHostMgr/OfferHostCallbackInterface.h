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

#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseCallbackInterface.h>

class OfferHostInfo;

class OfferHostCallbackInterface : public OfferBaseCallbackInterface
{
public:
    virtual void				callbackOfferHostAdded( OfferHostInfo * thumbInfo ){};
    virtual void				callbackOfferHostUpdated( OfferHostInfo * thumbInfo ){};
    virtual void				callbackOfferHostRemoved( VxGUID& thumbId ){};
};

