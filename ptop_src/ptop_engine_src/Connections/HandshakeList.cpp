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

#include "HandshakeList.h"
#include "HandshakeInfo.h"

#include <CoreLib/VxGUIDList.h>

namespace
{
    const int HANDSHAKE_TIMEOUT_MS = ( 5 * 60 * 1000 );
}

//============================================================================
void HandshakeList::getAndRemoveHandshakeInfo( const VxGUID& onlineId, std::vector<HandshakeInfo>& shakeList, std::vector<HandshakeInfo>& timedOutList )
{
    VxGUIDList removeList;
    uint64_t timeNow = GetTimeStampMs();
    for( auto iter = m_ShakeList.begin(); iter != m_ShakeList.end(); ++iter )
    {
        HandshakeInfo& shakeInfo = iter->second;
        if( timeNow - shakeInfo.getTimeStamp() > HANDSHAKE_TIMEOUT_MS )
        {
            timedOutList.push_back( shakeInfo );
        }
        else
        {
            shakeList.push_back( shakeInfo );
        }

        removeList.addGuid( shakeInfo.getSessionId() );
    }

    std::vector<VxGUID>& sessionList = removeList.getGuidList();
    for( VxGUID& sesId : sessionList )
    {
        removeHandshakeInfo( sesId );
    }
}

//============================================================================
void HandshakeList::removeHandshakeInfo( const VxGUID& sessionId )
{
    auto iter = m_ShakeList.find( sessionId );
    if( iter != m_ShakeList.end() )
    {
        m_ShakeList.erase( iter );
    }
}

//============================================================================
void HandshakeList::addHandshake( VxSktBase *sktBase, VxGUID& sessionId, VxGUID onlineId, IConnectRequestCallback* callback, EConnectReason connectReason )
{
    uint64_t timeNow = GetTimeStampMs();
    HandshakeInfo shakeInfo( sktBase, sessionId, onlineId, callback, connectReason, timeNow );
    m_ShakeList[sessionId] = shakeInfo;
}

//============================================================================
void HandshakeList::onSktDisconnected( VxSktBase* sktBase )
{
    std::vector<HandshakeInfo> disconnectedList;
    for( HandshakeInfo& shakeInfo : disconnectedList )
    {
        shakeInfo.onSktDisconnected();
    }
}

//============================================================================
void HandshakeList::getAndRemoveHandshakeInfo( VxSktBase* sktBase, std::vector<HandshakeInfo>& disconnectedList )
{
    VxGUIDList removeList;
    for( auto iter = m_ShakeList.begin(); iter != m_ShakeList.end(); ++iter )
    {
        HandshakeInfo& shakeInfo = iter->second;
        if( shakeInfo.getSktBase() == sktBase )
        {
            disconnectedList.push_back( shakeInfo );
            removeList.addGuid( shakeInfo.getSessionId() );
        }
    }

    std::vector<VxGUID>& sessionList = removeList.getGuidList();
    for( VxGUID& sesId : sessionList )
    {
        removeHandshakeInfo( sesId );
    }
}
