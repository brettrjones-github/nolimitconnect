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

#include "PluginPushToTalk.h"
#include "PluginMgr.h"

#include "P2PSession.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktsPushToTalk.h>
#include <PktLib/PktChatReq.h>

#include <memory.h>
#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

//============================================================================
PluginPushToTalk::PluginPushToTalk( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
, m_PluginSessionMgr( engine, *this, pluginMgr )
, m_PushToTalkFeedMgr( engine, *this, m_PluginSessionMgr )
{
	m_ePluginType = ePluginTypePushToTalk;
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool PluginPushToTalk::fromGuiMakePluginOffer(		VxNetIdent *	netIdent,		
													int			    pvUserData,
													const char *	pOfferMsg,		
													const char *	pFileName,
													uint8_t *		fileHashId,
													VxGUID			lclSessionId )	
{
	bool result = false;
	P2PSession * poSession = 0;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( lclSessionId.isVxGUIDValid() )
	{
		poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionBySessionId( lclSessionId, true  );
	}
	else
	{
		poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	}

	if( poSession )
	{
		LogMsg( LOG_ERROR, "PluginPushToTalk already in session\n");
		// assume some error in logic
		m_PluginSessionMgr.removeSessionBySessionId( true, netIdent->getMyOnlineId() );
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
bool PluginPushToTalk::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_PluginSessionMgr.fromGuiIsPluginInSession( false, netIdent, pvUserData, lclSessionId );
}

//============================================================================
//! called to start service or session with remote friend
void PluginPushToTalk::fromGuiStartPluginSession( VxNetIdent * netIdent,int, VxGUID )
{
	m_PushToTalkFeedMgr.fromGuiStartPluginSession( false, netIdent );
}

//============================================================================
//! called to stop service or session with remote friend
void PluginPushToTalk::fromGuiStopPluginSession( VxNetIdent * netIdent, int, VxGUID )
{
	m_PushToTalkFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_PluginSessionMgr.fromGuiStopPluginSession( false, netIdent );
}

//============================================================================
//! handle reply to offer
bool PluginPushToTalk::fromGuiOfferReply(	VxNetIdent *	netIdent,
											int				pvUserData,
											EOfferResponse	eOfferResponse,
											VxGUID			lclSessionId )
{
	return m_PluginSessionMgr.fromGuiOfferReply(	false, 
													netIdent,
													pvUserData,				
													eOfferResponse, lclSessionId );
}

//============================================================================
bool PluginPushToTalk::fromGuiInstMsg(	VxNetIdent *	netIdent, 
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

	return false;
}

//============================================================================
void PluginPushToTalk::callbackOpusPkt( void * userData, PktPushToTalkReq * pktOpusAudio )
{
	m_PushToTalkFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginPushToTalk::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_PushToTalkFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginPushToTalk::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginPushToTalk::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.onPktPluginOfferReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginPushToTalk::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.onPktSessionStopReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginPushToTalk::onPktPushToTalkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PushToTalkFeedMgr.onPktPushToTalkReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginPushToTalk::onPktPushToTalkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PushToTalkFeedMgr.onPktPushToTalkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginPushToTalk::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktChatReq * poPkt = (PktChatReq *)pktHdr;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	P2PSession * poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		IToGui::getToGui().toGuiInstMsg( netIdent, m_ePluginType, (const char *)poPkt->getDataPayload() );
	}
}

//============================================================================
void PluginPushToTalk::onSessionStart( PluginSessionBase * session, bool pluginIsLocked )
{
	PluginBase::onSessionStart( session, pluginIsLocked ); // mark user session time so contact list is sorted with latest used on top
	m_PushToTalkFeedMgr.fromGuiStartPluginSession( pluginIsLocked, session->getIdent() );
}

//============================================================================
void PluginPushToTalk::onSessionEnded( PluginSessionBase * session, bool pluginIsLocked, EOfferResponse eOfferResponse )
{
	m_PushToTalkFeedMgr.fromGuiStopPluginSession( pluginIsLocked, session->getIdent() );
}

//============================================================================
void PluginPushToTalk::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginPushToTalk::onConnectionLost( VxSktBase * sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginPushToTalk::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_PushToTalkFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}

