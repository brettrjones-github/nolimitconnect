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

class P2PEngine;
class IConnectRequestCallback;
class VxSktBase;

class HandshakeInfo
{
public:
    HandshakeInfo() = default;
    HandshakeInfo(VxSktBase *sktBase, VxGUID& sessionId, VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason, uint64_t timeStamp );
    HandshakeInfo( const HandshakeInfo& rhs );
    virtual ~HandshakeInfo() = default;

    HandshakeInfo&              operator =( HandshakeInfo && rhs ) = default;
    HandshakeInfo&				operator=( const HandshakeInfo& rhs );
    bool                        operator==( const HandshakeInfo& rhs );

    void                        setHanshakeComplete( bool complete ) { m_HanshakeComplete = complete; }

    uint64_t                    getTimeStamp( void ) const { return m_TimeStamp; }
    VxGUID&                     getSessionId( void ) { return m_SessionId; }
    VxSktBase *                 getSktBase( void ) { return m_SktBase; }
    IConnectRequestCallback*    getCallback( void ) { return m_Callback; }
    EConnectReason              getConnectReason( void ) { return m_ConnectReason; }

    void                        onContactConnected( void );
    void                        onHandshakeTimeout( void );
    void                        onContactSessionDone( void );
    void                        onSktDisconnected( void );

protected:
    VxSktBase *                 m_SktBase{ nullptr };
    VxGUID                      m_SessionId;
    VxGUID                      m_OnlineId;
    IConnectRequestCallback*    m_Callback{ nullptr };
    EConnectReason              m_ConnectReason{ eConnectReasonUnknown };
    uint64_t                    m_TimeStamp{ 0 };
    bool                        m_HanshakeComplete{ false };
};



