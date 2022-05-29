//============================================================================
// Copyright (C) 2020 Brett R. Jones
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

#include "PluginBaseMultimedia.h"
#include "P2PSession.h"
#include "PluginMgr.h"

#include <PktLib/PktsVideoFeed.h>
#include <PktLib/PktsMultiSession.h>
#include <PktLib/PktsTodGame.h>

#include <GuiInterface/IToGui.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetMgr.h>
#include <ptop_src/ptop_engine_src/AssetMgr/AssetXferMgr.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
PluginBaseMultimedia::PluginBaseMultimedia( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
, m_PluginSessionMgr( engine, *this, pluginMgr )
, m_VoiceFeedMgr( engine, *this, m_PluginSessionMgr )
, m_VideoFeedMgr( engine, *this, m_PluginSessionMgr )
, m_AssetXferMgr( engine, engine.getAssetMgr(), *this, "MessengerAssets.db3", "MessengerXfer.db3" )
{
}

//============================================================================
void PluginBaseMultimedia::fromGuiUserLoggedOn( void )
{
	m_AssetXferMgr.fromGuiUserLoggedOn();
}

//============================================================================
bool PluginBaseMultimedia::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		
													int				pvUserData,
													const char *	pOfferMsg,		
													const char *	pFileName,
													uint8_t *		fileHashId,
													VxGUID			lclSessionId )	
{
	bool result = false;
	P2PSession * poSession = 0;
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiMakePluginOffer autoLock start\n" );
	PluginBase::AutoPluginLock pluginMutexLock( this );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSetGameValueVar autoLock done\n" );

	poSession = (P2PSession *)m_PluginSessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		LogMsg( LOG_ERROR, "PluginBaseMultimedia already in session\n");
		result = true;
	}
	else
	{
		const char * offerMsg = pOfferMsg;
		if( ( 0 == offerMsg ) || ( 0 == strlen( offerMsg ) ) )
		{
			offerMsg = "Text Chat";
		}

		result = m_PluginSessionMgr.fromGuiMakePluginOffer(	true, 
															netIdent,
															pvUserData,	
															offerMsg,
															pFileName,
															fileHashId,
															lclSessionId );
	}

	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiMakePluginOffer autoLock destroy\n" );
	return result;
}

//============================================================================
bool PluginBaseMultimedia::fromGuiOfferReply(	VxNetIdent *	netIdent,
											    int				pvUserData,				
											    EOfferResponse	eOfferResponse,
											    VxGUID			lclSessionId )
{
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiOfferReply start\n" );
	bool result = m_PluginSessionMgr.fromGuiOfferReply( false,	netIdent, pvUserData, eOfferResponse, lclSessionId );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiOfferReply done\n" );
	return result;
}

//============================================================================
void PluginBaseMultimedia::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktSessionStopReq start\n" );
	m_PluginSessionMgr.onPktSessionStopReq( sktBase, pktHdr, netIdent );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktSessionStopReq done\n" );
}

//============================================================================
bool PluginBaseMultimedia::fromGuiSendAsset( AssetBaseInfo& assetInfo )
{
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSendAsset start\n" );
	return m_AssetXferMgr.fromGuiSendAssetBase( assetInfo );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSendAsset done\n" );
}

//============================================================================
bool PluginBaseMultimedia::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	return m_PluginSessionMgr.fromGuiIsPluginInSession( false, netIdent, pvUserData, lclSessionId );
}

//============================================================================
//! called to start service or session with remote friend
void PluginBaseMultimedia::fromGuiStartPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
}

//============================================================================
//! called to stop service or session with remote friend
void PluginBaseMultimedia::fromGuiStopPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiStopPluginSession start\n" );
	m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( true, &getEngine().getMyPktAnnounce() );

	m_PluginSessionMgr.fromGuiStopPluginSession( false, netIdent, pvUserData, lclSessionId );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiStopPluginSession done\n" );
}

//============================================================================
bool PluginBaseMultimedia::fromGuiMultiSessionAction( VxNetIdent * netIdent, EMSessionAction mSessionAction, int pos0to100000, VxGUID lclSessionId )
{
	LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiMultiSessionAction action %d \n", mSessionAction );
	bool sendSucces = false;

	if( eMSessionActionAccept == mSessionAction )
	{
		switch( pos0to100000 )
		{
		case eMSessionTypePhone:
			m_VoiceFeedMgr.fromGuiStartPluginSession( true, netIdent );
			break;

		case eMSessionTypeVidChat:
			m_VoiceFeedMgr.fromGuiStartPluginSession(  true, netIdent );
			m_VideoFeedMgr.fromGuiStartPluginSession(  true, &getEngine().getMyPktAnnounce() );
			break;

		case eMSessionTypeTruthOrDare:
			m_VoiceFeedMgr.fromGuiStartPluginSession(  true, netIdent );
			m_VideoFeedMgr.fromGuiStartPluginSession(  true, &getEngine().getMyPktAnnounce() );
			break;
		}
	}
	else if(  eMSessionActionHangup == mSessionAction )
	{
		switch( pos0to100000 )
		{
		case eMSessionTypePhone:
			m_VoiceFeedMgr.fromGuiStopPluginSession(  true, netIdent );
			break;

		case eMSessionTypeVidChat:
			m_VoiceFeedMgr.fromGuiStopPluginSession(  true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession(  true, &getEngine().getMyPktAnnounce() );
			break;

		case eMSessionTypeTruthOrDare:
			m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, &getEngine().getMyPktAnnounce() );
			break;
		}
	}

	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiMultiSessionAction autoLock start\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( this );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiMultiSessionAction autoLock done\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	PluginSessionBase * poSession = m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( 0 == poSession )
	{
		LogMsg( LOG_ERROR, "ERROR PluginBaseMultimedia::fromGuiMultiSessionAction missing plugin session \n" );
	}
	else
	{
		PktMultiSessionReq pktReq;
		pktReq.setMSessionAction( (uint32_t)mSessionAction );
		pktReq.setMSessionParam( (uint32_t)pos0to100000 );
		sendSucces = m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &pktReq );
	}

	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiMultiSessionAction autoLock destroy\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	return sendSucces;
}

//============================================================================
bool PluginBaseMultimedia::fromGuiSetGameValueVar( VxNetIdent * netIdent, int32_t varId, int32_t varValue )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSetGameValueVar autoLock start\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	bool sendSucces = false;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSetGameValueVar autoLock done\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	PluginSessionBase * poSession = m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktTodGameValue pktGameValue;
		pktGameValue.setValue( (ETodGameVarId) varId, varValue );
		sendSucces = m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &pktGameValue );
	}

	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSetGameValueVar autoLock destroy\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	return sendSucces;
}

//============================================================================
bool PluginBaseMultimedia::fromGuiSetGameActionVar( VxNetIdent * netIdent, int32_t actionId, int32_t actionValue )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSetGameActionVar autoLock start\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	bool sendSucces = false;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	PluginSessionBase * poSession = m_PluginSessionMgr.findPluginSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktTodGameAction pktGameAction;
		pktGameAction.setAction( (ETodGameAction) actionId, actionValue );
		sendSucces = m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), poSession->getSkt(), &pktGameAction );
	}

	#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_INFO, "PluginBaseMultimedia::fromGuiSetGameActionVar autoLock done\n" );
	#endif //DEBUG_AUTOPLUGIN_LOCK
	return sendSucces;
}

//============================================================================
void PluginBaseMultimedia::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	m_VoiceFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginBaseMultimedia::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_VoiceFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginBaseMultimedia::callbackVideoPktPic( void * /*userData*/, VxGUID& onlineId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum )
{
	m_VideoFeedMgr.callbackVideoPktPic( onlineId, pktVid, pktsInSequence, thisPktNum );
}

//============================================================================
void PluginBaseMultimedia::callbackVideoPktPicChunk( void * /*userData*/, VxGUID& onlineId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum ) 
{
	m_VideoFeedMgr.callbackVideoPktPicChunk( onlineId, pktVid, pktsInSequence, thisPktNum );
}

//============================================================================
void PluginBaseMultimedia::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktPluginOfferReq start\n" );
	m_PluginSessionMgr.onPktPluginOfferReq( sktBase, pktHdr, netIdent );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktPluginOfferReq done\n" );
}

//============================================================================
void PluginBaseMultimedia::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktPluginOfferReply start\n" );
	m_PluginSessionMgr.onPktPluginOfferReply( sktBase, pktHdr, netIdent );
	//LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktPluginOfferReply done\n" );
}

//============================================================================
void PluginBaseMultimedia::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedStatus( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPic( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPicChunk( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VideoFeedMgr.onPktVideoFeedPicAck( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginBaseMultimedia::onPktAssetSendReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendReq start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetSendReq( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendReq done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktAssetSendReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendReply start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetSendReply( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendReply done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktAssetChunkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetChunkReq start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetChunkReq( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetChunkReq done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktAssetChunkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetChunkReply start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetChunkReply( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetChunkReply done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktAssetSendCompleteReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendCompleteReq start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetSendCompleteReq( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendCompleteReq done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktAssetSendCompleteReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendCompleteReply start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetSendCompleteReply( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetSendCompleteReply done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktAssetXferErr( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetXferErr start\n" );
#endif // DEBUG_SEND_ASSET
	m_AssetXferMgr.onPktAssetXferErr( sktBase, pktHdr, netIdent );
#ifdef DEBUG_SEND_ASSET
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktAssetXferErr done\n" );
#endif // DEBUG_SEND_ASSET
}

//============================================================================
void PluginBaseMultimedia::onPktMultiSessionReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktMultiSessionReq * pktReq			= (PktMultiSessionReq *)pktHdr;
	EMSessionAction eMSessionAction		= ( EMSessionAction )pktReq->getMSessionAction();
	EMSessionType eMSessionType			= ( EMSessionType )pktReq->getMSessionParam(); 

	if( eMSessionActionAccept == eMSessionAction )
	{
		switch( eMSessionType )
		{
		case eMSessionTypePhone:
			m_VoiceFeedMgr.fromGuiStartPluginSession( true, netIdent );
			break;

		case eMSessionTypeVidChat:
			m_VoiceFeedMgr.fromGuiStartPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStartPluginSession( true, &getEngine().getMyPktAnnounce());
			break;

		case eMSessionTypeTruthOrDare:
			m_VoiceFeedMgr.fromGuiStartPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStartPluginSession( true, &getEngine().getMyPktAnnounce() );
			break;
		default:
			break;
		}
	}
	else if(  eMSessionActionHangup == eMSessionAction )
	{
		switch( eMSessionType )
		{
		case eMSessionTypePhone:
			m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
			break;

		case eMSessionTypeVidChat:
			m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, &getEngine().getMyPktAnnounce() );
			break;

		case eMSessionTypeTruthOrDare:
			m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, netIdent );
			m_VideoFeedMgr.fromGuiStopPluginSession( true, &getEngine().getMyPktAnnounce() );
			break;
		default:
			break;
		}
	}

	IToGui::getToGui().toGuiMultiSessionAction( eMSessionAction, netIdent->getMyOnlineId(), pktReq->getMSessionParam() );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktMultiSessionReq autoLock start\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( this );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktMultiSessionReq autoLock done\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	P2PSession * poSession = m_PluginSessionMgr.findOrCreateP2PSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
	poSession->setSkt( sktBase );

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktMultiSessionReq autoLock destroy\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PluginBaseMultimedia::onPktMultiSessionReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktMultiSessionReply start\n" );
	PktMultiSessionReply * pktReply = (PktMultiSessionReply *)pktHdr;

	IToGui::getToGui().toGuiMultiSessionAction( ( EMSessionAction )pktReply->getMSessionAction(), netIdent->getMyOnlineId(), pktReply->getMSessionParam() );
	LogMsg( LOG_INFO, "PluginBaseMultimedia::onPktMultiSessionReply done\n" );
}

//============================================================================
void PluginBaseMultimedia::onPktTodGameStats( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktTodGameStats * poPkt = (PktTodGameStats *)pktHdr;
	for( int i = 0; i < eMaxTodGameStatId; ++i )
	{
		IToGui::getToGui().toGuiSetGameValueVar( m_ePluginType, netIdent->getMyOnlineId(), i, poPkt->getVar((ETodGameVarId)i) );
	}

	IToGui::getToGui().toGuiSetGameActionVar( m_ePluginType, netIdent->getMyOnlineId(), eTodGameActionSendStats, 1 );
}

//============================================================================
void PluginBaseMultimedia::onPktTodGameAction( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktTodGameAction * poPkt = (PktTodGameAction *)pktHdr;
	IToGui::getToGui().toGuiSetGameActionVar( m_ePluginType, netIdent->getMyOnlineId(), poPkt->getActionVarId(), poPkt->getActionVarValue() );
}

//============================================================================
void PluginBaseMultimedia::onPktTodGameValue( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktTodGameValue * poPkt = (PktTodGameValue *)pktHdr;
	IToGui::getToGui().toGuiSetGameValueVar( m_ePluginType, netIdent->getMyOnlineId(), poPkt->getValueVarId(), poPkt->getValueVar() );
}

//============================================================================
void PluginBaseMultimedia::onSessionStart( PluginSessionBase * session, bool pluginIsLocked )
{
	PluginBase::onSessionStart( session, pluginIsLocked ); // mark user session time so contact list is sorted with latest used on top

	//NOTE: for chat session the video and audio is started/stopped through MSession commands
	//m_VoiceFeedMgr.fromGuiStartPluginSession( session->getIdent() );
	// in order to get my video packets to send out the ident has to by myself
	//m_VideoFeedMgr.fromGuiStartPluginSession( &getEngine().getMyPktAnnounce() );
}

//============================================================================
void PluginBaseMultimedia::onSessionEnded( PluginSessionBase * session, bool pluginIsLocked, EOfferResponse eOfferResponse )
{
	m_VoiceFeedMgr.fromGuiStopPluginSession( true, session->getIdent() );
	m_VideoFeedMgr.fromGuiStopPluginSession( true, session->getIdent() );
	m_VideoFeedMgr.fromGuiStopPluginSession( true, &getEngine().getMyPktAnnounce() );
}

//============================================================================
void PluginBaseMultimedia::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
#ifdef DEBUG_SKT_CONNECTIONS
	LogMsg( LOG_INFO, "PluginBaseMultimedia::replaceConnection start\n" );
#endif // DEBUG_SKT_CONNECTIONS
	m_AssetXferMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
#ifdef DEBUG_SKT_CONNECTIONS
    LogMsg( LOG_INFO, "PluginBaseMultimedia::replaceConnection done\n" );
#endif // DEBUG_SKT_CONNECTIONS
}

//============================================================================
void PluginBaseMultimedia::onConnectionLost( VxSktBase * sktBase )
{
#ifdef DEBUG_SKT_CONNECTIONS
    LogMsg( LOG_INFO, "PluginBaseMultimedia::onConnectionLost start\n" );
#endif // DEBUG_SKT_CONNECTIONS
	m_AssetXferMgr.onConnectionLost( sktBase );
	m_PluginSessionMgr.onConnectionLost( sktBase );
#ifdef DEBUG_SKT_CONNECTIONS
    LogMsg( LOG_INFO, "PluginBaseMultimedia::onConnectionLost done\n" );
#endif // DEBUG_SKT_CONNECTIONS
}

//============================================================================
void PluginBaseMultimedia::onContactWentOnline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_AssetXferMgr.onContactWentOnline( netIdent, sktBase );
}

//============================================================================
void PluginBaseMultimedia::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	LogModule( eLogPlugins, LOG_INFO, "PluginBaseMultimedia::onContactWentOffline start" );
	m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( true, netIdent );
	m_VideoFeedMgr.fromGuiStopPluginSession( true, &getEngine().getMyPktAnnounce() );
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
    LogModule( eLogPlugins, LOG_INFO, "PluginBaseMultimedia::onContactWentOffline done" );
}
