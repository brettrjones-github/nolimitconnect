//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "PluginSessionMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"

#include "PluginSessionBase.h"

#include <GuiInterface/IToGui.h>

#include <ptop_src/ptop_engine_src/OfferBase/OfferBaseInfo.h>

#include <NetLib/VxSktBase.h>

#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsPluginOffer.h>
#include <PktLib/PktsSession.h>
#include <PktLib/PktBlobEntry.h>

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxFileUtil.h>

//#define DEBUG_AUTOPLUGIN_LOCK 1

//============================================================================ 
PluginSessionMgr::PluginSessionMgr( P2PEngine& engine, PluginBase& plugin, PluginMgr& pluginMgr )
: SessionMgrBase( engine, plugin, pluginMgr )
{
}

//============================================================================ 
PluginSessionMgr::~PluginSessionMgr()
{
}

//============================================================================
PluginSessionBase* PluginSessionMgr::findPluginSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked )
{
	SessionIter iter;
	iter = m_aoSessions.find( sessionId );
	if( iter != m_aoSessions.end() )
	{
		return (PluginSessionBase*)(*iter).second;
	}

	return NULL;
}

//============================================================================
PluginSessionBase*	 PluginSessionMgr::findPluginSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked )
{
	SessionIter iter;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
		#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::findPluginSessionByOnlineId pluginMutex.lock start" );
		#endif //DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
		#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::findPluginSessionByOnlineId pluginMutex.lock done" );
		#endif //DEBUG_AUTOPLUGIN_LOCK
	}

	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->getOnlineId() == onlineId )
		{
			if( false == pluginIsLocked )
			{
				#ifdef DEBUG_AUTOPLUGIN_LOCK
								LogMsg( LOG_VERBOSE, "PluginSessionMgr::findPluginSessionByOnlineId pluginMutex.unlock" );
				#endif //DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return session;
		}
	}

	if( false == pluginIsLocked )
	{
		#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::findPluginSessionByOnlineId pluginMutex.unlock" );
		#endif //DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
void PluginSessionMgr::replaceConnection( VxNetIdent* netIdent, VxSktBase* poOldSkt, VxSktBase* poNewSkt )
{
	SessionIter iter;
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_VERBOSE, "PluginSessionMgr::replaceConnection autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_VERBOSE, "PluginSessionMgr::replaceConnection autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( poOldSkt == session->getSkt() )
		{
			session->setSkt( poNewSkt );
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_VERBOSE, "PluginSessionMgr::replaceConnection autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}

//============================================================================
//! called when connection is lost
void PluginSessionMgr::onContactWentOffline( VxNetIdent* netIdent, VxSktBase* sktBase )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	bool sktIsDisconnected = !sktBase->isConnected();
	// deadlock occurs here if use continuous lock so get the session to erase but dont end session until lock is remove 
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	bool bErased = true;
	while( bErased ) 
	{
		bErased = false;
		PluginSessionBase* sessionBase = nullptr;
		#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::onContactWentOffline pluginMutex.lock start" );
		#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::onContactWentOffline pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

		for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
		{
			sessionBase = ((*iter).second);
			if( ( netIdent == sessionBase->getIdent() )
				|| ( sktIsDisconnected && ( sessionBase->getSkt() == sktBase ) ) )
			{
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::onContactWentOffline erasing session for %s", sessionBase->getIdent()->getOnlineName() );
				bErased = true;
				break;
			}
		}

		#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::onContactWentOffline pluginMutex.unlock" );
		#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
		if( bErased )
		{
			doEndAndEraseSession( sessionBase, eOfferResponseUserOffline, false );
		}
	}
}

//============================================================================
//! called when connection is lost
void PluginSessionMgr::onConnectionLost( VxSktBase* sktBase )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	bool bErased = true;

	while( bErased ) 
	{
		bErased = false;
		PluginSessionBase* sessionBase = nullptr;
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::onConnectionLost pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::onConnectionLost pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

		for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
		{
			sessionBase = ((*iter).second);
			if( sktBase == sessionBase->getSkt() )
			{
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::onConnectionLost erasing session for %s", sessionBase->getIdent()->getOnlineName() );
				bErased = true;
				break;
			}
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::onConnectionLost pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
		if( bErased )
		{
			doEndAndEraseSession( sessionBase, eOfferResponseUserOffline, false );
		}
	}
}

//============================================================================
void PluginSessionMgr::cancelSessionByOnlineId( VxGUID& onlineId )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	bool bErased = true;

	while( bErased ) 
	{
		bErased = false;
		PluginSessionBase* sessionBase = 0;
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::cancelSessionByOnlineId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::cancelSessionByOnlineId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK

		for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
		{
			sessionBase = ((*iter).second);
			if( sessionBase->getIdent()->getMyOnlineId() == onlineId )
			{
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::cancelSessionByOnlineId erasing session for %s", sessionBase->getIdent()->getOnlineName() );
				bErased = true;
				break;
			}
		}

#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::cancelSessionByOnlineId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
		if( bErased )
		{
			doEndAndEraseSession( sessionBase, eOfferResponseCancelSession, false );
		}
	}
}

//============================================================================
void PluginSessionMgr::doEndAndEraseSession( PluginSessionBase* sessionBase, EOfferResponse offerResponse, bool pluginIsLocked )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	m_Plugin.onSessionEnded( sessionBase, pluginIsLocked, eOfferResponseUserOffline );

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::doEndAndEraseSession pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::doEndAndEraseSession pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( sessionBase == ((*iter).second) )
		{
			m_aoSessions.erase(iter);
			delete sessionBase;
			break;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::doEndAndEraseSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}
}

//============================================================================
bool PluginSessionMgr::fromGuiIsPluginInSession( bool pluginIsLocked, VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	bool isInSesion = false;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::fromGuiIsPluginInSession pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::fromGuiIsPluginInSession pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	if( lclSessionId.isVxGUIDValid() )
	{
		auto iter = m_aoSessions.find( lclSessionId );
		if( iter != m_aoSessions.end() )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::doEndAndEraseSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return true;
		}
	}

	VxGUID& onlineId = netIdent->getMyOnlineId();
	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->getOnlineId() == onlineId )
		{
			isInSesion = true;
			break;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::doEndAndEraseSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return isInSesion;
}

//============================================================================
//! user wants to send offer to friend.. return false if cannot connect
bool PluginSessionMgr::fromGuiMakePluginOffer( bool pluginIsLocked, VxNetIdent* netIdent, OfferBaseInfo& offerInfo )
{
	bool offerSentResult = false;
	PluginSessionBase* pluginSession = nullptr;
	VxGUID& lclSessionId = offerInfo.getOfferId();
	if( lclSessionId.isVxGUIDValid() && ( false == isPluginSingleSession() ) )
	{
		pluginSession = findOrCreateP2PSessionWithSessionId( lclSessionId, nullptr, netIdent, pluginIsLocked );
	}
	else
	{
		pluginSession = findOrCreateP2PSessionWithOnlineId( netIdent->getMyOnlineId(), nullptr, netIdent, pluginIsLocked, lclSessionId );
	}

	if( pluginSession )
	{
		PktPluginOfferReq pktReq;

		pktReq.setLclSessionId( lclSessionId );
		pktReq.setRmtSessionId( lclSessionId );

		offerInfo.addToBlob( pktReq.getBlobEntry() );
		pktReq.calcPktLen();

		offerSentResult = m_Plugin.txPacket( pluginSession->getIdent(), pluginSession->getSkt(), &pktReq );
	}

	return offerSentResult;
}

//============================================================================
//! handle reply to offer
bool PluginSessionMgr::fromGuiOfferReply( bool pluginIsLocked, VxNetIdent* netIdent, OfferBaseInfo& offerInfo )
{
	PluginSessionBase* poOffer = nullptr;
	VxGUID& lclSessionId = offerInfo.getOfferId();
	EOfferResponse offerResponse = offerInfo.getOfferResponse();
	if( lclSessionId.isVxGUIDValid() && ( false == isPluginSingleSession() ) )
	{
		poOffer = findPluginSessionBySessionId( lclSessionId, pluginIsLocked );
	}
	else
	{
		poOffer = findPluginSessionByOnlineId( netIdent->getMyOnlineId(), pluginIsLocked );
	}

	bool bResponseSent = false;
	if( poOffer )
	{
		poOffer->setOfferResponse( offerResponse );
		poOffer->setLclSessionId( lclSessionId );

		PktPluginOfferReply pktReply;
		pktReply.setOfferResponse( offerResponse );
		pktReply.setLclSessionId( poOffer->getLclSessionId() );
		pktReply.setRmtSessionId( poOffer->getRmtSessionId() );
		offerInfo.addToBlob( pktReply.getBlobEntry() );
		if( m_Plugin.txPacket( poOffer->getIdent(), poOffer->getSkt(), &pktReply ) )
		{
			bResponseSent = true;
		}

		if( ( false == bResponseSent ) 
			|| ( eOfferResponseReject == offerResponse )
			|| ( eOfferResponseCancelSession == offerResponse )
			|| ( eOfferResponseEndSession == offerResponse ) )
		{
			removeSession( pluginIsLocked, netIdent, lclSessionId, offerResponse );
		}
		else if( eOfferResponseAccept == offerResponse )
		{
			m_Plugin.onSessionStart( poOffer, pluginIsLocked );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, " PluginBase::fromGuiOfferReply: OFFER NOT FOUND");
	}

	return bResponseSent;
}

//============================================================================
void PluginSessionMgr::fromGuiStopPluginSession( bool pluginIsLocked, VxNetIdent* netIdent, int pvUserData, VxGUID lclSessionId )
{
	removeSession( pluginIsLocked, netIdent, lclSessionId, eOfferResponseEndSession );
}

//============================================================================
void PluginSessionMgr::onPktPluginOfferReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	if( netIdent )
	{
		LogMsg( LOG_ERROR, "PluginSessionMgr::doPktPluginOfferReq: offer from %s %s", netIdent->getOnlineName(), sktBase->describeSktType().c_str() );
		PktPluginOfferReq* pktReq = (PktPluginOfferReq *)pktHdr;

		OfferBaseInfo offerInfo;
		if( offerInfo.extractFromBlob( pktReq->getBlobEntry() ) )
		{
			if( IsPluginSingleSession( offerInfo.getPluginType() ) && m_Plugin.getIsPluginInSession() )
			{
				offerInfo.setOfferResponse( eOfferResponseBusy );

				PktPluginOfferReply pktReply;
				pktReply.setLclSessionId( pktReq->getLclSessionId() );
				pktReply.setRmtSessionId( pktReq->getRmtSessionId() );
				pktReply.setPluginType( getPluginType() );
				offerInfo.addToBlob( pktReply.getBlobEntry() );
				pktReply.calcPktLen();
				m_Plugin.txPacket( netIdent, sktBase, &pktReply );

				// for call missed
				IToGui::getToGui().toGuiRxedPluginOffer( netIdent, offerInfo );

				return;
			}

			VxGUID lclSessionId = pktReq->getLclSessionId();
			PluginSessionBase* pluginSession = nullptr;
			PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );

			if( lclSessionId.isVxGUIDValid() && (false == isPluginSingleSession()) )
			{
				pluginSession = findOrCreateP2PSessionWithSessionId( lclSessionId, nullptr, netIdent, true );
			}
			else
			{
				pluginSession = findOrCreateP2PSessionWithOnlineId( netIdent->getMyOnlineId(), nullptr, netIdent, true, lclSessionId );
			}

			pluginSession->setIsRmtInitiated( true );
			pluginSession->setSkt( sktBase );
			pluginSession->setIdent( netIdent );
			pluginSession->setLclSessionId( lclSessionId );
			pluginSession->setRmtSessionId( pktReq->getLclSessionId() );
			pluginSession->setOfferInfo( offerInfo );

			IToGui::getToGui().toGuiRxedPluginOffer( netIdent, offerInfo );
		}
		else
		{
			LogMsg( LOG_ERROR, "PluginSessionMgr::onPktPluginOfferReq: could not extract offer from %s", netIdent->getOnlineName() );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginSessionMgr::onPktPluginOfferReq: NOT FOUND user %s", netIdent->getOnlineName());
	}
}

//============================================================================
void PluginSessionMgr::onPktPluginOfferReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );

	PktPluginOfferReply* pktReply = (PktPluginOfferReply*)pktHdr;

	OfferBaseInfo offerInfo;
	if( offerInfo.extractFromBlob( pktReply->getBlobEntry() ) )
	{
		VxGUID lclSessionId = pktReply->getLclSessionId();
		PluginSessionBase* poOffer = findOrCreateP2PSessionWithSessionId( lclSessionId, sktBase, netIdent, true );

		if( poOffer )
		{
			// notify gui of response
			poOffer->setLclSessionId( pktReply->getLclSessionId() );
			poOffer->setRmtSessionId( pktReply->getRmtSessionId() );
			poOffer->setOfferResponse( pktReply->getOfferResponse() );
			if( eOfferResponseAccept == poOffer->getOfferResponse() )
			{
				m_Plugin.onSessionStart( poOffer, true );
			}
			else
			{
				removeSession( true, netIdent, poOffer->getRmtSessionId(), poOffer->getOfferResponse(), false );
			}
		}

		IToGui::getToGui().toGuiRxedOfferReply( netIdent, offerInfo );
	}
	else
	{
		LogMsg( LOG_ERROR, "PluginSessionMgr::onPktPluginOfferReq: could not extract offer from %s", netIdent->getOnlineName() );
	}
}

//============================================================================
void PluginSessionMgr::onPktSessionStopReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
	PktSessionStopReq * pkt = (PktSessionStopReq *)pktHdr;
	removeSession( false, netIdent, pkt->getRmtSessionId(), eOfferResponseEndSession );
}

//============================================================================
P2PSession* PluginSessionMgr::findP2PSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked )
{
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionBySessionId pluginMutex.lock start\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionBySessionId pluginMutex.lock done\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	SessionIter iter;
	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( sessionId == (*iter).first )
		{
			PluginSessionBase* session = (*iter).second;
			if( session->isP2PSession() )
			{
				if( false == pluginIsLocked )
				{
#ifdef DEBUG_AUTOPLUGIN_LOCK
					LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionBySessionId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
					pluginMutex.unlock();
				}

				return (P2PSession*)session;
			}
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionBySessionId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
P2PSession* PluginSessionMgr::findP2PSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked )
{
	SessionIter iter;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionByOnlineId pluginMutex.lock start\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionByOnlineId pluginMutex.lock done\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->isP2PSession()
			&& ( session->getOnlineId() == onlineId ) )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionByOnlineId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return (P2PSession*)session;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findP2PSessionByOnlineId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}
//============================================================================
P2PSession* PluginSessionMgr::findOrCreateP2PSessionWithSessionId( VxGUID& sessionId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked )
{
	P2PSession* session = findP2PSessionBySessionId( sessionId, pluginIsLocked );
	if( NULL == session )
	{
		if( NULL == sktBase )
		{
			m_PluginMgr.pluginApiSktConnectTo( getPlugin().getPluginType(), netIdent, 0, &sktBase );
		}

		if( 0 != sktBase )
		{
            session = m_Plugin.createP2PSession( sessionId, sktBase, netIdent );
			addSession( session->getLclSessionId(), session, pluginIsLocked );
		}
	}
	else
	{
		session->setSkt( sktBase );
	}

	return session;
}

//============================================================================
P2PSession* PluginSessionMgr::findOrCreateP2PSessionWithOnlineId( VxGUID& onlineId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked, VxGUID lclSessionId )
{
	P2PSession* session = findP2PSessionByOnlineId( onlineId, pluginIsLocked );
	if( NULL == session )
	{
		if( NULL == sktBase )
		{
			m_PluginMgr.pluginApiSktConnectTo( getPlugin().getPluginType(), netIdent, 0, &sktBase );
		}

		if( 0 != sktBase )
		{
            session = m_Plugin.createP2PSession( sktBase, netIdent );
			if( false == lclSessionId.isVxGUIDValid() )
			{
				lclSessionId.initializeWithNewVxGUID();
				session->setLclSessionId( lclSessionId );
				addSession( onlineId, session, pluginIsLocked );
			}
			else
			{
				session->setLclSessionId( lclSessionId );
				addSession( onlineId, session, pluginIsLocked );
			}
		}
	}
	else
	{
		session->setSkt( sktBase );
	}

	return session;
}

//============================================================================
TxSession * PluginSessionMgr::findTxSessionBySessionId( bool pluginIsLocked, VxGUID& sessionId )
{
	SessionIter iter;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionBySessionId pluginMutex.lock start\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionBySessionId pluginMutex.lock done\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( sessionId == (*iter).first )
		{
			PluginSessionBase* session = (*iter).second;
			if( session->isTxSession() )
			{
				if( false == pluginIsLocked )
				{
#ifdef DEBUG_AUTOPLUGIN_LOCK
					LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionBySessionId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK	
					pluginMutex.unlock();
				}

				return (TxSession *)session;
			}
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionBySessionId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK					
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
TxSession * PluginSessionMgr::findTxSessionByOnlineId( bool pluginIsLocked, VxGUID& onlineId )
{
	SessionIter iter;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionByOnlineId pluginMutex.lock start\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionByOnlineId pluginMutex.lock done\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->isTxSession()
			&& ( session->getOnlineId() == onlineId ) )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionByOnlineId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK					
				pluginMutex.unlock();
			}

			return (TxSession *)session;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findTxSessionByOnlineId pluginMutex.unlock\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK					
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
TxSession * PluginSessionMgr::findOrCreateTxSessionWithSessionId( VxGUID& sessionId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked )
{
	TxSession * session = findTxSessionBySessionId( pluginIsLocked, sessionId );
	if( NULL == session )
	{
        session = m_Plugin.createTxSession( sessionId, sktBase, netIdent );
		addSession( sessionId, session, pluginIsLocked );
	}
	else
	{
		session->setSkt( sktBase );
	}

	return session;
}

//============================================================================
TxSession * PluginSessionMgr::findOrCreateTxSessionWithOnlineId( VxGUID& onlineId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked, VxGUID lclSessionId )
{
	TxSession * session = findTxSessionByOnlineId( pluginIsLocked, onlineId );
	if( NULL == session )
	{
        session = m_Plugin.createTxSession( sktBase, netIdent );
		if( ( false == lclSessionId.isVxGUIDValid() ) && ( false == isPluginSingleSession() ) )
		{
			addSession( session->getLclSessionId(), session, pluginIsLocked );
		}
		else
		{
			session->setLclSessionId( lclSessionId );
			addSession( lclSessionId, session, pluginIsLocked );
		}
	}
	else
	{
		session->setSkt( sktBase );
	}

	return session;
}

//============================================================================
int PluginSessionMgr::getTxSessionCount( bool pluginIsLocked )
{
	int txSessionCnt = 0;
	if( !pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::getTxSessionCount lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->isTxSession() )
		{
			txSessionCnt++;
		}
	}

	if( !pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::getTxSessionCount lock end" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}

	return txSessionCnt;
}

//============================================================================
RxSession * PluginSessionMgr::findRxSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked  )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionBySessionId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionBySessionId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( sessionId == (*iter).first )
		{
			PluginSessionBase* session = (*iter).second;
			if( session->isRxSession() )
			{
				if( false == pluginIsLocked )
				{
#ifdef DEBUG_AUTOPLUGIN_LOCK
					LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionBySessionId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
					m_Plugin.unlockPlugin();
				}

				return (RxSession *)session;
			}
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionBySessionId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}

	return NULL;
}

//============================================================================
RxSession * PluginSessionMgr::findRxSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked  )
{
	SessionIter iter;
	VxMutex& pluginMutex = m_Plugin.getPluginMutex();
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionByOnlineId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.lock();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionByOnlineId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->isRxSession()
			&& ( session->getOnlineId() == onlineId ) )
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionByOnlineId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
				pluginMutex.unlock();
			}

			return (RxSession *)session;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::findRxSessionByOnlineId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		pluginMutex.unlock();
	}

	return NULL;
}

//============================================================================
RxSession * PluginSessionMgr::findOrCreateRxSessionWithSessionId( VxGUID& sessionId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked )
{
	RxSession * session = findRxSessionBySessionId( sessionId, pluginIsLocked );
	if( NULL == session )
	{
        session = m_Plugin.createRxSession( sessionId, sktBase, netIdent );
		addSession( sessionId, session, pluginIsLocked );
	}

	return session;
}

//============================================================================
RxSession * PluginSessionMgr::findOrCreateRxSessionWithOnlineId( VxGUID& onlineId, VxSktBase* sktBase, VxNetIdent* netIdent, bool pluginIsLocked, VxGUID lclSessionId )
{
	RxSession * session = findRxSessionByOnlineId( onlineId, pluginIsLocked );
	if( NULL == session )
	{
        session = m_Plugin.createRxSession( sktBase, netIdent );
		if( false == lclSessionId.isVxGUIDValid() )
		{
			addSession( session->getLclSessionId(), session, pluginIsLocked );
		}
		else
		{
			session->setLclSessionId( lclSessionId );
			addSession( lclSessionId, session, pluginIsLocked );
		}
	}

	return session;
}


//============================================================================ 
void PluginSessionMgr::addSession( VxGUID& sessionId, PluginSessionBase* session, bool pluginIsLocked )
{
	if( false == session->getLclSessionId().isVxGUIDValid() )
	{
		if( sessionId.isVxGUIDValid() )
		{
			session->setLclSessionId( sessionId );
		}
		else
		{
			session->getLclSessionId().initializeWithNewVxGUID();
		}
	}

	if( sessionId != session->getLclSessionId() )
	{
		LogMsg( LOG_WARNING, "WARNING SESSION IDS DONT MATCH PluginSessionMgr::addSession %s session id %s connect info %s", session->getOnlineName(), sessionId.toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
	}

	if( false == sessionId.isVxGUIDValid() )
	{
		sessionId = session->getLclSessionId();
	}

	LogMsg( LOG_VERBOSE, "PluginSessionMgr::addSession %s session id %s connect info %s", session->getOnlineName(), sessionId.toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
	if( pluginIsLocked )
	{
		m_aoSessions.insert(  std::make_pair( sessionId, session ) );
	}
	else
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::addSession autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::addSession autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_aoSessions.insert(  std::make_pair( sessionId, session ) );
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::addSession autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}
}

//============================================================================ 
void PluginSessionMgr::endPluginSession( PluginSessionBase* session, bool pluginIsLocked )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( session == (*iter).second )
		{
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession %s session id %s connect info %s", session->getOnlineName(), session->getLclSessionId().toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
			delete (*iter).second;
			m_aoSessions.erase(iter);
			break;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}
}

//============================================================================ 
void PluginSessionMgr::endPluginSession( VxGUID& sessionId, bool pluginIsLocked )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	SessionIter iter = m_aoSessions.find( sessionId );
	if( iter != m_aoSessions.end() )
	{
		PluginSessionBase* session = (*iter).second;
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession %s session id %s connect info %s", session->getOnlineName(), session->getLclSessionId().toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
		m_aoSessions.erase(iter);
		delete session;
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::endPluginSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}
}

//============================================================================ 
void PluginSessionMgr::removeTxSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionBySessionId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionBySessionId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( (sessionId == (*iter).first) )
		{
			PluginSessionBase* session = (*iter).second;
			if( session->isTxSession() )
			{
				m_aoSessions.erase(iter);	
				delete session;
				break;
			}
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionBySessionId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}
}

//============================================================================ 
void PluginSessionMgr::removeTxSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionByOnlineId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionByOnlineId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->isTxSession()
			&& ( session->getOnlineId() == onlineId ) )
		{
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionByOnlineId %s session id %s connect info %s", session->getOnlineName(), session->getLclSessionId().toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
			delete session;
			m_aoSessions.erase(iter);	
			break;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeTxSessionByOnlineId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}
}

//============================================================================ 
void PluginSessionMgr::removeRxSessionBySessionId( VxGUID& sessionId, bool pluginIsLocked )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionBySessionId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionBySessionId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		if( (sessionId == (*iter).first) )
		{
			PluginSessionBase* session = (*iter).second;
			if( session->isRxSession() )
			{
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionBySessionId %s session id %s connect info %s", session->getOnlineName(), session->getLclSessionId().toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
				delete session;
				m_aoSessions.erase(iter);	
				break;
			}
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionBySessionId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}
}

//============================================================================ 
void PluginSessionMgr::removeRxSessionByOnlineId( VxGUID& onlineId, bool pluginIsLocked )
{
	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionByOnlineId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionByOnlineId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->isRxSession()
			&& ( session->getOnlineId() == onlineId ) )
		{
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionByOnlineId %s session id %s connect info %s", 
				session->getOnlineName(), session->getLclSessionId().toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
			m_aoSessions.erase( iter );
			delete session;
			break;
		}
	}

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeRxSessionByOnlineId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}
}

//============================================================================ 
// returns true if found and removed session
bool PluginSessionMgr::removeSessionBySessionId( bool pluginIsLocked, VxGUID& sessionId, EOfferResponse offerResponse )
{
	bool wasRemoved = false;
	if( sessionId.isVxGUIDValid() )
	{
		if( false == pluginIsLocked )
		{
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSessionBySessionId pluginMutex.lock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
			m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSessionBySessionId pluginMutex.lock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		}

		auto iter = m_aoSessions.find( sessionId );
		if( iter != m_aoSessions.end() )
		{
			PluginSessionBase* session = (*iter).second;
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSessionBySessionId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
				m_Plugin.unlockPlugin();
			}

			doEndAndEraseSession( session, offerResponse, pluginIsLocked );
			wasRemoved = true;
		}
		else
		{
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSessionBySessionId pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
				m_Plugin.unlockPlugin();
			}
		}
	}

	return wasRemoved;
}

//============================================================================
bool PluginSessionMgr::removeSession( bool pluginIsLocked, VxNetIdent* netIdent, VxGUID& sessionId, EOfferResponse offerResponse, bool fromGui )
{
	if( removeSessionBySessionId( pluginIsLocked, sessionId, offerResponse ) )
	{
		return true;
	}

	VxGUID& onlineId = netIdent->getMyOnlineId();

	if( false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSession pluginMutex.lock start\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.lockPlugin();
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSession pluginMutex.lock done\n" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	}

	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;
		if( session->getOnlineId() == onlineId )
		{
			if( false == fromGui )
			{
				// notify gui session removed ??
			}
			
			LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSession %s session id %s connect info %s", 
				session->getOnlineName(), session->getLclSessionId().toHexString().c_str(), session->getSkt()->describeSktType().c_str() );
			if( false == pluginIsLocked )
			{
#ifdef DEBUG_AUTOPLUGIN_LOCK
				LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
				m_Plugin.unlockPlugin();
			}

			doEndAndEraseSession( session, offerResponse, pluginIsLocked );
			return true;
		}
	}

	if(  false == pluginIsLocked )
	{
#ifdef DEBUG_AUTOPLUGIN_LOCK
		LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeSession pluginMutex.unlock" );
#endif // DEBUG_AUTOPLUGIN_LOCK
		m_Plugin.unlockPlugin();
	}

	return false;
}

//============================================================================ 
void PluginSessionMgr::removeAllSessions( bool testSessionsOnly )
{
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeAllSessions autoLock start" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	PluginBase::AutoPluginLock pluginMutexLock( &m_Plugin );
#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeAllSessions autoLock done" );
#endif // DEBUG_AUTOPLUGIN_LOCK
	for( auto iter = m_aoSessions.begin(); iter != m_aoSessions.end(); ++iter )
	{
		PluginSessionBase* session = (*iter).second;

		if( ( false == testSessionsOnly )
			|| ( ( true == testSessionsOnly ) && session->isInTest() ) )
		{
			delete session;
			m_aoSessions.erase(iter);
		}
		else
		{
			++iter;
		}
	}

#ifdef DEBUG_AUTOPLUGIN_LOCK
	LogMsg( LOG_VERBOSE, "PluginSessionMgr::removeAllSessions autoLock destroy" );
#endif // DEBUG_AUTOPLUGIN_LOCK
}
