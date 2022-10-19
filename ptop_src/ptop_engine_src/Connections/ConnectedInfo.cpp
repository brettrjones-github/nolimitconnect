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

#include "ConnectedInfo.h"
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
ConnectedInfo::ConnectedInfo( P2PEngine& engine, BigListInfo* bigListInfo )
    : m_Engine( engine )
    , m_BigListInfo( bigListInfo )
{
    if( bigListInfo )
    {
        m_PeerOnlineId = bigListInfo->getMyOnlineId();
    }
}

//============================================================================
ConnectedInfo::ConnectedInfo( const ConnectedInfo& rhs )
    : m_Engine( rhs.m_Engine )
    , m_BigListInfo( rhs.m_BigListInfo )
    , m_PeerOnlineId( rhs.m_PeerOnlineId )
{
    m_CallbackList = rhs.m_CallbackList;
}

//============================================================================
ConnectedInfo& ConnectedInfo::operator=( const ConnectedInfo& rhs )
{
    if( this != &rhs )
    {
        m_BigListInfo = rhs.m_BigListInfo;
        m_PeerOnlineId = rhs.m_PeerOnlineId;
        m_CallbackList = rhs.m_CallbackList;
    }

    return *this;
}

//============================================================================
bool ConnectedInfo::operator==( const ConnectedInfo& rhs )
{
    return  m_PeerOnlineId == rhs.m_PeerOnlineId;
}

//============================================================================
VxSktBase* ConnectedInfo::getSktBase( void )
{
    if( m_CallbackList.size() )
    {
        return m_CallbackList[0].getSktBase();
    }

    return nullptr;
}

//============================================================================
void ConnectedInfo::addConnectReason( HandshakeInfo& shakeInfo )
{
    m_CallbackList.push_back( shakeInfo );
}

//============================================================================
void ConnectedInfo::onHandshakeComplete( HandshakeInfo& shakeInfo )
{
    addConnectReason( shakeInfo );
}

//============================================================================
bool ConnectedInfo::removeConnectReason( VxGUID& sessionId, IConnectRequestCallback* callback, EConnectReason connectReason, VxSktBase** retSktBaseIfDisconnected )
{
    bool sktDisconnected{ false };
    std::vector<HandshakeInfo>  completedList;
    for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.end();  )
    {
        if( iter->getSessionId() == sessionId && iter->getCallback() == callback && iter->getConnectReason() == connectReason )
        {
            iter->onContactSessionDone();
            completedList.push_back( *iter );
            iter = m_CallbackList.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }

    if( m_CallbackList.empty() && !completedList.empty())
    {
        HandshakeInfo& shakeInfo = completedList[0];
        if( shakeInfo.getSktBase() && shakeInfo.getSktBase()->isConnected() )
        {
            // return skt so caller can close it if disconnected to avoid mutex deadlock
            sktDisconnected = true;
            *retSktBaseIfDisconnected = shakeInfo.getSktBase();
        }
    }

    return sktDisconnected;
}

//============================================================================
void ConnectedInfo::onSktDisconnected( VxSktBase* sktBase )
{
    // this connected info is about to be erased in ConnectedListAll::onSktDisconnected
}

//============================================================================
void ConnectedInfo::aboutToDelete( void )
{
    // TODO callback those using the connection to say it is now unavailable
}
