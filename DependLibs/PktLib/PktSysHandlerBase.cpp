//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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
// http://www.gotvptop.net
//============================================================================
 
#include "PktTypes.h"
#include "PktSysHandlerBase.h"

#include <CoreLib/VxDebug.h>

PktHandlerBase::PktHandlerBase()
{
    for( size_t i = 0; i <= sizeof( m_aBaseSysPktFuncTable ) / sizeof( void * ); i++ )
	{
		m_aBaseSysPktFuncTable[ i ] = &PktHandlerBase::onPktUnhandled;
	}

	m_aBaseSysPktFuncTable[ 0 ] = &PktHandlerBase::onPktInvalid;

	m_aBaseSysPktFuncTable[ PKT_TYPE_ANNOUNCE ]							= &PktHandlerBase::onPktAnnounce;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ANN_LIST ]							= &PktHandlerBase::onPktAnnList;

	m_aBaseSysPktFuncTable[ PKT_TYPE_LOG_REQ ]							= &PktHandlerBase::onPktLogReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_LOG_REPLY ]						= &PktHandlerBase::onPktLogReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_OFFER_REQ ]					= &PktHandlerBase::onPktPluginOfferReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_OFFER_REPLY ]				= &PktHandlerBase::onPktPluginOfferReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_CHAT_REQ ]							= &PktHandlerBase::onPktChatReq;

	m_aBaseSysPktFuncTable[ PKT_TYPE_VOICE_REQ ]						= &PktHandlerBase::onPktVoiceReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VOICE_REPLY ]						= &PktHandlerBase::onPktVoiceReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_REQ ]					= &PktHandlerBase::onPktVideoFeedReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_STATUS ]				= &PktHandlerBase::onPktVideoFeedStatus;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_PIC ]					= &PktHandlerBase::onPktVideoFeedPic;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_PIC_CHUNK ]				= &PktHandlerBase::onPktVideoFeedPicChunk;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_PIC_ACK ]				= &PktHandlerBase::onPktVideoFeedPicAck;

	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_START_REQ ]				= &PktHandlerBase::onPktSessionStartReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_START_REPLY ]				= &PktHandlerBase::onPktSessionStartReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_STOP_REQ ]					= &PktHandlerBase::onPktSessionStopReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_STOP_REPLY ]				= &PktHandlerBase::onPktSessionStopReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_REQ ]						= &PktHandlerBase::onPktFileGetReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_REPLY ]					= &PktHandlerBase::onPktFileGetReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_REQ ]					= &PktHandlerBase::onPktFileSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_REPLY ]					= &PktHandlerBase::onPktFileSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_CHUNK_REQ ]					= &PktHandlerBase::onPktFileChunkReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_CHUNK_REPLY ]					= &PktHandlerBase::onPktFileChunkReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_COMPLETE_REQ ]			= &PktHandlerBase::onPktFileGetCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_COMPLETE_REPLY ]			= &PktHandlerBase::onPktFileGetCompleteReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_COMPLETE_REQ ]			= &PktHandlerBase::onPktFileSendCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_COMPLETE_REPLY ]			= &PktHandlerBase::onPktFileSendCompleteReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_FIND_REQ ]					= &PktHandlerBase::onPktFindFileReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_FIND_REPLY ]					= &PktHandlerBase::onPktFindFileReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_LIST_REQ ]					= &PktHandlerBase::onPktFileListReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_LIST_REPLY ]					= &PktHandlerBase::onPktFileListReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_INFO_REQ ]					= &PktHandlerBase::onPktFileInfoReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SHARE_ERR ]					= &PktHandlerBase::onPktFileShareErr;

	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REQ ]					= &PktHandlerBase::onPktAssetSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REPLY ]					= &PktHandlerBase::onPktAssetSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REQ ]					= &PktHandlerBase::onPktAssetChunkReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REPLY ]				= &PktHandlerBase::onPktAssetChunkReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_COMPLETE_REQ ]			= &PktHandlerBase::onPktAssetSendCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_COMPLETE_REPLY ]		= &PktHandlerBase::onPktAssetSendCompleteReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_XFER_ERR ]					= &PktHandlerBase::onPktAssetSendCompleteReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_MSESSION_REQ ]						= &PktHandlerBase::onPktMultiSessionReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_MSESSION_REPLY ]					= &PktHandlerBase::onPktMultiSessionReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SERVICE_REQ ]				= &PktHandlerBase::onPktRelayServiceReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SERVICE_REPLY ]				= &PktHandlerBase::onPktRelayServiceReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_REQ ]				= &PktHandlerBase::onPktRelayConnectReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_REPLY ]				= &PktHandlerBase::onPktRelayConnectReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SESSION_REQ ]				= &PktHandlerBase::onPktRelaySessionReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SESSION_REPLY ]				= &PktHandlerBase::onPktRelaySessionReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_TO_USER_REQ ]		= &PktHandlerBase::onPktRelayConnectToUserReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_TO_USER_REPLY ]		= &PktHandlerBase::onPktRelayConnectToUserReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_USER_DISCONNECT ]			= &PktHandlerBase::onPktRelayUserDisconnect;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_TEST_REQ ]					= &PktHandlerBase::onPktRelayTestReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_TEST_REPLY ]					= &PktHandlerBase::onPktRelayTestReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_SEARCH_REQ ]						= &PktHandlerBase::onPktSearchReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SEARCH_REPLY ]						= &PktHandlerBase::onPktSearchReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ ]		= &PktHandlerBase::onPktMyPicSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY ]		= &PktHandlerBase::onPktMyPicSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX ]			= &PktHandlerBase::onPktWebServerPicChunkTx;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK ]			= &PktHandlerBase::onPktWebServerPicChunkAck;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_GET_CHUNK_TX ]			= &PktHandlerBase::onPktWebServerGetChunkTx;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK ]			= &PktHandlerBase::onPktWebServerGetChunkAck;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX ]			= &PktHandlerBase::onPktWebServerPutChunkTx;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK ]			= &PktHandlerBase::onPktWebServerPutChunkAck;

	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_STATS ]					= &PktHandlerBase::onPktTodGameStats;
	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_ACTION ]					= &PktHandlerBase::onPktTodGameAction;
	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_VALUE ]					= &PktHandlerBase::onPktTodGameValue;

	m_aBaseSysPktFuncTable[ PKT_TYPE_TCP_PUNCH ]						= &PktHandlerBase::onPktTcpPunch;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PING_REQ ]							= &PktHandlerBase::onPktPingReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PING_REPLY ]						= &PktHandlerBase::onPktPingReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_IM_ALIVE_REQ ]						= &PktHandlerBase::onPktImAliveReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_IM_ALIVE_REPLY ]					= &PktHandlerBase::onPktImAliveReply;
}

//============================================================================
PktHandlerBase::~PktHandlerBase()
{
}

//============================================================================
//! Handle Incoming packet.. use function jump table for speed
void PktHandlerBase::handlePkt( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	uint16_t pktType = pktHdr->getPktType();
	if( pktType <= sizeof(  m_aBaseSysPktFuncTable ) / sizeof( void *) )
		return (this->*m_aBaseSysPktFuncTable[ pktType ])(sktBase, pktHdr);
	return onPktInvalid(sktBase, pktHdr);
}

//=== packet handlers ===//
//============================================================================
void PktHandlerBase::onPktUnhandled( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
}

//============================================================================
void PktHandlerBase::onPktInvalid( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	LogMsg( LOG_ERROR, "PktHandlerBase::onPktInvalid type 0x%x len 0x%x\n", pktHdr->getPktType(), pktHdr->getPktLength() );
}

//============================================================================
void PktHandlerBase::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktChatReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFindFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFindFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktFileGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileShareErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktMultiSessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktMultiSessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktAnnList( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktLogReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktLogReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayServiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayServiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayConnectReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayConnectReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelaySessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelaySessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayConnectToUserReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayConnectToUserReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayUserDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayTestReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktRelayTestReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktSearchReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTodGameStats( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTodGameAction( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTodGameValue( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTcpPunch( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktPingReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktPingReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktImAliveReq( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktImAliveReply( VxSktBase * sktBase, VxPktHdr * pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	
