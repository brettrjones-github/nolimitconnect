//============================================================================
// Copyright (C) 2014 Brett R. Jones
// All Rights Reserved ( After My Death I Brett R. Jones Issue this source code to Public Domain )
//
// You may not modify or redistribute this code for any reason
// without written consent of Brett R. Jones
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brettjones1900@gmail.com
// http://www.p2panarchy.com
//============================================================================

#include "UpnpPortForward.h"
#include "UpnpParser.h"
#include "UpnpRootDevice.h"

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxParse.h>

#include <NetLib/VxSktUtil.h>
#include <NetLib/VxParseUrlComponents.h>

#include <time.h>
#include <string.h>
#include <atomic>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

namespace
{
	const uint16_t		UPNP_DISCOVERY_PORT			= 1900;
	const char*	UPNP_GROUP_IP				= "239.255.255.250";

	//============================================================================
	void SSPDSearchResponseSktCallbackHandler( VxSktBase *  sktBase, void * pvUserData )
	{
		if( eSktCallbackReasonData == sktBase->getCallbackReason() )
		{
			//LogMsg( LOG_INFO, "SSPDSearchResponseSktCallbackHandler\n" );
			UpnpPortForward * poClient = (UpnpPortForward *)pvUserData;
			if( poClient )
			{
				poClient->doSearchResponseCallback( sktBase );
			}
		}
	}

	//============================================================================
	int32_t UpnpPortForwardThreadFunc(  void * pvContext )
	{
        static int threadCnt = 0;
        threadCnt++;
#ifdef DEBUG_THREADS
        LogMsg( LOG_INFO, "UpnpPortForwardThreadFunc thread starting %d\n", threadCnt );
#endif // DEBUG_THREADS

		VxThread* poThread = (VxThread*)pvContext;
		poThread->setIsThreadRunning( true );

		UpnpPortForward * poMgr = (UpnpPortForward *)poThread->getThreadUserParam();
        if( poMgr )
        {
            poMgr->runPortForward();
        }

#ifdef DEBUG_THREADS
        LogMsg( LOG_DEBUG, "UpnpPortForward thread about to exit\n" );
#endif // DEBUG_THREADS
		poThread->threadAboutToExit();
        threadCnt--;
#ifdef DEBUG_THREADS
        LogMsg( LOG_INFO, "UpnpPortForwardThreadFunc thread exiting %d\n", threadCnt );
#endif // DEBUG_THREADS
		return 0;
	}

	VxTimer g_ElapsedTimeBetweenUpnpRequests;
}

//============================================================================
UpnpPortForward::UpnpPortForward()
: m_iPortToForward( 0 )
, m_strServiceToForward( "" )
, m_strLocalIPv4( "" )
, m_pfuncPortForwardCallbackHandler( 0 )
, m_PortForwardCallbackUserData( 0 )
, m_UpnpSearchResponseSocket()
{
	m_UpnpSearchResponseSocket.setReceiveCallback( SSPDSearchResponseSktCallbackHandler, this );
}

//============================================================================
UpnpPortForward::~UpnpPortForward()
{
}

//============================================================================
void UpnpPortForward::setPortForwardResultCallback( PORT_FORWARD_CALLBACK_FUNCTION pfuncPortForwardCallbackHandler, void * userData )
{
	m_pfuncPortForwardCallbackHandler = pfuncPortForwardCallbackHandler;
	m_PortForwardCallbackUserData = userData;
}

//============================================================================
void UpnpPortForward::deviceAddPortMapSuccessFailCallback( bool bResult )
{
	if( m_pfuncPortForwardCallbackHandler )
	{
		m_pfuncPortForwardCallbackHandler( m_PortForwardCallbackUserData, bResult ? eAppErrNone : eAppErrFailedToConnect );
	}
}

//============================================================================
void UpnpPortForward::tcpPortForward( int port, const char* pServiceName, const char* pLocalIPv4  )
{
	//if( g_ElapsedTimeBetweenUpnpRequests.elapsedMs() < 30000 )
	//{
	//	// don't allow requests to often or confuses router
	//	return;
	//}

	g_ElapsedTimeBetweenUpnpRequests.startTimer();
	m_PortForwardThread.abortThreadRun( true );
	while( m_PortForwardThread.isThreadRunning() )
	{
		LogMsg( LOG_INFO, "UpnpPortForward::startup waiting for old thread to die\n" );
		VxSleep( 200 );
	}

	upnpPortForwardShutdown();
	m_iPortToForward		= port;
	m_strServiceToForward	= pServiceName;
	if( pLocalIPv4 )
	{
		m_strLocalIPv4			= pLocalIPv4;
	}
	else
	{
		m_strLocalIPv4			= "0.0.0.0";
	}

	m_LocalIpAddr.setIp( m_strLocalIPv4.c_str() );
#ifdef DEBUG_UPNP
	LogMsg( LOG_DEBUG, "UpnpPortForward::tcpPortForward: port %d ip %s service %s\n", port, pLocalIPv4, pServiceName );
#endif// DEBUG_UPNP
	m_PortForwardThread.startThread( (VX_THREAD_FUNCTION_T)UpnpPortForwardThreadFunc, this, "UpnpPortForwardThrd" );
}

//============================================================================
void UpnpPortForward::upnpPortForwardShutdown( void )
{
	m_UpnpSearchResponseSocket.closeSkt(eSktCloseUpnpDone);
	m_PortForwardThread.abortThreadRun( true );
	m_PortForwardThread.killThread();
	removeAllRootDevices();
	m_DeviceInfoUrls.clear();
	m_SearchResponseUrls.clear();
}

//============================================================================
void UpnpPortForward::removeAllRootDevices( void )
{
	std::vector<UpnpRootDevice *>::iterator iter;
	for( iter = m_RootDeviceList.begin(); iter != m_RootDeviceList.end(); ++iter )
	{
		delete (*iter);
	}

	m_RootDeviceList.clear();
}

//============================================================================
bool UpnpPortForward::shouldAbort( void )
{
	return m_PortForwardThread.isAborted() || VxIsAppShuttingDown();
}

//============================================================================
void UpnpPortForward::runPortForward( void )
{
	m_UpnpSearchResponseSocket.closeSkt( eSktCloseUpnpStart );
	m_UpnpSearchResponseSocket.udpOpen( m_LocalIpAddr, 0 );
	m_UpnpSearchResponseSocket.joinMulticastGroup( m_LocalIpAddr, UPNP_GROUP_IP );

	//m_VxNat.ForwardTcpPort( m_LocalIpAddr, getPortToMap(), m_ClientMgr->getServiceToForward() )

	// wait 6 seconds or thread abort whichever comes first then close
	for( int i = 0; i < 30 ; i++ )
	{
		if( m_PortForwardThread.isAborted() )
		{
			break;
		}

		if( ( i < 7 )
			&& ( 0 == (i % 3 ) ) )
		{
			// every 2 seconds send broadcast
			sendUpnpDeviceSearchBroadcast();
		}

		VxSleep( 500 );
	}

	m_UpnpSearchResponseSocket.closeSkt( eSktCloseUpnpDone );
}

//============================================================================
void UpnpPortForward::sendUpnpDeviceSearchBroadcast( void )
{
	if( shouldAbort() )
	{
		return;
	}

	static const char searchGatewayText[] = 
		"M-SEARCH * HTTP/1.1\r\n"
		"HOST:239.255.255.250:1900\r\n"
		"ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n"
		"MAN:\"ssdp:discover\"\r\n"
		"MX:3\r\n\r\n";


	if( shouldAbort() )
	{
		return;
	}

	RCODE rc = m_UpnpSearchResponseSocket.sendTo( searchGatewayText, sizeof( searchGatewayText ) - 1, UPNP_GROUP_IP, UPNP_DISCOVERY_PORT );
	if( rc )
	{
		LogMsg( LOG_ERROR, "sendUpnpDeviceSearchBroadcast error %d \n", rc );
	}		
}

//============================================================================
void UpnpPortForward::doSearchResponseCallback( VxSktBase* sktBase )
{
	if( shouldAbort() )
	{
		return;
	}

	if( eSktCallbackReasonData == sktBase->getCallbackReason() )
	{
		m_PortForwardMutex.lock();
		int iBufLen = sktBase->getSktBufDataLen();
		while( 0 < iBufLen )
		{
			int iDataLen = 0;
			char * pData = (char *)sktBase->getSktReadBuf();
			char * pEnd = strstr( pData, "\r\n\r\n");
			if( NULL != pEnd )
			{
				pEnd += 4;
				if( ( '\r' == pEnd[0] )
					&& ( '\n' == pEnd[1] ) )
				{
					// trim extra crlf
					pEnd += 2;
				}

				//LogMsg( LOG_INFO, "SSPDClient::doNotifyCallback %s\n", pData );
				iDataLen = ( pEnd - pData );
				UpnpParser upnpParser;
				upnpParser.setParserData( sktBase, pData );
				onSearchResponseReceived( &upnpParser );
				sktBase->sktBufAmountRead( iDataLen );
				iBufLen = sktBase->getSktBufDataLen();
			}
			else
			{
				sktBase->sktBufAmountRead( 0 );
				break;
			}
		}

		m_PortForwardMutex.unlock();
	}
}

//============================================================================
void UpnpPortForward::onSearchResponseReceived( UpnpParser * upnpParser )
{
	if( shouldAbort() )
	{
		return;
	}

// response should look something like
//HTTP/1.1 200 OK\r\n
//ST:urn:schemas-upnp-org:device:InternetGatewayDevice:1\r\n
//LOCATION:http://192.168.1.1:5000/Public_UPNP_gatedesc.xml\r\n
//SERVER:Linux/2....
	if( ( 200 != upnpParser->getHttpCodeNumber() ) &&
		( upnpParser->getMethod() != "notify" ) )
	{
#ifdef DEBUG_UPNP
		LogMsg( LOG_DEBUG, "UpnpPortForward::onSearchResponseReceived 200 != http %s\n", upnpParser->getStreamData() );
#endif // DEBUG_UPNP
		return;
	}

	std::string strUrl;
	upnpParser->getLocation( strUrl );
	if( strUrl.empty() )
	{
#ifdef DEBUG_UPNP
		LogMsg( LOG_DEBUG, "UpnpPortForward::onSearchResponseReceived empty url %s\n", upnpParser->getStreamData() );
#endif // DEBUG_UPNP
		return;
	}

	if( haveAlreadyRequestedDeviceInfo( strUrl ) )
	{
#ifdef DEBUG_UPNP
		LogMsg( LOG_DEBUG, "UPNP already requested info %s\n", strUrl.c_str() );
#endif // DEBUG_UPNP
		return;
	}

	m_SearchResponseUrls.push_back( strUrl );
#ifdef DEBUG_UPNP
	LogMsg( LOG_DEBUG, "UPNP discovered device %s\n", strUrl.c_str() );
#endif // DEBUG_UPNP
	//if( NULL == strstr( strUrl.c_str(), "gatedesc"))
	//{
	//	return;
	//}

	//if( 50 < m_RootDeviceList.size() )
	//{
	//	LogMsg( LOG_ERROR, "Too Many Root Devices\n" );
	//	return;
	//}

	//m_ClientMgr->statusMsg( "UPNP", "discovered device %s\n", strUrl.c_str() );

	UpnpRootDevice * pDevice = new UpnpRootDevice( *this, strUrl );

	std::string		strAuth;		// user:pass
	std::string		strProtocol;	// http or https

	if( false == VxParseUrlComponents(	strUrl, 
		pDevice->m_strHostName,
		strAuth,
		strProtocol,
		pDevice->m_iPort,
		pDevice->m_strPath ) )
	{
		LogMsg( LOG_ERROR, "UPNP: FAILED PARSE %s\n", strUrl.c_str() );
		delete pDevice;
		return;
	}

	if( strProtocol != "http" )
	{
		LogMsg( LOG_ERROR, "UPNP: NOT HTTP %s\n", strUrl.c_str() );
		delete pDevice;
		return;
	}

	if( 0 == pDevice->m_strHostName.length() )
	{
		LogMsg( LOG_ERROR, "UPNP: NO HOST NAME %s\n", strUrl.c_str() );
		delete pDevice;
		return;
	}

	if( 0 == pDevice->m_strPath.length() )
	{
		LogMsg( LOG_ERROR, "UPNP: NO PATH %s\n", strUrl.c_str() );
		delete pDevice;
		return;
	}

	if( 0 == pDevice->m_iPort )
	{
		LogMsg( LOG_ERROR, "UPNP: ZERO PORT %s\n", strUrl.c_str() );
		delete pDevice;
		return;
	}

	if( shouldAbort() )
	{
		LogMsg( LOG_ERROR, "UPNP: ABORT %s\n", strUrl.c_str() );
		delete pDevice;
		return;
	}

	m_RootDeviceList.push_back( pDevice );
	pDevice->startup();
}

//============================================================================
bool UpnpPortForward::haveAlreadyRequestedDeviceInfo( std::string& deviceUrl )
{
	std::vector<std::string>::iterator iter;
	for( iter = m_SearchResponseUrls.begin(); iter != m_SearchResponseUrls.end(); ++iter )
	{
		if( *iter == deviceUrl )
		{
			return true;
		}
	}

	return false;
}
