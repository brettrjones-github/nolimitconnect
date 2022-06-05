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

#include "NetConnector.h"
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
//#include <ptop_src/ptop_engine_src/HostMgr/HostList.h>

#include <ptop_src/ptop_engine_src/BigListLib/BigListLib.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <GuiInterface/IDefs.h>

#include <NetLib/VxSktConnect.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktCrypto.h>
#include <NetLib/VxSktUtil.h>

#include <PktLib/PktsRelay.h>

#include <CoreLib/VxParse.h>

#include <memory.h>

namespace
{
    const unsigned int		MIN_TIME_BETWEEN_CONNECT_ATTEMPTS_SEC			= (15 * 60); // 15 minutes
	const unsigned int		TIMEOUT_MILLISEC_STAY_CONNECTED					= 2000; 

	//============================================================================
    void * NetConnectorThreadFunction( void * pvParam )
	{
		VxThread * poThread = (VxThread *)pvParam;
		poThread->setIsThreadRunning( true );
		NetConnector * poMgr = (NetConnector *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->doNetConnectionsThread();
        }

		poThread->threadAboutToExit();
        return nullptr;
	}

	//============================================================================
    void * StayConnectedThreadFunction( void * pvParam )
	{
		VxThread * poThread = (VxThread *)pvParam;
		poThread->setIsThreadRunning( true );
		NetConnector * poMgr = (NetConnector *)poThread->getThreadUserParam();

		poMgr->doStayConnectedThread();

		poThread->threadAboutToExit();
        return nullptr;
	}

} // namespace

//============================================================================
NetConnector::NetConnector( P2PEngine& engine )
: m_Engine( engine )
, m_BigListMgr( engine.getBigListMgr() )
, m_PktAnn( engine.getMyPktAnnounce() )
, m_ConnectList( engine.getConnectList() )
, m_NetworkMgr( engine.getNetworkMgr() )
, m_PeerMgr( m_NetworkMgr.getPeerMgr() )
{
}

//============================================================================
NetConnector::~NetConnector()
{
	netConnectorShutdown();
	m_NetConnectThread.killThread();
	stayConnectedShutdown();
	m_StayConnectedThread.killThread();
}

//============================================================================
void NetConnector::netConnectorStartup( void )
{
	m_NetConnectThread.abortThreadRun( true );
	while( m_NetConnectThread.isThreadRunning() )
	{
		LogMsg( LOG_INFO, "NetConnector::startup waiting for old connect thread to die\n" );
		m_WaitForConnectWorkSemaphore.signal();
		VxSleep( 200 );
	}

	m_NetConnectThread.startThread( (VX_THREAD_FUNCTION_T)NetConnectorThreadFunction, this, "NetConnectorThread"  ); 
}

//============================================================================
void NetConnector::stayConnectedStartup( void )
{
	m_StayConnectedThread.abortThreadRun( true );
	while( m_StayConnectedThread.isThreadRunning() )
	{
		LogMsg( LOG_INFO, "NetConnector::startup waiting for stay connected thread to die\n" );
		VxSleep( 200 );
	}

	m_StayConnectedThread.startThread( (VX_THREAD_FUNCTION_T)StayConnectedThreadFunction, this, "StayConnectedThread" ); 

	//LogMsg( LOG_INFO, "NetConnector::startup done\n" );
}

//============================================================================
void NetConnector::netConnectorShutdown( void )
{
	m_NetConnectThread.abortThreadRun( true );
	m_WaitForConnectWorkSemaphore.signal();
}

//============================================================================
void NetConnector::stayConnectedShutdown( void )
{
	m_StayConnectedThread.abortThreadRun( true );
}

////============================================================================
//void NetConnector::handleRandomConnectResults( HostList * anchorList )
//{
//	//handleAnnounceResults( anchorList, eConnectReasonRandomConnect );
//}
//
////============================================================================
//void NetConnector::handleAnnounceResults( HostList * anchorList, EConnectReason connectReason )
//{
//	if( 0 == anchorList->m_EntryCount )
//	{
//		LogMsg( LOG_INFO, "handleAnnounceResults: no entries from anchor\n" );
//	}
//
//	// the list is in time order.. do oldest to newest so newest replace oldest
//	//for( int i = anchorList->m_EntryCount - 1; i >= 0; --i )
//	for( int i = 0; i < anchorList->m_EntryCount; ++i )
//	{	
//		HostListEntry * entry = &anchorList->m_List[i];
//		if( entry->getMyOnlineId() == m_PktAnn.getMyOnlineId() )
//		{
//			// it is ourself
//			continue;
//		}
//
//		std::string onlineName = entry->getOnlineName();
//		if( ( onlineName == "nolimitconnect.net" ) 
//			|| ( onlineName == "nolimitconnect.com" ) )
//		{
//			// hack to exclude nolimitconnect anchor and connect servers.. TODO fix with some kind of settings instead
//			continue;
//		}
//
//		if( false == m_ConnectList.isContactConnected( entry->getMyOnlineId() ) )
//		{
//			ConnectRequest connectRequest( connectReason );
//			VxConnectInfo& connectInfo = connectRequest.getConnectInfo();
//			memcpy( (VxConnectIdent *)&connectInfo, (VxConnectIdent *)entry, sizeof( VxConnectIdent ) );
//
//			addConnectRequestToQue( connectRequest, false, false );
//		}
//		else 
//		{
//			if( eConnectReasonRandomConnectJoin == connectReason )
//			{
//				BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( entry->getMyOnlineId() );
//				m_Engine.getToGui().toGuiScanResultSuccess( eScanTypeRandomConnect, bigListInfo );
//			}
//		}
//	}
//}

//============================================================================
void NetConnector::addConnectRequestToQue( VxConnectInfo& connectInfo, EConnectReason connectReason, bool addToHeadOfQue, bool replaceExisting )
{
    ConnectRequest connectRequest( connectInfo, connectReason );
    addConnectRequestToQue( connectRequest, addToHeadOfQue, replaceExisting );
}

//============================================================================
void NetConnector::addConnectRequestToQue( ConnectRequest& connectRequest, bool addToHeadOfQue, bool replaceExisting  )
{
	m_NetConnectorMutex.lock();
	if( m_IdentsToConnectList.size() )
	{
		// remove any previous that have the same id and reason
		std::vector<ConnectRequest>::iterator iter;
		for( iter = m_IdentsToConnectList.begin(); iter != m_IdentsToConnectList.end(); ++iter )
		{
			if( ( (*iter).getMyOnlineId() == connectRequest.getMyOnlineId() )
				&& ( (*iter).getConnectReason() == connectRequest.getConnectReason() ) )
			{
				if( replaceExisting )
				{
					m_IdentsToConnectList.erase( iter );
					break;
				}
				else
				{
					m_NetConnectorMutex.unlock();
					return;
				}
			}
		}
	}

	if( addToHeadOfQue )
	{
		m_IdentsToConnectList.insert( m_IdentsToConnectList.begin(), connectRequest );
	}
	else
	{
		m_IdentsToConnectList.push_back( connectRequest );
	}

	m_NetConnectorMutex.unlock();
	m_WaitForConnectWorkSemaphore.signal();
}

//============================================================================
bool NetConnector::connectToContact(	VxConnectInfo&		connectInfo, 
										VxSktBase **		ppoRetSkt,
										bool&				retIsNewConnection,
										EConnectReason		connectReason )
{
	bool gotConnected	= false;
	retIsNewConnection	 = false;
	if( connectInfo.getMyOnlineId() == m_PktAnn.getMyOnlineId() )
	{
		LogMsg( LOG_WARN, "NetConnector::connectToContact: connecting to ourself" );  
        *ppoRetSkt = m_Engine.getSktLoopback();
		return true;
	}

#ifdef DEBUG_CONNECTIONS
	LogMsg( LOG_SKT, "connectToContact: id %s %s",  
		connectInfo.getOnlineName(),
		connectInfo.getMyOnlineId().describeVxGUID().c_str() );
#endif // DEBUG_CONNECTIONS

	m_ConnectList.connectListLock();
	RcConnectInfo * rmtUserConnectInfo = m_ConnectList.findConnection( connectInfo.getMyOnlineId(), true );
	if( rmtUserConnectInfo )
	{
#ifdef DEBUG_CONNECTIONS
		std::string strId;
		connectInfo.getMyOnlineId(strId);
		LogMsg( LOG_SKT, "connectToContact: User is already connected %s id %s", 
			m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
			strId.c_str() );
#endif // DEBUG_CONNECTIONS

		VxSktBase* sktBase = rmtUserConnectInfo->m_SktBase;
		if( sktBase && sktBase->isConnected() )
		{
			int64_t timeNow = GetGmtTimeMs();
			sktBase->setLastActiveTimeMs( timeNow );
			sktBase->setLastSessionTimeMs( timeNow );
			*ppoRetSkt = sktBase;
			gotConnected = true;
		}

		m_ConnectList.connectListUnlock();
		if( sktBase && !gotConnected )
		{
			m_ConnectList.removeSocket( sktBase, true );
		}
	}
	
	if( !gotConnected )
	{
		m_ConnectList.connectListUnlock();
		if( connectUsingTcp( connectInfo, ppoRetSkt, connectReason ) )
		{
			gotConnected		= true;
			retIsNewConnection	= true;
		}
	}

	return gotConnected;
}

//============================================================================
bool NetConnector::connectUsingTcp(	VxConnectInfo&		connectInfo, 
									VxSktBase **		ppoRetSkt,
									EConnectReason		connectReason )
{
	* ppoRetSkt = NULL;
	if( false == connectInfo.m_DirectConnectId.isVxGUIDValid() )
	{
		LogMsg( LOG_ERROR, "connectUsingTcp: User invalid online id\n" );
		return false;
	}

	std::string strDirectConnectIp;
	VxSktBase * sktBase = NULL;
	bool requiresRelay = connectInfo.requiresRelay();

	if( ( connectInfo.getMyOnlineIPv4() == m_PktAnn.getMyOnlineIPv4() // uses same external ip
		|| connectReason == eConnectReasonNearbyLan || eConnectReasonSameExternalIp == connectReason ) // on same lan network
		&& connectInfo.getMyOnlineIPv4().isValid()
		&& connectInfo.getLanIPv4().isValid() )
	{
		strDirectConnectIp = connectInfo.getLanIPv4().toStdString();
		// probably on same LAN network
		if( 0 == directConnectTo(	connectInfo, 
									&sktBase, 
									eConnectReasonSameExternalIp ) )
		{		
			LogModule( eLogConnect, LOG_VERBOSE, "connectUsingTcp: SUCCESS skt %d LAN connect to %s ip %s port %d",
				sktBase->m_SktNumber,
				connectInfo.getOnlineName(),
				strDirectConnectIp.c_str(),
				connectInfo.m_DirectConnectId.getPort() );

			requiresRelay = false;
			* ppoRetSkt = sktBase;

			m_PktAnn.setAppAliveTimeSec( GetApplicationAliveSec() );
			PktAnnounce pktAnn;
			m_Engine.copyMyPktAnnounce( pktAnn );

			pktAnn.setMyFriendshipToHim( eFriendStateGuest );
			pktAnn.setHisFriendshipToMe( eFriendStateGuest );
            pktAnn.setIsNearby( true );

			return txPacket( connectInfo.getMyOnlineId(), sktBase, &pktAnn );
		}
		else
		{
			LogModule( eLogConnect, LOG_VERBOSE, "connectUsingTcp: FAIL LAN connect to %s ip %s port %d",
						connectInfo.getOnlineName(),
						strDirectConnectIp.c_str(),
						connectInfo.m_DirectConnectId.getPort() );
		}
	}

	std::string debugClientOnlineId;
	connectInfo.getMyOnlineId(debugClientOnlineId);
	LogModule( eLogConnect, LOG_VERBOSE, "connectUsingTcp %s id %s ip %s", connectInfo.getOnlineName(),
				debugClientOnlineId.c_str(), strDirectConnectIp.c_str() );

	// verify proxy if proxy required
	if( requiresRelay )
	{
		std::string strMyOnlineId;
		connectInfo.getMyOnlineId(strMyOnlineId);
#ifdef DEBUG_CONNECTIONS
		LogMsg( LOG_ERROR, "connectUsingTcp: FAIL User id %s does not have proxy set.. ", 
			strMyOnlineId.c_str());
#endif // DEBUG_CONNECTIONS
		return tryIPv6Connect( connectInfo, ppoRetSkt );
	}

	connectInfo.m_DirectConnectId.getIpAddress( strDirectConnectIp );

	//LogMsg( LOG_INFO, "User %s requires proxy? %d",  connectInfo.m_as8OnlineName, requiresRelay );
	if( false == requiresRelay )
	{
#ifdef DEBUG_CONNECTIONS
		LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: Attempting direct connect to %s ip %s port %d",
			m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
			strDirectConnectIp.c_str(),
			connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
		if( 0 == directConnectTo( connectInfo, &sktBase, connectReason ) )
		{
			//LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: success\n" );
			// direct connection success
#ifdef DEBUG_CONNECTIONS
			LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d direct connect to %s ip %s port %d",
				sktBase->m_SktNumber,
				m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
				strDirectConnectIp.c_str(),
				connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
			* ppoRetSkt = sktBase;

			return true;
		}
		else
		{
			#ifdef DEBUG_CONNECTIONS
				LogMsg( LOG_SKT, "directConnectTo: FAIL LAN connect to %s ip %s port %d",
							connectInfo.getOnlineName(),
							strDirectConnectIp.c_str(),
							connectInfo.m_DirectConnectId.getPort() );
			#endif // DEBUG_CONNECTIONS
			#ifdef SUPPORT_IPV6
				return tryIPv6Connect( connectInfo, ppoRetSkt );
			#else
				return false; // no ipv6 support
			#endif // SUPPORT_IPV6
		}
	}

#ifdef DEBUG_CONNECTIONS
	//LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: returning skt 0x%x\n", *ppoRetSkt );
#endif // DEBUG_CONNECTIONS
	if( *ppoRetSkt )
	{
		return true;
	}

	return tryIPv6Connect( connectInfo, ppoRetSkt );
}

//============================================================================
bool NetConnector::tryIPv6Connect(	VxConnectInfo&		connectInfo, 
									VxSktBase **		ppoRetSkt )
{
	bool connectSuccess = false;
	if( m_PktAnn.getMyOnlineIPv6().isValid()
		&& connectInfo.getMyOnlineIPv6().isValid() )
	{
		std::string ipv6;
		ipv6 = connectInfo.getMyOnlineIPv6().toStdString();
		// not likely to succeed so just see if we can get a socket
		SOCKET skt = ::VxConnectToIPv6( ipv6.c_str(), connectInfo.getOnlinePort() );
		if( INVALID_SOCKET != skt )
		{
			* ppoRetSkt = m_PeerMgr.createConnectionUsingSocket( skt, ipv6.c_str(), connectInfo.getOnlinePort() );
			connectSuccess = ( 0 != *ppoRetSkt );
		}
	}

	return connectSuccess;
}

//============================================================================-
RCODE NetConnector::directConnectTo(	VxConnectInfo&		connectInfo,
										VxSktBase **		ppoRetSkt,		// return pointer to socket if not null
										EConnectReason		connectReason )
{
	RCODE rc = -1;
	* ppoRetSkt = nullptr;
	int	iConnectTimeout = DIRECT_CONNECT_TIMEOUT; // how long to attempt connect

	std::string connectIpAddress;

	if( ( connectReason == eConnectReasonNearbyLan || connectReason == eConnectReasonSameExternalIp )
		&& connectInfo.getLanIPv4().isValid() )
	{
		connectIpAddress = connectInfo.getLanIPv4().toStdString();
		iConnectTimeout = LAN_CONNECT_TIMEOUT;
	}
	else
	{
		connectInfo.m_DirectConnectId.getIpAddress( connectIpAddress );
		if( eConnectReasonRelayService == connectReason )
		{
			iConnectTimeout = MY_PROXY_CONNECT_TIMEOUT;
		}
	}

	VxSktConnect * sktBase = m_PeerMgr.connectTo( connectIpAddress.c_str(),			// remote ip or url 
													connectInfo.getOnlinePort(),	// port to connect to
													iConnectTimeout );				// seconds before connect attempt times out
	if( sktBase )
	{
		LogModule( eLogConnect,  LOG_VERBOSE, "NetConnector::directConnectTo: connect SUCCESS to %s:%d", connectIpAddress.c_str(), connectInfo.getOnlinePort() );
		
		// generate encryption keys
		GenerateTxConnectionKey( sktBase, &connectInfo.m_DirectConnectId, m_NetworkMgr.getNetworkKey() );

		GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_NetworkMgr.getNetworkKey() );

		if( false == sendMyPktAnnounce( connectInfo.getMyOnlineId(), sktBase, true ) )
		{
            LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: connect failed sending announce" );
			return -1;
		}

		rc = 0;
		if( ppoRetSkt )
		{
			*ppoRetSkt = (VxSktBase *)sktBase;
		}
	}
	else
	{
        LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: failed to %s:%d", connectIpAddress.c_str(), connectInfo.getOnlinePort() );
	}

    LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: done" );

	return rc;
}

//============================================================================
//! encrypt and send my PktAnnounce to someone of whom we have no recored except from anchor announce
bool NetConnector::sendMyPktAnnounce(  VxGUID&				destinationId,
									   VxSktBase *			sktBase, 
									   bool					requestAnnReply,
									   bool					requestReverseConnection,
									   bool					requestSTUN )
{
	m_PktAnn.setAppAliveTimeSec( GetApplicationAliveSec() );
	PktAnnounce pktAnn;
	m_Engine.copyMyPktAnnounce( pktAnn );
	pktAnn.setIsPktAnnReplyRequested( requestAnnReply );
	pktAnn.setIsPktAnnRevConnectRequested( requestReverseConnection );
	pktAnn.setIsPktAnnStunRequested( requestSTUN );

	EFriendState eMyFriendshipToHim;
	EFriendState eHisFriendshipToMe;
	m_BigListMgr.getFriendships( destinationId, eMyFriendshipToHim, eHisFriendshipToMe );

	pktAnn.setMyFriendshipToHim( eMyFriendshipToHim );
	pktAnn.setHisFriendshipToMe( eHisFriendshipToMe );

	return txPacket( destinationId, sktBase, &pktAnn );	
}

//============================================================================
bool NetConnector::txPacket(	VxGUID&				destinationId, 
								VxSktBase *			sktBase, 
								VxPktHdr *			poPkt )
{
	bool bSendSuccess = false;
	poPkt->setSrcOnlineId( m_PktAnn.getMyOnlineId() );

	if( 0 == (poPkt->getPktLength() & 0xf ) )
	{
		if( sktBase->isConnected() && sktBase->isTxEncryptionKeySet() )
		{
			sktBase->m_u8TxSeqNum++;
			poPkt->setPktSeqNum( sktBase->m_u8TxSeqNum );
			RCODE rc = sktBase->txPacket( destinationId, poPkt, false );
			if( 0 == rc )
			{
				bSendSuccess = true;
			}
			else
			{
                LogMsg( LOG_ERROR, "NetConnector::txPacket: %s error %d", sktBase->describeSktType().c_str(), rc );
			}
		}
		else
		{
			if( false == sktBase->isConnected() )
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d not connected", sktBase->m_SktNumber );
			}
			else
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d has no encryption key", sktBase->m_SktNumber );
			}
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::txPluginPkt: Invalid system Packet length %d type %d", 
				poPkt->getPktLength(),
				poPkt->getPktType() );
	}

	return bSendSuccess;
}

//============================================================================
void NetConnector::doStayConnectedThread( void )
{
	int iConnectToIdx							= 0;
	VxMutex * poListMutex						= &m_BigListMgr.m_FriendListMutex;
	std::vector< BigListInfo * >& friendList	= m_BigListMgr.m_FriendList;
	int iSize;
	BigListInfo * poInfo;

	VxSktBase * sktBase;
	while( ( false == m_StayConnectedThread.isAborted() )
			&& ( false == VxIsAppShuttingDown() ) )
	{
		VxSleep( TIMEOUT_MILLISEC_STAY_CONNECTED );
		if( false == m_Engine.getNetworkStateMachine().isP2POnline() )
		{
			// don't ping friends until we are fully online with relay service if required and correct connect info in announcement
			continue;
		}

		if( 0 != friendList.size() )
		{
			//LogMsg( LOG_ERROR, "doStayConnected attempt lock\n" );
			poListMutex->lock();
			//LogMsg( LOG_ERROR, "doStayConnected attempt lock success\n" );
			iSize = (int)friendList.size();
			if( iSize )
			{
				iConnectToIdx++;
				if( iConnectToIdx >= iSize )
				{
					iConnectToIdx = 0;
				}

				poInfo = friendList[iConnectToIdx];
				if( false == poInfo->isConnected() )
				{
					if( MIN_TIME_BETWEEN_CONNECT_ATTEMPTS_SEC < ( GetGmtTimeMs() - poInfo->getTimeLastConnectAttemptMs() ) )
					{
						bool isNewConnection = false;
						if( m_Engine.connectToContact( poInfo->getConnectInfo(), &sktBase, isNewConnection, eConnectReasonStayConnected ) )
						{
							poInfo->contactWasAttempted( true );
						}
						else
						{
							poInfo->contactWasAttempted( false );
						}
					}
				}
			}

			poListMutex->unlock();
		}

		if( m_StayConnectedThread.isAborted() )
		{
			return;
		}
	}
}

//============================================================================
void NetConnector::doNetConnectionsThread( void )
{
	while( ( false == m_NetConnectThread.isAborted() )
		&& ( false == VxIsAppShuttingDown() ) )
	{
		m_WaitForConnectWorkSemaphore.wait( 1000 );
		while( m_IdentsToConnectList.size() )
		{
			if( m_NetConnectThread.isAborted() )
			{
				break;
			}

			m_NetConnectorMutex.lock();
			ConnectRequest connectRequest = m_IdentsToConnectList[0];
			m_IdentsToConnectList.erase( m_IdentsToConnectList.begin() );
			m_NetConnectorMutex.unlock();

			doConnectRequest( connectRequest );
		}
	}
}

//============================================================================
bool NetConnector::doConnectRequest( ConnectRequest& connectRequest, bool ignoreToSoonToConnectAgain )
{
	int64_t timeNow = GetGmtTimeMs();
	VxConnectInfo& connectInfo = connectRequest.getConnectInfo();
	if( false == m_Engine.getNetworkStateMachine().isP2POnline() )
	{
		LogMsg( LOG_ERROR, "NetConnector::doConnectRequest when not online" );
	}

	m_ConnectList.connectListLock();
	RcConnectInfo *	rcInfo = m_ConnectList.findConnection( connectRequest.getMyOnlineId(), true );
	if( rcInfo )
	{
		// already connected
		m_ConnectList.connectListUnlock();
		BigListInfo * bigInfo = rcInfo->getBigListInfo();
		bigInfo->setTimeLastTcpContactMs( timeNow );
		bigInfo->setTimeLastConnectAttemptMs( timeNow );
		connectRequest.setTimeLastConnectAttemptMs( timeNow );
		handleConnectSuccess( bigInfo, rcInfo->getSkt(), false, connectRequest.getConnectReason() );
		return true;
	}

	m_ConnectList.connectListUnlock();

	if( ( false == ignoreToSoonToConnectAgain )
		&& connectRequest.isTooSoonToAttemptConnectAgain() )
	{
        LogModule( eLogConnect,  LOG_VERBOSE, "NetConnector::doConnectRequest: to soon to connect again %s\n", m_Engine.describeContact( connectRequest ).c_str() );

		return false;
	}

	connectRequest.setTimeLastConnectAttemptMs( timeNow );
	BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
	if( bigListInfo )
	{
		bigListInfo->setTimeLastConnectAttemptMs( timeNow );
	}

	VxSktBase * retSktBase = NULL;
	bool isNewConnection = false;
	if( m_Engine.connectToContact( connectInfo, &retSktBase, isNewConnection, connectRequest.getConnectReason() ) )
	{
		// handle success connect
        LogModule( eLogConnect,  LOG_VERBOSE, "NetConnector::doConnectRequest: success  %s\n", m_Engine.describeContact( connectInfo ).c_str() );

		if( 0 == bigListInfo )
		{
			// when connected should have created a big list entry when got back a packet announce
			bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
		}

		if( bigListInfo )
		{
			handleConnectSuccess( bigListInfo, retSktBase, isNewConnection, connectRequest.getConnectReason() );
		}
		else
		{
            LogModule( eLogConnect,  LOG_VERBOSE, "NetConnector::doConnectRequest: No BigList for connected  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
		}

		return true;
	}

	// handle fail connect
    LogModule( eLogConnect,  LOG_VERBOSE, "NetConnector::doConnectRequest: connect fail  %s\n", m_Engine.describeContact( connectInfo ).c_str() );

	return false;
}

//============================================================================
void NetConnector::handleConnectSuccess( BigListInfo * bigListInfo, VxSktBase * sktBase, bool isNewConnection, EConnectReason connectReason )
{
	if( bigListInfo )
	{
		int64_t timeNow = GetGmtTimeMs();
		bigListInfo->setTimeLastConnectAttemptMs( timeNow );
		bigListInfo->setIsConnected( true );
		if( eConnectReasonRandomConnectJoin == connectReason )
		{
			m_Engine.getToGui().toGuiScanResultSuccess( eScanTypeRandomConnect, bigListInfo );
		}
		else if( eConnectReasonNearbyLan == connectReason || eConnectReasonSameExternalIp == connectReason )
		{
			m_Engine.getNetworkMgr().getNearbyMgr().handleTcpLanConnectSuccess( bigListInfo, sktBase, isNewConnection, connectReason );
		}
	}
}

////============================================================================
//void NetConnector::handlePossibleRelayConnect(	VxConnectInfo&		connectInfo, 
//												VxSktBase *			sktBase,
//												bool				isNewConnection,
//												EConnectReason		connectReason )
//{
//	NetworkStateMachine& netStateMachine = m_Engine.getNetworkStateMachine();
//	netStateMachine.lockResources();
//	NetworkStateBase * netState = netStateMachine.getCurNetworkState();
//	netStateMachine.unlockResources();
//	if( eNetworkStateTypeRelaySearch == netState->getNetworkStateType() )
//	{
//		( ( NetworkStateRelaySearch *)netState )->handlePossibleRelayConnect(	connectInfo, 
//																				sktBase,
//																				isNewConnection,
//																				connectReason );
//	}
//	else
//	{
//		BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
//		handleConnectSuccess( bigListInfo, sktBase, isNewConnection, connectReason );
//	}
//}

//============================================================================
void NetConnector::closeIfAnnonymous( ESktCloseReason closeReason, VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo )
{
	bool isAnonymouse = true;
	if( NULL == poInfo )
	{
		poInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
	}

	if( poInfo )
	{
		if( ( eFriendStateAnonymous < poInfo->getHisFriendshipToMe() )
			|| poInfo->isMyPreferedRelay()
			|| poInfo->isMyRelay() )
		{
			isAnonymouse = false;
		}
	}

	if( isAnonymouse )
	{
		closeConnection( closeReason, onlineId, skt, poInfo );
	}
}

//============================================================================
void  NetConnector::closeConnection( ESktCloseReason closeReason, VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo )
{
	if( NULL == poInfo )
	{
		poInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
	}

	if( NULL == poInfo )
	{
		LogMsg( LOG_ERROR, "Failed to find info for %s", onlineId.toOnlineIdString().c_str() );
		skt->closeSkt( eSktCloseFindBigInfoFail );
		return;
	}

	m_ConnectList.connectListLock();
	RcConnectInfo * poRmtUserConnectInfo = m_ConnectList.findConnection( onlineId, true );
	if( poRmtUserConnectInfo )
	{
		if( poRmtUserConnectInfo->isRelayServer()
			|| poRmtUserConnectInfo->isRelayClient() )
		{
			PktRelayUserDisconnect pktRelayDisconnect;
			pktRelayDisconnect.setSrcOnlineId( m_Engine.getMyOnlineId() );
			pktRelayDisconnect.setHostOnlineId( m_Engine.getMyOnlineId() );
			pktRelayDisconnect.setUserOnlineId( onlineId );
			skt->txPacket( onlineId, &pktRelayDisconnect );
		}
		else 
		{
			skt->closeSkt( closeReason );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "Failed to find RcConnectInfo for %s", onlineId.toOnlineIdString().c_str() );
		skt->closeSkt( eSktCloseFindConnectedInfoFail );
	}

	m_ConnectList.connectListLock();
}
