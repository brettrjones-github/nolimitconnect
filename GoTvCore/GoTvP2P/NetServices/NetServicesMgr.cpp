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

#include "NetServicesMgr.h"
#include "NetServiceHdr.h"
#include "NetActionQueryMyIp.h"
#include "NetActionIsMyPortOpen.h"
#include "NetActionAnnounce.h"
#include "NetActionRandomConnect.h"
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>
#include <GoTvCore/GoTvP2P/Network/NetConnector.h>

#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettingsDefaultValues.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/P2PEngine/EngineSettings.h>

#include <NetLib/VxSktBase.h>
#include <NetLib/VxSktUtil.h>

#include <CoreLib/OsDetect.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxTime.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

namespace
{
	//============================================================================
    void * NetServicesMgrThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );

		NetServicesMgr * poMgr = (NetServicesMgr *)poThread->getThreadUserParam();
        if( poMgr && false == poThread->isAborted() )
        {
            poMgr->runNetActions();
        }

		poThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
NetServicesMgr::NetServicesMgr( P2PEngine& engine )
: m_Engine( engine )
, m_PktAnn( engine.getMyPktAnnounce() )
, m_EngineSettings( engine.getEngineSettings() )
, m_NetworkMgr( engine.getNetworkMgr() )
, m_NetServiceUtils( engine )
, m_NetServiceHost( engine, *this, m_NetServiceUtils )
, m_NetActionIdle( *this )
, m_CurNetAction( &m_NetActionIdle )
{
}

//============================================================================
NetServicesMgr::~NetServicesMgr()
{
	netServicesShutdown();
}

//============================================================================
std::string NetServicesMgr::getNetworkKey( void )
{
    return m_NetworkMgr.getNetworkKey();
}

//============================================================================
VxGUID& NetServicesMgr::getMyOnlineId( void )
{
	return m_PktAnn.getMyOnlineId();
}

//============================================================================
void NetServicesMgr::netServicesStartup( void )
{
	m_NetServiceHost.netServiceHostStartup();
	m_NetActionThread.startThread( (VX_THREAD_FUNCTION_T)NetServicesMgrThreadFunc, this, "NetServMgrThrd" );
}

//============================================================================
void NetServicesMgr::netServicesShutdown( void )
{
	m_NetActionThread.abortThreadRun( true );
	m_NetActionMutex.lock();
	if( m_CurNetAction )
	{
		LogMsg( LOG_INFO, "NetServicesMgr::netServicesShutdown: action type %d thread 0x%x", m_CurNetAction->getNetActionType(), VxGetCurrentThreadId() );
	}

	m_NetActionMutex.unlock();
	m_NetActionSemaphore.signal();
	m_NetServiceHost.netServiceHostShutdown();
	m_NetActionThread.killThread();
}

//============================================================================
void NetServicesMgr::addNetActionCommand( NetActionBase * netActionBase )
{
	m_NetActionMutex.lock();
	m_NetActionList.push_back( netActionBase );
	m_NetActionMutex.unlock();
	m_NetActionSemaphore.signal();
}

//============================================================================
bool NetServicesMgr::isActionQued( ENetActionType eNetActionType )
{
	if( m_NetActionList.size() )
	{
		m_NetActionMutex.lock();
		std::vector<NetActionBase*>::iterator iter;
		for( iter = m_NetActionList.begin(); iter != m_NetActionList.end(); ++iter )
		{
			if( eNetActionType == (*iter)->getNetActionType() )
			{
				m_NetActionMutex.unlock();
				return true;
			}
		}

		m_NetActionMutex.unlock();
	}

	return false;
}

//============================================================================
void NetServicesMgr::runNetActions( void )
{
	while( ( false == m_NetActionThread.isAborted() )
			&& ( false == VxIsAppShuttingDown() ) )
	{
		m_NetActionSemaphore.wait();
		if( m_NetActionThread.isAborted()
			|| VxIsAppShuttingDown() )
		{
			return;
		}

		while( m_NetActionList.size() )
		{

			m_NetActionMutex.lock();
			m_CurNetAction = m_NetActionList[0];
			m_NetActionList.erase( m_NetActionList.begin() );
			m_NetActionMutex.unlock();

			if( m_NetActionThread.isAborted()
				|| VxIsAppShuttingDown() )
			{
				return;
			}

			m_CurNetAction->enterAction();
			m_CurNetAction->doAction();
			m_CurNetAction->exitAction();

			m_NetActionMutex.lock();
			delete m_CurNetAction;
			if( m_NetActionThread.isAborted()
				|| VxIsAppShuttingDown() )
			{
				m_CurNetAction = NULL;
				m_NetActionMutex.unlock();
				return;
			}

			m_CurNetAction = &m_NetActionIdle;
			m_NetActionMutex.unlock();

			if( m_NetActionThread.isAborted()
				|| VxIsAppShuttingDown() )
			{
				return;
			}

			// we keep the connection open until command que is empty
			if( 0 == m_NetActionList.size() )
			{
				if( m_SktToNetServices.isConnected() )
				{
					m_SktToNetServices.closeSkt(43);
				}
			}
		}
	}
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdPing( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	return  sendPong( sktBase, netServiceHdr );
}

//============================================================================
RCODE NetServicesMgr::sendPong( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
    LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "Got ping from %s thread 0x%x", sktBase->getRemoteIp(), VxGetCurrentThreadId() );
	std::string content;
	StdStringFormat( content, "PONG-%s", sktBase->getRemoteIp() );
	return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdPong, content );
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdPong( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
    LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "Got PONG from %s thread 0x%x", sktBase->getRemoteIp(), VxGetCurrentThreadId() );
	return 0;
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdIsMyPortOpenReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	InetAddrAndPort rmtAddr;
    VxGetRmtAddress( sktBase->m_Socket, rmtAddr );
	std::string strRmtAddr = rmtAddr.toStdString().c_str();

	std::string toClientContent = "0-"; // assume fail
	toClientContent += strRmtAddr;

	if( strRmtAddr.empty() )
	{
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "NetServicesMgr::handleNetCmdIsMyPortOpenReq: could not determine remote address\n");
		return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdIsMyPortOpenReply, toClientContent, (int)eAppErrFailedToResolveAddr );
	}

	char * pSktBuf = (char *)sktBase->getSktReadBuf();
	if( false == ( '/' == pSktBuf[ netServiceHdr.m_TotalDataLen - 1 ] ) )
	{
		sktBase->sktBufAmountRead( 0 );
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "NetServicesMgr::handleNetCmdIsMyPortOpenReq: invalid content\n");
		return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdIsMyPortOpenReply, toClientContent, (int)eAppErrFailedToResolveAddr );
	}

	sktBase->sktBufAmountRead( 0 );

	pSktBuf[ netServiceHdr.m_TotalDataLen - 1 ] = 0;

	std::string fromClientContent = &pSktBuf[ netServiceHdr.m_SktDataUsed ];
	
	if( fromClientContent.empty() )
	{
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "NetServicesMgr::handleNetCmdIsMyPortOpenReq: could not parse from client content\n");
		return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdIsMyPortOpenReply, toClientContent, (int)eAppErrParseError );
	}

	uint16_t u16Port = (uint16_t)atoi( fromClientContent.c_str() );
	if( 0 == u16Port )
	{
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "NetServicesMgr::handleNetCmdIsMyPortOpenReq: could not parse client port\n" );
		return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdIsMyPortOpenReply, toClientContent, (int)eAppErrParseError );
	}

    LogModule( eLogIsPortOpenTest, LOG_ERROR, "handleNetCmdIsMyPortOpenReq: Attempting ping ip %s port %d\n", strRmtAddr.c_str(), u16Port );
	std::string retPong;
	bool pingSuccess = doNetCmdPing( strRmtAddr.c_str(), u16Port, retPong );
	if( false == pingSuccess )
	{
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "handleNetCmdIsMyPortOpenReq: FAILED PING ip %s port %d\n", strRmtAddr.c_str(), u16Port );
		return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdIsMyPortOpenReply, toClientContent, (int)eAppErrPortIsClosed );
	}

    LogModule( eLogIsPortOpenTest, LOG_ERROR, "handleNetCmdIsMyPortOpenReq: SUCCESS PING ip %s port %d\n", strRmtAddr.c_str(), u16Port );
	toClientContent = "1-";
	toClientContent += strRmtAddr;
	return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdIsMyPortOpenReply, toClientContent, (int)eAppErrNone );
}

//============================================================================
bool NetServicesMgr::doNetCmdPing( const char * ipAddress, uint16_t u16Port,	std::string& retPong )
{
	VxSktConnectSimple toClientConn;
	VxTimer	pingTimer;

	if( INVALID_SOCKET == toClientConn.connectTo( ipAddress, u16Port, PORT_TEST_CONNECT_TO_CLIENT_TIMEOUT ) )
	{
		LogMsg( LOG_ERROR, "##P NetServicesMgr::doNetCmdPing: timeout %d could not connect to %s:%d %3.3f sec thread 0x%x", PORT_TEST_CONNECT_TO_CLIENT_TIMEOUT, ipAddress, u16Port, pingTimer.elapsedSec(), VxGetCurrentThreadId() );
		return false;
	}
	
	double connectTime = pingTimer.elapsedSec();
	LogMsg( LOG_INFO, "##P NetServicesMgr::doNetCmdPing connect to %s:%d in %3.3f sec thread 0x%x", ipAddress, u16Port, connectTime, VxGetCurrentThreadId() );
	return sendAndRecievePing( pingTimer, toClientConn, retPong, PONG_RX_TIMEOUT );
}

//============================================================================
bool NetServicesMgr::sendAndRecievePing( VxTimer& pingTimer, VxSktConnectSimple& toClientConn, std::string& retPong, int receiveTimeout )
{	
	std::string strNetCmd;
	std::string strPing = "PING";
	
	std::string netServChallengeHash;
	uint16_t cryptoKeyPort = toClientConn.getCryptoKeyPort();
    m_NetServiceUtils.generateNetServiceChallengeHash(netServChallengeHash, cryptoKeyPort, getNetworkKey() );

	m_NetServiceUtils.buildNetCmd( strNetCmd, eNetCmdPing, netServChallengeHash, strPing );

    LogModule( eLogIsPortOpenTest, LOG_ERROR, "##P NetServicesMgr::sendAndRecievePing: cypto port %d strNetCmd(%s) thread 0x%x", cryptoKeyPort, strNetCmd.c_str(), VxGetCurrentThreadId() );

    // startSendTime is also the time it took to connect
	double startSendTime = pingTimer.elapsedSec();
	RCODE rc = toClientConn.sendData( strNetCmd.c_str(), ( int )strNetCmd.length(), 8000 );
	if( rc )
	{
		double failSendTime = pingTimer.elapsedSec();
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "##P NetServicesMgr::sendAndRecievePing: sendData error %d in %3.3f sec thread 0x%x", rc, failSendTime - startSendTime, VxGetCurrentThreadId() );
		toClientConn.closeSkt();
		return false;
	}

	double endSendTime = pingTimer.elapsedSec();
	char rxBuf[ 513 ];
	rxBuf[0] = 0;
	NetServiceHdr netServiceHdr;
	if( false == m_NetServiceUtils.rxNetServiceCmd( &toClientConn, rxBuf, sizeof( rxBuf ) - 1, netServiceHdr, receiveTimeout, receiveTimeout )  )
	{
		double failResponseTime = pingTimer.elapsedSec();
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "##P NetServicesMgr::sendAndRecievePing: no response with timeout spec %d and times connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
			receiveTimeout, startSendTime, startSendTime - endSendTime,  failResponseTime - startSendTime, VxGetCurrentThreadId() );
		toClientConn.closeSkt();
		return false;
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	toClientConn.closeSkt();

	double successResponseTime = pingTimer.elapsedSec();

    if( netServiceHdr.m_ContentDataLen <= 0 )
    {
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "## NetServicesMgr::sendAndRecievePing: No Content connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   startSendTime, endSendTime - startSendTime, successResponseTime - startSendTime, VxGetCurrentThreadId() );
        return false;
    }

	if( ( 0 == netServiceHdr.m_TotalDataLen )
		|| ( 511 <= netServiceHdr.m_TotalDataLen )
		|| ( '/' != rxBuf[ netServiceHdr.m_ContentDataLen - 1 ] ) )
	{
        LogModule( eLogIsPortOpenTest, LOG_ERROR, "##P NetServicesMgr::sendAndRecievePing: invalid response  connect %3.3f sec send %3.3f sec fail respond %3.3f sec thread 0x%x",
                   startSendTime, endSendTime - startSendTime,  successResponseTime - startSendTime, VxGetCurrentThreadId() );
		return false;
	}

	LogModule( eLogIsPortOpenTest, LOG_VERBOSE, "##P NetServicesMgr::sendAndRecievePing: SUCCESS  connect %3.3f sec send %3.3f sec response %3.3f sec thread 0x%x",
               startSendTime, endSendTime - startSendTime,  successResponseTime - startSendTime, VxGetCurrentThreadId() );
	rxBuf[ netServiceHdr.m_ContentDataLen - 1 ] = 0; 
	retPong = rxBuf;
	return true;
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdHostReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	return m_NetServiceHost.handleNetCmdHostReq( sktBase, netServiceHdr );
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdHostReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
	return m_NetServiceHost.handleNetCmdHostReply( sktBase, netServiceHdr );
}

//============================================================================
void NetServicesMgr::queryWhatsMyIp( void )
{
	addNetActionCommand( new NetActionQueryMyIp( *this ) );
}

//============================================================================
void NetServicesMgr::netActionResultQueryWhatsMyIp( const char * ipAddress )
{
	LogMsg( LOG_INFO, "NetServicesMgr::netActionResultQueryWhatsMyIp IP %s\n", ipAddress );
}

//============================================================================
void NetServicesMgr::setMyPortOpenResultCallback( MY_PORT_OPEN_CALLBACK_FUNCTION pfuncPortOpenCallbackHandler, void * userData )
{
	m_pfuncPortOpenCallbackHandler = pfuncPortOpenCallbackHandler;
	m_PortOpenCallbackUserData = userData;
}

//============================================================================
void NetServicesMgr::setQueryHostOnlineIdResultCallback( QUERY_HOST_ID_CALLBACK_FUNCTION pfuncQueryHostIdCallbackHandler, void * userData )
{
    m_pfuncQueryHostIdCallbackHandler = pfuncQueryHostIdCallbackHandler;
    m_QueryHostIdCallbackUserData = userData;
}

//============================================================================
void NetServicesMgr::testIsMyPortOpen( void )
{
	addNetActionCommand( new NetActionIsMyPortOpen( *this ) );
}

//============================================================================
void NetServicesMgr::announceToHost( std::string& anchorIp, uint16_t u16HostPort, EHostAction eHostAction )
{
	addNetActionCommand( new NetActionAnnounce( *this, anchorIp, u16HostPort, eHostAction ) );
}

//============================================================================
void NetServicesMgr::performRandomConnect( void )
{
	if( false == isActionQued( eNetActionRandomConnect ) )
	{
		addNetActionCommand( new NetActionRandomConnect( *this, 
			m_Engine.getNetworkStateMachine().getHostIp(),
			m_Engine.getNetworkStateMachine().getHostPort(),
			eHostActionRandomConnect ) );
	}
	else
	{
		LogMsg( LOG_INFO, "Net Action Random Connect already qued\n" );
	}
}

//============================================================================
void NetServicesMgr::netActionResultRandomConnect( EAppErr eAppErr, HostList * anchorList )
{
	if( eAppErrNone == eAppErr )
	{
		m_Engine.getNetConnector().handleRandomConnectResults( anchorList );
	}
	else
	{
		LogMsg( LOG_INFO, "NetServicesMgr::netActionResultRandomConnect ERROR %d\n", eAppErr );
	}
}

//============================================================================
void NetServicesMgr::netActionResultIsMyPortOpen( EAppErr eAppErr, std::string& myExternalIp )
{
    if( eAppErr == eAppErrNone )
    {
        LogModule( eLogNetworkState, LOG_INFO, "NetServicesMgr::netActionResultIsMyPortOpen CAN DIRECT CONNECT extern ip %s\n", myExternalIp.c_str() );
        // tested and can direct connect
        m_Engine.getNetStatusAccum().setDirectConnectTested( true, false, myExternalIp );
    }
    else if( eAppErr == eAppErrPortIsClosed )
    {
        // tested but cannot direct connect
        LogModule( eLogNetworkState, LOG_INFO, "NetServicesMgr::netActionResultIsMyPortOpen REQUIRES RELAY extern ip %s\n", myExternalIp.c_str() );
        m_Engine.getNetStatusAccum().setDirectConnectTested( true, true, myExternalIp );
    }
    else
    {
        // port open test failed with other error
        LogModule( eLogNetworkState, LOG_INFO, "NetServicesMgr::netActionResultIsMyPortOpen err %d extern ip %s\n", eAppErr, myExternalIp.c_str() );
        m_Engine.getNetStatusAccum().setDirectConnectTested( false, false, myExternalIp );
    }

 	if( m_pfuncPortOpenCallbackHandler )
	{
		m_pfuncPortOpenCallbackHandler( m_PortOpenCallbackUserData, eAppErr, myExternalIp );
	}
}

//============================================================================
void NetServicesMgr::netActionResultAnnounce( EAppErr eAppErr, HostList * anchorList, EHostAction eHostAction )
{
	if( eAppErrNone == eAppErr )
	{
		m_Engine.getNetConnector().handleAnnounceResults( anchorList, ( eHostActionRelaysOnly == eHostAction ) ?  eConnectReasonRelaySearch : eConnectReasonAnnouncePing );
	}
}

//============================================================================
VxSktConnectSimple * NetServicesMgr::actionReqConnectToNetService( void )
{
	if( m_SktToNetServices.isConnected() )
	{
		// already connected
		return &m_SktToNetServices;
	}

	bool sktConnected = actionReqConnectToNetService( m_SktToNetServices );
	return sktConnected ? &m_SktToNetServices : 0;
}

//============================================================================
bool NetServicesMgr::actionReqConnectToNetService( VxSktConnectSimple& sktSimple )
{
	sktSimple.closeSkt();
	std::string netSrvUrl;
	if( m_Engine.getNetworkStateMachine().isNetworkWebsitesResolved() )
	{
		StdStringFormat( netSrvUrl, "ptop://%s:%d",  m_Engine.getNetworkStateMachine().getNetServiceIp().c_str(),  m_Engine.getNetworkStateMachine().getNetServicePort() );
	}
	else
	{
		m_EngineSettings.getConnectTestUrl( netSrvUrl );
	}

	std::string strHost;
	std::string strFile;
	uint16_t u16Port;

	if( false == sktSimple.connectToWebsite( netSrvUrl.c_str(), strHost, strFile, u16Port, NETSERVICE_CONNECT_TIMEOUT ) )
	{
#ifdef LOG_PORT_TEST
		LogMsg( LOG_ERROR, "NetServicesMgr::actionReqConnectToNetService: FAILED to Connect to %s\n", netSrvUrl.c_str() );
#endif // LOG_PORT_TEST

		return false;
	}

	//LogMsg( LOG_VERBOSE, "actionReqConnectToNetService: SUCCESS\n" );
	return true;
}

//============================================================================
VxSktConnectSimple * NetServicesMgr::actionReqConnectToHost( void )
{
	if( m_SktToHost.isConnected() )
	{
		// already connected
		return &m_SktToHost;
	}

	if( false == actionReqConnectToHost( m_SktToHost ) )
	{
		return 0;
	}

	return &m_SktToHost;
}

//============================================================================
bool NetServicesMgr::actionReqConnectToHost( VxSktConnectSimple& sktSimple )
{
	std::string anchorUrl;
	if( m_Engine.getNetworkStateMachine().isNetworkWebsitesResolved() )
	{
		StdStringFormat( anchorUrl, "http://%s:%d",  m_Engine.getNetworkStateMachine().getHostIp().c_str(),  m_Engine.getNetworkStateMachine().getHostPort() );
	}
	else
	{
		m_EngineSettings.getNetworkHostUrl( anchorUrl );
	}

	std::string strHost;
	std::string strFile;
	uint16_t u16Port;

	if( false == sktSimple.connectToWebsite( anchorUrl.c_str(), strHost, strFile, u16Port, ANCHOR_CONNECT_TIMEOUT ) )
	{
        if( IsLogEnabled( eLogNetworkState ) )
        {
            LogMsg( LOG_ERROR, "### ERROR NetServicesMgr::actionReqConnectToHost: FAILED to Connect to %s timeout %d\n", anchorUrl.c_str(), ANCHOR_CONNECT_TIMEOUT );
        }

		return false;
	}

	//LogMsg( LOG_INFO, "actionReqConnectToHost: SUCCESS\n" );
	return true;
}

//============================================================================
EAppErr NetServicesMgr::doIsMyPortOpen( std::string& retMyExternalIp, bool testLoopbackFirst )
{
    retMyExternalIp = "";
	std::string lclIP = m_NetworkMgr.getLocalIpAddress();
    if( lclIP.empty() )
    {
        if( IsLogEnabled( eLogNetworkState ) )
        {
            LogMsg( LOG_ERROR, "NetServicesMgr::doIsMyPortOpen no local ip\n" );
        }

        return eAppErrBadParameter;
    }

	uint16_t tcpListenPort = m_Engine.getMyPktAnnounce().getOnlinePort();

#if 0 //#ifdef TARGET_OS_WINDOWS
    // attempt open port through firewall
static int uint16_t = 0;
    if( tcpListenPort != m_Engine.getEngineSettings().getLastFirewallPort() )
	{
		// this only affects windows firewall and not external firewalls like macafee :(
		m_Engine.getEngineSettings().setLastFirewallPort( tcpListenPort );
		const wchar_t* name = L"NoLimitConnect";
		wchar_t exePathAndName[ VX_MAX_PATH ];
		int iRetStrLen = GetModuleFileNameW( NULL, exePathAndName, VX_MAX_PATH );
		if( 0 != iRetStrLen )
		{
			m_WindowsFirewall.addFilewallException( exePathAndName, name, tcpListenPort, DEFAULT_UDP_PORT );
			lastPort = tcpListenPort;
		}

		//didnt work for macafee firewall
		//Windows XP:
		//add: netsh firewall add allowedprogram mode=ENABLE profile=ALL name=[exception name] program=[program path]
		//remove: netsh firewall delete allowedprogram profile=ALL program=[program path]
		//Windows 7:
		//add: netsh advfirewall firewall add rule action=allow profile=any protocol=any enable=yes direction=[in|out] name=[exception name] program=[program path]
		//remove: advfirewall firewall delete rule profile=any name=[exception name]
		// example netsh advfirewall firewall add rule name="Allow Messenger" dir=in action=allow program="C:\programfiles\messenger\msnmsgr.exe"
		char exePathAndNameA[ VX_MAX_PATH ];
		int pathStrLen = GetModuleFileNameA( NULL, exePathAndNameA, VX_MAX_PATH );
		char cmdBuf[ 2048 ];
		if( 0 != pathStrLen )
		{
			//if( 6.0 <= OsDetect::getWindowsVersionNumber() )
			//{
			//	// requires admin but xp version doesnt and seems to work
			//	// vista or higher
			//	strcpy( cmdBuf, "netsh advfirewall firewall add rule action=allow profile=any protocol=any enable=yes direction=in name=MyP2PWeb program=" );
			//}
			//else
			//{
				strcpy( cmdBuf, "netsh firewall add allowedprogram mode=ENABLE profile=ALL name=MyP2PWeb program=" );
			//}

			strcat( cmdBuf, exePathAndNameA );
			system( cmdBuf );
			sprintf( cmdBuf, "netsh firewall add portopening TCP %d MyP2PWeb%d", tcpListenPort, tcpListenPort );			system( cmdBuf );
		}
	}
#endif // TARGET_OS_WINDOWS
	
	VxTimer portTestTimer;

    if( testLoopbackFirst )
	{
		// we need to wait until listen port is up and running.. keep testing till we can connect
		bool loopbackSuccess = false;
		int retryLoopbackCnt = 5;
		while( retryLoopbackCnt )
		{
			if( false == testLoobackPing( lclIP, tcpListenPort ) )
			{
				LogMsg( LOG_INFO, "666 FAILED LOOPBACK Test ip %s port %d app sec %d thread 0x%x", lclIP.c_str(), tcpListenPort, GetApplicationAliveSec(), VxGetCurrentThreadId() );
				VxSleep( 1000 );
				retryLoopbackCnt--;
				continue;
			}
			else
			{
				loopbackSuccess = true;
				break;
			}
		}

		if( false == loopbackSuccess )
		{
#ifdef LOG_PORT_TEST
			LogMsg( LOG_ERROR, "777######### FAILED ALL LOOPBACK Tests ip %s port %d app sec %d thread 0x%x", rmtIP.c_str(), tcpListenPort, GetApplicationAliveSec(), VxGetCurrentThreadId() );
#endif // LOG_PORT_TEST
		}
	}

    FirewallSettings::EFirewallTestType testType = getEngineSettings().getFirewallTestSetting();
    if( FirewallSettings::eFirewallTestAssumeNoFirewall == testType )
    {
        m_EngineSettings.getExternalIp( retMyExternalIp );
        if( !retMyExternalIp.empty() )
        {
            LogMsg( LOG_INFO, "NetActionIsMyPortOpen::doAction: Your TCP Port %d IS ASSUMED OPEN :) IP is %s->%s in %3.3f sec thread 0x%x", tcpListenPort, lclIP.c_str(), retMyExternalIp.c_str(), portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
            m_Engine.sendToGuiStatusMessage( "Your TCP Port %d IS ASSUMED OPEN ON IP %s)", tcpListenPort, retMyExternalIp.c_str() );
            return eAppErrNone;
        }
        else
        {
            LogMsg( LOG_ERROR, "NetActionIsMyPortOpen::doAction: Test Port %d Is Open ERROR no external IP specified for Assume No Firewall thread 0x%x", tcpListenPort, VxGetCurrentThreadId() );
            m_Engine.sendToGuiStatusMessage( "Test Port %d Is Open ERROR no external IP specified for Assume No Firewall thread 0x%x", tcpListenPort, VxGetCurrentThreadId() );
            return eAppErrBadParameter;
        }
    }

    // use connection test url to test if out port is open
    VxSktConnectSimple portOpenConn1;

    std::string netSrvUrl;
    if( m_Engine.getNetworkStateMachine().isNetworkWebsitesResolved() )
    {
        StdStringFormat( netSrvUrl, "ptop://%s:%d", m_Engine.getNetworkStateMachine().getNetServiceIp().c_str(), m_Engine.getNetworkStateMachine().getNetServicePort() );
    }
    else
    {
        m_EngineSettings.getConnectTestUrl( netSrvUrl );
    }

    LogModule( eLogIsPortOpenTest, LOG_DEBUG, "Starting IsPortOpen lcl ip %s port %d test url %s app sec %d thread 0x%x", lclIP.c_str(), tcpListenPort, netSrvUrl.c_str(), GetApplicationAliveSec(), VxGetCurrentThreadId() );
    m_Engine.sendToGuiStatusMessage( "Attempting Connect lcl ip %s to connect service %s\n", lclIP.c_str(), netSrvUrl.c_str() );

    bool isCellDataNetwork = m_NetworkMgr.isCellularNetwork();

    std::string strHost;
    std::string strFile;
    uint16_t u16Port = 0;
    EAppErr	portOpenTestError = eAppErrNone;
    if( VxSplitHostAndFile( netSrvUrl.c_str(), strHost, strFile, u16Port ) )
    {
        portOpenConn1.connectTo( lclIP.c_str(), strHost.c_str(), u16Port, NETSERVICE_CONNECT_TIMEOUT );
        if( !portOpenConn1.isConnected() )
        {
            LogModule( eLogIsPortOpenTest, LOG_ERROR, "NetServicesMgr::actionReqConnectToNetService: FAILED to Connect lcl ip %s to connect service %s thread 0x%x", lclIP.c_str(), netSrvUrl.c_str(), VxGetCurrentThreadId() );
            m_Engine.sendToGuiStatusMessage( "FAILED Connect lcl ip %s to connect service %s\n", lclIP.c_str(), netSrvUrl.c_str() );
            return eAppErrFailedConnectNetServices;
        }
        else
        {
            portOpenTestError = sendAndRecieveIsMyPortOpen( portTestTimer,
                                                            &portOpenConn1,
                                                            tcpListenPort,
                                                            retMyExternalIp,
                                                            isCellDataNetwork );
            portOpenConn1.closeSkt();
        }
    }
    else
    {
        LogModule( eLogIsPortOpenTest, LOG_INFO, "NetActionIsMyPortOpen::doAction: FAILED to Split Service URL %s thread 0x%x", netSrvUrl.c_str(), VxGetCurrentThreadId() );
        portOpenTestError = eAppErrParseError;
    }

    if( isCellDataNetwork 
		|| ( eAppErrNone == portOpenTestError )
		|| ( portTestTimer.elapsedMs() > 15000 ) )
	{

		if( eAppErrNone == portOpenTestError )
		{
            LogModule( eLogIsPortOpenTest, LOG_INFO, "NetActionIsMyPortOpen::doAction: Your TCP Port %d IS OPEN :) IP is %s to connect test %s result %s in %3.3f sec thread 0x%x", tcpListenPort, lclIP.c_str(), netSrvUrl.c_str(), retMyExternalIp.c_str(), portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
            m_Engine.sendToGuiStatusMessage( "Your TCP Port %d IS OPEN :)", tcpListenPort );
		}
		else
		{
            LogModule( eLogIsPortOpenTest, LOG_INFO, "NetActionIsMyPortOpen::doAction: Your TCP Port %d IS CLOSED :) IP %s->%s in %3.3f sec thread 0x%x", tcpListenPort, lclIP.c_str(), netSrvUrl.c_str(), portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
            m_Engine.sendToGuiStatusMessage( "Your TCP Port %d IS CLOSED :( IP is lcl ip %s to connect test %s (%3.3f sec)", tcpListenPort, lclIP.c_str(), netSrvUrl.c_str(), portTestTimer.elapsedSec() );
            portOpenTestError = eAppErrPortIsClosed;
		}

		return portOpenTestError; // messages sent and result set and no sense in retying because all ports blocked on cell network
	}

	return portOpenTestError; 
}

//============================================================================
bool NetServicesMgr::testLoobackPing( std::string localIP, uint16_t tcpListenPort )
{
	VxSktConnectSimple toClientConn;
	VxTimer	pingTimer;
	std::string ipAddress = localIP;

	if( INVALID_SOCKET == toClientConn.connectTo( ipAddress.c_str(), tcpListenPort, 2000 ) )
	{
        if( IsLogEnabled( eLogNetworkState ) )
        {
		    LogMsg( LOG_ERROR, "##P NetServicesMgr::testLoobackPing: could not connect to %s:%d %3.3f sec\n", ipAddress.c_str(), tcpListenPort, pingTimer.elapsedSec() );
        }

		return false;
	}

	toClientConn.closeSkt();
	return true;
}

//============================================================================
EAppErr NetServicesMgr::sendAndRecieveIsMyPortOpen( VxTimer&				portTestTimer, 
													VxSktConnectSimple *	netServConn, 
													int						tcpListenPort,
													std::string&			retMyExternalIp,
													bool					sendMsgToUser )
{
    if( nullptr == netServConn || !netServConn->isConnected() )
    {
		LogMsg( LOG_INFO, "Is Your TCP Port %d Open Test Connect FAILED (%3.3f sec) thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
		if( sendMsgToUser )
		{
			m_Engine.sendToGuiStatusMessage( "Is TCP Port %d Open Test Connect FAILED (%3.3f sec)\n", tcpListenPort, portTestTimer.elapsedSec()  );
		}

		return eAppErrFailedToConnect;
	}

	std::string strNetActionUrl;
	m_NetServiceUtils.buildIsMyPortOpenUrl( netServConn, strNetActionUrl, tcpListenPort );

	LogMsg( LOG_INFO, "Is Port %d Open Connected in  %3.3f sec now Sending data thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
	portTestTimer.startTimer();
	RCODE rc = netServConn->sendData( strNetActionUrl.c_str(), ( int )strNetActionUrl.length() );
	if( rc )
	{
		LogMsg( LOG_ERROR, "Is TCP Port %d Open Send Command Error (%3.3f sec) thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
		if( sendMsgToUser )
		{
			m_Engine.sendToGuiStatusMessage( "Is TCP Port %d Open Send Command Error (%3.3f sec)\n", tcpListenPort, portTestTimer.elapsedSec()  );
		}

		return eAppErrTxError;
	}

	VxSleep( 1000 );
	char rxBuf[ 513 ];
	rxBuf[ 0 ] = 0;
	NetServiceHdr netServiceHdr;
	if( false == m_NetServiceUtils.rxNetServiceCmd( netServConn, 
													rxBuf, 
													sizeof( rxBuf ) - 1, 
													netServiceHdr, 
													IS_PORT_OPEN_RX_HDR_TIMEOUT, 
													IS_PORT_OPEN_RX_DATA_TIMEOUT ) )
	{
		if( sendMsgToUser )
		{
			LogMsg( LOG_ERROR,  "Is TCP Port %d Open Connect Test Server Response Time Out (%3.3f sec) thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
		}

		m_Engine.sendToGuiStatusMessage( "Is TCP Port %d Open Connect Test Server Response Time Out (%3.3f sec)\n", tcpListenPort, portTestTimer.elapsedSec()  );
		return eAppErrNetServicesFailedToRespond;
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	std::string content = rxBuf;
	if( 0 == content.length() )
	{
		LogMsg( LOG_ERROR, "Is TCP Port %d Open No Server Response Content (%3.3f sec) thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
		if( sendMsgToUser )
		{
			m_Engine.sendToGuiStatusMessage( "Is TCP Port %d Open No Server Response Content (%3.3f sec)\n", tcpListenPort, portTestTimer.elapsedSec()  );
		}

		return eAppErrRxError;
	}

	const char * contentBuf = content.c_str();
	if( '/' != contentBuf[content.length() -1] )
	{
		LogMsg( LOG_ERROR, "Is TCP Port %d Open Test Invalid Response Content (%3.3f sec) thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
		if( sendMsgToUser )
		{
			m_Engine.sendToGuiStatusMessage( "Is TCP Port %d Open Test Invalid Response Content (%3.3f sec)\n", tcpListenPort, portTestTimer.elapsedSec()  );
		}

		return eAppErrRxError;
	}

	((char *)contentBuf)[content.length() -1] = 0;

	std::vector<std::string> contentParts;
	StdStringSplit( content, '-', contentParts );
	if( 2 != contentParts.size() )
	{
		LogMsg( LOG_ERROR, "Is TCP Port %d Open Test Invalid Content Parts (%3.3f sec) thread 0x%x", tcpListenPort, portTestTimer.elapsedSec(), VxGetCurrentThreadId() );
		if( sendMsgToUser )
		{
			m_Engine.sendToGuiStatusMessage( "Is TCP Port %d Open Test Invalid Content Parts (%3.3f sec)\n", tcpListenPort, portTestTimer.elapsedSec()  );
		}

		return eAppErrParseError;
	}

	retMyExternalIp = contentParts[1];

	std::string strPayload = contentParts[0];
    int iIsOpen = atoi( contentParts[ 0 ].c_str() );

    LogModule( eLogConnect, LOG_INFO, "NetActionIsMyPortOpen::doAction: can direct connect %s my ip %s:%d thread 0x%x", strPayload.c_str(), retMyExternalIp.c_str(), tcpListenPort, VxGetCurrentThreadId() );

	return iIsOpen ? eAppErrNone : eAppErrPortIsClosed;
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdIsMyPortOpenReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
    RCODE rc = -1;
    // not called here.. handled in NetAction
    return rc;
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdQueryHostIdReq( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
    std::string toClientContent = "0"; // assume fail

    char * pSktBuf = ( char * )sktBase->getSktReadBuf();
    if( false == ( '/' == pSktBuf[ netServiceHdr.m_TotalDataLen - 1 ] ) )
    {
        sktBase->sktBufAmountRead( 0 );
        LogModule( eLogNetService, LOG_ERROR, "NetServicesMgr::handleNetCmdQueryHostIdReq: invalid content\n" );
        return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdQueryHostOnlineIdReply, toClientContent, ( int )eAppErrBadParameter );
    }

    if( !m_Engine.getHasHostService( eHostServiceNetworkHost ) )
    {
        // only allowed to query host Id if we have Network Host Service
        LogModule( eLogNetService, LOG_ERROR, "NetServicesMgr::handleNetCmdQueryHostIdReq: Access Denied.. Not a Network Host" );
        return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdQueryHostOnlineIdReply, toClientContent, ( int )eAppErrAccessDenied );
    }

    VxGUID myId = m_Engine.getMyOnlineId();
    std::string hexMyId;
    if( !myId.toHexString( hexMyId ) )
    {
        LogModule( eLogNetService, LOG_ERROR, "NetServicesMgr::handleNetCmdQueryHostIdReq: Access Denied.. Invalid My Online Id" );
        return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdQueryHostOnlineIdReply, toClientContent, ( int )eAppErrAccessDenied );
    }

    LogModule( eLogNetService, LOG_INFO, "handleNetCmdQueryHostIdReq: SUCCESS %s", hexMyId.c_str() );
    return m_NetServiceUtils.buildAndSendCmd( sktBase, eNetCmdQueryHostOnlineIdReply, hexMyId, ( int )eAppErrNone );
}

//============================================================================
EAppErr NetServicesMgr::sendAndRecieveQueryHostId( VxTimer&				testTimer,
                                                   VxSktConnectSimple *	netServConn,
                                                   VxGUID&			    retHostId,
                                                   bool					sendMsgToUser )
{
    retHostId.clearVxGUID();
    if( nullptr == netServConn || !netServConn->isConnected() ) 
    {
        LogModule( eLogNetService, LOG_INFO, "Query Host Online Id Connect FAILED (%3.3f sec) thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
        if( sendMsgToUser )
        {
            m_Engine.sendToGuiStatusMessage( "Query Host Online Id Connect FAILED (%3.3f sec)\n", testTimer.elapsedSec() );
        }

        return eAppErrFailedToConnect;
    }

    std::string strContent("0");

    LogMsg( LOG_INFO, "Query Host Online Id Connected in  %3.3f sec now Sending data thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
    testTimer.startTimer();
    RCODE rc = m_NetServiceUtils.buildAndSendCmd( netServConn, eNetCmdQueryHostOnlineIdReq, strContent, ( int )eAppErrNone );
    if( rc )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id Send Command Error (%3.3f sec) thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
        if( sendMsgToUser )
        {
            m_Engine.sendToGuiStatusMessage( "Query Host Online Id Send Command Error (%3.3f sec)\n", testTimer.elapsedSec() );
        }

        return eAppErrTxError;
    }

    VxSleep( 100 );
    char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
    NetServiceHdr netServiceHdr;
    if( false == m_NetServiceUtils.rxNetServiceCmd( netServConn,
                                                    rxBuf,
                                                    sizeof( rxBuf ) - 1,
                                                    netServiceHdr,
                                                    IS_PORT_OPEN_RX_HDR_TIMEOUT,
                                                    IS_PORT_OPEN_RX_DATA_TIMEOUT ) )
    {
        if( sendMsgToUser )
        {
            LogMsg( LOG_ERROR, "Query Host Online Id Server Response Time Out (%3.3f sec) thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
        }

        m_Engine.sendToGuiStatusMessage( "Query Host Online Id Server Response Time Out (%3.3f sec)\n", testTimer.elapsedSec() );
        return eAppErrNetServicesFailedToRespond;
    }

    if( netServiceHdr.getError() )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id Server Response Has Error %d (%3.3f sec) thread 0x%x", netServiceHdr.getError(), testTimer.elapsedSec(), VxGetCurrentThreadId() );
        if( sendMsgToUser )
        {
            m_Engine.sendToGuiStatusMessage( "Query Host Online Id Server ResponseHas Error Code %d (%3.3f sec)\n", netServiceHdr.getError(), testTimer.elapsedSec() );
        }

        return eAppErrRxError;
    }

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
    std::string content = rxBuf;
    if( 0 == content.length() )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id Server Response Content (%3.3f sec) thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
        if( sendMsgToUser )
        {
            m_Engine.sendToGuiStatusMessage( "Query Host Online Id Server Response Content (%3.3f sec)\n", testTimer.elapsedSec() );
        }

        return eAppErrRxError;
    }

    const char * contentBuf = content.c_str();
    if( '/' != contentBuf[ content.length() - 1 ] )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id Invalid Response Content (%3.3f sec) thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
        if( sendMsgToUser )
        {
            m_Engine.sendToGuiStatusMessage( "Query Host Online Id Invalid Response Content (%3.3f sec)\n", testTimer.elapsedSec() );
        }

        return eAppErrSeviceError;
    }

    ( ( char * )contentBuf )[ content.length() - 1 ] = 0;

    VxGUID hostId;
    hostId.fromVxGUIDHexString( content.c_str() );
    if( !hostId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id Invalid Content (%3.3f sec) thread 0x%x", testTimer.elapsedSec(), VxGetCurrentThreadId() );
        if( sendMsgToUser )
        {
            m_Engine.sendToGuiStatusMessage( "Query Host Online Id Invalid Content (%3.3f sec)\n", testTimer.elapsedSec() );
        }

        return eAppErrParseError;
    }

    retHostId = hostId;
    return eAppErrNone;
}

//============================================================================
RCODE NetServicesMgr::handleNetCmdQueryHostIdReply( VxSktBase * sktBase, NetServiceHdr& netServiceHdr )
{
    RCODE rc = -1;
    // not called here.. handled in NetAction
    return rc;
}

//============================================================================
void NetServicesMgr::netActionResultQueryHostId( EAppErr eAppErr, VxGUID& hostId )
{
    if( eAppErr == eAppErrNone )
    {
        m_Engine.getNetStatusAccum().setQueryHostOnlineId( true, hostId );
    }
    else
    {
        m_Engine.getNetStatusAccum().setQueryHostOnlineId( false, hostId );
    }

    if( m_pfuncQueryHostIdCallbackHandler )
    {
        m_pfuncQueryHostIdCallbackHandler( m_QueryHostIdCallbackUserData, eAppErr, hostId );
    }
}
