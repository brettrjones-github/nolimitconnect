//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "PluginChatRoomClient.h"
#include "P2PSession.h"
#include "PluginMgr.h"

#include <PktLib/PktsVideoFeed.h>
#include <PktLib/PktsMultiSession.h>
#include <PktLib/PktsTodGame.h>

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
PluginChatRoomClient::PluginChatRoomClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBaseMultimedia( engine, pluginMgr, myIdent )
, m_HostClientMgr(engine, pluginMgr, myIdent, *this)
{
	setPluginType( ePluginTypeChatRoomClient );
}

//============================================================================
void PluginChatRoomClient::fromGuiJoinHost( EHostType hostType, const char * ptopUrl )
{
    m_HostClientMgr.fromGuiJoinHost( hostType, ptopUrl );
    /*
    if( !ptopUrl && hostType == eHostTypeUnknown )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
        return;
    }

    EConnectReason connectReason = eConnectReasonChatRoomJoin;
    std::string url = ptopUrl ? ptopUrl : m_ConnectionMgr.getDefaultHostUrl( hostType );
    if( !url.empty() )
    {
        VxGUID hostGuid;
        EHostJoinStatus joinStatus = m_ConnectionMgr.lookupOrQueryId( url.c_str(), hostGuid, this, connectReason);
        if( eHostJoinQueryIdSuccess == joinStatus )
        {
            // no need for id query.. just request connection
            onUrlActionQueryIdSuccess( url, hostGuid, connectReason );
        }
        else if( eHostJoinQueryIdInProgress == joinStatus )
        {
            // manager is attempting to query id
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinQueryIdInProgress );
        }
        else
        {
            m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
        }
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( hostType, eHostJoinInvalidUrl );
    }
    */
}

/*
//============================================================================
void PluginChatRoomClient::onUrlActionQueryIdSuccess( std::string& url, VxGUID& onlineId, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnecting );
        m_ConnectionMgr.requestConnection( url, onlineId, this, eConnectReasonChatRoomJoin );
    }
}

//============================================================================
void PluginChatRoomClient::onUrlActionQueryIdFail( std::string& url, ERunTestStatus testStatus, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinQueryIdFailed, DescribeRunTestStatus( testStatus ) );
    }
}

//============================================================================
bool PluginChatRoomClient::onContactConnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSendingJoinRequest );
        sendJoinRequest( sktBase );
    }

    return false;
}

//============================================================================
void PluginChatRoomClient::onConnectRequestFail( VxGUID& onlineId, EConnectStatus connectStatus, EConnectReason connectReason )
{
    if( eConnectReasonChatRoomJoin == connectReason )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinConnectFailed, DescribeConnectStatus( connectStatus ) );
    }
}

//============================================================================
void PluginChatRoomClient::onContactDisconnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{

}

//============================================================================
void PluginChatRoomClient::sendJoinRequest( VxSktBase* sktBase )
{

}
*/


//============================================================================
void PluginChatRoomClient::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got join request" );
}

//============================================================================
void PluginChatRoomClient::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got join reply" );
}

//============================================================================
void PluginChatRoomClient::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got join offer request" );
}

//============================================================================
void PluginChatRoomClient::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got join offer reply" );
}
