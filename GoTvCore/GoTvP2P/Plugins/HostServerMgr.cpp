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

#include "HostServerMgr.h"
#include "PluginBase.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <PktLib/VxCommon.h>

//============================================================================
HostServerMgr::HostServerMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
    : HostServerSearchMgr( engine, pluginMgr, myIdent, pluginBase )
{
}

//============================================================================
void HostServerMgr::onClientJoined( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    m_ServerMutex.lock();
    addClient( sktBase, netIdent );
    m_ServerMutex.unlock();
    addContact(sktBase, netIdent );
}

//============================================================================
void HostServerMgr::sendHostAnnounceToNetworkHost( VxGUID& sessionId, PktHostAnnounce& hostAnnounce, EConnectReason connectReason )
{
    m_ServerMutex.lock();
    memcpy( &m_PktHostAnnounce, &hostAnnounce, hostAnnounce.getPktLength() );
    m_ServerMutex.unlock();
    std::string url = m_ConnectionMgr.getDefaultHostUrl( eHostTypeNetwork );
    if( url.empty() )
    {
        LogMsg( LOG_VERBOSE, "HostServerMgr network host url is empty" );
        return;
    }

    connectToHost( eHostTypeNetwork, sessionId, url, connectReason );
}

//============================================================================
void HostServerMgr::onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    m_ServerMutex.lock();
    if( removeContact( onlineId ) )
    {
        // TODO send clients a status offline message
    }

    m_ServerMutex.unlock();
    HostBaseMgr::onContactDisconnected( sessionId, sktBase, onlineId, connectReason );
}

//============================================================================
void HostServerMgr::onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    if( hostType == eHostTypeNetwork &&
        ( connectReason == eConnectReasonChatRoomAnnounce ||
            connectReason == eConnectReasonGroupAnnounce ||
            connectReason == eConnectReasonRandomConnectAnnounce ) )
    {
        m_ServerMutex.lock();
        if( m_PktHostAnnounce.isValidPkt() )
        {
            m_Plugin.txPacket( onlineId, sktBase, &m_PktHostAnnounce, false, ePluginTypeNetworkHost );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "HostServerMgr m_PktHostAnnounce is invalid" );
        }

        m_ServerMutex.unlock();

        m_Engine.getConnectionMgr().doneWithConnection( sessionId, onlineId, this, connectReason );
    }
    else
    {
        HostBaseMgr::onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason );
    }
}

//============================================================================
bool HostServerMgr::addClient( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    bool wasAdded = m_ContactList.addGuidIfDoesntExist( netIdent->getMyOnlineId() );
    if( wasAdded )
    {
        // TODO implement contact added
    }

    return wasAdded;
}

//============================================================================
bool HostServerMgr::removeClient( VxGUID& onlineId )
{
    bool wasRemoved = m_ContactList.removeGuid( onlineId );
    if( wasRemoved )
    {
        // TODO implement contact removed
    }

    return wasRemoved;
}