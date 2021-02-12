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

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <PktLib/PktsVideoFeed.h>
#include <PktLib/PktsMultiSession.h>
#include <PktLib/PktsTodGame.h>

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
void PluginChatRoomClient::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : "";
    m_HostClientMgr.fromGuiAnnounceHost( hostType, sessionId, url );
}

//============================================================================
void PluginChatRoomClient::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : "";
    m_HostClientMgr.fromGuiJoinHost( hostType, sessionId, url );
}

//============================================================================
void PluginChatRoomClient::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    m_HostClientMgr.fromGuiSearchHost( hostType, searchParams, enable );
}

//============================================================================
void PluginChatRoomClient::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got join request" );
}

//============================================================================
void PluginChatRoomClient::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got join reply" );
    m_HostClientMgr.onPktHostJoinReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginChatRoomClient::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomClient got search reply" );
    m_HostClientMgr.onPktHostSearchReply( sktBase, pktHdr,  netIdent );
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
