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
#include "PluginServiceWebCam.h"

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
, m_Enabled( false )
, m_CamServerEnabled( false )
, m_AudioPktsRequested( false )
, m_MixerInputRequesed( false )
{
}

//============================================================================
bool PushToTalkFeedMgr::fromGuiPushToTalk( VxNetIdent* netIdent, bool enableTalk )
{
	enableAudioCapture( enableTalk, netIdent, eAppModulePushToTalk );
	return true;
}

//============================================================================
void PushToTalkFeedMgr::fromGuiStartPluginSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	enableAudioCapture( true, netIdent, eAppModulePushToTalk );
}

//============================================================================
void PushToTalkFeedMgr::fromGuiStopPluginSession( bool pluginIsLocked, VxNetIdent * netIdent )
{
	enableAudioCapture( false, netIdent, eAppModulePushToTalk );
}

//============================================================================
void PushToTalkFeedMgr::enableAudioCapture( bool enable, VxNetIdent * netIdent, EAppModule appModule )
{
	if( enable != m_Enabled )
	{
		m_Enabled = enable;
		bool isMyself = ( netIdent->getMyOnlineId() == m_PluginMgr.getEngine().getMyOnlineId() ); 
		if( enable )
		{
			if( m_GuidList.addGuidIfDoesntExist( netIdent->getMyOnlineId() ) )
			{
				if( ePluginTypeCamServer == m_Plugin.getPluginType() )
				{
					if( isMyself )
					{
						// web cam server.. need audio packets to send out but not mixer input
						m_CamServerEnabled = true;
						if( !m_AudioPktsRequested )
						{
							m_AudioPktsRequested = true;
							m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, appModule, true );
						}
					}
					else
					{
						if( !m_MixerInputRequesed )
						{
							m_MixerInputRequesed = true;
							m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, appModule, true );
						}
					}
				}
				else
				{
					if( !m_AudioPktsRequested )
					{
						//LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture eMediaInputAudioPkts %d\n", enable );
						m_AudioPktsRequested = true;
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, appModule, true );
					}

					//LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture eMediaInputMixer %d\n", enable );
					if( !m_MixerInputRequesed )
					{
						m_MixerInputRequesed = true;
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, appModule, true );
						//LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture done\n" );
					}
				}
			}
			else
			{
                LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture true GUID already in list %s", netIdent->getOnlineName() );
			}
		}
		else
		{
			if( m_GuidList.removeGuid( netIdent->getMyOnlineId() ) )
			{
				if( ePluginTypeCamServer == m_Plugin.getPluginType() )
				{
					if( isMyself )
					{
						m_CamServerEnabled = false;
						// web cam server.. need audio packets to send out but not mixer input
						if( m_AudioPktsRequested && ( 0 == m_GuidList.size() ) )
						{
							m_AudioPktsRequested = false;
							m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, eAppModulePushToTalk, false );
						}
					}
					else
					{
						if( m_MixerInputRequesed )
						{
							if(  ( 0 == m_GuidList.size() ) 
								|| ( m_CamServerEnabled && ( 1 == m_GuidList.size() ) ) )
							{
								m_MixerInputRequesed = false;
								m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, eAppModulePushToTalk, false );
							}
						}
					}
				}
				else
				{
					if(  0 == m_GuidList.size() ) 
					{
                        LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture false eMediaInputAudioPkts %d", enable );
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputAudioPkts, eAppModulePushToTalk, false );
						m_AudioPktsRequested = false;
                        LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture false eMediaInputMixer %d", enable );
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputMixer, eAppModulePushToTalk, false );
						m_MixerInputRequesed = false;
                        LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture false done" );
					}
					else
					{
                        LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture false GUID list not empty %s", netIdent->getOnlineName() );
					}
				}
			}
			else
			{
                LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture false GUID not found %s", netIdent->getOnlineName() );
			}
		}
	}

    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::enableCapture %d done %s", enable, netIdent->getOnlineName() );
}

//============================================================================
void PushToTalkFeedMgr::onPktPushToTalkReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	if( false == m_Enabled )
	{
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
    LogModule( eLogMediaStream,  LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq PluginBase::AutoPluginLock autoLock done" );
	#endif // DEBUG_AUTOPLUGIN_LOCK

	PluginSessionMgr::SessionIter iter;
	std::map<VxGUID, PluginSessionBase *>&	sessionList = m_SessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase * poSession = iter->second;
		if( netIdent->getMyOnlineId() == poSession->getOnlineId() )
		{
			AudioJitterBuffer& jitterBuf = poSession->getJitterBuffer();
			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::onPktPushToTalkReq jitterBuf.lockResource" );
			jitterBuf.lockResource();

			char * audioBuf = poSession->getJitterBuffer().getBufToFill();
			if( audioBuf )
			{
				PktPushToTalkReq * poPkt = (PktPushToTalkReq * )pktHdr;
				poSession->getAudioDecoder()->decodeToPcmData( poPkt->getCompressedData(), poPkt->getFrame1Len(), poPkt->getFrame2Len(), (uint16_t *)audioBuf, (int32_t)MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN );
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
void PushToTalkFeedMgr::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail PluginBase::AutoPluginLock autoLock start" );
	#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock autoLock( &m_Plugin );
	#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail PluginBase::AutoPluginLock autoLock done" );
	#endif // DEBUG_AUTOPLUGIN_LOCK

    //int sessionIdx = 0;
	PluginSessionMgr::SessionIter iter;
	std::map<VxGUID, PluginSessionBase *>&	sessionList = m_SessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		AudioJitterBuffer& jitterBuf = ((PluginSessionBase *)iter->second)->getJitterBuffer();
		//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail jitterBuf.lockResource sessionIdx %d\n", sessionIdx );
		jitterBuf.lockResource();
		char * audioBuf = jitterBuf.getBufToRead();
		if( audioBuf )
		{
			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail playAudio %d\n", sessionIdx );
			m_PluginMgr.getEngine().getMediaProcesser().playAudio( (int16_t *)audioBuf, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN );
			//VxGUID onlineId = iter->first; // local session id
			VxGUID onlineId = ((PluginSessionBase *)iter->second)->getOnlineId();
			// processor mutex was already locked by call to processor fromGuiAudioOutSpaceAvail which calls callbackAudioOutSpaceAvail
			//LogMsg( LOG_INFO, "PushToTalkFeedMgr::callbackAudioOutSpaceAvail processFriendAudioFeed %d\n", sessionIdx );
			m_PluginMgr.getEngine().getMediaProcesser().processFriendAudioFeed( onlineId, (int16_t *)audioBuf, MY_OPUS_PKT_UNCOMPRESSED_DATA_LEN, true );
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
void PushToTalkFeedMgr::onPktPushToTalkReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
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
	std::map<VxGUID, PluginSessionBase *>&	sessionList = m_SessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase * poSession = iter->second;
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
