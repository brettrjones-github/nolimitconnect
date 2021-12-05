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

#include "PluginSessionMgr.h"
#include "PluginBase.h"
#include "VoiceFeedMgr.h"
#include "VideoFeedMgr.h"

class PluginServiceWebCam : public PluginBase
{
public:
	PluginServiceWebCam( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginServiceWebCam();


	virtual void				fromGuiStartPluginSession( VxNetIdent * netIdent = NULL,	int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStopPluginSession( VxNetIdent * netIdent = NULL,		int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual bool				fromGuiIsPluginInSession( VxNetIdent * netIdent = NULL,		int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	//! user wants to send offer to friend.. return false if cannot connect
	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent,				// identity of friend
														int				pvUserData,
														const char *	pOfferMsg,				// offer message
														const char *	pFileName = NULL,
														uint8_t *			fileHashId = 0,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() );	

	bool						stopCamSession(	VxNetIdent *		netIdent,	
												VxSktBase *			sktBase );
	void						sendVidPkt( VxPktHdr * vidPkt, bool requiresAck );

protected:
    virtual EPluginAccess       canAcceptNewSession     ( VxNetIdent * netIdent );

    virtual void				replaceConnection       ( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );
    virtual void				onConnectionLost        ( VxSktBase * sktBase );
    virtual void				onContactWentOffline    ( VxNetIdent * netIdent, VxSktBase * sktBase );

    virtual void				onPktPluginOfferReq     ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
    virtual void				onPktPluginOfferReply   ( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktSessionStartReq	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktSessionStartReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktSessionStopReq		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktSessionStopReply	( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktVideoFeedReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedStatus		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPic			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPicChunk		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPicAck		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktVoiceReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVoiceReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio );
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

	virtual void				callbackVideoJpgSmall(	void * userData, VxGUID& feedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 );
	virtual void				callbackVideoPktPic( void * userData, VxGUID& feedId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum );
	virtual void				callbackVideoPktPicChunk( void * userData, VxGUID& feedId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum );

	// override this by plugin to create inherited RxSession
	RxSession *					createRxSession( VxSktBase * sktBase, VxNetIdent * netIdent );
	// override this by plugin to create inherited TxSession
	TxSession *					createTxSession( VxSktBase * sktBase, VxNetIdent * netIdent );

	bool						requestCamSession(	RxSession *			rxSession,
													bool				bWaitForSuccess = false );
	void						setIsPluginInSession( bool isInSession );

	//=== vars ===//
	PluginSessionMgr			m_PluginSessionMgr;					
	VoiceFeedMgr				m_VoiceFeedMgr;
	VideoFeedMgr				m_VideoFeedMgr;
};



