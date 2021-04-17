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

#include <GuiInterface/IDefs.h>

class UserHostInfo;
class VxGUID;

class UserHostCallbackInterface
{
public:
    virtual void				callbackUserHostAdded( UserHostInfo * userHostInfo ){};
    virtual void				callbackUserHostUpdated( UserHostInfo * userHostInfo ){};
    virtual void				callbackUserHostRemoved( VxGUID& userHostId ){};
    virtual void				callbackUserHostOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState ) {};
    virtual void				callbackUserHostOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId ) {};
};

