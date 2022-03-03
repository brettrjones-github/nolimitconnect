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
// http://www.nolimitconnect.com
//============================================================================
 
#include "PktTypes.h"
#include "PktPluginHandlerBase.h"

#include <CoreLib/VxDebug.h>

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

	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_UNJOIN_REQ ]							= &PktPluginHandlerBase::onPktHostUnJoinReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_UNJOIN_REPLY ]						= &PktPluginHandlerBase::onPktHostUnJoinReply;

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

    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_REQ ]					= &PktPluginHandlerBase::onPktAssetGetReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_REPLY ]					= &PktPluginHandlerBase::onPktAssetGetReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REQ ]					= &PktPluginHandlerBase::onPktAssetSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REPLY ]					= &PktPluginHandlerBase::onPktAssetSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REQ ]					= &PktPluginHandlerBase::onPktAssetChunkReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REPLY ]				= &PktPluginHandlerBase::onPktAssetChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_COMPLETE_REQ ]			= &PktPluginHandlerBase::onPktAssetGetCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_COMPLETE_REPLY ]			= &PktPluginHandlerBase::onPktAssetGetCompleteReply;
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
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_USER_DISCONNECT ]			= &PktPluginHandlerBase::onPktRelayUserDisconnect;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_TEST_REQ ]					= &PktPluginHandlerBase::onPktRelayTestReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_TEST_REPLY ]					= &PktPluginHandlerBase::onPktRelayTestReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_SCAN_REQ ]						    = &PktPluginHandlerBase::onPktScanReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_SCAN_REPLY ]						= &PktPluginHandlerBase::onPktScanReply;
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

    m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_SETTING_REQ ]               = &PktPluginHandlerBase::onPktPluginSettingReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_SETTING_REPLY ]             = &PktPluginHandlerBase::onPktPluginSettingReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SETTING_REQ ]                = &PktPluginHandlerBase::onPktThumbSettingReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SETTING_REPLY ]              = &PktPluginHandlerBase::onPktThumbSettingReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SETTING_CHUNK_REQ ]          = &PktPluginHandlerBase::onPktThumbSettingChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SETTING_CHUNK_REPLY ]        = &PktPluginHandlerBase::onPktThumbSettingChunkReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_AVATAR_REQ ]                 = &PktPluginHandlerBase::onPktThumbAvatarReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_AVATAR_REPLY ]               = &PktPluginHandlerBase::onPktThumbAvatarReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_AVATAR_CHUNK_REQ ]           = &PktPluginHandlerBase::onPktThumbAvatarChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_AVATAR_CHUNK_REPLY ]         = &PktPluginHandlerBase::onPktThumbAvatarChunkReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_FILE_REQ ]                   = &PktPluginHandlerBase::onPktThumbFileReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_FILE_REPLY ]                 = &PktPluginHandlerBase::onPktThumbFileReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_FILE_CHUNK_REQ ]             = &PktPluginHandlerBase::onPktThumbFileChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_FILE_CHUNK_REPLY ]           = &PktPluginHandlerBase::onPktThumbFileChunkReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_ANNOUNCE ]                    = &PktPluginHandlerBase::onPktHostAnnounce;

    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_REQ ]                    = &PktPluginHandlerBase::onPktBlobSendReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_REPLY ]                  = &PktPluginHandlerBase::onPktBlobSendReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_CHUNK_REQ ]                   = &PktPluginHandlerBase::onPktBlobChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_CHUNK_REPLY ]                 = &PktPluginHandlerBase::onPktBlobChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_COMPLETE_REQ ]           = &PktPluginHandlerBase::onPktBlobSendCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_COMPLETE_REPLY ]         = &PktPluginHandlerBase::onPktBlobSendCompleteReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_XFER_ERR ]                    = &PktPluginHandlerBase::onPktBlobXferErr;

    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_JOIN_REQ ]                    = &PktPluginHandlerBase::onPktHostJoinReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_JOIN_REPLY ]                  = &PktPluginHandlerBase::onPktHostJoinReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_LEAVE_REQ ]					= &PktPluginHandlerBase::onPktHostLeaveReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_LEAVE_REPLY ]					= &PktPluginHandlerBase::onPktHostLeaveReply;


    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_SEARCH_REQ ]                  = &PktPluginHandlerBase::onPktHostSearchReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_SEARCH_REPLY ]                = &PktPluginHandlerBase::onPktHostSearchReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_OFFER_REQ ]                   = &PktPluginHandlerBase::onPktHostOfferReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_OFFER_REPLY ]                 = &PktPluginHandlerBase::onPktHostOfferReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_FRIEND_OFFER_REQ ]                 = &PktPluginHandlerBase::onPktFriendOfferReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_FRIEND_OFFER_REPLY ]               = &PktPluginHandlerBase::onPktFriendOfferReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_GET_REQ ]					= &PktPluginHandlerBase::onPktThumbGetReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_GET_REPLY ]					= &PktPluginHandlerBase::onPktThumbGetReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SEND_REQ ]                   = &PktPluginHandlerBase::onPktThumbSendReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SEND_REPLY ]                 = &PktPluginHandlerBase::onPktThumbSendReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_CHUNK_REQ ]                  = &PktPluginHandlerBase::onPktThumbChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_CHUNK_REPLY ]                = &PktPluginHandlerBase::onPktThumbChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_GET_COMPLETE_REQ ]			= &PktPluginHandlerBase::onPktThumbGetCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_GET_COMPLETE_REPLY ]			= &PktPluginHandlerBase::onPktThumbGetCompleteReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SEND_COMPLETE_REQ ]          = &PktPluginHandlerBase::onPktThumbSendCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_SEND_COMPLETE_REPLY ]        = &PktPluginHandlerBase::onPktThumbSendCompleteReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_THUMB_XFER_ERR ]                   = &PktPluginHandlerBase::onPktThumbXferErr;

    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_REQ ]                   = &PktPluginHandlerBase::onPktOfferSendReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_REPLY ]                 = &PktPluginHandlerBase::onPktOfferSendReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_CHUNK_REQ ]                  = &PktPluginHandlerBase::onPktOfferChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_CHUNK_REPLY ]                = &PktPluginHandlerBase::onPktOfferChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_COMPLETE_REQ ]          = &PktPluginHandlerBase::onPktOfferSendCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_COMPLETE_REPLY ]        = &PktPluginHandlerBase::onPktOfferSendCompleteReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_XFER_ERR ]                   = &PktPluginHandlerBase::onPktOfferXferErr;

	m_aBaseSysPktFuncTable[PKT_TYPE_PUSH_TO_TALK_REQ]					= &PktPluginHandlerBase::onPktPushToTalkReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_PUSH_TO_TALK_REPLY]					= &PktPluginHandlerBase::onPktPushToTalkReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_MEMBERSHIP_REQ]						= &PktPluginHandlerBase::onPktMembershipReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_MEMBERSHIP_REPLY]					= &PktPluginHandlerBase::onPktMembershipReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INFO_REQ]						= &PktPluginHandlerBase::onPktHostInfoReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INFO_REPLY]					= &PktPluginHandlerBase::onPktHostInfoReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_ANN_REQ]				= &PktPluginHandlerBase::onPktHostInviteAnnReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_ANN_REPLY]				= &PktPluginHandlerBase::onPktHostInviteAnnReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_SEARCH_REQ]				= &PktPluginHandlerBase::onPktHostInviteSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_SEARCH_REPLY]			= &PktPluginHandlerBase::onPktHostInviteSearchReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_MORE_REQ]				= &PktPluginHandlerBase::onPktHostInviteMoreReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_MORE_REPLY]				= &PktPluginHandlerBase::onPktHostInviteMoreReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_INFO_REQ]					= &PktPluginHandlerBase::onPktGroupieInfoReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_INFO_REPLY]					= &PktPluginHandlerBase::onPktGroupieInfoReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_ANN_REQ]					= &PktPluginHandlerBase::onPktGroupieAnnReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_ANN_REPLY]					= &PktPluginHandlerBase::onPktGroupieAnnReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_SEARCH_REQ]					= &PktPluginHandlerBase::onPktGroupieSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_SEARCH_REPLY]				= &PktPluginHandlerBase::onPktGroupieSearchReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_MORE_REQ]					= &PktPluginHandlerBase::onPktGroupieMoreReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_MORE_REPLY]					= &PktPluginHandlerBase::onPktGroupieMoreReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_INFO_REQ]					= &PktPluginHandlerBase::onPktFileInfoInfoReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_INFO_REPLY]				= &PktPluginHandlerBase::onPktFileInfoInfoReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_ANN_REQ]					= &PktPluginHandlerBase::onPktFileInfoAnnReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_ANN_REPLY]				= &PktPluginHandlerBase::onPktFileInfoAnnReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_SEARCH_REQ]				= &PktPluginHandlerBase::onPktFileInfoSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_SEARCH_REPLY]				= &PktPluginHandlerBase::onPktFileInfoSearchReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_MORE_REQ]					= &PktPluginHandlerBase::onPktFileInfoMoreReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_MORE_REPLY]				= &PktPluginHandlerBase::onPktFileInfoMoreReply;
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
    LogMsg( LOG_WARN, "PktPluginHandlerBase::onPktUnhandled pkt type %d len %d", pktHdr->getPktType(), pktHdr->getPktLength() );
}

//============================================================================
void PktPluginHandlerBase::onPktInvalid( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_WARN, "PktPluginHandlerBase::onPktInvalid pkt type %d len %d", pktHdr->getPktType(), pktHdr->getPktLength() );
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
void PktPluginHandlerBase::onPktAssetGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
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
void PktPluginHandlerBase::onPktAssetGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktAssetGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
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
void PktPluginHandlerBase::onPktHostUnJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktHostUnJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayServiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayServiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayConnectReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayConnectReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelaySessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelaySessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayConnectToUserReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktRelayConnectToUserReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktRelayUserDisconnect( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktRelayTestReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktRelayTestReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktScanReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktScanReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
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
void PktPluginHandlerBase::onPktPluginSettingReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktPluginSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbSettingReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbSettingReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbSettingChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbSettingChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbAvatarReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbAvatarReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbAvatarChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbAvatarChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbFileReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbFileReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbFileChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}	

//============================================================================
void PktPluginHandlerBase::onPktThumbFileChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktBlobXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostLeaveReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostLeaveReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostSearchReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFriendOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFriendOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbGetReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbGetReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbGetCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbGetCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktThumbXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

// offers
//============================================================================
void PktPluginHandlerBase::onPktOfferSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktOfferSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktOfferChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktOfferChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktOfferSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktOfferSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktOfferXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktPushToTalkReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktPushToTalkReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktMembershipReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktMembershipReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInviteAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInviteAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInviteSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInviteSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInviteMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktHostInviteMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}

//============================================================================
void PktPluginHandlerBase::onPktFileInfoMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktUnhandled( sktBase, pktHdr, netIdent );
}