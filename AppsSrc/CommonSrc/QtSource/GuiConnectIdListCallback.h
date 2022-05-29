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

#include <PktLib/ConnectId.h>

class GuiGroupie;

class GuiConnectIdListCallback
{
public:
    virtual void				callbackNearbyStatusChange( VxGUID& onlineId, int64_t nearbyTimeOrZeroIfNot ) {};
    virtual void				callbackRelayStatusChange( VxGUID& onlineId, bool isRelayed ) {};
    virtual void				callbackOnlineStatusChange( VxGUID& onlineId, bool isOnline ) {};
    virtual void				callbackConnectionStatusChange( ConnectId& connectId, bool isConnected ) {};
};

