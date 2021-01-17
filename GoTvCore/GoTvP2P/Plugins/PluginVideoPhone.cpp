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

#include "PluginVideoPhone.h"
#include "P2PSession.h"
#include "PluginMgr.h"

#include <PktLib/PktsVideoFeed.h>
#include <PktLib/PktChatReq.h>

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//#define DEBUG_AUTOPLUGIN_LOCK 1

//============================================================================
PluginVideoPhone::PluginVideoPhone( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBase( engine, pluginMgr, myIdent )
, m_PluginSessionMgr( *this, pluginMgr )
, m_VoiceFeedMgr( *this, m_PluginSessionMgr )
, m_VideoFeedMgr( *this, m_PluginSessionMgr )
{
	m_ePluginType = ePluginTypeVideoPhone;
}

//============================================================================
PluginVideoPhone::~PluginVideoPhone()
{
}

//============================================================================
bool PluginVideoPhone::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		
												int				pvUserData,
												const char *	pOfferMsg,		
												const char *	pFileName,
												uint8_t *			fileHashId,
												VxGUID			lclSessionId )	
{
	bool result = false;
	P2PSession * poSession = 0;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( lclSessionId.isVxGUIDValid() )
	{
		poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionBySessionId( lclSessionId, true );
		if( poSession )
		{
			#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_ERROR, "PluginVideoPhone already in session\n");
			#endif // DEBUG_AUTOPLUGIN_LOCK
			// assume some error in logic
			m_PluginSessionMgr.removeSessionBySessionId( true, lclSessionId );
		}
	}
	else
	{
		poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
		if( poSession )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_ERROR, "PluginVideoPhone already in session\n");
#endif // DEBUG_AUTOPLUGIN_LOCK
			// assume some error in logic
			m_PluginSessionMgr.removeRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
		}
	}

		
	result = m_PluginSessionMgr.fromGuiMakePluginOffer(	true,
														netIdent,
														pvUserData,	
														pOfferMsg,
														pFileName,
														fileHashId,
														lclSessionId );

	return result;
}

//============================================================================
bool PluginVideoPhone::fromGuiOfferReply(	VxNetIdent *	netIdent,
											int				pvUserData,
											EOfferResponse	eOfferResponse,
											VxGUID			lclSessionId )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::fromGuiOfferReply %d\n", eOfferResponse );
#endif // DEBUG_AUTOPLUGIN_LOCK
	bool sentReply = m_PluginSessionMgr.fromGuiOfferReply(	false, netIdent, pvUserData, eOfferResponse, lclSessionId );

	return sentReply;
}

//============================================================================
bool PluginVideoPhone::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_PluginSessionMgr.fromGuiIsPluginInSession( false, netIdent, pvUserData, lclSessionId );
}

//============================================================================
void PluginVideoPhone::fromGuiStartPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	m_VoiceFeedMgr.fromGuiStartPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStartPluginSession( false, &getEngine().getMyPktAnnounce() );
}

//============================================================================
void PluginVideoPhone::fromGuiStopPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::fromGuiStopPluginSession %s start\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	m_VoiceFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, &getEngine().getMyPktAnnounce() );
	m_PluginSessionMgr.fromGuiStopPluginSession( false, netIdent, pvUserData, lclSessionId );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::fromGuiStopPluginSession %s done\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
bool PluginVideoPhone::fromGuiInstMsg(	VxNetIdent *	netIdent, 
										const char *	pMsg )
{
	PluginBase::AutoPluginLock pluginMutexLock( this );
	P2PSession * poSession = m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktChatReq oPkt;
		oPkt.addMsg( pMsg );
		return m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &oPkt );
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginVideoPhone::fromGuiInstMsg session not found\n" );
		return false;
	}

	return false;
}

//============================================================================
void PluginVideoPhone::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktChatReq * pkt = (PktChatReq *)pktHdr;
	IToGui::getToGui().toGuiInstMsg( netIdent, m_ePluginType, (const char *)pkt->getDataPayload() );
}

//============================================================================
void PluginVideoPhone::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onPktPluginOfferReq %s start\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	m_PluginSessionMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onPktPluginOfferReq %s done\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PluginVideoPhone::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onPktPluginOfferReply %s start\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	m_PluginSessionMgr.onPktPluginOfferReply( sktBase, pktHdr, netIdent );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onPktPluginOfferReply %s done\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PluginVideoPhone::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginVideoPhone::onPktVideoFeedReq %s start\n", netIdent->getOnlineName() );
	m_VideoFeedMgr.onPktVideoFeedReq( sktBase, pktHdr, netIdent );
	//LogMsg( LOG_INFO, "PluginVideoPhone::onPktVideoFeedReq %s done\n", netIdent->getOnlineName() );
}

//============================================================================
void PluginVideoPhone::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedStatus( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVideoPhone::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginVideoPhone::onPktVideoFeedPic %s start\n", netIdent->getOnlineName() );
	m_VideoFeedMgr.onPktVideoFeedPic( sktBase, pktHdr, netIdent );
	//LogMsg( LOG_INFO, "PluginVideoPhone::onPktVideoFeedPic %s done\n", netIdent->getOnlineName() );
}

//============================================================================
void PluginVideoPhone::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPicChunk( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVideoPhone::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPicAck( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVideoPhone::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onPktSessionStopReq %s start\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	m_PluginSessionMgr.onPktSessionStopReq( sktBase, pktHdr, netIdent );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onPktSessionStopReq %s done\n", netIdent->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PluginVideoPhone::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginVideoPhone::onPktVoiceReq %s start\n", netIdent->getOnlineName() );
	m_VoiceFeedMgr.onPktVoiceReq( sktBase, pktHdr, netIdent );
	//LogMsg( LOG_INFO, "PluginVideoPhone::onPktVoiceReq %s done\n", netIdent->getOnlineName() );
}

//============================================================================
void PluginVideoPhone::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVideoPhone::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	m_VoiceFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginVideoPhone::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_VoiceFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginVideoPhone::callbackVideoPktPic( void * userData, VxGUID& onlineId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum )
{
	m_VideoFeedMgr.callbackVideoPktPic( onlineId, pktVid, pktsInSequence, thisPktNum );
}

//============================================================================
void PluginVideoPhone::callbackVideoPktPicChunk( void * userData, VxGUID& onlineId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum )
{
	m_VideoFeedMgr.callbackVideoPktPicChunk( onlineId, pktVid, pktsInSequence, thisPktNum );
}

//============================================================================
void PluginVideoPhone::onSessionStart( PluginSessionBase * session, bool pluginIsLocked )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onSessionStart %s start\n", session->getIdent()->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::onSessionStart( session, pluginIsLocked );; // mark user session time so contact list is sorted with latest used on top
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onSessionStart %s starting voice feed\n", session->getIdent()->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	m_VoiceFeedMgr.fromGuiStartPluginSession( pluginIsLocked, session->getIdent() );
	// in order to get my video packets to send out the ident has to be myself
	m_VideoFeedMgr.fromGuiStartPluginSession( pluginIsLocked, &getEngine().getMyPktAnnounce() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onSessionStart %s done\n", session->getIdent()->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PluginVideoPhone::onSessionEnded( PluginSessionBase * session, bool pluginIsLocked, EOfferResponse eOfferResponse )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onSessionStart %s STOP voice feed\n", session->getIdent()->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
	m_VoiceFeedMgr.fromGuiStopPluginSession( pluginIsLocked, session->getIdent() );
	m_VideoFeedMgr.fromGuiStopPluginSession( pluginIsLocked, session->getIdent() );
	m_VideoFeedMgr.fromGuiStopPluginSession( pluginIsLocked, &getEngine().getMyPktAnnounce() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginVideoPhone::onSessionStart %s done\n", session->getIdent()->getOnlineName() );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PluginVideoPhone::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginVideoPhone::onConnectionLost( VxSktBase * sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginVideoPhone::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, &getEngine().getMyPktAnnounce() );
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}
