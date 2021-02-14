//============================================================================
// Copyright (C) 2013 Brett R. Jones
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

#include "InetAddress.h"
#include "VxSktUtil.h"

#include <CoreLib/VxDebug.h>
#include <PktLib/PktBlobEntry.h>

#ifdef TARGET_OS_WINDOWS
	#include "Winsock2.h"
	#include <Ws2tcpip.h>  
#else
    #include <netdb.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <net/if.h>
	#ifndef TARGET_OS_ANDROID
		#include <ifaddrs.h>
	#endif
#if defined(TARGET_OS_ANDROID)
    #include "ifaddrs-android-impl.h"
#endif
    #include <stdio.h>
    #include <sys/types.h>
    #include <ifaddrs.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <arpa/inet.h>
#endif

#include <string.h>
#include <memory.h>
#include <stdio.h>

#define IP4_PREFIX 0x0000ffff00000000ULL
#define INET6_MAX_STR_LEN 68
//============================================================================
// InetAddrIPv4
//============================================================================
InetAddrIPv4::InetAddrIPv4(const InetAddrIPv4& rhs)
: m_u32AddrIPv4(rhs.m_u32AddrIPv4)
{
}

//============================================================================
InetAddrIPv4::InetAddrIPv4( const char * pIpAddress )
: m_u32AddrIPv4(0)
{
	fromString( pIpAddress );
}

//============================================================================
InetAddrIPv4::InetAddrIPv4( uint32_t u32IpAddr )
{
	setIp( u32IpAddr );
}

//============================================================================
bool InetAddrIPv4::addToBlob( PktBlobEntry& blob )
{
    return blob.setValue( m_u32AddrIPv4 );
}

//============================================================================
bool InetAddrIPv4::extractFromBlob( PktBlobEntry& blob )
{
    return blob.getValue( m_u32AddrIPv4 );
}

//============================================================================
InetAddrIPv4& InetAddrIPv4::operator=( const InetAddrIPv4& inetAddr ) 
{
	if( inetAddr.isValid() )
	{
		m_u32AddrIPv4 = inetAddr.getIPv4AddressInNetOrder();
	}
	else
	{
		setToInvalid();
	}

	return *this;
}

//============================================================================
bool InetAddrIPv4::operator != ( const InetAddrIPv4& inetAddr )  const
{
	return ! (inetAddr.isIPv4() && ( m_u32AddrIPv4 == inetAddr.getIPv4AddressInNetOrder() ));
}

//============================================================================
bool InetAddrIPv4::operator == (const InetAddrIPv4& inetAddr)  const
{
	return inetAddr.isIPv4() && ( m_u32AddrIPv4 == inetAddr.getIPv4AddressInNetOrder() );
}

//============================================================================
InetAddrIPv4& InetAddrIPv4::operator=( const InetAddress& inetAddr ) 
{
	if( inetAddr.isIPv4() && inetAddr.isValid() )
	{
		setIp( inetAddr.getIPv4AddressInNetOrder() );
	}
	else
	{
		setToInvalid();
		LogMsg( LOG_ERROR, "ERROR InetAddrIPv4::operator=( const InetAddress& inetAddr ) is invalid\n" );
	}

	return *this;
}

//============================================================================
bool InetAddrIPv4::operator != ( const InetAddress& inetAddr )  const
{
	return ! (inetAddr.isIPv4() && ( this->m_u32AddrIPv4 == inetAddr.getIPv4AddressInNetOrder() ));
}

//============================================================================
bool InetAddrIPv4::operator == (const InetAddress& inetAddr)  const
{
	return (inetAddr.isIPv4() && ( this->m_u32AddrIPv4 == inetAddr.getIPv4AddressInNetOrder() ));
}

//============================================================================
bool InetAddrIPv4::isValid( void ) const
{
	return ( 0 != m_u32AddrIPv4 );
}

//============================================================================
void InetAddrIPv4::setToInvalid( void )
{
	m_u32AddrIPv4 = 0;
}

//============================================================================
bool InetAddrIPv4::isLoopBack() const
{
	return ( 0x7f000001 != m_u32AddrIPv4 );
}

//============================================================================
InetAddress InetAddrIPv4::toInetAddress( void )
{
	InetAddress inetAddr;
	inetAddr.setIp( this->getIPv4AddressInNetOrder(), false );
	return inetAddr;
}

//============================================================================
void InetAddrIPv4::fromString( const char * pIpAddress )
{
	if( ( NULL == pIpAddress ) || 
		( 0 == strlen( pIpAddress ) ) ||
		( 0 == strcmp( "0.0.0.0", pIpAddress ) ) ||
		( 0 == strcmp( "::", pIpAddress ) ) )
	{
		setToInvalid();
	}
	else if( isIPv4String( pIpAddress ) )
	{
		uint32_t u32Ip;
		VxIPv4_pton( pIpAddress, &u32Ip, false );
		setIp( u32Ip );
	}
	else
	{
		LogMsg( LOG_ERROR, "ERROR InetAddrIPv4::fromString %s is invalid\n", pIpAddress );
	}
}

//============================================================================
bool InetAddrIPv4::isIPv4String( const char * pIpAddress ) const
{
	if( pIpAddress
		&& (strlen(pIpAddress) < 16 )
		&& strchr(pIpAddress, '.')
		&& ( 0 == strchr(pIpAddress, ':')) )
	{
		return true;
	}

	return false;
}

//============================================================================
uint32_t InetAddrIPv4::getIPv4AddressInHostOrder( void ) const
{
	return ntohl( m_u32AddrIPv4 );
}

//============================================================================
uint32_t InetAddrIPv4::getIPv4AddressInNetOrder( void ) const
{
	return m_u32AddrIPv4;
}

//============================================================================
// note.. internally kept in network order instead of host order
void InetAddrIPv4::setIp( uint32_t u32IPv4Addr, bool bIsHostOrder )
{
	if( bIsHostOrder )
	{
		u32IPv4Addr = htonl( u32IPv4Addr );
	}

	if( u32IPv4Addr )
	{
		m_u32AddrIPv4 = u32IPv4Addr;
	}
	else
	{
		setToInvalid();
	}
}

//============================================================================
void InetAddrIPv4::setIp( const char * pIp )
{
	fromString( pIp );
}

//============================================================================
//! returns port in host order
uint16_t InetAddrIPv4::setIp( struct sockaddr_in& oIPv4Addr )
{
	setIp(*((uint32_t*)&oIPv4Addr.sin_addr), true );
	return ntohs( oIPv4Addr.sin_port );
}

//============================================================================
//! returns port in host order
uint16_t InetAddrIPv4::setIp( struct sockaddr& ipAddr )
{
	if( AF_INET == ipAddr.sa_family )
	{
		return setIp( *((sockaddr_in *)&ipAddr) );
	}
	else if( AF_INET6 == ipAddr.sa_family )
	{
		LogMsg( LOG_ERROR, "ERROR InetAddrIPv4::setIp tried to set IPv6 address\n" );
		setToInvalid();
		return 0;
	}
	else
	{
		LogMsg( LOG_ERROR, "InetAddress::setIp unknown family\n" );
		return 0;
	}
}

//============================================================================
//! returns port in host order
uint16_t InetAddrIPv4::setIp( struct sockaddr_storage& oAddr )
{
	switch( oAddr.ss_family ) 
	{
	case AF_INET:
		return setIp(*((struct sockaddr_in *)&oAddr));
		break;

	case AF_INET6:
		LogMsg( LOG_ERROR, "ERROR InetAddrIPv4::setIp tried to set IPv6 address\n" );
		setToInvalid();
		break;

	default:
		//vx_assert(false);
		break;
	};

	return 0;
}

//============================================================================
bool InetAddrIPv4::isLocalAddress( void ) const
{
	uint32_t hostOrderIpv4 = getIPv4AddressInHostOrder();
	if( ( 0x00000000 == ( hostOrderIpv4 & 0xff000000 ) ) || // 0.
		( 0x7f000000 == ( hostOrderIpv4 & 0xff000000 ) ) || // 127.
		( 0x0a000000 == ( hostOrderIpv4 & 0xff000000 ) ) || // 10.		
		( 0xc0a80000 == ( hostOrderIpv4 & 0xffff0000 ) ) || // 192.168.
		( 0xa9fe0000 == ( hostOrderIpv4 & 0xffff0000 ) ) || // 169.254.
		( 0xac100000 == ( hostOrderIpv4 & 0xfff00000 ) ) )  // 172.16?
	{
		return true;
	}

	return false;
}

//============================================================================
//! fill address with this ip address and the given port
int InetAddrIPv4::fillAddress( struct sockaddr_storage& oAddr, uint16_t u16Port )
{
	return fillAddress( *((struct sockaddr_in*)&oAddr), u16Port );
}

//============================================================================
//! fill address with this ip address and the given port.. returns struct len
int InetAddrIPv4::fillAddress( struct sockaddr_in& oIPv4Addr, uint16_t u16Port )
{
	// setup the address and port
	memset( &oIPv4Addr, 0, sizeof( sockaddr_in ) );

	oIPv4Addr.sin_family			= AF_INET;
	*((long*)&oIPv4Addr.sin_addr)	= getIPv4AddressInHostOrder();

	oIPv4Addr.sin_port				= htons( u16Port );
	return (int)sizeof( struct sockaddr_in);
}

//============================================================================
std::string InetAddrIPv4::toStdString( void )
{
	std::string retIpAddress;
	char as8Buf[ INET6_MAX_STR_LEN ];
	as8Buf[0] = 0; 

	if( isValid() )
	{
		uint32_t u32Ip = getIPv4AddressInNetOrder();
		VxIPv4_ntop( &u32Ip, as8Buf, sizeof( as8Buf ), false );
			//LogMsg( LOG_INFO, "InetAddress::toStdString %s uint32_t 0x%x host order false\n", as8Buf, u32Ip );
		retIpAddress = as8Buf;
	}
	else
	{
		retIpAddress = "0.0.0.0";
	}

	return retIpAddress;
}

//============================================================================
//! returns port in host order
uint16_t	InetAddrIPv4::getIpFromAddr(const struct sockaddr *sa, std::string& retStr)
{
	uint16_t u16Port = 0;
	char as8Addr[ INET6_MAX_STR_LEN ];
	switch(sa->sa_family)
	{
	case AF_INET:
		VxIPv4_ntop(&(((struct sockaddr_in *)sa)->sin_addr), as8Addr, sizeof(as8Addr), true );
		u16Port = ntohs( (((struct sockaddr_in *)sa)->sin_port) );
		break;

	case AF_INET6:
		VxIPv6_ntop( &(((struct sockaddr_in6 *)sa)->sin6_addr), as8Addr, sizeof(as8Addr) );
		u16Port = ntohs( (((struct sockaddr_in6 *)sa)->sin6_port) );
		break;

	default:
		strcpy(as8Addr, "Unknown AF");
		return 0;
	}
	retStr = as8Addr;
	return u16Port;
}

//============================================================================
bool InetAddrIPv4::isLittleEndian( void )
{
	return ( ntohl(4L) == 4L );
}

//============================================================================
uint32_t InetAddrIPv4::swap32Bit( uint32_t src )
{
	return htonl( src );
}

//============================================================================
// InetAddrIPv4AndPort
//============================================================================
InetAddrIPv4AndPort::InetAddrIPv4AndPort( const InetAddrIPv4AndPort& rhs )
    : InetAddrIPv4( rhs )
    , m_u16Port( rhs.m_u16Port )
{
}

//============================================================================
bool InetAddrIPv4AndPort::addToBlob( PktBlobEntry& blob )
{
    bool result = InetAddrIPv4::addToBlob( blob );
    result &= blob.setValue( m_u16Port );
    return result;
}

//============================================================================
bool InetAddrIPv4AndPort::extractFromBlob( PktBlobEntry& blob )
{
    bool result = InetAddrIPv4::extractFromBlob( blob );
    result &= blob.getValue( m_u16Port );
    return result;
}

//============================================================================
InetAddrIPv4AndPort& InetAddrIPv4AndPort::operator=(const InetAddrIPv4& inetAddr) 
{
	if( this != &inetAddr )
	{
		m_u32AddrIPv4	= inetAddr.getIPv4AddressInNetOrder();
	}

	return *this;
}

//============================================================================
//! equal operator
InetAddrIPv4AndPort& InetAddrIPv4AndPort::operator=(const InetAddrIPv4AndPort& inetAddr) 
{
	if( this != &inetAddr )
	{
		m_u32AddrIPv4	= inetAddr.getIPv4AddressInNetOrder();
		m_u16Port		= inetAddr.getPort();
	}

	return *this;
}

//============================================================================
InetAddrIPv4AndPort& InetAddrIPv4AndPort::operator=(const InetAddress& inetAddr) 
{
	m_u32AddrIPv4	= inetAddr.getIPv4AddressInNetOrder();
	return *this;
}

//============================================================================
InetAddrIPv4AndPort& InetAddrIPv4AndPort::operator=(const InetAddrAndPort& inetAddr) 
{
	m_u16Port		= inetAddr.getPort();
	m_u32AddrIPv4	= inetAddr.getIPv4AddressInNetOrder();
	return *this;
}

//============================================================================
void InetAddrIPv4AndPort::setIpAndPort( struct sockaddr_storage& oAddr )
{
	m_u16Port = setIp( oAddr );
}

//============================================================================
void InetAddrIPv4AndPort::setIpAndPort( struct sockaddr& oAddr )
{
	m_u16Port = setIp( oAddr );
}

//============================================================================
//============================================================================
//============================================================================
//============================================================================
// InetAddress

//============================================================================
InetAddress::InetAddress( const char * pIpAddress )
: m_u64AddrHi(0)
, m_u64AddrLo(0)
{
	fromString( pIpAddress );
}

//============================================================================
InetAddress::InetAddress( uint32_t u32IpAddr )
{
	setIp( u32IpAddr );
}

//============================================================================
InetAddress::InetAddress( const InetAddress& rhs )
    : m_u64AddrHi( rhs.m_u64AddrHi )
    , m_u64AddrLo( rhs.m_u64AddrLo )
{
}

//============================================================================
bool InetAddress::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_u64AddrHi );
    result &= blob.setValue( m_u64AddrLo );
    return result;
}

//============================================================================
bool InetAddress::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_u64AddrHi );
    result &= blob.getValue( m_u64AddrLo );
    return result;
}

//============================================================================
InetAddress InetAddress::getDefaultIp( void )
{
	 std::vector<InetAddress> retAddresses;
	 getAllAddresses( retAddresses );
	 if( retAddresses.size() )
	 {
		 return retAddresses[0];
	 }
	 return InetAddress();
}

//============================================================================
int InetAddress::getAllAddresses( std::vector<InetAddress>& retAddresses )
{
#if defined(TARGET_OS_WINDOWS) || defined(TARGET_OS_ANDROID)
	// for unknown reasons this code that works on windows only return loopback in android
	// NO known fix and the linux version not viable because of missing ifaddr.h
	// TODO.. if android fetch addresses from JAVA
	char as8HostName[ 1025 ];
	//first get host name
	if( gethostname( as8HostName, sizeof( as8HostName ) ) )
	{
        LogMsg( LOG_ERROR, "getAllAddresses: Unable to get host name\n" );
		#ifdef TARGET_OS_WINDOWS
			return WSAGetLastError();
		#else
			return VxGetLastError();
		#endif // TARGET_OS_WINDOWS
	}

	struct addrinfo Hints;
	struct addrinfo * AI;
	struct addrinfo * AddrInfo;

	memset(&Hints, 0, sizeof(Hints));

    Hints.ai_family   = PF_UNSPEC;
    Hints.ai_socktype = SOCK_STREAM;
    Hints.ai_protocol = IPPROTO_TCP;
    //Hints.ai_flags    = AI_PASSIVE;
	Hints.ai_flags = AI_ADDRCONFIG;

	char as8Buf[16];
	sprintf( as8Buf, "%d", 65000 );

	int RetVal = getaddrinfo(as8HostName, "echo", &Hints, &AddrInfo);
	if (RetVal != 0)
	{
		//char * pErr = gai_strerror(RetVal);
		//printf("getaddrinfo() failed with error %d: %s\n", RetVal, pErr );
        LogMsg( LOG_ERROR, "InetAddress::getAllAddresses getaddrinfo error %d\n", RetVal );
		return RetVal;
	}

	for(  AI = AddrInfo; AI != NULL; AI = AI->ai_next )
	{
		if ((AI->ai_family != PF_INET) && (AI->ai_family != PF_INET6))
		{
			continue;
		}
		// Open a socket with the correct address family for this address.
		SOCKET oSkt = socket(AI->ai_family, AI->ai_socktype, AI->ai_protocol);

		if( oSkt == INVALID_SOCKET )
		{
			continue;
		}

        struct sockaddr_storage * poSktAddr = (struct sockaddr_storage *)AI->ai_addr;
        InetAddress oTestAddr;
        VxSetSktAddressPort(poSktAddr, 0);
        oTestAddr.setIp( *poSktAddr );
        std::string strTestIpAddress = oTestAddr.toStdString();        

        if( ( false == oTestAddr.isValid() ) ||
            ( oTestAddr.isLoopBack() ) )
        {
            if( ! oTestAddr.isLoopBack() )
            {
                LogMsg( LOG_INFO, "InetAddress::getAllAddresses: invalid addr %s\n", strTestIpAddress.c_str() );
            }

            VxCloseSkt( oSkt );
            continue;
        }

		//LogMsg( LOG_INFO, "binding skt %d\n", oSkt );
		if( false == VxBindSkt( oSkt, poSktAddr ) )
        {
            LogMsg( LOG_INFO, "InetAddress::getAllAddresses: could not bind addr %s\n", strTestIpAddress.c_str() );
            continue;
        }

        if( oTestAddr.isValid() &&
            ( false == oTestAddr.isLoopBack() ) )
        {
            retAddresses.push_back(oTestAddr);
        }

//#define TEST_CONNECTION
#ifdef TEST_CONNECTION
        if( false == VxTestConnection( oSkt, oTestAddr ) )
        {
            LogMsg( LOG_INFO, "Connection using local address %s OK\n", strTestIpAddress.c_str() );
        }
        else
        {
            LogMsg( LOG_INFO, "Connection using local address %s FAIL\n", strTestIpAddress.c_str() );
        }
#else
		//LogMsg( LOG_INFO, "closing skt %d\n", oSkt );
        VxCloseSkt( oSkt );
#endif // TEST_CONNECTION
	}

	//LogMsg( LOG_INFO, "freeing addr info\n" );
	freeaddrinfo( AddrInfo ); // free the linked list
#else
    struct ifaddrs *myaddrs, *ifa;
    struct sockaddr_storage * poSktAddr = NULL;
    char buf[64];

    if(getifaddrs(&myaddrs) != 0)
    {
        perror("InetAddress::getAllAddresses: getifaddrs");
        LogMsg( LOG_ERROR, "InetAddress::getAllAddresses: getifaddrs FAIL\n" );
    }

    for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;
        if (!(ifa->ifa_flags & IFF_UP))
            continue;

        switch (ifa->ifa_addr->sa_family)
        {
            case AF_INET:
            case AF_INET6:
            {
                poSktAddr = (struct sockaddr_storage *)ifa->ifa_addr;
                break;
            }

            default:
                continue;
        }

        if (!inet_ntop(ifa->ifa_addr->sa_family, poSktAddr, buf, sizeof(buf)))
        {
           LogMsg( LOG_ERROR, "InetAddress::getAllAddresses: %s: inet_ntop failed!\n", ifa->ifa_name);
        }
        else
        {
            // Open a socket with the correct address family for this address.
            SOCKET oSkt = socket(ifa->ifa_addr->sa_family,
                                 SOCK_STREAM,
                                 0 );

            if( oSkt == INVALID_SOCKET )
            {
                continue;
            }

            //LogMsg( LOG_INFO, "InetAddress::getAllAddresses: %s: %s\n", ifa->ifa_name, buf);
            InetAddress oTestAddr;
            VxSetSktAddressPort(poSktAddr, 0);
            oTestAddr.setIp( *poSktAddr );
            std::string strTestIpAddress = oTestAddr.toStdString();

            if( ( false == oTestAddr.isValid() ) ||
                ( oTestAddr.isLoopBack() ) )
            {
                //if( oTestAddr.isLoopBack() )
                //{
                //    LogMsg( LOG_INFO, "InetAddress::getAllAddresses: loopback addr %s\n", strTestIpAddress.c_str() );
                //}
                //else
                //{
                //    LogMsg( LOG_INFO, "InetAddress::getAllAddresses: invalid addr %s\n", strTestIpAddress.c_str() );
                //}
                VxCloseSkt( oSkt );
                continue;
            }

            if( false == VxBindSkt( oSkt, poSktAddr ) )
            {
                LogMsg( LOG_INFO, "InetAddress::getAllAddresses: could not bind addr %s\n", strTestIpAddress.c_str() );
                VxCloseSkt( oSkt );
                continue;
            }

            if( oTestAddr.isValid() &&
                ( false == oTestAddr.isLoopBack() ) )
            {
                retAddresses.push_back(oTestAddr);
            }

    //#define TEST_CONNECTION
    #ifdef TEST_CONNECTION
            if( false == VxTestConnection( oSkt, oTestAddr ) )
            {
                LogMsg( LOG_INFO, "Connection using local address %s OK\n", strTestIpAddress.c_str() );
            }
            else
            {
                LogMsg( LOG_INFO, "Connection using local address %s FAIL\n", strTestIpAddress.c_str() );
            }
    #else
            VxCloseSkt( oSkt );
    #endif // TEST_CONNECTION
        }
    }

    freeifaddrs(myaddrs);
#endif // TARGET_OS_WINDOWS

#if defined(TARGET_OS_LINUX)
    if( retAddresses.empty() )
    {
        struct ifaddrs * ifAddrStruct=NULL;
        struct ifaddrs * ifa=NULL;
        void * tmpAddrPtr=NULL;

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (!ifa->ifa_addr)
            {
                continue;
            }

            if (ifa->ifa_addr->sa_family == AF_INET)
            { // check it is IP4
                // is a valid IP4 Address
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                //LogMsg( LOG_INFO, "%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                InetAddress oTestAddr;
                oTestAddr.setIp( *addressBuffer );
                if( oTestAddr.isValid() && ( false == oTestAddr.isLoopBack() ) )
                {
                    retAddresses.push_back(oTestAddr);
                }
            }
            else if (ifa->ifa_addr->sa_family == AF_INET6)
            { // check it is IP6
                // is a valid IP6 Address
                tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                //LogMsg( LOG_INFO, "%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                InetAddress oTestAddr;
                oTestAddr.setIp( *addressBuffer );
                if( oTestAddr.isValid() && ( false == oTestAddr.isLoopBack() ) )
                {
                    retAddresses.push_back(oTestAddr);
                }
            }
        }

        if (ifAddrStruct != nullptr)
        {
            freeifaddrs(ifAddrStruct);
        }
    }
#endif // defined(TARGET_OS_LINUX)
#if defined(TARGET_OS_ANDROID)
    if( retAddresses.empty() )
    {
        struct ifaddrs_android * ifAddrStruct=NULL;
        struct ifaddrs_android * ifa=NULL;
        void * tmpAddrPtr=NULL;

        getifaddrs_android(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (!ifa->ifa_addr)
            {
                continue;
            }

            if (ifa->ifa_addr->sa_family == AF_INET)
            { // check it is IP4
                // is a valid IP4 Address
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                //LogMsg( LOG_INFO, "%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                InetAddress oTestAddr;
                oTestAddr.setIp( (const char *)(&addressBuffer[0]) );
                if( oTestAddr.isValid() && ( false == oTestAddr.isLoopBack() ) )
                {
                    retAddresses.push_back(oTestAddr);
                }
            }
            else if (ifa->ifa_addr->sa_family == AF_INET6)
            { // check it is IP6
                // is a valid IP6 Address
                tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                //LogMsg( LOG_INFO, "%s IP Address %s\n", ifa->ifa_name, addressBuffer);
                InetAddress oTestAddr;
                oTestAddr.setIp( (const char *)(&addressBuffer[0]) );
                if( oTestAddr.isValid() && ( false == oTestAddr.isLoopBack() ) )
                {
                    retAddresses.push_back(oTestAddr);
                }
            }
        }

        if (ifAddrStruct != nullptr)
        {
            freeifaddrs_android(ifAddrStruct);
        }
    }

#endif // defined(TARGET_OS_ANDROID)

	return 0;
}

//============================================================================
void InetAddress::dumpAddresses( std::vector<InetAddress>& addressList )
{
    LogMsg( LOG_INFO, "InetAddress::dumpAddresses count %d", addressList.size() );
    static int addrIdx = 0;
    for( InetAddress& addr : addressList )
    {
        addrIdx++;
        LogMsg( LOG_INFO, "Addr %d - %s isIPv4 %d isLoopback %d isLocal %d", addrIdx, addr.toStdString().c_str(), addr.isIPv4(), addr.isLoopBack(), addr.isLocalAddress() );
    }
}

//============================================================================
bool InetAddress::isLoopBack() const
{
	bool isLoopBack = true;
	if( isIPv4() )
	{
		if( 0x7f000001 != getIPv4AddressInNetOrder() )
		{
			isLoopBack = false;
		}
	}
	else
	{
		uint8_t * pu8Bytes = (uint8_t * )this;
		for(int i = 0; i < 16; i++ )
		{
			if( pu8Bytes[i] && (i != 15) )
			{
				isLoopBack = false;
				break;
			}
			else if(  (1 == pu8Bytes[i]) && (i == 15) )
			{
				isLoopBack = true;
				break;
			}
		}
	}
	return isLoopBack;
}

//============================================================================
bool InetAddress::isLocalAddress( bool forLocalListening ) const
{
	if( false == isValid() )
	{
		LogMsg( LOG_ERROR, "InetAddrIPv4::isLocalAddress invalid address " );
		return false;
	}

	if( isIPv6() ) 
	{
		uint32_t * netOrder = (uint32_t *)this;

		uint32_t ip0 = ntohl(netOrder[0]);
		uint32_t ip1 = ntohl(netOrder[1]);
		uint32_t ip2 = ntohl(netOrder[2]);
		uint32_t ip3 = ntohl(netOrder[3]);

		if( forLocalListening && !ip0 && !ip1 && !ip2 && !ip3 ) // :: *
		{
			return false;
		}

		if( !ip0 && !ip1 && !ip2 &&
			( 0x00000000 == (ip3 & 0xfffffffe) ) )  // ::/127
		{
			return true;
		}

		if( ( 0xfe800000 == ( ip0 & 0xffc00000 ) ) || // fe80  RFC4291
			( 0xfc000000 == ( ip0 & 0xfe000000 ) ) || // fc00  RFC4193
			( 0xfec00000 == ( ip0 & 0xffc00000 ) ) )   // fec0  RFC3879
		{
			return true;
		}

		return false;
	} 
	else 
	{
		uint32_t hostOrderIpv4 = getIPv4AddressInHostOrder();
		if( ( 0x00000000 == ( hostOrderIpv4 & 0xff000000 ) ) || // 0.
			( 0x7f000000 == ( hostOrderIpv4 & 0xff000000 ) ) || // 127.
			( 0x0a000000 == ( hostOrderIpv4 & 0xff000000 ) ) || // 10.		
			( 0xc0a80000 == ( hostOrderIpv4 & 0xffff0000 ) ) || // 192.168.
			( 0xa9fe0000 == ( hostOrderIpv4 & 0xffff0000 ) ) || // 169.254.
			( 0xac100000 == ( hostOrderIpv4 & 0xfff00000 ) ) )  // 172.16?
		{
			return true;
		}

		return false;
	}
}

//============================================================================
InetAddress& InetAddress::operator=(const InetAddress& oAddr) 
{
	if( this != &oAddr )
	{
		m_u64AddrHi = oAddr.m_u64AddrHi;
		m_u64AddrLo = oAddr.m_u64AddrLo;
	}

	return *this;
}

//============================================================================
bool InetAddress::operator == (const InetAddress& oAddr)  const
{
	return (m_u64AddrHi == oAddr.m_u64AddrHi) && (m_u64AddrLo == oAddr.m_u64AddrLo);
}

//============================================================================
bool InetAddress::operator != (const InetAddress& oAddr)  const
{
	return (m_u64AddrHi != oAddr.m_u64AddrHi) || (m_u64AddrLo != oAddr.m_u64AddrLo);
}


//============================================================================
InetAddress& InetAddress::operator=(const InetAddrIPv4& inetAddr) 
{
	setIp( inetAddr.getIPv4AddressInNetOrder() );
	return *this;
}

//============================================================================
bool InetAddress::operator == (const InetAddrIPv4& inetAddr)  const
{
	return ( getIPv4AddressInNetOrder() == inetAddr.getIPv4AddressInHostOrder() );
}

//============================================================================
bool InetAddress::operator != (const InetAddrIPv4& inetAddr)  const
{
	return ( getIPv4AddressInNetOrder() != inetAddr.getIPv4AddressInHostOrder() );
}

//============================================================================
bool InetAddress::isValid( void ) const
{
	return ( 0 != m_u64AddrLo );
}

//============================================================================
void InetAddress::setToInvalid( void )
{
	m_u64AddrHi = 0;
	m_u64AddrLo = 0;
}

//============================================================================
void InetAddress::fromString( const char * pIpAddress )
{
	if( ( NULL == pIpAddress ) || 
		( 0 == strlen( pIpAddress ) ) ||
		( 0 == strcmp( "0.0.0.0", pIpAddress ) ) ||
		( 0 == strcmp( "::", pIpAddress ) ) )
	{
		setToInvalid();
	}
	else if( isIPv4String( pIpAddress ) )
	{
		uint32_t u32Ip;
		VxIPv4_pton( pIpAddress, &u32Ip, false );
		setIp( u32Ip );
	}
	else
	{
        VxIPv6_pton( pIpAddress, this );
	}
}

//============================================================================
bool InetAddress::isIPv4String( const char * pIpAddress ) const
{
	if( pIpAddress
		 && (strlen(pIpAddress) < 16 )
		 && strchr(pIpAddress, '.')
		 && ( 0 == strchr(pIpAddress, ':')) )
	{
		return true;
	}

	return false;
}

//============================================================================
bool InetAddress::isIPv4( void ) const
{
	if( (false == isValid() ) ||
		(0xffffffffffffffffULL == m_u64AddrHi)) 
	{
		return true;
	}
	return false;
}

//============================================================================
bool InetAddress::isIPv6( void ) const
{
	return isIPv4() ? false : true;
}

//============================================================================
bool InetAddress::isIPv6GlobalAddress( void ) const
{
	// NOTE: bad assumption. better to check for locals instead.
	if( isIPv6() && 
		( false == isLocalAddress() ) )
	{
		return true;
	}

	return false;
}

//============================================================================
uint32_t InetAddress::getIPv4AddressInHostOrder( void ) const
{
	return ntohl(*((uint32_t*)&m_u64AddrLo));
}

//============================================================================
uint32_t InetAddress::getIPv4AddressInNetOrder( void ) const
{
	return *((uint32_t*)&m_u64AddrLo);
}

//============================================================================
// note.. internally kept in network order instead of host order
void InetAddress::setIp( uint32_t u32IPv4Addr, bool bIsHostOrder )
{
    if( bIsHostOrder )
	{
		u32IPv4Addr = htonl( u32IPv4Addr );
	}

	if( u32IPv4Addr )
	{
		m_u64AddrLo = 0;
		*((uint32_t*)&m_u64AddrLo) = u32IPv4Addr;
		m_u64AddrHi = 0xffffffffffffffffULL;
	}
	else
	{
		setToInvalid();
	}
}

//============================================================================
void InetAddress::setIp( const char * pIp )
{
	fromString( pIp );
}

//============================================================================
//! returns port in host order
uint16_t InetAddress::setIp( struct sockaddr_in& oIPv4Addr )
{
    setIp(*((uint32_t*)&oIPv4Addr.sin_addr), true );
	return ntohs( oIPv4Addr.sin_port );
}

//============================================================================
//! returns port in host order
uint16_t InetAddress::setIp( struct sockaddr_in6& oIPv6Addr )
{
	memcpy(this, &oIPv6Addr.sin6_addr, 16);
	return ntohs( oIPv6Addr.sin6_port );
}

//============================================================================
//! returns port in host order
uint16_t InetAddress::setIp( struct sockaddr& ipAddr )
{
	if( AF_INET == ipAddr.sa_family )
	{
		return setIp( *((sockaddr_in *)&ipAddr) );
	}
	else if( AF_INET6 == ipAddr.sa_family )
	{
		return setIp( *((sockaddr_in6 *)&ipAddr) );
	}
	else
	{
		LogMsg( LOG_ERROR, "InetAddress::setIp unknown family\n" );
		return 0;
	}
}

//============================================================================
//! returns port in host order
uint16_t InetAddress::setIp( struct sockaddr_storage& oAddr )
{
	switch( oAddr.ss_family ) 
	{
	case AF_INET:
		return setIp(*((struct sockaddr_in *)&oAddr));
		break;

	case AF_INET6:
		return setIp(*((struct sockaddr_in6 *)&oAddr));
		break;

	default:
		//vx_assert(false);
		return 0;
	}
}

//============================================================================
//! fill address with this ip address and the given port
int InetAddress::fillAddress( struct sockaddr_storage& oAddr, uint16_t u16Port )
{
    memset( &oAddr, 0, sizeof( struct sockaddr_storage ) );
	if( isIPv4() )
	{
        return fillAddress( *((struct sockaddr_in*)&oAddr), u16Port );
	}
	else
	{
        return fillAddress( *((struct sockaddr_in6*)&oAddr), u16Port );
	}
}

//============================================================================
//! fill address with this ip address and the given port.. returns struct len
int InetAddress::fillAddress( struct sockaddr_in& oIPv4Addr, uint16_t u16Port )
{
	// setup the address and port
	memset( &oIPv4Addr, 0, sizeof( sockaddr_in ) );

	oIPv4Addr.sin_family			= AF_INET;
	*((long*)&oIPv4Addr.sin_addr)	= getIPv4AddressInHostOrder();

	oIPv4Addr.sin_port				= htons( u16Port );
	return (int)sizeof( struct sockaddr_in);
}

//============================================================================
//! fill address with this ip address and the given port.. returns struct len
int InetAddress::fillAddress( struct sockaddr_in6& oIPv6Addr, uint16_t u16Port )
{
	// setup the address and port
	memset( &oIPv6Addr, 0, sizeof( sockaddr_in6 ) );

	oIPv6Addr.sin6_family			= AF_INET6;

	memcpy( &oIPv6Addr.sin6_addr, this, 16 );

	oIPv6Addr.sin6_port	= htons( u16Port );
	return (int)sizeof( struct sockaddr_in6);
}


//============================================================================
std::string InetAddress::toStdString( void )
{
	std::string retIpAddress;
	char as8Buf[ INET6_MAX_STR_LEN ];
	as8Buf[0] = 0; 

	if( isValid() )
	{
		if( isIPv4() )
		{
			uint32_t u32Ip = getIPv4AddressInNetOrder();
            VxIPv4_ntop( &u32Ip, as8Buf, sizeof( as8Buf ), false );
			//LogMsg( LOG_INFO, "InetAddress::toStdString %s uint32_t 0x%x host order false\n", as8Buf, u32Ip );
		}
		else
		{
			VxIPv6_ntop( this, as8Buf, sizeof( as8Buf ) );
		}
		retIpAddress = as8Buf;
	}
	else
	{
		retIpAddress = "0.0.0.0";
	}

	return retIpAddress;
}

//============================================================================
//! returns port in host order
uint16_t	InetAddress::getIpFromAddr(const struct sockaddr *sa, std::string& retStr)
{
	uint16_t u16Port = 0;
	char as8Addr[ INET6_MAX_STR_LEN ];
    switch(sa->sa_family)
    {
		case AF_INET:
			VxIPv4_ntop(&(((struct sockaddr_in *)sa)->sin_addr), as8Addr, sizeof(as8Addr), true );
			u16Port = ntohs( (((struct sockaddr_in *)sa)->sin_port) );
			break;

		case AF_INET6:
			VxIPv6_ntop( &(((struct sockaddr_in6 *)sa)->sin6_addr), as8Addr, sizeof(as8Addr));
			u16Port = ntohs( (((struct sockaddr_in6 *)sa)->sin6_port) );
			break;

		default:
			strcpy(as8Addr, "Unknown AF");
            return 0;
	}
    retStr = as8Addr;
	return u16Port;
}

//============================================================================
bool InetAddress::isLittleEndian( void )
{
	return ( 4L == ntohl(4L));
}

//============================================================================
uint64_t InetAddress::swap64Bit( uint64_t src )
{
	uint64_t dest;
	uint16_t * pu8Src = (uint16_t *)&src;
	uint16_t * pu8Dest = (uint16_t *)&dest;
	pu8Dest[1] = htons(pu8Src[0]);
	pu8Dest[0] = htons(pu8Src[1]);
	pu8Dest[3] = htons(pu8Src[2]);
	pu8Dest[2] = htons(pu8Src[3]);
	return dest;
}

//============================================================================
void InetAddress::litteEndianToNetIPv6( uint16_t * src, uint16_t * dest )
{
	if( isLittleEndian() )
	{
		uint64_t u64Hi = *((uint64_t *)src);
		uint64_t u64Lo = *((uint64_t *)(&src[4]));
		*((uint64_t *)dest) = swap64Bit(u64Hi);
		*((uint64_t *)(&dest[4])) = swap64Bit(u64Lo);
	}
	else
	{
		uint32_t u32Hi1 = *((uint32_t *)src);
		uint32_t u32Hi2 = *((uint32_t *)(&src[2]));
		uint32_t u32Lo1 = *((uint32_t *)(&src[4]));
		uint32_t u32Lo2 = *((uint32_t *)(&src[6]));
		*((uint32_t *)dest) = htonl(u32Hi2);
		*((uint32_t *)(&dest[2])) = htonl(u32Hi1);
		*((uint32_t *)(&dest[4])) = htonl(u32Lo2);
		*((uint32_t *)(&dest[6])) = htonl(u32Lo1);
	}
}

//============================================================================
// InetAddrAndPort
//============================================================================

//============================================================================
InetAddrAndPort::InetAddrAndPort( const char * ipAddr )
: InetAddress( ipAddr )
, m_u16Port(0)
{
}

//============================================================================
InetAddrAndPort::InetAddrAndPort( const InetAddrAndPort& rhs )
    : InetAddress( rhs )
    , m_u16Port( rhs.m_u16Port )
{
}

//============================================================================
bool InetAddrAndPort::addToBlob( PktBlobEntry& blob )
{
    bool result = InetAddress::addToBlob( blob );
    result &= blob.setValue( m_u16Port );
    return result;
}

//============================================================================
bool InetAddrAndPort::extractFromBlob( PktBlobEntry& blob )
{
    bool result = InetAddress::extractFromBlob( blob );
    result &= blob.getValue( m_u16Port );
    return result;
}

//============================================================================
InetAddrAndPort::InetAddrAndPort( const char * ipAddr, uint16_t port )
: InetAddress( ipAddr )
, m_u16Port( port )
{
}

//============================================================================
InetAddrAndPort& InetAddrAndPort::operator=(const InetAddress& oAddr) 
{
	if( this != &oAddr )
	{
		m_u64AddrHi = oAddr.m_u64AddrHi;
		m_u64AddrLo = oAddr.m_u64AddrLo;
	}

	return *this;
}

//============================================================================
InetAddrAndPort& InetAddrAndPort::operator=(const InetAddrAndPort& oAddr) 
{
	if( this != &oAddr )
	{
		m_u64AddrHi = oAddr.m_u64AddrHi;
		m_u64AddrLo = oAddr.m_u64AddrLo;
		m_u16Port = oAddr.m_u16Port;
	}

	return *this;
}

//============================================================================
InetAddrAndPort& InetAddrAndPort::operator=(const InetAddrIPv4& inetAddr) 
{
	setIp( inetAddr.getIPv4AddressInHostOrder() );
	return *this;
}

//============================================================================
InetAddrAndPort& InetAddrAndPort::operator=(const InetAddrIPv4AndPort& inetAddr) 
{
	setIp( inetAddr.getIPv4AddressInHostOrder() );
	m_u16Port = inetAddr.getPort();
	return *this;
}

//============================================================================
void InetAddrAndPort::setIpAndPort( struct sockaddr_storage& inetAddr )
{
	m_u16Port = setIp( inetAddr );
}

//============================================================================
void InetAddrAndPort::setIpAndPort( struct sockaddr& oAddr )
{
	m_u16Port = setIp( oAddr );
}

//============================================================================
void InetAddrAndPort::setIpAndPort( const char * ipAddr, uint16_t port )
{
    setIp( ipAddr );
    m_u16Port = port;
}
