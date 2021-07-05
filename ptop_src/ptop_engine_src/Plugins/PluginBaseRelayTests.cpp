//============================================================================
// Copyright (C) 2010 Brett R. Jones
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

#include "PluginBaseRelay.h"
#include "PluginMgr.h"

#include "RelayClientTestSession.h"
#include "RelayServerTestSession.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxGlobals.h>


//=== tests ===//
//============================================================================
bool PluginBaseRelay::doRelayTest( VxNetIdent * netIdent )
{
	bool bResult = false;
	VxSktBase * sktBase;
	if( m_PluginMgr.pluginApiSktConnectTo( m_ePluginType, netIdent, 0, &sktBase ) )
	{
		//m_PluginMgr.pluginApiSktClose(sktBase);

		PktRelayTestReq pktReq;
        pktReq.setTimeStampMs( GetGmtTimeMs() );
        VxGUID myOnlineId = getEngine().getMyOnlineId();
        pktReq.setLclSessionId( myOnlineId );
		pktReq.setRmtSessionId( netIdent->getMyOnlineId() );
		bResult = txPacket( netIdent, sktBase, &pktReq );
		if( bResult )
		{
			m_PluginMgr.pluginApiLog( m_ePluginType, "Connect to Relay SUCCESS\n" );
			LogMsg( LOG_INFO, "doRelayTest SUCCESS\n" );
		}
		else
		{
			m_PluginMgr.pluginApiLog( m_ePluginType, "Failed Sending Relay Test Packet\n" );
			LogMsg( LOG_INFO, "Failed Sending Relay Test Packet\n" );
		}
	}
	else
	{
		m_PluginMgr.pluginApiLog(m_ePluginType, "Failed to connect to Relay\n" );
		LogMsg( LOG_INFO, "doRelayTest FAILED\n" );
	}


	/*
	RelayClientTestSession * poClientSession = new RelayClientTestSession();
	poClientSession->setLclSessionId( poClientSession );
	poClientSession->setIsInTest(true);
	m_aoSessions.push_back( poClientSession );

	RelayServerTestSession * poServerSession = new RelayServerTestSession();
	poServerSession->setLclSessionId( poServerSession );
	poServerSession->setIsInTest(true);
	m_aoSessions.push_back( poServerSession );

	const char * pName1 = "jackblack";
	const char * pName2 = "Jill";
	const char * pName3 = "Tarzan";

	VxNetIdent * poLclIdent				= m_PluginMgr.pluginApiGetMyIdentity();
	VxNetIdent * poContactIdent			= m_PluginMgr.pluginApiFindUser( pName3 );
	VxNetIdent * poRmtIdent				= NULL;
	if( 0 == strcmp(poLclIdent->getOnlineName(), pName1 ) )
	{
		poRmtIdent = m_PluginMgr.pluginApiFindUser( pName2 );
	}
	else
	{
		poRmtIdent = m_PluginMgr.pluginApiFindUser( pName1 );
	}
	if( poLclIdent && poContactIdent)
	{
		RelayClientTestSession * poClientSession = new RelayClientTestSession();
		if( poLclIdent && poRmtIdent, poContactIdent)
		{
			bool bResult = onTestAll( poClientSession, poServerSession, poLclIdent, poRmtIdent, poContactIdent );
		}
		else if( poClientSession && poServerSession )
		{
			onTestServerClient(poClientSession, poServerSession, poLclIdent, poRmtIdent);
		}
	}
	else
	{
		LogMsg( LOG_INFO, "PluginBaseRelay::doTests: missing contact 0x%x, 0x%x 0x%x\n", poLclIdent, poRmtIdent, poContactIdent );
		onTestComplete();	
	}
	//removeSession( poClientSession );
	//removeSession( poServerSession );
	*/
	return bResult;
}

//============================================================================
bool PluginBaseRelay::onTestAll(	RelayClientTestSession * poClientSession, 
								RelayServerTestSession * poServerSession, 
								VxNetIdent * poLclIdent, 
								VxNetIdent * poRmtIdent, 
								VxNetIdent * po3rdIdent )
{
	bool bResult = onTestRequestRelayService(	poClientSession,
												poRmtIdent );
	return bResult;
}

//============================================================================
void PluginBaseRelay::onTestComplete( void )
{
	m_PluginSessionMgr.removeAllSessions( true );
}

//============================================================================
bool PluginBaseRelay::onTestServerClient(	RelayClientTestSession * poClientSession, 
										RelayServerTestSession * poServerSession, 
										VxNetIdent * poLclIdent, 
										VxNetIdent * poRmtIdent )
{
	bool bResult = onTestRequestRelayService( poClientSession, poRmtIdent );

	return bResult;
}

//============================================================================
//! test to verify can connect and use proxy session
bool PluginBaseRelay::onTestRequestRelayService(	RelayClientTestSession * poClientSession,
												VxNetIdent * netIdent )
{
	// send proxy request
	LogMsg( LOG_INFO, "onTestRequestRelayService sending proxy request\n");
	requestRelayService( netIdent, poClientSession, true );
	bool bResult = poClientSession->waitForTestSemaphore( 1000 );
	if( true == bResult )
	{
		LogMsg( LOG_INFO, "onTestRequestRelayService requestRelayService SUCCESS\n");
		LogMsg( LOG_INFO, "onTestRequestRelayService sending Test Pkt\n");
		// if proxy request is ok sent test packet
		PktRelayTestReq oPkt;
		oPkt.setLclSessionId( poClientSession->getLclSessionId() );
		oPkt.setRmtSessionId( poClientSession->getRmtSessionId() );
		poClientSession->proxySendPkt( &oPkt, netIdent->getMyOnlineId() );
		bResult = poClientSession->waitForTestSemaphore( 1000 );
		if( false == bResult )
		{
			LogMsg( LOG_INFO, "onTestRequestRelayService FAIL.. test packet\n");
		}
		else
		{
			LogMsg( LOG_INFO, "onTestRequestRelayService SUCCESS\n");

		}
	}
	else
	{
		LogMsg( LOG_INFO, "onTestRequestRelayService requestRelayService FAILED (timeout)\n");
	}


	// test packet returns with same identity id then the test success

	return bResult;
}
//============================================================================
//! test to verify can be used as proxy service
bool PluginBaseRelay::onTestAcceptRelayServiceRequest( VxNetIdent * netIdent )
{
	// send proxy request
	// if proxy request is ok sent test packet
	// test packet returns with same identity id then the test success

	return false;
}
//============================================================================
//! test can use current proxy session to connect to another user
bool PluginBaseRelay::onTestConnectUsingLclRelay( RelaySession * poSession, VxNetIdent * poIdentRelay )
{
	// send proxy request
	// if proxy request is ok sent test packet
	// test packet returns the test success

	return false;
}

//============================================================================
//! test can use normal connection to connect to remote proxy which is connected to user 
bool PluginBaseRelay::onTestConnectUsingRmtRelay( VxNetIdent * netIdent )
{

	return false;
}
