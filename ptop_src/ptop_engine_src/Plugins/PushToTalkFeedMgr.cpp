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

#include "PushToTalkFeedMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"
#include "PluginSessionMgr.h"
#include "P2PSession.h"
#include "PluginCamServer.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <MediaToolsLib/OpusAudioDecoder.h>

#include <PktLib/PktsPushToTalk.h>
#include <NetLib/VxSktBase.h>

#include <memory.h>

//#define DEBUG_AUTOPLUGIN_LOCK 1

//============================================================================
PushToTalkFeedMgr::PushToTalkFeedMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr )
: m_Engine( engine )
, m_Plugin( plugin )
, m_PluginMgr( engine.getPluginMgr() )
, m_SessionMgr( sessionMgr )
{
}

//============================================================================
bool PushToTalkFeedMgr::fromGuiPushToTalk( VxNetIdent* netIdent, bool enableTalk, VxSktBase* sktBase )
{
	return enableAudioCapture( enableTalk, netIdent, eAppModulePushToTalk, sktBase );
}

//============================================================================
bool PushToTalkFeedMgr::enableAudioCapture( bool enable, VxNetIdent* netIdent, EAppModule appModule, VxSktBase* sktBase )
{
	if( enable && sktBase )
	{
		if( m_TxOnlineIdList.addGuidIfDoesntExist( netIdent->getMyOnlineId() ) )
		{
			if( sendPushToTalkReq( netIdent, sktBase ) )
			{
				if( !m_AudioPktsRequested )
				{
					//LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture eMediaInputAudioPkts %d\n", enable );
					m_AudioPktsRequested = true;
					m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, appModule, true );
					m_Engine.getToGui().getAudioRequests().toGuiWantSpeakerOutput( eAppModulePushToTalk, true );
				}

				updatePushToTalkStatus( netIdent->getMyOnlineId() );
			}
			else
			{
				m_Engine.getToGui().toGuiPushToTalkStatus( netIdent->getMyOnlineId(), ePushToTalStatuskNoConnection );
				LogModule( eLogMediaStream, LOG_VERBOSE, "PushToTalkFeedMgr::enableCapture failed sendPushToTalkReq %s", netIdent->getOnlineName() );
			}
		}
		else
		{
            LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture true GUID already in list %s", netIdent->getOnlineName() );
		}

		m_SessionMgr.findOrCreateTxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, false );
		return true;
	}
	else
	{
		sendPushToTalkStop( netIdent, sktBase );
		removePushToTalkUser( netIdent->getMyOnlineId(), true );
		return true;
	}

    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture %d done %s", enable, netIdent->getOnlineName() );
	return false;
}

//============================================================================
void PushToTalkFeedMgr::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail PluginBase::AutoPluginLock autoLock start" );
	#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock autoLock( &m_Plugin );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail PluginBase::AutoPluginLock autoLock done" );
	#endif // DEBUG_AUTOPLUGIN_LOCK

	std::map<VxGUID, PluginSessionBase*>&	sessionList = m_SessionMgr.getSessions();
	for( auto iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		AudioJitterBuffer& jitterBuf = ((PluginSessionBase*)iter->second)->getJitterBuffer();
		//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail jitterBuf.lockResource sessionIdx %d\n", sessionIdx );
		jitterBuf.lockResource();
		char * audioBuf = jitterBuf.getBufToRead();
		if( audioBuf )
		{
			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail playAudio %d\n", sessionIdx );
			m_PluginMgr.getEngine().getMediaProcessor().playAudio( (int16_t *)audioBuf, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN );
			//VxGUID onlineId = iter->first; // local session id
			VxGUID onlineId = ((PluginSessionBase*)iter->second)->getOnlineId();
			// processor mutex was already locked by call to processor fromGuiAudioOutSpaceAvail which calls callbackAudioOutSpaceAvail
			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail processFriendAudioFeed %d\n", sessionIdx );
			m_PluginMgr.getEngine().getMediaProcessor().processFriendAudioFeed( onlineId, (int16_t *)audioBuf, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN, true );
		}

		//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail jitterBuf.unlockResource sessionIdx %d\n", sessionIdx );
		jitterBuf.unlockResource();
		//sessionIdx++;
	}

	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream,  LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail PluginBase::AutoPluginLock autoLock destroy" );
	#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PushToTalkFeedMgr::onPktPushToTalkReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	bool allowed = netIdent->isMyAccessAllowedFromHim( ePluginTypePushToTalk ) && netIdent->isHisAccessAllowedFromMe( ePluginTypePushToTalk );
	if( !allowed )
	{
		// should this be a hack offence?
		LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr user %s insufficient permaision", netIdent->getOnlineName() );
		return;
	}

	if( m_RxOnlineIdList.addGuidIfDoesntExist( netIdent->getMyOnlineId() ) )
	{
		m_SessionMgr.findOrCreateRxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, false );

		//LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture eMediaInputMixer %d\n", enable );
		if( !m_MixerInputRequesed )
		{
			m_MixerInputRequesed = true;
			m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, eAppModulePushToTalk, true );
			//LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture done\n" );
		}

		updatePushToTalkStatus( netIdent->getMyOnlineId() );
	}
	else
	{
		LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture true GUID already in list %s", netIdent->getOnlineName() );
	}
}

//============================================================================
void PushToTalkFeedMgr::onPktPushToTalkReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void PushToTalkFeedMgr::onPktPushToTalkStart( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//removePushToTalkUser( netIdent->getMyOnlineId() );
	//m_Engine.getToGui().toGuiPushToTalkStop( netIdent->getMyOnlineId() );
}

//============================================================================
void PushToTalkFeedMgr::onPktPushToTalkStop( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	removePushToTalkUser( netIdent->getMyOnlineId() );
	updatePushToTalkStatus( netIdent->getMyOnlineId() );
}

//============================================================================
void PushToTalkFeedMgr::onPktVoiceReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	bool allowed = netIdent->isMyAccessAllowedFromHim( ePluginTypePushToTalk ) && netIdent->isHisAccessAllowedFromMe( ePluginTypePushToTalk );
	if( !allowed )
	{
		// should this be a hack offence?
		LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr user %s insufficient permaision", netIdent->getOnlineName() );
		return;
	}

	if( m_Plugin.isAppPaused() )
	{
		return;
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq PluginBase::AutoPluginLock autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock autoLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq PluginBase::AutoPluginLock autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

	PluginSessionMgr::SessionIter iter;
	std::map<VxGUID, PluginSessionBase*>& sessionList = m_SessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase* poSession = iter->second;
		if( netIdent->getMyOnlineId() == poSession->getOnlineId() )
		{
			AudioJitterBuffer& jitterBuf = poSession->getJitterBuffer();
			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq jitterBuf.lockResource" );
			jitterBuf.lockResource();

			char* audioBuf = poSession->getJitterBuffer().getBufToFill();
			if( audioBuf )
			{
				PktVoiceReq* poPkt = (PktVoiceReq*)pktHdr;
				poSession->getAudioDecoder()->decodeToPcmData( poPkt->getCompressedData(), poPkt->getFrame1Len(), poPkt->getFrame2Len(), (uint16_t*)audioBuf, (int32_t)MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN );
			}

			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq jitterBuf.unlockResource" );
			jitterBuf.unlockResource();
			break;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq PluginBase::AutoPluginLock autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PushToTalkFeedMgr::onPktVoiceReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	onPktPushToTalkReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PushToTalkFeedMgr::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackOpusPkt PluginBase::AutoPluginLock autoLock start" );
	#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock autoLock( &m_Plugin );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackOpusPkt PluginBase::AutoPluginLock autoLock done" );
	#endif // DEBUG_AUTOPLUGIN_LOCK

	PluginSessionMgr::SessionIter iter;
	std::map<VxGUID, PluginSessionBase*>&	sessionList = m_SessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase* poSession = iter->second;
		if( false == poSession->isRxSession() )
		{
			bool result = m_Plugin.txPacket( poSession->getIdent(), poSession->getSkt(), pktOpusAudio );
			if( false == result )
			{
				// TODO handle lost connection
			}
		}
	}

	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackOpusPkt PluginBase::AutoPluginLock autoLock destroy" );
	#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void PushToTalkFeedMgr::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	removePushToTalkUser( netIdent->getMyOnlineId() );
	//m_PushToTalkFeedMgr.onContactWentOffline( netIdent, sktBase );
	//m_PushToTalkFeedMgr.fromGuiStopPluginSession( false, netIdent );
	//m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
}

//============================================================================
void PushToTalkFeedMgr::removePushToTalkUser( VxGUID& onlineId, bool txOnly )
{
	if( m_TxOnlineIdList.removeGuid( onlineId ) )
	{
		m_SessionMgr.removeTxSessionByOnlineId( onlineId, false );

		if( 0 == m_TxOnlineIdList.size() )
		{
			m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, eAppModulePushToTalk, false );
			m_AudioPktsRequested = false;
			m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, eAppModulePushToTalk, false );
			m_MixerInputRequesed = false;
			m_Engine.getToGui().getAudioRequests().toGuiWantSpeakerOutput( eAppModulePushToTalk, false );
			LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::removePushToTalkUser false done" );
		}
	}

	if( !txOnly && m_RxOnlineIdList.removeGuid( onlineId ) )
	{
		m_SessionMgr.removeRxSessionByOnlineId( onlineId, false );

		if( 0 == m_TxOnlineIdList.size() )
		{
			m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, eAppModulePushToTalk, false );
			m_AudioPktsRequested = false;
			m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, eAppModulePushToTalk, false );
			m_MixerInputRequesed = false;
			m_Engine.getToGui().getAudioRequests().toGuiWantSpeakerOutput( eAppModulePushToTalk, false );
			LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::removePushToTalkUser false done" );
		}
	}
}

//============================================================================
bool PushToTalkFeedMgr::sendPushToTalkStop( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	PktPushToTalkStop pktStop;
	return m_Plugin.txPacket( netIdent->getMyOnlineId(), sktBase, &pktStop );
}

//============================================================================
bool PushToTalkFeedMgr::sendPushToTalkReq( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	PktPushToTalkReq pktReq;
	return m_Plugin.txPacket( netIdent->getMyOnlineId(), sktBase, &pktReq );
}

//============================================================================
void PushToTalkFeedMgr::updatePushToTalkStatus( VxGUID& onlineId )
{
	EPushToTalkStatus status{ ePushToTalkStatusInvalid };
	if( !m_Engine.isUserConnected( onlineId ) )
	{
		status = ePushToTalStatuskNoConnection;
	}
	else
	{
		bool isTx = m_TxOnlineIdList.doesGuidExist( onlineId );
		bool isRx = m_RxOnlineIdList.doesGuidExist( onlineId );
		if( isTx && isRx )
		{
			status = ePushToTalkStatusDuplexEnabled;
		}
		else if( isTx )
		{
			status = ePushToTalkStatusTxEnabled;
		}
		else if( isRx )
		{
			status = ePushToTalkStatusRxEnabled;
		}
		else
		{
			status = ePushToTalkStatusNotActive;
		}
	}

	m_Engine.getToGui().toGuiPushToTalkStatus( onlineId, status );
}

