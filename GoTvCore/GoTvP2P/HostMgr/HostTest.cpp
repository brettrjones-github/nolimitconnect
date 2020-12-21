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

#include "HostTest.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>
#include <GoTvInterface/IToGui.h>
#include <GoTvInterface/IGoTv.h>


#include <CoreLib/VxGlobals.h>
#include <NetLib/VxSktUtil.h>
#include <NetLib/VxResolveHost.h>
#include <NetLib/VxSktConnectSimple.h>

#include <stdio.h>
#include <stdarg.h>

namespace
{
	//============================================================================
    void * HostUrlTestThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		HostTest * anchorTest = (HostTest *)poThread->getThreadUserParam();
        if( anchorTest && false == poThread->isAborted() )
		{
			anchorTest->doHostUrlTest();
		}

		poThread->threadAboutToExit();
        return nullptr;
	}

	//============================================================================
	uint32_t NetServiceUrlTestThreadFunc( void * pvContext )
	{
		VxThread * poThread = (VxThread *)pvContext;
		poThread->setIsThreadRunning( true );
		HostTest * anchorTest = (HostTest *)poThread->getThreadUserParam();
        if( anchorTest && false == poThread->isAborted() )
		{
			anchorTest->doNetServiceUrlTest();
		}

		poThread->threadAboutToExit();
		return 0;
	}
}

//============================================================================
HostTest::HostTest( P2PEngine& engine, EngineSettings& engineSettings, NetServicesMgr& netServicesMgr )
: m_Engine( engine )
, m_EngineSettings( engineSettings )
, m_NetServicesMgr( netServicesMgr )
{
}

//============================================================================
IToGui& HostTest::getToGui()
{
    return IToGui::getToGui();
}

//============================================================================
const char * HostTest::getTestName( bool isHost )
{
	if( isHost )
	{
		return "ANCHOR TEST: ";
	}

	return "CONNECT TEST: ";
}

//============================================================================
void HostTest::fromGuiVerifyNetHostSettings( void )
{
	startHostUrlTest();
	startNetServiceUrlTest();
}

//============================================================================
void HostTest::startHostUrlTest( void )
{
	m_HostThread.abortThreadRun( true );
	while( m_HostThread.isThreadRunning() )
	{
		VxSleep( 200 );
	}

	m_HostThread.startThread( (VX_THREAD_FUNCTION_T)HostUrlTestThreadFunc, this, "HostUrlTestThread" );
}

//============================================================================
void HostTest::startNetServiceUrlTest( void )
{
	m_NetServiceThread.abortThreadRun( true );
	while( m_NetServiceThread.isThreadRunning() )
	{
		VxSleep( 200 );
	}

	m_NetServiceThread.startThread( (VX_THREAD_FUNCTION_T)NetServiceUrlTestThreadFunc, this, "NetServiceTestThread" );
}

//============================================================================
void HostTest::sendTestStatus( EHostTestStatus eStatus, const char * msg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start( argList, msg );
	vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	LogMsg( LOG_STATUS, "Test Status %s: %s\n", DescribeHostStatus( eStatus), as8Buf ); 
	IToGui::getToGui().toGuiHostStatus( eStatus, as8Buf );
}

//============================================================================
void HostTest::sendTestLog( const char * msg, ... )
{
	char as8Buf[ 1024 ];
	va_list argList;
	va_start( argList, msg );
	vsnprintf( as8Buf, sizeof( as8Buf ), msg, argList );
	as8Buf[sizeof( as8Buf ) - 1] = 0;
	va_end( argList );
	LogMsg( LOG_STATUS, "Host Test Log %s\n", as8Buf ); 
	IToGui::getToGui().toGuiHostStatus( eHostTestStatusLogMsg, as8Buf );
}

//============================================================================
void HostTest::doHostUrlTest( void )
{
	if( m_Engine.getHasHostService( eHostServiceNetworkHost ) )
	{
		sendTestStatus( eHostTestStatusHostOk,
			":Skipped Host Test Because Is Host\n" );
		return;
	}

	std::string anchorUrl;
	m_EngineSettings.getNetworkHostUrl( anchorUrl );
	doConnectionTest( anchorUrl, true );
}

//============================================================================
void HostTest::doNetServiceUrlTest( void )
{
	std::string netServiceUrl;
	m_EngineSettings.getConnectTestUrl( netServiceUrl );
	doConnectionTest( netServiceUrl, false );
}

//============================================================================
bool HostTest::doConnectionTest( std::string& nodeUrl, bool isHost )
{
	sendTestLog( "%s test start using %s", getTestName( isHost ), nodeUrl.c_str() );

	VxSktConnectSimple sktSimple;
	VxTimer testTimer;
	std::string strHost;
	std::string strFile;
	uint16_t u16Port;

	if( false == sktSimple.connectToWebsite(	nodeUrl.c_str(), 
												strHost, 
												strFile, 
												u16Port, 
												ANCHOR_CONNECT_TIMEOUT ) )
	{
		sendTestStatus( isHost ? eHostTestStatusHostConnectFail : eHostTestStatusNetServiceConnectFail,
			"%s Could not connect to %s (%s)", getTestName( isHost ), nodeUrl.c_str(), sktSimple.getRemoteIpAddress()  );
		return doConnectTestFailed( isHost );
	}

	sktSimple.dumpConnectionInfo();

	std::string retPong;
	if( false == m_NetServicesMgr.sendAndRecievePing( testTimer, sktSimple, retPong, 5000 ) )
	{
		sendTestStatus( isHost ? eHostTestStatusHostConnectionDropped : eHostTestStatusNetServiceConnectionDropped,
			"%s Connected to %s but dropped connection (wrong network key ?)", getTestName( isHost ), nodeUrl.c_str() );
		sktSimple.closeSkt();
		return doConnectTestFailed( isHost );
	}

	sktSimple.closeSkt();
	return doConnectTestSuccess( isHost, retPong );
}

//============================================================================
bool HostTest::doConnectTestFailed( bool isHost )
{
	sendTestStatus( isHost ? eHostTestStatusHostTestComplete : eHostTestStatusNetServiceTestComplete,
		"%s Test Failed", getTestName( isHost ) );
	return false;
}

//============================================================================
bool HostTest::doConnectTestSuccess( bool isHost, std::string& pongResult )
{
	sendTestStatus( isHost ? eHostTestStatusHostTestComplete : eHostTestStatusNetServiceTestComplete,
		"%s Test Success %s", getTestName( isHost ), pongResult.c_str() );
	return true;
}


