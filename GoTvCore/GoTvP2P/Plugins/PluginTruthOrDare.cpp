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

#include <PktLib/VxCommon.h>

#include "PluginTruthOrDare.h"
#include "PluginMgr.h"

#include "TodGameSession.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <PktLib/PktsTodGame.h>
#include <PktLib/PktChatReq.h>

#include <memory.h>
#ifdef _MSC_VER
# pragma warning( disable: 4355 ) //'this' : used in base member initializer list
#endif // _MSC_VER

//============================================================================
PluginTruthOrDare::PluginTruthOrDare( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBase( engine, pluginMgr, myIdent )
, m_PluginSessionMgr( engine, *this, pluginMgr )
, m_VoiceFeedMgr( engine, *this, m_PluginSessionMgr )
, m_VideoFeedMgr( engine, *this, m_PluginSessionMgr )
{
	m_ePluginType = ePluginTypeTruthOrDare;
}

//============================================================================
P2PSession * PluginTruthOrDare::createP2PSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	return new TodGameSession( sktBase, netIdent );
}

//============================================================================
bool PluginTruthOrDare::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		
												int			pvUserData,
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
	}
	else
	{
		poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	}

	if( poSession )
	{
		LogMsg( LOG_ERROR, "PluginTruthOrDare already in session\n");
		// assume some error in logic
		m_PluginSessionMgr.removeSessionBySessionId( true, netIdent->getMyOnlineId() );
	}
		
	result = m_PluginSessionMgr.fromGuiMakePluginOffer(		true,
															netIdent,
															pvUserData,	
															pOfferMsg,
															pFileName,
															fileHashId,
															lclSessionId );

	return result;
}

//============================================================================
bool PluginTruthOrDare::fromGuiOfferReply(	VxNetIdent *	netIdent,
											int			    pvUserData,				
											EOfferResponse	eOfferResponse,
											VxGUID			lclSessionId )
{
	return m_PluginSessionMgr.fromGuiOfferReply( false, netIdent, pvUserData, eOfferResponse, lclSessionId );
}

//============================================================================
bool PluginTruthOrDare::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_PluginSessionMgr.fromGuiIsPluginInSession( false, netIdent, pvUserData, lclSessionId );
}

//============================================================================
//! called to start service or session with remote friend
void PluginTruthOrDare::fromGuiStartPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	m_VoiceFeedMgr.fromGuiStartPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStartPluginSession( false, netIdent );
}

//============================================================================
//! called to stop service or session with remote friend
void PluginTruthOrDare::fromGuiStopPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, &getEngine().getMyPktAnnounce() );
	m_PluginSessionMgr.fromGuiStopPluginSession( false, netIdent, pvUserData, lclSessionId );
}

//============================================================================
bool PluginTruthOrDare::fromGuiInstMsg( VxNetIdent * netIdent, const char * msg )
{
	LogMsg( LOG_ERROR, "PluginServiceWebCam::fromGuiInstMsg\n" );
	PluginBase::AutoPluginLock pluginMutexLock( this );
	P2PSession * poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktChatReq pkt;
		pkt.addMsg( msg );
		return m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &pkt );
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginTruthOrDare::fromGuiInstMsg session not found\n" );
		return false;
	}
}

//============================================================================
bool PluginTruthOrDare::fromGuiSetGameValueVar( VxNetIdent * netIdent, int32_t varId, int32_t varValue )
{
	PluginBase::AutoPluginLock pluginMutexLock( this );
	PluginSessionBase * poSession = m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktTodGameValue pktGameValue;
		pktGameValue.setValue( (ETodGameVarId) varId, varValue );
		return m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &pktGameValue );
	}

	return false;
}

//============================================================================
bool PluginTruthOrDare::fromGuiSetGameActionVar( VxNetIdent * netIdent, int32_t actionId, int32_t actionValue )
{
	PluginBase::AutoPluginLock pluginMutexLock( this );
	PluginSessionBase * poSession = m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktTodGameAction pktGameAction;
		pktGameAction.setAction( (ETodGameAction) actionId, actionValue );
		return m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &pktGameAction );
	}

	return false;
}

//============================================================================
void PluginTruthOrDare::onPktChatReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktChatReq * pkt = (PktChatReq *)pktHdr;
	IToGui::getToGui().toGuiInstMsg( netIdent, m_ePluginType, (const char *)pkt->getDataPayload() );
}

//============================================================================
void PluginTruthOrDare::onPktTodGameStats( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktTodGameStats * poPkt = (PktTodGameStats *)pktHdr;
	for( int i = 0; i < eMaxTodGameStatId; ++i )
	{
		IToGui::getToGui().toGuiSetGameValueVar( m_ePluginType, netIdent->getMyOnlineId(), i, poPkt->getVar((ETodGameVarId)i) );
	}

	IToGui::getToGui().toGuiSetGameActionVar( m_ePluginType, netIdent->getMyOnlineId(), eTodGameActionSendStats, 1 );
}

//============================================================================
void PluginTruthOrDare::onPktTodGameAction( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktTodGameAction * poPkt = (PktTodGameAction *)pktHdr;
	IToGui::getToGui().toGuiSetGameActionVar( m_ePluginType, netIdent->getMyOnlineId(), poPkt->getActionVarId(), poPkt->getActionVarValue() );
}

//============================================================================
void PluginTruthOrDare::onPktTodGameValue( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktTodGameValue * poPkt = (PktTodGameValue *)pktHdr;
	IToGui::getToGui().toGuiSetGameValueVar( m_ePluginType, netIdent->getMyOnlineId(), poPkt->getValueVarId(), poPkt->getValueVar() );
}

//============================================================================
void PluginTruthOrDare::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.onPktPluginOfferReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedStatus( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPic( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPicChunk( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPicAck( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	m_VoiceFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginTruthOrDare::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_VoiceFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginTruthOrDare::callbackVideoPktPic( void * userData, VxGUID& onlineId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum )
{
	m_VideoFeedMgr.callbackVideoPktPic( onlineId, pktVid, pktsInSequence, thisPktNum );
}

//============================================================================
void PluginTruthOrDare::callbackVideoPktPicChunk( void * userData, VxGUID& onlineId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum ) 
{
	m_VideoFeedMgr.callbackVideoPktPicChunk( onlineId, pktVid, pktsInSequence, thisPktNum );
}

//============================================================================
void PluginTruthOrDare::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.onPktSessionStopReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginTruthOrDare::onSessionStart( PluginSessionBase * session, bool pluginIsLocked )
{
	PluginBase::onSessionStart( session, pluginIsLocked ); // mark user session time so contact list is sorted with latest used on top
	m_VoiceFeedMgr.fromGuiStartPluginSession( pluginIsLocked, session->getIdent() );
	// in order to get my video packets to send out the ident has to be myself
	m_VideoFeedMgr.fromGuiStartPluginSession( pluginIsLocked, &getEngine().getMyPktAnnounce() );
}

//============================================================================
void PluginTruthOrDare::onSessionEnded( PluginSessionBase * session, bool pluginIsLocked, EOfferResponse eOfferResponse )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( pluginIsLocked, session->getIdent() );
	m_VideoFeedMgr.fromGuiStopPluginSession( pluginIsLocked, session->getIdent() );
	m_VideoFeedMgr.fromGuiStopPluginSession( pluginIsLocked, &getEngine().getMyPktAnnounce() );
}

//============================================================================
void PluginTruthOrDare::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginTruthOrDare::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( false, &getEngine().getMyPktAnnounce() );
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}

//============================================================================
void PluginTruthOrDare::onConnectionLost( VxSktBase * sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}



