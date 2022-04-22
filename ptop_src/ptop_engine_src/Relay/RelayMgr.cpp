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


#include "RelayMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>

#include <PktLib/PktsRelay.h>

//============================================================================
RelayMgr::RelayMgr( P2PEngine& engine )
	: m_Engine( engine )
{
}

//============================================================================
bool RelayMgr::handleRelayPkt( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	if( pktHdr->getDestOnlineId() == m_Engine.getMyOnlineId() )
	{
		// not a pkt that needs relay
		return false;
	}

	VxGUID srcOnlineId = pktHdr->getSrcOnlineId();
	if( !isJoinedToRelayHost( srcOnlineId ) )
	{
		sendRelayError( srcOnlineId, sktBase, eRelayErrSrcNotJoined );
		return true;
	}

	VxGUID destOnlineId = pktHdr->getDestOnlineId();
	if( !isJoinedToRelayHost( destOnlineId ) )
	{
		sendRelayError( srcOnlineId, sktBase, eRelayErrDestNotJoined );
		return true;
	}

	VxSktBase* sktBaseRelay = m_Engine.getConnectIdListMgr().findRelayMemberConnection( destOnlineId );
	if( !sktBaseRelay )
	{
		sendRelayError( srcOnlineId, sktBase, eRelayErrUserNotOnline );
		return true;
	}

	if( !sktBaseRelay->txPacket( destOnlineId, pktHdr ) )
	{
		sendRelayError( srcOnlineId, sktBase, eRelayErrUserNotOnline );
		return true;
	}

	return true;
}

//============================================================================
bool RelayMgr::isJoinedToRelayHost( VxGUID& onlineId )
{
	return m_Engine.getHostJoinMgr().isUserJoinedToRelayHost( onlineId );
}

//============================================================================
bool RelayMgr::sendRelayError( VxGUID& onlineId, VxSktBase* sktBase, ERelayErr relayErr )
{
	PktRelayConnectToUserReply pktReply;
	pktReply.setRelayError( relayErr );

	return 0 == sktBase->txPacket( onlineId, &pktReply );
}

//============================================================================
void RelayMgr::onPktRelayConnectToUserReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktRelayConnectToUserReply* pktReq = (PktRelayConnectToUserReply*)pktHdr;
	LogMsg( LOG_VERBOSE, "onPktRelayConnectToUserReply err %s", DescribeRelayError( pktReq->getRelayError() ) );
}