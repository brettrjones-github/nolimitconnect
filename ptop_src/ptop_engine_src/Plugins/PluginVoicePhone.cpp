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

#include "PluginVoicePhone.h"
#include "PluginMgr.h"

#include "P2PSession.h"

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <PktLib/PktVoiceReq.h>
#include <PktLib/PktVoiceReply.h>
#include <PktLib/PktChatReq.h>

#include <memory.h>
#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif

//============================================================================
PluginVoicePhone::PluginVoicePhone( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
, m_PluginSessionMgr( engine, *this, pluginMgr )
, m_VoiceFeedMgr( engine, *this, m_PluginSessionMgr )
{
	m_ePluginType = ePluginTypeVoicePhone;
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool PluginVoicePhone::fromGuiMakePluginOffer( VxNetIdent* netIdent, OfferBaseInfo& offerInfo, VxGUID& lclSessionId )
{
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
		LogMsg( LOG_ERROR, "PluginVoicePhone already in session");
		// assume some error in logic
		m_PluginSessionMgr.removeSessionBySessionId( true, netIdent->getMyOnlineId() );
	}

	return m_PluginSessionMgr.fromGuiMakePluginOffer( true, netIdent, offerInfo, lclSessionId );
}

//============================================================================
bool PluginVoicePhone::fromGuiIsPluginInSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_PluginSessionMgr.fromGuiIsPluginInSession( false, netIdent, pvUserData, lclSessionId );
}

//============================================================================
//! called to start service or session with remote friend
void PluginVoicePhone::fromGuiStartPluginSession( VxNetIdent* netIdent,int, VxGUID )
{
	m_VoiceFeedMgr.fromGuiStartPluginSession( false, eAppModuleVoicePhone, netIdent );
}

//============================================================================
//! called to stop service or session with remote friend
void PluginVoicePhone::fromGuiStopPluginSession( VxNetIdent* netIdent, int, VxGUID )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( false, eAppModuleVoicePhone, netIdent );
	m_PluginSessionMgr.fromGuiStopPluginSession( false, netIdent );
}

//============================================================================
//! handle reply to offer
bool PluginVoicePhone::fromGuiOfferReply( VxNetIdent* netIdent, OfferBaseInfo& offerInfo, VxGUID& lclSessionId, EOfferResponse offerResponse )
{
	return m_PluginSessionMgr.fromGuiOfferReply( false, netIdent, offerInfo, lclSessionId, offerResponse );
}

//============================================================================
bool PluginVoicePhone::fromGuiInstMsg( VxNetIdent* netIdent, const char* pMsg )
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
void PluginVoicePhone::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	m_VoiceFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginVoicePhone::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_VoiceFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginVoicePhone::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_PluginSessionMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVoicePhone::onPktPluginOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_PluginSessionMgr.onPktPluginOfferReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVoicePhone::onPktSessionStopReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_PluginSessionMgr.onPktSessionStopReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVoicePhone::onPktVoiceReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVoicePhone::onPktVoiceReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginVoicePhone::onPktChatReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktChatReq* poPkt = (PktChatReq *)pktHdr;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	P2PSession * poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		IToGui::getToGui().toGuiInstMsg( netIdent, m_ePluginType, (const char*)poPkt->getDataPayload() );
	}
}

//============================================================================
void PluginVoicePhone::onSessionStart( PluginSessionBase* session, bool pluginIsLocked )
{
	PluginBase::onSessionStart( session, pluginIsLocked ); // mark user session time so contact list is sorted with latest used on top
	m_VoiceFeedMgr.fromGuiStartPluginSession( pluginIsLocked, eAppModuleVoicePhone, session->getIdent() );
}

//============================================================================
void PluginVoicePhone::onSessionEnded( PluginSessionBase* session, bool pluginIsLocked, EOfferResponse offerResponse )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( pluginIsLocked, eAppModuleVoicePhone, session->getIdent() );
}

//============================================================================
void PluginVoicePhone::replaceConnection( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginVoicePhone::onConnectionLost( VxSktBase* sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginVoicePhone::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( false, eAppModuleVoicePhone, netIdent );
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}

