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
#include "PktSysHandlerBase.h"

#include <NetLib/VxSktBase.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

PktHandlerBase::PktHandlerBase()
{
    for( size_t i = 0; i <= (sizeof( m_aBaseSysPktFuncTable )) / sizeof( void * ); i++ )
	{
		m_aBaseSysPktFuncTable[ i ] = &PktHandlerBase::onPktUnhandled;
	}

	int maxPktType = MAX_PKT_TYPE_CNT;
    vx_assert( 134 == maxPktType );

	m_aBaseSysPktFuncTable[ 0 ] = &PktHandlerBase::onPktInvalid;

	m_aBaseSysPktFuncTable[ PKT_TYPE_ANNOUNCE ]							= &PktHandlerBase::onPktAnnounce;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ANN_LIST ]							= &PktHandlerBase::onPktAnnList;

	m_aBaseSysPktFuncTable[PKT_TYPE_SCAN_REQ]							= &PktHandlerBase::onPktScanReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_SCAN_REPLY]							= &PktHandlerBase::onPktScanReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_OFFER_REQ ]					= &PktHandlerBase::onPktPluginOfferReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PLUGIN_OFFER_REPLY ]				= &PktHandlerBase::onPktPluginOfferReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_CHAT_REQ ]							= &PktHandlerBase::onPktChatReq;
	//#define PKT_TYPE_CHAT_REPLY							( PKT_TYPE_CHAT_REQ	+ 1 )				        // 10 ( 0x0a )

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

    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_REQ ]					= &PktHandlerBase::onPktAssetGetReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_REPLY ]					= &PktHandlerBase::onPktAssetGetReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REQ ]					= &PktHandlerBase::onPktAssetSendReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_REPLY ]					= &PktHandlerBase::onPktAssetSendReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REQ ]					= &PktHandlerBase::onPktAssetChunkReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_CHUNK_REPLY ]				= &PktHandlerBase::onPktAssetChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_COMPLETE_REQ ]			= &PktHandlerBase::onPktAssetGetCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_GET_COMPLETE_REPLY ]			= &PktHandlerBase::onPktAssetGetCompleteReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_COMPLETE_REQ ]			= &PktHandlerBase::onPktAssetSendCompleteReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_SEND_COMPLETE_REPLY ]		= &PktHandlerBase::onPktAssetSendCompleteReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_ASSET_XFER_ERR ]					= &PktHandlerBase::onPktAssetXferErr;

	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_GET_REQ]						= &PktHandlerBase::onPktThumbGetReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_GET_REPLY]					= &PktHandlerBase::onPktThumbGetReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_SEND_REQ]						= &PktHandlerBase::onPktThumbSendReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_SEND_REPLY]					= &PktHandlerBase::onPktThumbSendReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_CHUNK_REQ]					= &PktHandlerBase::onPktThumbChunkReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_CHUNK_REPLY]					= &PktHandlerBase::onPktThumbChunkReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_GET_COMPLETE_REQ]				= &PktHandlerBase::onPktThumbGetCompleteReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_GET_COMPLETE_REPLY]			= &PktHandlerBase::onPktThumbGetCompleteReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_SEND_COMPLETE_REQ]			= &PktHandlerBase::onPktThumbSendCompleteReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_SEND_COMPLETE_REPLY]			= &PktHandlerBase::onPktThumbSendCompleteReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_THUMB_XFER_ERR]						= &PktHandlerBase::onPktThumbXferErr;

	m_aBaseSysPktFuncTable[ PKT_TYPE_MSESSION_REQ ]						= &PktHandlerBase::onPktMultiSessionReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_MSESSION_REPLY ]					= &PktHandlerBase::onPktMultiSessionReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_TCP_PUNCH]							= &PktHandlerBase::onPktTcpPunch;
	m_aBaseSysPktFuncTable[PKT_TYPE_PING_REQ]							= &PktHandlerBase::onPktPingReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_PING_REPLY]							= &PktHandlerBase::onPktPingReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_IM_ALIVE_REQ]						= &PktHandlerBase::onPktImAliveReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_IM_ALIVE_REPLY]						= &PktHandlerBase::onPktImAliveReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_STATS ]					= &PktHandlerBase::onPktTodGameStats;
	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_ACTION ]					= &PktHandlerBase::onPktTodGameAction;
	m_aBaseSysPktFuncTable[ PKT_TYPE_TOD_GAME_VALUE ]					= &PktHandlerBase::onPktTodGameValue;
  
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_REQ ]                    = &PktHandlerBase::onPktBlobSendReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_REPLY ]                  = &PktHandlerBase::onPktBlobSendReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_CHUNK_REQ ]                   = &PktHandlerBase::onPktBlobChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_CHUNK_REPLY ]                 = &PktHandlerBase::onPktBlobChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_COMPLETE_REQ ]           = &PktHandlerBase::onPktBlobSendCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_SEND_COMPLETE_REPLY ]         = &PktHandlerBase::onPktBlobSendCompleteReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_BLOB_XFER_ERR ]                    = &PktHandlerBase::onPktBlobXferErr;

    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_JOIN_REQ ]                    = &PktHandlerBase::onPktHostJoinReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_JOIN_REPLY ]                  = &PktHandlerBase::onPktHostJoinReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_UNJOIN_REQ ]					= &PktHandlerBase::onPktHostUnJoinReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_UNJOIN_REPLY ]				= &PktHandlerBase::onPktHostUnJoinReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_LEAVE_REQ ]					= &PktHandlerBase::onPktHostLeaveReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_LEAVE_REPLY ]					= &PktHandlerBase::onPktHostLeaveReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_OFFER_REQ ]                   = &PktHandlerBase::onPktHostOfferReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_HOST_OFFER_REPLY ]                 = &PktHandlerBase::onPktHostOfferReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_SEARCH_REQ]					= &PktHandlerBase::onPktHostSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_SEARCH_REPLY]					= &PktHandlerBase::onPktHostSearchReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_FRIEND_OFFER_REQ ]                 = &PktHandlerBase::onPktFriendOfferReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_FRIEND_OFFER_REPLY ]               = &PktHandlerBase::onPktFriendOfferReply;

    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_REQ ]                   = &PktHandlerBase::onPktOfferSendReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_REPLY ]                 = &PktHandlerBase::onPktOfferSendReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_CHUNK_REQ ]                  = &PktHandlerBase::onPktOfferChunkReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_CHUNK_REPLY ]                = &PktHandlerBase::onPktOfferChunkReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_COMPLETE_REQ ]          = &PktHandlerBase::onPktOfferSendCompleteReq;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_SEND_COMPLETE_REPLY ]        = &PktHandlerBase::onPktOfferSendCompleteReply;
    m_aBaseSysPktFuncTable[ PKT_TYPE_OFFER_XFER_ERR ]                   = &PktHandlerBase::onPktOfferXferErr;

	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REQ]			= &PktHandlerBase::onPktMyPicSendReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_MY_PIC_SEND_REPLY]		= &PktHandlerBase::onPktMyPicSendReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_PIC_CHUNK_TX]			= &PktHandlerBase::onPktWebServerPicChunkTx;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_PIC_CHUNK_ACK]			= &PktHandlerBase::onPktWebServerPicChunkAck;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_GET_CHUNK_TX]			= &PktHandlerBase::onPktWebServerGetChunkTx;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_GET_CHUNK_ACK]			= &PktHandlerBase::onPktWebServerGetChunkAck;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_PUT_CHUNK_TX]			= &PktHandlerBase::onPktWebServerPutChunkTx;
	m_aBaseSysPktFuncTable[PKT_TYPE_WEB_SERVER_PUT_CHUNK_ACK]			= &PktHandlerBase::onPktWebServerPutChunkAck;

	m_aBaseSysPktFuncTable[PKT_TYPE_PUSH_TO_TALK_REQ]					= &PktHandlerBase::onPktPushToTalkReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_PUSH_TO_TALK_REPLY]					= &PktHandlerBase::onPktPushToTalkReply;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PUSH_TO_TALK_START ]				= &PktHandlerBase::onPktPushToTalkStart;
	m_aBaseSysPktFuncTable[ PKT_TYPE_PUSH_TO_TALK_STOP ]				= &PktHandlerBase::onPktPushToTalkStop;

	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INFO_REQ]						= &PktHandlerBase::onPktHostInfoReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INFO_REPLY]					= &PktHandlerBase::onPktHostInfoReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_ANN_REQ]				= &PktHandlerBase::onPktHostInviteAnnReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_ANN_REPLY]				= &PktHandlerBase::onPktHostInviteAnnReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_SEARCH_REQ]				= &PktHandlerBase::onPktHostInviteSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_SEARCH_REPLY]			= &PktHandlerBase::onPktHostInviteSearchReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_MORE_REQ]				= &PktHandlerBase::onPktHostInviteMoreReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_HOST_INVITE_MORE_REPLY]				= &PktHandlerBase::onPktHostInviteMoreReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_INFO_REQ]					= &PktHandlerBase::onPktGroupieInfoReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_INFO_REPLY]					= &PktHandlerBase::onPktGroupieInfoReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_ANN_REQ]					= &PktHandlerBase::onPktGroupieAnnReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_ANN_REPLY]					= &PktHandlerBase::onPktGroupieAnnReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_SEARCH_REQ]					= &PktHandlerBase::onPktGroupieSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_SEARCH_REPLY]				= &PktHandlerBase::onPktGroupieSearchReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_MORE_REQ]					= &PktHandlerBase::onPktGroupieMoreReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_GROUPIE_MORE_REPLY]					= &PktHandlerBase::onPktGroupieMoreReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_INFO_REQ]					= &PktHandlerBase::onPktFileInfoInfoReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_INFO_REPLY]				= &PktHandlerBase::onPktFileInfoInfoReply;

	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_ANN_REQ]					= &PktHandlerBase::onPktFileInfoAnnReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_ANN_REPLY]				= &PktHandlerBase::onPktFileInfoAnnReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_SEARCH_REQ]				= &PktHandlerBase::onPktFileInfoSearchReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_SEARCH_REPLY]				= &PktHandlerBase::onPktFileInfoSearchReply;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_MORE_REQ]					= &PktHandlerBase::onPktFileInfoMoreReq;
	m_aBaseSysPktFuncTable[PKT_TYPE_FILE_INFO_MORE_REPLY]				= &PktHandlerBase::onPktFileInfoMoreReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_MEMBERSHIP_REQ ]					= &PktHandlerBase::onPktMembershipReq;
	m_aBaseSysPktFuncTable[ PKT_TYPE_MEMBERSHIP_REPLY ]					= &PktHandlerBase::onPktMembershipReply;

	m_aBaseSysPktFuncTable[ PKT_TYPE_RELAY_USER_DISCONNECT ]			= &PktHandlerBase::onPktRelayUserDisconnect;
}

//============================================================================
//! Handle Incoming packet.. use function jump table for speed
void PktHandlerBase::handlePkt( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    if( VxIsAppShuttingDown() || !sktBase->isConnected() )
    {
        return;
    }

	uint16_t pktType = pktHdr->getPktType();
    if( pktType <= sizeof(  m_aBaseSysPktFuncTable ) / (sizeof( void * )) )
    {
		return (this->*m_aBaseSysPktFuncTable[ pktType ])(sktBase, pktHdr);
    }

	return onPktInvalid(sktBase, pktHdr);
}

//=== packet handlers ===//
//============================================================================
void PktHandlerBase::onPktUnhandled( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
}

//============================================================================
void PktHandlerBase::onPktInvalid( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	LogMsg( LOG_ERROR, "PktHandlerBase::onPktInvalid type 0x%x len 0x%x\n", pktHdr->getPktType(), pktHdr->getPktLength() );
}

//============================================================================
void PktHandlerBase::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktPluginOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktChatReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktChatReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVoiceReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVoiceReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedStatus( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedPic( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedPicChunk( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktVideoFeedPicAck( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFindFileReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFindFileReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileListReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileListReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktFileSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktFileGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileShareErr( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktAssetXferErr( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktMultiSessionReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktMultiSessionReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStartReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStartReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStopReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktSessionStopReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktAnnounce( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktAnnList( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktHostUnJoinReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktHostUnJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktScanReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktScanReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktMyPicSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktMyPicSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPicChunkTx( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPicChunkAck( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerGetChunkTx( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerGetChunkAck( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPutChunkTx( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktWebServerPutChunkAck( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTodGameStats( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTodGameAction( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTodGameValue( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktTcpPunch( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktPingReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktPingReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktImAliveReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}	

//============================================================================
void PktHandlerBase::onPktImAliveReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktBlobXferErr( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostJoinReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostJoinReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostLeaveReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostLeaveReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFriendOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFriendOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbGetReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbGetReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbGetCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbGetCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktThumbXferErr( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

// offers

//============================================================================
void PktHandlerBase::onPktOfferSendReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktOfferSendReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktOfferChunkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktOfferChunkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktOfferSendCompleteReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktOfferSendCompleteReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktOfferXferErr( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
    onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktPushToTalkReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktPushToTalkReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktPushToTalkStart( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktPushToTalkStop( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktMembershipReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktMembershipReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInviteAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInviteAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInviteSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInviteSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInviteMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktHostInviteMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktFileInfoMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}

//============================================================================
void PktHandlerBase::onPktRelayUserDisconnect( VxSktBase* sktBase, VxPktHdr* pktHdr )
{
	onPktUnhandled( sktBase, pktHdr );
}
