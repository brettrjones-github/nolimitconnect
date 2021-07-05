//============================================================================
// Copyright (C) 2019 Brett R. Jones
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

#include "PluginGroupHost.h"
#include "PluginMgr.h"

#include "RelayServerSession.h"
#include "RelayClientSession.h"
#include "Relay.h"
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <PktLib/PktsRelay.h>
#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsRelay.h>

#include <NetLib/VxSktBase.h>

#include <string.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif
 
//============================================================================
PluginGroupHost::PluginGroupHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBase( engine, pluginMgr, myIdent )
, m_PluginSessionMgr( *this, pluginMgr )
, m_TestTimer()
, m_bVerbose( false )
, m_bTest( false )
, m_MaxUserRelayAllowCnt( 1 )
, m_MaxAnonRelayAllowCnt( 2 )
{
    m_ePluginType = ePluginTypeGroupHost;
}

//============================================================================
PluginGroupHost::~PluginGroupHost()
{
}

//============================================================================
void PluginGroupHost::fromGuiRelayPermissionCount( int userPermittedCount, int anonymousCount )
{
	m_MaxUserRelayAllowCnt = userPermittedCount;
	if( 65000 < m_MaxUserRelayAllowCnt )
	{
		m_MaxUserRelayAllowCnt = 65000;
	}

	if( 0 < m_MaxUserRelayAllowCnt )
	{
		m_MaxUserRelayAllowCnt = 0;
	}
	
	if( 2 > m_MaxAnonRelayAllowCnt )
	{
		m_MaxAnonRelayAllowCnt = 2;
	}

	if( 65000 < m_MaxAnonRelayAllowCnt )
	{
		m_MaxAnonRelayAllowCnt = 65000;
	}
}

//============================================================================
EPluginAccessState PluginGroupHost::canAcceptNewSession( VxNetIdent * netIdent )
{
	if( m_PluginMgr.getPktAnnounce().requiresRelay() )
	{
		return ePluginAccessDisabled;
	}

	int userSessionCount = 0;
	int anonSessionCount = 0;
	std::map<VxGUID, PluginSessionBase *>& sessions = m_PluginSessionMgr.getSessions();
	std::map<VxGUID, PluginSessionBase *>::iterator iter;
	for( iter = sessions.begin(); iter != sessions.end(); ++iter )
	{
		PluginSessionBase * sesIdent = iter->second;
        if( ePluginAccessOk == sesIdent->getIdent()->getHisAccessPermissionFromMe( m_ePluginType ) )
		{
			userSessionCount++;
		}
		else
		{
			anonSessionCount++;
		}
	}

	if( ( ePluginAccessOk == netIdent->getHisAccessPermissionFromMe( m_ePluginType ) )
		&& ( userSessionCount < m_MaxUserRelayAllowCnt ) )
	{
		LogMsg( LOG_INFO, "Plugin Permitted Relay Access Ok User %s\n", netIdent->getOnlineName() );
		return ePluginAccessOk;
	}

	if( anonSessionCount < m_MaxAnonRelayAllowCnt )
	{
		LogMsg( LOG_INFO, "Plugin Anonymous Relay Access Ok User %s\n", netIdent->getOnlineName() );
		return ePluginAccessOk;
	}

	LogMsg( LOG_INFO, "Plugin Relay Busy user cnt %d anon cnt %d\n", userSessionCount, anonSessionCount );
	return ePluginAccessBusy;
}

//============================================================================
bool PluginGroupHost::isUserRelayOk( VxGUID& srcOnlineId, VxGUID& destOnlineId )
{
	if( findRelay( destOnlineId ) || findRelay( srcOnlineId )  )
	{
		return true;
	}

	return false;
}

//============================================================================
EPluginAccessState PluginGroupHost::handlePktRelayServiceReq( BigListInfo * bigListInfo, VxSktBase * sktBase, PktRelayServiceReq * relayServiceReq, PktRelayServiceReply& pktReply )
{
	pktReply.setAccessState( ePluginAccessBusy );
	if( relayServiceReq->m_u8CancelService )
	{
		// we are canceling service
		m_PluginSessionMgr.cancelSessionByOnlineId( bigListInfo->getMyOnlineId() );
	}
	else
	{
		if( m_PluginMgr.getPktAnnounce().requiresRelay() )
		{
			// cannot provide relay service if we require relay ourself
			pktReply.setAccessState( ePluginAccessDisabled );
		}
		else
		{
			if( m_FriendGuidList.doesGuidExist( bigListInfo->getMyOnlineId() )
				||  m_AnonGuidList.doesGuidExist( bigListInfo->getMyOnlineId() ) )
			{
				// already exists in list
				pktReply.setAccessState( ePluginAccessOk );
			}
			else
			{
				// first see if has permission.. if so can be added to user list else see if have room in anon list
				bool grantedUserSession = false;
				if( ePluginAccessOk == bigListInfo->getHisAccessPermissionFromMe( m_ePluginType ) )
				{
					if( m_FriendGuidList.size() < m_MaxUserRelayAllowCnt )
					{
						m_FriendGuidList.addGuid( bigListInfo->getMyOnlineId() );
						grantedUserSession = true;
					}
				}

				bool grantedAnonSession = false;
				if( false == grantedUserSession )
				{
					// try anonymous list
					if( m_AnonGuidList.size() < m_MaxAnonRelayAllowCnt )
					{
						m_AnonGuidList.addGuid( bigListInfo->getMyOnlineId() );
						grantedAnonSession = true;
					}
				}

				if( grantedUserSession || grantedAnonSession )
				{
					pktReply.setAccessState( ePluginAccessOk );
					VxGUID rmtSessionId = relayServiceReq->getLclSessionId();
					if( false == rmtSessionId.isVxGUIDValid() )
					{
						rmtSessionId.initializeWithNewVxGUID();
					}

					RelaySession * session = startNewRelayService( sktBase, bigListInfo, rmtSessionId );
					if( NULL == session )
					{
						pktReply.setAccessState( ePluginAccessBusy );
					}
				}
			}
		}
	}

	return pktReply.getAccessState();
}

//============================================================================
// called by Plugin Session Mgr.. plugin mutex is NOT locked
void PluginGroupHost::onSessionEnded( PluginSessionBase * poSession, bool pluginIsLocked, EOfferResponse eOfferResponse )
{
	if( false == pluginIsLocked )
	{
		m_PluginMutex.lock();
	}

	m_FriendGuidList.removeGuid( poSession->getIdent()->getMyOnlineId() );
	m_AnonGuidList.removeGuid( poSession->getIdent()->getMyOnlineId() );

	if( false == pluginIsLocked )
	{
		m_PluginMutex.unlock();
	}
}

//============================================================================
//! user wants to send offer to friend
bool PluginGroupHost::fromGuiMakePluginOffer(	VxNetIdent *	netIdent,
											int				pvUserData, 
											const char *	pOfferMsg, 
											const char *	pFileName,
											uint8_t *			fileHashId,
											VxGUID			lclSessionId )
{
	bool bOfferSent = false;
	m_bVerbose = true;
	m_bTest = true;

	if( 0 == strcmp( pOfferMsg, "ActionTest" ) )
	{
		/*
		m_TestTimer.startTimer();
		m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Starting Relay Test", m_TestTimer.elapsedSec() );
		RelaySession * poSession = requestRelayService(  netIdent );
		if( NULL == poSession )
		{
			m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Relay Test FAIL", m_TestTimer.elapsedSec() );

			m_bVerbose = false;
			m_bTest = false;
		}
		else
		{
			bOfferSent = true;
		}
		*/
		bOfferSent = doRelayTest( netIdent );
        LogMsg( LOG_INFO, "PluginGroupHost::fromGuiMakePluginOffer: doRelayTest done\n" );
	}
	else
	{
        LogMsg( LOG_ERROR, "PluginGroupHost::fromGuiMakePluginOffer: Invalid Offer\n" );
	}
	return bOfferSent;
}

//============================================================================
RelaySession * PluginGroupHost::startNewRelayService( VxSktBase * sktBase, VxNetIdent * netIdent, VxGUID& rmtInstance )
{
	RelayServerSession * serverSession = findOrCreateRelayServer( sktBase, netIdent );
	serverSession->setRmtSessionId( rmtInstance );
	if( m_bVerbose )
	{
//			m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f startNewRelayService", m_TestTimer.elapsedSec() );
	}

	return serverSession;
}

//============================================================================
RelaySession * PluginGroupHost::startNewRelayClient( VxSktBase * sktBase, VxNetIdent * netIdent, VxGUID& rmtInstance )
{
	RelayClientSession * poSession = findOrCreateRelayClient( sktBase, netIdent );
	poSession->setRmtSessionId( rmtInstance );
	if( m_bVerbose )
	{
//		m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f startNewRelayClient", m_TestTimer.elapsedSec() );
	}

	return poSession;
}

//============================================================================
RelaySession * PluginGroupHost::requestRelayService( VxNetIdent * netIdent, RelayClientSession * poSessionIn, bool bTest )
{
	VxSktBase * sktBase = NULL;
	RelaySession * poSession = poSessionIn;
	bool bNewSession = ( NULL == poSessionIn );

	m_PluginMgr.pluginApiSktConnectTo( m_ePluginType, netIdent, 0, &sktBase );
	if( sktBase )
	{
		if( m_bVerbose )
		{
			//m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Connected.. sending session request", m_TestTimer.elapsedSec() );
		}

		if( bNewSession )
		{
			// make offer and keep copy
			poSession = new RelayClientSession( sktBase, netIdent );
		}

		poSession->setSkt( sktBase );
		poSession->setIdent( netIdent );

		PktRelaySessionReq oPkt;
		oPkt.setLclSessionId( poSession->getLclSessionId() );
		if( txPacket( poSession->getIdent(), sktBase, &oPkt ) )
		{
			if( bNewSession )
			{
				m_PluginSessionMgr.addSession( netIdent->getMyOnlineId(), poSession, false );
			}

			if( m_bVerbose )
			{
				//m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f session request sent", m_TestTimer.elapsedSec() );
			}
		}
		else
		{
			if( bNewSession )
			{
				delete poSession;
				poSession = NULL;
			}
			if( m_bVerbose )
			{
				//m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f send session request FAILED", m_TestTimer.elapsedSec() );
			}
		}
	}
	else if( m_bVerbose )
	{
		//m_PluginMgr.pluginApiLog( m_ePluginType, "%3.3f Connected.. sending session request", m_TestTimer.elapsedSec() );
	}

	return poSession;
}

//============================================================================
PluginSessionBase * PluginGroupHost::findRelay( VxGUID& onlineId )
{
	AutoPluginLock pluginMutexLock( this );
	std::map<VxGUID, PluginSessionBase *>&	sessionList = m_PluginSessionMgr.getSessions();
	std::map<VxGUID, PluginSessionBase *>::iterator iter =  sessionList.find( onlineId );
	if( iter != sessionList.end() )
	{
		return iter->second;
	}

	return NULL;
}

//============================================================================
RelayClientSession * PluginGroupHost::findOrCreateRelayClient( VxSktBase * sktBase, VxNetIdent * netIdent )
{
	RelayClientSession * poSession = NULL;

	PluginSessionMgr::SessionIter iter;
	AutoPluginLock pluginMutexLock( this );
	std::map<VxGUID, PluginSessionBase *>&	sessionList = m_PluginSessionMgr.getSessions();
	for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
	{
		RelaySession * thisSession = (RelaySession *)(*iter).second;
		if( ePluginSessionTypeRelayClient == thisSession->getSessionType() )
		{
			if( netIdent == thisSession->getIdent() )
			{
				thisSession->setSkt( sktBase );
				poSession = (RelayClientSession *) thisSession;
				break;
			}
		}
	}

	if( NULL == poSession )
	{
		poSession = new RelayClientSession( sktBase, netIdent );
		m_PluginSessionMgr.addSession( netIdent->getMyOnlineId(), poSession, false );
	}

	return (RelayClientSession *)poSession;
}

//============================================================================
RelayServerSession * PluginGroupHost::findOrCreateRelayServer(  VxSktBase * sktBase, VxNetIdent * netIdent )
{
	RelayServerSession * poSession = NULL;

	PluginSessionMgr::SessionIter iter;

	{
		AutoPluginLock pluginMutexLock( this );
		std::map<VxGUID, PluginSessionBase *>&	sessionList = m_PluginSessionMgr.getSessions();
		for( iter = sessionList.begin(); iter != sessionList.end(); ++iter )
		{
			RelaySession * thisSession = (RelaySession *)(*iter).second;
			if( ePluginSessionTypeRelayServer == thisSession->getSessionType() )
			{
				if( netIdent == thisSession->getIdent() )
				{
					thisSession->setSkt( sktBase );
					poSession = (RelayServerSession *) thisSession;
					break;
				}
			}
		}
	}

	if( NULL == poSession )
	{
		poSession = new RelayServerSession( sktBase, netIdent);
		m_PluginSessionMgr.addSession( netIdent->getMyOnlineId(), poSession, false );
	}

	return poSession;
}

//============================================================================
void PluginGroupHost::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
	m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginGroupHost::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
	m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
	m_FriendGuidList.removeGuid( netIdent->getMyOnlineId() );
}

//============================================================================
void PluginGroupHost::onConnectionLost( VxSktBase * sktBase )
{
	m_PluginSessionMgr.onConnectionLost( sktBase );
}
