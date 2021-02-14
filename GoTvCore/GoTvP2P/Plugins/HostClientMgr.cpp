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

#include "HostClientMgr.h"
#include "PluginBase.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

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
        if( ePluginAccessOk == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, netIdent->getMyOnlineId(), eHostJoinSuccess );
            onHostJoined( sktBase, netIdent );
       
        }
        else if( ePluginAccessOk == hostReply->getAccessState() )
        {
            m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, netIdent->getMyOnlineId(), eHostJoinFailPermission );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, eConnectReasonChatRoomJoin );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, netIdent->getMyOnlineId(), eHostJoinFail, DescribePluginAccess2( hostReply->getAccessState() ) );
            m_Engine.getConnectionMgr().doneWithConnection( hostReply->getSessionId(), netIdent->getMyOnlineId(), this, eConnectReasonChatRoomJoin );
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

}

//============================================================================
void HostClientMgr::onHostJoined( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    m_ServerList.addGuidIfDoesntExist( netIdent->getMyOnlineId() );
    //addContact(sktBase, netIdent );
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

    addSearchSession( sessionId,searchParams );
    connectToHost( eHostTypeNetwork, sessionId, url, connectReason );
}

//============================================================================
void HostClientMgr::addSearchSession( VxGUID& sessionId, SearchParams& searchParams )
{
    removeSearchSession( sessionId );
    m_SearchListMutex.lock();
    m_SearchList[sessionId] = searchParams;
    m_SearchListMutex.unlock();
}

//============================================================================
void HostClientMgr::removeSearchSession( VxGUID& sessionId )
{
    m_SearchListMutex.lock();
    auto iter = m_SearchList.find( sessionId );
    if( iter != m_SearchList.end() )
    {
        m_SearchList.erase( iter );
    }

    m_SearchListMutex.unlock();
}

//============================================================================
void HostClientMgr::removeSession( VxGUID& sessionId, EConnectReason connectReason )
{
    if( isAnnounceConnectReason( connectReason ) )
    {
        removeSearchSession( sessionId );
    }

    HostClientSearchMgr::removeSession( sessionId, connectReason );
}

//============================================================================
void HostClientMgr::onContactDisconnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    m_ServerList.removeGuid( onlineId );
    removeContact( onlineId );
}

//============================================================================
void HostClientMgr::onConnectToHostSuccess( EHostType hostType, VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    if( hostType == eHostTypeNetwork &&
        ( connectReason == eConnectReasonChatRoomSearch ||
            connectReason == eConnectReasonGroupSearch ||
            connectReason == eConnectReasonRandomConnectSearch ) )
    {
        m_SearchListMutex.lock();
        auto iter = m_SearchList.find( sessionId );
        if( iter != m_SearchList.end() )
        {
            PktHostSearchReq searchReq;
            bool result = iter->second.addToBlob( searchReq.getBlobEntry() );
            searchReq.calculateLength();
            if( result && searchReq.isValidPkt() )
            {
                // BRJ temporary for debugging
                // TODO REMOVE
                searchReq.setIsLoopback( true );
                m_Plugin.txPacket( onlineId, sktBase, &searchReq, false, ePluginTypeNetworkHost );
            }
            else
            {
                LogMsg( LOG_VERBOSE, "HostServerMgr m_PktHostAnnounce is invalid" );
            }

            m_SearchList.erase( iter );
        }

        m_SearchListMutex.unlock();
        // not done with connection.. wait for search results
        // m_Engine.getConnectionMgr().doneWithConnection( sessionId, onlineId, this, connectReason );
    }
    else
    {
        HostBaseMgr::onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason );
    }
}
