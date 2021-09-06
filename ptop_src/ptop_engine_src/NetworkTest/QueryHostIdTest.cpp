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

#include "QueryHostIdTest.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceHdr.h>
#include <ptop_src/ptop_engine_src/NetServices/NetServiceUtils.h>
#include <ptop_src/ptop_engine_src/Network/NetworkStateMachine.h>
#include <ptop_src/ptop_engine_src/Network/NetworkMgr.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxParse.h>

#include <NetLib/VxSktUtil.h>
#include <NetLib/VxPeerMgr.h>
#include <NetLib/VxSktConnectSimple.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
QueryHostIdTest::QueryHostIdTest( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils )
    : NetworkTestBase( engine, engineSettings, netServicesMgr, netServiceUtils )
{
    setTestName( "QUERY HOST ID: " );
}

//============================================================================
void QueryHostIdTest::runTestShutdown( void )
{
    m_RunTestThread.abortThreadRun( true );
}

//============================================================================
void QueryHostIdTest::fromGuiRunQueryHostIdTest( void )
{
    if ( !m_RunTestThread.isThreadRunning() )
	{
        m_TestIsRunning = true;
        LogModule( eLogRunTest, LOG_INFO, "QueryHostIdTest::fromGuiRunQueryHostIdTest thread 0x%x",VxGetCurrentThreadId() );
        sendTestLog( "Starting Query Host Id test" );
        startNetworkTest();
    }
    else
    {
        LogModule( eLogRunTest, LOG_INFO, "QueryHostIdTest already running fromGuiRunQueryHostIdTest thread 0x%x",VxGetCurrentThreadId() );
        sendTestLog( "Already Started Query Host Id test" );
    }
}

//============================================================================
void QueryHostIdTest::threadRunNetworkTest( void )
{
    std::string netHostUrl;

    m_EngineSettings.getNetworkHostUrl( netHostUrl );
    LogModule( eLogRunTest, LOG_INFO, "attempting connection test with host test url %s thread 0x%x", 
               netHostUrl.c_str(), VxGetCurrentThreadId() );
    doRunTest( netHostUrl );
}

//============================================================================
ERunTestStatus QueryHostIdTest::doRunTest( std::string& nodeUrl )
{
    std::string testName = getTestName();
	sendTestLog(	"start %s node %s \n", 
					testName.c_str(), 
					nodeUrl.c_str());

	VxSktConnectSimple netServConn;
	std::string strHost;
	std::string strFile;
	uint16_t u16Port;
	VxTimer testTimer;
	double connectTime = 0;
	double sendTime= 0;
	double reponseTime= 0;
	std::string strResolveIp( "" );

    LogModule( eLogRunTest, LOG_INFO, "QueryHostIdTest: sec %3.3f : connecting to %s thread 0x%x", 
               testTimer.elapsedSec(), nodeUrl.c_str(), VxGetCurrentThreadId() );
	if( false == netServConn.connectToWebsite(	nodeUrl.c_str(), 
		strHost, 
		strFile, 
		u16Port, 
		strResolveIp,
		NETSERVICE_CONNECT_TIMEOUT ) )
	{
        sendRunTestStatus( eRunTestStatusConnectFail, "%s Could not connected to %s IP %s..Please check settings",
                        getTestName().c_str(), nodeUrl.c_str(), strResolveIp.c_str() );

		doRunTestFailed();

		return eRunTestStatusConnectFail;
	}

	netServConn.dumpConnectionInfo();
	std::string strNetActionUrl;
	m_NetServiceUtils.buildQueryHostIdUrl( &netServConn, strNetActionUrl );
    LogModule( eLogRunTest, LOG_INFO, "QueryHostIdTest: action url %s thread 0x%x", strNetActionUrl.c_str(), VxGetCurrentThreadId() );
	connectTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "QueryHostIdTest: sec %3.3f : sending %d action data thread 0x%x", 
               connectTime, (int)strNetActionUrl.length(), VxGetCurrentThreadId() );

	RCODE rc = netServConn.sendData( strNetActionUrl.c_str(), (int)strNetActionUrl.length() );
	if( rc )
	{
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest: sendData error %d thread 0x%x", rc, VxGetCurrentThreadId() );
        sendRunTestStatus( eRunTestStatusConnectionDropped,
			"%s Connected to %s but dropped connection (wrong network key ?) %s thread 0x%x", testName.c_str(), nodeUrl.c_str(), m_Engine.getNetworkMgr().getNetworkKey(), VxGetCurrentThreadId() );
		return doRunTestFailed();
	}

	sendTime = testTimer.elapsedSec();
	LogMsg( LOG_INFO, "QueryHostIdTest: sec %3.3f : waiting for is port open response thread 0x%x", sendTime, VxGetCurrentThreadId() );
	char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
	NetServiceHdr netServiceHdr;
	if( false == m_NetServiceUtils.rxNetServiceCmd( &netServConn, 
													rxBuf, 
													sizeof( rxBuf ) - 1, 
													netServiceHdr, 
                                                    QUERY_HOST_ID_RX_HDR_TIMEOUT,
                                                    QUERY_HOST_ID_RX_DATA_TIMEOUT ) )
	{
		sendRunTestStatus( eRunTestStatusConnectionDropped,
			"%s Connected to %s but failed to respond (wrong network key ?) thread 0x%x", testName.c_str(), nodeUrl.c_str(), VxGetCurrentThreadId() );
		return doRunTestFailed();
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	std::string content = rxBuf;
	reponseTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "QueryHostIdTest: response len %d total time %3.3fsec connect %3.3fsec send %3.3fsec response %3.3f sec thread 0x%x",
		content.length(),
		reponseTime, connectTime, sendTime - connectTime, reponseTime - sendTime, VxGetCurrentThreadId() );
	if( 0 == content.length() )
	{
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest: no content in response" );
		sendRunTestStatus( eRunTestStatusInvalidResponse, "%s invalid response content %s\n", testName.c_str(), content.c_str(), VxGetCurrentThreadId() );
		return doRunTestFailed();
	}

	const char * contentBuf = content.c_str();
	if( '/' != contentBuf[content.length() -1] )
	{
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest no trailing / in content thread 0x%x", VxGetCurrentThreadId() );
		sendRunTestStatus( eRunTestStatusInvalidResponse, "%s invalid response content %s\n", testName.c_str(), content.c_str() );
		return doRunTestFailed();
	}

	((char *)contentBuf)[content.length() -1] = 0;

	std::string strPayload = content;
    if( content.empty() )
    {
        LogModule( eLogRunTest, LOG_ERROR, "QueryHostIdTest no host id in content thread 0x%x", VxGetCurrentThreadId() );
        sendRunTestStatus( eRunTestStatusInvalidResponse, "%s invalid host id %s\n", testName.c_str(), content.c_str() );
        return doRunTestFailed();
    }

    VxGUID hostId;
    hostId.fromVxGUIDHexString( strPayload.c_str() );
    if( !hostId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "Query Host Online Id %s Invalid Content (%3.3f sec) thread 0x%x", content.c_str(), testTimer.elapsedSec(), VxGetCurrentThreadId() );
        sendRunTestStatus( eRunTestStatusInvalidResponse, "%s invalid host id %s\n", testName.c_str(), content.c_str() );
        return doRunTestFailed();
    }

    std::string hostIdStr = hostId.toHexString();
    LogModule( eLogRunTest, LOG_VERBOSE, "test success %s host id %s thread 0x%x", testName.c_str(), hostIdStr.c_str(), VxGetCurrentThreadId() );
	sendTestLog( "Test %s complete with Id %s Elapsed Seconds Connect %3.3fsec Send %3.3fsec Respond %3.3f sec", testName.c_str(), hostIdStr.c_str(), connectTime, sendTime - connectTime, reponseTime - sendTime );
	return doRunTestSuccess( );
}

//============================================================================
ERunTestStatus QueryHostIdTest::doRunTestFailed()
{
	sendRunTestStatus( eRunTestStatusTestComplete,
		"\n" );
	return eRunTestStatusTestComplete;
}

//============================================================================
ERunTestStatus QueryHostIdTest::doRunTestSuccess( void )
{
	sendRunTestStatus( eRunTestStatusTestComplete,
		"\n" );
	return eRunTestStatusTestComplete;
}

