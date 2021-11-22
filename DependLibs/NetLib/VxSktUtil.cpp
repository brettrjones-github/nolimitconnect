//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include <CoreLib/VxDefs.h>

#include "VxSktUtil.h"
#include "VxResolveHost.h"

#include <PktLib/VxCommon.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>
#include <CoreLib/VxTimer.h>
#include <CoreLib/VxTime.h>
#include <CoreLib/VxLinuxOnly.h>
#include <CoreLib/IsBigEndianCpu.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <memory.h>

#ifdef TARGET_OS_WINDOWS
    // helpers for adapter info
	//#include <iphlpapi.h>
	//#pragma comment(lib, "IPHLPAPI.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
	#include <net/if.h> 
    #include <netdb.h>
	#include <ctype.h>
	#include <fcntl.h>
    #include <sys/types.h>
# ifdef TARGET_OS_ANDROID
# include <sys/ioctl.h>
# endif
#endif // _MSC_VER
#ifdef _MSC_VER
# pragma warning( disable: 4996 ) //'strnicmp': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strnicmp.
#endif // _MSC_VER


namespace
{
	std::vector<InetAddress>	g_aoAllIps;
	InetAddress					g_oSelectedLocalIp;
	InetAddress					g_oDefaultLocalIp;
	InetAddress					g_oDefaultIPv4;
	InetAddress					g_oDefaultIPv6;
	InetAddress					g_oGlobalIPv6;
}

//============================================================================
//! initialize sockets
RCODE VxSocketsStartup( void )
{
	srand( (unsigned int)time( NULL ) );
	static bool bIsInitialized = 0;
	if( bIsInitialized )
	{
		return 0;
	}

#ifdef TARGET_OS_WINDOWS
	static struct WSAData wsa_state;
	memset(&wsa_state,0,sizeof(wsa_state));
	if (WSAStartup(0x0202,&wsa_state)!=0)
	{
		int err = WSAGetLastError();
		LogMsg( LOG_INFO, "SocketsStartup: Error %d\n", err );
		vx_assert( false );
		return(-1);
	}
#endif // TARGET_OS_WINDOWS

	VxRefreshDefaultIps();
	bIsInitialized = true;
	return  0;
}

//============================================================================
bool VxIsIpValid( std::string& ipAddr )
{
    static const std::string nullIpV4 = "0.0.0.0";
    static const std::string loopBackIpV4 = "127.0.0.1";
    static const std::string loopBackIp1V6 = "0000:0000:0000:0000:0000:0000:0000:0001/128";
    static const std::string loopBackIp2V6 = "::1/128";
    return ipAddr.size() >= nullIpV4.size() &&
        ipAddr != nullIpV4 &&
        ipAddr != loopBackIpV4 &&
        ipAddr != loopBackIp1V6 &&
        ipAddr != loopBackIp2V6 &&
        ( ipAddr.find('.') != std::string::npos || ipAddr.find(':') != std::string::npos );
}

//============================================================================
void VxIpInNetOrderToString( uint32_t u32IpAddr, std::string& retIp )
{
	char as8Buf[ 32 ];
	unsigned char * pTemp = (unsigned char * )&u32IpAddr;
	sprintf( as8Buf, "%i.%i.%i.%i", pTemp[0], pTemp[1], pTemp[2], pTemp[3] );

	//int32_t u32IpHostOrder = ntohl( u32IpAddr );
	//unsigned char * pTemp = (unsigned char * )&u32IpHostOrder;
	//sprintf( as8Buf, "%i.%i.%i.%i", pTemp[0], pTemp[1], pTemp[2], pTemp[3] );
	retIp  = as8Buf;
}

//============================================================================
void VxIpInHostOrderToString( uint32_t u32IpAddr, std::string& retIp )
{
	if( IsBigEndianCpu() )
	{
		char as8Buf[ 32 ];
		int32_t u32IpHostOrder = ntohl( u32IpAddr );
		unsigned char * pTemp = (unsigned char * )&u32IpHostOrder;
		sprintf( as8Buf, "%i.%i.%i.%i", pTemp[0], pTemp[1], pTemp[2], pTemp[3] );
		retIp  = as8Buf;
	}
	else
	{
		VxIpInNetOrderToString( u32IpAddr, retIp )	;
	}
}

//============================================================================
uint32_t VxStringToIpInNetOrder( std::string ip )
{
	return inet_addr( ip.c_str() );
}

//============================================================================
std::string	VxIpToString( struct sockaddr * addr )
{
	if( AF_INET == addr->sa_family )
	{
		return std::string( inet_ntoa( ((sockaddr_in *)(addr))->sin_addr ) );
	}
	else if( AF_INET6 == addr->sa_family )
	{
		char ip6Buf[64];
		ip6Buf[0] = 0;
		VxIP_ntop( AF_INET6, &((sockaddr_in6 *)(addr))->sin6_addr, ip6Buf, sizeof( ip6Buf ), true );
		return std::string( ip6Buf );
	}

	return std::string( "UNKNOWN" );
}

//============================================================================
void VxFillHints( struct addrinfo& hints, bool bUdpSkt, bool ipv6Only )
{
	memset(&hints, 0, sizeof hints);
	//hints.ai_family = AF_UNSPEC;
	//hints.ai_family = AF_INET
	//hints.ai_family = AF_INET6
	hints.ai_family = ipv6Only ? AF_INET6 : AF_INET; /* AF_INET, AF_INET6 or AF_UNSPEC */
	hints.ai_socktype = bUdpSkt ? SOCK_DGRAM : SOCK_STREAM;
	hints.ai_protocol = bUdpSkt ? IPPROTO_UDP : IPPROTO_TCP;
	//hints.ai_flags = AI_PASSIVE; // AI_NUMERICHOST | AI_PASSIVE
}

//============================================================================
void VxIP_ntop( int family, void * pvBinary, char * pBuf, int iBufLen, bool isHostOrder )
{
	if( AF_INET == family )
	{
		VxIPv4_ntop( pvBinary, pBuf, iBufLen, isHostOrder );
	}
	else
	{
		VxIPv6_ntop( pvBinary, pBuf, iBufLen );
	}
}

//============================================================================
void VxIPv4_ntop( void * pvBinary, char * pBuf, int iBufLen, bool isHostOrder )
{
    uint8_t * pTemp = 0;
	uint32_t u32NetOrder;
	if( isHostOrder )
	{
		// change to net order
        uint32_t * binaryIPv4 = static_cast<uint32_t*>(pvBinary);
        u32NetOrder = (uint32_t)htonl( *binaryIPv4 );
		pTemp = (uint8_t *)&u32NetOrder;
	}
	else
	{
		pTemp = (uint8_t *)pvBinary;
	}

    sprintf( pBuf, "%i.%i.%i.%i", pTemp[3], pTemp[2], pTemp[1], pTemp[0] );
}

//============================================================================
void VxIPv6_ntop( void * pvBinary, char * pBuf, int iBufLen )
{
	uint16_t * pVal = (uint16_t *)pvBinary;

	bool isValid = false;
	for( int i = 0; i <8; ++i )
	{
		if( 0 != pVal[i] )
		{
			isValid = true;
			break;
		}
	}

	if( false == isValid )
	{
		strcat( pBuf, "::" );
	}
	else
	{
		int colonCount = 0;
		for( int i = 0; i < 8; i++)
		{
			if( 0 == pVal[i])
			{
				if( 0 == colonCount )
				{
					// beginning of address
					strcat( pBuf, "::");
					colonCount = 2;
				}
				else if( 1 == colonCount )
				{
					strcat( pBuf, ":");
					colonCount++;
				}
			}
			else
			{
				sprintf(&pBuf[strlen(pBuf)], "%4.4x", htons(pVal[i]));
				colonCount = 0;
				if( 7 != i )
				{
					strcat( pBuf, ":" );
					colonCount = 1;
				}
			}
		}
	}
}

//============================================================================
void VxIPv4_pton( const char * pIpStr, void * pvBinary, bool wantHostOrder )
{
	uint32_t u32Ip = 0;
	if( NULL == pIpStr )
	{
		vx_assert( pIpStr );
		return;
	}

	int iStrLen = (int)strlen( pIpStr );
	if( ( iStrLen >=  7 ) &&
		( iStrLen <  16 ) && 
		isdigit( pIpStr[0] ) )
	{
		u32Ip = inet_addr(pIpStr);	
	}
	else
	{
		vx_assert( iStrLen >=  7 );
		vx_assert( iStrLen <  16 );
		vx_assert( isdigit( pIpStr[0] ) );
		u32Ip = 0;
	}

	if( wantHostOrder )
	{
		*((uint32_t *)pvBinary) = u32Ip;
	}
	else
	{
		*((uint32_t *)pvBinary) = htonl( u32Ip );
	}
}

//============================================================================
void VxIPv6_pton( const char * pIpStringIn,  void * pvBinary )
{
	uint16_t * pBinary = (uint16_t *)pvBinary; 
	std::vector<uint16_t> aoValues;
	size_t iStrLen = strlen( pIpStringIn );
	char * pIpString = new char[iStrLen + 1];
	strcpy( pIpString, pIpStringIn );

	while( 0 != pIpString[0] )
	{
		if( ':' == pIpString[0] )
		{
			aoValues.push_back(0);
			pIpString += 1;
		}
		else 
		{
			uint16_t u16Value;
			if( strchr( pIpString, ':') )
			{
				char * pValue = strtok( (char *)pIpString, ":");
				if( pValue )
				{
					u16Value = (uint16_t)strtol( pIpString, NULL, 16 );
					aoValues.push_back( htons( u16Value ) );
					pIpString += strlen( pValue );
				}
				pIpString += + 1;
				
			}
			else
			{
				u16Value = (uint16_t)strtol( pIpString, NULL, 16 );
				aoValues.push_back( htons( u16Value ) );
				break;
			}
		}
	}
	int iSize =  (int)aoValues.size();
	if( 8 != iSize )
	{
		int iZerosToAdd = 8 - iSize;
		uint16_t u16Zero = 0;
		std::vector<uint16_t>::iterator iter;
		for( iter = aoValues.begin(); iter != aoValues.end(); ++iter )
		{
			if( 0 == *iter )
			{
				aoValues.insert( iter, iZerosToAdd, u16Zero );
				break;
			}
		}
	}
	if( 8 == aoValues.size() )
	{
		for( int i = 0; i < 8; i++ )
		{
			pBinary[i] = aoValues[i];
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "Invalid IP %s\n", pIpStringIn );
	}
}

//============================================================================
//! if new connection refresh default ip(s)
void VxRefreshDefaultIps( void )
{
	g_aoAllIps.clear();
	g_oDefaultLocalIp.setToInvalid();
	g_oDefaultIPv4.setToInvalid();
	g_oDefaultIPv6.setToInvalid();
	g_oGlobalIPv6.setToInvalid();

    g_oDefaultLocalIp.getAllAddresses(g_aoAllIps);
	if( g_aoAllIps.size() )
	{
		g_oDefaultLocalIp = g_aoAllIps[0];
		if( false == g_oSelectedLocalIp.isValid() )
		{
			g_oSelectedLocalIp = g_oDefaultLocalIp;
		}

		std::vector<InetAddress>::iterator iter;
		for( iter = g_aoAllIps.begin(); iter != g_aoAllIps.end(); ++iter )
		{
			if( (false == g_oDefaultIPv4.isValid()) && (*iter).isIPv4() )
			{
				g_oDefaultIPv4 = *iter;
			}
			if( (false == g_oDefaultIPv6.isValid()) && (*iter).isIPv6() )
			{
				g_oDefaultIPv6 = *iter;
			}			
			if( (false == g_oGlobalIPv6.isValid()) && (*iter).isIPv6GlobalAddress() )
			{
				g_oGlobalIPv6 = *iter;
			}
		}
	}
}

//============================================================================
bool VxCanConnectUsingIPv6( void )
{
	return ( g_oDefaultIPv6.isValid() || g_oGlobalIPv6.isValid() );
}

//============================================================================
//! get list of local ips.. needed for systems with multiple tcp hardware
std::vector<InetAddress>& VxGetLocalIps( void )	
{
	if( 0 == g_aoAllIps.size() )
	{
		VxRefreshDefaultIps();
	}
	return 	g_aoAllIps;
}

//============================================================================
void VxGetLocalIps(	std::vector<InetAddress>& aRetIpAddress )
{
    if( 0 == g_aoAllIps.size() )
    {
        VxRefreshDefaultIps();
    }
	aRetIpAddress.clear();
	std::vector<InetAddress>::iterator iter;
	for( iter = g_aoAllIps.begin(); iter != g_aoAllIps.end(); ++iter )
	{
		aRetIpAddress.push_back( *iter );
	}
}

//============================================================================
//! get default local ip address.. returns 0 if none
InetAddress VxGetDefaultLocalIp( void )
{
	return g_oDefaultLocalIp;
}

//============================================================================
//! get default local ip address.. returns 0 if none
InetAddress VxGetSelectedLocalIp( void )
{
	return g_oSelectedLocalIp;
}

//============================================================================
InetAddress	VxGetMyGlobalIPv6Address( void )
{
	return g_oGlobalIPv6;
}

//============================================================================
InetAddress	VxGetDefaultIPv4Address( void )
{
	return g_oDefaultIPv4;
}

//============================================================================
InetAddress	VxGetDefaultIPv6Address( void )
{
	return g_oDefaultIPv6;
}

//============================================================================
//! return true if ip is in list of local ips
bool VxLocalIpExists(  std::string& strIpAddress )
{
	std::vector<InetAddress>::iterator iter;
	for( iter = g_aoAllIps.begin(); iter != g_aoAllIps.end(); ++iter )
	{
		if( strIpAddress == (*iter).toStdString() )
		{
			return true;
		}
	}
	return false;
}

//============================================================================
//! return true if port is already in use on this local ip address
bool VxIsIpPortInUse(  uint16_t u16Port, const char * pLocalIp, bool useBind )
{
	bool bInUse = true;

	InetAddress addr;
	if( pLocalIp )
	{
		addr.setIp( pLocalIp );
	}
    else
    {
        // use loopback
        LogMsg( LOG_DEBUG, "VxIsIpPortInUse: null ip so default to loopback\n" );
        addr.setIp( "127.0.0.1" );
    }

	SOCKET listenSocket = socket(addr.isIPv4() ? AF_INET : AF_INET6, SOCK_STREAM, 0);
	if( INVALID_SOCKET == listenSocket )
	{
		//socket create error
		LogMsg( LOG_ERROR, "VxIsIpPortInUse: socket create error %s\n", VxDescribeSktError( VxGetLastError() ) );
		return	bInUse;
	}

    // must always bind when checking for random port or on windows will get
    // WSAEINVAL:"The socket is not already bound to an address."
    if( useBind )
    {
        if( false == VxBindSkt( listenSocket, addr, u16Port ) )
        {
            ::VxCloseSkt( listenSocket );
            LogMsg( LOG_INFO, "VxIsIpPortInUse: bind error %s\n", VxDescribeSktError( VxGetLastError() ) );
            return bInUse;
        }
    }

	RCODE rc;
	if( 0 != ( rc = listen( listenSocket, SOMAXCONN ) ) )
	{
		//listen error
		rc = VxGetLastError();
		LogMsg( LOG_ERROR, "VxIsIpPortInUse: listen error %s\n", VxDescribeSktError( rc ) );

	}
	else
	{
		bInUse = false;
	}

	::VxCloseSkt( listenSocket );
	return bInUse;
}



//============================================================================
//! split host name from website file path
bool VxSplitHostAndFile(	const char * pFullUrl,		// full url.. example http://www.mysite.com/index.html or www.mysite.com/images/me.jpg
							std::string& strRetHost,	// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
							std::string& strRetFileName,	// return file name.. images/me.jpg
							uint16_t& u16RetPort )			// return port if specified else return 80 as default				
{
	char * pTemp;
	char as8Url[2048];

	u16RetPort = 80;
	strRetHost = "";
	strRetFileName = "";

	pTemp = (char *)strchr( pFullUrl, '/' );
	if( NULL == pTemp )
	{
		// assume is just ip address
		if( '[' == pFullUrl[0] )
		{
			strcpy( as8Url, &pFullUrl[1] );
            if( 0 != ( pTemp = strchr(  as8Url, ']' ) ) )
			{
				*pTemp = 0;
				strRetHost = as8Url;
			}
		}
		else
		{
			strRetHost = pFullUrl;
		}

		return true;
	}

	//copy to buffer without protocol specification like https:
	bool hasNetProtocol = false;
	bool isDottedIp = true;
	pTemp = (char *)strstr( pFullUrl, "://" );
	if( pTemp )
	{
		pTemp += 3;
		strcpy( as8Url, pTemp ) ;
		hasNetProtocol = true;
	}
	else
	{
		strcpy( as8Url, pFullUrl ) ;
	}

	if( 0 == strnicmp( pFullUrl, "https://", 8 ) )
	{
		u16RetPort = 443;
	}

	if( ( false == isdigit( as8Url[0] ) && ( '[' != as8Url[0] ) && ( ':' != as8Url[0] ) ) )
	{
		// plain url 
		isDottedIp = false;
	}

	if( hasNetProtocol && (false == isDottedIp) )
	{
		// see if just plain url with no file 
		if( NULL == strchr( as8Url, '/' ) )
		{
			// no file part but may have port specified
			pTemp = (char *)strchr( as8Url, ':' );
			if( pTemp )
			{
				// has port
				*pTemp = 0;
				pTemp++;
				u16RetPort = (uint16_t)atoi( pTemp );
			}
			strRetHost = as8Url;
			return true;
		}
	}

	bool bHasCustomPort = false;
	if( '[' == as8Url[0] )
	{
		// its a ipv6 address
		pTemp = (char *)strchr( as8Url,']' );
		if( pTemp )
		{
			*pTemp = 0;
		}
		strRetHost = &as8Url[ 1 ];
		strcpy( &as8Url[0], pTemp + 1 );
		if( ':' == as8Url[0] )
		{
			bHasCustomPort = true;
		}
	}
	else
	{
		int iHostLen = (int)strlen( as8Url );
		for( int i = 0; i < iHostLen; i++ )
		{
			char c = as8Url[ i ];
			if( c == ':' ||
				c == '/' ||
				c == '\\' )
			{
				if( ':' == c )
				{
					bHasCustomPort = true;
				}
				as8Url[i] = 0;
				strRetHost = as8Url;
				as8Url[i] = c;
				break;
			}
		}
	}
	if( bHasCustomPort )
	{
		//see if has  custom Port 
		pTemp = strchr( as8Url, ':' );
		if ( pTemp !=NULL)
		{
			//had custom port so set it
			u16RetPort = (uint16_t) atoi( pTemp + 1 );
		}
	}

	// get the url
	int iHostLen = (int)strlen( as8Url );
	for( int i = 0; i < iHostLen; i++ )
	{
		if( ('/' == as8Url[ i ] ) ||
			('\\' == as8Url[ i ] ) )
		{
			strRetFileName = &as8Url[ i + 1 ];
			break;
		}
	}

	return true;
}

//============================================================================
void VxSetSktAllowReusePort( SOCKET skt )
{
#ifndef TARGET_OS_WINDOWS 
	// on windows need to don't do this or will allow binding to a port that is in use
	// on Linux this just lets you bind right after it has been released
	int reusePort = 1;
	if( SOCKET_ERROR == setsockopt( skt, SOL_SOCKET, SO_REUSEADDR, &reusePort, sizeof(int)) ) 
	{
		LogMsg( LOG_ERROR,  "VxSktBase::setReuseSocket error %d", VxGetLastError() );
	}
#endif // TARGET_OS_WINDOWS 
}

//============================================================================
SOCKET VxConnectTo( InetAddrAndPort& lclIp, InetAddrAndPort& rmtIp, uint16_t u16Port, int iConnectTimeoutMs, RCODE * retSktErr )
{
	if( 0 != retSktErr )
	{
		*retSktErr = 0;
	}

    SOCKET			sktHandle = INVALID_SOCKET;
	std::string		strRmtIp = rmtIp.toStdString();

	RCODE rc = 0;

	// Try all returned addresses until one works
#if USE_SKT_HINTS
    struct addrinfo * poAddrInfo;
    struct addrinfo * poResultAddr;
    struct addrinfo oHints;
    VxFillHints( oHints, false, false );

    char as8Port[ 16 ];
    sprintf( as8Port, "%d", u16Port );

    //LogMsg( LOG_INFO, "VxConnectTo port %d ms %d\n", u16Port, iConnectTimeoutMs ); 
    int error = getaddrinfo( strRmtIp.c_str(), as8Port, &oHints, &poAddrInfo );
    if( error != 0 )
    {
        if( 0 != retSktErr )
        {
            LogMsg( LOG_INFO, "VxConnectTo port %d ms %d getaddrinfo FAILED with error %d\n", u16Port, iConnectTimeoutMs, error );
            *retSktErr = error;
        }

        return INVALID_SOCKET;
    }

    for( poResultAddr = poAddrInfo; poResultAddr != NULL; poResultAddr = poResultAddr->ai_next )
    {
        sktHandle = socket( poResultAddr->ai_family, poResultAddr->ai_socktype, poResultAddr->ai_protocol );
        if( INVALID_SOCKET == sktHandle )
        {
            // create socket error
            rc = VxGetLastError();
            LogModule( eLogConnect, LOG_VERBOSE, "VxConnectTo: socket create error %s\n", VxDescribeSktError( rc ) );
            continue;
        }

        socklen_t sktAddrLen = sizeof( struct sockaddr_storage );
        sktHandle = VxConnectToAddr( sktHandle, poAddrInfo, sktAddrLen, iConnectTimeoutMs, retSktErr );
        if( INVALID_SOCKET != sktHandle )
        {
            break;
        }
    }
#else
    struct sockaddr_storage sktAddrStorage;
    memset( &sktAddrStorage, 0, sizeof( struct sockaddr_storage ) );

    struct sockaddr_in *sktAddrIn = ( struct sockaddr_in* )&sktAddrStorage;
    sktAddrIn->sin_family = AF_INET;
    sktAddrIn->sin_port = htons( ( unsigned short )( u16Port & 0xFFFF ) );
    sktAddrIn->sin_addr.s_addr = inet_addr( strRmtIp.c_str() );
    socklen_t sktAddrLen = sizeof( struct sockaddr_storage );
    struct sockaddr *sktAddr = ( struct sockaddr* )&sktAddrStorage;

    sktHandle = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if( INVALID_SOCKET != sktHandle )
    {
        sktHandle = VxConnectToAddr( sktHandle, sktAddr, sktAddrLen, iConnectTimeoutMs, retSktErr );
    }
#endif // USE_SKT_HINTS

    if( INVALID_SOCKET == sktHandle )
	{
        rc = VxGetLastError();
        LogModule( eLogConnect, LOG_VERBOSE, "VxConnectTo: socket connect error %s to %s", VxDescribeSktError( rc ), strRmtIp.c_str() );
        if( 0 != retSktErr )
        {
            if( 0 == *retSktErr )
            {
                *retSktErr = rc;
            }
        }
    }
    else
    {
        VxGetLclAddress( sktHandle, lclIp );
    }

    return sktHandle;
}

//============================================================================
std::string VxSktAddrToString( struct sockaddr* sktAddr, int sktAddrLen )
{
    std::string ipAndPort = "0.0.0.0:0";
    if( sktAddr )
    {
        struct sockaddr_in *sktAddrIn = ( struct sockaddr_in* )sktAddr;
        if( sktAddrIn->sin_family == AF_INET )
        {
            ipAndPort = inet_ntoa( sktAddrIn->sin_addr );
            ipAndPort += ":";
            ipAndPort += std::to_string( ntohs( sktAddrIn->sin_port ) );
        }
    }

    return ipAndPort;
}

//============================================================================
SOCKET VxConnectToAddr(SOCKET sktHandle, struct sockaddr* sktAddr, socklen_t sktAddrLen, int iConnectTimeoutMs, RCODE * retSktErr)
{
    if( INVALID_SOCKET == sktHandle )
    {
        LogModule( eLogConnect, LOG_ERROR, "VxConnectToAddr: invalid socket handle" );
        return INVALID_SOCKET;
    }

    if( !sktAddr )
    {
        LogModule( eLogConnect, LOG_ERROR, "VxConnectToAddr: null sktAddr" );
        return INVALID_SOCKET;
    }

    std::string ipAndPort = VxSktAddrToString( sktAddr, sktAddrLen );

    int64_t timeStartConnect = GetGmtTimeMs();
    //VxSetSktAllowReusePort( sktHandle );

    if( iConnectTimeoutMs > 0 )
    {
        //LogMsg( LOG_SKT, "VxConnectTo: timeout %d skt handle %d connect no block ip %s port %d\n", iConnectTimeoutMs, sktHandle, strRmtIp.c_str(), u16Port );
        // first try connect to the ip without timeout
        // set to non blocking
        ::VxSetSktBlocking( sktHandle, false );

        int iResult = connect( sktHandle, sktAddr, sktAddrLen );

        if( 0 == iResult )
        {
            // connected
            ::VxSetSktBlocking( sktHandle, true );
            LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: SUCCESS skt handle %d to %s in %d sec",
                       sktHandle, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ) );
            return sktHandle;
        }

        RCODE rc = VxGetLastError();
#ifdef TARGET_OS_WINDOWS
        if( rc != WSAEWOULDBLOCK )
#else
        if( rc != EINPROGRESS )
#endif // TARGET_OS_WINDOWS
        {
            // some other error other than need more time
            LogModule( eLogConnect, LOG_VERBOSE,  "VxConnectToAddr: skt connect error %s in %d seconds to %s\n",
                       VxDescribeSktError( rc ), TimeElapsedGmtSec( timeStartConnect ), ipAndPort.c_str() );

            VxCloseSktNow( sktHandle );
            return INVALID_SOCKET;
        }

        // needed more time so do connect with timeout
        ::VxSetSktBlocking( sktHandle, true );
        struct timeval tv;
        fd_set sktSet;

        do
        {
            LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: skt handle %d Attempt connect with timeout %d to %s\n", sktHandle, iConnectTimeoutMs, ipAndPort.c_str() );

            tv.tv_sec = iConnectTimeoutMs / 1000;
            tv.tv_usec = (iConnectTimeoutMs % 1000) * 1000;
            FD_ZERO( &sktSet );
            FD_SET( sktHandle, &sktSet );
            iResult = select( sktHandle+1,  // __nfds
                              NULL,         // read ready
                              &sktSet,      // write ready
                              NULL,         // exceptions
                              &tv );        // time spec

            if( iResult < 0 && TimeElapsedGmtMs( timeStartConnect ) < iConnectTimeoutMs)
            {
                LogMsg( LOG_ERROR, "VxConnectToAddr: ERROR socket select time out is %d but %lld ms elapsed to %s",
                        iConnectTimeoutMs, TimeElapsedGmtMs( timeStartConnect ), ipAndPort.c_str() );
            }

            if ( ( iResult > 0 ) && FD_ISSET(sktHandle, &sktSet ) )
            {
                LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: Connect Success skt handle %d to %s in %d sec",
                           sktHandle, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ));
                ::VxSetSktBlocking( sktHandle, true );
                return sktHandle;
            }
            else if (iResult < 0 && errno == EINTR)
            {
                /* We were interrupted by a signal.  Just indicate a
                   timeout even though we are early. */
                if( 0 != retSktErr )
                {
                    *retSktErr = errno;
                }

                LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: skt handle %d timeout %d select error %d to %s in %d sec",
                           sktHandle, iConnectTimeoutMs, errno, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ) );
                VxCloseSktNow( sktHandle );
                sktHandle = INVALID_SOCKET;
                break;
            }
            else if( ( iResult < 0 )
               && ( errno != 4 )
               && ( errno != 10004 ) )
            {
                LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: skt handle %d timeout %d select error %d to %s in %d sec",
                           sktHandle, iConnectTimeoutMs, errno, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ) );

                if( 0 != retSktErr )
                {
                    *retSktErr = errno;
                }

                VxCloseSktNow( sktHandle );
                sktHandle = INVALID_SOCKET;
                break;
            }
            else if( iResult > 0 )
            {
                // Socket selected for write
                socklen_t iSktLen = sizeof(int);
                int valopt = 0;
                if( getsockopt( sktHandle, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &iSktLen ) < 0 )
                {
                    rc = VxGetLastError();
                    LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: skt handle %d Error %d in getsockopt() to %s in %d sec",
                               sktHandle, rc, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ) );
                    VxCloseSktNow( sktHandle );
                    sktHandle = INVALID_SOCKET;
                    break;
                }
                else
                {
                    //LogMsg( LOG_INFO, "VxConnectTo: skt %d Selecting for write value returned %d\n", m_iSktId, valopt );
                    // Check the value returned...
                    if( valopt )
                    {
                        LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: skt handle %d timeout %d getsockopt valopt %d result %d in delayed connection to %s in %d sec\n",
                                   sktHandle, iConnectTimeoutMs, valopt, iResult, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ) );

                        if( 0 != retSktErr )
                        {
                            *retSktErr = valopt;
                        }

                        VxCloseSktNow( sktHandle );
                        sktHandle = INVALID_SOCKET;
                        break;
                    }
                    else
                    {
                        // connected
                        LogModule( eLogConnect, LOG_VERBOSE, "VxConnectTo: SUCCESS skt handle %d connected in delayed connection to %s in %d sec\n",
                                   sktHandle, ipAndPort.c_str(), TimeElapsedGmtSec( timeStartConnect ) );
                        ::VxSetSktBlocking( sktHandle, true );
                        break;
                    }
                }
            }
            else
            {
                LogModule( eLogConnect, LOG_VERBOSE, "VxConnectTo: skt handle %d connect timed out %d ... canceling at %d seconds to %s",
                           sktHandle, iConnectTimeoutMs, TimeElapsedGmtSec( timeStartConnect ), ipAndPort.c_str() );
                if( 0 != retSktErr )
                {
                    *retSktErr = -1;
                }

                VxCloseSktNow( sktHandle );
                sktHandle = INVALID_SOCKET;
                break;
            }
        } while (1);
    }
    else
    {
        // connect with no timeout
        LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: NO TIMEOUT skt handle %d connect no block to %s", sktHandle, ipAndPort.c_str() );

        ::VxSetSktBlocking( sktHandle, true );
        int iResult = connect( sktHandle, sktAddr, sktAddrLen );
        if( 0 != iResult )
        {
            // connect error
            RCODE rc = VxGetLastError();
            LogModule( eLogConnect, LOG_VERBOSE, "VxConnectToAddr: skt handle %d connect error %s in %d seconds to %s",
                            sktHandle,
                            VxDescribeSktError( rc ), TimeElapsedGmtSec( timeStartConnect ), ipAndPort.c_str() );
            if( 0 != retSktErr )
            {
                *retSktErr = rc;
            }

            VxCloseSktNow( sktHandle );
            sktHandle = INVALID_SOCKET;
        }
    }

    return sktHandle;
}

//============================================================================
SOCKET VxConnectToIPv6( const char * ipv6Str, uint16_t u16Port, int iConnectTimeoutMs, RCODE * retSktErr )
{
	fd_set			oSktSet; 
	socklen_t		iSktLen;
	struct timeval	tv;
	SOCKET			sktHandle;
	int             iResult = 0;
	std::string		strRmtIp = ipv6Str;

	RCODE rc = 0;

	struct addrinfo * poAddrInfo;
	struct addrinfo * poResultAddr;
	struct addrinfo oHints;
	VxFillHints( oHints, false, true );

	char as8Port[16];
	sprintf( as8Port, "%d", u16Port);

	//LogMsg( LOG_INFO, "VxConnectToIPv6 port %d ms %d\n", u16Port, iConnectTimeoutMs ); 
	int error = getaddrinfo( strRmtIp.c_str(), as8Port, &oHints, &poAddrInfo );
	if (error != 0) 
	{
		if( 0 != retSktErr )
		{
			LogMsg( LOG_INFO, "VxConnectToIPv6 port %d ms %d getaddrinfo FAILED with error %d\n", u16Port, iConnectTimeoutMs, error ); 
			*retSktErr = error;
		}

		return INVALID_SOCKET;
	}

	bool bConnectSuccess = false;
	// Try all returned addresses until one works
	for( poResultAddr = poAddrInfo; poResultAddr != NULL; poResultAddr = poResultAddr->ai_next ) 
	{
		//LogMsg( LOG_SKT, "VxConnectTo: create socket for ip %s port %d\n", strRmtIp.c_str(), u16Port );
		sktHandle = socket( poResultAddr->ai_family, poResultAddr->ai_socktype, poResultAddr->ai_protocol );
		if( INVALID_SOCKET == sktHandle )
		{
			// create socket error
			rc = VxGetLastError();
			LogMsg( LOG_INFO, "VxConnectTo: socket create error %s\n", VxDescribeSktError( rc ) );
			return sktHandle;
		}

		VxSetSktAllowReusePort( sktHandle );
		if( iConnectTimeoutMs )
		{
			//LogMsg( LOG_SKT, "VxConnectTo: timeout %d skt handle %d connect no block ip %s port %d\n", iConnectTimeoutMs, sktHandle, strRmtIp.c_str(), u16Port );
			// connect to the ip without timeout
			// set to non blocking
			::VxSetSktBlocking( sktHandle, false );
			//LogMsg( LOG_INFO, "VxConnectTo: skt %d attempt no block connect\n", m_iSktId );
			iResult = connect( sktHandle, poResultAddr->ai_addr, poResultAddr->ai_addrlen );
			if( 0 == iResult )
			{
				// connected
				::VxSetSktBlocking( sktHandle, true );
                if( IsLogEnabled( eLogSkt ) )
				    LogMsg( LOG_DEBUG, "VxConnectTo: SUCCESS skt handle %d connect no block ip %s port %d\n", sktHandle, strRmtIp.c_str(), u16Port );

				bConnectSuccess = true;
				goto connected;
			}

			rc = VxGetLastError();
#ifdef TARGET_OS_WINDOWS
			if (rc != WSAEWOULDBLOCK)
#else
			if( rc != EINPROGRESS )
#endif // TARGET_OS_WINDOWS
			{
				// some other error other than need more time
                if( IsLogEnabled( eLogSkt ) )
				    LogMsg( LOG_DEBUG, "VxConnectTo: skt connect to %s %d error %s\n", strRmtIp.c_str(), u16Port, VxDescribeSktError( rc ) );

				VxCloseSktNow( sktHandle );
				continue;
			}

			// needed more time so do connect with timeout
			::VxSetSktBlocking( sktHandle, true );
			do
			{ 
                if( IsLogEnabled( eLogSkt ) )
				    LogMsg( LOG_DEBUG, "VxConnectTo: skt handle %d Attempt connect with timeout %d\n", sktHandle, iConnectTimeoutMs );

				tv.tv_sec = iConnectTimeoutMs / 1000; 
				tv.tv_usec = (iConnectTimeoutMs % 1000) * 1000; 
				FD_ZERO(&oSktSet); 
				FD_SET(sktHandle, &oSktSet); 
				iResult = select(sktHandle+1, NULL, &oSktSet, NULL, &tv); 
				//LogMsg( LOG_INFO, "VxConnectTo: skt %d Attempt connect with timeout result %d\n", m_iSktId, iResult );
				if (iResult < 0 && 
					(errno != 4) &&
					(errno != 10004)  )

				{ 
					// error
                    if( IsLogEnabled( eLogSkt ) )
					    LogMsg( LOG_INFO, "VxConnectTo: skt handle %d timeout %d select error %d\n", sktHandle, iConnectTimeoutMs, errno );

					if( 0 != retSktErr )
					{
						*retSktErr = errno;
					}

					VxCloseSktNow( sktHandle );
					break;
				} 
				else if( iResult > 0 )
				{ 
					//LogMsg( LOG_INFO, "VxConnectTo: skt %d Selecting for write\n", m_iSktId );
					// Socket selected for write 
					iSktLen = sizeof(int); 
					socklen_t valopt = 0;
					if( getsockopt(sktHandle, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &iSktLen) < 0 )
					{ 
                        if( IsLogEnabled( eLogSkt ) )
						    LogMsg( LOG_INFO, "VxConnectTo: skt handle %d Error %d in getsockopt() to ip %s\n", sktHandle, VxGetLastError(), strRmtIp.c_str() );

						VxCloseSktNow( sktHandle );
						break;
					} 
					else
					{
						//LogMsg( LOG_INFO, "VxConnectTo: skt %d Selecting for write value returned %d\n", m_iSktId, valopt );
						// Check the value returned... 
						if( valopt ) 
						{ 
                            if( IsLogEnabled( eLogSkt ) )
							    LogMsg( LOG_DEBUG, "VxConnectTo: skt handle %d Error %d in delayed connection to ip %s\n", sktHandle, valopt, strRmtIp.c_str() );

							VxCloseSktNow( sktHandle );
							break;
						}
						else
						{
							// connected
                            if( IsLogEnabled( eLogSkt ) )
							    LogMsg( LOG_INFO, "VxConnectTo: SUCCESS skt handle %d connected in delayed connection to ip %s\n", sktHandle, strRmtIp.c_str() );

							::VxSetSktBlocking( sktHandle, true );
							bConnectSuccess = true;
							break;
						} 
					}
				} 
				else 
				{ 
                    if( IsLogEnabled( eLogSkt ) )
					    LogMsg( LOG_DEBUG, "VxConnectTo: skt handle %d connect to %s port %d timed out.. canceling\n", sktHandle, strRmtIp.c_str(), u16Port );

					VxCloseSktNow( sktHandle );
					break; 
				} 
			} while (1); 
			if( bConnectSuccess )
			{
				break;
			}
		}
		else
		{
            if( IsLogEnabled( eLogSkt ) )
			    LogMsg( LOG_DEBUG, "VxConnectTo: NO TIMEOUT skt handle %d connect no block ip %s port %d\n", sktHandle, strRmtIp.c_str(), u16Port );

			// connect to the ip without timeout
			iResult = connect( sktHandle, poResultAddr->ai_addr, poResultAddr->ai_addrlen );
			if( 0 != iResult )
			{
				// connect error
				rc = VxGetLastError();
                if( IsLogEnabled( eLogSkt ) )
				    LogMsg( LOG_DEBUG, "VxConnectTo: skt handle %d connect to %s error %s\n",
					        sktHandle,
					        strRmtIp.c_str(),
					        VxDescribeSktError( rc ) );

				VxCloseSktNow( sktHandle );
				return INVALID_SOCKET;
			}
			else
			{
				bConnectSuccess = true;
				break;
			}
		}
	}
connected:
	if( bConnectSuccess )
	{
		return sktHandle;
	}
	else
	{
		return INVALID_SOCKET;
	}
}

//============================================================================
SOCKET VxConnectTo(		InetAddrAndPort&	lclIp,
						InetAddrAndPort&	rmtIp,
						const char *		pIpAddr,				// remote ip
						uint16_t			u16Port,				// port to connect to
						int					iTimeoutMilliSeconds,
						RCODE *				retSktError )	// milli seconds before connect attempt times out
{
	if( VxIsIPv4Address( pIpAddr) || VxIsIPv6Address( pIpAddr) )
	{
		rmtIp.setIp( pIpAddr );
        rmtIp.setPort( u16Port );
	}
    else if( VxResolveHostToIp( pIpAddr, u16Port, rmtIp ) )
    {
        rmtIp.setPort( u16Port );
    }
    else
	{
        LogModule( eLogConnect, LOG_INFO, "VxConnectTo: FAILED to resolve %s\n", pIpAddr );
		return INVALID_SOCKET;
	}

	VxTimer connectToTimer;
	SOCKET sktHandle = VxConnectTo( lclIp, rmtIp, u16Port, iTimeoutMilliSeconds, retSktError );
	if( INVALID_SOCKET == sktHandle )
	{
        LogModule( eLogConnect, LOG_DEBUG, "VxConnectTo: FAIL connect %3.3f sec lcl ip %s:%d to %s:%d timeout %d error %d thread 0x%x",
	               connectToTimer.elapsedSec(), lclIp.toStdString().c_str(), lclIp.getPort(), rmtIp.toStdString().c_str(), u16Port, iTimeoutMilliSeconds, retSktError ? *retSktError : -1, VxGetCurrentThreadId() );
	}
	else
	{
        LogModule( eLogConnect, LOG_DEBUG, "VxConnectTo: SUCCESS connect %3.3f sec lcl ip %s:%d to %s:%d thread 0x%x",
			       connectToTimer.elapsedSec(), lclIp.toStdString().c_str(), lclIp.getPort(), rmtIp.toStdString().c_str(), u16Port, VxGetCurrentThreadId());
	}

	return sktHandle;
}

//============================================================================
// connects to website and returns socket.. if fails returns INVALID_SOCKET
SOCKET VxConnectToWebsite(	InetAddrAndPort&	lclIp,			// ip of adapter to use
							InetAddrAndPort&	rmtIp,			// return ip and port url resolves to
							const char *		pWebsiteUrl,
							std::string&		strHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
							std::string&		strFile,		// return file name.. images/me.jpg
							uint16_t&			u16Port,		// return port
							int					iConnectTimeoutMs )
{
	// split host name from file path
	bool bSplitOk = VxSplitHostAndFile(	pWebsiteUrl,	// full url.. example http://www.mysite.com/index.html or www.mysite.com/images/me.jpg
										strHost,		// return host name.. example http://www.mysite.com/index.htm returns www.mysite.com
										strFile,		// return file name.. images/me.jpg
										u16Port );		// return port if specified else return 80 as default
	if( false == bSplitOk )
	{
		LogMsg( LOG_ERROR, "VxConnectToWebsite: Invalid URL %s\n", pWebsiteUrl );
		return INVALID_SOCKET;
	}

	const char * hostname = strHost.c_str();
	//LogMsg( LOG_VERBOSE, "VxConnectToWebsite: Attempting to connect to %s port %d...\n", hostname, u16Port );

	SOCKET hSocket = VxConnectTo(	lclIp,					// ip of adapter to use
									rmtIp,					// return ip and port url resolves to
									hostname,				// remote ip or url
									u16Port,				// port to connect to
									iConnectTimeoutMs );	// timeout attempt to connect
	if( INVALID_SOCKET == hSocket )
	{
		//LogMsg( LOG_ERROR, "VxConnectToWebsite: failed to connect to %s %s\n", hostname, VxDescribeSktError( VxGetLastError() ) );
		return INVALID_SOCKET;
	}

	//LogMsg( LOG_VERBOSE, "VxConnectToWebsite: Connected to %s:%d...\n", hostname, u16Port );
	return hSocket;
}

//============================================================================
bool VxBindSkt( SOCKET oSocket, InetAddress & oLclAddr, uint16_t u16Port )
{
	struct sockaddr_storage 		oSktAddr;		// listen socket address
	oLclAddr.fillAddress( oSktAddr, u16Port );
	return VxBindSkt( oSocket, &oSktAddr );
}

//============================================================================
bool VxTestConnectionOnSpecificLclAddress( InetAddress& oLclAddr )
{
	bool bConnectionOk = false;
	std::string strLclAddr = oLclAddr.toStdString();

	InetAddress oRmtAddr;
	if ( VxResolveUrl( "www.nolimitconnect.com", 80, oRmtAddr ) )
	{
		// attempt connect
		// Open a socket with the correct address family for this address.
		SOCKET oVxSktConnect = socket( oRmtAddr.isIPv4() ? PF_INET : PF_INET6, SOCK_STREAM, 0 );
		if ( oVxSktConnect == INVALID_SOCKET )
		{
			LogMsg( LOG_INFO, "TestConnectionOnSpecificLclAddress: failed to create socket\n" );
		}
		else
		{
#if USE_BIND_LOCAL_IP
            struct sockaddr_storage oLclSktStorage;
            oLclAddr.fillAddress( oLclSktStorage, 0 );
			if ( false == VxBindSkt( oVxSktConnect, &oLclSktStorage ) )
			{
				LogMsg( LOG_INFO, "TestConnectionOnSpecificLclAddress: failed to bind skt with ip %s\n", strLclAddr.c_str() );
			}
			else
			{
#endif // USE_BIND_LOCAL_IP
				struct sockaddr_storage oRmtSktAddr;
				int iRmtSktAddrLen = oRmtAddr.fillAddress( oRmtSktAddr, 80 );

				int result = connect( oVxSktConnect, ( struct sockaddr * )&oRmtSktAddr, iRmtSktAddrLen );
				if ( 0 == result )
				{
					bConnectionOk = true;
				}
				else
				{
					result = VxGetLastError();
					LogMsg( LOG_ERROR, "TestConnectionOnSpecificLclAddress: connect error %s\n", VxDescribeSktError( result ) );
				}
#if USE_BIND_LOCAL_IP
			}
#endif // USE_BIND_LOCAL_IP
			::VxCloseSkt( oVxSktConnect );
		}
	}
	return bConnectionOk;
}

//============================================================================
bool VxResolveUrl( const char * pUrl, uint16_t u16Port, InetAddress& oRetAddr )
{
	bool bResult = false;
	oRetAddr.setToInvalid();

	// get addresses
	struct addrinfo Hints;
	struct addrinfo * AI;
	struct addrinfo * AddrInfo;

	memset( &Hints, 0, sizeof( Hints ) );

#ifdef TARGET_OS_WINDOWS
	Hints.ai_family = PF_UNSPEC;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;
#else
	Hints.ai_family = AI_PASSIVE | AI_ADDRCONFIG;
	Hints.ai_socktype = SOCK_STREAM;
	Hints.ai_protocol = IPPROTO_TCP;
#endif

	char as8Buf[ 32 ];
	sprintf( as8Buf, "%d", u16Port );

	//LogMsg( LOG_INFO, "VxResolveUrl %s:%d\n", pUrl, u16Port ); 
	int RetVal = getaddrinfo( pUrl, as8Buf, &Hints, &AddrInfo );
	if ( RetVal != 0 )
	{
		LogMsg( LOG_ERROR, " getaddrinfo() failed with error %d\n", RetVal );
	}
	else
	{
		for ( AI = AddrInfo; AI != NULL; AI = AI->ai_next )
		{
			if ( ( AI->ai_family != PF_INET ) && ( AI->ai_family != PF_INET6 ) )
			{
				continue;
			}
			struct sockaddr_storage * poSktAddr = ( struct sockaddr_storage * )AI->ai_addr;
			oRetAddr.setIp( *poSktAddr );
			std::string strIp = oRetAddr.toStdString();
			if ( oRetAddr.isIPv6() )
			{
				continue;
			}
			bResult = true;
			break;
		}
		freeaddrinfo( AddrInfo ); // free the linked list
	}
	return bResult;
}

//============================================================================
bool VxIsIPv6Address( const char *addr )
{
	if ( NULL == addr )
	{
		return false;
	}
	std::string addrStr = addr;
	if ( addrStr.find( ":" ) != std::string::npos )
	{
		return true;
	}
	return false;
}


//============================================================================
bool VxIsIPv4Address( const char *addr )
{
	if ( NULL == addr )
	{
		return false;
	}
	int strLen = strlen( addr );
	for ( int i = 0; i < strLen; i++ )
	{
		if ( !isdigit( addr[ i ] ) && ( '.' != addr[ i ] ) )
		{
			return false;
		}
	}

	return true;
}

//============================================================================
int VxGetIPv6ScopeID( const char *addr )
{
	if ( false == VxIsIPv6Address( addr ) )
	{
		return 0;
	}
	std::string addrStr = addr;
	int pos = (int)addrStr.find( "%" );
	if ( pos == (int)std::string::npos )
	{
		return 0;
	}
	std::string scopeStr = addrStr.substr( pos + 1, addrStr.length() );
	return atoi( scopeStr.c_str() );
}

//============================================================================	
RCODE VxGetRmtAddress( SOCKET sktHandle, InetAddrAndPort& oRetAddr )
{
	// Get the IP address of the the remote side of connection
	RCODE rc = 0;
	struct sockaddr oSktAddr;
	socklen_t iSktAddrLen = sizeof( oSktAddr );
	memset( &oSktAddr, 0, sizeof( oSktAddr ) );

	if ( getpeername( sktHandle, ( struct sockaddr* )&oSktAddr, &iSktAddrLen ) )
	{
		// error occurred
		oRetAddr.setToInvalid();
		rc = VxGetLastError();

        //if( IsLogEnabled( eLogSkt ) )
            LogMsg( LOG_DEBUG, "VxGetRmtAddress: skt handle %d error %d %s\n", sktHandle, rc, VxDescribeSktError( rc ) );
	}
	else
	{
		oRetAddr.setIpAndPort( oSktAddr );
	}

	return rc;
}

//============================================================================
RCODE VxGetLclAddress( SOCKET sktHandle, InetAddrAndPort& oRetAddr )
{
	// Get the IP address of the the local side of connection
	RCODE rc = 0;
	struct sockaddr oSktAddr;
    memset( &oSktAddr, 0, sizeof( struct sockaddr ) );
	socklen_t iSktAddrLen = sizeof( struct sockaddr );

    if( getsockname( sktHandle, ( struct sockaddr* )&oSktAddr, &iSktAddrLen ) )
	{
		// error occurred
		oRetAddr.setToInvalid();
		rc = VxGetLastError();
        LogMsg( LOG_DEBUG, "VxGetRmtAddress: skt handle %d error %d %s\n", sktHandle, rc, VxDescribeSktError( rc ) );
	}
	else
	{
		oRetAddr.setIpAndPort( oSktAddr );
	}

	return rc;
}

//============================================================================
std::string VxGetRmtHostName( SOCKET& skt )
{
	struct sockaddr_in	oSktAddr;
	socklen_t			sktAddrlen;
	struct hostent		*hp;
	char				hostName[ 255 ];

	hostName[ 0 ] = 0;
	// get peer address/port
	sktAddrlen = sizeof( oSktAddr );
	memset( &oSktAddr, 0, sizeof( oSktAddr ) );
	if ( 0 > getpeername( skt, ( struct sockaddr * )&oSktAddr, &sktAddrlen ) )
	{
		strcpy( hostName, "UNKNOWN" );
		VxReportHack( eHackerLevelSuspicious, eHackerReasonPeerName, skt, hostName, "HACK ALERT:getpeername failed: %.100s",
					  strerror( errno ) );
	}
	else
	{
		// get host name
		hp = gethostbyaddr( (char *)&oSktAddr.sin_addr, sizeof( struct in_addr ), oSktAddr.sin_family );
		if ( hp )
		{
			strncpy( hostName, hp->h_name, sizeof( hostName ) );
			hostName[ sizeof( hostName ) - 1 ] = '\0';

			// convert to all lower case
			for ( int i = 0; hostName[ i ]; i++ )
				if ( isupper( hostName[ i ] ) )
					hostName[ i ] = tolower( hostName[ i ] );

			// Map it back to an IP address and check that the given address actually
			// is an address of this host as a security check against spoofing a ip
			hp = gethostbyname( hostName );
			if ( !hp )
			{
				strcpy( hostName, inet_ntoa( oSktAddr.sin_addr ) );
				VxReportHack( eHackerLevelSuspicious, eHackerReasonHostByName, skt, hostName, "reverse mapping checking gethostbyname for %.700s failed",
							  hostName );
			}
			else
			{
				// find the address from the list of addresses
				int addrIdx;
				for ( addrIdx = 0; hp->h_addr_list[ addrIdx ]; addrIdx++ )
					if ( 0 == memcmp( hp->h_addr_list[ addrIdx ], &oSktAddr.sin_addr, sizeof( oSktAddr.sin_addr ) ) )
						break;
				if ( !hp->h_addr_list[ addrIdx ] )
				{
					// address not found for the host name 
					strcpy( hostName, inet_ntoa( oSktAddr.sin_addr ) );
					VxReportHack( eHackerLevelSuspicious, eHackerReasonNoHostIpAddr, skt, hostName, "Address %.100s maps to %.600s, but does not map back to the address",
								  hostName );
				}
			}
		}
		else
		{
			// Host name not found. use ip address. 
			strcpy( hostName, inet_ntoa( oSktAddr.sin_addr ) );
			LogMsg( LOG_ERROR, "Could not find host name of address %.100s.", hostName );
		}
	}

#ifdef IP_OPTIONS
	// If IP options are supported, make sure there are none 
	// options can be used to pretend you are from a ip address you are not from
	// close any connection with options
	unsigned char		options[ 200 ];
	unsigned char *		ucp;
	char text[ 1024 ], *cp;
	socklen_t			optionLen;
	int					ipproto;
	struct protoent *	ip;

	if ( NULL != ( ip = getprotobyname( "ip" ) ) )
		ipproto = ip->p_proto;
	else
		ipproto = IPPROTO_IP;
	optionLen = sizeof( options );
	if ( ( 0 >= getsockopt( skt, ipproto, IP_OPTIONS, (char *)options, &optionLen ) )
		 && ( 0 != optionLen ) )
	{
		cp = text;
		if ( optionLen > 256 )
			optionLen = 256;
		//NOTE: buffer must be at least 3 times as big as options
		for ( ucp = options; optionLen > 0; ucp++, optionLen--, cp += 3 )
			sprintf( cp, " %2.2x", *ucp );
		VxReportHack( eHackerLevelSuspicious, eHackerReasonHostIpOptions, skt, hostName, "Connection from %.100s with IP options:%.800s",
					  inet_ntoa( oSktAddr.sin_addr ), text );
		VxCloseSktNow( skt );
	}
#endif

	return std::string( hostName );
}

//============================================================================
std::string VxGetRemoteIpAddress( SOCKET skt )
{
	struct sockaddr oSktAddr;
	socklen_t addrLen = sizeof( oSktAddr );
	memset( &oSktAddr, 0, sizeof( oSktAddr ) );
	if ( 0 > getpeername( skt, ( struct sockaddr * )&oSktAddr, &addrLen ) )
	{
		return std::string( "UNKNOWN" );
	}

	return VxIpToString( &oSktAddr );
}

//============================================================================
//! receive data.. if timeout is set then will keep trying till buffer is full or error or timeout expires
RCODE VxReceiveSktData( SOCKET&			sktHandle,
						char *			pRetBuf,				// buffer to receive data into
						int				iBufLenIn,				// length of buffer
						int *			iRetBytesReceived,		// number of bytes actually received
						int				iTimeoutMilliSeconds,	// milliseconds before receive attempt times out ( 0 = dont wait )
						bool			bAbortIfCrLfCrLf,		// if true then abort receive when \r\n\r\n is received
						bool *			pbRetGotCrLfCrLf )		// if received \r\n\r\n set to true
{
	RCODE m_rcLastError = 0; // clear out any previous error
	*iRetBytesReceived = 0;
	if ( pbRetGotCrLfCrLf )
	{
		*pbRetGotCrLfCrLf = 0;
	}

	int iBufLen = iBufLenIn;
	if ( bAbortIfCrLfCrLf )
	{
		// reserve a little space for /r/n etc if doing strings.. else try to do exact amount
		iBufLen -= 8;
	}

	int	iRecvResult;
	vx_assert( pRetBuf );
	vx_assert( iBufLen > 0 );
	vx_assert( iRetBytesReceived );
	::VxSetSktBlocking( sktHandle, false );

	VxTimer		oTimer;
    int64_t timeStart = GetApplicationAliveMs();
	do
	{
		iRecvResult = recv( sktHandle,		// socket
							pRetBuf,		// buffer to read into
							iBufLen,		// length of buffer space
							0 );			// flags

		if ( 0 > iRecvResult )
		{
			iRecvResult = VxGetLastError();
			//LogMsg( LOG_INFO, "VxSktCode::ReceiveData: error %d timeout %d\n", iDataLen, iTimeoutMilliSeconds );
#ifdef TARGET_OS_WINDOWS
			if ( (( 11 == iRecvResult ) || ( WSAEWOULDBLOCK == iRecvResult )) && iTimeoutMilliSeconds )
#else
			if ( (( 11 == iRecvResult ) || ( EINPROGRESS == iRecvResult )) && iTimeoutMilliSeconds )
#endif // TARGET_OS_WINDOWS
			{
				// see if have timeout value
				if ( iTimeoutMilliSeconds )
				{
					if ( iTimeoutMilliSeconds < oTimer.elapsedMs() )
					{
						// timeout
						break;
					}
					else
					{
						VxSleep( 10 );
						continue;
					}
				}
				else
				{
					// no timeout.. if any data already received then bail with the data received
					if ( *iRetBytesReceived > 0 )
					{
						break;
					}
				}

				VxSleep( 20 );
				continue;
			}

			// not in progress so is a bad error
			m_rcLastError = iRecvResult;
			return m_rcLastError;
		}

		if ( INVALID_SOCKET == sktHandle )
		{
			m_rcLastError = VxGetLastError();
			LogMsg( LOG_INFO, "VxSktCode:recv error %d %s thread 0x%x", m_rcLastError, VxDescribeSktError( m_rcLastError ), VxGetCurrentThreadId() );
			return m_rcLastError;
		}

		if ( 0 == iRecvResult )
		{
			// may not of had time to get some data
			// see if have timeout value
			if ( iTimeoutMilliSeconds )
			{
				if ( iTimeoutMilliSeconds < oTimer.elapsedMs() )
				{
					// timeout
					break;
				}
				else
				{
					VxSleep( 10 );
					continue;
				}
			}
		}

		// got some data
		*iRetBytesReceived += iRecvResult;
		iBufLen -= iRecvResult;
		if ( bAbortIfCrLfCrLf )								// if abort when \r\n\r\n is received 
		{
			// null terminate the string
			pRetBuf[ iRecvResult ] = 0;
			if ( strstr( pRetBuf, "\r\n\r\n" ) )
			{
				// all done
				if ( pbRetGotCrLfCrLf )
				{
					*pbRetGotCrLfCrLf = true;
				}
			}
		}

		// advance to end of received data for next receive
		pRetBuf += iRecvResult;
		if( ( 0 >= iBufLen ) ||
			( pbRetGotCrLfCrLf && *pbRetGotCrLfCrLf ) )
		{
			// all full or got a CrLfCrLf
			break;
		}

		if( iTimeoutMilliSeconds )
		{
			if( iTimeoutMilliSeconds < oTimer.elapsedMs() )
			{
				// timeout
				break;
			}
			else
			{
				continue;
			}
		}

        if( iRecvResult > 0 )
        {
            // reset timeout if we have recieved at least some data this pass
            oTimer.startTimer();
        }
	} while ( ( iRecvResult > 0 ) || ( iTimeoutMilliSeconds && ( iTimeoutMilliSeconds > oTimer.elapsedMs() ) ) );

    int64_t timeEnd = GetApplicationAliveMs();
    LogMsg( LOG_VERBOSE, "VxReceiveSktData len %d in %lld ms with timeout %d ms thread 0x%x", iRecvResult, timeEnd - timeStart, iTimeoutMilliSeconds, VxGetCurrentThreadId() );

	return m_rcLastError;
}

//============================================================================
bool VxBindSkt( SOCKET oSocket, struct sockaddr_storage * poAddr )
{
	if ( SOCKET_ERROR == bind( oSocket, ( struct sockaddr * )poAddr, VxGetSktAddressLength( poAddr ) ) )
	{
		LogMsg( LOG_ERROR, "ERROR Bind Socket Failed rc = %d\n", VxGetLastError() );
		return false;
	}
	return true;
}

//============================================================================
// C functions
//============================================================================
GOTV_BEGIN_CDECLARES

//============================================================================
//! set socket to blocking or not
RCODE VxSetSktBlocking( SOCKET sktHandle, bool bBlock )
{
	//LogMsg( LOG_INFO, "VxSktBase::setSktBlocking %d skt %d\n", bBlock, this->m_iSktId );
	if ( bBlock )
	{
		// set to non blocking
#if defined(TARGET_OS_WINDOWS)
		int iResult;
		u_long s32NonBlock = false;
		iResult = ioctlsocket( sktHandle, FIONBIO, &s32NonBlock );
		if ( iResult != 0 )
		{
			LogMsg( LOG_ERROR, "VxSktBase::setSktBlocking skt %d ioctlsocket error %s\n", sktHandle, VxDescribeSktError( iResult ) );
			return iResult;
		}

#elif defined(TARGET_OS_LINUX)
		// this works in linux but not android
        int result = fcntl(sktHandle, F_SETFL, fcntl(sktHandle, F_GETFL) & ~O_NONBLOCK);
        if( result )
        {
            LogMsg( LOG_ERROR, "VxSktBase::setSktBlocking fcntl error\n" );
            return -1;
        }
#elif defined(TARGET_OS_ANDROID)
		// this works in android
		int mode = 0;
		ioctl( sktHandle, FIONBIO, &mode );
#endif
	}
	else
	{
		// set to non blocking
#ifdef TARGET_OS_WINDOWS
		u_long s32NonBlock = true;
		int iResult;
		iResult = ioctlsocket( sktHandle, FIONBIO, &s32NonBlock );
		if ( iResult != 0 )
		{
			LogMsg( LOG_INFO, "VxSktBase::setSktBlocking ioctlsocket error %s\n", VxDescribeSktError( iResult ) );
			return iResult;
		}
#elif defined(TARGET_OS_LINUX)
		// this works in linux but not android
        int result = fcntl(sktHandle, F_SETFL, fcntl(sktHandle, F_GETFL) | O_NONBLOCK);
        if( result )
        {
            LogMsg( LOG_ERROR, "VxSktBase::setSktBlocking fcntl error\n" );
            return -1;
        }

#elif defined(TARGET_OS_ANDROID)
		// this works in android
		int mode = 1;
		ioctl( sktHandle, FIONBIO, &mode );
#endif
	}

	//LogMsg( LOG_INFO, "VxSktBase::setSktBlocking %d skt %d done\n", bBlock, this->m_iSktId );
	return 0;
}

//============================================================================
void VxFlushThenCloseSkt( SOCKET oSocket )
{
	if( INVALID_SOCKET != oSocket )
	{
		#ifdef TARGET_OS_WINDOWS
			// set linger time to give socket time to send
			linger oLinger;
			oLinger.l_linger = 20; // how long to linger
			oLinger.l_onoff = 1;
			setsockopt(	oSocket,                 
				(int) IPPROTO_TCP,   //level             
				(int) SO_LINGER,              
				(const char *)&oLinger,  
				(int)sizeof( linger ) );
			// windows crap is broken .. this is the workaround
			// from http://msdn.microsoft.com/en-us/library/ms738547(VS.85).aspx

			//Anyone who wants to do a graceful shutdown (with IOCP in my case) should
			//look at bug 4468997 on the Sun Developers Network.
			//Windows closesocket() is completely broken and actually doesn't work properly.
			//The SO_LINGER and SO_DONTLINGER options also do not work.
			//See:
			//http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4468997
			//What DOES work is it do a
			//DisconnectEx(sock, NULL, 0, 0);
			//closesocket(sock)
			//and this is the only way to get a proper shutdown without clipping trailing
			//bytes from the stream you have just written.
		/*
			LPFN_DISCONNECTEX pdisc = NULL; 
			DWORD dwBytesReturned; 
			GUID guidDisconnectEx = WSAID_DISCONNECTEX; 
			WSAIoctl( oSocket, 
				SIO_GET_EXTENSION_FUNCTION_POINTER, 
				&guidDisconnectEx, 
				sizeof(GUID), 
				&pdisc, 
				sizeof(pdisc), 
				&dwBytesReturned, 
				NULL, 
				NULL); 

			if( pdisc )
			{
				//pdisc(oSocket, NULL, TF_REUSE_SOCKET, NULL); 
				pdisc(oSocket, NULL, TF_WRITE_BEHIND, NULL);		
			}
			*/
			shutdown (oSocket, SD_SEND );
		#else
			//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d setting linger\n", m_iSktId );
			// set linger time to give socket time to send
			linger oLinger;
			oLinger.l_linger = 1;
			oLinger.l_onoff = 1;
			setsockopt(	oSocket,                 
				(int) IPPROTO_TCP,   //level             
				(int) SO_LINGER,              
				(const char *)&oLinger,  
				(int)sizeof( linger ) );
			//The shutdown command has three options: 0 = done receiving, 1 = done sending, 2 = both
			//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d done setting linger\n", m_iSktId );
			shutdown(oSocket,0);
			//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d shutdown complete\n", m_iSktId );
		#endif
		oSocket = INVALID_SOCKET;
	}
}

//============================================================================
void VxCloseSktNow( SOCKET oSocket )
{
	if( INVALID_SOCKET != oSocket )
	{
		//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d force close start\n", m_iSktId );
		// set linger time to zero to force a close right now
		linger oLinger;
		oLinger.l_linger = 0;
		oLinger.l_onoff = 1;
		setsockopt(	oSocket,                 
			(int) IPPROTO_TCP,   //level             
			(int) SO_LINGER,              
			(const char *)&oLinger,  
			(int)sizeof( linger ) );

			shutdown(oSocket, 0);
		#ifdef TARGET_OS_WINDOWS
			closesocket( oSocket );
		#else // LINUX
			close( oSocket );
		#endif //LINUX
		oSocket = INVALID_SOCKET;
	}
	//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d force close done\n", m_iSktId );
}

//============================================================================
void VxCloseSkt( SOCKET oSocket )
{
	if( INVALID_SOCKET != oSocket )
	{
		#ifdef TARGET_OS_WINDOWS
			closesocket( oSocket );
		#else // LINUX
			close( oSocket );
		#endif //LINUX
		oSocket = INVALID_SOCKET;
	}
}


//============================================================================
socklen_t VxGetSktAddressLength( struct sockaddr_storage * poAddr )
{
    socklen_t oAddrLen = 0;
    switch( poAddr->ss_family )
    {
    case AF_INET:
        oAddrLen = sizeof(struct sockaddr_in);
        break;

    case AF_INET6:
        oAddrLen = sizeof(struct sockaddr_in6);
        break;

    default:
        LogMsg( LOG_FATAL, "GetSktAddressLength: Invalid Address Family %d", poAddr->ss_family);
        vx_assert(false);
    }

    return oAddrLen;
}

//============================================================================
void VxSetSktAddressPort( struct sockaddr_storage * poAddr, uint16_t u16Port )
{
    switch( poAddr->ss_family )
    {
    case AF_INET:
        ((struct sockaddr_in *)poAddr)->sin_port = htons(u16Port);
        break;

    case AF_INET6:
        ((struct sockaddr_in6 *)poAddr)->sin6_port = htons(u16Port);
        break;

    default:
        LogMsg( LOG_FATAL, "SetSktAddressPort: Invalid Address Family %d", poAddr->ss_family);
        vx_assert(false);
    }
}


//============================================================================
RCODE VxSendSktData(	SOCKET			sktHandle,
						const char *	pData,					// data to send
						int				iDataLen,				// length of data
						int				iTimeoutSeconds )		// seconds before send attempt times out
{
	vx_assert( iDataLen > 0 );
	int iSendResult		= 0;
	RCODE m_rcLastError		= 0;
	if( iTimeoutSeconds )
	{
		do
		{
			if( iTimeoutSeconds )
			{
				fd_set	oFdSet;
				FD_ZERO( &oFdSet );
				FD_SET( sktHandle, &oFdSet );

				// set timeout val
				struct timeval		oTimeVal;
				oTimeVal.tv_sec		= iTimeoutSeconds;
				oTimeVal.tv_usec	= 0;

				int iSelectResult = select( (int)sktHandle + 1, NULL, &oFdSet, NULL, &oTimeVal );
				if( 0 >= iSelectResult )
				{
					//select timed out or select error occurred
					m_rcLastError = VxGetLastError();
					if( 0 == m_rcLastError )
					{
						m_rcLastError = iSelectResult;
					}
					//LogMsg( LOG_INFO, "VxSktCode:sendData: Send to %s Select Error %d\n", getRemoteIp(), iSelectResult );
					VxCloseSktNow(sktHandle);
					return m_rcLastError;
				}
			}
			// send the data
#ifdef TARGET_OS_WINDOWS
			iSendResult =  send( sktHandle, pData, iDataLen, 0);
#else // LINUX
			// fix so broken pipe does not cause app to exit ( code 141 )
			iSendResult  =  send( sktHandle, pData, iDataLen, MSG_NOSIGNAL );
#endif // TARGET_OS_WINDOWS
			if( 0 >= iSendResult )
			{
				// send error
				m_rcLastError = VxGetLastError();
				if( 0 == m_rcLastError )
				{
					m_rcLastError = iSendResult;
				}
				VxCloseSktNow(sktHandle);
				return m_rcLastError;
			}
			pData += iSendResult;
			iDataLen -= iSendResult;
			if( 0 >= iDataLen )
			{
				// all sent
				break;
			}
			// not all was sent
			VxSleep( 10 ); 
			//if( false == this->isConnected() )
			//{
			//	LogMsg( LOG_INFO, "VxSktCode::sendData: attempted send on disconnected socket\n" );
			//	return -1;
			//}
		}while( iDataLen > 0 );
	}
	else
	{
		// send the data
#ifdef TARGET_OS_WINDOWS
		iSendResult =  send( sktHandle, pData, iDataLen, 0);
#else // LINUX
		// fix so broken pipe does not cause app to exit ( code 141 )
		iSendResult  =  send( sktHandle, pData, iDataLen, MSG_NOSIGNAL );
#endif // TARGET_OS_WINDOWS
		if( 0 >= iSendResult )
		{
			// send error
			m_rcLastError = VxGetLastError();
			if( 0 == m_rcLastError )
			{
				m_rcLastError = iSendResult;
			}

			VxCloseSktNow(sktHandle);
			return m_rcLastError;
		}
	}

	return 0;
}


//============================================================================
uint16_t VxGetRmtPort( SOCKET skt )
{
	struct sockaddr from;
	socklen_t addrLen;

	addrLen = sizeof(from);
	memset(&from, 0, sizeof(from));
	if( 0 > getpeername( skt, (struct sockaddr *)&from, &addrLen) )
	{
		LogMsg( LOG_ERROR, "getpeername failed: %.100s", strerror(errno) );
		return 0;
	}

	if( AF_INET == from.sa_family )
	{
		return ((sockaddr_in *)(&from))->sin_port;
	}
	else if( AF_INET6 == from.sa_family )
	{
		return ((sockaddr_in6 *)(&from))->sin6_port;
	}

	return 0;
}



//============================================================================
//=== socket errors ===//
//============================================================================
//============================================================================
const char * VxDescribeSktError( int iErr )
{
    if( iErr == 0xFFFFFFFF )
    {
        return "Connection Dropped";
    }

	static char as8Buf[ 128 ];
#ifdef TARGET_OS_WINDOWS
	switch ( iErr )
	{
	case WSANOTINITIALISED:	return "A successful AfxSocketInit must occur before using this API.";
	case WSAEADDRINUSE:		return "The specified address is already in use.";
	case WSAEINPROGRESS:	return "A blocking Windows Sockets call is in progress.";
	case WSAEADDRNOTAVAIL:	return "The specified address is not available from the local machine.";
	case WSAEAFNOSUPPORT:	return "Addresses in the specified family cannot be used with this socket.";
		//	case WSAEDESTADDREQ:	return "A destination address is required.";
	case WSAEFAULT:			return "The nSockAddrLen argument is incorrect.";
	case WSAEINVAL:			return "The socket is not already bound to an address.";
	case WSAEMFILE:			return "No more file descriptors are available.";
	case WSAENETDOWN:		return "A socket operation encountered a dead network.";
	case WSAENETUNREACH:	return "The network cannot be reached from this host at this time.";
	case WSAENETRESET:		return "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.";
	case WSAECONNABORTED:	return "An established connection was aborted by the software in your host machine.";
	case WSAECONNRESET:		return "An existing connection was forcibly closed by the remote host.";
	case WSAENOBUFS:		return "An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
	case WSAEISCONN:		return "A connect request was made on an already connected socket.";
	case WSAENOTCONN:		return "A request to send or receive data was disallowed because the socket is not connected.";
	case WSAENOTSOCK:		return "The descriptor is not a socket.";
	case WSAEWOULDBLOCK:	return "The socket is marked as nonblocking and the connection cannot be completed immediately.";
	case WSAEOPNOTSUPP:		return "MSG_OOB was specified, but the socket is not of type SOCK_STREAM.";
	case WSAESHUTDOWN:		return "The socket has been shut down; it is not possible to call Receive on a socket after ShutDown has been invoked with nHow set to 0 or 2.";
	case WSAEMSGSIZE:		return "The datagram was too large to fit into the specified buffer and was truncated.";
	case WSAETOOMANYREFS:	return "Too many references to some kernel object.";
	case WSAETIMEDOUT:		return "Attempt to connect timed out without establishing a connection.";
	case WSAECONNREFUSED:	return "The attempt to connect was rejected.";
	case WSAELOOP:			return "Cannot translate name.";
	case WSAENAMETOOLONG:	return "Name component or name was too long.";
	case WSAEHOSTDOWN:		return "A socket operation failed because the destination host was down.";
	case WSAEHOSTUNREACH:	return "A socket operation was attempted to an unreachable host.";
	case WSAENOTEMPTY:		return "Cannot remove a directory that is not empty.";
	case WSAEPROCLIM:		return "A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.";
	case WSAEUSERS:			return "Ran out of quota.";
	case WSAEDQUOT:			return "Ran out of disk quota.";
	case WSAESTALE:			return "File handle reference is no longer available.";
	case WSAEREMOTE:		return "Item is not available locally.";
	case WSASYSNOTREADY:	return "WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.";
	case WSAVERNOTSUPPORTED:return "The Windows Sockets version requested is not supported.";
	case EIM_ALIVE_TIMEDOUT: return "Connection I Am Alive Request Timed Out";
	default:				return "Windows Unknown socket error";
	}
#else
	LogMsg( LOG_ERROR, "VxDescribeSktError Linux %d\n", iErr );
	switch ( iErr )
	{
	case 1: // EPERM
		return "Operation not permitted.";
	case 2: // ENOENT           2      
		return "No such file or directory.";
	case 3: // ESRCH            3      
		return "No such process.";
	case 4: // EINTR            4      
		return "Interrupted system call.";
	case 5: // EIO              5     
		return "I/O error.";
	case 6: // ENXIO            6      
		return "No such device or address.";
	case 7: // E2BIG            7      
		return "Arg list too long.";
	case 8: // ENOEXEC          8      
		return "Exec format error.";
	case 9: // EBADF            9      
		return "Bad file number.";
	case 10: // ECHILD          10     
		return "No child processes.";
	case 11: // EAGAIN          11     
		return "Try again.";
	case 12: // ENOMEM          12     
		return "Out of memory.";

	case 13: // SOCACCES
		return "Permission denied";
	case 14: // SOCFAULT
		return "Bad address";
	case 15: // ENOTBLK
		return "Block device required";
	case 16: // EBUSY
		return "Device is busy";
	case 17: // EEXIST
		return "File exists";
	case 18: // EXDEV
		return "Cross device link";
	case 19: // ENODEV
		return "No such device";
	case 20: // ENOTDIR
		return "Not a directory";
	case 21: // EISDIR
		return "Is a directory";
	case 22: // SOCINVAL
		return "Invalid argument";
	case 23: // SOCINVAL
		return "File Table overflow";
	case 32: // EPIPE
		return "Broken Pipe";
	case 35: // SOCWOULDBLOCK
		return "Operation would block";
	case 36: //SOCINPROGRESS
		return "Operation now in progress";
	case 37: // SOCALREADY
		return "Operation already in progress";
	case 38: // SOCNOTSOCK
		return "Socket operation on non-socket";
	case 39: // SOCDESTADDRREQ
		return "Destination address required";
	case 40: //SOCMSGSIZE
		return "Message too long";
	case 42: // SOCNOPROTOOPT
		return "Protocol not available";
	case 43: // SOCPROTONOSUPPORT
		return "Protocol not supported";
	case 44: // SOCSOCKTNOSUPPORT
		return "Socket type not supported";
	case 45: // SOCOPNOTSUP
		return "Operation not supported on socket";
	case 47: // SOCAFNOSUPPORT
		return "Address family not support by protocol family";
	case 48: // SOCADDRINUSE
		return "Address already in use";
	case 49: // SOCADDRNOTAVAIL
		return "Can't assign requested address";
		/*
		#define EDEADLK 35
		#define ENAMETOOLONG 36
		#define ENOLCK 37
		#define ENOSYS 38
		#define ENOTEMPTY 39
		#define ELOOP 40
		#define EWOULDBLOCK EAGAIN
		#define ENOMSG 42
		#define EIDRM 43
		#define ECHRNG 44
		#define EL2NSYNC 45
		#define EL3HLT 46
		#define EL3RST 47
		#define ELNRNG 48
		#define EUNATCH 49
		#define ENOCSI 50
		#define EL2HLT 51
		#define EBADE 52
		#define EBADR 53
		#define EXFULL 54
		#define ENOANO 55
		#define EBADRQC 56
		#define EBADSLT 57
		#define EDEADLOCK EDEADLK
		#define EBFONT 59
		#define ENOSTR 60
		#define ENODATA 61
		#define ETIME 62
		#define ENOSR 63
		#define ENONET 64
		#define ENOPKG 65
		#define EREMOTE 66
		#define ENOLINK 67
		#define EADV 68
		#define ESRMNT 69
		#define ECOMM 70
		#define EPROTO 71
		#define EMULTIHOP 72
		#define EDOTDOT 73
		#define EBADMSG 74
		#define EOVERFLOW 75
		#define ENOTUNIQ 76
		#define EBADFD 77
		#define EREMCHG 78
		#define ELIBACC 79
		#define ELIBBAD 80
		#define ELIBSCN 81
		#define ELIBMAX 82
		#define ELIBEXEC 83
		#define EILSEQ 84
		#define ERESTART 85
		#define ESTRPIPE 86
		#define EUSERS 87
		#define ENOTSOCK 88
		#define EDESTADDRREQ 89
		#define EMSGSIZE 90
		#define EPROTOTYPE 91
		#define ENOPROTOOPT 92
		#define EPROTONOSUPPORT 93
		*/

	case 55: // SOCNOBUFS
		return "No buffer space available";
	case 56: //"SOCISCONN"
		return "Socket is already connected";
	case 57: // SOCNOTCONN
		return "Socket is not connected";
	case 60: // SOCTIMEDOUT
		return "Connection timed out";
	case 61: // SOCCONNREFUSED
		return "Connection refused";
	case 88:
		return "Operation on non socket";
	case ESOCKTNOSUPPORT:
		return "ESOCKTNOSUPPORT";
	case EOPNOTSUPP:
		return "EOPNOTSUPP";
	case EPFNOSUPPORT:
		return "EPFNOSUPPORT";
	case EAFNOSUPPORT:
		return "EAFNOSUPPORT";
	case EADDRINUSE:
		return "EADDRINUSE";
	case EADDRNOTAVAIL:
		return "EADDRNOTAVAIL";
	case ENETDOWN:
		return "ENETDOWN";
	case ENETUNREACH:
		return "ENETUNREACH";
	case ECONNABORTED:
		return "ECONNABORTED";
	case ECONNRESET: // 104  
		return "ECONNRESET";
	case ENOBUFS: //  105  
		return "ENOBUFS";
	case EISCONN:
		return "EISCONN";
	case ENOTCONN:
		return "ENOTCONN";
	case ESHUTDOWN:
		return "ESHUTDOWN";
	case ETOOMANYREFS:
		return "ETOOMANYREFS";
	case ETIMEDOUT: //110
		return "ETIMEDOUT";
	case ECONNREFUSED: //111
		return "ECONNREFUSED";
	case EHOSTDOWN: //112
		return "EHOSTDOWN";
	case EHOSTUNREACH: //113
		return "EHOSTUNREACH";
	case EALREADY: //114
		return "EALREADY";
	case EINPROGRESS: //115
		return "EINPROGRESS";
	case ESTALE: //116
		return "ESTALE";
	case EUCLEAN: //117
		return "EUCLEAN";
	case ENOTNAM: //118
		return "ENOTNAM";
	case ENAVAIL: //119
		return "ENAVAIL";
	case EISNAM: //129
		return "EISNAM";
	case EREMOTEIO: //121
		return "EREMOTEIO";
	case EDQUOT: //122
		return "EDQUOT";

	case 252: //ESOCINACT
		return "Socket creation is disabled in the system";
	case 254: // ESYSTEMERROR
		return "Socket system error";
	case EIM_ALIVE_TIMEDOUT:
		return "Connection I Am Alive Request Timed Out";

	default:
		sprintf( as8Buf, "Unknown Linux Socket Error %d", iErr );
		return as8Buf;
	}
#endif // TARGET_OS_WINDOWS
}

GOTV_END_CDECLARES

