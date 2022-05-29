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

#include "PluginServiceWebCam.h"
#include "PluginMgr.h"
#include "TxSession.h"
#include "RxSession.h"

#include <libjpg/VxJpgLib.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/MediaProcessor/MediaProcessor.h>
#include <GuiInterface/IToGui.h>

#include <NetLib/VxSktBase.h>

#include <PktLib/PktsVideoFeed.h>
#include <PktLib/PktsSession.h>
#include <PktLib/PktsPluginOffer.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
PluginServiceWebCam::PluginServiceWebCam( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
, m_PluginSessionMgr( engine, *this, pluginMgr )
, m_VoiceFeedMgr( engine, *this, m_PluginSessionMgr )
, m_VideoFeedMgr( engine, *this, m_PluginSessionMgr )
{
	m_ePluginType = ePluginTypeCamServer;
}

//============================================================================
PluginServiceWebCam::~PluginServiceWebCam()
{
}

//============================================================================
void PluginServiceWebCam::setIsPluginInSession( bool isInSession )
{
	setIsServerInSession( isInSession );
	IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, isInSession ? 0 : -1 );
}

//============================================================================
// override this by plugin to create inherited RxSession
RxSession * PluginServiceWebCam::createRxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	return new RxSession( sktBase, netIdent, getPluginType() );
}

//============================================================================
// override this by plugin to create inherited TxSession
TxSession * PluginServiceWebCam::createTxSession( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	return new TxSession( sktBase, netIdent, getPluginType() );
}

//============================================================================
void PluginServiceWebCam::callbackVideoJpgSmall( void * /*userData*/, VxGUID& feedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
	//LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiVideoData sessions %d\n", m_PluginSessionMgr.m_aoSessions.size() );
	m_PluginMgr.pluginApiPlayVideoFrame( m_ePluginType, jpgData, jpgDataLen, m_MyIdent, motion0to100000 );
}

//============================================================================
void PluginServiceWebCam::sendVidPkt( VxPktHdr * vidPkt, bool requiresAck )
{
	if( m_PluginSessionMgr.getSessionCount() )
	{
		//LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiVideoData %d clients\n", m_PluginSessionMgr.m_TxSessions.size() );

		PluginSessionMgr::SessionIter iter;
		std::map<VxGUID, PluginSessionBase *>&	sessionList = m_PluginSessionMgr.getSessions();
		PluginBase::AutoPluginLock pluginMutexLock( this );
		for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
		{
			PluginSessionBase * sessionBase = iter->second;
			if( sessionBase->isTxSession() )
			{
				TxSession * poSession = (TxSession *)sessionBase;
				if( poSession 
					&& ( !requiresAck  || (10 > poSession->getOutstandingAckCnt() ) ) )
				{
					m_PluginMgr.pluginApiTxPacket(	m_ePluginType, 
													poSession->getIdent()->getMyOnlineId(), 
													poSession->getSkt(), 
													vidPkt ); 
					if( requiresAck )
					{
						poSession->setOutstandingAckCnt( poSession->getOutstandingAckCnt() + 1 );
					}
				}
			}
		}
	}
	//LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiVideoData done\n" );
}

//============================================================================
void PluginServiceWebCam::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	m_VoiceFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginServiceWebCam::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_VoiceFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginServiceWebCam::callbackVideoPktPic( void * /*userData*/, VxGUID& feedId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum )
{
	sendVidPkt( pktVid, true );
}

//============================================================================
void PluginServiceWebCam::callbackVideoPktPicChunk( void * /*userData*/, VxGUID& feedId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum )
{
	sendVidPkt( pktVid, false );
}

//============================================================================
//! called to start service or session with remote friend
void PluginServiceWebCam::fromGuiStartPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	if( netIdent->getMyOnlineId() == m_MyIdent->getMyOnlineId() )
	{
		LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiStartPluginSession is ME\n" );
		if( false == fromGuiIsPluginInSession() )
		{
			m_Engine.setHasSharedWebCam(true);
			setIsPluginInSession(true);
			// request video capture
			m_VideoFeedMgr.fromGuiStartPluginSession( false, netIdent );
			m_VoiceFeedMgr.fromGuiStartPluginSession( false, netIdent );
		}
	}
	else
	{
		LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiStartPluginSession is NOT ME\n" );
		AutoPluginLock pluginMutexLock( this );
		RxSession * rxSession = m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
		if( 0 == rxSession )
		{
			VxSktBase * sktBase = 0;
			m_PluginMgr.pluginApiSktConnectTo( ePluginTypeCamServer, netIdent, 0, &sktBase );
			if( 0 != sktBase )
			{
				rxSession = m_PluginSessionMgr.findOrCreateRxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true, lclSessionId );
			}
		}

		if( 0 != rxSession )
		{
			if( lclSessionId.isVxGUIDValid() )
			{
				rxSession->setLclSessionId( lclSessionId );
			}
			else
			{
				lclSessionId = rxSession->getLclSessionId();
			}

			requestCamSession(	rxSession,
								false );

			m_VideoFeedMgr.fromGuiStartPluginSession( true, netIdent );
			m_VoiceFeedMgr.fromGuiStartPluginSession( true, netIdent );
		}
		else
		{
			LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiStartPluginSession could not connect to %s\n", netIdent->getOnlineName() );
		}
	}
}

//============================================================================
//! called to stop service or session with remote friend
void PluginServiceWebCam::fromGuiStopPluginSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	LogMsg( LOG_INFO, "PluginServiceWebCam::fromGuiStopPluginSession\n" );
	PluginBase::AutoPluginLock pluginMutexLock( this );
	bool isServerSession = ( netIdent->getMyOnlineId() == m_MyIdent->getMyOnlineId() );
	if( isServerSession )
	{
		m_Engine.setHasSharedWebCam(false);
		setIsPluginInSession( false );
		m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
		// don't want video capture anymore
		m_VideoFeedMgr.fromGuiStopPluginSession( true, netIdent );
		if( true == fromGuiIsPluginInSession() )
		{
			setIsPluginInSession(false);

			PktVideoFeedStatus oPkt;
			oPkt.setFeedStatus( eFeedStatusOffline );

			PluginSessionMgr::SessionIter iter;
			std::map<VxGUID, PluginSessionBase *>&	sessionList = m_PluginSessionMgr.getSessions();
			bool bErased = true;
			while( bErased ) 
			{
				bErased = false;				
				iter = sessionList.begin();
				while( iter != sessionList.end() )
				{
					PluginSessionBase * sessionBase = iter->second;
					if( sessionBase->isTxSession() )
					{
						TxSession * poSession = (TxSession *)sessionBase;
						if( poSession->getSkt() )
						{
							oPkt.setLclSessionId( poSession->getLclSessionId() );
							oPkt.setRmtSessionId( poSession->getRmtSessionId() );
							m_PluginMgr.pluginApiTxPacket( m_ePluginType, poSession->getIdent()->getMyOnlineId(), poSession->getSkt(), &oPkt );
							delete poSession;
							sessionList.erase( iter );
							bErased = true;
							break;
						}
						else
						{
							++iter;
						}
					}
				}
			}
		}
	}
	else
	{
		m_VoiceFeedMgr.fromGuiStopPluginSession( true, netIdent );
		PktSessionStopReq oPkt;

		RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
		if( poSession )
		{
			oPkt.setLclSessionId( poSession->getLclSessionId() );
			oPkt.setRmtSessionId( poSession->getRmtSessionId() );
			m_PluginMgr.pluginApiTxPacket( m_ePluginType, poSession->getIdent()->getMyOnlineId(), poSession->getSkt(), &oPkt );
		}

		m_PluginSessionMgr.removeRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	}
}

//============================================================================
bool PluginServiceWebCam::fromGuiIsPluginInSession( VxNetIdent * netIdent, int pvUserData, VxGUID lclSessionId )
{
	// for cam server we really want to know if server is running
	return getIsServerInSession();
}

//============================================================================
EPluginAccess PluginServiceWebCam::canAcceptNewSession( VxNetIdent * netIdent )
{
	EFriendState eHisPermissionToMe = netIdent->getHisFriendshipToMe();
	EFriendState eMyPermissionToHim = netIdent->getMyFriendshipToHim();

	if( (eFriendStateIgnore == eHisPermissionToMe) ||
		(eFriendStateIgnore == eMyPermissionToHim) )
	{
		return ePluginAccessIgnored;
	}
	EFriendState ePermissionLevel = this->m_MyIdent->getPluginPermission(m_ePluginType);
	if( eFriendStateIgnore == ePermissionLevel )
	{
		return ePluginAccessDisabled;
	}
	if( ePermissionLevel > eMyPermissionToHim )
	{
		return ePluginAccessLocked;
	}

	return ePluginAccessOk;
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool PluginServiceWebCam::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		// identity of friend
												int				pvUserData,
												const char *	pOfferMsg,		// offer message
												const char *	pFileName,
												uint8_t *			fileHashId,
												VxGUID			lclSessionId )		// filename if any
{
	VxSktBase * sktBase = NULL;
	LogMsg( LOG_INFO, " PluginServiceWebCam::fromGuiMakePluginOffer %s\n", netIdent->getOnlineName());
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( true == m_PluginMgr.pluginApiSktConnectTo( m_ePluginType, netIdent, pvUserData, &sktBase ) )
	{
		PktPluginOfferReq oPkt;
		oPkt.setLclSessionId( lclSessionId );
        // force session to be created so have session to lookup on reply
		RxSession * rxSession = (RxSession *)m_PluginSessionMgr.findOrCreateRxSessionWithSessionId( lclSessionId, sktBase, netIdent, true );
        if( 0 != rxSession )
        {
            if( true == m_PluginMgr.pluginApiTxPacket(	m_ePluginType,
                                                        netIdent->getMyOnlineId(),
                                                        sktBase,
                                                        &oPkt ) )
            {
                LogMsg( LOG_INFO, " PluginServiceWebCam::fromGuiMakePluginOffer success\n");
                return true;
            }
            else
            {
                LogMsg( LOG_INFO, " PluginServiceWebCam::fromGuiMakePluginOffer failed to send pkt\n");
            }
        }
        else
        {
            LogMsg( LOG_ERROR, " PluginServiceWebCam::fromGuiMakePluginOffer failed to create session\n");
        }
    }

	return false;
}

//============================================================================
bool PluginServiceWebCam::requestCamSession(	RxSession *			rxSession,
											bool				bWaitForSuccess )
{
	PktSessionStartReq oPkt;
	oPkt.setLclSessionId( rxSession->getLclSessionId() );
	bool bSuccess = m_PluginMgr.pluginApiTxPacket(	m_ePluginType, 
													rxSession->getIdent()->getMyOnlineId(), 
													rxSession->getSkt(), 
													&oPkt );
	if( ( true == bSuccess ) && bWaitForSuccess )
	{
		bSuccess = false;
		bool bResponseReceived = rxSession->waitForResponse( 9000 );
		if( bResponseReceived )
		{
			if( rxSession->getIsSessionStarted() )
			{
				bSuccess = true;
			}
		}
	}
	
	return bSuccess;
}
//============================================================================
bool PluginServiceWebCam::stopCamSession(	VxNetIdent *		netIdent,	
										VxSktBase *			sktBase )
{
	LogMsg( LOG_ERROR, "PluginServiceWebCam::stopCamSession\n");
	PktSessionStopReq oPkt;
	bool bSuccess = m_PluginMgr.pluginApiTxPacket(	m_ePluginType, 
												netIdent->getMyOnlineId(), 
												sktBase, 
												&oPkt );
	m_PluginSessionMgr.removeRxSessionByOnlineId( netIdent->getMyOnlineId(), false );

	return bSuccess;
}


//============================================================================
//! packet with remote users offer
void PluginServiceWebCam::onPktPluginOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginServiceWebCam::onPktPluginOfferReq\n" );
	PktPluginOfferReq * pktOfferReq = ( PktPluginOfferReq * )pktHdr;
	PktPluginOfferReply pktReply;
	pktReply.setRmtSessionId( pktOfferReq->getLclSessionId() );
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( getIsServerInSession() && ( ePluginAccessOk == canAcceptNewSession( netIdent ) ) ) 
	{
		TxSession * txSession = (TxSession *)m_PluginSessionMgr.findOrCreateTxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
		pktReply.setLclSessionId( txSession->getLclSessionId() );
		pktReply.setOfferResponse(eOfferResponseAccept);
	}
	else
	{
		LogMsg( LOG_INFO, "PluginServiceWebCam::onPktPluginOfferReq REJECTED in session %d canAcceptNewSession %d\n",
			getIsServerInSession(), canAcceptNewSession( netIdent ));
		pktReply.setOfferResponse(eOfferResponseReject);
	}

	m_PluginMgr.pluginApiTxPacket(	m_ePluginType, 
									netIdent->getMyOnlineId(), 
									sktBase, 
									&pktReply ); 
}

//============================================================================
//! packet with remote users reply to offer
void PluginServiceWebCam::onPktPluginOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktPluginOfferReply * poPkt = (PktPluginOfferReply *)pktHdr;
	EOfferResponse eResponse = poPkt->getOfferResponse();
	LogMsg( LOG_INFO, "PluginServiceWebCam::onPktPluginOfferReply %d\n", eResponse );
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( eResponse == eOfferResponseAccept )
	{
		RxSession * rxSession = (RxSession *)m_PluginSessionMgr.findOrCreateRxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
		rxSession->setOfferResponse( eResponse );
	}

	IToGui::getToGui().toGuiRxedOfferReply(	netIdent,			
									m_ePluginType,		
									0,				// plugin defined data
									eResponse,
									0,
									0,
									poPkt->getRmtSessionId(),
									poPkt->getLclSessionId() );
}

//============================================================================
void PluginServiceWebCam::onPktSessionStartReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_INFO, "PluginServiceWebCam::onPktSessionStartReq\n" );
	PktSessionStartReply oPkt;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( getIsServerInSession() && ( ePluginAccessOk == canAcceptNewSession( netIdent ) ) ) 
	{
		oPkt.setOfferResponse(eOfferResponseAccept);
        TxSession * txSession = (TxSession *)m_PluginSessionMgr.findOrCreateTxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
        if( 0 == txSession )
        {
            LogMsg( LOG_ERROR, "PluginServiceWebCam::onPktSessionStartReq failed to create or find session\n" );
            oPkt.setOfferResponse( eOfferResponseReject );
        }
		else
		{
			IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
		}
	}
	else
	{
		oPkt.setOfferResponse( eOfferResponseReject );
	}

	m_PluginMgr.pluginApiTxPacket(	m_ePluginType, 
									netIdent->getMyOnlineId(), 
									sktBase, 
									&oPkt ); 
}

//============================================================================
void PluginServiceWebCam::onPktSessionStartReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktSessionStartReply * poPkt = (PktSessionStartReply *)pktHdr;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		LogMsg( LOG_INFO, "PluginServiceWebCam::onPktSessionStartReply %d\n", poPkt->getOfferResponse() );
		if( eOfferResponseAccept == poPkt->getOfferResponse() )
		{
			poSession->setIsSessionStarted( true );
		}
		else
		{
			poSession->setIsSessionStarted( false );
		}

		poSession->signalResponseRecieved();
	}
}

//============================================================================
void PluginServiceWebCam::onPktSessionStopReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_PluginSessionMgr.removeTxSessionByOnlineId( netIdent->getMyOnlineId(), false );
	if( getIsServerInSession() )
	{
		IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
	}
}

//============================================================================
void PluginServiceWebCam::onPktSessionStopReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	LogMsg( LOG_ERROR, "PluginServiceWebCam::onPktSessionStopReply" );
}

//============================================================================
void PluginServiceWebCam::onPktVideoFeedReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
}

//============================================================================
void PluginServiceWebCam::onPktVideoFeedStatus( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktVideoFeedStatus * pktVideoStatus = ( PktVideoFeedStatus * )pktHdr;
	PluginBase::AutoPluginLock pluginMutexLock( this );
	RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		LogMsg( LOG_INFO, "PluginServiceWebCam::onPktVideoFeedStatus %d\n", pktVideoStatus->getFeedStatus() );
		if( eFeedStatusOnline != pktVideoStatus->getFeedStatus() )
		{
			IToGui::getToGui().toGuiRxedOfferReply(	netIdent,			
											m_ePluginType,		
											0,				// plugin defined data
											( eFeedStatusBusy == pktVideoStatus->getFeedStatus() ) ? eOfferResponseBusy : eOfferResponseEndSession,
											0,
											0,
											pktVideoStatus->getRmtSessionId(),
											pktVideoStatus->getLclSessionId() );

			m_PluginSessionMgr.endPluginSession( netIdent->getMyOnlineId(), true );
			m_PluginSessionMgr.removeTxSessionByOnlineId( netIdent->getMyOnlineId(), true );
			if( getIsServerInSession() )
			{
				IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
			}
		}
	}
}

//============================================================================
void PluginServiceWebCam::onPktVideoFeedPic( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	//LogMsg( LOG_INFO, "PluginServiceWebCam::onPktCastPic\n" );
	PktVideoFeedPicAck oPkt;
	m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), sktBase, &oPkt ); 

	PktVideoFeedPic * poPktCastPic = ( PktVideoFeedPic * )pktHdr;
	if( poPktCastPic->getTotalDataLen() == poPktCastPic->getThisDataLen() )
	{
		PluginBase::AutoPluginLock pluginMutexLock( this );
		m_Engine.getMediaProcesser().processFriendVideoFeed( netIdent->getMyOnlineId(), poPktCastPic->getDataPayload(), poPktCastPic->getTotalDataLen(), poPktCastPic->getMotionDetect() );
		RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
		if( poSession )
		{
			if( poSession->getVideoFeedPkt() )
			{
				delete poSession->getVideoFeedPkt();
				poSession->setVideoFeedPkt( NULL );
			}
		}
	}
	else
	{
		// picture was too big for one packet
		PluginBase::AutoPluginLock pluginMutexLock( this );

		RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
		if( poSession )
		{
			if( poSession->getVideoFeedPkt() )
			{
				delete poSession->getVideoFeedPkt();
				poSession->setVideoFeedPkt( NULL );
			}
		}
		else
		{
			poSession = (RxSession *)m_PluginSessionMgr.findOrCreateRxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
			LogMsg( LOG_INFO, "PluginServiceWebCam::onPktVideoFeedPic: creating rx session because could not be found\n");
		}

		PktVideoFeedPic * poPic = ( PktVideoFeedPic * ) new char[ sizeof( PktVideoFeedPic ) + 16 + poPktCastPic->getTotalDataLen() ];
		poPic->setThisDataLen( poPktCastPic->getThisDataLen() );
		memcpy( poPic, poPktCastPic, poPktCastPic->getPktLength() );
		poSession->setVideoFeedPkt( poPic );

		// wait for rest of picture to arrive
	}
}

//============================================================================
void PluginServiceWebCam::onPktVideoFeedPicChunk( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PktVideoFeedPicChunk * poPktPicChunk = ( PktVideoFeedPicChunk * )pktHdr;

	PluginBase::AutoPluginLock pluginMutexLock( this );

	RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( NULL == poSession )
	{
		// this may occur normally with dropped frames due to slow acknowledgment
		//LogMsg( LOG_ERROR, "PluginServiceWebCam::onPktVideoFeedPicChunk: could not find RxSession\n");
		return;
	}

	PktVideoFeedPic * poPktCastPic = poSession->getVideoFeedPkt();
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
			PktVideoFeedPicAck oPkt;
			m_PluginMgr.pluginApiTxPacket(	m_ePluginType, 
				netIdent->getMyOnlineId(), 
				sktBase, 
				&oPkt ); 

			m_PluginMgr.pluginApiPlayVideoFrame(	m_ePluginType, 
				poPktCastPic->getDataPayload(), 
				poPktCastPic->getTotalDataLen(), 
				netIdent,
				poPktCastPic->getMotionDetect() );
			
			delete poSession->getVideoFeedPkt();
			poSession->setVideoFeedPkt( NULL );
		}
	}
}

//============================================================================
void PluginServiceWebCam::onPktVideoFeedPicAck( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( this );

	TxSession * poSession = (TxSession *)m_PluginSessionMgr.findTxSessionByOnlineId( true, netIdent->getMyOnlineId() );
	if( NULL == poSession )
	{
		LogMsg( LOG_ERROR, "PluginServiceWebCam::onPktVideoFeedPicAck: could not find TxSession\n");
		return;
	}

	poSession->decrementOutstandingAckCnt();
}

//============================================================================
void PluginServiceWebCam::onPktVoiceReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginServiceWebCam::onPktVoiceReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginServiceWebCam::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginServiceWebCam::onConnectionLost( VxSktBase * sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
	if( getIsServerInSession() )
	{
		IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
	}
}

//============================================================================
void PluginServiceWebCam::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
	if( getIsServerInSession() )
	{
		IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
	}
}

