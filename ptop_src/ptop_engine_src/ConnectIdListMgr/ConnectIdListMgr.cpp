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

#include "ConnectIdListMgr.h"
#include "ConnectIdListCallbackInterface.h"

#include <ptop_src/ptop_engine_src/BaseInfo/BaseSessionInfo.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>

#include <NetLib/VxPeerMgr.h>

//============================================================================
ConnectIdListMgr::ConnectIdListMgr( P2PEngine& engine )
    : IdentListMgrBase( engine )
{
    setIdentListType( eUserViewTypeOnline );
}

//============================================================================
bool ConnectIdListMgr::getConnections( HostedId& hostId, std::set<ConnectId>& directConnectIdSet, std::set<ConnectId>& relayConnectIdSet )
{
    directConnectIdSet.clear();
    relayConnectIdSet.clear();
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast<ConnectId&>(connectId).getHostedId() == hostId )
        {
            directConnectIdSet.insert( connectId );
        }
    }

    for( auto& connectId : m_RelayedIdList )
    {
        if( const_cast<ConnectId&>(connectId).getHostedId() == hostId )
        {
            relayConnectIdSet.insert( connectId );
        }
    }

    unlockList();
    return !directConnectIdSet.empty() || !relayConnectIdSet.empty();
}

//============================================================================
bool ConnectIdListMgr::isOnline( VxGUID& onlineId )
{
    if( onlineId == m_Engine.getMyOnlineId() )
    {
        return true;
    }

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

    if( !isOnlined )
    {
        for( auto& connectId : m_RelayedIdList )
        {
            if( const_cast<ConnectId&>(connectId).getGroupieOnlineId() == onlineId )
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
bool ConnectIdListMgr::isHosted( VxGUID& onlineId )
{
    if( onlineId == m_Engine.getMyOnlineId() )
    {
        return true;
    }

    bool isHosted = false;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        ConnectId& noConstConnectId = const_cast< ConnectId& >( connectId );
        if( noConstConnectId.getGroupieOnlineId() == onlineId && IsHostARelayForUser( noConstConnectId.getHostType() ) )
        {
            isHosted = true;
            break;
        }
    }

    if( !isHosted )
    {
        for( auto& connectId : m_RelayedIdList )
        {
            ConnectId& noConstConnectId = const_cast<ConnectId&>(connectId);
            if( noConstConnectId.getGroupieOnlineId() == onlineId && IsHostARelayForUser( noConstConnectId.getHostType() ) )
            {
                isHosted = true;
                break;
            }
        }
    }

    unlockList();
    return isHosted;
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

    if( !isOnlined )
    {
        for( auto& connectId : m_RelayedIdList )
        {
            if( const_cast<ConnectId&>(connectId).getGroupieId() == groupieId )
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
void ConnectIdListMgr::addConnection( VxGUID& sktConnectId, GroupieId& groupieId, bool relayed )
{
    if( !groupieId.isValid() || !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::addConnection invalid id" );
        return;
    }

    ConnectId connectId( sktConnectId,  groupieId );
    bool alreadyOnline = isOnline( groupieId.getGroupieOnlineId() );
    if( relayed )
    {
        lockList();
        bool hasDirectConnection = m_ConnectIdList.find( connectId ) != m_ConnectIdList.end();
        bool isInList = m_RelayedIdList.find( connectId ) != m_RelayedIdList.end();
        if( !isInList )
        {
            // new connection
            m_RelayedIdList.insert( connectId );
        }

        unlockList();

        announceRelayStatus(connectId, true);
    }
    else
    {
        lockList();
        bool isInList = m_ConnectIdList.find( connectId ) != m_ConnectIdList.end();
        if( !isInList )
        {
            // new connection
            m_ConnectIdList.insert( connectId );
        }

        unlockList();

        announceConnectionStatus( connectId, true );
    }

    announceOnlineStatus( groupieId.getGroupieOnlineId(), isOnline( groupieId.getGroupieOnlineId() ) );
}

//============================================================================
void ConnectIdListMgr::removeConnection( VxGUID& sktConnectId, GroupieId& groupieId )
{
    if( !groupieId.isValid() || !sktConnectId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::removeConnection invalid id" );
        return;
    }

    bool wasRemovedFromDirectConnect = false;
    bool wasRemovedFromRelayed = false;
    VxGUID& onlineId = groupieId.getGroupieOnlineId();
    ConnectId connectId( sktConnectId, groupieId );

    bool wasOnline = isOnline( onlineId );

    lockList();
    auto iter = m_ConnectIdList.find( connectId );
    if( iter != m_ConnectIdList.end() )
    {
        m_ConnectIdList.erase( iter );
        wasRemovedFromDirectConnect = true;
    }

    auto iterRelayed = m_RelayedIdList.find( connectId );
    if( iterRelayed != m_RelayedIdList.end() )
    {
        m_RelayedIdList.erase( iterRelayed );
        wasRemovedFromRelayed = true;
    }

    unlockList();

    if( wasRemovedFromDirectConnect )
    {
        announceConnectionStatus( connectId, false );
    }

    if( wasRemovedFromRelayed )
    {
        announceRelayStatus( connectId, false );
    }

    if( wasOnline && !isOnline( onlineId ) )
    {
        announceOnlineStatus( onlineId, false );
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

    std::set<VxGUID> userList;
    std::set<ConnectId> lostConnectList;
    std::set<ConnectId> lostRelayList;
    lockList();
    auto iter = m_ConnectIdList.begin();
    while( iter != m_ConnectIdList.end() )
    {
        ConnectId& connectId = const_cast<ConnectId&>(*iter);
        if( connectId.getSocketId() == sktConnectId )
        {
            userList.insert( connectId.getGroupieOnlineId() );
            lostConnectList.insert( connectId );
            iter = m_ConnectIdList.erase( iter );
        }
        else
        {
            ++iter;
        }
    }

    auto iterRelay = m_RelayedIdList.begin();
    while( iterRelay != m_RelayedIdList.end() )
    {
        ConnectId& connectId = const_cast<ConnectId&>(*iterRelay);
        if( connectId.getSocketId() == sktConnectId )
        {
            userList.insert( connectId.getGroupieOnlineId() );
            lostRelayList.insert( connectId );
            iterRelay = m_RelayedIdList.erase( iterRelay );
        }
        else
        {
            ++iterRelay;
        }
    }

    unlockList();

    for( auto& connectId : lostConnectList )
    {
        announceConnectionStatus( const_cast< ConnectId& >( connectId ), false );
    }

    for( auto& connectId : lostRelayList )
    {
        announceRelayStatus( const_cast<ConnectId&>(connectId), false );
    }

    for( auto& onlineId : userList )
    {
        announceOnlineStatus( const_cast<VxGUID&>(onlineId), isOnline( const_cast<VxGUID&>(onlineId) ) );
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

    addConnection( sktConnectId, groupieId, false );
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
    // host connection can only be a direct connection
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
VxSktBase* ConnectIdListMgr::findRelayMemberConnection( VxGUID& onlineId )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::findUserConnection invalid id" );
        return nullptr;
    }

    if( onlineId == m_Engine.getMyOnlineId() )
    {
        return m_Engine.getSktLoopback();
    }

    VxSktBase* sktBase = nullptr;
    lockList();
    for( auto& connectIdConst : m_ConnectIdList )
    {
        ConnectId& connectId = const_cast<ConnectId&>(connectIdConst);
        if( connectId.getGroupieOnlineId() == onlineId && IsHostARelayForUser( connectId.getHostType() ) )
        {
            sktBase = findSktBase( connectId.getSocketId() );
            if( sktBase )
            {
                break;
            }
        }
    }

    if( !sktBase )
    {
        for( auto& connectIdConst : m_RelayedIdList )
        {
            ConnectId& connectId = const_cast<ConnectId&>(connectIdConst);
            if( connectId.getGroupieOnlineId() == onlineId && IsHostARelayForUser( connectId.getHostType() ) )
            {
                sktBase = findSktBase( connectId.getSocketId() );
                if( sktBase )
                {
                    break;
                }
            }
        }
    }

    unlockList();

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

    VxGUID connectId;
    if( findConnectionId( groupieIdDirect, connectId ) )
    {
        sktBase = findSktBase( connectId );
    }
    else if( findRelayConnectionId( onlineId, connectId ) )
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

    if( !foundConnection )
    {
        for( auto& connectId : m_RelayedIdList )
        {
            if( const_cast<ConnectId&>(connectId).getGroupieId() == groupieId )
            {
                retSktConnectId = const_cast<ConnectId&>(connectId).getSocketId();
                foundConnection = retSktConnectId.isVxGUIDValid();
                break;
            }
        }
    }

    unlockList();
    return foundConnection;
}

//============================================================================
bool ConnectIdListMgr::findRelayConnectionId( VxGUID& onlineId, VxGUID& retSktConnectId )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "ConnectIdListMgr::findRelayConnectionId invalid id" );
        return false;
    }

    bool foundConnection = false;
    lockList();
    for( auto& connectId : m_RelayedIdList )
    {
        if( const_cast<ConnectId&>(connectId).getGroupieOnlineId() == onlineId )
        {
            retSktConnectId = const_cast<ConnectId&>(connectId).getSocketId();
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
    if( onlineId == m_Engine.getMyOnlineId() )
    {
        return m_Engine.getSktLoopback();
    }

    std::set<VxGUID> sktConnectIdList;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast<ConnectId& >( connectId ).getGroupieId().getHostedOnlineId() == onlineId )
        {
            sktConnectIdList.insert( const_cast< ConnectId& >( connectId ).getSocketId() );
        }
    }

    if( sktConnectIdList.empty() )
    {
        for( auto& connectId : m_RelayedIdList )
        {
            if( const_cast<ConnectId&>(connectId).getGroupieId().getHostedOnlineId() == onlineId )
            {
                sktConnectIdList.insert( const_cast<ConnectId&>(connectId).getSocketId() );
            }
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
VxSktBase* ConnectIdListMgr::findBestOnlineConnection( VxGUID& onlineId )
{
    if( onlineId == m_Engine.getMyOnlineId() )
    {
        return m_Engine.getSktLoopback();
    }

    std::vector<ConnectId> connectIdList;
    lockList();
    for( auto& connectId : m_ConnectIdList )
    {
        if( const_cast< ConnectId& >( connectId ).getGroupieId().getHostedOnlineId() == onlineId )
        {
            connectIdList.push_back( connectId );
        }
    }

    unlockList();

    VxGUID sktConnectId;
    // first check for direct connection
    GroupieId directGroupieId( onlineId, onlineId, eHostTypePeerUserDirect );
    for( auto& connectId : connectIdList )
    {
        if( connectId.getGroupieId() == directGroupieId )
        {
            sktConnectId = connectId.getSocketId();
            break;
        }
    }

    if( sktConnectId.isVxGUIDValid() )
    {
        return findSktBase( sktConnectId );
    }

    return findAnyOnlineConnection( onlineId );
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
void ConnectIdListMgr::announceRelayStatus( ConnectId& connectId, bool isRelayed )
{
    lockClientList();

    for( auto iter = m_CallbackClients.begin(); iter != m_CallbackClients.end(); ++iter )
    {
        ConnectIdListCallbackInterface* client = *iter;
        if( client )
        {
            client->callbackRelayStatusChange( connectId, isRelayed );
        }
        else
        {
            LogMsg( LOG_ERROR, "ConnectIdListMgr::announceRelayStatus null client" );
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

//============================================================================
void ConnectIdListMgr::onGroupUserAnnounce( PktAnnounce* pktAnn, VxSktBase* sktBase, VxNetIdent* netIdent, bool relayed )
{
    if( relayed )
    {
        onGroupRelayedUserAnnounce( pktAnn, sktBase, netIdent );
        return;
    }

    VxGUID onlineId = netIdent->getMyOnlineId();
    VxGUID connectionId = sktBase->getSocketId();
    // if relayed then the peer id should be the host that relayed the packet
    VxGUID hostOnlineId = sktBase->getPeerOnlineId();
    if( onlineId.isVxGUIDValid() && hostOnlineId.isVxGUIDValid(), connectionId.isVxGUIDValid() )
    {
        if( onlineId == hostOnlineId )
        {
            EHostType hostType{ eHostTypeUnknown };

            lockList();
            for( auto& connectIdConst : m_ConnectIdList )
            {
                ConnectId& connectId = const_cast<ConnectId&>(connectIdConst);
                if( IsHostARelayForUser( connectId.getHostType() ) )
                {
                    if( connectId.getHostedId().getOnlineId() == hostOnlineId )
                    {
                        hostType = connectId.getHostType();
                        hostOnlineId = connectId.getHostedId().getOnlineId();
                        break;
                    }
                }
            }

            unlockList();

            if( hostType != eHostTypeUnknown )
            {
                LogMsg( LOG_VERBOSE, "ConnectIdListMgr::onGroupUserAnnounce %s from host %s",
                    netIdent->getOnlineName(), sktBase->getPeerOnlineName().c_str() );

                GroupieId groupieId( onlineId, hostOnlineId, hostType );
                addConnection( connectionId, groupieId, relayed );
                m_Engine.getUserOnlineMgr().onUserOnline( groupieId, sktBase, netIdent );
            }
            else
            {
                LogMsg( LOG_WARNING, "ConnectIdListMgr::onGroupUserAnnounce hostId not found" );
            }
        }
        else
        {
            LogMsg( LOG_WARNING, "ConnectIdListMgr::onGroupUserAnnounce onlineId != hostOnlineId" );
        }
    }
    else
    {
        LogMsg( LOG_WARNING, "ConnectIdListMgr::onGroupUserAnnounce invalid id" );
    }
}

//============================================================================
void ConnectIdListMgr::onGroupRelayedUserAnnounce( PktAnnounce* pktAnn, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    VxGUID onlineId = netIdent->getMyOnlineId();
    VxGUID socketId = sktBase->getSocketId();
    // if relayed then the peer id should be the host that relayed the packet
    VxGUID hostOnlineId = sktBase->getPeerOnlineId();

    LogMsg( LOG_VERBOSE, "ConnectIdListMgr::onGroupRelayedUserAnnounce from %s id %s hosted by %s id %s",
            netIdent->getOnlineName(), onlineId.describeVxGUID().c_str(), sktBase->getPeerOnlineName().c_str(), hostOnlineId.describeVxGUID().c_str());
    if( onlineId.isVxGUIDValid() && hostOnlineId.isVxGUIDValid(), socketId.isVxGUIDValid() )
    {
        if( onlineId != hostOnlineId )
        {
            std::set<EHostType> hostTypes;

            lockList();
            for( auto& connectIdConst : m_ConnectIdList )
            {
                ConnectId& connectId = const_cast<ConnectId&>(connectIdConst);
                if( connectId.getSocketId() == socketId && IsHostARelayForUser( connectId.getHostType() ) )
                {
                    if( connectId.getHostedId().getOnlineId() == hostOnlineId )
                    {
                        hostTypes.insert( connectId.getHostType() );
                    }
                }
            }

            unlockList();

            if( !hostTypes.empty() )
            {
                for( auto hostType : hostTypes )
                {
                    LogMsg( LOG_VERBOSE, "ConnectIdListMgr::onGroupRelayedUserAnnounce %s from host %s %s",
                        netIdent->getOnlineName(), DescribeHostType( hostType ), sktBase->getPeerOnlineName().c_str() );

                    GroupieId groupieId( onlineId, hostOnlineId, hostType );

                    // add the connection first so if user is added it's online status is valid
                    addConnection( socketId, groupieId, true );
                    // next make the user up todate
                    m_Engine.getUserOnlineMgr().onUserOnline( groupieId, sktBase, netIdent );
                    // finally add the user group join info
                    PluginBase* plugin = m_Engine.getPluginMgr().findPlugin( HostTypeToClientPlugin( hostType ) );
                    if( plugin )
                    {
                        plugin->onGroupRelayedUserAnnounce( groupieId, sktBase, netIdent );
                    }
                    else
                    {
                        LogMsg( LOG_VERBOSE, "ConnectIdListMgr::onGroupRelayedUserAnnounce %s from host %s %s Faield to get plugin",
                            netIdent->getOnlineName(), DescribeHostType( hostType ), sktBase->getPeerOnlineName().c_str() );
                    }
                }
            }
            else
            {
                LogMsg( LOG_WARNING, "ConnectIdListMgr::onGroupRelayedUserAnnounce hostId not found" );
            }
        }
        else
        {
            LogMsg( LOG_WARNING, "ConnectIdListMgr::onGroupRelayedUserAnnounce onlineId != hostOnlineId" );
        }
    }
    else
    {
        LogMsg( LOG_WARNING, "ConnectIdListMgr::onGroupRelayedUserAnnounce invalid id" );
    }
}
