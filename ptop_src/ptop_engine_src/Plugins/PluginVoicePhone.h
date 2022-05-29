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

#include <PktLib/VxCommon.h>

class PluginVoicePhone : public PluginBase
{
public:
	PluginVoicePhone( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType );
	virtual ~PluginVoicePhone() = default;

	virtual bool				fromGuiMakePluginOffer( VxNetIdent *	netIdent,				
														int				pvUserData,
														const char *	pOfferMsg,				
														const char *	pFileName = NULL,
														uint8_t *		fileHashId = 0,
														VxGUID			lclSessionId = VxGUID::nullVxGUID() );		

	virtual bool				fromGuiOfferReply(	VxNetIdent *	netIdent,
													int				pvUserdata,
													EOfferResponse	eOfferResponse,
													VxGUID			lclSessionId );

	virtual bool				fromGuiIsPluginInSession( VxNetIdent * netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStartPluginSession( VxNetIdent * netIdent = nullptr, int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );
	virtual void				fromGuiStopPluginSession( VxNetIdent * netIdent = nullptr,  int pvUserData = 0, VxGUID lclSessionId = VxGUID::nullVxGUID() );

	virtual bool				fromGuiInstMsg(		VxNetIdent *	netIdent, 
													const char *	pMsg ); 

	virtual void				replaceConnection			( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt );

protected:
	virtual void				onPktPluginOfferReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktPluginOfferReply		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktChatReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				onPktVoiceReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVoiceReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	
	virtual void				onPktSessionStopReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual	void				onContactWentOffline		( VxNetIdent *	netIdent, VxSktBase * sktBase );
	virtual	void				onConnectionLost			( VxSktBase * sktBase );

	virtual void				onSessionStart				( PluginSessionBase * poSession, bool pluginIsLocked );
	virtual void				onSessionEnded				( PluginSessionBase * poSession, bool pluginIsLocked, EOfferResponse eOfferResponse );

protected:
	virtual void				callbackOpusPkt				( void * userData, PktVoiceReq * pktOpusAudio );
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

	PluginSessionMgr			m_PluginSessionMgr;
	VoiceFeedMgr				m_VoiceFeedMgr;
};



