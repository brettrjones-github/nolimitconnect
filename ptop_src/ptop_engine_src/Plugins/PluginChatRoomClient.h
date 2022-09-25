#pragma once
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

#include "PluginBaseMultimedia.h"
#include "PluginSessionMgr.h"
#include "VoiceFeedMgr.h"
#include "VideoFeedMgr.h"

#include "HostClientMgr.h"

#include <CoreLib/VxGUIDList.h>

// ideally chat room client would be derived from PluginBaseClient but causes inheritance conflict
// so mostly contains duplicate code of PluginBaseClient

class ConnectionMgr;

class PluginChatRoomClient : public PluginBaseMultimedia
{
public:
	PluginChatRoomClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
	virtual ~PluginChatRoomClient() = default;

    virtual EAppModule			getAppModule( void ) override { return eAppModuleChatRoomClient; }

    //=== hosting ===//
    virtual void				fromGuiAnnounceHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiUnJoinHost( EHostType hostType, VxGUID& sessionId, std::string& ptopUrl ) override;
    virtual void				fromGuiSearchHost( EHostType hostType, SearchParams& searchParams, bool enable ) override;

protected:

    virtual void				onPktHostJoinReq                ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostJoinReply              ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostSearchReply            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostOfferReq               ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktHostOfferReply             ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    //=== vars ===//
    HostClientMgr               m_HostClientMgr;
};

