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

#include "PluginSessionMgr.h"
#include "PluginBase.h"
#include "VoiceFeedMgr.h"
#include "VideoFeedMgr.h"

class PluginCamClient : public PluginBase
{
public:
	PluginCamClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
	virtual ~PluginCamClient() = default;

    virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = nullptr,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
    virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

	//! user wants to send offer to friend.. return false if cannot connect
	virtual bool				fromGuiMakePluginOffer( VxNetIdent*		netIdent,				// identity of friend
														int				pvUserData,
														const char*		pOfferMsg,				// offer message
														const char*		pFileName = nullptr,
														uint8_t*		fileHashId = 0,
                                                        VxGUID			lclSessionId = VxGUID::nullVxGUID() ) override;

    virtual void				fromGuiUpdatePluginPermission( EPluginType pluginType, EFriendState pluginPermission ) override;

	bool						stopCamSession(	VxNetIdent* netIdent, VxSktBase* sktBase );

	void						sendVidPkt( VxPktHdr* vidPkt, bool requiresAck );

	void						stopAllSessions( EPluginType pluginType );

protected:
    virtual EPluginAccess       canAcceptNewSession			( VxNetIdent* netIdent ) override;

    virtual void				replaceConnection			( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt ) override;
    virtual void				onConnectionLost			( VxSktBase* sktBase ) override;
    virtual void				onContactWentOffline		( VxNetIdent* netIdent, VxSktBase* sktBase ) override;

    virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktSessionStartReq		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStartReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStopReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktSessionStopReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktVideoFeedReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedStatus		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedPic			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedPicChunk		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVideoFeedPicAck		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				onPktVoiceReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
    virtual void				onPktVoiceReply				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio ) override;
    virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen ) override;

    virtual void				callbackVideoJpgSmall(	void * userData, VxGUID& feedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 ) override;
    virtual void				callbackVideoPktPic( void * userData, VxGUID& feedId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum ) override;
    virtual void				callbackVideoPktPicChunk( void * userData, VxGUID& feedId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum ) override;

	// override this by plugin to create inherited RxSession
    RxSession *					createRxSession( VxSktBase* sktBase, VxNetIdent* netIdent ) override;
	// override this by plugin to create inherited TxSession
    TxSession *					createTxSession( VxSktBase* sktBase, VxNetIdent* netIdent ) override;

	bool						requestCamSession(	RxSession *			rxSession,
													bool				bWaitForSuccess = false );
    void						setIsPluginInSession( bool isInSession ) override;

	virtual	void				onNetworkConnectionReady( bool requiresRelay ) override;

	void						stopAllSessions( void );

	void						enableCamServerService( bool enable );

	//=== vars ===//
	PluginSessionMgr			m_PluginSessionMgr;					
	VoiceFeedMgr				m_VoiceFeedMgr;
	VideoFeedMgr				m_VideoFeedMgr;

	bool						m_IsCamServiceEnabled{ false };
};



