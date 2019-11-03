#ifndef VOICE_FEED_MGR_H
#define VOICE_FEED_MGR_H
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
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/MediaCallbackInterface.h>
#include <CoreLib/VxGUID.h>
#include <CoreLib/VxGUIDList.h>

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
	VoiceFeedMgr( PluginBase& plugin, PluginSessionMgr& sessionMgr );

	virtual void				fromGuiStartPluginSession( bool pluginIsLocked, VxNetIdent * netIdent = NULL );
	virtual void				fromGuiStopPluginSession( bool pluginIsLocked, VxNetIdent * netIdent = NULL );

	virtual void				onPktVoiceReq				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVoiceReply				( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	virtual void				callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio );
	virtual void				callbackAudioOutSpaceAvail( int freeSpaceLen );

protected:
	void						enableAudioCapture( bool enable, VxNetIdent * netIdent );

	PluginBase&					m_Plugin;
	PluginMgr&					m_PluginMgr;
	PluginSessionMgr&			m_SessionMgr;
	bool						m_Enabled;
	VxGUIDList					m_GuidList;
	bool						m_CamServerEnabled;
	bool						m_AudioPktsRequested;
	bool						m_MixerInputRequesed;

};

#endif // VOICE_FEED_MGR_H
