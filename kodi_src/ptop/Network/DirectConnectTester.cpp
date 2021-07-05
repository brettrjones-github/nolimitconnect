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

#include "DirectConnectTester.h"
#include "NetworkStateMachine.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/NetServices/NetServicesMgr.h>

namespace
{
	void MyPortOpenCallback( void * userData, EAppErr eAppErr, std::string& myExternalIp )
	{
		if( userData )
		{
			((DirectConnectTester *)userData)->myPortOpenCallback( eAppErr, myExternalIp );
		}
	}
}

//============================================================================
DirectConnectTester::DirectConnectTester( NetworkStateMachine& stateMachine )
: m_NetworkStateMachine( stateMachine )
, m_Engine( stateMachine.getEngine() )
, m_NetworkMgr( stateMachine.getNetworkMgr() )
, m_NetServicesMgr( stateMachine.getEngine().getNetServicesMgr() )
, m_bTestIsRunning( false )
{
}

//============================================================================
DirectConnectTester::~DirectConnectTester()
{
}

//============================================================================
void DirectConnectTester::testCanDirectConnect( void )
{
	while( m_bTestIsRunning )
	{
		LogMsg( LOG_INFO, "DirectConnectTester::testCanDirectConnect: waiting for prev test to complete\n" );
		VxSleep( 400 );
	}

	m_bTestIsRunning = true;
	m_TestResults.m_eAppErr  = eAppErrUnknown;

	m_NetServicesMgr.setMyPortOpenResultCallback( MyPortOpenCallback, this );

	m_NetServicesMgr.testIsMyPortOpen();
}

//============================================================================
bool DirectConnectTester::isDirectConnectTestComplete( void )
{
	return !m_bTestIsRunning;
}

//============================================================================
bool DirectConnectTester::isTestResultCanDirectConnect( void )
{
	return m_TestResults.getCanDirectConnect();
}

//============================================================================
void DirectConnectTester::myPortOpenCallback( EAppErr eAppErr, std::string& myExternalIp )
{
	// results can be
	// eAppErrNone						// port is open
	// eAppErrPortIsClosed				// port is closed
	// eAppErrFailedConnectNetServices	// could not connect to net services
	// eAppErrTxError;
	// eAppErrRxError;
	// eAppErrParseError;
	m_TestResults.m_eAppErr  = eAppErr;
	m_TestResults.m_MyIpAddr = myExternalIp;

	m_bTestIsRunning = false;
}