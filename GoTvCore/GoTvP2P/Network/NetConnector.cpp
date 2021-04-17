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
// http://www.nolimitconnect.com
//============================================================================

#include "NetConnector.h"
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateRelaySearch.h>

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/HostMgr/HostList.h>

#include <GoTvCore/GoTvP2P/BigListLib/BigListLib.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

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

//============================================================================
void NetConnector::handleRandomConnectResults( HostList * anchorList )
{
	//handleAnnounceResults( anchorList, eConnectReasonRandomConnect );
}

//============================================================================
void NetConnector::handleAnnounceResults( HostList * anchorList, EConnectReason connectReason )
{
	if( 0 == anchorList->m_EntryCount )
	{
		LogMsg( LOG_INFO, "handleAnnounceResults: no entries from anchor\n" );
	}

	// the list is in time order.. do oldest to newest so newest replace oldest
	//for( int i = anchorList->m_EntryCount - 1; i >= 0; --i )
	for( int i = 0; i < anchorList->m_EntryCount; ++i )
	{	
		HostListEntry * entry = &anchorList->m_List[i];
		if( entry->getMyOnlineId() == m_PktAnn.getMyOnlineId() )
		{
			// it is ourself
			continue;
		}

		std::string onlineName = entry->getOnlineName();
		if( ( onlineName == "nolimitconnect.net" ) 
			|| ( onlineName == "nolimitconnect.com" ) )
		{
			// hack to exclude nolimitconnect anchor and connect servers.. TODO fix with some kind of settings instead
			continue;
		}

		if( false == m_ConnectList.isContactConnected( entry->getMyOnlineId() ) )
		{
			ConnectRequest connectRequest( connectReason );
			VxConnectInfo& connectInfo = connectRequest.getConnectInfo();
			memcpy( (VxConnectIdent *)&connectInfo, (VxConnectIdent *)entry, sizeof( VxConnectIdent ) );

			addConnectRequestToQue( connectRequest, false, false );
		}
		else 
		{
			if( eConnectReasonRandomConnectJoin == connectReason )
			{
				BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( entry->getMyOnlineId() );
				m_Engine.getToGui().toGuiScanResultSuccess( eScanTypeRandomConnect, bigListInfo );
			}
		}
	}
}

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
										bool&				retIsNewConnection )
{
	bool gotConnected	= false;
	retIsNewConnection	 = false;
	if( connectInfo.getMyOnlineId() == m_PktAnn.getMyOnlineId() )
	{
		LogMsg( LOG_ERROR, "NetConnector::connectToContact: cannot connect to ourself\n" );  
		return false;
	}

#ifdef DEBUG_CONNECTIONS
	LogMsg( LOG_SKT, "connectToContact: id %s %s\n",  
		connectInfo.getOnlineName(),
		connectInfo.getMyOnlineId().describeVxGUID().c_str() );
#endif // DEBUG_CONNECTIONS
	//if( 30200 == connectInfo.getOnlinePort() ) // Core8's port
	//{
	//	// just for debug break point
	//	LogMsg( LOG_INFO, "Connecting to Core8's port\n" );
	//}

	m_ConnectList.connectListLock();
	RcConnectInfo * rmtUserConnectInfo = m_ConnectList.findConnection( connectInfo.getMyOnlineId() );
	if( rmtUserConnectInfo )
	{
#ifdef DEBUG_CONNECTIONS
		std::string strId;
		connectInfo.getMyOnlineId(strId);
		LogMsg( LOG_SKT, "connectToContact: User is already connected %s id %s\n", 
			m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
			strId.c_str() );
#endif // DEBUG_CONNECTIONS

		* ppoRetSkt = rmtUserConnectInfo->m_SktBase;
		m_ConnectList.connectListUnlock();
		gotConnected = true;
	}
	else
	{
		m_ConnectList.connectListUnlock();
		if( connectUsingTcp( connectInfo, ppoRetSkt ) )
		{
			gotConnected		= true;
			retIsNewConnection	= true;
		}
	}

	return gotConnected;
}

//============================================================================
bool NetConnector::connectUsingTcp(	VxConnectInfo&		connectInfo, 
									VxSktBase **		ppoRetSkt )
{
	* ppoRetSkt = NULL;
	if( false == connectInfo.m_DirectConnectId.isVxGUIDValid() )
	{
		LogMsg( LOG_ERROR, "connectUsingTcp: User invalid online id\n" );
		return false;
	}

	std::string strDirectConnectIp;
	VxSktBase * sktBase = NULL;

	//if( 8319 == connectInfo.getOnlinePort() ) // Tarzan's port
	//{
	//	// just for debug break point
	//	LogMsg( LOG_INFO, "Connecting to Tarzan's port\n" );
	//}

	//if( 30200 == connectInfo.getOnlinePort() ) // Core8's port
	//{
	//	// just for debug break point
	//	LogMsg( LOG_INFO, "Connecting to Core8's port\n" );
	//}

	//if( 7771 == connectInfo.getOnlinePort() ) 
	//{
	//	// just for debug break point
	//	LogMsg( LOG_INFO, "Connecting to Mary's port\n" );
	//}

	if( ( connectInfo.getMyOnlineIPv4() == m_PktAnn.getMyOnlineIPv4() )
		&& connectInfo.getMyOnlineIPv4().isValid()
		&& connectInfo.getLanIPv4().isValid() )
	{
		if( connectInfo.getMyOnlinePort() == m_PktAnn.getMyOnlinePort() )
		{
			LogMsg( LOG_ERROR, "ERROR connectUsingTcp: attempting connect to our ip and port for different id %s\n", connectInfo.getOnlineName() );
			return false;
		}

		// probably on same network so use local ip
		if( 0 == directConnectTo(	connectInfo, 
									&sktBase, 
									LAN_CONNECT_TIMEOUT,
									false, 
									true ) )
		{
#ifdef DEBUG_CONNECTIONS
			connectInfo.getLanIPv4().toStdString( strDirectConnectIp );
			LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d LAN connect to %s ip %s port %d\n",
				sktBase->m_iSktId,
				connectInfo.getOnlineName(),
				strDirectConnectIp.c_str(),
				connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
			* ppoRetSkt = sktBase;
			return true;
		}
#ifdef DEBUG_CONNECTIONS
		else
		{
			LogMsg( LOG_SKT, "connectUsingTcp: FAIL LAN connect to %s ip %s port %d\n",
				connectInfo.getOnlineName(),
				strDirectConnectIp.c_str(),
				connectInfo.m_DirectConnectId.getPort() );
		}
#endif // DEBUG_CONNECTIONS
	}

#ifdef DEBUG_CONNECTIONS
	std::string debugClientOnlineId;
	connectInfo.getMyOnlineId(debugClientOnlineId);
	LogMsg( LOG_INFO, "connectUsingTcp %s id %s \n", 
		connectInfo.getOnlineName(),
		debugClientOnlineId.c_str() );
#endif // DEBUG_CONNECTIONS

	// verify proxy if proxy required
	bool requiresRelay = connectInfo.requiresRelay();
	if( requiresRelay )
	{
		if( connectInfo.m_RelayConnectId.isVxGUIDValid() )
		{
			if( connectInfo.m_RelayConnectId.getOnlineId() == m_PktAnn.getMyOnlineId() )
			{
				// we are this persons proxy
				// dont attempt to connect.. he has to connect to us
#ifdef DEBUG_CONNECTIONS
				LogMsg( LOG_INFO, "We are Users proxy must wait for them to connect to me\n" );
#endif // DEBUG_CONNECTIONS
				// try ipv6 if available
				return tryIPv6Connect( connectInfo, ppoRetSkt );
			}

			//LogMsg( LOG_INFO, "User %s has proxy.. attempting proxy connect\n",  connectInfo.m_as8OnlineName );
		}
		else
		{
			std::string strMyOnlineId;
			connectInfo.getMyOnlineId(strMyOnlineId);
#ifdef DEBUG_CONNECTIONS
			LogMsg( LOG_ERROR, "connectUsingTcp: FAIL User id %s does not have proxy set.. \n", 
				strMyOnlineId.c_str());
#endif // DEBUG_CONNECTIONS
			return tryIPv6Connect( connectInfo, ppoRetSkt );
		}
	}

	connectInfo.m_DirectConnectId.getIpAddress( strDirectConnectIp );

	//LogMsg( LOG_INFO, "User %s requires proxy? %d\n",  connectInfo.m_as8OnlineName, requiresRelay );
	if( false == requiresRelay )
	{
#ifdef DEBUG_CONNECTIONS
		LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: Attempting direct connect to %s ip %s port %d\n",
			m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
			strDirectConnectIp.c_str(),
			connectInfo.m_DirectConnectId.getPort() );
#endif // DEBUG_CONNECTIONS
		if( 0 == directConnectTo( connectInfo, &sktBase, DIRECT_CONNECT_TIMEOUT ) )
		{
			//LogMsg( LOG_INFO, "P2PEngine::connectUsingTcp: success\n" );
			// direct connection success
#ifdef DEBUG_CONNECTIONS
			LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS skt %d direct connect to %s ip %s port %d\n",
				sktBase->m_iSktId,
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
				LogMsg( LOG_SKT, "directConnectTo: FAIL LAN connect to %s ip %s port %d\n",
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
	else
	{
		std::string strRelayConnectIp;
		connectInfo.m_RelayConnectId.getIpAddress( strRelayConnectIp );
		std::string strUserId;
		connectInfo.getMyOnlineId( strUserId );
#ifdef DEBUG_MUTEXES
		LogMsg( LOG_SKT, "connectUsingTcp: m_ConnectListMutex.lock()\n" );
#endif // DEBUG_MUTEXES
		m_ConnectList.connectListLock();
		RcConnectInfo * poRelayConnectInfo = m_ConnectList.findConnection( connectInfo.m_RelayConnectId.getOnlineId() );
		//LogMsg( LOG_INFO, "P2PEngine::ConnectToContact:User %s existing connection to proxy %d\n", connectInfo.getOnlineName(), poConnectInfo );
		if( poRelayConnectInfo )
		{
			sktBase = poRelayConnectInfo->m_SktBase;
#ifdef DEBUG_NET_CONNECTOR
			LogMsg( LOG_SKT, "connectUsingTcp: FOUND User %s proxy %s connection skt %d proxy ip %s port %d to ip %s port %d id %s\n",
				m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
				poRelayConnectInfo->m_BigListInfo->getOnlineName(),
				poRelayConnectInfo->m_SktBase->m_iSktId,
				strRelayConnectIp.c_str(),
				connectInfo.m_RelayConnectId.getPort(),
				strDirectConnectIp.c_str(), 
				connectInfo.m_DirectConnectId.getPort(),
				strUserId.c_str() );
#endif // DEBUG_NET_CONNECTOR
			// now send announce to remote user
			bool requestReverseConnection = ( ( false == m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );
			bool requestSTUN = ( ( m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );

			//LogMsg( LOG_INFO, "sendMyPktAnnounce 1\n" ); 
			bool sendAnnResult = sendMyPktAnnounce(	connectInfo.getMyOnlineId(), 
													sktBase, 
													true, 
													getShouldRequestTop10(),
													requestReverseConnection,
													requestSTUN );
#ifdef DEBUG_NET_CONNECTOR
			LogMsg( LOG_SKT, "connectUsingTcp: m_ConnectList.connectListUnlock()\n" );
#endif // DEBUG_NET_CONNECTOR
			m_ConnectList.connectListUnlock();
			if( false == sendAnnResult )
			{
                //RCODE rc = sktBase->getLastSktError();
				//LogMsg( LOG_INFO, "Error %d %s Transmitting PktAnn to contact %s\n", 
				//	rc, 
				//	sktBase->describeSktError( rc ),
				//	m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ) );
				sktBase->closeSkt( 2154 );
				sktBase = NULL;
			}
		}
		else
		{
#ifdef DEBUG_NET_CONNECTOR
			LogMsg( LOG_SKT, "connectUsingTcp: m_ConnectList.connectListUnlock()\n" );
#endif // DEBUG_NET_CONNECTOR
			m_ConnectList.connectListUnlock();
			//LogMsg( LOG_INFO, "P2PEngine::ConnectToContact:User %s RmtUserRelayConnectTo\n", connectInfo.getOnlineName() );
			// attempt to connect to users proxy
			if( 0 == rmtUserRelayConnectTo( connectInfo, &sktBase, TO_PROXY_CONNECT_TIMEOUT ) )
			{
#ifdef DEBUG_NET_CONNECTOR
				LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS connect proxy skt %d to %s ip %s port %d for id %s\n",
					sktBase->m_iSktId,
					m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
					strRelayConnectIp.c_str(),
					connectInfo.m_RelayConnectId.getPort(),
					strUserId.c_str() );
#endif // DEBUG_NET_CONNECTOR
			}
			else
			{
#ifdef DEBUG_NET_CONNECTOR
                if( IsLogEnabled( eLogConnect ) )
				    LogMsg( LOG_SKT, "connectUsingTcp: FAIL connect to proxy to %s with proxy ip %s port %d for id %s\n",
					    m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() ),
					    strRelayConnectIp.c_str(),
					    connectInfo.m_RelayConnectId.getPort(),
					    strUserId.c_str() );
#endif // DEBUG_NET_CONNECTOR
			}
		}

		if( sktBase  )
		{
			// if user is behind proxy but we are not then wait for user to direct connect to us
			if( ( false == m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() )
			{
				for( int i = 0; i < 60; i++ )
				{
					if( m_Engine.isContactConnected( connectInfo.getMyOnlineId() ) )
					{
						break;
					}

					VxSleep( 100 );
				}
			}
			else if( sendRequestConnectionThroughRelay(	sktBase, connectInfo ) )
			{
				sktBase->m_RelayEventSemaphore.wait( THROUGH_PROXY_RESPONSE_TIMEOUT );
			}

			m_ConnectList.connectListLock();
			RcConnectInfo * poUserConnectInfo = m_ConnectList.findConnection( connectInfo.getMyOnlineId() );
			if( poUserConnectInfo )
			{
#ifdef DEBUG_NET_CONNECTOR
				LogMsg( LOG_SKT, "connectUsingTcp: SUCCESS requestConnectionThroughRelay %s to %s\n",
					m_Engine.knownContactNameFromId( connectInfo.m_RelayConnectId.getOnlineId() ),
					m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() )
					);
#endif // DEBUG_NET_CONNECTOR
				* ppoRetSkt = sktBase;
			}
			else
			{
#ifdef DEBUG_NET_CONNECTOR
				LogMsg( LOG_SKT, "connectUsingTcp: FAIL requestConnectionThroughRelay %s to %s\n",
					m_Engine.knownContactNameFromId( connectInfo.m_RelayConnectId.getOnlineId() ),
					m_Engine.knownContactNameFromId( connectInfo.getMyOnlineId() )
					);
#endif // DEBUG_NET_CONNECTOR
			}

			m_ConnectList.connectListUnlock();
		}

#ifdef DEBUG_SKTS
		else
		{
			LogMsg( LOG_SKT, "connectUsingTcp: NULL Socket\n" );
		}
#endif // DEBUG_SKTS
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

//============================================================================
bool NetConnector::sendRequestConnectionThroughRelay(	VxSktBase *			sktBase, 
														VxConnectInfo&		connectInfo )
{
	// request connection to user through his proxy

	PktRelayConnectReq oPkt;
	oPkt.m_DestOnlineId =  connectInfo.m_DirectConnectId;
#ifdef DEBUG_CONNECTIONS
	LogMsg( LOG_INFO, "NetworkMgr::sendRequestConnectionThroughRelay\n" );
#endif // DEBUG_CONNECTIONS
	if( true == m_Engine.txSystemPkt( connectInfo.m_RelayConnectId.getOnlineId(), sktBase, &oPkt ) && sktBase->isConnected() )
	{
		return true;
	}

	return false;
}

//============================================================================-
RCODE NetConnector::directConnectTo(	VxConnectInfo&		connectInfo,
										VxSktBase **		ppoRetSkt,		// return pointer to socket if not null
										int					iConnectTimeout,// how long to attempt connect
										bool				bUseUdpIp,
										bool				bUseLanIp )
{
	RCODE rc = -1;
	* ppoRetSkt = NULL;

	std::string strIpAddress;

	if( bUseLanIp 
			&& connectInfo.getLanIPv4().isValid() )
	{
		strIpAddress = connectInfo.getLanIPv4().toStdString();
	}
	else
	{
		connectInfo.m_DirectConnectId.getIpAddress( strIpAddress );
	}

	//if( 32522 ==  connectInfo.getOnlinePort() ) // joes port
	//{
	//	// just for debug break point
	//	LogMsg( LOG_INFO, "Connecting to Joe's port\n" );
	//}

	//if( 7771 ==  connectInfo.getOnlinePort() ) 
	//{
	//	// just for debug break point
	//	LogMsg( LOG_INFO, "Connecting to Mary's port\n" );
	//}

	VxSktConnect * sktBase = m_PeerMgr.connectTo(	strIpAddress.c_str(),			// remote ip or url 
													connectInfo.getOnlinePort(),	// port to connect to
													iConnectTimeout );				// seconds before connect attempt times out
	if( sktBase )
	{
		//LogMsg( LOG_INFO, "NetConnector::directConnectTo: connect SUCCESS to %s:%d\n", strIpAddress.c_str(), connectInfo.getOnlinePort() );
		// generate encryption keys
#ifdef DEBUG_SKTS
		LogMsg( LOG_SKT, "NetworkMgr::DirectConnectTo: connect success.. generating tx key\n" );
#endif // DEBUG_SKTS

		GenerateTxConnectionKey( sktBase, &connectInfo.m_DirectConnectId, m_NetworkMgr.getNetworkKey() );
#ifdef DEBUG_SKTS
		LogMsg( LOG_SKT, "NetworkMgr::DirectConnectTo: connect success.. generating rx key\n" );
#endif // DEBUG_SKTS

		GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_NetworkMgr.getNetworkKey() );
#ifdef DEBUG_SKTS
		LogMsg( LOG_SKT, "NetworkMgr::DirectConnectTo: connect success.. sending announce\n" );
#endif // DEBUG_SKTS

		//LogMsg( LOG_INFO, "sendMyPktAnnounce 2\n" ); 
		if( false == sendMyPktAnnounce( connectInfo.getMyOnlineId(), sktBase, true, getShouldRequestTop10() ) )
		{
            LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: connect failed sending announce\n" );
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

		//LogMsg( LOG_INFO, "NetConnector::directConnectTo: connect FAIL to %s:%d\n", strIpAddress.c_str(), connectInfo.getOnlinePort() );
        LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: failed\n" );
	}

    LogModule( eLogConnect, LOG_DEBUG, "NetworkMgr::DirectConnectTo: done\n" );

	return rc;
}

//============================================================================-
//! attempt connect to remote user's proxy ( only connects and send pkt announce )
RCODE NetConnector::rmtUserRelayConnectTo(	VxConnectInfo&		connectInfo,
											VxSktBase **		ppoRetSkt,			// return pointer to socket if not null
											int					iConnectTimeout )	// seconds before connect attempt times out
{
	RCODE rc = -1;
	std::string strIpAddress;
	connectInfo.m_RelayConnectId.getIpAddress( strIpAddress );

	VxSktConnect * sktBase = m_PeerMgr.connectTo(	strIpAddress.c_str(),					// remote ip or url 
													connectInfo.m_RelayConnectId.getPort(),	// port to connect to
													iConnectTimeout );						// seconds before connect attempt times out
	if( sktBase )
	{
		// generate encryption keys
		GenerateTxConnectionKey( sktBase, &connectInfo.m_RelayConnectId, m_NetworkMgr.getNetworkKey() );
		GenerateRxConnectionKey( sktBase, &m_PktAnn.m_DirectConnectId, m_NetworkMgr.getNetworkKey() );

		// we are connected to users proxy
		// first send announcement to his proxy then to him
		VxGUID& oRelayOnlineId = connectInfo.m_RelayConnectId.getOnlineId();
		LogMsg( LOG_INFO, "sendMyPktAnnounce 3\n" ); 
		bool bResult =  sendMyPktAnnounce(	oRelayOnlineId, 
			sktBase, 
			eFriendStateAnonymous, 
			eFriendStateAnonymous,
			true,
			false );
		if( true == bResult )
		{
			// now send announce to remote user
			bool requestReverseConnection = ( ( false == m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );
			bool requestSTUN = ( ( m_PktAnn.requiresRelay() ) && connectInfo.requiresRelay() );
			LogMsg( LOG_INFO, "sendMyPktAnnounce 4\n" ); 
			bResult = sendMyPktAnnounce( connectInfo.getMyOnlineId(), 
										sktBase, 
										true, 
										getShouldRequestTop10(),
										requestReverseConnection,
										requestSTUN );
			if( false == bResult )
			{
				RCODE rc = sktBase->getLastSktError();
				LogMsg( LOG_INFO, "Error %d %s Transmitting PktAnn to contact\n", rc, sktBase->describeSktError( rc ) );
				sktBase->closeSkt( 2154 );
				sktBase = NULL;
			}
		}
		else
		{
			sktBase->closeSkt( 2155 );
			sktBase = NULL;
		}

		if( ppoRetSkt )
		{
			*ppoRetSkt = (VxSktBase *)sktBase;
		}
	}

	return rc;
}

//============================================================================
//! encrypt and send my PktAnnounce to someone of whom we have no recored except from anchor announce
bool NetConnector::sendMyPktAnnounce(  VxGUID&				destinationId,
									   VxSktBase *			sktBase, 
									   bool					requestAnnReply,
									   bool					requestTop10,
									   bool					requestReverseConnection,
									   bool					requestSTUN )
{
	m_PktAnn.setAppAliveTimeSec( GetApplicationAliveSec() );
	PktAnnounce pktAnn;
	memcpy( &pktAnn, &m_PktAnn, sizeof(PktAnnounce) );
	pktAnn.setIsPktAnnReplyRequested( requestAnnReply );
	pktAnn.setIsTopTenRequested( requestTop10 );
	pktAnn.setIsPktAnnRevConnectRequested( requestReverseConnection );
	pktAnn.setIsPktAnnStunRequested( requestSTUN );

	EFriendState eMyFriendshipToHim;
	EFriendState eHisFriendshipToMe;
	m_BigListMgr.getFriendships( destinationId, eMyFriendshipToHim, eHisFriendshipToMe );

	pktAnn.setMyFriendshipToHim( eMyFriendshipToHim );
	pktAnn.setHisFriendshipToMe( eHisFriendshipToMe );

	//LogMsg( LOG_INFO, "SendMyPktAnnounce; to %s Hi 0x%llX, Lo 0x%llX skt %d request reply %d\n", 
	//	m_Engine.knownContactNameFromId( destinationId ),
	//	destinationId.getVxGUIDHiPart(),
	//	destinationId.getVxGUIDLoPart(),
	//	sktBase->m_iSktId,
	//	requestAnnReply );

	return txPacket( destinationId, sktBase, &pktAnn );	
}

//============================================================================
bool NetConnector::getShouldRequestTop10( void )
{
	if( (  m_PktAnn.requiresRelay() && ( false == m_Engine.getConnectList().isMyRelayAvailable() ) )
		|| ( eScanTypeNone != m_Engine.getRcScan().getScanType() ) )
	{
		return true;
	}

	return false;
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
                LogMsg( LOG_ERROR, "NetConnector::txPacket: %s error %d\n", sktBase->describeSktType().c_str(), rc );
			}
		}
		else
		{
			if( false == sktBase->isConnected() )
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d not connected\n", sktBase->m_iSktId );
			}
			else
			{
				LogMsg( LOG_ERROR, "P2PEngine::txSystemPkt: error skt %d has no encryption key\n", sktBase->m_iSktId );
			}
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "P2PEngine::txPluginPkt: Invalid system Packet length %d type %d\n", 
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

	std::vector< BigListInfo * >::iterator iter;
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
		if( eConnectReasonRelaySearch != connectRequest.getConnectReason() )
		{
			LogMsg( LOG_ERROR, "NetConnector::doConnectRequest when not online\n" );
		}
	}


	P2PConnectList& connectedList = m_Engine.getConnectList();
	connectedList.connectListLock();
	RcConnectInfo *	rcInfo = connectedList.findConnection( connectRequest.getMyOnlineId() );
	if( rcInfo )
	{
		// already connected
		connectedList.connectListUnlock();
		BigListInfo * bigInfo = rcInfo->getBigListInfo();
		bigInfo->setTimeLastTcpContactMs( timeNow );
		bigInfo->setTimeLastConnectAttemptMs( timeNow );
		connectRequest.setTimeLastConnectAttemptMs( timeNow );
		handleConnectSuccess( bigInfo, rcInfo->getSkt(), false, connectRequest.getConnectReason() );
		return true;
	}

	connectedList.connectListUnlock();

	if( ( false == ignoreToSoonToConnectAgain )
		&& connectRequest.isTooSoonToAttemptConnectAgain() )
	{
		#ifdef DEBUG_CONNECTIONS
			LogMsg( LOG_INFO, "NetConnector::doConnectRequest: to soon to connect again %s\n", m_Engine.describeContact( connectRequest ).c_str() );
		#endif // DEBUG_CONNECTIONS
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
		#ifdef DEBUG_CONNECTIONS
			LogMsg( LOG_INFO, "NetConnector::doConnectRequest: success  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
		#endif // DEBUG_CONNECTIONS
		if( 0 == bigListInfo )
		{
			// when connected should have created a big list entry when got back a packet announce
			bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
		}

		if( bigListInfo )
		{
			handleConnectSuccess( bigListInfo, retSktBase, isNewConnection, connectRequest.getConnectReason() );
		}
#ifdef DEBUG_CONNECTIONS
		else
		{
			LogMsg( LOG_INFO, "NetConnector::doConnectRequest: No BigList for connected  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
		}
#endif // DEBUG_CONNECTIONS

		return true;
	}

	// handle fail connect
#ifdef DEBUG_CONNECTIONS
	LogMsg( LOG_INFO, "NetConnector::doConnectRequest: connect fail  %s\n", m_Engine.describeContact( connectInfo ).c_str() );
#endif // DEBUG_CONNECTIONS
	return false;
}

//============================================================================
void NetConnector::handleConnectSuccess(  BigListInfo * bigListInfo, VxSktBase * skt, bool isNewConnection, EConnectReason connectReason )
{
	if( 0 != bigListInfo )
	{
		int64_t timeNow = GetGmtTimeMs();
		bigListInfo->setTimeLastConnectAttemptMs( timeNow );
		bigListInfo->setIsOnline( true );
		bigListInfo->setIsConnected( true );
		if( eConnectReasonRandomConnectJoin == connectReason )
		{
			m_Engine.getToGui().toGuiScanResultSuccess( eScanTypeRandomConnect, bigListInfo );
		}
	}
}

//============================================================================
void NetConnector::handlePossibleRelayConnect(	VxConnectInfo&		connectInfo, 
												VxSktBase *			sktBase,
												bool				isNewConnection,
												EConnectReason		connectReason )
{
	NetworkStateMachine& netStateMachine = m_Engine.getNetworkStateMachine();
	netStateMachine.lockResources();
	NetworkStateBase * netState = netStateMachine.getCurNetworkState();
	netStateMachine.unlockResources();
	if( eNetworkStateTypeRelaySearch == netState->getNetworkStateType() )
	{
		( ( NetworkStateRelaySearch *)netState )->handlePossibleRelayConnect(	connectInfo, 
																				sktBase,
																				isNewConnection,
																				connectReason );
	}
	else
	{
		BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( connectInfo.getMyOnlineId() );
		handleConnectSuccess( bigListInfo, sktBase, isNewConnection, connectReason );
	}
}

//============================================================================
void NetConnector::closeIfAnnonymous( VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo )
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
		closeConnection( onlineId, skt, poInfo );
	}
}

//============================================================================
void  NetConnector::closeConnection( VxGUID& onlineId, VxSktBase * skt, BigListInfo * poInfo )
{
	if( NULL == poInfo )
	{
		poInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
	}

	if( NULL == poInfo )
	{
		LogMsg( LOG_ERROR, "Failed to find info for %s\n", onlineId.toHexString().c_str() );
		skt->closeSkt( 235 );
		return;
	}

	RcConnectInfo * poRmtUserConnectInfo = m_ConnectList.findConnection( onlineId );
	if( poRmtUserConnectInfo )
	{
		if( poRmtUserConnectInfo->isRelayServer()
			|| poRmtUserConnectInfo->isRelayClient() )
		{
			PktRelayUserDisconnect pktRelayDisconnect;
			pktRelayDisconnect.setSrcOnlineId( m_Engine.getMyPktAnnounce().getMyOnlineId() );
			pktRelayDisconnect.m_UserId = onlineId;
			skt->txPacket( onlineId, &pktRelayDisconnect );
		}
		else 
		{
			skt->closeSkt( 236 );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "Failed to find RcConnectInfo for %s\n", onlineId.toHexString().c_str() );
		skt->closeSkt( 237 );
	}
}
