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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginBase.h"
#include "PluginSessionMgr.h"
#include "VoiceFeedMgr.h"
#include "VideoFeedMgr.h"

class PluginBaseMultimedia : public PluginBase
{
public:
	PluginBaseMultimedia( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
	virtual ~PluginBaseMultimedia() = default;

    virtual EAppModule          getAppModule( void ) = 0;

    virtual void				fromGuiUserLoggedOn( void ) override;

	virtual bool				fromGuiMakePluginOffer( VxNetIdent*	netIdent,				
                                                        int             pvUserData,
														const char*	pOfferMsg,				
														const char*	pFileName = NULL,
                                                        uint8_t *		fileHashId = nullptr,
                                                        VxGUID			lclSessionId = VxGUID::nullVxGUID() ) override;

	virtual bool				fromGuiOfferReply(	VxNetIdent*	netIdent,
                                                    int             pvUserdata,
													EOfferResponse	eOfferResponse,
                                                    VxGUID			lclSessionId ) override;

    virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

	virtual bool				fromGuiSendAsset( AssetBaseInfo& assetInfo ) override;
    virtual bool				fromGuiMultiSessionAction( VxNetIdent*	netIdent, EMSessionAction mSessionAction, int pos0to100000, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

    virtual bool				fromGuiSetGameValueVar(	VxNetIdent* netIdent, int32_t varId, int32_t varValue ) override;
    virtual bool				fromGuiSetGameActionVar( VxNetIdent* netIdent, int32_t	actionId, int32_t actionValue ) override;

protected:

    virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStopReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktVoiceReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVoiceReply				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktVideoFeedReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedStatus		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedPic			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedPicChunk		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedPicAck		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktAssetSendReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktAssetSendReply			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktAssetChunkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktAssetChunkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktAssetSendCompleteReq	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktAssetSendCompleteReply	( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktAssetXferErr			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktMultiSessionReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktMultiSessionReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktTodGameStats			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktTodGameAction			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktTodGameValue			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio ) override;
    virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen ) override;

	//virtual void				callbackVideoJpgSmall(	void * userData, VxGUID& vidFeedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 );
    virtual void				callbackVideoPktPic( void * userData, VxGUID& onlineId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum ) override;
    virtual void				callbackVideoPktPicChunk( void * userData, VxGUID& onlineId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum ) override;

    virtual void				onSessionStart( PluginSessionBase * session, bool pluginIsLocked ) override;
    virtual void				onSessionEnded( PluginSessionBase * session, bool pluginIsLocked, EOfferResponse eOfferResponse ) override;

protected:
	virtual void				onContactWentOnline( VxNetIdent* netIdent, VxSktBase* sktBase ) override;
	virtual void				onContactWentOffline( VxNetIdent*	netIdent, VxSktBase* sktBase ) override;

	virtual void				replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override;
	virtual void				onConnectionLost( VxSktBase* sktBase ) override;

	//=== vars ===
	PluginSessionMgr			m_PluginSessionMgr;
	VoiceFeedMgr				m_VoiceFeedMgr;
	VideoFeedMgr				m_VideoFeedMgr;
    AssetXferMgr                m_AssetXferMgr;
};



