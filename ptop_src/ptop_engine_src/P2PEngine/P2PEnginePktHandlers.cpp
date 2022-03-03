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

#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>
#include <ptop_src/ptop_engine_src/Membership/Membership.h>

#include <NetLib/VxSktCrypto.h>
#include <NetLib/VxSktBase.h>

#include <PktLib/PktTcpPunch.h>
#include <PktLib/PktsPing.h>
#include <PktLib/PktsMembership.h>

#include <memory.h>

//============================================================================
void P2PEngine::onPktUnhandled( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogMsg( LOG_ERROR, "onPktUnhandled pkt type %d", pktHdr->getPktType() );
}

//============================================================================
void P2PEngine::onPktInvalid( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogMsg( LOG_ERROR, "onPktInvalid pkt type %d", pktHdr->getPktType() );
}

//============================================================================
void P2PEngine::onPktAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	PktAnnounce * pkt = (PktAnnounce *)pktHdr;
	VxGUID contactOnlineId = pkt->getMyOnlineId();
	if( contactOnlineId == m_PktAnn.getMyOnlineId() )
	{
		// it is ourself
        LogMsg( LOG_ERROR, "onPktAnnounce Cannot send a packet to ourself  " );
        std::string rmAddr = sktBase->getRemoteIpAddress() ? sktBase->getRemoteIpAddress() : "";
        std::string ourAddr = getNetStatusAccum().getIpAddress();
        if( rmAddr.empty() || sktBase->getRemoteIpAddress() != ourAddr )
        {
            // remote attack.. serious
            hackerOffense( eHackerLevelSevere, eHackerReasonPktOnlineIdMeFromAnotherIp, pkt, "rxed same as our online id from another " );
            sktBase->closeSkt( eSktClosePktOnlineIdMeFromAnotherIp );
        }
        else
        {
            hackerOffense( eHackerLevelSuspicious, eHackerReasonPktOnlineIdMeFromMyIp, pkt, "rxed same as our online from our address " );
            sktBase->closeSkt( eSktClosePktOnlineIdMeFromMyIp );
        }

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
	else if( !sktBase->getIsPeerPktAnnSet() )
	{
		isFirstAnnounce = true;
	}

	pkt->reversePermissions();

	// TODO validate if really nearby

	BigListInfo * bigListInfo = 0;
	EPktAnnUpdateType updateType = m_BigListMgr.updatePktAnn(	pkt,				// announcement pkt received
																&bigListInfo );		// return pointer to all we know about this contact
	if( !bigListInfo->isValidNetIdent() )
	{
		LogMsg( LOG_ERROR, "PktAnnounce updatePktAnn INVALID" );
		return;
	}

	if( ePktAnnUpdateTypeIgnored == updateType )
	{
		LogModule( eLogConnect, LOG_VERBOSE, "Ignoring %s ip %s id %s",
			pkt->getOnlineName(),
            sktBase->getRemoteIp().c_str(),
			contactOnlineId.toHexString().c_str() );
		// TODO do not close connection if is our group relay connection
		// if( !m_PktAnn.requiresRelay() )
		{
			m_NetConnector.closeConnection( eSktCloseUserIgnored, contactOnlineId, sktBase );
		}

		return;
	}

	if( pkt->getIsPktAnnReplyRequested() )
	{
        LogModule( eLogConnect, LOG_VERBOSE, "P2PEngine::onPktAnnounce from %s at %s relay %d reply requested", pkt->getOnlineName(), sktBase->getRemoteIp().c_str(), pkt->requiresRelay() );
		if( !m_NetConnector.sendMyPktAnnounce( pkt->getMyOnlineId(),
				sktBase,
				false,
				false,
				false ) )
		{
			LogModule( eLogConnect, LOG_VERBOSE, "P2PEngine::sendMyPktAnnounce failed to %s at %s reply requested", pkt->getOnlineName(), sktBase->getRemoteIp().c_str() );
			sktBase->closeSkt( eSktClosePktAnnSendFail );
			return;
		}
	}

    if( sktBase->setPeerPktAnn( *pkt ) )
    {
		getConnectList().addConnection( sktBase, bigListInfo, ( ePktAnnUpdateTypeNewContact == updateType ) );
        getConnectionMgr().onSktConnectedWithPktAnn( sktBase, bigListInfo );
    }
	else
	{
		getConnectList().addConnection( sktBase, bigListInfo, ( ePktAnnUpdateTypeNewContact == updateType ) );
	}

	if( pkt->getTTL() > 0 )
	{
		pkt->setTTL( pkt->getTTL() - 1 );
		pkt->setIsPktAnnReplyRequested( false );
		pkt->setIsPktAnnStunRequested( false );
	}

	if( pkt->getIsPktAnnRevConnectRequested() )
	{
		LogModule( eLogConnect, LOG_VERBOSE, "P2PEngine::onPktAnnounce from %s at %s reverse connect requested", pkt->getOnlineName(), sktBase->getRemoteIp().c_str() );
		VxSktBase * poNewSkt = 0;
		m_NetConnector.directConnectTo( pkt->getConnectInfo(), &poNewSkt, eConnectReasonReverseConnectRequested );
		if( poNewSkt )
		{
			LogModule( eLogConnect, LOG_VERBOSE, "sendMyPktAnnounce 6" );
            if( m_NetConnector.sendMyPktAnnounce(   pkt->getMyOnlineId(),
													poNewSkt,
                                                    true,
                                                    false,
                                                    false ) )
            {
				if( poNewSkt->setPeerPktAnn( *pkt ) )
				{
					getConnectList().addConnection( poNewSkt, bigListInfo, ( ePktAnnUpdateTypeContactIsSame == updateType ) );
					getConnectionMgr().onSktConnectedWithPktAnn( sktBase, bigListInfo );
				}
				else
				{
					getConnectList().addConnection( poNewSkt, bigListInfo, ( ePktAnnUpdateTypeContactIsSame == updateType ) );
				}
            }
            else
            {
                poNewSkt->closeSkt( eSktClosePktAnnSendFail );
				return;
            }
		}
	}
	else
	{
		PktPingReq pktPingReq;
		pktPingReq.setSrcOnlineId( m_PktAnn.getSrcOnlineId() );
		if( 0 != sktBase->txPacket( bigListInfo->getMyOnlineId(), &pktPingReq ) )
		{
			getConnectList().onConnectionLost( sktBase );
            sktBase->closeSkt(eSktClosePktPingReqSendFail);
			return;
		}
	}

    if( isFirstAnnounce )
    {
		updateOnFirstConnect( sktBase, bigListInfo, false );
        onFirstPktAnnounce( pkt, sktBase, bigListInfo );
    }
}

//============================================================================
void P2PEngine::onPktAnnList( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAnnList" );
}
	
//============================================================================
void P2PEngine::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPluginOfferReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPluginOfferReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktChatReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktChatReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktChatReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVoiceReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVoiceReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVideoFeedReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVideoFeedStatus" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVideoFeedPic" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVideoFeedPicChunk" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktVideoFeedPicAck" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileGetReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileGetReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileSendReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileSendReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileListReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileListReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileChunkReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileChunkReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileSendCompleteReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileSendCompleteReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileGetCompleteReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileGetCompleteReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileShareErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileShareErr" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetGetReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetGetReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetSendReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetSendReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetChunkReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetChunkReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetGetCompleteReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetGetCompleteReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetSendCompleteReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetSendCompleteReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktAssetXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktAssetXferErr" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMultiSessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktMultiSessionReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMultiSessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktMultiSessionReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktSessionStartReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktSessionStartReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktSessionStopReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktSessionStopReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelayDisconnect" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayConnectReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelayConnectReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayConnectReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelayConnectReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelaySessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelaySessionReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelaySessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelaySessionReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayTestReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelayTestReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktRelayTestReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktRelayTestReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktMyPicSendReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktMyPicSendReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktWebServerPicChunkTx" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktWebServerPicChunkAck" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktWebServerGetChunkTx" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktWebServerGetChunkAck" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktWebServerPutChunkTx" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktWebServerPutChunkAck" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTodGameStats( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktTodGameStats" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTodGameAction( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktTodGameAction" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTodGameValue( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktTodGameValue" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktTcpPunch( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktTcpPunch" );

	PktTcpPunch * pktPunch = ( PktTcpPunch * )pktHdr;
	VxSktBase * poNewSkt = 0;
	if( 0 == m_NetConnector.directConnectTo( pktPunch->m_ConnectInfo, &poNewSkt, eConnectReasonPktTcpPunch ) )
	{
		LogMsg( LOG_INFO, "P2PEngine:: TcpPunch SUCCESS" );
		if( 0 != poNewSkt )
		{
			LogMsg( LOG_INFO, "sendMyPktAnnounce 7" ); 
			m_NetConnector.sendMyPktAnnounce(	pktPunch->m_ConnectInfo.getMyOnlineId(), 
												poNewSkt,
												true,
												false,
												false );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "P2PEngine:: TcpPunch FAIL" );
	}
}

//============================================================================
void P2PEngine::onPktPingReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPingReq" );

	PktPingReq * pktPingReq = (PktPingReq *)pktHdr;
	PktPingReply pktPingReply;
	pktPingReply.setSrcOnlineId( m_PktAnn.getMyOnlineId() );
	pktPingReply.setTimestamp( pktPingReq->getTimestamp() );
	sktBase->txPacket( pktPingReq->getSrcOnlineId(), &pktPingReply );
}

//============================================================================
void P2PEngine::onPktPingReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPingReply" );

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
	
	LogMsg( LOG_DEBUG, "Ping %s Time ms %d", onlineName.c_str(), timeDiff );
}


//============================================================================
void P2PEngine::onPktImAliveReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktImAliveReq" );

	sktBase->setLastImAliveTimeMs(  GetGmtTimeMs() );
	PktImAliveReq * pktImAliveReq = (PktImAliveReq *)pktHdr;
	PktImAliveReply pktImAliveReply;
	pktImAliveReply.setSrcOnlineId( m_PktAnn.getMyOnlineId() );

	sktBase->txPacket( pktImAliveReq->getSrcOnlineId(), &pktImAliveReply );
}

//============================================================================
void P2PEngine::onPktImAliveReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktImAliveReply" );

	sktBase->setLastImAliveTimeMs(  GetGmtTimeMs() );
}

//============================================================================
void P2PEngine::onPktPluginSettingReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPluginSettingReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktPluginSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPluginSettingReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSettingReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSettingReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSettingReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSettingChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSettingChunkReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSettingChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSettingChunkReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbAvatarReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbAvatarReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbAvatarReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbAvatarReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbAvatarChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbAvatarChunkReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbAvatarChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbAvatarChunkReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbFileReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbFileReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbFileChunkReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbFileChunkReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostAnnounce" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobSendReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobSendReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobChunkReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobChunkReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobSendCompleteReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobSendCompleteReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktBlobXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktBlobXferErr" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostJoinReq" );
    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostJoinReply" );
    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostLeaveReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostLeaveReq" );
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostLeaveReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostLeaveReply" );
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostUnJoinReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostUnJoinReq" );
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostUnJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostUnJoinReply" );
	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostSearchReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostSearchReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostSearchReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostOfferReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostOfferReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFriendOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFriendOfferReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFriendOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFriendOfferReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbGetReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbGetReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSendReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSendReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbChunkReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbChunkReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbGetCompleteReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbGetCompleteReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSendCompleteReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbSendCompleteReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktThumbXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktThumbXferErr" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

// offers
//============================================================================
void P2PEngine::onPktOfferSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferSendReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktOfferSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferSendReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktOfferChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferChunkReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktOfferChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferChunkReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktOfferSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferSendCompleteReq" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktOfferSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferSendCompleteReply" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktOfferXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktOfferXferErr" );

    m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktPushToTalkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPushToTalkReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktPushToTalkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktPushToTalkReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktMembershipReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktMembershipReq" );

	VxNetIdent* netIdent = pktHdr->getSrcOnlineId() == getMyOnlineId() ? getMyNetIdent() : m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
	if( netIdent && !netIdent->isIgnored() && sktBase && sktBase->isConnected() )
	{
		PktAnnounce pktAnn;
		copyMyPktAnnounce( pktAnn );
		EFriendState myFriendshipToHim = netIdent->getMyFriendshipToHim();
		PktMembershipReply pktReply;
		pktReply.setCanPushToTalk( pktAnn.getPluginPermission( ePluginTypePushToTalk ) != eFriendStateIgnore && myFriendshipToHim >= pktAnn.getPluginPermission( ePluginTypePushToTalk ) );
		pktReply.setHostMembership( eHostTypeNetwork, getMembershipState( pktAnn, netIdent, ePluginTypeHostNetwork, myFriendshipToHim ) );
		pktReply.setHostMembership( eHostTypeConnectTest, getMembershipState( pktAnn, netIdent, ePluginTypeHostConnectTest, myFriendshipToHim ) );
		pktReply.setHostMembership( eHostTypeGroup, getMembershipState( pktAnn, netIdent, ePluginTypeHostGroup, myFriendshipToHim ) );
		pktReply.setHostMembership( eHostTypeChatRoom, getMembershipState( pktAnn, netIdent, ePluginTypeHostChatRoom, myFriendshipToHim ) );
        pktReply.setHostMembership( eHostTypeRandomConnect, getMembershipState( pktAnn, netIdent, ePluginTypeHostRandomConnect, myFriendshipToHim ) );

        sktBase->txPacket( netIdent->getMyOnlineId(), &pktReply );
	}
}

//============================================================================
void P2PEngine::onPktMembershipReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktMembershipReply" );

	PktMembershipReply* pktReply = ( PktMembershipReply* )pktHdr;
	if( pktReply && pktReply->isValidPkt() )
	{
		VxNetIdent* netIdent = pktHdr->getSrcOnlineId() == getMyOnlineId() ? getMyNetIdent() : m_BigListMgr.findBigListInfo( pktHdr->getSrcOnlineId() );
		if( netIdent && !netIdent->isIgnored() && sktBase && sktBase->isConnected() )
		{
			MembershipAvailable membership;
			membership.setCanPushToTalk( pktReply->getCanPushToTalk() );
			membership.setMembershipState( eHostTypeNetwork, pktReply->getHostMembership( eHostTypeNetwork ) );
			membership.setMembershipState( eHostTypeConnectTest, pktReply->getHostMembership( eHostTypeConnectTest ) );
			membership.setMembershipState( eHostTypeGroup, pktReply->getHostMembership( eHostTypeGroup ) );
			membership.setMembershipState( eHostTypeChatRoom, pktReply->getHostMembership( eHostTypeChatRoom ) );
			membership.setMembershipState( eHostTypeRandomConnect, pktReply->getHostMembership( eHostTypeRandomConnect ) );
		}
	}
}

//============================================================================
EMembershipState P2PEngine::getMembershipState( PktAnnounce& myPktAnn, VxNetIdent* netIdent, EPluginType pluginType, EFriendState myFriendshipToHim )
{
	EMembershipState membershipState{ eMembershipStateNone };
	if( myFriendshipToHim == eFriendStateIgnore )
	{
		membershipState = eMembershipStateJoinDenied;
	}
	else if( myPktAnn.getPluginPermission( pluginType ) != eFriendStateIgnore )
	{
		if( ePluginTypeHostGroup == pluginType || ePluginTypeHostChatRoom == pluginType )
		{
			// look up joined state from group manager
			PluginBase* pluginBase = m_PluginMgr.findPlugin( ePluginTypeHostGroup );
			if( pluginBase )
			{
				membershipState = pluginBase->getMembershipState( netIdent );
			}
		}
		else
		{
			if( myFriendshipToHim >= myPktAnn.getPluginPermission( pluginType ) )
			{
				membershipState = eMembershipStateJoined;
			}
			else
			{
				membershipState = eMembershipStateCanBeRequested;
			}
		}
	}
	else
	{
		membershipState = eMembershipStateJoinDenied;
	}

	return membershipState;
}

//============================================================================
void P2PEngine::onPktHostInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInfoReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInfoReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInviteAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInviteAnnReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInviteAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInviteAnnReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInviteSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInviteSearchReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInviteSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInviteSearchReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInviteMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInviteMoreReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktHostInviteMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktHostInviteMoreReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieInfoReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieInfoReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieAnnReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieAnnReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieSearchReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieSearchReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieMoreReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktGroupieMoreReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoInfoReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoInfoReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoAnnReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoAnnReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoSearchReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoSearchReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoMoreReq" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}

//============================================================================
void P2PEngine::onPktFileInfoMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogModule( eLogPkt, LOG_VERBOSE, "P2PEngine::onPktFileInfoMoreReply" );

	m_PluginMgr.handleNonSystemPackets( sktBase, pktHdr );
}
