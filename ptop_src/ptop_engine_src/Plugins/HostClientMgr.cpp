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

#include "HostClientMgr.h"
#include "PluginBase.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BaseInfo/BaseSessionInfo.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserOnlineMgr/UserOnlineMgr.h>

#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>
#include <PktLib/PluginIdList.h>

//============================================================================
HostClientMgr::HostClientMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
    : HostClientSearchMgr( engine, pluginMgr, myIdent, pluginBase )
{
}

//============================================================================
void HostClientMgr::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostJoinReply* hostReply = ( PktHostJoinReply* )pktHdr;
    if( hostReply->isValidPkt() )
    {
        GroupieId groupieId( m_Engine.getMyOnlineId(), netIdent->getMyOnlineId(), hostReply->getHostType() );
        m_Engine.getConnectIdListMgr().addConnection( sktBase->getSocketId(), groupieId, false );

        if( ePluginAccessOk == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinSuccess );
            BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), hostReply->getSessionId(), sktBase->getSocketId() );
            onUserJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
        }
        else if( ePluginAccessLocked == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinFailPermission );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, HostTypeToConnectJoinReason( hostReply->getHostType() ) );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinFail, DescribePluginAccess( hostReply->getAccessState() ) );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, HostTypeToConnectJoinReason( hostReply->getHostType() ) );
        }
    }
    else
    {
        onInvalidRxedPacket( sktBase, pktHdr, netIdent );     
    }
}

//============================================================================
void HostClientMgr::onPktHostLeaveReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    PktHostLeaveReply* hostReply = ( PktHostLeaveReply* )pktHdr;
    if( hostReply->isValidPkt() )
    {
        GroupieId groupieId =hostReply->getGroupieId();
        m_Engine.getConnectIdListMgr().removeConnection( sktBase->getSocketId(), groupieId );

        if( ePluginAccessOk == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinUnJoinSuccess );
            BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), hostReply->getSessionId(), sktBase->getSocketId() );
            onUserJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
        }
        else if( ePluginAccessLocked == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinFailPermission );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, HostTypeToConnectJoinReason( hostReply->getHostType() ) );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinFail, DescribePluginAccess( hostReply->getAccessState() ) );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, HostTypeToConnectJoinReason( hostReply->getHostType() ) );
        }
    }
    else
    {
        onInvalidRxedPacket( sktBase, pktHdr, netIdent );
    }
}

//============================================================================
void HostClientMgr::onPktHostUnJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    PktHostUnJoinReply* hostReply = ( PktHostUnJoinReply* )pktHdr;
    if( hostReply->isValidPkt() )
    {
        GroupieId groupieId( m_Engine.getMyOnlineId(), netIdent->getMyOnlineId(), hostReply->getHostType() );
        m_Engine.getConnectIdListMgr().removeConnection( sktBase->getSocketId(), groupieId );

        if( ePluginAccessOk == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinUnJoinSuccess );
            BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), netIdent->getMyOnlineId(), hostReply->getSessionId(), sktBase->getSocketId() );     
            onUserUnJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
        }
        else if( ePluginAccessLocked == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinFailPermission );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, HostTypeToConnectJoinReason( hostReply->getHostType() ) );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostJoinFail, DescribePluginAccess( hostReply->getAccessState() ) );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, HostTypeToConnectJoinReason( hostReply->getHostType() ) );
        }
    }
    else
    {
        onInvalidRxedPacket( sktBase, pktHdr, netIdent );
    }
}

//============================================================================
void HostClientMgr::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostSearchReply* hostReply = ( PktHostSearchReply* )pktHdr;
    ECommErr commErr = hostReply->getCommError();
    if( 0 == hostReply->getTotalMatches() )
    {
        if( eCommErrNone != commErr )
        {
            if( eCommErrPluginNotEnabled == commErr )
            {
                m_Engine.getToGui().toGuiHostSearchStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostSearchPluginDisabled, hostReply->getCommError() );
            }
            else if( eCommErrPluginPermission == commErr )
            {
                m_Engine.getToGui().toGuiHostSearchStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostSearchFailPermission, hostReply->getCommError() );
            }
            else
            {
                m_Engine.getToGui().toGuiHostSearchStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostSearchFail, hostReply->getCommError() );
            }
        }
        else
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostReply->getHostType(), netIdent->getMyOnlineId(), eHostSearchNoMatches, hostReply->getCommError() );
        }

        LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::onPktHostSearchReply no matches" );
        stopHostSearch( hostReply->getHostType(), hostReply->getSearchSessionId(), sktBase, netIdent->getMyOnlineId() );
    }
    else
    {
        startHostDetailSession( hostReply, sktBase, netIdent );
    }
}

//============================================================================
void HostClientMgr::onUserJoinedHost( GroupieId& groupieId, VxSktBase * sktBase, VxNetIdent * netIdent, BaseSessionInfo& sessionInfo )
{
    m_ServerListMutex.lock();
    m_ServerList.insert( groupieId );
    m_ServerListMutex.unlock();

    m_Engine.getUserJoinMgr().onUserJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onUserJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
    m_Engine.getThumbMgr().queryThumbIfNeeded( sktBase, netIdent, sessionInfo.getPluginType() );
}

//============================================================================
void HostClientMgr::onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    m_ServerListMutex.lock();
    m_ServerList.erase( groupieId );
    m_ServerListMutex.unlock();

    m_Engine.getUserJoinMgr().onUserLeftHost( groupieId, sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onUserLeftHost( groupieId, sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostClientMgr::onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    m_ServerListMutex.lock();
    m_ServerList.erase( groupieId );
    m_ServerListMutex.unlock();

    m_Engine.getUserJoinMgr().onUserUnJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
    m_Engine.getUserOnlineMgr().onUserUnJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostClientMgr::sendHostSearchToNetworkHost( VxGUID& sessionId, SearchParams& searchParams, EConnectReason connectReason )
{
    // save announce pkt in announce session list
    std::string url = m_ConnectionMgr.getDefaultHostUrl( eHostTypeNetwork );
    if( url.empty() )
    {
        LogMsg( LOG_VERBOSE, "HostServerMgr network host url is empty" );
        return;
    }

    addSearchSession( sessionId, searchParams );
    connectToHost( eHostTypeNetwork, sessionId, url, connectReason );
}

//============================================================================
void HostClientMgr::addPluginRxSession( VxGUID& sessionId, PluginIdList& pluginIdList )
{
    removePluginRxSession( sessionId );
    m_PluginRxListMutex.lock();
    m_PluginRxList[sessionId] = pluginIdList;
    m_PluginRxListMutex.unlock();
}

//============================================================================
void HostClientMgr::removePluginRxSession( VxGUID& sessionId )
{
    m_PluginRxListMutex.lock();
    auto iter = m_PluginRxList.find( sessionId );
    if( iter != m_PluginRxList.end() )
    {
        m_PluginRxList.erase( iter );
    }

    m_PluginRxListMutex.unlock();
}

//============================================================================
void HostClientMgr::removeSession( VxGUID& sessionId, EConnectReason connectReason )
{
    if( isSearchConnectReason( connectReason ) )
    {
        removeSearchSession( sessionId );
    }

    HostClientSearchMgr::removeSession( sessionId, connectReason );
}

//============================================================================
void HostClientMgr::onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    GroupieId groupieId( m_Engine.getMyOnlineId(), onlineId, getHostType() );
    m_ServerList.erase( groupieId );
    removeContact( onlineId );
}

//============================================================================
bool HostClientMgr::onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    bool result{ false };
    if( isSearchConnectReason( connectReason ) )
    {
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchConnectSuccess );
    }

    if( hostType == eHostTypeNetwork &&
        ( connectReason == eConnectReasonChatRoomSearch ||
            connectReason == eConnectReasonGroupSearch ||
            connectReason == eConnectReasonRandomConnectSearch ) )
    {
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, sessionId, eHostSearchSendingSearchRequest );

        m_SearchParamsMutex.lock();
        auto iter = m_SearchParamsList.find( sessionId );
        if( iter != m_SearchParamsList.end() )
        {
            SearchParams& searchParams = iter->second;
            PktHostSearchReq searchReq;
            searchReq.setHostType( searchParams.getHostType() );
            searchReq.setPluginType( m_Plugin.getPluginType() );
            searchReq.setSearchSessionId( sessionId );
            PktBlobEntry& blobEntry = searchReq.getBlobEntry();
            bool result = searchParams.addToBlob( blobEntry );
            searchReq.calcPktLen();
            // unlock before txPacket else in looback mode can cause a deadlock
            m_SearchParamsList.erase( iter );
            m_SearchParamsMutex.unlock();

            if( result && searchReq.isValidPkt() )
            {
                if( !m_Plugin.txPacket( onlineId, sktBase, &searchReq, false, m_Plugin.getDestinationPluginOverride( hostType ) ) )
                {
                    LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::onConnectToHostSuccess failed send PktHostSearchReq" );
                }
                else
                {
                    result = true;
                }
            }
            else
            {
                LogMsg( LOG_ERROR, "HostServerMgr PktHostSearchReq is invalid" );
            }

            // not done with connection.. wait for search results
        }
        else
        {
            m_SearchParamsMutex.unlock();
            LogMsg( LOG_ERROR, "HostServerMgr Search Params Not Found" );
            stopHostSearch( hostType, sessionId, sktBase, onlineId );
        }     
    }
    else
    {
        HostBaseMgr::onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason );
    }

    return result;
}

//============================================================================
void HostClientMgr::startHostDetailSession( PktHostSearchReply* hostReply, VxSktBase * sktBase, VxNetIdent * netIdent )
{
    EHostType hostType = hostReply->getHostType();
    VxGUID sessionId = hostReply->getSearchSessionId();
    int pluginIdCnt = hostReply->getTotalMatches();
    ECommErr commErr = hostReply->getCommError();
    if( eCommErrNone != commErr )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::startHostDetailSession comm error %s", DescribeCommError( commErr ) );
        if( eCommErrPluginNotEnabled == commErr )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, netIdent->getMyOnlineId(), eHostSearchPluginDisabled );
        }

        stopHostSearch( hostReply->getHostType(), hostReply->getSearchSessionId(), sktBase, netIdent->getMyOnlineId() );
        return;
    }

    if( !sessionId.isVxGUIDValid() )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::startHostDetailSession session id invalid");
    }

    bool result = sessionId.isVxGUIDValid() && pluginIdCnt > 0 && eCommErrNone == commErr;
    if( result )
    {
        // insert ids and send first request for plugin settings
        PluginIdList pluginIdList;
        PktBlobEntry& blobEntry = hostReply->getBlobEntry();
        blobEntry.resetRead();
        for( int i = 0; i < pluginIdCnt; i++ )
        {
            PluginId pluginId;
            if( blobEntry.getValue( pluginId ) )
            {
                pluginIdList.addPluginId( pluginId );
            }
            else
            {
                LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::startHostDetailSession error getting plug id at index %d", i);
                result = false;
                break;
            }
        }

        if( result )
        {
            addPluginRxSession( sessionId, pluginIdList );
            result = sendNextPluginSettingRequest( hostReply->getHostType(), sessionId, sktBase, netIdent );
        }
    }

    if( !result )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::startHostDetailSession failed");
        stopHostSearch( hostReply->getHostType(), hostReply->getSearchSessionId(), sktBase, netIdent->getMyOnlineId() );
    }
}

//============================================================================
bool HostClientMgr::sendNextPluginSettingRequest( EHostType hostType, VxGUID& sessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
    bool result = false;
    m_PluginRxListMutex.lock();
    auto iter = m_PluginRxList.find( sessionId );
    if( iter != m_PluginRxList.end() )
    {
        PluginIdList& pluginAllList = iter->second;
        std::vector<PluginId>& pluginList = pluginAllList.getPluginIdList();
        if( pluginList.size() )
        {
            PluginId pluginId = pluginList.back();
            pluginList.pop_back();
            PktPluginSettingReq pluginIdReq;
            pluginIdReq.setHostType( hostType );
            pluginIdReq.setPluginId( pluginId );
            pluginIdReq.setPluginType( m_Plugin.getPluginType() );
            pluginIdReq.setSessionId( sessionId );

            // TODO debug only REMOVE ME
            // pluginIdReq.setIsLoopback( true );
            if( m_Plugin.txPacket( netIdent->getMyOnlineId(), sktBase, &pluginIdReq, false, ePluginTypeHostNetwork ) )
            {
                result = true;
            }
            else
            {
                LogModule( eLogHostSearch, LOG_DEBUG, "sendNextPluginSettingRequest send failed" );
            }
        }
        else
        {
            LogMsg( LOG_VERBOSE, "HostClientMgr rxed all plugin settings" );
        }
    }

    m_PluginRxListMutex.unlock();
    if( !result )
    {
        LogMsg( LOG_VERBOSE, "HostClientMgr rxed all plugin settings" );
        stopHostSearch( hostType, sessionId, sktBase, netIdent->getMyOnlineId() );
    }

    return result;
}

//============================================================================
bool HostClientMgr::stopHostSearch( EHostType hostType, VxGUID& sessionId, VxSktBase * sktBase, VxGUID& onlineId )
{
    m_Engine.getToGui().toGuiHostSearchStatus( hostType, onlineId, eHostSearchCompleted );
    m_Engine.getToGui().toGuiHostSearchComplete( hostType, onlineId );

    removeSearchSession( sessionId );
    EConnectReason connectReason = getSearchConnectReason(hostType);
    m_Engine.getConnectionMgr().doneWithConnection( sessionId, onlineId, this, connectReason );
    return true;
}

//============================================================================
void HostClientMgr::onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    VxGUID sessionId;
    sessionId.initializeWithNewVxGUID();
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), groupieId.getGroupieOnlineId(), sessionId, sktBase->getSocketId() );
    onUserJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostClientMgr::onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    VxGUID sessionId;
    sessionId.initializeWithNewVxGUID();
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), groupieId.getGroupieOnlineId(), sessionId, sktBase->getSocketId() );
    onUserLeftHost( groupieId, sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostClientMgr::onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    VxGUID sessionId;
    sessionId.initializeWithNewVxGUID();
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), groupieId.getGroupieOnlineId(), sessionId, sktBase->getSocketId() );
    onUserUnJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
}

//============================================================================
void HostClientMgr::onGroupRelayedUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    LogMsg( LOG_VERBOSE, "HostClientMgr::onGroupRelayedUserAnnounce from %s id %s hosted by %s id %s groupieId %s",
            netIdent->getOnlineName(), netIdent->getMyOnlineId().describeVxGUID().c_str(), sktBase->getPeerOnlineName().c_str(),
        sktBase->getPeerOnlineId().describeVxGUID().c_str(), groupieId.describeGroupieId().c_str());

    m_Engine.getToGui().toGuiHostJoinStatus( groupieId.getHostType(), groupieId.getGroupieOnlineId(), eHostJoinSuccess );
    VxGUID sessionId;
    sessionId.initializeWithNewVxGUID();
    BaseSessionInfo sessionInfo( m_Plugin.getPluginType(), groupieId.getGroupieOnlineId(), sessionId, sktBase->getSocketId() );
    onUserJoinedHost( groupieId, sktBase, netIdent, sessionInfo );
}

