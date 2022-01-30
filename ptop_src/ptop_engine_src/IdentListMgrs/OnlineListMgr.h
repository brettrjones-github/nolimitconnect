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

#include "OnlineSktList.h"

#include <vector>

class OnlineListCallbackInterface;

class OnlineListMgr : public OnlineSktList
{
public:
    OnlineListMgr() = delete;
    OnlineListMgr( P2PEngine& engine );
    virtual ~OnlineListMgr() = default;

    virtual void                onUserOnlineStatusChange( VxGUID& onlineId, bool isOnline ) override;
    virtual void                onUserOnlineStatusChange( GroupieId& groupieId, bool isOnline ) override;

    void                        wantOnlineCallback( OnlineListCallbackInterface* client, bool enable );

    void                        announceOnlineStatus( VxGUID& onlineId, bool isOnline );

    void						lockClientList( void ) { m_ClientCallbackMutex.lock(); }
    void						unlockClientList( void ) { m_ClientCallbackMutex.unlock(); }

    std::vector<OnlineListCallbackInterface*> m_CallbackClients;
    VxMutex						m_ClientCallbackMutex;
};

