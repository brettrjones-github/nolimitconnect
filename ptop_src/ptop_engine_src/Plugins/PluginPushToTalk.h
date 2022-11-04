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
#include "PushToTalkFeedMgr.h"

#include <PktLib/VxCommon.h>

class PluginPushToTalk : public PluginBase
{
public:
	PluginPushToTalk( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType );
	virtual ~PluginPushToTalk() = default;

	virtual bool				fromGuiMakePluginOffer( VxNetIdent* netIdent, OfferBaseInfo& offerInfo ) override;
	virtual bool				fromGuiOfferReply( VxNetIdent* netIdent, OfferBaseInfo& offerInfo ) override;

	virtual bool				fromGuiIsPluginInSession( VxNetIdent* netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
	virtual void				fromGuiStartPluginSession( VxNetIdent* netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;
	virtual void				fromGuiStopPluginSession( VxNetIdent* netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() ) override;

	virtual bool				fromGuiInstMsg(	VxNetIdent*	netIdent, const char* pMsg ) override;
	virtual bool				fromGuiPushToTalk( VxNetIdent* netIdent, bool enableTalk ) override;

	virtual void				replaceConnection			( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt ) override;
protected:
	virtual void				onPktPluginOfferReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktPluginOfferReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual void				onPktChatReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual void				onPktPushToTalkReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktPushToTalkReply		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktPushToTalkStart		( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktPushToTalkStop         ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	
	virtual void				onPktSessionStopReq			( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

	virtual void				onPktVoiceReq				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;
	virtual void				onPktVoiceReply				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent ) override;

    virtual	void				onContactWentOffline		( VxNetIdent*	netIdent, VxSktBase* sktBase ) override;
    virtual	void				onConnectionLost			( VxSktBase* sktBase ) override;

    virtual void				onSessionStart				( PluginSessionBase* poSession, bool pluginIsLocked ) override;
    virtual void				onSessionEnded				( PluginSessionBase* poSession, bool pluginIsLocked, EOfferResponse offerResponse ) override;

protected:
    virtual void				callbackOpusPkt				( void * userData, PktVoiceReq * pktOpusAudio ) override;
    virtual void				callbackAudioOutSpaceAvail	( int freeSpaceLen ) override;

	PluginSessionMgr			m_PluginSessionMgr;
	PushToTalkFeedMgr			m_PushToTalkFeedMgr;
};



