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
class PktVoiceReq;

class VoiceFeedMgr
{
public:
	VoiceFeedMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr );

	virtual void				fromGuiStartPluginSession( bool pluginIsLocked, EAppModule appModule, VxNetIdent * netIdent = nullptr, bool wantAudioCapture = true );
	virtual void				fromGuiStopPluginSession( bool pluginIsLocked, EAppModule appModule, VxNetIdent * netIdent = nullptr, bool wantAudioCapture = true );

	virtual void				onPktVoiceReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVoiceReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio );
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

	void						stopAllSessions( EAppModule appModule, EPluginType pluginType );

protected:
	void						enableAudioCapture( bool enable, VxNetIdent* netIdent, EAppModule appModule, bool wantAudioCapture = true );

    P2PEngine&                  m_Engine;
	PluginBase&					m_Plugin;
	PluginMgr&					m_PluginMgr;
	PluginSessionMgr&			m_SessionMgr;
	bool						m_Enabled;
	VxGUIDList					m_GuidList;
	bool						m_CamServerEnabled;
	bool						m_AudioPktsRequested;
	bool						m_MixerInputRequesed;

};

