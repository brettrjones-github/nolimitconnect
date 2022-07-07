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

#include "VideoFeedMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"
#include "PluginSessionMgr.h"
#include "P2PSession.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>

#include <PktLib/PktsVideoFeed.h>
#include <NetLib/VxSktBase.h>

#include <memory.h>

//#define DEBUG_AUTOPLUGIN_LOCK 1

namespace
{
	// max number of video frames in the air (unacknowledged) before start dropping frames
	// android captures at about 80ms per frame and satellite link can take up to 3 seconds so 3 / .080 = 37.5
	const int					MAX_OUTSTANDING_VID_ACKS		= 30;
}

//============================================================================
VideoFeedMgr::VideoFeedMgr( P2PEngine& engine, PluginBase& plugin, PluginSessionMgr& sessionMgr )
: m_Engine( engine )
, m_Plugin( plugin )
, m_PluginMgr( engine.getPluginMgr() )
, m_SessionMgr( sessionMgr )
, m_CamServerEnabled( false )
, m_VideoPktsRequested( false )
, m_VideoJpgRequesed( false )
{
}

//============================================================================
void VideoFeedMgr::fromGuiStartPluginSession( bool pluginIsLocked, EAppModule appModule, VxNetIdent * netIdent )
{
	enableVideoCapture( true, netIdent, appModule );
}

//============================================================================
void VideoFeedMgr::fromGuiStopPluginSession( bool pluginIsLocked, EAppModule appModule, VxNetIdent * netIdent )
{
	enableVideoCapture( false, netIdent, appModule );
	//LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::fromGuiStopPluginSession\n" );

	PktVideoFeedStatus oPkt;
	oPkt.m_u8Status = 1;

	PluginSessionMgr::SessionIter iter;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{ 
#ifdef DEBUG_AUTOPLUGIN_LOCK
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::fromGuiStopPluginSession PluginBase::AutoPluginLock autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock(); 
#ifdef DEBUG_AUTOPLUGIN_LOCK
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::fromGuiStopPluginSession PluginBase::AutoPluginLock autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	std::map<VxGUID, PluginSessionBase *>&	sessionList = m_SessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase * sessionBase = iter->second;
		if( sessionBase->isP2PSession() )
		{
			P2PSession * poSession = (P2PSession *)sessionBase;
			if( poSession->getSkt() )
			{
				m_PluginMgr.pluginApiTxPacket( m_Plugin.getPluginType(), poSession->getIdent()->getMyOnlineId(), poSession->getSkt(), &oPkt );
			}
		}
	}

	if( false == pluginIsLocked )
	{ 
		#ifdef DEBUG_AUTOPLUGIN_LOCK
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::fromGuiStopPluginSession sessions done\n" );
		#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock(); 
	}
}

//============================================================================
void VideoFeedMgr::enableVideoCapture( bool enable, VxNetIdent * netIdent, EAppModule appModule )
{
	//LogMsg( LOG_INFO, "VideoFeedMgr::enableCapture %d start %s\n", enable, netIdent->getOnlineName() );
	// kind of a strange way of handling the problem of which video to enable
	// if there are any myIdents in requests for eMediaInputVideoPkts then vid capture will be enabled but others
	// in that list allow processing of incoming packets without enabling video capture for the case of cam server client which does not require video capture
	VxNetIdent * myIdent = &m_PluginMgr.getEngine().getMyPktAnnounce();
	bool isMyself = ( netIdent->getMyOnlineId() == myIdent->getMyOnlineId() ); 
	if( enable )
	{
		if( m_GuidList.addGuidIfDoesntExist( netIdent->getMyOnlineId() ) )
		{
			if( ePluginTypeCamServer == m_Plugin.getPluginType() )
			{
				if( isMyself )
				{
					m_CamServerEnabled = true;
					// web cam server.. need to send out video pkts
					if( !m_VideoPktsRequested )
					{
						m_VideoPktsRequested = true;
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoPkts, appModule, true, myIdent );
					}
				}
				else
				{
					// receive video pkts but don't enable video capture
					if( !m_VideoPktsRequested )
					{
						m_VideoPktsRequested = true;
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoPkts, appModule, true, (void *)m_Plugin.getPluginType() );
					}

					// dont need the jpgs because dont need to see ourself
					//if( !m_VideoJpgRequesed )
					//{
					//	m_VideoJpgRequesed = true;
					//	m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoJpgSmall, true );
					//}

				}
			}
			else
			{
				if( !m_VideoJpgRequesed )
				{
					m_VideoJpgRequesed = true;
					m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoJpgSmall, appModule, true, (void *)m_Plugin.getPluginType() );
				}

				if( !m_VideoPktsRequested )
				{
					m_VideoPktsRequested = true;
					m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoPkts, appModule, true, (void *)myIdent );
				}
			}
		}
		else
		{
            LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::enableCapture true GUID already in list %s", netIdent->getOnlineName() );
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
					// web cam server..
					if( m_VideoPktsRequested && ( 0 == m_GuidList.size() ) )
					{
						m_VideoPktsRequested = false;
						m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoPkts, appModule, false, netIdent );
					}
				}
				else
				{
					if( m_VideoPktsRequested )
					{
						if(  ( 0 == m_GuidList.size() ) 
							|| ( m_CamServerEnabled && ( 1 == m_GuidList.size() ) ) )
						{
							m_VideoPktsRequested = false;
							m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoPkts, appModule, false, (void *)m_Plugin.getPluginType() );
						}
					}

					if( m_VideoJpgRequesed )
					{
						if(  ( 0 == m_GuidList.size() ) 
							|| ( m_CamServerEnabled && ( 1 == m_GuidList.size() ) ) )
						{
							m_VideoJpgRequesed = false;
							m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoJpgSmall, appModule, false, (void *)m_Plugin.getPluginType() );
						}
					}
				}
			}
			else
			{
				if(  0 == m_GuidList.size() ) 
				{
                    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::enableCapture eMediaInputVideoJpgSmall false %s", netIdent->getOnlineName() );
					m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoJpgSmall, appModule, false, (void *)m_Plugin.getPluginType() );
					m_VideoJpgRequesed = false;
					//LogMsg( LOG_INFO, "VideoFeedMgr::enableCapture eMediaInputVideoPkts false %s\n", netIdent->getOnlineName() );
					m_PluginMgr.pluginApiWantMediaInput( m_Plugin.getPluginType(), eMediaInputVideoPkts, appModule, false, (void *)myIdent );
					m_VideoPktsRequested = false;
					//LogMsg( LOG_INFO, "VideoFeedMgr::enableCapture eMediaInputVideoPkts false done %s\n", netIdent->getOnlineName() );
				}
				//else
				//{
				//	LogMsg( LOG_INFO, "VideoFeedMgr::enableCapture false GUID list not empty %s\n", netIdent->getOnlineName() );
				//}
			}
		}
		else
		{
            LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::enableCapture false GUID not found %s", netIdent->getOnlineName() );
		}
	}

    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::enableCapture %d done %s", enable, netIdent->getOnlineName() );
}

//============================================================================
void VideoFeedMgr::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void VideoFeedMgr::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void VideoFeedMgr::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktVideoFeedPicAck oPkt;
	m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
									netIdent->getMyOnlineId(), 
									sktBase, 
									&oPkt ); 
	
	PktVideoFeedPic * poPktCastPic = ( PktVideoFeedPic * )pktHdr;
	uint32_t picTotalLen = poPktCastPic->getTotalDataLen();
	uint16_t picWidth	= poPktCastPic->getWidth();
	uint16_t picHeight	= poPktCastPic->getHeight();

	if( ( 32 > picTotalLen  ) 
		|| ( picTotalLen > 4000000 )
		|| ( poPktCastPic->getThisDataLen() > picTotalLen ) )
	{
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPic invalid pic length %d", picTotalLen );
		return;
	}

	if( ( 32 > picWidth  ) 
		|| ( 1280 < picWidth )
		|| ( 32 > picHeight  ) 
		|| ( 1280 < picHeight ) )
	{
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPic invalid pic width %d height %d", picWidth, picHeight );
		return;
	}


	if( picTotalLen == poPktCastPic->getThisDataLen() )
	{
		m_Plugin.getEngine().getMediaProcesser().processFriendVideoFeed(	netIdent->getMyOnlineId(),
																			poPktCastPic->getDataPayload(), 
																			poPktCastPic->getTotalDataLen(),
																			poPktCastPic->getMotionDetect() );

		/*
		if( false == m_Plugin.isAppPaused() )
		{
			m_PluginMgr.pluginApiPlayVideoFrame(	m_Plugin.getPluginType(), 
													poPktCastPic->getDataPayload(), 
													poPktCastPic->getTotalDataLen(), 
													netIdent,
													poPktCastPic->getMotionDetect() );
		}*/
	}
	else
	{
		// picture was too big for one packet
#ifdef DEBUG_AUTOPLUGIN_LOCK
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPic autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPic autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

		P2PSession * poSession = m_SessionMgr.findOrCreateP2PSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
		if( poSession )
		{
			if( poSession->getVideoCastPkt() )
			{
				delete poSession->getVideoCastPkt();
				poSession->setVideoCastPkt( NULL );
			}

			PktVideoFeedPic * poPic = ( PktVideoFeedPic * ) new char[ sizeof( PktVideoFeedPic ) + 16 + picTotalLen ];
			memcpy( poPic, poPktCastPic, poPktCastPic->getPktLength() );
			poSession->setVideoCastPkt( poPic );

			// wait for rest of picture to arrive
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
        LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPic autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================
void VideoFeedMgr::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktVideoFeedPicAck oPkt;
	m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
									netIdent->getMyOnlineId(), 
									sktBase, 
									&oPkt ); 

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream,  LOG_INFO, "VideoFeedMgr::onPktVideoFeedPicChunk autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPicChunk autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

	PktVideoFeedPicChunk * poPktPicChunk = ( PktVideoFeedPicChunk * )pktHdr;
	P2PSession * poSession = m_SessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		PktVideoFeedPic * poPktCastPic = (PktVideoFeedPic *)poSession->getVideoCastPkt();
		if( poPktCastPic && 
			(poPktCastPic->getTotalDataLen() >=  (poPktCastPic->getThisDataLen() + poPktPicChunk->getThisDataLen() ) ) )
		{
			memcpy( &poPktCastPic->getDataPayload()[ poPktCastPic->getThisDataLen() ], 
				poPktPicChunk->getDataPayload(), 
				poPktPicChunk->getThisDataLen() );
			poPktCastPic->setThisDataLen( poPktCastPic->getThisDataLen() + poPktPicChunk->getThisDataLen() );
			if( poPktCastPic->getThisDataLen() >= poPktCastPic->getTotalDataLen() )
			{
				// all of picture arrived
				m_Plugin.getEngine().getMediaProcesser().processFriendVideoFeed(	netIdent->getMyOnlineId(),
																					poPktCastPic->getDataPayload(), 
																					poPktCastPic->getTotalDataLen(),
																					poPktCastPic->getMotionDetect() );
								

/*				if( false == m_Plugin.isAppPaused() )
				{
					m_PluginMgr.pluginApiPlayVideoFrame(	m_Plugin.getPluginType(), 
															poPktCastPic->getDataPayload(), 
															poPktCastPic->getTotalDataLen(), 
															netIdent,
															poPktCastPic->getMotionDetect() );
				}*/

				delete poSession->getVideoCastPkt();
				poSession->setVideoCastPkt( NULL );
			}
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPicChunk autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void VideoFeedMgr::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPicAck autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPicAck autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

	P2PSession * poSession = m_SessionMgr.findP2PSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		poSession->decrementOutstandingAckCnt();
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::onPktVideoFeedPicAck autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void VideoFeedMgr::callbackVideoPktPic( VxGUID& feedId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum )
{
	PluginSessionMgr::SessionIter iter;
	std::map<VxGUID, PluginSessionBase *>&sessionList = m_SessionMgr.getSessions();

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::callbackVideoPktPic autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::callbackVideoPktPic autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase * sessionBase = iter->second;
		if( sessionBase->isP2PSession() )
		{
			P2PSession * poSession = (P2PSession *)iter->second;
			int ackCnt = poSession->getOutstandingAckCnt();
			if( poSession && ( MAX_OUTSTANDING_VID_ACKS > ackCnt ) )
			{
				if( m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
													poSession->getIdent()->getMyOnlineId(), 
													poSession->getSkt(), 
													pktVid ) )
				{
					poSession->incrementOutstandingAckCnt();
				}
			}
			else
			{
				poSession->setIsSendingPkts( false );
			}
		}
		else if( sessionBase->isTxSession() )
		{
			TxSession * poSession = (TxSession *)iter->second;
			int ackCnt = poSession->getOutstandingAckCnt();
			if( poSession && ( MAX_OUTSTANDING_VID_ACKS > ackCnt ) )
			{
				if( m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
													poSession->getIdent()->getMyOnlineId(), 
													poSession->getSkt(), 
													pktVid ) )
				{
					poSession->incrementOutstandingAckCnt();
				}
			}
			else
			{
				poSession->setIsSendingPkts( false );
			}
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::callbackVideoPktPic autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
void VideoFeedMgr::callbackVideoPktPicChunk( VxGUID& feedId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum )
{
	PluginSessionMgr::SessionIter iter;
	std::map<VxGUID, PluginSessionBase *>&sessionList = m_SessionMgr.getSessions();

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::callbackVideoPktPicChunk autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::callbackVideoPktPicChunk autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		PluginSessionBase * sessionBase = iter->second;
		if( sessionBase->isP2PSession() )
		{
			P2PSession * poSession = (P2PSession *)iter->second;
			if( m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
					                            poSession->getIdent()->getMyOnlineId(), 
					                            poSession->getSkt(), 
					                            pktVid ) )
			{
				poSession->setOutstandingAckCnt( poSession->getOutstandingAckCnt() + 1 );
			}
		}
		else if( sessionBase->isTxSession() )
		{
			TxSession * poSession = (TxSession *)iter->second;
			if( m_PluginMgr.pluginApiTxPacket(	m_Plugin.getPluginType(), 
											    poSession->getIdent()->getMyOnlineId(), 
											    poSession->getSkt(), 
											    pktVid ) )
			{
				poSession->setOutstandingAckCnt( poSession->getOutstandingAckCnt() + 1 );
			}
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
    LogModule( eLogMediaStream, LOG_INFO, "VideoFeedMgr::callbackVideoPktPicChunk autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}
