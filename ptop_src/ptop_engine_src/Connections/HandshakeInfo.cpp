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
// http://www.nolimitconnect.org
//============================================================================

#include "HandshakeInfo.h"

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktBase.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

#include <algorithm>

//============================================================================
HandshakeInfo::HandshakeInfo(VxSktBase *sktBase, VxGUID& sessionId, VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason, uint64_t timeStamp)
    : m_SktBase(sktBase)
    , m_SessionId(sessionId)
    , m_OnlineId(onlineId)
    , m_Callback(callback)
    , m_ConnectReason(connectReason)
    , m_TimeStamp(timeStamp)
{
}

//============================================================================
HandshakeInfo::HandshakeInfo( const HandshakeInfo& rhs )
    : m_SktBase( rhs.m_SktBase )
    , m_SessionId( rhs.m_SessionId )
    , m_OnlineId( rhs.m_OnlineId )
    , m_Callback( rhs.m_Callback )
    , m_ConnectReason( rhs.m_ConnectReason )
    , m_TimeStamp( rhs.m_TimeStamp )
    , m_HanshakeComplete( rhs.m_HanshakeComplete )
{
}

//============================================================================
HandshakeInfo& HandshakeInfo::operator=( const HandshakeInfo& rhs )
{
    if( this != &rhs )
    {
        m_SktBase = rhs.m_SktBase;
        m_SessionId = rhs.m_SessionId;
        m_OnlineId = rhs.m_OnlineId;
        m_Callback = rhs.m_Callback;
        m_ConnectReason = rhs.m_ConnectReason;
        m_TimeStamp = rhs.m_TimeStamp;
        m_HanshakeComplete = rhs.m_HanshakeComplete;
    }

    return *this;
}

//============================================================================
bool HandshakeInfo::operator==( const HandshakeInfo& rhs )
{
    return  m_SessionId == rhs.m_SessionId;
}

//============================================================================
void HandshakeInfo::onContactConnected( void )
{
    if( m_Callback )
    {
        m_Callback->onContactConnected( m_SessionId, m_SktBase, m_OnlineId, m_ConnectReason );
    }
}

//============================================================================
void HandshakeInfo::onHandshakeTimeout( void )
{
    if( m_Callback )
    {
        m_Callback->onHandshakeTimeout( m_SessionId, m_SktBase, m_OnlineId, m_ConnectReason );
    }
}

//============================================================================
void HandshakeInfo::onContactSessionDone()
{
    if( m_Callback )
    {
        m_Callback->onContactSessionDone( m_SessionId, m_SktBase, m_OnlineId, m_ConnectReason );
    }
}

//============================================================================
void HandshakeInfo::onSktDisconnected( void )
{
    if( m_Callback )
    {
        m_Callback->onContactDisconnected( m_SessionId, m_SktBase, m_OnlineId, m_ConnectReason );
    }
}
