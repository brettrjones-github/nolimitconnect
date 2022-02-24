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

#include "ConnectIdListMgr.h"
#include "ConnectIdListCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <NetLib/VxPeerMgr.h>

//============================================================================
ConnectIdListMgr::ConnectIdListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
    setIdentListType( eUserViewTypeOnline );
}

//============================================================================
bool ConnectIdListMgr::getConnections( HostedId& hostId, std::set<ConnectId>& retConnectIdSet )
{
    retConnectIdSet.clear();
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast<ConnectId&>(connectId).getHostedId() == hostId )
        {
            retConnectIdSet.insert( connectId );
        }
    }

    unlockList();
    return !retConnectIdSet.empty();
}

//============================================================================
bool ConnectIdListMgr::isOnline( VxGUID& onlineId )
{
    bool isOnlined = false;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast< ConnectId& >( connectId ).getGroupieOnlineId() == onlineId )
        {
            isOnlined = true;
            break;
        }
    }

    unlockList();
    return isOnlined;
}

//============================================================================
bool ConnectIdListMgr::isOnline( GroupieId& groupieId )
{
    bool isOnlined = false;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast< ConnectId& >( connectId ).getGroupieId() == groupieId )
        {
            isOnlined = true;
            break;
        }
    }

    unlockList();
    return isOnlined;
}

//============================================================================
void ConnectIdListMgr::addConnection( VxGUID& sktConnectId, GroupieId& groupieId )
{
    if( !groupieId.isValid() || !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::addConnection invalid id" );
        return;
    }

    ConnectId connectId( sktConnectId,  groupieId );
    bool alreadyOnline = isOnline( groupieId.getGroupieOnlineId() );
    lockList();
    bool isInList = m_ConnectIdList.find( sktConnectId ) != m_ConnectIdList.end();
    if( !isInList )
    {
        // new connection
        m_ConnectIdList.insert( connectId );
    }

    unlockList();
    if( !alreadyOnline )
    {
        onUserOnlineStatusChange( groupieId, true );
    }

    if( !isInList )
    {
        onNewConnection( connectId );
    }
}

//============================================================================
void ConnectIdListMgr::removeConnection( VxGUID& sktConnectId, GroupieId& groupieId )
{
    if( !groupieId.isValid() || !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::removeConnection invalid id" );
        return;
    }

    bool wasRemoved = false;
    VxGUID& onlineId = groupieId.getGroupieOnlineId();
    ConnectId connectId( sktConnectId, groupieId );
    lockList();
    auto iter = m_ConnectIdList.find( connectId );
    if( iter != m_ConnectIdList.end() )
    {
        m_ConnectIdList.erase( iter );
        wasRemoved = true;
    }

    unlockList();

    if( wasRemoved && !isOnline( onlineId ) )
    {
        onUserOnlineStatusChange( groupieId, false );
    }

    if( wasRemoved )
    {
        onLostConnection( connectId );
    }
}

//============================================================================
void ConnectIdListMgr::addConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason )
{
    lockList();
    auto iter = m_ConnectReasonList.find( sktConnectId );
    if( iter != m_ConnectReasonList.end() )
    {
        iter->second.insert( connectReason );
    }
    else
    {
        std::set<EConnectReason> reasonSet{ connectReason };
        m_ConnectReasonList[sktConnectId] = reasonSet;
    }

    unlockList();
    announceConnectionReason( sktConnectId, connectReason, true );
}

//============================================================================
void ConnectIdListMgr::removeConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason )
{
    bool wasRemoved{ false };
    lockList();
    auto iter = m_ConnectReasonList.find( sktConnectId );
    if( iter != m_ConnectReasonList.end() )
    {
        auto iterReason = iter->second.find( connectReason );
        if( iterReason != iter->second.end() )
        {
            iter->second.erase( iterReason );
        }
    }

    unlockList();
    if( wasRemoved )
    {
        announceConnectionReason( sktConnectId, connectReason, false );
    } 
}

//============================================================================
void ConnectIdListMgr::onConnectionLost( VxGUID& sktConnectId )
{
    if( !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::onConnectionLost invalid id" );
        return;
    }

    std::set<ConnectId> lostConnectList;
    lockList();
    auto iter = m_ConnectIdList.begin();
    while( iter != m_ConnectIdList.end() )
    {
        if( const_cast< ConnectId& >( *iter ).getSocketId() == sktConnectId )
        {
            lostConnectList.insert( *iter );
            iter = m_ConnectIdList.erase( iter );
        }
        else
        {
            ++iter;
        }
    }

    unlockList();
    for( auto& connectId : lostConnectList )
    {
        if( !isOnline( const_cast< ConnectId& >( connectId ).getGroupieId() ) )
        {
            onUserOnlineStatusChange( const_cast< ConnectId& >( connectId ).getGroupieId(), false );
        }

        onLostConnection( const_cast< ConnectId& >( connectId ) );
    }

    announceConnectionLost( sktConnectId );
}

//============================================================================
void ConnectIdListMgr::userJoinedHost( VxGUID& sktConnectId, GroupieId& groupieId )
{
    if( !groupieId.isValid() || !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::userJoinedHost invalid id" );
        return;
    }

    addConnection( sktConnectId, groupieId );
}

//============================================================================
void ConnectIdListMgr::userLeftHost( VxGUID& sktConnectId, GroupieId& groupieId )
{
    if( !groupieId.isValid() || !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::userJoinedHost invalid id" );
        return;
    }

    removeConnection( sktConnectId, groupieId );
}

//============================================================================
void ConnectIdListMgr::disconnectIfIsOnlyUser( GroupieId& groupieId )
{
    /*
    bool wasLastUser = false;
    VxGUID disconnectConnectId;
    lockList();
    for( auto iter = m_ConnectIdList.begin(); iter != m_ConnectIdList.end(); ++iter )
    {
        auto iter = iter->second.find( groupieId );
        if( iter != iter->second.end() )
        {
            iter->second.erase( iter );
            if( iter->second.empty() )
            {
                wasLastUser = true;
                disconnectConnectId = iter->first;
            }

            break;
        }
    }

    unlockList();

    if( wasLastUser && disconnectConnectId.isVxGUIDValid() )
    {
        m_Engine.getPeerMgr().closeConnection( disconnectConnectId, eSktCloseNotNeeded );
    }
    */
}

//============================================================================
VxSktBase* ConnectIdListMgr::findHostConnection( GroupieId& groupieId, bool tryPeerFirst )
{
    if( !groupieId.isValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::findHostConnection invalid id" );
        return nullptr;
    }

    VxSktBase* sktBase = nullptr;
    if( groupieId.isValid() )
    {
        if( groupieId.getHostedOnlineId() == m_Engine.getMyOnlineId() && groupieId.getGroupieOnlineId() == m_Engine.getMyOnlineId() )
        {
            return m_Engine.getSktLoopback();
        }

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
VxSktBase* ConnectIdListMgr::findPeerConnection( VxGUID& onlineId )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::findPeerConnection invalid id" );
        return nullptr;
    }

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
bool ConnectIdListMgr::findConnectionId( GroupieId& groupieId, VxGUID& retSktConnectId )
{
    if( !groupieId.isValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::findConnectionId invalid id" );
        return false;
    }

    bool foundConnection = false;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast< ConnectId& >( connectId ).getGroupieId() == groupieId )
        {
            retSktConnectId = const_cast< ConnectId& >( connectId ).getSocketId();
            foundConnection = retSktConnectId.isVxGUIDValid();
            break;
        }
    }

    unlockList();
    return foundConnection;
}

//============================================================================
VxSktBase* ConnectIdListMgr::findSktBase( VxGUID& connectId )
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

//============================================================================
VxSktBase* ConnectIdListMgr::findAnyOnlineConnection( VxGUID& onlineId )
{
    std::set<VxGUID> sktConnectIdList;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast<ConnectId& >( connectId ).getGroupieId().getHostedOnlineId() == onlineId )
        {
            sktConnectIdList.insert( const_cast< ConnectId& >( connectId ).getSocketId() );
        }
    }

    unlockList();

    VxSktBase* sktBase = nullptr;
    for( auto sktConnectId : sktConnectIdList )
    {
        sktBase = findSktBase( sktConnectId );
        if( sktBase )
        {
            break;
        }
    }

    return sktBase;
}

//============================================================================
void ConnectIdListMgr::onUserOnlineStatusChange( VxGUID& onlineId, bool isOnline )
{
    announceOnlineStatus( onlineId, isOnline );
}

//============================================================================
void ConnectIdListMgr::onUserOnlineStatusChange( GroupieId& groupieId, bool isOnline )
{
    announceOnlineStatus( groupieId.getGroupieOnlineId(), isOnline );
}

//============================================================================
void ConnectIdListMgr::onNewConnection( ConnectId& connectId )
{
    announceConnectionStatus( connectId, true );
}

//============================================================================
void ConnectIdListMgr::onLostConnection( ConnectId& connectId )
{
    announceConnectionStatus( connectId, false );
}

//============================================================================
void ConnectIdListMgr::wantConnectIdListCallback( ConnectIdListCallbackInterface* client, bool enable )
{
    if( !client )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::wantConnectIdListCallback null client" );
        return;
    }

    lockClientList();
    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_CallbackClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_CallbackClients.push_back( client );
    }

    unlockClientList();
}

//============================================================================
void ConnectIdListMgr::announceOnlineStatus( VxGUID& onlineId, bool isOnline )
{
    lockClientList();

    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        ConnectIdListCallbackInterface* client = *iter;
        if( client )
        {
            client->callbackOnlineStatusChange( onlineId, isOnline );
        }
        else
        {
            LogMsg( LOG_ERROR, "ConnectIdListMgr::announceOnlineStatus null client" );
        }    
    }

    unlockClientList();
}

//============================================================================
void ConnectIdListMgr::announceConnectionStatus( ConnectId& connectId, bool isConnected )
{
    lockClientList();

    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        ConnectIdListCallbackInterface* client = *iter;
        if( client )
        {
            client->callbackConnectionStatusChange( connectId, isConnected );
        }
        else
        {
            LogMsg( LOG_ERROR, "ConnectIdListMgr::announceConnectionStatus null client" );
        }
    }

    unlockClientList();
}

//============================================================================
void ConnectIdListMgr::announceConnectionReason( VxGUID& sktConnectId, EConnectReason connectReason, bool enableReason )
{
    lockClientList();

    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        ConnectIdListCallbackInterface* client = *iter;
        if( client )
        {
            client->callbackConnectionReason( sktConnectId, connectReason, enableReason );
        }
        else
        {
            LogMsg( LOG_ERROR, "ConnectIdListMgr::announceConnectionReason null client" );
        }
    }

    unlockClientList();
}

//============================================================================
void ConnectIdListMgr::announceConnectionLost( VxGUID& sktConnectId )
{
    lockClientList();

    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        ConnectIdListCallbackInterface* client = *iter;
        if( client )
        {
            client->callbackConnectionLost( sktConnectId );
        }
        else
        {
            LogMsg( LOG_ERROR, "ConnectIdListMgr::announceConnectionLost null client" );
        }
    }

    unlockClientList();
}
