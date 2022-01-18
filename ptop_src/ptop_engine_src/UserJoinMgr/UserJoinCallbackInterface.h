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

class UserJoinInfo;
class VxGUID;

class UserJoinCallbackInterface
{
public:
    virtual void				callbackUserJoinAdded( UserJoinInfo* hostJoinInfo ) {};
    virtual void				callbackUserJoinUpdated( UserJoinInfo* hostJoinInfo ) {};
    virtual void				callbackUserJoinRemoved( VxGUID& userOnlineId, EPluginType pluginType ) {};

    virtual void				callbackUserJoinOfferState( VxGUID& userOnlineId, EPluginType pluginType, EJoinState userOfferState ) {};
    virtual void				callbackUserJoinOnlineState( VxGUID& userOnlineId, EPluginType pluginType, EOnlineState onlineState, VxGUID& connectionId ) {};
};

