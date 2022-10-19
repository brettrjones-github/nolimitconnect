#pragma once
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

class VxSktBase;
class VxPktHdr;

class PktHandlerBase
{
public:
	PktHandlerBase();
	virtual ~PktHandlerBase() = default;

	virtual void				handlePkt				    ( VxSktBase* sktBase, VxPktHdr* pktHdr );

	//=== packet handlers ===//
	virtual void				onPktUnhandled				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktInvalid				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktAnnounce				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAnnList				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktHostUnJoinReq					( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostUnJoinReply				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktScanReq				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktScanReply			    ( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktChatReq				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktChatReply				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktVoiceReq				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktVoiceReply				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktVideoFeedReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktVideoFeedStatus		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktVideoFeedPic			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktVideoFeedPicChunk		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktVideoFeedPicAck		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktFileGetReq				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFindFileReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFindFileReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileListReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileListReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktFileInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktFileChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileChunkReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileGetCompleteReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileShareErr			( VxSktBase* sktBase, VxPktHdr* pktHdr );

    virtual void				onPktAssetGetReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktAssetGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetChunkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktAssetGetCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktAssetGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktAssetXferErr			( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktMultiSessionReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktMultiSessionReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktSessionStartReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktSessionStartReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktSessionStopReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktSessionStopReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktMyPicSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktMyPicSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktWebServerPicChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktWebServerPicChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktWebServerGetChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktWebServerGetChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktWebServerPutChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktWebServerPutChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktTodGameStats			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktTodGameAction			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktTodGameValue			( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktTcpPunch				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktPingReq				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktPingReply				( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktImAliveReq				( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktImAliveReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );

    virtual void				onPktBlobSendReq            ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktBlobSendReply          ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktBlobChunkReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktBlobChunkReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktBlobSendCompleteReq    ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktBlobSendCompleteReply  ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktBlobXferErr            ( VxSktBase* sktBase, VxPktHdr* pktHdr );

    virtual void				onPktHostJoinReq            ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktHostJoinReply          ( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostLeaveReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostLeaveReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );

    virtual void				onPktHostSearchReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktHostSearchReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktHostOfferReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktHostOfferReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktFriendOfferReq         ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktFriendOfferReply       ( VxSktBase* sktBase, VxPktHdr* pktHdr );

    virtual void				onPktThumbGetReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbSendReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbSendReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbChunkReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbChunkReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbGetCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbSendCompleteReq   ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbSendCompleteReply ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktThumbXferErr           ( VxSktBase* sktBase, VxPktHdr* pktHdr );

    virtual void				onPktOfferSendReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktOfferSendReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktOfferChunkReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktOfferChunkReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktOfferSendCompleteReq   ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktOfferSendCompleteReply ( VxSktBase* sktBase, VxPktHdr* pktHdr );
    virtual void				onPktOfferXferErr           ( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktPushToTalkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktPushToTalkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktPushToTalkStart		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktPushToTalkStop         ( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktMembershipReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktMembershipReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktHostInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostInfoReply			( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktHostInviteAnnReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostInviteAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostInviteSearchReq	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostInviteSearchReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostInviteMoreReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktHostInviteMoreReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktGroupieInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktGroupieInfoReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktGroupieAnnReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktGroupieAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktGroupieSearchReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktGroupieSearchReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktGroupieMoreReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktGroupieMoreReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktFileInfoInfoReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileInfoInfoReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktFileInfoAnnReq			( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileInfoAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileInfoSearchReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileInfoSearchReply	( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileInfoMoreReq		( VxSktBase* sktBase, VxPktHdr* pktHdr );
	virtual void				onPktFileInfoMoreReply		( VxSktBase* sktBase, VxPktHdr* pktHdr );

	virtual void				onPktRelayUserDisconnect	( VxSktBase* sktBase, VxPktHdr* pktHdr );

	//packet type 250 and greater not allowed
	typedef void (PktHandlerBase::*RC_SYS_BASE_PKT_FUNCTION)( VxSktBase*, VxPktHdr* );  
protected:
	//=== vars ====//
	RC_SYS_BASE_PKT_FUNCTION m_aBaseSysPktFuncTable[ MAX_PKT_TYPE_CNT + 2 ];
};