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
    if( 0 == hostReply->getTotalMatches() )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::onPktHostSearchReply no matches" );
        stopHostSearch( hostReply->getHostType(), hostReply->getSearchSessionId(), sktBase, netIdent );
    }
    else
    {
        startHostDetailSession( hostReply, sktBase, netIdent );
    }
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
        m_SearchParamsMutex.lock();
        auto iter = m_SearchParamsList.find( sessionId );
        if( iter != m_SearchParamsList.end() )
        {
            SearchParams& searchParams = iter->second;
            PktHostSearchReq searchReq;
            searchReq.setHostType( searchParams.getHostType() );
            searchReq.setSearchSessionId( sessionId );
            PktBlobEntry& blobEntry = searchReq.getBlobEntry();
            bool result = searchParams.addToBlob( blobEntry );
            searchReq.calcPktLen();
            if( result && searchReq.isValidPkt() )
            {
                // BRJ temporary for debugging
                // TODO REMOVE
                searchReq.setIsLoopback( true );
                if( !m_Plugin.txPacket( onlineId, sktBase, &searchReq, false, ePluginTypeNetworkHost ) )
                {
                    LogModule( eLogHostSearch, LOG_DEBUG, "HostClientMgr::onConnectToHostSuccess failed send PktHostSearchReq" );
                }
            }
            else
            {
                LogMsg( LOG_ERROR, "HostServerMgr PktHostSearchReq is invalid" );
            }

            m_SearchParamsList.erase( iter );
        }

        m_SearchParamsMutex.unlock();
        // not done with connection.. wait for search results
    }
    else
    {
        HostBaseMgr::onConnectToHostSuccess( hostType, sessionId, sktBase, onlineId, connectReason );
    }
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
        if( commErr = eCommErrPluginNotEnabled )
        {
            m_Engine.getToGui().toGuiHostSearchStatus( hostType, netIdent->getMyOnlineId(), eHostSearchPluginDisabled );
        }

        stopHostSearch( hostReply->getHostType(), hostReply->getSearchSessionId(), sktBase, netIdent );
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
        stopHostSearch( hostReply->getHostType(), hostReply->getSearchSessionId(), sktBase, netIdent );
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
            pluginIdReq.setSessionId( sessionId );

            // TODO debug only REMOVE ME
            pluginIdReq.setIsLoopback( true );
            if( m_Plugin.txPacket( netIdent->getMyOnlineId(), sktBase, &pluginIdReq, false, ePluginTypeNetworkHost ) )
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
        stopHostSearch( hostType, sessionId, sktBase, netIdent );
    }

    return result;
}

//============================================================================
void HostClientMgr::stopHostSearch( EHostType hostType, VxGUID& sessionId, VxSktBase * sktBase, VxNetIdent * netIdent )
{
    m_Engine.getToGui().toGuiHostSearchStatus( hostType, netIdent->getMyOnlineId(), eHostSearchCompleted );

    removeSearchSession( sessionId );
    EConnectReason connectReason = getSearchConnectReason(hostType);
    m_Engine.getConnectionMgr().doneWithConnection( sessionId, netIdent->getMyOnlineId(), this, connectReason );
}

//============================================================================
void HostClientMgr::onPktPluginSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktPluginSettingReply* settingReply = ( PktPluginSettingReply* )pktHdr;
    PktBlobEntry& blobEntry = settingReply->getBlobEntry();
    if( 0 != blobEntry.getBlobLen() )
    {
        // extract ident and plugin settings and send to gui
        VxNetIdent hostIdent;
        PluginSetting pluginSetting;
        bool extractResult = hostIdent.extractFromBlob( blobEntry );
        extractResult &= pluginSetting.extractFromBlob( blobEntry );
        if( extractResult )
        {
            m_Engine.getToGui().toGuiHostSearchResult( settingReply->getHostType(), settingReply->getSessionId(), hostIdent, pluginSetting );
        }
        else
        {
            LogMsg( LOG_VERBOSE, "HostClientMgr plugin setting reply extract error" );
        }

        sendNextPluginSettingRequest( settingReply->getHostType(), settingReply->getSessionId(), sktBase, netIdent );
    }
    else
    {
        LogMsg( LOG_VERBOSE, "HostClientMgr plugin setting reply empty" );
        stopHostSearch( settingReply->getHostType(), settingReply->getSessionId(), sktBase, netIdent );
    }
}
