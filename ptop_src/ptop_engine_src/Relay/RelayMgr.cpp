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
	else if( sktBaseRelay->getIsPeerPktAnnSet() && sktBaseRelay->getPeerOnlineId() != destOnlineId )
	{
		LogMsg( LOG_ERROR, "handleRelayPkt wrong socket found" );
		sendRelayError( srcOnlineId, sktBase, eRelayErrUserNotOnline );
		return true;
	}

	
	if( 0 != sktBaseRelay->txPacket( destOnlineId, pktHdr ) )
	{
        LogMsg( LOG_VERBOSE, "handleRelayPkt FAILED sent relay pkt %s srcId %s %s destId %s %s", pktHdr->describePkt().c_str(),
			srcOnlineId.toOnlineIdString().c_str(), sktBase->getPeerOnlineName().c_str(), destOnlineId.toOnlineIdString().c_str(), sktBaseRelay->getPeerOnlineName().c_str() );
		sendRelayError( srcOnlineId, sktBase, eRelayErrUserNotOnline );
		return true;
	}

    LogMsg( LOG_VERBOSE, "handleRelayPkt sent relay pkt %s srcId %s %s destId %s %s", pktHdr->describePkt().c_str(),
		srcOnlineId.toOnlineIdString().c_str(), sktBase->getPeerOnlineName().c_str(), destOnlineId.toOnlineIdString().c_str(), sktBaseRelay->getPeerOnlineName().c_str() );
	return true;
}

//============================================================================
bool RelayMgr::isJoinedToRelayHost( VxGUID& onlineId )
{
	return m_Engine.getHostJoinMgr().isUserJoinedToRelayHost( onlineId );
}

//============================================================================
bool RelayMgr::requestRelayConnection( VxSktBase* sktBase, GroupieInfo& groupieInfo )
{
    bool sentAnn{false};
    VxGUID destOnlineId = groupieInfo.getGroupieOnlineId();
    if(destOnlineId.isVxGUIDValid())
    {
        PktAnnounce myPktAnn;
        m_Engine.copyMyPktAnnounce( myPktAnn );
        myPktAnn.setIsPktAnnReplyRequested( true );
        myPktAnn.setIsPktAnnRevConnectRequested( false );
        myPktAnn.setIsPktAnnStunRequested( false );
		VxNetIdent* netIdent = m_Engine.getBigListMgr().findNetIdent( groupieInfo.getGroupieOnlineId() );
		if( netIdent )
		{
			myPktAnn.setMyFriendshipToHim( netIdent->getMyFriendshipToHim() );
		}
		else
		{
			myPktAnn.setMyFriendshipToHim( eFriendStateAnonymous );
		}


        sentAnn = 0 == sktBase->txPacket( destOnlineId, &myPktAnn );
    }

    return sentAnn;
}

//============================================================================
bool RelayMgr::sendRequestedReplyPktAnnIfNeeded( PktAnnounce* hisPktAnn, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	if( hisPktAnn && hisPktAnn->getIsPktAnnReplyRequested() )
	{
		hisPktAnn->setIsPktAnnReplyRequested( false );

		PktAnnounce myPktAnn;
		m_Engine.copyMyPktAnnounce( myPktAnn );
		myPktAnn.setIsPktAnnReplyRequested( false );
		myPktAnn.setIsPktAnnRevConnectRequested( false );
		myPktAnn.setIsPktAnnStunRequested( false );
		myPktAnn.setMyFriendshipToHim( netIdent->getMyFriendshipToHim() );
		int sentAnn =sktBase->txPacket( netIdent->getMyOnlineId(), &myPktAnn );
		if( 0 != sentAnn )
		{
			LogMsg( LOG_VERBOSE, "ERROR %d RelayMgr::sendRequestedReplyPktAnn %s", sentAnn, netIdent->getOnlineName() );
			sktBase->closeSkt( eSktClosePktAnnSendFail );
			return false;
		}
	}

	return true;
}

//============================================================================
void RelayMgr::onRelayPktAnnounce( PktAnnounce* pktAnn, VxSktBase* sktBase, VxNetIdent* netIdent )
{
	LogMsg( LOG_VERBOSE, "RelayMgr::onRelayPktAnnounce %s", netIdent->getOnlineName() );
}

//============================================================================
bool RelayMgr::sendRelayError( VxGUID& onlineId, VxSktBase* sktBase, ERelayErr relayErr )
{
	PktRelayUserDisconnect pktReply;
	pktReply.setSrcOnlineId( m_Engine.getMyOnlineId() );
	pktReply.setUserOnlineId( onlineId );
	pktReply.setHostOnlineId( m_Engine.getMyOnlineId() );
	//pktReply.setRelayError( relayErr );

	return 0 == sktBase->txPacket( onlineId, &pktReply );
}
