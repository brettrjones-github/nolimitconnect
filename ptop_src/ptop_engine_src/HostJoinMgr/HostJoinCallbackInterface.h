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

class HostJoinInfo;
class VxGUID;

class HostJoinCallbackInterface
{
public:
    virtual void				callbackHostJoinAdded( HostJoinInfo * userHostInfo ){};
    virtual void				callbackHostJoinUpdated( HostJoinInfo * userHostInfo ){};
    virtual void				callbackHostJoinRequested( HostJoinInfo* userHostInfo ) {};
    virtual void				announceHostJoinRequestUpdated( HostJoinInfo* userHostInfo ) {};
    virtual void				callbackHostJoinRemoved( VxGUID& userHostId ){};

    virtual void				callbackHostJoinOfferState( VxGUID& hostOnlineId, EOfferState userHostOfferState ) {};
    virtual void				callbackHostJoinOnlineState( VxGUID& hostOnlineId, EOnlineState onlineState, VxGUID& connectionId ) {};
};

