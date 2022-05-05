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

class VxGUID;
class ConnectId;

class ConnectIdListCallbackInterface
{
public:
    virtual void				callbackRelayStatusChange( VxGUID& connectId, bool isRelayed ) {};
    virtual void				callbackRelayStatusChange( ConnectId& connectId, bool isRelayed ) {};
    virtual void				callbackOnlineStatusChange( VxGUID& onlineId, bool isOnline ) {};
    virtual void				callbackConnectionStatusChange( ConnectId& connectId, bool isConnected ) {};

    virtual void				callbackConnectionReason( VxGUID& socketId, enum EConnectReason connectReason, bool enableReason ) {};
    virtual void				callbackConnectionLost( VxGUID& socketId ) {};
};

