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
class VxNetIdent;

class PktPluginHandlerBase
{
public:
	PktPluginHandlerBase();
	virtual ~PktPluginHandlerBase() = default;

	virtual void				handlePkt				    ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );


	//=== packet handlers ===//
	virtual void				onPktUnhandled				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktInvalid				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktChatReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktVoiceReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktVoiceReply				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktVideoFeedReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktVideoFeedStatus		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktVideoFeedPic			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktVideoFeedPicChunk		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktVideoFeedPicAck		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktSessionStartReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktSessionStartReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktSessionStopReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktSessionStopReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktFileGetReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFindFileReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFindFileReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileListReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileListReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileChunkReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileGetCompleteReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileShareErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				onPktAssetGetReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktAssetGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetChunkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktAssetGetCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktAssetGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAssetXferErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktMultiSessionReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktMultiSessionReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktAnnounce				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktAnnList				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktScanReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktScanReply			    ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktMyPicSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktMyPicSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktWebServerPicChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktWebServerPicChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktWebServerGetChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktWebServerGetChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktWebServerPutChunkTx	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktWebServerPutChunkAck	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktTodGameStats			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktTodGameAction			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktTodGameValue			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktTcpPunch				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktPingReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPingReply				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktImAliveReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktImAliveReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				onPktBlobSendReq            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktBlobSendReply          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktBlobChunkReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktBlobChunkReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktBlobSendCompleteReq    ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktBlobSendCompleteReply  ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktBlobXferErr            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				onPktHostJoinReq            ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktHostJoinReply          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostUnJoinReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostUnJoinReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostLeaveReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostLeaveReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				onPktHostSearchReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktHostSearchReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktHostOfferReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktHostOfferReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktFriendOfferReq         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktFriendOfferReply       ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				onPktThumbGetReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbGetReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbChunkReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbChunkReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbGetCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbGetCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendCompleteReq   ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbSendCompleteReply ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktThumbXferErr           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				onPktOfferSendReq           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktOfferSendReply         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktOfferChunkReq          ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktOfferChunkReply        ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktOfferSendCompleteReq   ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktOfferSendCompleteReply ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktOfferXferErr           ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktPushToTalkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPushToTalkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPushToTalkStart		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPushToTalkStop			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktMembershipReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktMembershipReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktHostInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostInfoReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktHostInviteAnnReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostInviteAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostInviteSearchReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostInviteSearchReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostInviteMoreReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktHostInviteMoreReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktGroupieInfoReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktGroupieInfoReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktGroupieAnnReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktGroupieAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktGroupieSearchReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktGroupieSearchReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktGroupieMoreReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktGroupieMoreReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktFileInfoInfoReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoInfoReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktFileInfoAnnReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoAnnReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoSearchReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoSearchReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoMoreReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktFileInfoMoreReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktRelayUserDisconnect	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	//packet type 250 and greater not allowed
	typedef void (PktPluginHandlerBase::*RC_PLUGIN_BASE_PKT_FUNCTION)( VxSktBase*, VxPktHdr*, VxNetIdent* );  
protected:
	//=== vars ====//
	RC_PLUGIN_BASE_PKT_FUNCTION m_aBaseSysPktFuncTable[ MAX_PKT_TYPE_CNT + 2 ];
};