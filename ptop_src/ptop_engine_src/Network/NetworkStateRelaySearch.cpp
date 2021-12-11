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

#include "NetworkStateRelaySearch.h"
#include "NetworkStateMachine.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginServiceRelay.h>

#include <ptop_src/ptop_engine_src/HostMgr/HostList.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServicesMgr.h>
#include <ptop_src/ptop_engine_src/Network/NetConnector.h>

#include <PktLib/PktsRelay.h>
#include <NetLib/VxSktConnectSimple.h>

#include <time.h>
#include <string.h>
#include <atomic>

namespace
{
	const int	RELAY_SERVICE_REQUEST_TIMEOUT_MS			= 6000;
	//============================================================================
    void * NetworkStateRelaySearchThreadFunction( void * pvParam )
	{
        static std::atomic<int> threadCnt(0);
        threadCnt++;
        //LogMsg( LOG_INFO, "NetworkStateRelaySearchThreadFunction thread starting %d\n", threadCnt );

		VxThread * poThread = (VxThread *)pvParam;
		poThread->setIsThreadRunning( true );
		NetworkStateRelaySearch * poMgr = (NetworkStateRelaySearch *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->doRelayAnnounceListAndSearch();
            poMgr->setRelayListSearchComplete( true );
        }

		poThread->threadAboutToExit();
        threadCnt--;
        //LogMsg( LOG_INFO, "NetworkStateRelaySearchThreadFunction Thread exiting %d\n", threadCnt );
        return nullptr;
	}
} // namespace

//============================================================================
NetworkStateRelaySearch::NetworkStateRelaySearch( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
, m_ConnectionList( stateMachine.getEngine().getConnectList() )
, m_NetworkMgr( m_Engine.getNetworkMgr() )
, m_NetServiceUtils( m_NetServicesMgr.getNetUtils() )
, m_PreferredRelayList( stateMachine.getEngine().getConnectList().getPreferredRelayList() )
, m_PossibleRelayList( stateMachine.getEngine().getConnectList().getPossibleRelayList() )
, m_eRelayRequestStatus( eRelayStatusUnknown )
, m_SearchThreadComplete( false )
, m_HaveRelayService( false )
, m_DirectConnectTester( stateMachine.getDirectConnectTester() )
{
	setNetworkStateType( eNetworkStateTypeRelaySearch );
}

//============================================================================
NetworkStateRelaySearch::~NetworkStateRelaySearch()
{
	m_RelaySearchThread.abortThreadRun( true );
	m_RelaySearchThread.killThread();
}

//============================================================================
void NetworkStateRelaySearch::enterNetworkState( void )
{
	//m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeRelaySearch );
}

//============================================================================
void NetworkStateRelaySearch::exitNetworkState( void )
{
	m_RelaySearchThread.abortThreadRun( true );
	m_RelaySearchThread.killThread();
}

//============================================================================
void NetworkStateRelaySearch::fromGuiUseRelay( VxConnectInfo& connectInfo, bool useRelay )
{
	m_RelaySearchMutex.lock();
	if( useRelay )
	{
		m_FromGuiRelayList.push_back( connectInfo );
	}
	else
	{
		std::vector<ConnectRequest>::iterator iter;
		for( iter = m_FromGuiRelayList.begin(); iter != m_FromGuiRelayList.end(); ++iter )
		{
			if( (*iter).getMyOnlineId() == connectInfo.getMyOnlineId() )
			{
				m_FromGuiRelayList.erase( iter );
				break;
			}
		}

		m_PreferredRelayList.removeContactInfo( connectInfo );
	}

	m_RelaySearchMutex.unlock();
}

//============================================================================
void NetworkStateRelaySearch::runNetworkState( void )
{
    LogModule( eLogNetworkRelay, LOG_INFO, "111 NetworkStateRelaySearch::runNetworkState start\n" );
    // BRJ for now do not do relay stuff.. relay service now must be provided by group host
    VxSleep( 200 );
	// doRelaySearchState();
	// m_RelaySearchThread.abortThreadRun( true );
	// LogModule( eLogNetworkRelay, LOG_INFO, "111 NetworkStateRelaySearch::runNetworkState done\n" );
}

//============================================================================
void NetworkStateRelaySearch::startAnnounceServersListAndSearchThread( void )
{
	m_RelaySearchThread.abortThreadRun( true );
	while( m_RelaySearchThread.isThreadRunning() )
	{
        LogModule( eLogNetworkRelay, LOG_INFO, "m_RelaySearchThread::startup waiting for old connect thread to die\n" );
		VxSleep( 200 );
	}

	if( VxIsAppShuttingDown() )
	{
		return;
	}

	m_RelaySearchThread.startThread( (VX_THREAD_FUNCTION_T)NetworkStateRelaySearchThreadFunction, this, "RelaySearchThread" ); 
}

//============================================================================
void NetworkStateRelaySearch::doRelaySearchState( void )
{
    // BRJ for now do not do relay search.. later on will design a way to attempt relay search if we connect to a group host
    // group host will provide relay service
    return;
    if( eNetAvailOnlineButNoRelay != m_Engine.getNetStatusAccum().getNetAvailStatus() )
    {
        return;
    }

	m_RelaySearchMutex.lock();
	m_BusyRelayList.clear();
	m_RelaySearchMutex.unlock();

	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	if( checkHaveRelayService() )
	{
		moveToAnnounceState();
		return;
	}

	// first we try what is in pkt announce
	if( m_PktAnn.m_RelayConnectId.isVxGUIDValid() )
	{
		BigListInfo * poInfoTry = m_Engine.getBigListMgr().findBigListInfo( m_PktAnn.m_RelayConnectId.getOnlineId() );
		if( poInfoTry )
		{
			ConnectRequest connectRequest( poInfoTry->getConnectInfo() );
			if( connectRelayService( connectRequest ) )
			{
				// we are connected
				return;
			}
		}
	}

	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	// we may have been connected to by friends before announce.. see if any can be used as relay
	doCurrentConnectionsList();
	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	if( checkHaveRelayService() )
	{
		moveToAnnounceState();
		return;
	}

	static int64_t timeLastGetList = 0;
	// last resort.. just get list without announcing. this is because of catch 22 in that we don't announce until online
	// and we cannot find relays until online unless they just happen to be a in our database and at last known ip and not
	// behind a firewall and not busy with too many relay connections
	int64_t timeNow = GetGmtTimeMs();
	int64_t timeStartSearch = timeNow;
	if( 120 > ( timeNow - timeLastGetList ) )
	{
        LogModule( eLogNetworkRelay, LOG_INFO, "Waiting 120 seconds before getting relays from anchor\n" );
		m_SearchThreadComplete = true;
	}
	else
	{
		m_SearchThreadComplete = false;
		startAnnounceServersListAndSearchThread();
	}

    LogModule( eLogNetworkRelay, LOG_STATUS, "eMyRelayStatusSearching\n" );
	//m_Engine.getToGui().toGuiMyRelayStatus( eMyRelayStatusSearching );

	if( 0 == m_PreferredRelayList.m_ContactList.size() )
	{
        LogModule( eLogNetworkRelay, LOG_STATUS, "eMyRelayStatusNoRelaysListed 0 == m_PreferredRelayList.m_ContactList.size()\n" );
		//m_Engine.getToGui().toGuiMyRelayStatus( eMyRelayStatusNoRelaysListed );
	}

	while( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		bool searchComplete = m_SearchThreadComplete;
		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		doPreferedRelayList();

		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		doPossibleRelayList();

		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		doBusyList();

		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		VxSleep( 1000 );
		//m_Engine.getToGui().toGuiStatusMessage( "No Relays Available - will try again " );
		VxSleep( 1000 );
		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		VxSleep( 2000 );
		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( checkHaveRelayService() )
		{
			moveToAnnounceState();
			return;
		}

		timeNow = GetGmtTimeMs();
		if( searchComplete || ( ( timeNow - timeStartSearch ) > 120000 ) )
		{
            if( ( timeNow - timeStartSearch ) < 30000 )
            {
                VxSleep( (int)( 30000 - ( timeNow - timeStartSearch  ) ) );
            }

			//BRJ.. we queried anchors and tried all.. just start over
            LogModule( eLogNetworkRelay, LOG_INFO, "NetworkStateRelaySearch Search Timeout.. resetting network\n" );
			// m_NetworkStateMachine.restartNetwork();
			return;
		}
	}
}

//============================================================================
void NetworkStateRelaySearch::moveToAnnounceState( void )
{
	// try one more time to see if port is open
    EFirewallTestType firewallTestType = m_Engine.getEngineSettings().getFirewallTestSetting();
	if( eFirewallTestUrlConnectionTest == firewallTestType )
	{
        LogModule( eLogNetworkRelay, LOG_INFO, "111 NetworkStateRelaySearch Starting Direct connect Test\n" );
		DirectConnectTestResults& directConnectTestResults = m_DirectConnectTester.getDirectConnectTestResults();
		m_DirectConnectTester.testCanDirectConnect();
		while( ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
			&& ( false == m_NetworkStateMachine.getDirectConnectTester().isDirectConnectTestComplete() ) )
		{
			VxSleep( 250 );
		}

		if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		{
			return;
		}

		if( directConnectTestResults.getCanDirectConnect() )
		{
            LogModule( eLogNetworkRelay, LOG_INFO, "NetworkStateRelaySearch Can Direct Connect.. switching to announce\n" );
			m_ConnectionList.cancelRelayService();
			m_PktAnn.setRequiresRelay( false );
			m_PktAnn.setHasRelay( false );			
		}
		else
		{
            LogModule( eLogNetworkRelay, LOG_INFO, "NetworkStateRelaySearch Cannot Direct Connect.. announcing with relay\n" );
		}
	}

	m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeAnnounce );
}

//============================================================================
void NetworkStateRelaySearch::doCurrentConnectionsList( void )
{
	unsigned int connectIdx = 0;
	P2PConnectList& p2pConnectedList = m_Engine.getConnectList();
#ifdef TARGET_OS_ANDROID
	std::map<VxGUID, RcConnectInfo *>& connectedList				= p2pConnectedList.getConnectedList();
#else
	std::map<VxGUID, RcConnectInfo *, cmp_vxguid>& connectedList	= p2pConnectedList.getConnectedList();
#endif
	P2PConnectList::ConnectListIter connectedIter;

	while( connectIdx < connectedList.size() )
	{
		p2pConnectedList.connectListLock();
		if( connectIdx < connectedList.size() )
		{
			//sigh can't directly idx so have to iterate to correct index
			RcConnectInfo *	rcInfo = NULL;
            unsigned int listIdx = 0;
			for( connectedIter = connectedList.begin(); connectedIter != connectedList.end(); ++connectedIter )
			{
				if( listIdx == connectIdx )
				{
					rcInfo = connectedIter->second;
					break;
				}

				listIdx++;
			}

			connectIdx++;
			if( rcInfo )
			{
				BigListInfo * bigInfo = rcInfo->getBigListInfo();
				if( bigInfo && ( false == bigInfo->requiresRelay() ) )
				{
					VxSktBase * sktBase = rcInfo->getSkt();
					p2pConnectedList.connectListUnlock(); // need to unlock so response to relay service can be handled
					ERelayStatus relayStatus = requestRelayService( bigInfo->getMyOnlineId(), sktBase );
					if( eRelayStatusOk ==  relayStatus )
					{
                        LogModule( eLogNetworkRelay, LOG_ERROR, "NetworkStateRelaySearch::connectRelayService: id %s Relay Service Success\n", bigInfo->getMyOnlineId().toHexString().c_str());
						return;
					}

					continue;
				}
			}
		}
		else
		{
			p2pConnectedList.connectListUnlock();
			break;
		}

		p2pConnectedList.connectListUnlock();
	}
}

//============================================================================
void NetworkStateRelaySearch::doPreferedRelayList( void )
{
	attemptRelayServiceInContactList( m_PreferredRelayList );
}

//============================================================================
void NetworkStateRelaySearch::doPossibleRelayList( void )
{
	attemptRelayServiceInContactList( m_PossibleRelayList );
}

//============================================================================
bool NetworkStateRelaySearch::checkFromGuiList( void )
{
	bool hasContacts = false;
	bool haveRelayService = false;

	while( m_FromGuiRelayList.size() 
		&& ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() ) )
	{

		m_RelaySearchMutex.lock();
		if( 0 == m_FromGuiRelayList.size() )
		{
			// all done
			m_RelaySearchMutex.unlock();
			return hasContacts;
		}

		hasContacts = true;

		removeContactInfo( m_FromGuiRelayList[0].getConnectInfo() );
		m_PreferredRelayList.addContactInfo( m_FromGuiRelayList[0].getConnectInfo() );
		if( false == haveRelayService )
		{
			haveRelayService = connectRelayService( m_FromGuiRelayList[0] );
		}

		m_FromGuiRelayList.erase( m_FromGuiRelayList.begin() );
		m_RelaySearchMutex.unlock();
	}

	return hasContacts;
}

//============================================================================
void NetworkStateRelaySearch::attemptRelayServiceInContactList( ContactList& contactList )
{
	unsigned int uiListIdx = 0;
	ConnectRequest connectInfo;
	while( ( uiListIdx < contactList.m_ContactList.size() )  
		&& ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		&& ( false == checkHaveRelayService() ) )
	{
		if( checkFromGuiList() )
		{
			continue;
		}

		contactList.m_ContactListMutex.lock();
		if( uiListIdx <= contactList.m_ContactList.size() )
		{
			// all done
			contactList.m_ContactListMutex.unlock();
			return;
		}

		if( false == contactList.m_ContactList[uiListIdx].isTooSoonToAttemptConnectAgain() )
		{
			connectInfo = contactList.m_ContactList[uiListIdx];
			contactList.m_ContactListMutex.unlock();
			if( connectRelayService( connectInfo ) )
			{
				return;
			}
		}
		else
		{
			contactList.m_ContactListMutex.unlock();
		}

		uiListIdx++;
	}
}

//============================================================================
void NetworkStateRelaySearch::doBusyList( void )
{
	unsigned int uiListIdx = 0;
	ConnectRequest connectInfo;
	while( ( uiListIdx < m_BusyRelayList.size() ) 
		&& ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() ) 
		&& ( false == checkHaveRelayService() ) )
	{
		if( checkFromGuiList() )
		{
			continue;
		}

		m_RelaySearchMutex.lock();
		if( uiListIdx <= m_BusyRelayList.size() )
		{
			// all done
			m_RelaySearchMutex.unlock();
			return;
		}

		if( false == m_BusyRelayList[uiListIdx]->isTooSoonToAttemptConnectAgain() )
		{
			connectInfo = *m_BusyRelayList[uiListIdx];
			m_RelaySearchMutex.unlock();
			if( connectRelayService( connectInfo ) )
			{
				return;
			}
		}
		else
		{
			m_RelaySearchMutex.unlock();
		}
		uiListIdx++;
	}
}

//============================================================================
void NetworkStateRelaySearch::doRelayAnnounceListAndSearch( void )
{
	getMoreRelaysFromAnnounceServers();
}

//============================================================================
void NetworkStateRelaySearch::getMoreRelaysFromAnnounceServers( void )
{
    LogModule( eLogNetworkRelay, LOG_STATUS, "NetworkStateRelaySearch::getMoreRelaysFromAnnounceServers\n" );
	if( m_Engine.getHasHostService( eHostServiceNetworkHost ) )
	{
		// we are the anchor
		// put ourself directly into the database
		HostList			anchorListIn;
		anchorListIn.addEntry( &m_Engine.getMyPktAnnounce() );
		anchorListIn.m_HostAction = eHostActionRelaysOnly;
		HostList			anchorListOut;

		m_NetServicesMgr.getNetServiceHost().getHostDb().handleAnnounce( anchorListIn, anchorListOut );
		m_NetServicesMgr.netActionResultAnnounce( eAppErrNone, &anchorListOut );
		return;
	}

	VxSktConnectSimple netServConn;
	if( false == m_NetServicesMgr.actionReqConnectToHost( netServConn ) )
	{
        LogModule( eLogNetworkRelay, LOG_ERROR, "getMoreRelaysFromAnnounceServers: Could not connect to Host\n" );
		return;
	}

	std::string strNetCmdHdr;
	HostList anchorList;
	uint16_t acceptedPort = netServConn.getRemotePort();
	buildAnnounceCmd( strNetCmdHdr, acceptedPort, anchorList, eHostActionRelaysOnly );
	int anchorDataLen = anchorList.m_TotalLen;

	RCODE rc = netServConn.sendData( strNetCmdHdr.c_str(), (int)strNetCmdHdr.length() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "getMoreRelaysFromAnnounceServers: send header error %d %s\n", rc, VxDescribeSktError( rc ) );
		netServConn.closeSkt();
		return;
	}

	VxKey cryptKey;
    m_NetServiceUtils.generateNetServiceCryptoKey( cryptKey, acceptedPort, m_NetServiceUtils.getNetworkKey() );
	VxSymEncrypt( &cryptKey, (char *)&anchorList, anchorDataLen );

	rc = netServConn.sendData( (char *)&anchorList, anchorDataLen );
	if( rc )
	{
		netServConn.closeSkt();
		LogMsg( LOG_ERROR, "getMoreRelaysFromAnnounceServers: send anchor list error %d %s\n", rc, VxDescribeSktError( rc ) );
		return;
	}

	NetServiceHdr netServiceHdr;
	char rxBuf[ (sizeof( HostList ) + 1024) + 1 ];
	if( false == m_NetServiceUtils.rxNetServiceCmd( &netServConn, 
													rxBuf, 
													sizeof( rxBuf ) - 1, 
													netServiceHdr,
													ANCHOR_RX_HDR_TIMEOUT,
													ANCHOR_RX_DATA_TIMEOUT ) )
	{
		netServConn.closeSkt();
		LogMsg( LOG_ERROR, "getMoreRelaysFromAnnounceServers: receive cmd error \n" );
		return;
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	if( false == decryptHostList( rxBuf, netServiceHdr.m_ContentDataLen, acceptedPort ) )
	{
		netServConn.closeSkt();
		LogMsg( LOG_ERROR, "getMoreRelaysFromAnnounceServers: decrypt error \n" );
		return;
	}

	netServConn.closeSkt();
	m_NetServicesMgr.netActionResultAnnounce( eAppErrNone, (HostList *)rxBuf, eHostActionRelaysOnly );
}

//============================================================================
bool NetworkStateRelaySearch::decryptHostList( char * content, int contentDataLen, uint16_t clientPort )
{
	if( false == m_NetServiceUtils.decryptNetServiceContent( content, contentDataLen, clientPort ) )
	{
		return 0;
	}

	HostList * anchorList = (HostList *)content;
	if( anchorList->m_TotalLen != contentDataLen )
	{
		LogMsg( LOG_ERROR, "getMoreRelaysFromAnnounceServers:: invalid anchorList->m_TotalLen\n" );
		return false;
	}

	if( MAX_ANCHOR_ENTRIES <= anchorList->m_EntryCount )
	{
		LogMsg( LOG_ERROR, "getMoreRelaysFromAnnounceServers: invalid anchorList->m_EntryCount\n" );
		return false;
	}

	return true;
}

//============================================================================
int NetworkStateRelaySearch::buildAnnounceCmd( std::string& strNetCmdHdr, uint16_t clientPort, HostList& anchorList, EHostAction anchorAction )
{
	// build Host list
	PktAnnounce& pktAnn = m_Engine.getMyPktAnnounce();

	anchorList.m_HostAction = anchorAction;
	anchorList.addEntry( &pktAnn );
	int listLen = anchorList.calculateLength();

	std::string netServChallengeHash;
    m_NetServiceUtils.generateNetServiceChallengeHash( netServChallengeHash, clientPort, m_NetServiceUtils.getNetworkKey() );

	int totalLen = m_NetServiceUtils.buildNetCmdHeader( strNetCmdHdr, eNetCmdHostReq, netServChallengeHash, listLen );
	return totalLen;
}

//============================================================================
int NetworkStateRelaySearch::getAnnounceData( VxSktConnectSimple * netServConn, char * pRxBuf, int bufLen )
{
	int iMaxToReceive = bufLen - 8;
	int iTotalRxed = 0;
	for( int i = 0; i < 10; ++i )
	{
		int iRxed = 0;
		netServConn->recieveData(	&pRxBuf[ iTotalRxed ],		// data buffer to read into
									iMaxToReceive,				// length of data	
									&iRxed,						// number of bytes actually received
									ANCHOR_RX_DATA_TIMEOUT );	// timeout attempt to receive
		if( iRxed <= 0 )
		{
			break;
		}
		pRxBuf[ iTotalRxed + iRxed ] = 0; 
		if( strstr( &pRxBuf[ iTotalRxed ], "Footer" ) )
		{
			iTotalRxed += iRxed;
			break;
		}
		iTotalRxed += iRxed;
		iMaxToReceive -= iRxed;
	}

	pRxBuf[ iTotalRxed ] = 0; 
	return iTotalRxed;
}

//============================================================================
void NetworkStateRelaySearch::getMoreRelaysToPick( void )
{
	//TODO Do list relays from announce then search
	VxSleep( 300 );
}


//============================================================================
bool NetworkStateRelaySearch::handlePossibleRelayConnect(	VxConnectInfo&		connectInfo, 
															VxSktBase *			sktBase,
															bool				isNewConnection,
															EConnectReason		connectReason )
{
	bool serviceConnected = false;
	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	if( eNetworkStateTypeRelaySearch == m_NetworkStateMachine.getCurNetworkStateType() )
	{
		std::string onlineHexStr;
		connectInfo.getMyOnlineId().toHexString( onlineHexStr );
		ConnectRequest connectRequest( connectInfo, eConnectReasonRelaySearch );

		ERelayStatus relayStatus = requestRelayService( connectInfo.getMyOnlineId(), sktBase );
		if( eRelayStatusBusy == relayStatus )
		{
			LogMsg( LOG_ERROR, "NetworkStateRelaySearch::connectRelayService: id %s busy\n", onlineHexStr.c_str());
			addToBusyList( connectRequest );
		}
		else if( eRelayStatusOk ==  relayStatus )
		{
			LogMsg( LOG_ERROR, "NetworkStateRelaySearch::connectRelayService: id %s Relay Service Success\n", onlineHexStr.c_str());
			if( eNetworkStateTypeRelaySearch == m_NetworkStateMachine.getCurNetworkStateType() )
			{
				moveToAnnounceState();
			}

			serviceConnected = true;
		}
		else if( eRelayStatusPermissionErr ==  relayStatus )
		{
			LogMsg( LOG_ERROR, "NetworkStateRelaySearch::connectRelayService: id %s Permission Error\n", onlineHexStr.c_str());
			removeContactInfo( connectRequest.getConnectInfo() );
			//m_Engine.getNetConnector().closeIfAnnonymous( connectInfo.getMyOnlineId(), sktBase );
		}
		else
		{
			LogMsg( LOG_ERROR, "NetworkStateRelaySearch::connectRelayService: id %s Unknown Error %d\n", onlineHexStr.c_str(), m_eRelayRequestStatus );
			removeContactInfo( connectRequest.getConnectInfo() );
			//m_Engine.getNetConnector().closeIfAnnonymous( connectInfo.getMyOnlineId(), sktBase );
		}
	}

	return serviceConnected;
}

//============================================================================
bool NetworkStateRelaySearch::connectRelayService( ConnectRequest& connectRequest )
{
	connectRequest.setTimeLastConnectAttemptMs( GetGmtTimeMs() );
	VxConnectInfo& connectInfo = connectRequest.getConnectInfo();
	std::string onlineId;
	connectInfo.getMyOnlineId().toHexString( onlineId );
	VxSktBase * sktBase = NULL;
	if( connectInfo.requiresRelay() )
	{
		LogMsg( LOG_ERROR, "NetworkStateRelaySearch::connectRelayService: id %s requires relay", onlineId.c_str());
		return false;
	}

	bool relayServiceConnected		= false;
	bool isNewConnection			= false;
	if( true == m_Engine.getNetConnector().connectToContact( connectInfo, &sktBase, isNewConnection, connectRequest.getConnectReason() ) )
	{
		relayServiceConnected = handlePossibleRelayConnect(	connectInfo, 
															sktBase,
															isNewConnection,
															connectRequest.getConnectReason() );
		ERelayStatus relayStatus = requestRelayService( connectInfo.getMyOnlineId(), sktBase );
        if( eRelayStatusOk != relayStatus )
        {
            LogMsg( LOG_INFO, "NetworkStateRelaySearch::connectRelayService: id %s connected but status %d\n", onlineId.c_str(), relayStatus );
        }
	}
	else
	{
		LogMsg( LOG_INFO, "NetworkStateRelaySearch::connectRelayService: id %s failed connect\n", onlineId.c_str() );
	}

	return relayServiceConnected;
}

//============================================================================
ERelayStatus NetworkStateRelaySearch::requestRelayService( VxGUID& onlineId, VxSktBase * sktBase )
{
	if( checkHaveRelayService() )
	{
		return eRelayStatusOk;
	}

	m_RelayRequestOnlineId	= onlineId;
	m_eRelayRequestStatus	= eRelayStatusUnknown;
	PktRelayServiceReq pkt;
	m_Engine.txSystemPkt( onlineId, sktBase, &pkt );
	std::string strRelayOnlineName;
	BigListInfo * bigListInfo = m_Engine.getBigListMgr().findBigListInfo( onlineId );
	if( 0 != bigListInfo )
	{
		strRelayOnlineName = bigListInfo->getOnlineName();
	}
	else
	{
		onlineId.toHexString( strRelayOnlineName );
	}

    LogModule( eLogNetworkRelay, LOG_STATUS, " Requesting Relay Service ( %s )\n", strRelayOnlineName.c_str()  );
	//m_Engine.getToGui().toGuiMyRelayStatus( eMyRelayStatusRequestService, strRelayOnlineName.c_str() );
	m_WaitForPktRelayServiceSemaphore.wait( RELAY_SERVICE_REQUEST_TIMEOUT_MS );
	return m_eRelayRequestStatus;
}

//============================================================================
void NetworkStateRelaySearch::removeContactInfo( VxConnectInfo& connectInfo )
{
	removeFromBusyList( connectInfo );
	m_PreferredRelayList.removeContactInfo( connectInfo );
	m_PossibleRelayList.removeContactInfo( connectInfo );
}

//============================================================================
void NetworkStateRelaySearch::addToBusyList( ConnectRequest& connectInfo )
{
	m_RelaySearchMutex.lock();
	std::vector<ConnectRequest*>::iterator iter;
	for( iter = m_BusyRelayList.begin(); iter != m_BusyRelayList.end(); ++iter )
	{
		if( (*iter)->getMyOnlineId() == connectInfo.getMyOnlineId() )
		{
			m_RelaySearchMutex.unlock();
			return;
		}
	}

	m_BusyRelayList.push_back( &connectInfo );
	m_RelaySearchMutex.unlock();
}

//============================================================================
void NetworkStateRelaySearch::removeFromBusyList( VxConnectInfo& connectInfo )
{
	m_RelaySearchMutex.lock();
	std::vector<ConnectRequest*>::iterator iter;
	for( iter = m_BusyRelayList.begin(); iter != m_BusyRelayList.end(); ++iter )
	{
		if( (*iter)->getMyOnlineId() == connectInfo.getMyOnlineId() )
		{
			m_BusyRelayList.erase(iter);
			break;
		}
	}

	m_RelaySearchMutex.unlock();
}

//============================================================================
bool NetworkStateRelaySearch::checkHaveRelayService( void )
{
	//if( ( NULL == m_ConnectionList.m_RelayServiceConnection )
	//	|| ( false == m_ConnectionList.m_RelayServiceConnection->getSkt()->isConnected() )
	//	|| ( false == m_ConnectionList.m_RelayServiceConnection->isRelayServer() ) )
	//{
	//	return false;
	//}

	//return true;

	return ( NULL != m_ConnectionList.m_RelayServiceConnection ); // thread safe only check
}

//============================================================================
void NetworkStateRelaySearch::onPktRelayServiceReply( VxSktBase * sktBase, PktRelayServiceReply * poPkt )
{
	if( checkHaveRelayService()  )
	{
		return;
	}

	if( m_RelayRequestOnlineId != poPkt->getSrcOnlineId() )
	{
		// not the right id
#ifdef DEBUG_RELAY
		LogMsg( LOG_ERROR, "onPktRelayServiceReply wrong id got %s expected %s\n", 
			poPkt->getSrcOnlineId().toHexString().c_str(), m_RelayRequestOnlineId.toHexString().c_str() );
#endif // DEBUG_RELAY
		return;
	}

	ERelayStatus relayStatus = poPkt->getRelayStatus();

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "NetworkStateRelaySearch::onPktRelayServiceReply: m_ConnectListMutex.lock()\n" );
#endif // DEBUG_MUTEXES
	m_ConnectionList.connectListLock();
	m_RelayRequestOnlineId = poPkt->getSrcOnlineId();
	RcConnectInfo * poInfo = m_ConnectionList.findConnection( m_RelayRequestOnlineId );
	if( poInfo )
	{
		if( eRelayStatusOk == relayStatus )
		{
#ifdef DEBUG_RELAY
			LogMsg( LOG_ERROR, "onPktRelayServiceReply %s SUCCESS startNewRelayClient\n", 
				poPkt->getSrcOnlineId().toHexString().c_str() );
#endif // DEBUG_RELAY
			poInfo->setIsRelayServer(true);
			m_Engine.onRelayServiceAvailable( poInfo, true );
			m_ConnectionList.m_RelayServiceConnection = poInfo;
		}
		else
		{
			poInfo->setIsRelayServer(false);
			if( NULL != m_ConnectionList.m_RelayServiceConnection )
			{
				if( ( false == m_ConnectionList.m_RelayServiceConnection->getSkt()->isConnected() )
					|| ( false == m_ConnectionList.m_RelayServiceConnection->isRelayServer() ) )
				{
#ifdef DEBUG_RELAY
					LogMsg( LOG_ERROR, "onPktRelayServiceReply removing dead relay\n" );
#endif // DEBUG_RELAY
					m_ConnectionList.m_RelayServiceConnection = NULL;
				}
			}
		}
	}

#ifdef DEBUG_RELAY
	else
	{
		LogMsg( LOG_ERROR, "onPktRelayServiceReply %s Connection NOT FOUND\n", poPkt->getSrcOnlineId().toHexString().c_str() );
	}
#endif // DEBUG_RELAY

#ifdef DEBUG_MUTEXES
	LogMsg( LOG_INFO, "P2PEngine::onPktRelayServiceReply: m_ConnectListMutex.unlock()\n" );
#endif // DEBUG_MUTEXES
	m_ConnectionList.connectListUnlock();
	m_eRelayRequestStatus = poPkt->getRelayStatus();
	m_WaitForPktRelayServiceSemaphore.signal();
}


