//============================================================================
// Copyright (C) 2009 Brett R. Jones 
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

#include "VxSktUtil.h"
#include "VxSktAccept.h"
#include "VxServerMgr.h"
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxParse.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/AppErr.h>
#include <CoreLib/VxTime.h>
#include <CoreLib/VxThread.h>

#include <stdio.h>
#include <memory.h>
#include <time.h>

//#define DEBUG_VXSERVER_MGR
#define DISABLE_WATCHDOG 1

int VxServerMgr::m_iAcceptMgrCnt = 0;				// number of accept managers that have been created

//============================================================================
namespace
{
    void * VxServerMgrVxThreadFunc(  void * pvContext )
	{
        if( pvContext )
        {
            VxThread * poVxThread = (VxThread *)pvContext;
            poVxThread->setIsThreadRunning( true );
            VxServerMgr * poMgr = (VxServerMgr *)poVxThread->getThreadUserParam();
            if( poMgr )
            {
                LogModule( eLogListen, LOG_INFO, "#### VxServerMgr: Mgr id %d Listen port %d thread started thread 0x%x", poMgr->m_iMgrId, poMgr->getListenPort(), VxGetCurrentThreadId() );
                poMgr->listenForConnectionsToAccept( poVxThread );
                // quitting
                LogModule( eLogListen, LOG_INFO, "#### VxServerMgr: Mgr id %d Listen port %d thread 0x%x tid %d quiting\n", poMgr->m_iMgrId, poMgr->getListenPort(), VxGetCurrentThreadId(), poVxThread->getThreadTid() );
            }

            //! VxThread calls this just before exit
            poVxThread->threadAboutToExit();
        }
        return nullptr;
	}
}

//============================================================================
VxServerMgr::VxServerMgr()
: VxSktBaseMgr()
{
#if defined(TARGET_OS_ANDROID)
    m_IsAndroidOs = true;
#endif // defined(TARGET_OS_ANDROID)

	m_iAcceptMgrCnt++;
	m_iMgrId = m_iAcceptMgrCnt;
	m_eSktMgrType = eSktMgrTypeTcpAccept;
	memset( m_aoListenSkts, 0, sizeof( m_aoListenSkts ) );
}

//============================================================================
VxServerMgr::~VxServerMgr()
{
	stopListening();
}

//============================================================================
// overrides VxSktBaseMgr
void VxServerMgr::sktMgrShutdown( void )
{
	stopListening();
	VxSktBaseMgr::sktMgrShutdown();
}

//============================================================================
void VxServerMgr::fromGuiKickWatchdog( void )
{
	//LogMsg( LOG_SKT, "VxServerMgr: fromGuiKickWatchdog\n" );
	m_LastWatchdogKickMs = GetTimeStampMs();
}

//============================================================================
bool VxServerMgr::checkWatchdog( void )
{
	// there is an issue with android.. in some cases it can leave the listen socket open even though the
	// app is stopped or crashed. this watchdog is used to force the listen socket to close
	// and terminate the app if it is unresponsive.
#ifdef _DEBUG
	return true; // if we are debugging don't timeout because we are on a breakpoint
#endif 
#ifdef DISABLE_WATCHDOG
	return true; // if we are debugging don't timeout because we are on a breakpoint
#endif 

	if( ( GetTimeStampMs() - m_LastWatchdogKickMs ) < 4000 )
	{
		return true;
	}

	LogMsg( LOG_INFO, "Listen Watchdog Timeout\n" );
	return false;
}

//============================================================================
VxSktBase * VxServerMgr::makeNewAcceptSkt( void )				
{ 
	return new VxSktAccept(); 
}

//============================================================================
bool VxServerMgr::isListening( void )							
{ 
    return m_u16ListenPort && m_ListenVxThread.isThreadRunning() && m_aoListenSkts[ 0 ] != INVALID_SOCKET;
}

//============================================================================
bool VxServerMgr::startListening( uint16_t u16ListenPort, const char * ip )
{
    stopListening();

	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	if( 0 == u16ListenPort )
	{
		AppErr( eAppErrBadParameter, "VxServerMgr::startListening Bad param port %d", u16ListenPort );
		return false;
	}

	m_LastWatchdogKickMs = GetTimeStampMs();    

    std::string ipv4String("");
    if( ip && strlen(ip) )
    {
        ipv4String = ip;
    }

    // TODO should be able to bind to a specific adapter but seams to have many issues
    // Instead do the most generic listen socket
    LogModule( eLogListen, LOG_INFO, "### NOT IN THREAD VxServerMgr::startListening prefered ip %s port %d", ipv4String.c_str(), u16ListenPort );

    SOCKET listenSock = INVALID_SOCKET;
    std::string actualListenLclIp;
    if( createListenSocket( ipv4String, listenSock, u16ListenPort, actualListenLclIp ) )
    {
        // m_LclIp was set in call to createListenSocket
        // m_LclIp.setIp( actualListenLclIp.c_str() );
        // m_LclIp.setPort( u16ListenPort );

        LogModule( eLogListen, LOG_INFO, "VxServerMgr::startListening create listen skt %d success lcl ip %s port %d", listenSock, actualListenLclIp.c_str(), u16ListenPort );

        m_u16ListenPort = u16ListenPort;
        m_aoListenSkts[ m_iActiveListenSktCnt ] = listenSock;
        m_iActiveListenSktCnt++;
        if( 0 != internalStartListen() )
        {
            LogModule( eLogListen, LOG_ERROR, "ipv4 listen() internalStartListen failed" );
            closeListenSocket();
            return false;
        }
        
        return true;
    }
    else
    {
        LogModule( eLogListen, LOG_ERROR, "VxServerMgr::startListening create listen socket failed port %d for preferred ip %s", u16ListenPort, ipv4String.c_str() );
        return false;
    }

    // TODO should be able to bind to a specific adapter but it seams to have many cross platform issues
    return false;

    bool haveAdapterIp = !ipv4String.empty();
	m_u16ListenPort = u16ListenPort;

    LogModule( eLogListen, LOG_INFO, "### NOT IN THREAD VxServerMgr::startListening ip %s port %d app sec %d thread 0x%X", ip, u16ListenPort, GetApplicationAliveSec(), VxGetCurrentThreadId() );

    if( haveAdapterIp || m_IsAndroidOs )
    {
#if defined(TARGET_OS_WINDOWS) || defined(TARGET_OS_ANDROID)
        // can't get ip's in native android... for now just do ipv4 TODO listen for ipv6 in android
        // ipv4 only
        SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
        if( sock < 0 )
        {
            RCODE rc = VxGetLastError();
            LogMsg( LOG_ERROR, "VxServerMgr::startListening create skt error %d thread 0x%x", rc, VxGetCurrentThreadId() );

            if( 0 == rc )
            {
                rc = -1;
            }

            return false;
        }

        // don't know why reuse port doesn't work
        VxSetSktAllowReusePort( sock );

        struct sockaddr_in listenAddr;
        memset( &listenAddr, 0, sizeof( struct sockaddr_in ) );
        listenAddr.sin_family = AF_INET;
        listenAddr.sin_port = htons( u16ListenPort );
        listenAddr.sin_addr.s_addr = haveAdapterIp ? inet_addr( ipv4String.c_str() ) : INADDR_ANY;

        if( haveAdapterIp || m_IsAndroidOs)
        {
            bool bindSuccess = false;
            for( int tryCnt = 0; tryCnt < 5; tryCnt++ )
            {
                if( 0 > bind( sock, ( struct sockaddr * ) &listenAddr, sizeof( sockaddr_in ) ) )
                {
                    RCODE rc = VxGetLastError();
                    LogMsg( LOG_ERROR, "VxServerMgr::startListening bind skt %d error %d %s try cnt %d thread 0x%x", sock, rc, VxDescribeSktError( rc ), tryCnt + 1, VxGetCurrentThreadId() );
                    VxSleep( 1000 );
                    continue;
                }

                bindSuccess = true;
                break;
            }

            if( false == bindSuccess )
            {
                LogMsg( LOG_ERROR, "VxServerMgr::startListening bind skt %d FAILED thread 0x%x", sock, VxGetCurrentThreadId() );
                ::VxCloseSktNow( sock );
                return false;
            }
        }

        LogModule( eLogListen, LOG_INFO, "StartListen socket %d index %d ip %s port %d thread 0x%x", sock, m_iActiveListenSktCnt, ip, u16ListenPort, VxGetCurrentThreadId() );

        m_aoListenSkts[ m_iActiveListenSktCnt ] = sock;
        m_iActiveListenSktCnt++;
        m_LclIp.setIp( ip );
        m_LclIp.setPort( u16ListenPort );

        return internalStartListen();
#endif // TARGET_OS_WINDOWS
    }


#ifdef TARGET_OS_ANDROID
	// can't get ip's in native android... for now just do ipv4 TODO listen for ipv6 in android
	// ipv4 only
    SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock < 0 )
	{
		RCODE rc = VxGetLastError();
        LogMsg( LOG_ERROR, "VxServerMgr::startListening create skt error %d thread 0x%x", rc, VxGetCurrentThreadId() );

        if( 0 == rc )
        {
            rc = -1;
        }

        return false;
	}

	// don't know why reuse port doesn't work
	VxSetSktAllowReusePort( sock );

	struct sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof( struct sockaddr_in ) );
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_port = htons(u16ListenPort);
#if !USE_BIND_LOCAL_IP
    listenAddr.sin_addr.s_addr = INADDR_ANY; //inet_addr(ip);;
#else
    listenAddr.sin_addr.s_addr = ( ip && strlen(ip) ) ? inet_addr(ip) : INADDR_ANY;
#endif
	bool bindSuccess = false;
	for( int tryCnt = 0; tryCnt < 5; tryCnt++ )
	{
		if( 0 > bind (sock, (struct sockaddr *) &listenAddr, sizeof (sockaddr_in) ) )
		{
			RCODE rc = VxGetLastError();
			LogMsg( LOG_ERROR, "VxServerMgr::startListening bind skt %d error %d %s try cnt %d thread 0x%x", sock, rc, VxDescribeSktError( rc ), tryCnt + 1, VxGetCurrentThreadId() );
			VxSleep( 1000 );
			continue;
		}
		
		bindSuccess = true;
		break;
	}

	if( false == bindSuccess )
	{
		LogMsg( LOG_ERROR, "VxServerMgr::startListening bind skt %d FAILED thread 0x%x", sock, VxGetCurrentThreadId() );
		::VxCloseSktNow( sock );
		return false;
	}

    LogModule( eLogListen, LOG_INFO, "StartListen socket %d index %d ip %s port %d thread 0x%x", sock, m_iActiveListenSktCnt, ip, u16ListenPort, VxGetCurrentThreadId() );

	m_aoListenSkts[ m_iActiveListenSktCnt ] = sock;
	m_iActiveListenSktCnt++;
    if( ip )
    {
        m_LclIp.setIp( ip );
    }

	m_LclIp.setPort( u16ListenPort );

    return internalStartListen();
#endif // TARGET_OS_ANDROID

	//LogMsg( LOG_INFO, " VxServerMgr::startListening %s:%d\n", ip, u16ListenPort ); 
	std::vector<InetAddress> allAddresses;
	InetAddress addrGetter;
	addrGetter.getAllAddresses( allAddresses );

	std::vector<InetAddress>::iterator iter;
	for( iter = allAddresses.begin(); iter != allAddresses.end(); ++iter )
	{
		// Highly unlikely, but check anyway.
		if( FD_SETSIZE == m_iActiveListenSktCnt ) 
		{
			LogMsg( LOG_ERROR, "getaddrinfo returned more addresses than we could use.\n");
			break;
		}

		InetAddress thisAddr = *iter;
		std::string thisIp = thisAddr.toStdString();
		if( thisAddr.isIPv4() )
		{
			// user specified which ipv4 address to use. If this is not it then skip it
			if( thisIp != ipv4String )
			{
				//LogMsg( LOG_INFO, "VxServerMgr::startListening Skip ip %s\n", thisIp.c_str() );
				continue;
			}
			else
			{
                LogModule( eLogListen, LOG_INFO, "VxServerMgr::startListening found local ip4 %s\n", thisIp.c_str() );
			}
		}
		else
		{
#ifdef SUPPORT_IPV6
			if( false == thisAddr.isIPv6GlobalAddress() )
			{
				continue;
			}

            LogModule( eLogListen, LOG_INFO, "VxServerMgr::startListening found global ipv6 %s\n", thisIp.c_str() );
#else
			continue;
#endif //  SUPPORT_IPV6
		}

		// Open a socket with the correct address family for this address.
		SOCKET sock = socket( thisAddr.isIPv4() ? AF_INET : AF_INET6, SOCK_STREAM, 0);
		if( INVALID_SOCKET == sock )
		{
			LogMsg( LOG_ERROR,  "VxServerMgr::startListening socket() port %d ip %s failed with error %d: %s thread 0x%x", u16ListenPort, thisIp.c_str(),
				VxGetLastError(), VxDescribeSktError( VxGetLastError() ), VxGetCurrentThreadId() );
			continue;
		}

		if( thisAddr.isIPv4() )
		{
			struct sockaddr_in listenAddr;
			memset(&listenAddr, '\0', sizeof( struct sockaddr_in ) );
            listenAddr.sin_family = AF_INET;
            listenAddr.sin_port = htons(u16ListenPort);
#if !USE_BIND_LOCAL_IP
            listenAddr.sin_addr.s_addr = INADDR_ANY;
#else
            listenAddr.sin_addr.s_addr = ip ? inet_addr( thisIp.c_str() ) : INADDR_ANY;
#endif
			bool bindFailed = true;
			for( int i = 0; i < 3; i++ )
			{
				if (bind (sock, (struct sockaddr *) &listenAddr, sizeof ( struct sockaddr_in )) < 0)
				{
					RCODE rc = VxGetLastError();
                    LogModule( eLogListen, LOG_INFO, "VxServerMgr::startListening bind skt error %d %s try %d\n", rc, VxDescribeSktError( rc ), i+1 );
					VxSleep( 1000 );
					continue;
				}
				else
				{
					bindFailed = false;
					break;
				}
			}

			if( bindFailed )
			{
                ::VxCloseSktNow( sock );
				continue;
			}

			if( SOCKET_ERROR == listen( sock, 10 ) ) // SOMAXCONN) ) ( SOMAXCONN == maximum number of qued allowed
			{
                LogModule( eLogListen, LOG_ERROR, "ipv4 listen() failed with error %d: %s thread 0x%x", VxGetLastError(), VxDescribeSktError( VxGetLastError() ), VxGetCurrentThreadId() );
				// fail completely
				::VxCloseSktNow( sock );
				continue;
			}
            else
            {
                LogModule( eLogListen, LOG_DEBUG, "ipv4 listen() success lcl ip %s port %d thread 0x%x", thisIp.c_str(), u16ListenPort, VxGetCurrentThreadId() );
            }

			m_LclIp.setIp( thisIp.c_str() );
			m_LclIp.setPort( u16ListenPort );

			m_aoListenSkts[ m_iActiveListenSktCnt ] = sock;
			m_iActiveListenSktCnt++;
			break;
		}
		else
		{
			struct sockaddr_in6 oAddr;
			memset(&oAddr, '\0', sizeof( struct sockaddr_in6 ) );
			oAddr.sin6_family = AF_INET6;
			oAddr.sin6_port = htons(u16ListenPort);
			inet_pton(AF_INET6, thisIp.c_str(), (void *)&oAddr.sin6_addr.s6_addr);
			bool bindFailed = true;
			for( int i = 0; i < 3; i++ )
			{
				if( bind (sock, (struct sockaddr *) &oAddr, sizeof (oAddr)) < 0)
				{
					RCODE rc = VxGetLastError();
					LogMsg( LOG_INFO, "VxServerMgr::startListening bind skt ipv6 error %d %s\n", rc, VxDescribeSktError( rc ) );
					::VxCloseSkt( sock );
					continue;
				}
				else
				{
					bindFailed = false;
					break;
				}
			}

			if( bindFailed )
			{
				::VxCloseSktNow( sock );
				continue;
			}


			if( SOCKET_ERROR == listen( sock, 10 ) ) // SOMAXCONN) ) ( SOMAXCONN == maximum number of qued allowed
			{
				::VxCloseSkt( sock );
				continue;
			}

			m_LclIp.setIp( thisIp.c_str() );
			m_LclIp.setPort( u16ListenPort );
			m_aoListenSkts[ m_iActiveListenSktCnt ] = sock;
			m_iActiveListenSktCnt++;

            LogModule( eLogListen, LOG_INFO, "VxServerMgr::startListening success ipv6 %s thread 0x%x", thisIp.c_str(), VxGetCurrentThreadId() );
			break;
		}
	}

	if( 0 == m_iActiveListenSktCnt ) 
	{
		LogMsg( LOG_ERROR, "### ERROR: VxServerMgr::startListening unable to serve on any address thread 0x%x", VxGetCurrentThreadId());
		return -1;
	}

	return internalStartListen();
 }

//============================================================================
RCODE VxServerMgr::internalStartListen( void )
{
	// make a useful thread name
	std::string strVxThreadName;
	StdStringFormat( strVxThreadName, "VxServerMgr%d", m_iMgrId );
    return m_ListenVxThread.startThread( (VX_THREAD_FUNCTION_T)VxServerMgrVxThreadFunc, this, strVxThreadName.c_str() );
}

/*
//============================================================================
bool VxServerMgr::startListening(  uint16_t u16ListenPort )
{
#if !USE_BIND_LOCAL_IP
    return startListeningNoBind( u16ListenPort );
#endif 
	stopListening();

	if( VxIsAppShuttingDown() )
	{
		return false;
	}

	if( 0 == u16ListenPort )
	{
		AppErr( eAppErrBadParameter, "VxServerMgr::startListening Bad param port %d\n", u16ListenPort );
		return false;
	}

    LogModule( eLogConnect, LOG_INFO, "VxServerMgr::startListening attempt on port %d thread 0x%x", u16ListenPort, VxGetCurrentThreadId() );

	// By setting the AI_PASSIVE flag in the hints to getaddrinfo, we're
	// indicating that we intend to use the resulting address(es) to bind
	// to a socket(s) for accepting incoming connections.  This means that
	// when the Address parameter is NULL, getaddrinfo will return one
	// entry per allowed protocol family containing the unspecified address
	// for that family.
	//
	char *Address = NULL;

	struct addrinfo *AddrInfo, *AI;
	struct addrinfo oHints;
	VxFillHints( oHints );

    char as8Port[10];
	sprintf( as8Port, "%d", u16ListenPort );

	//LogMsg( LOG_INFO, " VxServerMgr::startListening port %d\n",  u16ListenPort ); 
	int RetVal = getaddrinfo(Address, as8Port, &oHints, &AddrInfo);
	if( 0 != RetVal ) 
	{
		LogMsg( LOG_INFO, "getaddrinfo failed with error %d: %s\n", RetVal, gai_strerror(RetVal));
		return false;
	}
	//
	// For each address getaddrinfo returned, we create a new socket,
	// bind that address to it, and create a queue to listen on.
	//
	for(AI = AddrInfo; AI != NULL; AI = AI->ai_next) 
	{
		// Highly unlikely, but check anyway.
		if( FD_SETSIZE == m_iActiveListenSktCnt ) 
		{
			LogMsg( LOG_ERROR, "getaddrinfo returned more addresses than we could use.\n");
			break;
		}

		// only support PF_INET and PF_INET6.
		if ((AI->ai_family != PF_INET) && (AI->ai_family != PF_INET6))
			continue;

		// Open a socket with the correct address family for this address.
        SOCKET sock = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);
        if( INVALID_SOCKET == sock )
        {
			LogMsg( LOG_ERROR,  "socket() failed with error %d: %s\n",
				VxGetLastError(), VxDescribeSktError(VxGetLastError()));
			continue;
		}

#if 0 //#ifdef TARGET_OS_WINDOWS
		if ((PF_INET6 == AI->ai_family ) &&
			IN6_IS_ADDR_LINKLOCAL((IN6_ADDR *) INETADDR_ADDRESS(AI->ai_addr)) &&
			(((SOCKADDR_IN6 *) (AI->ai_addr))->sin6_scope_id == 0)
			) 
		{
			LogMsg( LOG_ERROR, "IPv6 link local addresses should specify a scope ID!\n");
		}
#endif // TARGET_OS_WINDOWS
		//
		// bind() associates a local address and port combination
		// with the socket just created. This is most useful when
		// the application is a server that has a well-known port
		// that clients know about in advance.
		//

		bool bindFailed = true;
		for( int i = 0; i < 3; i++ )
		{
			if( bind( sock, AI->ai_addr, (int) AI->ai_addrlen ) < 0 )
			{
				RCODE rc = VxGetLastError();
				LogMsg( LOG_INFO, "VxServerMgr::startListening bind() skt error %d %s try %d\n", rc, VxDescribeSktError( rc ), i+1 );
				VxSleep( 1000 );
				continue;
			}
			else
			{
				bindFailed = false;
				break;
			}
		}

		if( bindFailed )
		{
			::VxCloseSktNow( sock );
			continue;
		}

		m_LclIp.setIpAndPort( *AI->ai_addr );
		m_LclIp.setPort( u16ListenPort );
        m_u16ListenPort = u16ListenPort;
		m_aoListenSkts[ m_iActiveListenSktCnt ] = sock;
		m_iActiveListenSktCnt++;

        if( 0 != internalStartListen() )
        {
			m_iActiveListenSktCnt = 0;
            continue;
        }

        LogModule( eLogListen, LOG_INFO, "'Listening' on port %s, protocol %s, protocol family %s\n",
			as8Port, 
			"TCP",
			(AI->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
		break;
	}

	freeaddrinfo(AddrInfo);

	if( 0 == m_iActiveListenSktCnt ) 
	{
		LogMsg( LOG_ERROR, "### Fatal error: unable to serve on any address.\n");
		return false;
	}

	return true;
}
*/

//============================================================================
/// @brief listen on port without binding to a ip address
bool VxServerMgr::startListeningNoBind( uint16_t u16ListenPort )
{
    stopListening();

    if( VxIsAppShuttingDown() )
    {
        return false;
    }

    if( 0 == u16ListenPort )
    {
        AppErr( eAppErrBadParameter, "VxServerMgr::startListeningNoBind Bad param port %d\n", u16ListenPort );
        return false;
    }

    LogModule( eLogConnect, LOG_INFO, "VxServerMgr::startListeningNoBind attempt on port %d thread 0x%x", u16ListenPort, VxGetCurrentThreadId() );

    SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
    if( sock < 0 ) {
        perror( "ERROR opening socket" );
        LogModule( eLogListen, LOG_ERROR, "VxServerMgr::startListeningNoBind failed to create socket" );
        return false;
    }

    struct sockaddr_in sktAddr;
    memset( &sktAddr, 0, sizeof( struct sockaddr_in ) );
    sktAddr.sin_family = AF_INET;
    sktAddr.sin_port = htons( u16ListenPort );
    sktAddr.sin_addr.s_addr = INADDR_ANY; // if set to specific address then PureVPN does not work //inet_addr( thisIp.c_str() );
    bool bindFailed = true;
    for( int i = 0; i < 3; i++ )
    {
        if( bind( sock, ( struct sockaddr * ) &sktAddr, sizeof( struct sockaddr_in ) ) < 0 )
        {
            RCODE rc = VxGetLastError();
            LogModule( eLogListen, LOG_INFO, "VxServerMgr::startListening bind skt error %d %s try %d\n", rc, VxDescribeSktError( rc ), i + 1 );
            VxSleep( 1000 );
            continue;
        }
        else
        {
            bindFailed = false;
            m_ListenSktIsBoundToIp = true;
            break;
        }
    }

    if( bindFailed )
    {
        ::VxCloseSktNow( sock );
        return false;
    }

    if( SOCKET_ERROR == listen( sock, 10 ) ) // SOMAXCONN) ) ( SOMAXCONN == maximum number of qued allowed
    {
        LogModule( eLogListen, LOG_ERROR, "ipv4 listen() failed with error %d: %s thread 0x%x", VxGetLastError(), VxDescribeSktError( VxGetLastError() ), VxGetCurrentThreadId() );
        // fail completely
        ::VxCloseSktNow( sock );
        return false;
    }
    else
    {
        LogModule( eLogListen, LOG_DEBUG, "ipv4 listen() success any ip port %d thread 0x%x", u16ListenPort, VxGetCurrentThreadId() );
    }

    m_ListenMutex.lock();
    bool listenStatus = true;
    m_u16ListenPort = u16ListenPort;
    m_LclIp.setIpAndPort( "", u16ListenPort );
    m_aoListenSkts[ m_iActiveListenSktCnt ] = sock;
    m_iActiveListenSktCnt++;
    m_ListenMutex.unlock();

    if( 0 != internalStartListen() )
    {
        LogModule( eLogListen, LOG_ERROR, "ipv4 listen() internalStartListen failed" );
        closeListenSocket();
        listenStatus = false;
    }

     return listenStatus;
}

//============================================================================
void VxServerMgr::closeListenSocket( void )
{
    if( m_iActiveListenSktCnt )
    {
        m_ListenVxThread.abortThreadRun( true );

        m_ListenSktIsBoundToIp = false;
        m_IsReadyToAcceptConnections = false;
        LogModule( eLogListen, LOG_DEBUG, "### VxServerMgr: Mgr %d stop listening %d skt cnt %d thread 0x%x\n", m_iMgrId, m_u16ListenPort, m_iActiveListenSktCnt, VxGetCurrentThreadId() );
        m_u16ListenPort = 0;

        // kill previous thread if running
        m_ListenVxThread.abortThreadRun( true );
        m_ListenMutex.lock();
        for( int i = 0; i < m_iActiveListenSktCnt; i++ )
        {
            if( INVALID_SOCKET != m_aoListenSkts[ i ] )
            {
                LogModule( eLogListen, LOG_INFO, "VxServerMgr: Mgr %d closing listen skt %d\n", m_iMgrId, i );

                // closing the thread should release it so it can exit
                SOCKET sktToClose = m_aoListenSkts[ i ];
                m_aoListenSkts[ i ] = INVALID_SOCKET;
                ::VxCloseSktNow( sktToClose );
            }
            else
            {
                LogModule( eLogListen, LOG_ERROR, "VxServerMgr:stopListening skt idx %d had invalid socket\n", i );
            }
        }

        m_iActiveListenSktCnt = 0;
        m_ListenMutex.unlock();
        if( m_ListenVxThread.isThreadRunning() )
        {
            m_ListenVxThread.killThread();
        }
    }
    else
    {
        LogModule( eLogListen, LOG_DEBUG, "VxServerMgr:stopListening called with no listen sockets\n" );
    }
}

//============================================================================
RCODE VxServerMgr::stopListening( void )
{
    if( !isListening() )
    {
        return 0; // not listening
    }

    closeListenSocket();

    m_ListenMutex.lock();
    m_u16ListenPort = 0;
    m_LclIp.setIpAndPort( "", m_u16ListenPort );
    m_ListenMutex.unlock();

	return 0;
}

//============================================================================
RCODE VxServerMgr::acceptConnection( VxThread * poVxThread, SOCKET oListenSkt )
{
	RCODE rc = 0;
	if( INVALID_SOCKET == oListenSkt )
	{
        LogModule( eLogListen, LOG_ERROR, "VxServerMgr::acceptConnection INVALID LISTEN SOCKET thread 0x%x", VxGetCurrentThreadId() );
		return -2;
	}

	if( VxIsAppShuttingDown() )
	{
        LogModule( eLogListen, LOG_ERROR, "VxServerMgr::acceptConnection App Shutting down thread 0x%x", VxGetCurrentThreadId() );
        return -3;
	}

    static int dumpAcceptCnt = 0;
    dumpAcceptCnt++;
    if( dumpAcceptCnt > 20 )
    {
        dumpAcceptCnt = 0;
        LogModule( eLogAcceptConn, LOG_INFO, "VxServerMgr: start acceptConnection skt %d rc %d thread 0x%x", oListenSkt, VxGetLastError(), VxGetCurrentThreadId() );
    }

	// perform accept
	// setup address
    struct  sockaddr acceptAddr;
    socklen_t acceptAddrLen = sizeof( struct  sockaddr );
    memset( &acceptAddr, 0, sizeof( struct sockaddr ) );

#if defined( TARGET_OS_WINDOWS ) || defined( TARGET_OS_ANDROID )
    // NOTE: in android the return to blocking on listen doesn't work so we just set it once before start listening so accept does not get hung
    SOCKET oAcceptSkt = accept( oListenSkt, &acceptAddr, &acceptAddrLen );
#else //LINUX
	// NOTE: accept can hang waiting for connection in linux or android if
	// connection is dropped before the accept happens so set to non blocking.. the reason it hangs is it will wait until next connection occures	
	VxSetSktBlocking( oListenSkt, false );
    SOCKET oAcceptSkt = accept( oListenSkt, &acceptAddr, &acceptAddrLen );
	VxSetSktBlocking( oListenSkt, true );
#endif // LINUX

static int acceptErrCnt = 0;
static int dumpSktStatsCnt = 0;
    if( INVALID_SOCKET == oAcceptSkt )
    {
		rc = VxGetLastError();
#if defined(TARGET_OS_WINDOWS)
        if( rc == WSAEWOULDBLOCK )
        {
            rc = EAGAIN;
        }
#endif // defined(TARGET_OS_WINDOWS)

        if( rc )
        {
            acceptErrCnt++;
            if( acceptErrCnt > 100 )
            {
                acceptErrCnt = 0;
                dumpSktStatsCnt++;
                if( rc != EAGAIN )
                {
                    LogModule( eLogListen, LOG_DEBUG, "VxServerMgr::acceptConnection: listen port %d skt %d error %d thread 0x%x", m_u16ListenPort, oListenSkt, rc, VxGetCurrentThreadId() );
                }

                if( dumpSktStatsCnt > 10 )
                {
                    dumpSktStatsCnt = 0;
                    dumpSocketStats("full dump", true);
                }
                else
                {
                    dumpSocketStats();
                }
            }
        }
        else
        {
            acceptErrCnt = 0;
        }

		if( 0 == rc )
		{
			// not sure how it happens but seems to get in a loop where the clear doesn't clear and there is no error
			// so sleep just in case so doesn't eat up all the CPU
            LogModule( eLogListen, LOG_INFO, "VxServerMgr: no rc acceptConnection skt %d rc %d thread 0x%x", oListenSkt, VxGetLastError(), VxGetCurrentThreadId() );
			VxSleep( 500 );
			return -1;
		}
        else if( EAGAIN == rc )
        {
            // windows non blocking operation could not be done immediate error
            VxSleep( 200 );
            static int intRetry1Cnt = 0;
            intRetry1Cnt++;
            if( intRetry1Cnt > 20 )
            {
                intRetry1Cnt = 0;
                LogModule( eLogAcceptConn, LOG_INFO, "VxServerMgr: non blocking operation  acceptConnection skt %d rc %d thread 0x%x", oListenSkt, VxGetLastError(), VxGetCurrentThreadId() );
            }

            return 0;
        }
 		else
		{
            LogModule( eLogListen, LOG_INFO, "VxServerMgr: other error acceptConnection skt %d rc %d thread 0x%x", oListenSkt, VxGetLastError(), VxGetCurrentThreadId() );
			VxSleep( 200 );
			return rc;
		}
    }
    else
    {
        acceptErrCnt = 0;
    }

    LogModule( eLogListen, LOG_DEBUG, "VxServerMgr::acceptConnection: listen skt %d accepted skt %d thread 0x%x", oListenSkt, oAcceptSkt, VxGetCurrentThreadId() );
	if( poVxThread->isAborted() || VxIsAppShuttingDown() ) 
	{
		return -1;
	}

	// valid accept socket
	if( m_aoSkts.size() >= m_u32MaxConnections )
	{
        LogMsg( LOG_ERROR, "VxServerMgr: reached max connections %d thread 0x%x", m_u32MaxConnections, VxGetCurrentThreadId() );
        dumpSocketStats("VxServerMgr");
        // we have reached max connections
        // just close it immediately
        VxCloseSktNow( oAcceptSkt );

        doSktDeleteCleanup();
		// sleep awhile
		VxSleep( 200 );
		return 0; // keep running until number of connections clear up
	}

	// add a skt to our list	
	VxSktAccept * sktBase = (VxSktAccept *)this->makeNewAcceptSkt();
	m_SktMgrMutex.lock(__FILE__, __LINE__); // dont let other threads mess with array while we add
	m_aoSkts.push_back( sktBase );
	// do tell skt to do accept stuff
	sktBase->m_Socket = oAcceptSkt;
	sktBase->setReceiveCallback( m_pfnOurReceive, this );
	sktBase->setTransmitCallback( m_pfnOurTransmit, this );
	m_SktMgrMutex.unlock(__FILE__, __LINE__);

    LogModule( eLogListen, LOG_INFO, "VxServerMgr: doing accept skt %d skt id %d thread 0x%x", sktBase->m_Socket, sktBase->getSktId(), VxGetCurrentThreadId() );

    RCODE rcAccept = sktBase->doAccept( this, *(( struct sockaddr * )&acceptAddr) );
	if( rcAccept || poVxThread->isAborted() || INVALID_SOCKET == oListenSkt )
	{
		sktBase->closeSkt(eSktCloseAcceptFailed);
		LogMsg( LOG_ERROR, "VxServerMgr: error %d doing accept skt %d skt id %d thread 0x%x", rc, sktBase->m_Socket, sktBase->getSktId(), VxGetCurrentThreadId() );
        moveToEraseList( sktBase );

        rc = -5;
	}
    else
    {
        acceptErrCnt = 0; // reset counter
        LogModule( eLogListen, LOG_INFO, "VxServerMgr: accept success skt %d skt id %d thread 0x%x", sktBase->m_Socket, sktBase->getSktId(), VxGetCurrentThreadId() );
    }

    doSktDeleteCleanup();
	return rc;
}

//============================================================================
void VxServerMgr::listenForConnectionsToAccept( VxThread * poVxThread )
{
#ifdef DEBUG_SKT_CONNECTIONS
	//LogMsg( LOG_INFO, "111 IN THREAD VxServerMgr::listenForConnectionsToAccept started\n" ); 
	//LogMsg( LOG_INFO, "111 IN THREAD VxServerMgr::listen port %d ip %s skt %d\n", m_LclIp.getPort(), m_LclIp.toStdString().c_str(), m_aoListenSkts[0] ); 
#endif // DEBUG_SKT_CONNECTIONS

    m_IsReadyToAcceptConnections = true;
//#ifdef TARGET_OS_ANDROID
	// android set listen skt back to blocking doesn't work so just set to non blocking always ( part of accept hang fix ) 
	VxSetSktBlocking( m_aoListenSkts[0], false );

	// for some unknown reason code that works on mac/windows/linux to use select does not work on android
	// on android when use select the select seems to work but in the accept it gets error 22 (invalid param) .. so do this crap
	while ( ( false == poVxThread->isAborted() )
	   && ( false == VxIsAppShuttingDown() )
	   && ( 0 < m_iActiveListenSktCnt )
	   && ( checkWatchdog() ) )
	{
        RCODE rc = 0;
        int listenResult = listen( m_aoListenSkts[0], 8 );
        if( 0 > listenResult )
		{
            rc = VxGetLastError();
#if defined(TARGET_OS_WINDOWS)
            if( rc == WSAEWOULDBLOCK )
            {
                rc == EAGAIN;
            }
#endif // defined(TARGET_OS_WINDOWS)
		}
		
		if( poVxThread->isAborted() 
			|| VxIsAppShuttingDown()
			|| ( 0 >= m_iActiveListenSktCnt ) ) 
		{
			break;
		}
		
        if( rc )
        {
            if( rc == EAGAIN )
            {
                static int listenErrCnt = 0;
                listenErrCnt++;
                if( listenErrCnt > 50 )
                {
                    listenErrCnt = 0;
                    LogModule( eLogListen, LOG_DEBUG, "listenForConnectionsToAccept: try again: listen ip %s port %d skt %d error %d thread 0x%x", m_LclIp.toStdString().c_str(), m_u16ListenPort, m_aoListenSkts[0], rc, VxGetCurrentThreadId() );
                }

                VxSleep( 200 );
                continue;
            }
            else if( rc == EADDRINUSE )
            {
                LogModule( eLogListen, LOG_DEBUG, "listen: Another socket is already listening on the same port.");
            }
            else if( rc == EBADF )
            {
                LogModule( eLogListen, LOG_DEBUG, "listen: The argument sockfd is not a valid file descriptor.");
                break;
            }
            else if( rc == ENOTSOCK )
            {
                LogModule( eLogListen, LOG_DEBUG, "listen: The file descriptor sockfd does not refer to a socket.");
                break;
            }
            else if( rc == EOPNOTSUPP )
            {
                LogModule( eLogListen, LOG_DEBUG, "Tlisten: he socket is not of a type that supports the listen() operations.");
                break;
            }
            else
            {
                LogModule( eLogListen, LOG_DEBUG, "listen: UNKNOW ERROR");
                break;
            }
        }

		acceptConnection( poVxThread, m_aoListenSkts[0] );
	}	
	
	m_IsReadyToAcceptConnections = false;
	if( INVALID_SOCKET != m_aoListenSkts[ 0 ] )
	{
		SOCKET sktToClose = m_aoListenSkts[ 0 ];
        LogModule( eLogListen, LOG_INFO, "VxServerMgr:listenForConnectionsToAccept closing listen skt %d", sktToClose );
		m_aoListenSkts[ 0 ] = INVALID_SOCKET;
		::VxCloseSktNow( sktToClose );
	}

	if( (false == VxIsAppShuttingDown() )
		&& ( false == checkWatchdog() ) )
	{
        LogModule( eLogListen, LOG_ERROR, "Listen Failed Watchdog" );
		std::terminate();
	}

    LogModule( eLogConnect, LOG_INFO, "Listen Thread is exiting thread 0x%x", VxGetCurrentThreadId() );
	m_IsReadyToAcceptConnections = false;
}

//============================================================================
bool VxServerMgr::createListenSocket( std::string &preferredIp, SOCKET &listenSock, uint16_t listenPort, std::string &actualListenLocalIp )
{
    actualListenLocalIp = "";
    if( listenPort < 80 )
    {
        LogMsg( LOG_ERROR, "VxServerMgr::createListenSocket invalid listen port %d", listenPort );
        return false;
    }

    listenSock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    memset( &serverAddr, 0, sizeof( struct sockaddr_in ) );

    serverAddr.sin_family = AF_INET;                // sets listen socket protocol type
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // sets our local IP address
    serverAddr.sin_port = htons(listenPort);        // sets the listen port number 

    listenSock = socket(AF_INET, SOCK_STREAM, 0);               // creates IP based TCP socket
    if( listenSock < 0 ) 
    { 
        LogMsg( LOG_ERROR, "VxServerMgr::createListenSocket failed" );
        return false;
    }

    // Bind Socket
    int bindStatus = bind( listenSock, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr) );
    int retryCnt = 0;
    while( bindStatus < 0 )
    {
        retryCnt++;
        if( retryCnt >= 3 )
        {
            LogMsg( LOG_ERROR, "VxServerMgr::createListenSocket bind socket %d failed event after %d tries", listenSock, retryCnt );
            return false;
        }

        VxSleep( 1000 );
        bindStatus = bind( listenSock, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr) );
    }

    if( bindStatus >= 0 )
    {
        // unfortunately (At least in windows) getpeername is invalid and getsockname returns 0.0.0.0 if you bind to INADDR_ANY
        // there seems to be no way to get the actual address of the adapter we will get the incomming connection on
        struct sockaddr lclSockName;
        memset( &lclSockName, 0, sizeof( struct sockaddr ) );
        socklen_t sockNameLen = sizeof( struct sockaddr );
        getsockname( listenSock, &lclSockName, &sockNameLen );

        VxGetLclAddress( listenSock, m_LclIp );
        actualListenLocalIp = m_LclIp.toStdString();
    }

    return bindStatus >= 0;
}
