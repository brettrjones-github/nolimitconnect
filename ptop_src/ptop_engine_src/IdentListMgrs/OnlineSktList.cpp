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
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <NetLib/VxPeerMgr.h>

//============================================================================
OnlineSktList::OnlineSktList( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
}

//============================================================================
bool OnlineSktList::getConnections( HostedId& hostId, std::set<std::pair<VxGUID, VxGUID>>& retConnectIdSet )
{
    retConnectIdSet.clear();
    lockList();
    for( auto mapIter = m_OnlineSktList.begin(); mapIter != m_OnlineSktList.end(); ++mapIter )
    {
        for( auto iter = mapIter->second.begin(); iter != mapIter->second.end(); ++iter )
        {
            if( ( ( GroupieId& )*iter ).getHostedId() == hostId )
            {
                retConnectIdSet.insert( std::make_pair(mapIter->first, ( ( GroupieId& )*iter ).getGroupieOnlineId() ) );
            }
        }
    }

    unlockList();
    return !retConnectIdSet.empty();
}

//============================================================================
bool OnlineSktList::isOnline( VxGUID& onlineId )
{
    bool isOnlined = false;
    lockList();
    for( auto mapIter = m_OnlineSktList.begin(); mapIter != m_OnlineSktList.end(); ++mapIter )
    {
        for( auto iter = mapIter->second.begin(); iter != mapIter->second.end(); ++iter )
        {
            if( ((GroupieId&)*iter).getGroupieOnlineId() == onlineId )
            {
                isOnlined = true;
                break;
            }
        }
    }

    unlockList();
    return isOnlined;
}

//============================================================================
bool OnlineSktList::isOnline( GroupieId& groupieId )
{
    bool isOnlined = false;
    lockList();
    for( auto mapIter = m_OnlineSktList.begin(); mapIter != m_OnlineSktList.end(); ++mapIter )
    {
        auto iter = mapIter->second.find( groupieId );
        if( iter != mapIter->second.end() )
        {
            isOnlined = true;
            break;
        }
    }

    unlockList();
    return isOnlined;
}

//============================================================================
void OnlineSktList::addConnection( VxGUID& sktConnectId, GroupieId& groupieId )
{
    if( !groupieId.isValid() )
    {
        LogMsg( LOG_ERROR, "OnlineSktList::addConnection invalid id" );
        return;
    }

    bool alreadyOnline = isOnline( groupieId.getGroupieOnlineId() );
    lockList();
    auto mapIter = m_OnlineSktList.find( sktConnectId );
    if( mapIter == m_OnlineSktList.end() )
    {
        // new connection
        std::set<GroupieId> groupieSet{ groupieId };
        m_OnlineSktList[sktConnectId] = groupieSet;
    }
    else
    {
        if( mapIter->second.empty() )
        {
            std::set<GroupieId> groupieSet{ groupieId };
            mapIter->second = groupieSet;
        }
        else
        {
            mapIter->second.insert( groupieId );

        }
    }

    unlockList();
    if( !alreadyOnline )
    {
        onUserOnlineStatusChange( groupieId, true );
    }
}

//============================================================================
void OnlineSktList::removeConnection( VxGUID& sktConnectId, GroupieId& groupieId )
{
    bool wasRemoved = false;
    VxGUID& onlineId = groupieId.getGroupieOnlineId();
    lockList();
    auto mapIter = m_OnlineSktList.find( sktConnectId );
    if( mapIter != m_OnlineSktList.end() )
    {
        for( auto iter = mapIter->second.begin(); iter != mapIter->second.end(); ++iter )
        {
            if( groupieId == *iter )
            {
                mapIter->second.erase( iter );
                wasRemoved = true;
                break;
            }
        }
    }

    unlockList();

    if( wasRemoved && !isOnline( onlineId ) )
    {
        onUserOnlineStatusChange( groupieId, false );
    }
}

//============================================================================
void OnlineSktList::onConnectionLost( VxGUID& sktConnectId )
{
    std::set<VxGUID> identList;
    lockList();
    auto mapIter = m_OnlineSktList.find( sktConnectId );
    if( mapIter != m_OnlineSktList.end() )
    {
        for( auto iter = mapIter->second.begin(); iter != mapIter->second.end(); ++iter )
        {
            identList.insert( (( GroupieId& )( *iter )).getGroupieOnlineId() );
        }
    }

    unlockList();
    for( auto iter = identList.begin(); iter != identList.end(); ++iter )
    {
        if( !isOnline( ( VxGUID& )(*iter) ) )
        {
            onUserOnlineStatusChange( ( VxGUID& )( *iter ), false );
        }
    }
}

//============================================================================
void OnlineSktList::userJoinedHost( VxGUID& sktConnectId, GroupieId& groupieId )
{
    addConnection( sktConnectId, groupieId );
}

//============================================================================
void OnlineSktList::userLeftHost( VxGUID& sktConnectId, GroupieId& groupieId )
{
    removeConnection( sktConnectId, groupieId );
}

//============================================================================
void OnlineSktList::disconnectIfIsOnlyUser( GroupieId& groupieId )
{
    bool wasLastUser = false;
    VxGUID disconnectConnectId;
    lockList();
    for( auto mapIter = m_OnlineSktList.begin(); mapIter != m_OnlineSktList.end(); ++mapIter )
    {
        auto iter = mapIter->second.find( groupieId );
        if( iter != mapIter->second.end() )
        {
            mapIter->second.erase( iter );
            if( mapIter->second.empty() )
            {
                wasLastUser = true;
                disconnectConnectId = mapIter->first;
            }

            break;
        }
    }

    unlockList();

    if( wasLastUser && disconnectConnectId.isVxGUIDValid() )
    {
        m_Engine.getPeerMgr().closeConnection( disconnectConnectId, eSktCloseNotNeeded );
    }
}

//============================================================================
VxSktBase* OnlineSktList::findHostConnection( GroupieId& groupieId, bool tryPeerFirst )
{
    VxSktBase* sktBase = nullptr;
    if( groupieId.isValid() )
    {
        if( tryPeerFirst )
        {
            sktBase = findPeerConnection( groupieId.getGroupieOnlineId() );
        }

        if( !sktBase )
        {
            VxGUID connectId;
            if( findConnectionId( groupieId, connectId ) )
            {
                sktBase = findSktBase( connectId );
            }
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "findHostConnection invalid groupieId" );
    }

    return sktBase;
}

//============================================================================
VxSktBase* OnlineSktList::findPeerConnection( VxGUID& onlineId )
{
    VxSktBase* sktBase = nullptr;
    GroupieId groupieIdDirect( onlineId, onlineId, eHostTypePeerUserDirect );
    GroupieId groupieIdRelayed( onlineId, onlineId, eHostTypePeerUserRelayed );
    VxGUID connectId;
    if( findConnectionId( groupieIdDirect, connectId ) )
    {
        sktBase = findSktBase( connectId );
    }
    else if( findConnectionId( groupieIdRelayed, connectId ) )
    {
        sktBase = findSktBase( connectId );
    }

    return sktBase;
}

//============================================================================
bool OnlineSktList::findConnectionId( GroupieId& groupieId, VxGUID& retSktConnectId )
{
    bool foundConnection = false;
    lockList();
    for( auto mapIter = m_OnlineSktList.begin(); mapIter != m_OnlineSktList.end(); ++mapIter )
    {
        auto iter = mapIter->second.find( groupieId );
        if( iter != mapIter->second.end() )
        {
            foundConnection = true;
            retSktConnectId = mapIter->first;
            break;
        }
    }

    unlockList();
    return foundConnection;
}

//============================================================================
VxSktBase* OnlineSktList::findSktBase( VxGUID& connectId )
{
    bool isConnected{ false };
    m_Engine.getPeerMgr().lockSktList();
    VxSktBase* sktBase = m_Engine.getPeerMgr().findSktBase( connectId );
    if( sktBase )
    {
        sktBase = sktBase->isConnected() ? sktBase : nullptr;
    }

    m_Engine.getPeerMgr().unlockSktList();
    return sktBase;
}
