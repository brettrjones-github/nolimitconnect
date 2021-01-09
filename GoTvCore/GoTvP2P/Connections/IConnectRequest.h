#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include <vector>

enum EConnectRequestType
{
    eConnectRequestNone,

    eConnectRequesttRelayFind,
    eConnectRequestRelayJoin,

    eConnectRequestGroupAnnounce,
    eConnectRequestGroupFind,
    eConnectRequesttGroupJoin,
 
    eConnectRequestChatRoomAnnounce,
    eConnectRequestChatRoomFind,
    eConnectRequestChatRoomJoin,

    eMaxConnectRequestType
};

class VxSktBase;

class IConnectRequestCallback
{
public:
    /// return true if have use for this connection
    virtual bool                onContactConnected( EConnectRequestType hostConnectType, VxSktBase* sktBase ) = 0;
    virtual void                onContactDisconnected( EConnectRequestType hostConnectType, VxSktBase* sktBased ) = 0;

    // these should only be called by Host Connect Mgr
    bool                        hasHostConnectType( EConnectRequestType hostConnectType );
    void                        addHostConnectType( EConnectRequestType hostConnectType );
    void                        removeConnectType( EConnectRequestType hostConnectType );

    /// a single callback instance may want multiple host services connection
    std::vector<EConnectRequestType> m_ConnectTypeList;
};
