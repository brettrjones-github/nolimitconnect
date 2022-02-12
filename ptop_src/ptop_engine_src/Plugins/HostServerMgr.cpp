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

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>
#include <ptop_src/ptop_engine_src/UrlMgr/UrlMgr.h>

#include <PktLib/PktsHostInvite.h>
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
void HostServerMgr::sendHostAnnounceToNetworkHost( VxGUID& sessionId, PktHostInviteAnnounceReq& hostAnnounce, EConnectReason connectReason )
{
    // save announce pkt in announce session list
    std::string url = m_ConnectionMgr.getDefaultHostUrl( eHostTypeNetwork );
    if( url.empty() )
    {
        LogMsg( LOG_VERBOSE, "HostServerMgr network host url is empty" );
        return;
    }
    
    url = m_Engine.getUrlMgr().resolveUrl(url);

    if( m_Engine.getMyPktAnnounce().requiresRelay() )
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( hostAnnounce.getHostType(), sessionId, eHostAnnounceFailRequiresOpenPort, "Announce Host Requires An Open Port");
        return;
    }

    LogModule( eLogHosts, LOG_DEBUG, "sendHostAnnounceToNetworkHost %s", DescribePluginType( m_Plugin.getPluginType() ) );
    addAnnounceSession( sessionId, hostAnnounce.makeHostAnnCopy() );
    connectToHost( eHostTypeNetwork, sessionId, url, connectReason );
}

//============================================================================
void HostServerMgr::addAnnounceSession( VxGUID& sessionId, PktHostInviteAnnounceReq* hostAnn )
{
    removeAnnounceSession( sessionId );
    m_AnnListMutex.lock();
    m_AnnList[sessionId] = hostAnn;
    m_AnnListMutex.unlock();
}

//============================================================================
void HostServerMgr::removeAnnounceSession( VxGUID& sessionId )
{
    m_AnnListMutex.lock();
    auto iter = m_AnnList.find( sessionId );
    if( iter != m_AnnList.end() )
    {
        delete iter->second;
        m_AnnList.erase( iter );
    }

    m_AnnListMutex.unlock();
}

//============================================================================
void HostServerMgr::removeSession( VxGUID& sessionId, EConnectReason connectReason )
{
    if( isAnnounceConnectReason( connectReason ) )
    {
        removeAnnounceSession( sessionId );
    }

    HostServerSearchMgr::removeSession( sessionId, connectReason );
}

//============================================================================
void HostServerMgr::onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    removeSession( sessionId, connectReason );

    HostBaseMgr::onContactDisconnected( sessionId, sktBase, onlineId, connectReason );
}

//============================================================================
bool HostServerMgr::onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    bool result{ false };
    if( hostType == eHostTypeNetwork &&
        ( connectReason == eConnectReasonChatRoomAnnounce ||
            connectReason == eConnectReasonGroupAnnounce ||
            connectReason == eConnectReasonRandomConnectAnnounce ) )
    {
        m_AnnListMutex.lock();
        auto iter = m_AnnList.find( sessionId );
        if( iter != m_AnnList.end() )
        {
            if( iter->second->isValidPkt() )
            {
                result = m_Plugin.txPacket( onlineId, sktBase, iter->second, false, ePluginTypeHostNetwork );
            }
            else
            {
                LogMsg( LOG_VERBOSE, "HostServerMgr m_PktHostInviteAnnounceReq is invalid" );
            }

            delete iter->second;
            m_AnnList.erase( iter );
        }

        m_AnnListMutex.unlock();

        m_Engine.getConnectionMgr().doneWithConnection( sessionId, onlineId, this, connectReason );
    }
    else
    {
        HostBaseMgr::onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason );
        result = true;
    }

    return result;
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

//============================================================================
void HostServerMgr::onJoinRequested( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType )
{
    LogModule( eLogHosts, LOG_DEBUG, "onJoinRequested %s user %s", DescribePluginType( m_Plugin.getPluginType() ), netIdent->getOnlineName() );
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), sessionId, sktBase->getConnectionId() );
    m_Engine.getHostJoinMgr().onHostJoinRequestedByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onHostJoinRequestedByUser( sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostServerMgr::onUserJoined( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType )
{
    LogModule( eLogHosts, LOG_DEBUG, "onUserJoined %s user %s", DescribePluginType( m_Plugin.getPluginType() ), netIdent->getOnlineName() );
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), sessionId, sktBase->getConnectionId() );
    m_Engine.getHostJoinMgr().onHostJoinedByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getGroupieListMgr().onHostJoinedByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onHostJoinedByUser( sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostServerMgr::onUserLeftHost( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType )
{
    LogModule( eLogHosts, LOG_DEBUG, "onUserJoined %s user %s", DescribePluginType( m_Plugin.getPluginType() ), netIdent->getOnlineName() );
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), sessionId, sktBase->getConnectionId() );
    m_Engine.getHostJoinMgr().onHostLeftByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getGroupieListMgr().onHostLeftByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onHostLeftByUser( sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostServerMgr::onUserUnJoined( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID sessionId, EHostType hostType )
{
    LogModule( eLogHosts, LOG_DEBUG, "onUserJoined %s user %s", DescribePluginType( m_Plugin.getPluginType() ), netIdent->getOnlineName() );
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), sessionId, sktBase->getConnectionId() );
    m_Engine.getHostJoinMgr().onHostLeftByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getGroupieListMgr().onHostLeftByUser( sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onHostLeftByUser( sktBase, netIdent, sessionInfo );
}

//============================================================================
EJoinState HostServerMgr::getJoinState( VxNetIdent* netIdent, EHostType hostType )
{
    return m_Engine.getHostJoinMgr().fromGuiQueryJoinState( hostType, *netIdent );
}

//============================================================================
EMembershipState HostServerMgr::getMembershipState( VxNetIdent* netIdent, EHostType hostType )
{
    return m_Engine.getHostJoinMgr().fromGuiQueryMembership( hostType, *netIdent );
}

//============================================================================
void HostServerMgr::fromGuiListAction( EListAction listAction )
{
    HostServerSearchMgr::fromGuiListAction( listAction );
}
