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

#include "PluginClientPeerUser.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PConnectList.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginClientPeerUser::PluginClientPeerUser( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBaseHostClient( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeClientPeerUser );
}

//============================================================================
void PluginClientPeerUser::fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : "";
    m_HostClientMgr.fromGuiAnnounceHost( hostType, sessionId, url );
}

//============================================================================
void PluginClientPeerUser::fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, const char * ptopUrl )
{
    std::string url = ptopUrl ? ptopUrl : "";
    m_HostClientMgr.fromGuiJoinHost( hostType, sessionId, url );
}

//============================================================================
void PluginClientPeerUser::fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable )
{
    m_HostClientMgr.fromGuiSearchHost( hostType, searchParams, enable );
}

//============================================================================
void PluginClientPeerUser::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginClientPeerUser got join request" );
}

//============================================================================
void PluginClientPeerUser::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginClientPeerUser got join reply" );
    m_HostClientMgr.onPktHostJoinReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginClientPeerUser::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginClientPeerUser got search reply" );
    m_HostClientMgr.onPktHostSearchReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginClientPeerUser::onPktPluginSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginClientPeerUser got plugin setting reply" );
    m_HostClientMgr.onPktPluginSettingReply( sktBase, pktHdr,  netIdent );
}

//============================================================================
void PluginClientPeerUser::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginClientPeerUser got join offer request" );
}

//============================================================================
void PluginClientPeerUser::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginClientPeerUser got join offer reply" );
}
