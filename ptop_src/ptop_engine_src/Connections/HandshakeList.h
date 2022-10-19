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
#pragma once

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxGUID.h>
#include <CoreLib/VxMutex.h>

#include <map>

class HandshakeInfo;
class VxSktBase;
class IConnectRequestCallback;

class HandshakeList
{
public:
    HandshakeList() = default;
    virtual ~HandshakeList() = default;

    void                        addHandshake(VxSktBase*sktBase, VxGUID& sessionId, VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason);
    void                        getAndRemoveHandshakeInfo( const VxGUID& onlineId, std::vector<HandshakeInfo>& shakeList, std::vector<HandshakeInfo>& timedOutList );
    void                        removeHandshakeInfo( const VxGUID& onlineId );
    void                        onSktDisconnected( VxSktBase* sktBase );

protected:
    void                        getAndRemoveHandshakeInfo( VxSktBase* sktBase, std::vector<HandshakeInfo>& disconnectedList );

    //=== vars ===//
    std::map<VxGUID, HandshakeInfo> m_ShakeList;
};
