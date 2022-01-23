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
    virtual void				callbackHostJoinRequested( HostJoinInfo* hostJoinInfo ) {};
    virtual void				callbackHostJoinUpdated( HostJoinInfo* hostJoinInfo ){};
    virtual void				callbackHostJoinRemoved( GroupieId& groupieId ){};

    virtual void				callbackHostJoinOfferState( GroupieId& groupieId, EJoinState userHostOfferState ) {};
    virtual void				callbackHostJoinOnlineState( GroupieId& groupieId, EOnlineState onlineState, VxGUID& connectionId ) {};
};

