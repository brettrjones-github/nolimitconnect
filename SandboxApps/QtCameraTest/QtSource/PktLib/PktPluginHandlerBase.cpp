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
// http://www.nolimitconnect.org
//============================================================================
 
#include "PktTypes.h"
#include "PktPluginHandlerBase.h"

#include <memory.h>

PktPluginHandlerBase::PktPluginHandlerBase()
{
	memset( m_aBaseSysPktFuncTable, 0, sizeof( m_aBaseSysPktFuncTable ) );
	for( int i = 0; i < MAX_PKT_TYPE_CNT; i++ )
	{
		m_aBaseSysPktFuncTable[ i ] = &PktPluginHandlerBase::onPktUnhandled;
	}

	m_aBaseSysPktFuncTable[ 0 ] = &PktPluginHandlerBase::onPktInvalid;

	m_aBaseSysPktFuncTable[ PKT_TYPE_ANNOUNCE ]							= &PktPluginHandlerBase::onPktAnnounce;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ANN_LIST ]							= &PktPluginHandlerBase::onPktAnnList;

	m_aBaseSysPktFuncTable[ PKT_TYPE_LOG_REQ ]							= &PktPluginHandlerBase::onPktLogReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_LOG_REPLY ]						= &PktPluginHandlerBase::onPktLogReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_OFFER_REQ ]					= &PktPluginHandlerBase::onPktPluginOfferReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_OFFER_REPLY ]				= &PktPluginHandlerBase::onPktPluginOfferReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_CHAT_REQ ]							= &PktPluginHandlerBase::onPktChatReq;

	m_aBaseSysPktFuncTable[ PKT_TYPE_VOICE_REQ ]						= &PktPluginHandlerBase::onPktVoiceReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VOICE_REPLY ]						= &PktPluginHandlerBase::onPktVoiceReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_REQ ]					= &PktPluginHandlerBase::onPktVideoFeedReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_STATUS ]				= &PktPluginHandlerBase::onPktVideoFeedStatus;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_PIC ]					= &PktPluginHandlerBase::onPktVideoFeedPic;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_PIC_CHUNK ]				= &PktPluginHandlerBase::onPktVideoFeedPicChunk;
	m_aBaseSysPktFuncTable[ PKT_TYPE_VIDEO_FEED_PIC_ACK ]				= &PktPluginHandlerBase::onPktVideoFeedPicAck;

	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_START_REQ ]				= &PktPluginHandlerBase::onPktSessionStartReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_START_REPLY ]				= &PktPluginHandlerBase::onPktSessionStartReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_STOP_REQ ]					= &PktPluginHandlerBase::onPktSessionStopReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SESSION_STOP_REPLY ]				= &PktPluginHandlerBase::onPktSessionStopReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_REQ ]						= &PktPluginHandlerBase::onPktFileGetReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_REPLY ]					= &PktPluginHandlerBase::onPktFileGetReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_REQ ]					= &PktPluginHandlerBase::onPktFileSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_REPLY ]					= &PktPluginHandlerBase::onPktFileSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_CHUNK_REQ ]					= &PktPluginHandlerBase::onPktFileChunkReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_CHUNK_REPLY ]					= &PktPluginHandlerBase::onPktFileChunkReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_COMPLETE_REQ ]			= &PktPluginHandlerBase::onPktFileGetCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_GET_COMPLETE_REPLY ]			= &PktPluginHandlerBase::onPktFileGetCompleteReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_COMPLETE_REQ ]			= &PktPluginHandlerBase::onPktFileSendCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SEND_COMPLETE_REPLY ]			= &PktPluginHandlerBase::onPktFileSendCompleteReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_FIND_REQ ]					= &PktPluginHandlerBase::onPktFindFileReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_FIND_REPLY ]					= &PktPluginHandlerBase::onPktFindFileReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_LIST_REQ ]					= &PktPluginHandlerBase::onPktFileListReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_LIST_REPLY ]					= &PktPluginHandlerBase::onPktFileListReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_INFO_REQ ]					= &PktPluginHandlerBase::onPktFileInfoReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_FILE_SHARE_ERR ]					= &PktPluginHandlerBase::onPktFileShareErr;

	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REQ ]					= &PktPluginHandlerBase::onPktAssetSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REPLY ]					= &PktPluginHandlerBase::onPktAssetSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REQ ]					= &PktPluginHandlerBase::onPktAssetChunkReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REPLY ]				= &PktPluginHandlerBase::onPktAssetChunkReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_COMPLETE_REQ ]			= &PktPluginHandlerBase::onPktAssetSendCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_COMPLETE_REPLY ]		= &PktPluginHandlerBase::onPktAssetSendCompleteReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_XFER_ERR ]					= &PktPluginHandlerBase::onPktAssetXferErr;

	m_aBaseSysPktFuncTable[ PKT_TYPE_MSESSION_REQ ]						= &PktPluginHandlerBase::onPktMultiSessionReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_MSESSION_REPLY ]					= &PktPluginHandlerBase::onPktMultiSessionReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SERVICE_REQ ]				= &PktPluginHandlerBase::onPktRelayServiceReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SERVICE_REPLY ]				= &PktPluginHandlerBase::onPktRelayServiceReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_REQ ]				= &PktPluginHandlerBase::onPktRelayConnectReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_REPLY ]				= &PktPluginHandlerBase::onPktRelayConnectReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SESSION_REQ ]				= &PktPluginHandlerBase::onPktRelaySessionReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_SESSION_REPLY ]				= &PktPluginHandlerBase::onPktRelaySessionReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_TO_USER_REQ ]		= &PktPluginHandlerBase::onPktRelayConnectToUserReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_CONNECT_TO_USER_REPLY ]		= &PktPluginHandlerBase::onPktRelayConnectToUserReply;
	
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_TEST_REQ ]					= &PktPluginHandlerBase::onPktRelayTestReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_TEST_REPLY ]					= &PktPluginHandlerBase::onPktRelayTestReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_SEARCH_REQ ]						= &PktPluginHandlerBase::onPktSearchReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SEARCH_REPLY ]						= &PktPluginHandlerBase::onPktSearchReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ ]		= &PktPluginHandlerBase::onPktMyPicSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY ]		= &PktPluginHandlerBase::onPktMyPicSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX ]			= &PktPluginHandlerBase::onPktWebServerPicChunkTx;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK ]			= &PktPluginHandlerBase::onPktWebServerPicChunkAck;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_GET_CHUNK_TX ]			= &PktPluginHandlerBase::onPktWebServerGetChunkTx;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK ]			= &PktPluginHandlerBase::onPktWebServerGetChunkAck;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX ]			= &PktPluginHandlerBase::onPktWebServerPutChunkTx;
	m_aBaseSysPktFuncTable[ PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK ]			= &PktPluginHandlerBase::onPktWebServerPutChunkAck;

	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_STATS ]					= &PktPluginHandlerBase::onPktTodGameStats;
	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_ACTION ]					= &PktPluginHandlerBase::onPktTodGameAction;
	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_VALUE ]					= &PktPluginHandlerBase::onPktTodGameValue;

	m_aBaseSysPktFuncTable[ PKT_TYPE_TCP_PUNCH ]						= &PktPluginHandlerBase::onPktTcpPunch;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PING_REQ ]							= &PktPluginHandlerBase::onPktPingReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PING_REPLY ]						= &PktPluginHandlerBase::onPktPingReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_IM_ALIVE_REQ ]						= &PktPluginHandlerBase::onPktImAliveReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_IM_ALIVE_REPLY ]					= &PktPluginHandlerBase::onPktImAliveReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_USER_DISCONNECT ]			= &PktPluginHandlerBase::onPktRelayUserDisconnect;
}

//============================================================================
PktPluginHandlerBase::~PktPluginHandlerBase()
{
}

//============================================================================
//! Handle Incoming packet.. use function jump table for speed
void PktPluginHandlerBase::handlePkt( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	if( pktHdr->getPktType() <= sizeof(  m_aBaseSysPktFuncTable ) / sizeof( void *) )
		return (this->*m_aBaseSysPktFuncTable[ pktHdr->getPktType() ])(sktBase, pktHdr, netIdent);
	return onPktInvalid(sktBase, pktHdr, netIdent);
}


//=== packet handlers ===//
//============================================================================
void PktPluginHandlerBase::onPktUnhandled( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void PktPluginHandlerBase::onPktInvalid( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void PktPluginHandlerBase::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFindFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFindFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileListReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileListReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktFileSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktFileGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktFileShareErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktMultiSessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktMultiSessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktAnnList( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktLogReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktLogReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktSearchReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktMyPicSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktMyPicSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktWebServerPicChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktWebServerPicChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktWebServerGetChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktWebServerGetChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktWebServerPutChunkTx( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktWebServerPutChunkAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktTodGameStats( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktTodGameAction( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktTodGameValue( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktTcpPunch( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktPingReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktPingReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktImAliveReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktImAliveReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayUserDisconnect( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}
