#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include <GuiInterface/IDefs.h>

class GroupieInfo;
class P2PEngine;
class PluginBaseRelay;
class VxSktBase;
class VxPktHdr;
class VxNetIdent;
class VxGUID;

class RelayMgr
{
public:
	RelayMgr( P2PEngine& engine );
	virtual ~RelayMgr() = default;

	bool						handleRelayPkt( VxSktBase* sktBase, VxPktHdr* pktHdr );

	bool						requestRelayConnection( VxSktBase* sktBase, GroupieInfo& groupieInfo ) { return false; }

	virtual void				onPktRelaySessionReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};
	virtual void				onPktRelaySessionReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};
	virtual void				onPktRelayConnect( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};
	virtual void				onPktRelayDisconnect( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) {};

	virtual void				onPktRelayConnectToUserReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	bool						isJoinedToRelayHost( VxGUID& onlineId );
	bool						sendRelayError( VxGUID& onlineId, VxSktBase* sktBase, ERelayErr relayErr );

	//=== vars ====//
	P2PEngine&					m_Engine;

};
