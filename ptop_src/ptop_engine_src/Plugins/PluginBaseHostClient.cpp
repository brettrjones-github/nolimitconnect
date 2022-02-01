//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "PluginBaseHostClient.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <NetLib/VxPeerMgr.h>

#include <CoreLib/VxPtopUrl.h>
#include <PktLib/SearchParams.h>
#include <PktLib/PktsHostJoin.h>

//============================================================================
PluginBaseHostClient::PluginBaseHostClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
    : PluginBase( engine, pluginMgr, myIdent, pluginType )
    , m_ConnectionMgr(engine.getConnectionMgr())
    , m_HostClientMgr(engine, pluginMgr, myIdent, *this)
{
}

//============================================================================
void PluginBaseHostClient::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    std::string url = !ptopUrl.empty() ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        VxGUID hostGuid;
        //EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( hostType, url.c_str(), hostGuid, this);
    }
    else
    {
        m_Engine.getToGui().toGuiHostAnnounceStatus( hostType, sessionId, eHostAnnounceInvalidUrl );
    }
}

//============================================================================
void PluginBaseHostClient::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    std::string url = !ptopUrl.empty() ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        VxGUID hostGuid;
        //EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( hostType, url.c_str(), hostGuid, this);
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinInvalidUrl );
    }
}

//============================================================================
void PluginBaseHostClient::fromGuiLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    std::string url = !ptopUrl.empty() ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        sendLeaveHost( hostType, sessionId, url );
    }
}

//============================================================================
void PluginBaseHostClient::fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl )
{
    std::string url = !ptopUrl.empty() ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        VxGUID hostGuid;
        //EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( hostType, url.c_str(), hostGuid, this);
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, sessionId, eHostJoinInvalidUrl );
    }
}

//============================================================================
void PluginBaseHostClient::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    std::string url = searchParams.getSearchUrl();
    if( !url.empty() )
    {
        VxGUID hostGuid; // TODO
        //EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( hostType, url.c_str(), hostGuid, this);
    }
    else
    {
        m_Engine.getToGui().toGuiHostSearchStatus( hostType, searchParams.getSearchSessionId(), eHostSearchInvalidUrl );
        m_Engine.getToGui().toGuiHostSearchComplete( hostType, searchParams.getSearchSessionId() );
    }
}

//============================================================================
bool PluginBaseHostClient::fromGuiRequestPluginThumb( VxNetIdent* netIdent, VxGUID& thumbId )
{
    if( netIdent && thumbId.isVxGUIDValid() )
    {
        VxSktBase* sktBase = 0;
        m_PluginMgr.pluginApiSktConnectTo( getPluginType(), netIdent, 0, &sktBase );
        if( sktBase && sktBase->isConnected() )
        {
            // the netIdent from gui is not the same one as in big list
            BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( netIdent->getMyOnlineId() );
            if( bigListInfo )
            {
                return ptopEngineRequestPluginThumb( sktBase, bigListInfo->getVxNetIdent(), thumbId );
            }
        }
    }

    return false;
}

//============================================================================
bool PluginBaseHostClient::ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId )
{
    return m_ThumbXferMgr.requestPluginThumb( sktBase, netIdent, thumbId );
}

//============================================================================
void PluginBaseHostClient::onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getHostedListMgr().onPktHostInfoReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostClient::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieInfoReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostClient::onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieAnnReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostClient::onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieSearchReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostClient::onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieMoreReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostClient::sendLeaveHost( EHostType hostType, VxGUID& sessionId, std::string& hostUrl )
{
    VxPtopUrl ptopUrl( hostUrl );
    if( ptopUrl.isValid() )
    {
        GroupieId groupieId( m_Engine.getMyOnlineId(), ptopUrl.getOnlineId(), hostType );
        sendLeaveHost( groupieId );
    }
}

//============================================================================
bool PluginBaseHostClient::sendLeaveHost( GroupieId& groupieId )
{
    bool leaveSent{ false };
    VxGUID connectId;
    if( m_Engine.getOnlineListMgr().findConnection( groupieId, connectId ) )
    {
        PktHostLeaveReq leaveReq;
        leaveReq.setHostType( groupieId.getHostType() );
        leaveReq.setPluginType( HostTypeToClientPlugin( groupieId.getHostType() ) );
        if( m_Engine.getMyOnlineId() == groupieId.getHostedOnlineId() )
        {
            // is ourself
            leaveSent = txPacket( m_Engine.getMyOnlineId(), m_Engine.getSktLoopback(), &leaveReq );
        }
        else
        {
            m_Engine.getPeerMgr().lockSktList();
            VxSktBase* sktBase = m_Engine.getPeerMgr().findSktBase( connectId );
            if( sktBase )
            {
                leaveSent = txPacket( m_Engine.getMyOnlineId(), sktBase, &leaveReq );
            }

            m_Engine.getPeerMgr().unlockSktList();
        }
    }

    return leaveSent;
}