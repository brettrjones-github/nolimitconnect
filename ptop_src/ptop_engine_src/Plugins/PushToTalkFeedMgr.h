#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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

#include <GuiInterface/IDefs.h>

#include <CoreLib/MediaCallbackInterface.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxGUIDList.h>

class P2PEngine;
class PluginBase;
class PluginMgr;
class PluginSessionMgr;
class VxNetIdent;
class VxSktBase;
class VxPktHdr;
class PktPushToTalkReq;

class PushToTalkFeedMgr
{
public:
	PushToTalkFeedMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr );

	virtual bool				fromGuiPushToTalk( VxNetIdent* netIdent, bool enableTalk, VxSktBase* sktBase );

	virtual void				onPktPushToTalkReq					( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPushToTalkReply				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPushToTalkStart				( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktPushToTalkStop                 ( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

	virtual void				onPktVoiceReq						( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
	virtual void				onPktVoiceReply						( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

    virtual void				callbackOpusPkt( void * userData, PktVoiceReq* pktOpusAudio );
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

	virtual void				onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual void				removePushToTalkUser( VxGUID& onlineId, bool txOnly = false );

	virtual bool				sendPushToTalkStop( VxNetIdent* netIdent, VxSktBase* sktBase );
	virtual bool				sendPushToTalkReq( VxNetIdent* netIdent, VxSktBase* sktBase );

protected:
	bool						enableAudioCapture( bool enable, VxNetIdent* netIdent, EAppModule appModule, VxSktBase* sktBase );
	void						updatePushToTalkStatus( VxGUID& onlineId );

    P2PEngine&                  m_Engine;
	PluginBase&					m_Plugin;
	PluginMgr&					m_PluginMgr;
	PluginSessionMgr&			m_SessionMgr;

	VxGUIDList					m_TxOnlineIdList;
	VxGUIDList					m_RxOnlineIdList;

	bool						m_AudioPktsRequested{ false };
	bool						m_MixerInputRequesed{ false };
};

