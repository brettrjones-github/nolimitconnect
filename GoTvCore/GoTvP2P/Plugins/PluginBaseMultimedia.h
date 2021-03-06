#pragma once
//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginBase.h"
#include "PluginSessionMgr.h"
#include "VoiceFeedMgr.h"
#include "VideoFeedMgr.h"

class PluginBaseMultimedia : public PluginBase
{
public:
	PluginBaseMultimedia( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent );
	virtual ~PluginBaseMultimedia() = default;

	virtual void				fromGuiUserLoggedOn( void );

	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent,				
                                                        int             pvUserData,
														const char *	pOfferMsg,				
														const char *	pFileName = NULL,
                                                        uint8_t *		fileHashId = nullptr,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() );		

	virtual bool				fromGuiOfferReply(	VxNetIdent *	netIdent,
                                                    int             pvUserdata,
													EOfferResponse	eOfferResponse,
													VxGUID			lclSessionId );

    virtual bool				fromGuiIsPluginInSession( VxNetIdent * netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
    virtual void				fromGuiStartPluginSession( VxNetIdent * netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
    virtual void				fromGuiStopPluginSession( VxNetIdent * netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual void				fromGuiSendAsset( AssetInfo& assetInfo );
	virtual bool				fromGuiMultiSessionAction( VxNetIdent *	netIdent, EMSessionAction mSessionAction, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() ); 

	virtual bool				fromGuiSetGameValueVar(	VxNetIdent * netIdent, int32_t varId, int32_t varValue );
	virtual bool				fromGuiSetGameActionVar( VxNetIdent * netIdent, int32_t	actionId, int32_t actionValue );

protected:

	virtual void				onPktPluginOfferReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktPluginOfferReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktSessionStopReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktVoiceReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVoiceReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktVideoFeedReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedStatus		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPic			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPicChunk		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPicAck		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktAssetSendReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendReply			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetChunkReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetChunkReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendCompleteReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetSendCompleteReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktAssetXferErr			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktMultiSessionReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktMultiSessionReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktTodGameStats			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktTodGameAction			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktTodGameValue			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio );
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

	//virtual void				callbackVideoJpgSmall(	void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 );
	virtual void				callbackVideoPktPic( void * userData, VxGUID& onlineId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum );
	virtual void				callbackVideoPktPicChunk( void * userData, VxGUID& onlineId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum );

	virtual void				onSessionStart( PluginSessionBase * session, bool pluginIsLocked );
	virtual void				onSessionEnded( PluginSessionBase * session, bool pluginIsLocked, EOfferResponse eOfferResponse );

protected:
	virtual void				onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase ) override;
	virtual void				onContactWentOffline( VxNetIdent *	netIdent, VxSktBase * sktBase ) override;

	virtual void				replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override;
	virtual void				onConnectionLost( VxSktBase * sktBase ) override;

	//=== vars ===
	PluginSessionMgr			m_PluginSessionMgr;
	VoiceFeedMgr				m_VoiceFeedMgr;
	VideoFeedMgr				m_VideoFeedMgr;
	AssetXferMgr				m_AssetXferMgr;
};



