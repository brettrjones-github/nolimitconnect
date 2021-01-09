//============================================================================
// Copyright (C) 2014 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "P2PEngine.h"

#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginMgr.h>
#include <GoTvCore/GoTvP2P/Network/NetConnector.h>

#include <NetLib/VxSktCrypto.h>
#include <NetLib/VxSktBase.h>

#include <PktLib/PktTcpPunch.h>
#include <PktLib/PktsPing.h>

#include <memory.h>

//#define DEBUG_ENGINE_PKTS

//============================================================================
void P2PEngine::onPktUnhandled( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogMsg( LOG_INFO, "onPktUnhandled pkt type %d\n", pktHdr->getPktType() );
}

//============================================================================
void P2PEngine::onPktInvalid( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogMsg( LOG_INFO, "onPktInvalid pkt type %d\n", pktHdr->getPktType() );
}

//============================================================================
void P2PEngine::onPktAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	PktAnnounce * pkt = (PktAnnounce *)pktHdr;
	VxGUID contactOnlineId = pkt->getMyOnlineId();
	if( contactOnlineId == m_PktAnn.getMyOnlineId() )
	{
		// it is ourself
		return;
	}

	bool isFirstAnnounce = false;
	if( false == sktBase->m_TxCrypto.isKeyValid() )
	{
		// setup tx crypto
		isFirstAnnounce = true;
		std::string networkName;
		m_EngineSettings.getNetworkKey( networkName );
		GenerateTxConnectionKey( sktBase, &pkt->m_DirectConnectId, networkName.c_str() );
	}

	pkt->reversePermissions();
	BigListInfo * bigListInfo = 0;
	EPktAnnUpdateType updateType = m_BigListMgr.updatePktAnn(	pkt,				// announcement pkt received
																&bigListInfo );		// return pointer to all we know about this contact
	if( ePktAnnUpdateTypeIgnored == updateType )
	{
		LogMsg( LOG_INFO, "Ignoring %s ip %s id %s\n",
			pkt->getOnlineName(),
			sktBase->getRemoteIp(),
			contactOnlineId.toHexString().c_str() );
		m_NetConnector.closeConnection( contactOnlineId, sktBase );	
		return;
	}

	if( isFirstAnnounce )
	{
		LogMsg( LOG_INFO, "P2PEngine::onPktAnnounce from %s at %s myFriendship %d hisFriendship %d\n",
			pkt->getOnlineName(),
			sktBase->getRemoteIp(),
			bigListInfo->getMyFriendshipToHim(),
			bigListInfo->getHisFriendshipToMe()
			);
	}

	if( pkt->getIsPktAnnReplyRequested() )
	{
		LogMsg( LOG_INFO, "P2PEngine::onPktAnnounce from %s at %s reply requested\n", pkt->getOnlineName(), sktBase->getRemoteIp() ); 
		m_NetConnector.sendMyPktAnnounce( pkt->getMyOnlineId(), 
											sktBase,
											false,
											false,
											false );
	}

    if( sktBase->setPeerPktAnn( *pkt ) )
    {
        getConnectionMgr().onSktConnectedWithPktAnn( sktBase, bigListInfo );
    }

	getConnectList().addConnection( sktBase, bigListInfo, ( ePktAnnUpdateTypeNewContact == updateType ) );

	if( pkt->getIsTopTenRequested() )
	{
		LogMsg( LOG_INFO, "P2PEngine::onPktAnnounce from %s at %s top ten requested\n", pkt->getOnlineName(), sktBase->getRemoteIp() ); 
		pkt->setIsTopTenRequested( false );
		m_ConnectionList.sendMyTop10( sktBase, pkt->getSrcOnlineId() );
	}

	if( pkt->getTTL() )
	{
		pkt->setTTL( pkt->getTTL() - 1 );
		pkt->setIsPktAnnReplyRequested( false );
		pkt->setIsPktAnnStunRequested( false );
	}

	if( pkt->getIsPktAnnRevConnectRequested() )
	{
		LogMsg( LOG_INFO, "P2PEngine::onPktAnnounce from %s at %s reverse connect requested\n", pkt->getOnlineName(), sktBase->getRemoteIp() ); 
		VxSktBase * poNewSkt = 0;
		m_NetConnector.directConnectTo( pkt->getConnectInfo(), &poNewSkt );
		if( poNewSkt )
		{
			LogMsg( LOG_INFO, "sendMyPktAnnounce 6\n" ); 
			m_NetConnector.sendMyPktAnnounce(	pkt->getMyOnlineId(), 
												sktBase,
												true,
												false,
												false );
			getConnectList().addConnection( poNewSkt, bigListInfo, ( ePktAnnUpdateTypeContactIsSame == updateType ) );
		}
	}
	else
	{
		PktPingReq pktPingReq;
		pktPingReq.setSrcOnlineId( m_PktAnn.getSrcOnlineId() );
		if( 0 != sktBase->txPacket( bigListInfo->getMyOnlineId(), &pktPingReq ) )
		{
			getConnectList().onConnectionLost( sktBase );
		}
	}
}

//============================================================================
void P2PEngine::onPktAnnList( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAnnList\n" );
#endif // DEBUG_ENGINE_PKTS
}

//============================================================================
void P2PEngine::onPktLogReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktLogReq\n" );
#endif // DEBUG_ENGINE_PKTS
}

//============================================================================
void P2PEngine::onPktLogReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktLogReply\n" );
#endif // DEBUG_ENGINE_PKTS
}
	
//============================================================================
void P2PEngine::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktPluginOfferReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktPluginOfferReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktChatReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktChatReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktChatReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVoiceReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVoiceReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVideoFeedReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVideoFeedStatus\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVideoFeedPic\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVideoFeedPicChunk\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktVideoFeedPicAck\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileGetReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileGetReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileSendReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileSendReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//virtual void				onPktFindFileReq				( VxSktBase * sktBase, VxPktHdr * pktHdr );
//virtual void				onPktFindFileReply				( VxSktBase * sktBase, VxPktHdr * pktHdr );

//============================================================================
void P2PEngine::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileListReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileListReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileInfoReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileChunkReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileChunkReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileSendCompleteReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileSendCompleteReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileGetCompleteReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileGetCompleteReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileShareErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktFileShareErr\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetSendReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetSendReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetChunkReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetChunkReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetSendCompleteReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetSendCompleteReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktAssetXferErr\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMultiSessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktMultiSessionReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMultiSessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktMultiSessionReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktSessionStartReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktSessionStartReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktSessionStopReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktSessionStopReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayDisconnect\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayConnectReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayConnectReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayConnectReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayConnectReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelaySessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelaySessionReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelaySessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelaySessionReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayTestReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayTestReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayTestReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayTestReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktMyPicSendReq\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktMyPicSendReply\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktWebServerPicChunkTx\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktWebServerPicChunkAck\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktWebServerGetChunkTx\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktWebServerGetChunkAck\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktWebServerPutChunkTx\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktWebServerPutChunkAck\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTodGameStats( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktTodGameStats\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTodGameAction( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktTodGameAction\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTodGameValue( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktTodGameValue\n" );
#endif // DEBUG_ENGINE_PKTS
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTcpPunch( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
//#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktTcpPunch\n" );
//#endif // DEBUG_ENGINE_PKTS
	PktTcpPunch * pktPunch = ( PktTcpPunch * )pktHdr;
	VxSktBase * poNewSkt = 0;
	if( 0 == m_NetConnector.directConnectTo( pktPunch->m_ConnectInfo, &poNewSkt ) )
	{
		LogMsg( LOG_INFO, "P2PEngine:: TcpPunch SUCCESS\n" );
		if( 0 != poNewSkt )
		{
			LogMsg( LOG_INFO, "sendMyPktAnnounce 7\n" ); 
			m_NetConnector.sendMyPktAnnounce(	pktPunch->m_ConnectInfo.getMyOnlineId(), 
												poNewSkt,
												true,
												false,
												false );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "P2PEngine:: TcpPunch FAIL\n" );
	}
}

//============================================================================
void P2PEngine::onPktPingReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktPingReq\n" );
#endif // DEBUG_ENGINE_PKTS
	PktPingReq * pktPingReq = (PktPingReq *)pktHdr;
	PktPingReply pktPingReply;
	pktPingReply.setSrcOnlineId( m_PktAnn.getMyOnlineId() );
	pktPingReply.setTimestamp( pktPingReq->getTimestamp() );
	sktBase->txPacket( pktPingReq->getSrcOnlineId(), &pktPingReply );
}

//============================================================================
void P2PEngine::onPktPingReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktPingReply\n" );
#endif // DEBUG_ENGINE_PKTS
	PktPingReply * pktPingReply = (PktPingReply *)pktHdr;
	int64_t timeDiffTmp = GetGmtTimeMs() - pktPingReply->getTimestamp();
	uint16_t timeDiff = timeDiffTmp > 30000 ? 30000 : (uint16_t)timeDiffTmp;

	std::string onlineName;
	BigListInfo * bigListInfo = m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
	if( 0 != bigListInfo )
	{
		onlineName = bigListInfo->getOnlineName();
		bigListInfo->setPingTimeMs( timeDiff );
	}
	else
	{
		pktHdr->getSrcOnlineId().toHexString( onlineName );
	}
	
	LogMsg( LOG_DEBUG, "Ping %s Time ms %d\n", onlineName.c_str(), timeDiff );
}


//============================================================================
void P2PEngine::onPktImAliveReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktImAliveReq\n" );
#endif // DEBUG_ENGINE_PKTS
	sktBase->setLastImAliveTimeMs(  GetGmtTimeMs() );
	PktImAliveReq * pktImAliveReq = (PktImAliveReq *)pktHdr;
	PktImAliveReply pktImAliveReply;
	pktImAliveReply.setSrcOnlineId( m_PktAnn.getMyOnlineId() );
	//pktImAliveReply.setTimestamp( pktImAliveReq->getTimestamp() );
	sktBase->txPacket( pktImAliveReq->getSrcOnlineId(), &pktImAliveReply );
}

//============================================================================
void P2PEngine::onPktImAliveReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
#ifdef DEBUG_ENGINE_PKTS
	LogMsg( LOG_INFO, "P2PEngine::onPktImAliveReply\n" );
#endif // DEBUG_ENGINE_PKTS
	sktBase->setLastImAliveTimeMs(  GetGmtTimeMs() );
}

