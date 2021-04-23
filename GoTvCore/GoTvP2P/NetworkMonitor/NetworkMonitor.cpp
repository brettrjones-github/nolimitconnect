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

#include "NetworkMonitor.h"

#include "../P2PEngine/P2PEngine.h"

#include <CoreLib/VxGlobals.h>
#include <NetLib/InetAddress.h>
#include <NetLib/VxSktUtil.h>
#include <NetLib/VxSktConnectSimple.h>


#include <vector>
#include <time.h>

namespace
{
    const int NET_MONITOR_CHECK_INTERVAL_SEC = 5;
    const int NET_MONITOR_CONNECT_TO_HOST_TIMOUT_MS = 8000;

    const int NET_INTERNET_ATTEMPT_CONNECT_TIMEOUT_MS = 10000;
    const int NET_INTERNET_VERIFY_ACITIVE_TIMEOUT_MS = 55000;

    const char * NET_TEST_WEB_CONNECTION_HOST = "www.google.com";

    //============================================================================
    static void * NetworkMonitorThreadFunc( void * pvContext )
    {
        VxThread * poThread = ( VxThread * )pvContext;
        poThread->setIsThreadRunning( true );
        NetworkMonitor * netMonitor = ( NetworkMonitor * )poThread->getThreadUserParam();
        netMonitor->doNetworkConnectTestThread( poThread );
        poThread->threadAboutToExit();
        return nullptr;
    }
}

//============================================================================
NetworkMonitor::NetworkMonitor( P2PEngine& engine )
: m_Engine( engine )
, m_bIsStarted( false )
, m_iCheckInterval( NET_MONITOR_CHECK_INTERVAL_SEC )
, m_NetMonitorThread()
, m_NetSemaphore()
{
}

//============================================================================
void NetworkMonitor::networkMonitorStartup( const char * preferredNetIp, const char * cellNetIp )
{
    m_strPreferredAdapterIp	= preferredNetIp;
    m_strCellNetIp			= cellNetIp;
    m_bIsStarted			= true;
    LogModule( eLogStartup, LOG_INFO, "networkMonitorStartup preferred ip %s cell ip %s", m_strPreferredAdapterIp.c_str(), m_strCellNetIp.c_str() );
}

//============================================================================
void NetworkMonitor::networkMonitorShutdown( void )
{
    m_NetMonitorThread.abortThreadRun( true );
    m_bIsStarted		= false;
    m_strPreferredAdapterIp	= "";
    m_strCellNetIp		= "";
    m_NetMonitorThread.killThread();
    //m_Engine.fromGuiNetworkLost();
}

//============================================================================
void NetworkMonitor::setIsInternetAvailable( bool isAvail )
{
    m_InternetAvailable = isAvail;
    m_Engine.getNetStatusAccum().setInternetAvail( isAvail );
}

//============================================================================
void NetworkMonitor::onOncePerSecond( void )
{
    if( ( false == m_bIsStarted )
        || VxIsAppShuttingDown() )
    {
        return;
    }

    if( 0 != m_iCheckInterval && NET_MONITOR_CHECK_INTERVAL_SEC >= m_iCheckInterval )
    {
        m_iCheckInterval++;
        return;
    }

    m_iCheckInterval = 1;

    bool assumeFixedIp = m_Engine.getHasFixedIpAddress();
    if( assumeFixedIp )
    {
        std::string externIp;
        m_Engine.getEngineSettings().getUserSpecifiedExternIpAddr( externIp );
        if( externIp != m_LastConnectedLclIp )
        {
            m_LastConnectedLclIp = externIp;
            LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond new fixed ip addr %s", externIp.c_str() );

            setIsInternetAvailable( true );
            m_Engine.fromGuiNetworkAvailable( externIp.c_str(), false );
        }

        if( m_Engine.getHasHostService( eHostServiceNetworkHost ) )
        {
            // also assume we can connect to network host because we are network host
            m_Engine.getNetStatusAccum().setNetHostAvail( true );
        }

        return;
    }

    if( m_Engine.getHasHostService( eHostServiceNetworkHost ) )
    {
        // assume we can connect to network host because we are network host
        m_Engine.getNetStatusAccum().setNetHostAvail( true );
    }

    if( m_NetMonitorThread.isThreadRunning() )
    {
        // still trying to get ip from connection.. wait till next time
        return;
    }

    bool foundSameAsLastIp	= false;
    bool foundCellIp		= false;
    bool foundPreferredAdapterIp	= false;
    std::vector<std::string> pickAddresss;

    //LogMsg( LOG_INFO, "NetworkStateMachine::onOncePerSecond\n" );
    m_strPreferredAdapterIp = m_Engine.getEngineSettings().getPreferredNetworkAdapterIp();

    std::vector<InetAddress> aipAddresses;
    InetAddress	netAddr;
    netAddr.getAllAddresses( aipAddresses );

    for( InetAddress& inetAddr : aipAddresses )
    {
        if( ( false == inetAddr.isIPv4() )
            || inetAddr.isLoopBack() )
        {
            continue;
        }

        std::string strIp = inetAddr.toStdString();
        pickAddresss.push_back( strIp );
        if( strIp == m_strLastFoundIp )
        {
            foundSameAsLastIp = true;
            break;
        }

        if( strIp == m_strCellNetIp )
        {
            foundCellIp = true;
        }

        if( strIp == m_strPreferredAdapterIp )
        {
            foundPreferredAdapterIp = true;
        }
    }

    if( foundSameAsLastIp && getIsInternetAvailable() )
    {
        // LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond foundSameAsLastIp" );
        return;
    }

    if( foundPreferredAdapterIp )
    {
        m_strLastFoundIp = m_strPreferredAdapterIp;
        LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond foundPreferredAdapterIp %s", m_strLastFoundIp.c_str() );
        setIsInternetAvailable( true );
        m_Engine.fromGuiNetworkAvailable( m_strLastFoundIp.c_str(), false );
        return;
    }

    if( foundCellIp )
    {
        m_strLastFoundIp = m_strCellNetIp;
        LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond foundCellIp %s", m_strLastFoundIp.c_str() );
        setIsInternetAvailable( true );
        m_Engine.fromGuiNetworkAvailable( m_strLastFoundIp.c_str(), true );
        return;
    }

    int64_t timeNow = GetTimeStampMs();

    if( m_ConnectAttemptCompleted )
    {
        m_ConnectAttemptCompleted = false;
        if( m_ConnectAttemptSucessfull && !m_ConnectedLclIp.empty() )
        {
            if( m_LastConnectAttemptSuccessfull && ( m_LastConnectedLclIp == m_ConnectedLclIp ) )
            {
                // same as last time connection test was executed
                LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond same as last ip %s", m_ConnectedLclIp.c_str() );
            }
            else
            {
                m_LastConnectedLclIp = m_ConnectedLclIp;
                LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond new ip %s", m_ConnectedLclIp.c_str() );
                setIsInternetAvailable( true );
                m_Engine.fromGuiNetworkAvailable( m_ConnectedLclIp.c_str(), false );
             }
        }
        else
        {
            // failed .. if the last attempt was successfull let it try one more time
            if( !m_LastConnectAttemptSuccessfull )
            {
                LogModule( eLogNetworkState, LOG_INFO, " NetworkMonitor::onOncePerSecond network lost" );
                m_Engine.getNetStatusAccum().setNetHostAvail( false );
                setIsInternetAvailable( false );
                m_Engine.fromGuiNetworkLost();
            }
        }

        m_LastConnectAttemptSuccessfull = m_ConnectAttemptSucessfull;
    }
    else if( ( !getIsInternetAvailable() && ( timeNow - m_LastConnectAttemptTimeGmtMs ) > NET_INTERNET_ATTEMPT_CONNECT_TIMEOUT_MS ) || // time to attempt internet connect/verifiy local ip
        ( getIsInternetAvailable() && ( timeNow - m_LastConnectAttemptTimeGmtMs ) > NET_INTERNET_VERIFY_ACITIVE_TIMEOUT_MS ) ) // time to verify internet still connected and what is local ip
    {
        // only reevaluate network every 10 seconds
        m_LastConnectAttemptTimeGmtMs = timeNow;
        m_ConnectAttemptCompleted = false;
        m_ConnectAttemptSucessfull = false;
        m_ConnectedLclIp = "";

        // for debug only
        // netAddr.dumpAddresses( aipAddresses );

        // start thread that will run ping/pong is port open test
        triggerDetermineIp();
    }
}

//============================================================================
void NetworkMonitor::triggerDetermineIp( void )
{
    // connection test must be done in thread of may hang the gui thread for many seconds
    if( !m_NetMonitorThread.isThreadRunning() )
    {
        m_NetMonitorThread.startThread( ( VX_THREAD_FUNCTION_T )NetworkMonitorThreadFunc, this, "NetMonitorThread" );
    }
}

//============================================================================
void NetworkMonitor::doNetworkConnectTestThread( VxThread * startupThread )
{
    if( startupThread )
    {
        m_ConnectedLclIp = determineLocalIp();
        m_ConnectAttemptSucessfull = !m_ConnectedLclIp.empty();
        m_ConnectAttemptCompleted = true;
    }
}

//============================================================================
std::string NetworkMonitor::determineLocalIp( void )
{
    std::string localIp;
    std::string externIp;
    m_Engine.getEngineSettings().setUserSpecifiedExternIpAddr( externIp );

    VxSktConnectSimple sktConnect;
    static int connectAttemptCnt = 0;
    connectAttemptCnt++;

    // only attempt connect to network host if we are not the host
    if( !m_Engine.getHasHostService( eHostServiceNetworkHost )
        || ( VxIsIPv4Address( VxGetNetworkHostName() ) && !( !externIp.empty() && ( externIp == VxGetNetworkHostName() ) ) ) )
    {
        SOCKET skt = sktConnect.connectTo( VxGetNetworkHostName(),		// remote ip or url
                                           VxGetNetworkHostPort(),		// port to connect to
                                           NET_MONITOR_CONNECT_TO_HOST_TIMOUT_MS );	// timeout attempt to connect
        if( INVALID_SOCKET != skt )
        {
            connectAttemptCnt = 0;
            m_Engine.getNetStatusAccum().setInternetAvail( true );
            // get local address
            InetAddrAndPort lclAddr;
            if( 0 == VxGetLclAddress( skt, lclAddr ) )
            {
                localIp = lclAddr.toStdString();
                if( localIp == "0.0.0.0" )
                {
                    LogModule( eLogNetworkState, LOG_INFO, "determineLocalIp sktConnect.connectTo invalid local ip" );
                    localIp = "";
                    m_Engine.getNetStatusAccum().setNetHostAvail( false );
                }
                else
                {
                    m_Engine.getNetStatusAccum().setNetHostAvail( true );
                }
            }
            else
            {
                m_Engine.getNetStatusAccum().setNetHostAvail( true );
            }

            VxCloseSkt( skt );
        }
        else
        {
            m_Engine.getNetStatusAccum().setNetHostAvail( false );
        }
    }

    if( localIp.empty() )
    {
        LogModule( eLogNetworkState, LOG_WARNING, "Failed verify No Limit Hosted internet conection to %s:%d", VxGetNetworkHostName(), VxGetNetworkHostPort() );

        // try using google.. a really bad idea but since we have no official site we can connect to this should be reliable
        SOCKET skt = sktConnect.connectTo( NET_TEST_WEB_CONNECTION_HOST,		// remote ip or url
                                           80,						// port to connect to
                                           NET_MONITOR_CONNECT_TO_HOST_TIMOUT_MS );	// timeout attempt to connect
        if( INVALID_SOCKET != skt )
        {
            connectAttemptCnt = 0;
            m_Engine.getNetStatusAccum().setInternetAvail( true );
            // get local address
            InetAddrAndPort lclAddr;
            if( 0 == VxGetLclAddress( skt, lclAddr ) )
            {
                localIp = lclAddr.toStdString();
                if( localIp == "0.0.0.0" )
                {
                    LogModule( eLogNetworkState, LOG_INFO, "determineLocalIp sktConnect.connectTo invalid local ip" );
                    localIp = "";
                }
            }

            VxCloseSkt( skt );
            /* NOT REQUIRED TO VERIFY LOCAL ADAPTER IP.. also while using VPN specifing local ip address causes issues
            skt = sktConnect2.connectTo( m_strLastFoundIp.c_str(),  // local adapter ip
                                         "www.google.com",		    // remote ip or url
                                         80,						// port to connect to
                                         NET_MONITOR_CONNECT_TO_HOST_TIMOUT_MS );					// timeout attempt to connect
            if( INVALID_SOCKET != skt )
            {
                VxCloseSkt( skt );
            }
            else
            {
                m_strLastFoundIp.clear();
            }
            */
        }

        if( localIp.empty() )
        {
            LogModule( eLogNetworkState, LOG_WARNING, "Failed verify internet conection to %s:%d", NET_TEST_WEB_CONNECTION_HOST, VxGetNetworkHostName(), 80 );
        }
    }

    if( connectAttemptCnt > 1 )
    {
        // if failed to even connect multiple times then mark internet unavailable
        m_Engine.getNetStatusAccum().setInternetAvail( false );
    }

    return localIp;
}


