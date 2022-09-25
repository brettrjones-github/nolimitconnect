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

#include "PluginCamClient.h"
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

// #define DEBUG_PLUGIN_LOCK

//============================================================================
PluginCamClient::PluginCamClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent* myIdent, EPluginType pluginType )
: PluginBase( engine, pluginMgr, myIdent, pluginType )
, m_PluginSessionMgr( engine, *this, pluginMgr )
, m_VoiceFeedMgr( engine, *this, m_PluginSessionMgr )
, m_VideoFeedMgr( engine, *this, m_PluginSessionMgr )
{
	m_ePluginType = ePluginTypeCamClient;
}

//============================================================================
void PluginCamClient::setIsPluginInSession( bool isInSession )
{
	setIsServerInSession( isInSession );
	IToGui::getToGui().toGuiPluginStatus( m_ePluginType, isInSession ? 1 : 0, isInSession ? m_PluginSessionMgr.getSessionCount() : -1 );
}

//============================================================================
// override this by plugin to create inherited RxSession
RxSession * PluginCamClient::createRxSession( VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return new RxSession( sktBase, netIdent, getPluginType() );
}

//============================================================================
// override this by plugin to create inherited TxSession
TxSession * PluginCamClient::createTxSession( VxSktBase* sktBase, VxNetIdent* netIdent )
{
	return new TxSession( sktBase, netIdent, getPluginType() );
}

//============================================================================
void PluginCamClient::callbackVideoJpgSmall( void * /*userData*/, VxGUID& feedId, uint8_t * jpgData, uint32_t jpgDataLen, int motion0to100000 )
{
	//LogMsg( LOG_VERBOSE, "PluginCamClient::fromGuiVideoData sessions %d\n", m_PluginSessionMgr.m_aoSessions.size() );
	m_PluginMgr.pluginApiPlayVideoFrame( m_ePluginType, jpgData, jpgDataLen, m_MyIdent, motion0to100000 );
}

//============================================================================
void PluginCamClient::sendVidPkt( VxPktHdr * vidPkt, bool requiresAck )
{
	if( m_PluginSessionMgr.getSessionCount() )
	{
		//LogMsg( LOG_VERBOSE, "PluginCamClient::fromGuiVideoData %d clients\n", m_PluginSessionMgr.m_TxSessions.size() );
		std::map<VxGUID, PluginSessionBase *>&	sessionList = m_PluginSessionMgr.getSessions();
		#if defined(DEBUG_PLUGIN_LOCK)
			LogMsg( LOG_VERBOSE, "PluginCamClient::sendVidPkt lock" );
		#endif defined(DEBUG_PLUGIN_LOCK)
		PluginBase::AutoPluginLock pluginMutexLock( this );
		for( auto iter = sessionList.begin(); iter != sessionList.end(); ++iter )
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

#if defined(DEBUG_PLUGIN_LOCK)
		LogMsg( LOG_VERBOSE, "PluginCamClient::sendVidPkt unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	}
}

//============================================================================
void PluginCamClient::callbackOpusPkt( void * userData, PktVoiceReq * pktOpusAudio )
{
	m_VoiceFeedMgr.callbackOpusPkt( userData, pktOpusAudio );
}

//============================================================================
void PluginCamClient::callbackAudioOutSpaceAvail( int freeSpaceLen )
{
	m_VoiceFeedMgr.callbackAudioOutSpaceAvail( freeSpaceLen );
}

//============================================================================
void PluginCamClient::callbackVideoPktPic( void * /*userData*/, VxGUID& feedId, PktVideoFeedPic * pktVid, int pktsInSequence, int thisPktNum )
{
	sendVidPkt( pktVid, true );
}

//============================================================================
void PluginCamClient::callbackVideoPktPicChunk( void * /*userData*/, VxGUID& feedId, PktVideoFeedPicChunk * pktVid, int pktsInSequence, int thisPktNum )
{
	sendVidPkt( pktVid, false );
}

//============================================================================
//! called to start service or session with remote friend
void PluginCamClient::fromGuiStartPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	LogMsg( LOG_VERBOSE, "PluginCamClient::fromGuiStartPluginSession %s", netIdent->getOnlineName() );
	AutoPluginLock pluginMutexLock( this );
	RxSession* rxSession = m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( !rxSession )
	{
		VxSktBase* sktBase = nullptr;
		m_PluginMgr.pluginApiSktConnectTo( ePluginTypeCamClient, netIdent, 0, &sktBase );
		if( 0 != sktBase )
		{
			rxSession = m_PluginSessionMgr.findOrCreateRxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true, lclSessionId );
		}
	}

	if( rxSession )
	{
		if( lclSessionId.isVxGUIDValid() )
		{
			rxSession->setLclSessionId( lclSessionId );
		}
		else
		{
			lclSessionId = rxSession->getLclSessionId();
		}

		requestCamSession( rxSession, false );

		m_VideoFeedMgr.fromGuiStartPluginSession( true, eAppModuleCamClient, netIdent, false );
		m_VoiceFeedMgr.fromGuiStartPluginSession( true, eAppModuleCamClient, netIdent, false );
		setIsPluginInSession( true );
	}
	else
	{
		LogMsg( LOG_VERBOSE, "PluginCamClient::fromGuiStartPluginSession could not connect to %s", netIdent->getOnlineName() );
	}
}

//============================================================================
//! called to stop service or session with remote friend
void PluginCamClient::fromGuiStopPluginSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	LogMsg( LOG_VERBOSE, "PluginCamClient::fromGuiStopPluginSession" );
	PluginBase::AutoPluginLock pluginMutexLock( this );
	bool isMyself = ( netIdent->getMyOnlineId() == m_MyIdent->getMyOnlineId() );
	if( isMyself )
	{
		m_Engine.setHasSharedWebCam(false);
		m_VoiceFeedMgr.fromGuiStopPluginSession( true, eAppModuleCamClient, netIdent, false );
		// don't want video capture anymore
		m_VideoFeedMgr.fromGuiStopPluginSession( true, eAppModuleCamClient, netIdent, false );
		if( true == fromGuiIsPluginInSession() )
		{
			setIsPluginInSession( false );

			PktVideoFeedStatus oPkt;
			oPkt.setFeedStatus( eFeedStatusOffline );

			std::map<VxGUID, PluginSessionBase*>& sessionList = m_PluginSessionMgr.getSessions();
			for( auto iter = sessionList.begin(); iter != sessionList.end(); )
			{
				PluginSessionBase* sessionBase = iter->second;
				if( sessionBase->isTxSession() )
				{
					TxSession* poSession = (TxSession*)sessionBase;
					if( poSession->getSkt() )
					{
						oPkt.setLclSessionId( poSession->getLclSessionId() );
						oPkt.setRmtSessionId( poSession->getRmtSessionId() );
						m_PluginMgr.pluginApiTxPacket( m_ePluginType, poSession->getIdent()->getMyOnlineId(), poSession->getSkt(), &oPkt );
						delete poSession;
						iter = sessionList.erase( iter );
						break;
					}
					else
					{
						++iter;
					}
				}
				else
				{
					++iter;
				}
			}
		}
	}
	else
	{
		m_VoiceFeedMgr.fromGuiStopPluginSession( true, eAppModuleCamClient, netIdent, false );
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
bool PluginCamClient::fromGuiIsPluginInSession( VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	// for cam server we really want to know if server is running
	return getIsServerInSession();
}

//============================================================================
EPluginAccess PluginCamClient::canAcceptNewSession( VxNetIdent* netIdent )
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
bool PluginCamClient::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,		// identity of friend
													int				pvUserData,
													const char *	pOfferMsg,		// offer message
													const char *	pFileName,
													uint8_t *		fileHashId,
													VxGUID			lclSessionId )		// filename if any
{
	VxSktBase* sktBase = NULL;
	LogMsg( LOG_VERBOSE, " PluginCamClient::fromGuiMakePluginOffer %s", netIdent->getOnlineName());

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
                LogMsg( LOG_VERBOSE, " PluginCamClient::fromGuiMakePluginOffer success");
                return true;
            }
            else
            {
                LogMsg( LOG_VERBOSE, " PluginCamClient::fromGuiMakePluginOffer failed to send pkt");
            }
        }
        else
        {
            LogMsg( LOG_ERROR, " PluginCamClient::fromGuiMakePluginOffer failed to create session");
        }
    }

	return false;
}

//============================================================================
bool PluginCamClient::requestCamSession( RxSession* rxSession, bool	bWaitForSuccess )
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
bool PluginCamClient::stopCamSession( VxNetIdent* netIdent,	VxSktBase* sktBase )
{
	LogMsg( LOG_ERROR, "PluginCamClient::stopCamSession");
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
void PluginCamClient::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	if( !isAccessAllowed( netIdent, true, "onPktPluginOfferReq" ) )
	{
		return;
	}

	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktPluginOfferReq" );
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
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktPluginOfferReq REJECTED in session %d canAcceptNewSession %d",
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
void PluginCamClient::onPktPluginOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktPluginOfferReply * poPkt = (PktPluginOfferReply *)pktHdr;
	EOfferResponse eResponse = poPkt->getOfferResponse();
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktPluginOfferReply %d", eResponse );
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
void PluginCamClient::onPktSessionStartReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	if( !isAccessAllowed( netIdent, true, "onPktSessionStartReq" ) )
	{
		return;
	}

	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktSessionStartReq" );
	PktSessionStartReply oPkt;
#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktSessionStartReq lock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	PluginBase::AutoPluginLock pluginMutexLock( this );
	if( getIsServerInSession() && ( ePluginAccessOk == canAcceptNewSession( netIdent ) ) ) 
	{
		oPkt.setOfferResponse(eOfferResponseAccept);
        TxSession * txSession = (TxSession *)m_PluginSessionMgr.findOrCreateTxSessionWithOnlineId( netIdent->getMyOnlineId(), sktBase, netIdent, true );
        if( 0 == txSession )
        {
            LogMsg( LOG_ERROR, "PluginCamClient::onPktSessionStartReq failed to create or find session" );
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
#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktSessionStartReq unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
}

//============================================================================
void PluginCamClient::onPktSessionStartReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktSessionStartReply * poPkt = (PktSessionStartReply *)pktHdr;
#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktSessionStartReply lock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	PluginBase::AutoPluginLock pluginMutexLock( this );
	RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktSessionStartReply %d", poPkt->getOfferResponse() );
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

#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktSessionStartReply unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
}

//============================================================================
void PluginCamClient::onPktSessionStopReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_PluginSessionMgr.removeTxSessionByOnlineId( netIdent->getMyOnlineId(), false );
	if( getIsServerInSession() )
	{
		IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
	}
}

//============================================================================
void PluginCamClient::onPktSessionStopReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	LogMsg( LOG_ERROR, "PluginCamClient::onPktSessionStopReply" );
}

//============================================================================
void PluginCamClient::onPktVideoFeedReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
}

//============================================================================
void PluginCamClient::onPktVideoFeedStatus( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktVideoFeedStatus * pktVideoStatus = ( PktVideoFeedStatus * )pktHdr;
#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedStatus lock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	PluginBase::AutoPluginLock pluginMutexLock( this );
	RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( poSession )
	{
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedStatus %d", pktVideoStatus->getFeedStatus() );
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

#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedStatus unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
}

//============================================================================
void PluginCamClient::onPktVideoFeedPic( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	//LogMsg( LOG_VERBOSE, "PluginCamClient::onPktCastPic\n" );
	PktVideoFeedPicAck oPkt;
	m_PluginMgr.pluginApiTxPacket( m_ePluginType, netIdent->getMyOnlineId(), sktBase, &oPkt ); 

	PktVideoFeedPic * poPktCastPic = ( PktVideoFeedPic * )pktHdr;
	if( poPktCastPic->getTotalDataLen() == poPktCastPic->getThisDataLen() )
	{
		m_Engine.getMediaProcessor().processFriendVideoFeed( netIdent->getMyOnlineId(), poPktCastPic->getDataPayload(), poPktCastPic->getTotalDataLen(), poPktCastPic->getMotionDetect() );
#if defined(DEBUG_PLUGIN_LOCK)
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPic lock" );
#endif defined(DEBUG_PLUGIN_LOCK)
		PluginBase::AutoPluginLock pluginMutexLock( this );
		RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
		if( poSession )
		{
			if( poSession->getVideoFeedPkt() )
			{
				delete poSession->getVideoFeedPkt();
				poSession->setVideoFeedPkt( nullptr );
			}
		}

#if defined(DEBUG_PLUGIN_LOCK)
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPic unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	}
	else
	{
#if defined(DEBUG_PLUGIN_LOCK)
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPic chunk lock" );
#endif defined(DEBUG_PLUGIN_LOCK)
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
			LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPic: creating rx session because could not be found");
		}

		PktVideoFeedPic * poPic = ( PktVideoFeedPic * ) new char[ sizeof( PktVideoFeedPic ) + 16 + poPktCastPic->getTotalDataLen() ];
		poPic->setThisDataLen( poPktCastPic->getThisDataLen() );
		memcpy( poPic, poPktCastPic, poPktCastPic->getPktLength() );
		poSession->setVideoFeedPkt( poPic );

		// wait for rest of picture to arrive
#if defined(DEBUG_PLUGIN_LOCK)
		LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPic chunk unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	}
}

//============================================================================
void PluginCamClient::onPktVideoFeedPicChunk( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktVideoFeedPicChunk * poPktPicChunk = ( PktVideoFeedPicChunk * )pktHdr;
#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPicChunk lock" );
#endif defined(DEBUG_PLUGIN_LOCK)
	PluginBase::AutoPluginLock pluginMutexLock( this );

	RxSession * poSession = (RxSession *)m_PluginSessionMgr.findRxSessionByOnlineId( netIdent->getMyOnlineId(), true );
	if( NULL == poSession )
	{
		// this may occur normally with dropped frames due to slow acknowledgment
		//LogMsg( LOG_ERROR, "PluginCamClient::onPktVideoFeedPicChunk: could not find RxSession\n");
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

#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPicChunk unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
}

//============================================================================
void PluginCamClient::onPktVideoFeedPicAck( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPicAck lock" );
#endif defined(DEBUG_PLUGIN_LOCK)

	PluginBase::AutoPluginLock pluginMutexLock( this );

	TxSession * poSession = (TxSession *)m_PluginSessionMgr.findTxSessionByOnlineId( true, netIdent->getMyOnlineId() );
	if( NULL == poSession )
	{
		LogMsg( LOG_ERROR, "PluginCamClient::onPktVideoFeedPicAck: could not find TxSession");
		return;
	}

	poSession->decrementOutstandingAckCnt();

#if defined(DEBUG_PLUGIN_LOCK)
	LogMsg( LOG_VERBOSE, "PluginCamClient::onPktVideoFeedPicAck unlock" );
#endif defined(DEBUG_PLUGIN_LOCK)
}

//============================================================================
void PluginCamClient::onPktVoiceReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginCamClient::onPktVoiceReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	m_VoiceFeedMgr.onPktVoiceReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginCamClient::replaceConnection( VxNetIdent* netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginCamClient::onConnectionLost( VxSktBase* sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
	if( getIsServerInSession() )
	{
		IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
	}
}

//============================================================================
void PluginCamClient::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
	if( getIsServerInSession() )
	{
		IToGui::getToGui().toGuiPluginStatus( m_ePluginType, 1, m_PluginSessionMgr.getTxSessionCount() );
	}
}

//============================================================================
void PluginCamClient::onNetworkConnectionReady( bool requiresRelay )
{
	if( eFriendStateIgnore != m_MyIdent->getPluginPermission( getPluginType() ) )
	{
		// automatically start web cam on startup if enabled
		enableCamServerService( true );
	}
}

//============================================================================
void PluginCamClient::fromGuiUpdatePluginPermission( EPluginType pluginType, EFriendState pluginPermission )
{
	if( pluginType == getPluginType() )
	{
		enableCamServerService( eFriendStateIgnore != pluginPermission );
	}
}

//============================================================================
void PluginCamClient::stopAllSessions( void )
{
	if( true == fromGuiIsPluginInSession() )
	{
		setIsPluginInSession( false );

		// tell everyone we are no longer online
		PktVideoFeedStatus oPkt;
		oPkt.setFeedStatus( eFeedStatusOffline );

		std::map<VxGUID, PluginSessionBase*>& sessionList = m_PluginSessionMgr.getSessions();
		for( auto iter = sessionList.begin(); iter != sessionList.end(); )
		{
			PluginSessionBase* sessionBase = iter->second;
			if( sessionBase->isTxSession() )
			{
				TxSession* poSession = (TxSession*)sessionBase;
				if( poSession->getSkt() )
				{
					oPkt.setLclSessionId( poSession->getLclSessionId() );
					oPkt.setRmtSessionId( poSession->getRmtSessionId() );
					m_PluginMgr.pluginApiTxPacket( m_ePluginType, poSession->getIdent()->getMyOnlineId(), poSession->getSkt(), &oPkt );
					iter = sessionList.erase( iter );
					delete poSession;
				}
				else
				{
					++iter;
				}
			}
			else
			{
				++iter;
			}
		}
	}

	m_VideoFeedMgr.stopAllSessions( eAppModuleCamClient, getPluginType() );
	m_VoiceFeedMgr.stopAllSessions( eAppModuleCamClient, getPluginType() );
}

//============================================================================
void PluginCamClient::enableCamServerService( bool enable )
{
	if( m_IsCamServiceEnabled == enable )
	{
		return;
	}

	m_IsCamServiceEnabled = enable;

	if( m_IsCamServiceEnabled )
	{
		m_Engine.setHasSharedWebCam( true );
		// request video capture
		m_VideoFeedMgr.fromGuiStartPluginSession( false, eAppModuleCamClient, m_MyIdent, false );
		m_VoiceFeedMgr.fromGuiStartPluginSession( false, eAppModuleCamClient, m_MyIdent, false );
		setIsPluginInSession( true );

	}
	else
	{
		// stop video capture
		m_VideoFeedMgr.fromGuiStopPluginSession( false, eAppModuleCamClient, m_MyIdent, false );
		m_VoiceFeedMgr.fromGuiStopPluginSession( false, eAppModuleCamClient, m_MyIdent, false );
		m_Engine.setHasSharedWebCam( false );
		stopAllSessions();
		setIsPluginInSession( false );
	}
}