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

#include "NetworkStateAvail.h"
#include "NetworkStateMachine.h"
#include "NetworkMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/P2PEngine/EngineSettings.h>
#include <ptop_src/ptop_engine_src/UrlMgr/UrlMgr.h>

#include <NetLib/VxPeerMgr.h>

#include <CoreLib/VxTime.h>
#include <CoreLib/VxParse.h>

#include <stdio.h>

//============================================================================
NetworkStateAvail::NetworkStateAvail( NetworkStateMachine& stateMachine )
: NetworkStateBase( stateMachine )
, m_DirectConnectTester( stateMachine.getDirectConnectTester() )
{
	setNetworkStateType( eNetworkStateTypeAvail );
}

//============================================================================
void NetworkStateAvail::enterNetworkState( void )
{
    if( IsLogEnabled( eLogNetworkState ) )
    {
        LogMsg( LOG_STATUS, "eNetworkStateTypeAvail start" );
    }

	m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeAvail );
}

//============================================================================
void NetworkStateAvail::runNetworkState( void )
{
    VxTimer availTimer;
    LogModule( eLogNetworkState, LOG_VERBOSE, "111 NetworkStateAvail::runNetworkState start" ); 

	// wait for log on if need be
	while( ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		&& ( false == m_NetworkStateMachine.isUserLoggedOn() ) )
	{
		VxSleep( 500 );
	}

	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

    std::string netServiceUrl;
    m_Engine.getEngineSettings().getConnectTestUrl( netServiceUrl );
    netServiceUrl = m_Engine.getUrlMgr().resolveUrl( netServiceUrl );

	// NOTE: it seems that while upnp is communicating with router the router may temporarily stop accepting incoming connections
	// so startup order has been changed. 
	// 1. test if port is open
	// 2. if open move to online
	// 3. if closed start upnp open port and move to relay search but retest for port open before accepting relay connection

	//// tell Upnp to open port if possible
	//m_NetworkStateMachine.startUpnpOpenPort();
 //   if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
 //   {
 //       return;
 //   }
    //if( IsLogEnabled( eLogNetworkState ) )
    //{
	//LogMsg( LOG_INFO, "111 NetworkStateAvail::runNetworkState resolving urls time %3.3f\n", availTimer.elapsedSec() );
    //}

    bool websitesResolved = m_NetworkStateMachine.resolveWebsiteUrls();
	if( false == websitesResolved )
	{
		m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeFailedResolveHostNetwork, "" );
	}
    else
    {
        m_Engine.getNetStatusAccum().setWebsiteUrlsResolved( true );
    }

    if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
    {
        return;
    }

    EFirewallTestType firewallTestType = m_Engine.getEngineSettings().getFirewallTestSetting();
	if( eFirewallTestAssumeNoFirewall == firewallTestType )
	{
		std::string externIp = m_Engine.getEngineSettings().getUserSpecifiedExternIpAddr();
		
		InetAddress externAddr;
		externAddr.setIp( externIp.c_str() );
		if( false == externAddr.isValid() )
		{
            if( IsLogEnabled( eLogNetworkState ) )
            {
                LogMsg( LOG_STATUS, "eNetworkStateTypeLost Assume No Firewall Setting Must Specify An Valid External IP Address" );
                AppErr( eAppErrBadParameter, "Assume No Firewall Setting Must Specify An Valid External IP Address" );
            }

			m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeLost, "Assume No Firewall Setting Must Specify An Valid External IP Address" );
		}
		else
		{
			m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( externIp, false );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeOnlineDirect );
            if( IsLogEnabled( eLogNetworkState ) )
            {
                LogMsg( LOG_STATUS, "eNetworkStateTypeAvail Assume No Firewall User Extern IP %s", externIp.c_str() );
            }

			m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeAvail, externIp.c_str() );
			return;
		}
	}

	//LogMsg( LOG_INFO, "111 NetworkStateAvail::runNetworkState checking listen ready %3.3f\n", availTimer.elapsedSec() );
	int waitForListenCnt = 0;
	while( ( false == m_NetworkStateMachine.checkAndHandleNetworkEvents() )
		&& ( false == m_Engine.getPeerMgr().isReadyToAcceptConnections() ) )
	{
		VxSleep( 1000 );
		waitForListenCnt++;
		if( waitForListenCnt > 5 )
		{
            if( IsLogEnabled( eLogNetworkState ) )
            {
                LogMsg( LOG_ERROR, "NetworkStateAvail::runNetworkState timed out waiting for isReadyToAcceptConnections" );
                char timeoutMsg[128];
                sprintf( timeoutMsg, "ERROR Timeout waiting for listen port %d", m_PktAnn.getOnlinePort() );
                m_Engine.getToGui().toGuiStatusMessage( timeoutMsg );
            }

			break;
		}
	}

    if( IsLogEnabled( eLogNetworkState ) )
    {
	    LogMsg( LOG_INFO, "Notify GUI Starting Direct connect Test %3.3f", availTimer.elapsedSec() );            
        m_Engine.getToGui().toGuiStatusMessage( "#Network Testing if port is open" );
    }

    // wait for listen port time to open
    bool shouldAbort = false;
    int64_t	timeStart = GetGmtTimeMs();
    int64_t	timeEnd = timeStart;
    while( TimeElapsedMs( timeStart, timeEnd ) < 3500 )
    {
        if( m_NetworkStateMachine.shouldAbort() ) // TODO Add || m_NetworkStateMachine.networkIsLost() )
        {
            shouldAbort = true;
            break;
        }

        if( m_Engine.getPeerMgr().isListening() )
        {
            // already listening
            LogMsg( LOG_INFO, "Listening on port %d at time %3.3f", m_Engine.getPeerMgr().getListenPort(), availTimer.elapsedSec() );
            m_Engine.getToGui().toGuiStatusMessage( "#Network Testing listen port is open" );
            break;
        }

        LogMsg( LOG_INFO, "Waiting for listen port %d to open at sec %3.3f", m_Engine.getPeerMgr().getListenPort(), availTimer.elapsedSec() );
        m_Engine.getToGui().toGuiStatusMessage( "#Network Testing waiting for our listen port to open" );
        VxSleep( 1000 );
        timeEnd = GetGmtTimeMs();
    }

    if( shouldAbort )
    {
        return;
    }

    if( m_Engine.getNetStatusAccum().isDirectConnectTested() )
    {
        // all done.. move to next state
        if( m_Engine.getMyPktAnnounce().canDirectConnectToUser() )
        {
            LogModule( eLogNetworkState, LOG_VERBOSE, "Network State Avail already tested.. move to direct connect" );
            m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeOnlineDirect );
        }
        else
        {
            LogModule( eLogNetworkState, LOG_VERBOSE, "Network State Avail already tested.. move to wait for relay" );
            m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeWaitForRelay );
        }

        VxSleep( 500 );
        return;
    }

    LogModule( eLogNetworkState, LOG_VERBOSE, "Network State Avail Starting Direct connect Test %3.3f thread %d", availTimer.elapsedSec(), VxGetCurrentThreadId() );

    // wait for test result or timeout
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

    if( eNetCmdErrorNone == directConnectTestResults.m_eNetCmdErr )
    {
        m_Engine.getNetStatusAccum().setDirectConnectTested( true, false, directConnectTestResults.m_MyIpAddr );
    }
    else if( eNetCmdErrorPortIsClosed == directConnectTestResults.m_eNetCmdErr )
    {
        m_Engine.getNetStatusAccum().setDirectConnectTested( true, true, directConnectTestResults.m_MyIpAddr );
    }
    else
    {
        LogModule( eLogNetworkState, LOG_STATUS, "eNetworkStateTypeAvail %s Connection Test Server %s  %3.3f", 
            DescribeNetCmdError( directConnectTestResults.m_eNetCmdErr ), netServiceUrl.c_str(), availTimer.elapsedSec() );

        if( IsLogEnabled( eLogNetworkState ) )
        {
            if( eFirewallTestUrlConnectionTest == firewallTestType )
            {
                if( eNetCmdErrorResponseTimedOut == directConnectTestResults.m_eNetCmdErr )
                {
                    AppErr( eAppErrFailedConnectNetServices, "Connection test node failed to respond.\nPlease check connection test settings" );
                }
                else
                {
                    AppErr( eAppErrFailedConnectNetServices, "Could not connect to connection test node.\nPlease check connection test settings" );
                }
            }
            else
            {
                // TODO handle failed to connect and other errors
            }
        }
    }

	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	InetAddress externAddr;
	externAddr.setIp( directConnectTestResults.m_MyIpAddr.c_str() );
    if( false == externAddr.isValid() )
	{
        if( IsLogEnabled( eLogNetworkState ) )
        {
            AppErr( eAppErrBadParameter, "Could not determine external IP." );
        }
	}
    else
    {
        m_Engine.getNetStatusAccum().setIpAddress( directConnectTestResults.m_MyIpAddr );
    }

    bool canDirectConnect = false;
    bool testConnectHadError = false;
	if( ( false == directConnectTestResults.getCanDirectConnect() )
		&& m_Engine.getEngineSettings().getUseUpnpPortForward() )
	{
        LogModule( eLogNetworkState, LOG_INFO, "NetworkStateAvail::runNetworkState Starting UPNP %3.3f", availTimer.elapsedSec() );

		m_NetworkStateMachine.startUpnpOpenPort();
	}
	else
	{
        LogModule( eLogNetworkState, LOG_STATUS, "eNetworkStateTypeAvail extern ip %s", directConnectTestResults.m_MyIpAddr.c_str() );

        m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeAvail, directConnectTestResults.m_MyIpAddr.c_str() );
	}

	if( m_NetworkStateMachine.checkAndHandleNetworkEvents() )
	{
		return;
	}

	if( m_DirectConnectTester.isTestResultCanDirectConnect() )
	{
        // port is open
		std::string availMsg;
		StdStringFormat( availMsg, "IP %s", directConnectTestResults.m_MyIpAddr.c_str() );
        LogModule( eLogNetworkState, LOG_STATUS, "eNetworkStateTypeAvail %s %3.3f", availMsg.c_str(), availTimer.elapsedSec() );

        m_Engine.getToGui().toGuiNetworkState( eNetworkStateTypeAvail, availMsg.c_str() );
		
		if( eFirewallTestAssumeFirewalled == firewallTestType )
		{
            LogModule( eLogNetworkState, LOG_STATUS, "NetworkStateAvail::runNetworkState eFirewallTestAssumeFirewalled %3.3f", availTimer.elapsedSec() );
            m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( directConnectTestResults.m_MyIpAddr, true );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeWaitForRelay );
		}
		else
		{
            LogModule( eLogNetworkState, LOG_STATUS, "NetworkStateAvail::runNetworkState announce with direct connect %3.3f", availTimer.elapsedSec() );
            m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( directConnectTestResults.m_MyIpAddr, false );
			m_Engine.getToGui().toGuiUpdateMyIdent( &m_PktAnn );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeOnlineDirect );
            canDirectConnect = true;
		}
	}
	else
	{
        // port is closed
		if( eFirewallTestUrlConnectionTest == firewallTestType )
		{
            LogModule( eLogNetworkState, LOG_STATUS, "NetworkStateAvail::runNetworkState relay search %3.3f", availTimer.elapsedSec() );
            m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( directConnectTestResults.m_MyIpAddr, true );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeWaitForRelay );
		}
		else if( eFirewallTestAssumeNoFirewall == firewallTestType )
		{
            LogModule( eLogNetworkState, LOG_STATUS, "NetworkStateAvail::runNetworkState assume no firewall %3.3f", availTimer.elapsedSec() );
            m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( directConnectTestResults.m_MyIpAddr, false );
			m_Engine.getToGui().toGuiUpdateMyIdent( &m_PktAnn );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeOnlineDirect );
            canDirectConnect = true;
		}
		else if( eFirewallTestAssumeFirewalled == firewallTestType )
		{
            LogModule( eLogNetworkState, LOG_STATUS, "NetworkStateAvail::runNetworkState assume firewall %3.3f", availTimer.elapsedSec() );
            m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( directConnectTestResults.m_MyIpAddr, true );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeWaitForRelay );
		}
		else
		{
            LogModule( eLogNetworkState, LOG_ERROR, "ERROR invalid firewall test type %d  %3.3f", firewallTestType, availTimer.elapsedSec() );
			m_NetworkStateMachine.setPktAnnounceWithCanDirectConnect( directConnectTestResults.m_MyIpAddr, true );
			m_NetworkStateMachine.changeNetworkState( eNetworkStateTypeTestConnection );
            testConnectHadError = true;
		}
	}

    m_Engine.getNetStatusAccum().setNearbyAvailable( true );

    m_NetworkStateMachine.getNetworkMgr().getNearbyMgr().onPktAnnUpdated();

    /*
    if( !testConnectHadError )
    {
        //LogModule( eLogNetworkState,  LOG_INFO, "111 NetworkStateAvail::runNetworkState done  %3.3f\n", availTimer.elapsedSec() );
        // setup our hosting services
        //m_Engine.getMyHostSrvMgr().enableHostedServices( true, canDirectConnect );

        // setup connect to other hosting services
        m_Engine.getOtherHostSrvMgr().enableHostedServices( true, canDirectConnect );

    }
    */
}

