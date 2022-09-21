#ifndef VIDEO_FEED_MGR_H
#define VIDEO_FEED_MGR_H
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

class VideoFeedMgr
{
public:
	VideoFeedMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr );
	virtual ~VideoFeedMgr() = default;

	virtual void				fromGuiStartPluginSession( bool pluginIsLocked, EAppModule appModule, VxNetIdent * netIdent = NULL );
	virtual void				fromGuiStopPluginSession( bool pluginIsLocked, EAppModule appModule, VxNetIdent * netIdent = NULL );

	virtual void				onPktVideoFeedReq			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedStatus		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPic			( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPicChunk		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );
	virtual void				onPktVideoFeedPicAck		( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent );

	//virtual void				callbackVideoJpgSmall(	void * userData, VxGUID& feedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 );
	virtual void				callbackVideoPktPic( VxGUID& feedId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum );
	virtual void				callbackVideoPktPicChunk( VxGUID& feedId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum );

	void						stopAllSessions( EAppModule appModule, EPluginType pluginType );

protected:
	void						enableVideoCapture( bool bStart, VxNetIdent * netIdent, EAppModule appModule );

    P2PEngine&                  m_Engine;
	PluginBase&					m_Plugin;
	PluginMgr&					m_PluginMgr;
	PluginSessionMgr&			m_SessionMgr;
	VxGUIDList					m_GuidList;
	bool						m_CamServerEnabled;
	bool						m_VideoPktsRequested;
	bool						m_VideoJpgRequesed;
};

#endif // VIDEO_FEED_MGR_H
