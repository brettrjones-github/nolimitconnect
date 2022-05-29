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

#include <GuiInterface/IDefs.h>

class User;
class VxGUID;
class BaseSessionInfo;

class UserOnlineCallbackInterface
{
public:
    virtual void				callbackUserOnlineState( User* user, bool online ){};
    virtual void				callbackUserOffline( VxGUID& onlineId ){};

    virtual void				callbackUserOnlineAdded( User* user ){};
    virtual void				callbackUserOnlineUpdated( User * user ){};
    virtual void				callbackUserOnlineRemoved( VxGUID& user ){};

    virtual void				callbackUserSessionAdded( User* user, BaseSessionInfo& sessionInfo ){};
    virtual void				callbackUserSessionUpdated( User * user, BaseSessionInfo& sessionInfo ){};
    virtual void				callbackUserSessionRemoved( VxGUID& user, BaseSessionInfo& sessionInfo ){};
};

