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

#include "ConnectedInfo.h"
#include "HandshakeInfo.h"

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktBase.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxTime.h>

#include <algorithm>

namespace
{
    const uint64_t HOST_ACTION_TIMEOUT_MS = 30000;
    const uint64_t HOST_CONNECT_ATTEMPT_TIMEOUT_MS = 60000;
}

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
VxSktBase * ConnectedInfo::getSktBase( void )
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
void ConnectedInfo::removeConnectReason( VxGUID& sessionId, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    std::vector<HandshakeInfo>  completedList;
    for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.end(); ++iter )
    {
        if( iter->getSessionId() == sessionId && iter->getCallback() == callback && iter->getConnectReason() == connectReason )
        {
            iter->onContactSessionDone();
            completedList.push_back( *iter );
            iter = m_CallbackList.erase(iter);
            break;
        }
    }

    if( m_CallbackList.empty() && !completedList.empty())
    {
        HandshakeInfo& shakeInfo = completedList[0];
        if( shakeInfo.getSktBase() && shakeInfo.getSktBase()->isConnected() )
        {
            // let the normal socket disconnected code do the work of removing the connection
            shakeInfo.getSktBase()->closeSkt( 9191 );
        }
    }
}

//============================================================================
void ConnectedInfo::onSktDisconnected( VxSktBase * sktBase )
{
    for( auto iter = m_CallbackList.begin(); iter != m_CallbackList.end(); ++iter )
    {
        if( iter->getSktBase() == sktBase )
        {
            iter->onSktDisconnected();
            iter = m_CallbackList.erase( iter );
        }
    }
}

//============================================================================
void ConnectedInfo::aboutToDelete( void )
{
    // TODO callback those using the connection to say it is now unavailable
}