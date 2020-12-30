//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

#include "RunTestUrlTest.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceHdr.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServiceUtils.h>
#include <GoTvCore/GoTvP2P/Network/NetworkStateMachine.h>
#include <GoTvCore/GoTvP2P/Network/NetworkMgr.h>

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
RunTestUrlTest::RunTestUrlTest( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr, NetServiceUtils& netServiceUtils )
    : NetworkTestBase( engine, engineSettings, netServicesMgr, netServiceUtils )
{
}

//============================================================================
void RunTestUrlTest::runTestShutdown( void )
{
    m_RunTestThread.abortThreadRun( true );
}

//============================================================================
void RunTestUrlTest::fromGuiRunTestUrlTest( const char * myUrl, const char * ptopUrl, ENetCmdType testType )
{
    if ( !m_TestIsRunning && !m_RunTestThread.isThreadRunning() )
	{
        m_TestIsRunning = true;
        setTestName( "PING TEST: " );
        sendTestLog( "Starting Ping Test URL test" );
        m_MyUrl.setUrl( myUrl );
        m_TestUrl.setUrl( ptopUrl );
        if( !m_MyUrl.validateUrl( true ) )
        {
            sendRunTestStatus( eRunTestStatusTestFail, "Local URL is invalid\n" );
            m_TestIsRunning = false;
            return;
        }
        else if( !m_TestUrl.validateUrl( false ) )
        {
            sendRunTestStatus( eRunTestStatusTestFail, "Test URL is invalid\n" );
            m_TestIsRunning = false;
            return;
        }
        else
        {
            LogModule( eLogRunTest, LOG_INFO, "RunTestUrlTest::fromGuiRunRunTestUrlTest" );
            startNetworkTest();
        }
    }
    else
    {
        LogModule( eLogRunTest, LOG_INFO, "RunTestUrlTest already running fromGuiRunRunTestUrlTest" );
        sendTestLog( "Already Started Ping Test URL test" );
    }
}

//============================================================================
void RunTestUrlTest::threadRunNetworkTest( void )
{
    std::string netHostUrl;

    m_EngineSettings.getNetworkHostUrl( netHostUrl );
    LogModule( eLogRunTest, LOG_INFO, "attempting connection test with host test url %s thread 0x%x", 
               netHostUrl.c_str(), VxGetCurrentThreadId() );
    doRunTest( netHostUrl );
}

//============================================================================
ERunTestStatus RunTestUrlTest::doRunTest( std::string& nodeUrl )
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

    LogModule( eLogRunTest, LOG_INFO, "RunTestUrlTest: sec %3.3f : connecting to %s thread 0x%x", 
               testTimer.elapsedSec(), nodeUrl.c_str(), VxGetCurrentThreadId() );
	if( false == netServConn.connectToWebsite(	nodeUrl.c_str(), 
		strHost, 
		strFile, 
		u16Port, 
		NETSERVICE_CONNECT_TIMEOUT ) )
	{
        sendRunTestStatus( eRunTestStatusConnectFail, "%s Could not connected to %s..Please check settings thread 0x%x",
                        getTestName().c_str(), nodeUrl.c_str(), VxGetCurrentThreadId() );

		doRunTestFailed();

		return eRunTestStatusConnectFail;
	}

	netServConn.dumpConnectionInfo();
	std::string strNetActionUrl;
	m_NetServiceUtils.buildPingTestUrl( &netServConn, strNetActionUrl );
    LogModule( eLogRunTest, LOG_INFO, "RunTestUrlTest: action url %s thread 0x%x", strNetActionUrl.c_str(), VxGetCurrentThreadId() );
	connectTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "RunTestUrlTest: sec %3.3f : sending %d action data thread 0x%x", 
               connectTime, (int)strNetActionUrl.length(), VxGetCurrentThreadId() );

	RCODE rc = netServConn.sendData( strNetActionUrl.c_str(), (int)strNetActionUrl.length() );
	if( rc )
	{
        LogModule( eLogRunTest, LOG_ERROR, "NetActionIsMyPortOpen::doAction: sendData error %d thread 0x%x", rc, VxGetCurrentThreadId() );
        sendRunTestStatus( eRunTestStatusConnectionDropped,
			"%s Connected to %s but dropped connection (wrong network key ?) %s thread 0x%x", testName.c_str(), nodeUrl.c_str(), m_Engine.getNetworkMgr().getNetworkKey(), VxGetCurrentThreadId() );
		return doRunTestFailed();
	}

	sendTime = testTimer.elapsedSec();
	LogMsg( LOG_INFO, "RunTestUrlTest: sec %3.3f : waiting for is port open response thread 0x%x", sendTime, VxGetCurrentThreadId() );
	char rxBuf[ 513 ];
    rxBuf[ 0 ] = 0;
	NetServiceHdr netServiceHdr;
	if( false == m_NetServiceUtils.rxNetServiceCmd( &netServConn, 
													rxBuf, 
													sizeof( rxBuf ) - 1, 
													netServiceHdr, 
                                                    PING_TEST_RX_HDR_TIMEOUT,
                                                    PING_TEST_RX_DATA_TIMEOUT ) )
	{
		sendRunTestStatus( eRunTestStatusConnectionDropped,
			"%s Connected to %s but failed to respond (wrong network key ?) thread 0x%x", testName.c_str(), nodeUrl.c_str(), VxGetCurrentThreadId() );
        netServConn.closeSkt();
		return doRunTestFailed();
	}

    rxBuf[ sizeof( rxBuf ) - 1 ] = 0;
	std::string content = rxBuf;
	reponseTime = testTimer.elapsedSec();
    LogModule( eLogRunTest, LOG_INFO, "RunTestUrlTest: response len %d total time %3.3fsec connect %3.3fsec send %3.3fsec response %3.3f sec thread 0x%x",
		content.length(),
		reponseTime, connectTime, sendTime - connectTime, reponseTime - sendTime, VxGetCurrentThreadId() );
	if( 0 == content.length() )
	{
        LogModule( eLogRunTest, LOG_ERROR, "RunTestUrlTest: no content in response" );
		sendRunTestStatus( eRunTestStatusInvalidResponse, "%s invalid response content %s\n", testName.c_str(), content.c_str() );
        netServConn.closeSkt();
		return doRunTestFailed();
	}

	const char * contentBuf = content.c_str();
	if( '/' != contentBuf[content.length() -1] )
	{
        LogModule( eLogRunTest, LOG_ERROR, "RunTestUrlTest no trailing / in PONG content" );
		sendRunTestStatus( eRunTestStatusInvalidResponse, "%s invalid response PONG content %s\n", testName.c_str(), content.c_str() );
        netServConn.closeSkt();
		return doRunTestFailed();
	}

	((char *)contentBuf)[content.length() -1] = 0;

	std::string strPayload = content;
    if( strPayload.empty() || strPayload.length() < 5  )
    {
        LogModule( eLogRunTest, LOG_ERROR, "RunTestUrlTest no content" );
        sendRunTestStatus( eRunTestStatusInvalidResponse, "%s No PONG content\n", testName.c_str() );
        netServConn.closeSkt();
        return doRunTestFailed();
    }

    sendTestLog( "%s PONG Content (%s)", testName.c_str(), strPayload.c_str() );
    std::string strPongSig = strPayload.substr( 0, 5 );
    if( strPongSig != "PONG-" )
    {
        sendRunTestStatus( eRunTestStatusInvalidResponse, "%s content did not contain PONG- %s\n", testName.c_str(), content.c_str() );
        netServConn.closeSkt();
        return doRunTestFailed();
    }
    else
    {
        std::string strMyIP = strPayload.substr( 5, strPayload.length() - 5 );
        LogModule( eLogRunTest, LOG_VERBOSE, "%s PING test success .. My IP is %s", testName.c_str(), strMyIP.c_str() );
        sendTestLog( "Test %s success. PONG returned My IP (%s)", testName.c_str(), strMyIP.c_str() );
        sendTestLog( "Test %s Elapsed Seconds Connect %3.3f sec Send %3.3f sec Respond %3.3f sec", testName.c_str(), connectTime, sendTime - connectTime, reponseTime - sendTime );
        netServConn.closeSkt();
        return doRunTestSuccess( );
    }
}

//============================================================================
ERunTestStatus RunTestUrlTest::doRunTestFailed()
{
	sendRunTestStatus( eRunTestStatusTestCompleteFail,
		"\n" );
    m_TestIsRunning = false;
	return eRunTestStatusTestCompleteFail;
}

//============================================================================
ERunTestStatus RunTestUrlTest::doRunTestSuccess( void )
{
	sendRunTestStatus( eRunTestStatusTestCompleteSuccess,
		"\n" );
    m_TestIsRunning = false;
	return eRunTestStatusTestCompleteSuccess;
}

