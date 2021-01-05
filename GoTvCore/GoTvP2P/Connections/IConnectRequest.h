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

enum EHostConnectType
{
    eHostConnectNone,

    eHostConnectRelayFind,
    eHostConnectRelayJoin,

    eHostConnectGroupAnnounce,
    eHostConnectGroupFind,
    eHostConnectGroupJoin,
 
    eHostConnectChatRoomAnnounce,
    eHostConnectChatRoomFind,
    eHostConnectChatRoomJoin,

    eMaxHostConnectNone
};

class VxSktBase;

class IConnectRequestCallback
{
public:
    /// return true if have use for this connection
    virtual bool                onContactConnected( EHostConnectType hostConnectType, VxSktBase* sktBase ) = 0;
    virtual void                onContactDisconnected( EHostConnectType hostConnectType, VxSktBase* sktBase ) = 0;
};

class IHostConnectCallback : public IConnectRequestCallback
{
public:
    /// return true if have use for this connection
    virtual bool                onContactConnected( EHostConnectType hostConnectType, VxSktBase* sktBase ) = 0;
    virtual void                onContactDisconnected( EHostConnectType hostConnectType, VxSktBase* sktBased ) = 0;

    // these should only be called by Host Connect Mgr
    bool                        hasHostConnectType( EHostConnectType hostConnectType );
    void                        addHostConnectType( EHostConnectType hostConnectType );
    void                        removeConnectType( EHostConnectType hostConnectType );

    /// a single callback instance may want multiple host services connection
    std::vector<EHostConnectType> m_ConnectTypeList;
};
