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

#include "PluginGroupClient.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PConnectList.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginGroupClient::PluginGroupClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBaseHostClient( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeClientGroup );
}

//============================================================================
void PluginGroupClient::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : "";
    m_HostClientMgr.fromGuiAnnounceHost( hostType, sessionId, url );
}

//============================================================================
void PluginGroupClient::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : "";
    m_HostClientMgr.fromGuiJoinHost( hostType, sessionId, url );
}

//============================================================================
void PluginGroupClient::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    m_HostClientMgr.fromGuiSearchHost( hostType, searchParams, enable );
}

//============================================================================
void PluginGroupClient::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginGroupClient got join request" );
}

//============================================================================
void PluginGroupClient::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginGroupClient got join reply" );
    m_HostClientMgr.onPktHostJoinReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginGroupClient::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginGroupClient got search reply" );
    m_HostClientMgr.onPktHostSearchReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginGroupClient::onPktPluginSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginGroupClient got plugin setting reply" );
    m_HostClientMgr.onPktPluginSettingReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginGroupClient::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginGroupClient got join offer request" );
}

//============================================================================
void PluginGroupClient::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginGroupClient got join offer reply" );
}
