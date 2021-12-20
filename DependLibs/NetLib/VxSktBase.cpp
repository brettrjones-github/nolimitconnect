//============================================================================
// Copyright (C) 2008 Brett R. Jones 
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

#include "VxSktBase.h"
#include "VxSktBaseMgr.h"
#include "VxResolveHost.h"
#include "VxSktUtil.h"
#include "InetAddress.h"

#include <PktLib/VxPktHdr.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxTimeUtil.h>

#include <time.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef TARGET_OS_WINDOWS
	#include "Mswsock.h"
#else
	#include <sys/ioctl.h>
#endif // TARGET_OS_WINDOWS

namespace
{
	const int					SKT_RX_RETRY_SLEEP_TIME_MS		= 400;
	const int					IM_ALIVE_TIMEOUT_MS			    = 180000;
}

//============================================================================
static void * VxSktBaseReceiveVxThreadFunc( void * pvContext );
int VxSktBase::m_TotalCreatedSktCnt{ 0 };
int VxSktBase::m_CurrentSktCnt{ 0 };
std::string VxSktBase::m_SktDirConnect{ "->" };
std::string VxSktBase::m_SktDirAccept{ "<-" };
std::string VxSktBase::m_SktDirUdp{  "<->" };
std::string VxSktBase::m_SktDirBroadcast{ "->>" };
std::string VxSktBase::m_SktDirLoopback{ "<==>" };
std::string VxSktBase::m_SktDirUnknown{ "<?\?>" }; // use \? instead of just ? to avoid warning: trigraph ignored
VxGUID VxSktBase::m_LoopbackConnectId{ 8740338989118525749, 11880411481645876119 };	// !794BEC0096E81135A4DFB34C24F98397!

//============================================================================
VxSktBase::VxSktBase()
: VxSktBuf()
, VxSktThrottle()	
, m_iSktId(0)
, m_LclIp()
, m_RmtIp()
, m_LastImAliveTimeGmtMs( GetGmtTimeMs() )
, m_SktRxThread()
, m_SktTxThread()
, m_SktTxSemaphore()
, m_SktMutex()
, m_CryptoMutex()

, m_RxKey()					// encryption key for receive
, m_RxCrypto()				// encryption object for receive
, m_TxKey()					// encryption key for transmit
, m_TxCrypto()				// encryption object for transmit
//, m_u8TxSeqNum;			// sequence number used to twart replay attacks ( do not set )
, m_RelayEventSemaphore()
{
	m_TotalCreatedSktCnt++;
	m_iSktId = m_TotalCreatedSktCnt;
	m_CurrentSktCnt++;
    m_ConnectionId.generateNewVxGUID( m_ConnectionId );
	m_u8TxSeqNum = (uint8_t)rand();
	m_LclIp.setToInvalid();
	m_RmtIp.setToInvalid();
#ifdef DEBUG_SKTS
	LogMsg( LOG_SKT,  "skt %d created\n", m_iSktId );
#endif // DEBUG_SKTS
}

//============================================================================
VxSktBase::~VxSktBase()
{
	m_bIsConnected = false;
	m_bClosingFromDestructor = true;
	m_SktRxThread.abortThreadRun( true );
	m_SktTxThread.abortThreadRun( true );
	closeSkt( eSktCloseSktDestroy, true );
	m_SktTxSemaphore.signal();
	m_SktRxThread.killThread();
	m_SktTxThread.killThread();
#ifdef DEBUG_SKTS
	LogMsg( LOG_SKT,  "VxSktBase::~VxSktBase skt %d 0x%x destroyed\n", m_iSktId, this );
#endif // DEBUG_SKTS
	m_CurrentSktCnt--;
}

//============================================================================
bool VxSktBase::checkForImAliveTimeout( bool calledFromSktThread )
{
	bool timedOut = false;
	if( ( INVALID_SOCKET != m_Socket )
		&& ( ( eSktTypeTcpConnect == m_eSktType ) || ( eSktTypeTcpAccept == m_eSktType ) )
		&& !m_bIsWebSkt
		&& !m_bIsPluginSpecificSkt
		&& ( IM_ALIVE_TIMEOUT_MS < ( GetGmtTimeMs() - getLastImAliveTimeMs() ) ) )
	{
		timedOut = true;
		m_bClosingFromRxThread = calledFromSktThread;
		LogMsg( LOG_INFO, "VxSktBase::checkForImAliveTimeout skt %d %s\n", m_iSktId, m_strRmtIp.c_str() );
		closeSkt( eSktCloseImAliveTimeout );
	}

	return timedOut;
}

//============================================================================
bool VxSktBase::toSocketAddrInfo(	int sockType, 
									const char *addr, 
									int port, 
									struct addrinfo **addrInfo, 
									bool isBindAddr )
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_socktype = sockType;
	hints.ai_flags= AI_NUMERICHOST | AI_PASSIVE;
	char portStr[32];
	sprintf(portStr, "%d", port);
	//LogMsg( LOG_INFO, "VxSktBase::toSocketAddrInfo %s:%d\n", addr, port ); 
	if( 0 != getaddrinfo( addr, portStr, &hints, addrInfo ))
	{
		setLastSktError( VxGetLastError() );
		LogMsg( LOG_INFO, "VxSktBase::toSocketAddrInfo: error %d %s\n", 
			getLastSktError(),
			VxDescribeSktError( getLastSktError() ) );
		return false;
	}

	if (isBindAddr == true)
	{
		return true;
	}

	hints.ai_family = (*addrInfo)->ai_family;
	freeaddrinfo(*addrInfo);
	if (getaddrinfo(NULL, portStr, &hints, addrInfo) != 0)
	{
		setLastSktError( VxGetLastError() );
		LogMsg( LOG_ERROR, "VxSktBase::toSocketAddrInfo: error %d %s\n",
			getLastSktError(),
			VxDescribeSktError( getLastSktError() ) );
		return false;
	}

	return true;
}

//============================================================================
bool VxSktBase::toSocketAddrIn(	const char *addr, 
								int port, 
								struct sockaddr_in *sockaddr, 
								bool isBindAddr )
{
	memset(sockaddr, 0, sizeof(sockaddr_in));

	sockaddr->sin_family = AF_INET;
	sockaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr->sin_port = htons((uint16_t)port);

	if( true == isBindAddr ) 
	{
		sockaddr->sin_addr.s_addr = inet_addr(addr);
		if (sockaddr->sin_addr.s_addr == INADDR_NONE) 
		{
			struct hostent *poHostEnt = gethostbyname(addr);
			if( NULL == poHostEnt )
			{
				return false;
			}

			memcpy(&(sockaddr->sin_addr), poHostEnt->h_addr, poHostEnt->h_length);
		}
	}

	return true;
}

//============================================================================
bool VxSktBase::bindSocket( struct addrinfo * poResultAddr )	
{
	setLastSktError( 0 );
	if( SOCKET_ERROR == bind( m_Socket, poResultAddr->ai_addr, (int)poResultAddr->ai_addrlen ) )
	{
		// connect error
		m_eSktCallbackReason = eSktCallbackReasonConnectError;
		setLastSktError( VxGetLastError() );
		LogMsg( LOG_ERROR, "VxSktBase::udpOpen: bind error %d %s\n", 
			getLastSktError(),
			VxDescribeSktError( getLastSktError() ) );
		m_pfnReceive( this, getRxCallbackUserData() );
		return false;
	}
	return true;
}

//============================================================================
bool VxSktBase::isIPv6Address(const char *addr)
{
	if( NULL == addr )
	{
		return false;
	}
	std::string addrStr = addr;
	if (addrStr.find(":") != std::string::npos)
	{
		return true;
	}

	return false;
}

//============================================================================
int VxSktBase::getIPv6ScopeID(const char *addr)
{
	if( false == isIPv6Address( addr ) )
	{
		return 0;
	}

	std::string addrStr = addr;
	int pos = (int)addrStr.find("%");
	if (pos == (int)std::string::npos)
	{
		return 0;
	}

	std::string scopeStr = addrStr.substr(pos+1, addrStr.length());
	return atoi(scopeStr.c_str());
}

//============================================================================
const char *VxSktBase::stripIPv6ScopeID( const char *addr, std::string &buf )
{
	std::string addrStr = addr;
	if( true == isIPv6Address( addr ) ) 
	{
		std::string::size_type pos = addrStr.find("%");
		if( pos != std::string::npos )
		{
			addrStr = addrStr.substr(0, pos);
		}
	}

	buf = addrStr;
	return buf.c_str();
}

//============================================================================
RCODE VxSktBase::joinMulticastGroup( InetAddress& oLclAddress, const char * muliticastGroupIp )
{
	setLastSktError( 0 );
	std::string strLclIp = oLclAddress.toStdString();

	struct ip_mreq mreq = {};
	mreq.imr_multiaddr.s_addr = inet_addr( muliticastGroupIp );
	mreq.imr_interface.s_addr = htonl( INADDR_ANY );
	if( setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, ( char* )&mreq, sizeof( mreq ) ) < 0 )
	{
		m_rcLastSktError = VxGetLastError();
		LogModule( eLogMulticast, LOG_ERROR, "VxSktUdp::joinMulticastGroup setsockopt mreq failed %s", VxDescribeSktError( m_rcLastSktError ) );
	}

	/*
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags= AI_NUMERICHOST | AI_PASSIVE;

	struct addrinfo * mcastAddrInfo; 
	struct addrinfo * lclAddrInfo;
	//LogMsg( LOG_INFO, "VxSktBase::joinMulticastGroup %s\n", mcastAddr ); 

	if( 0 != getaddrinfo( mcastAddr, NULL, &hints, &mcastAddrInfo ) )
	{
		setLastSktError( VxGetLastError() );
		LogModule( eLogMulticast, LOG_ERROR, "VxSktBase::joinGroup unable to get multicast address info error %d\n", getLastSktError() );
		return getLastSktError();
	}

	if( 0 != getaddrinfo( m_strLclIp.c_str(), NULL, &hints, &lclAddrInfo ) ) 
	{
		setLastSktError( VxGetLastError() );
        LogModule( eLogMulticast, LOG_ERROR, "VxSktBase::joinGroup unable to get local address info error %d\n", getLastSktError() );
		freeaddrinfo(mcastAddrInfo);
		return getLastSktError();
	}

	if( isIPv6Address( strLclIp.c_str() ) ) 
	{
		struct ipv6_mreq ipv6mr;
		struct sockaddr_in6 toaddr6, ifaddr6;
		memcpy(&toaddr6, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
		memcpy(&ifaddr6, lclAddrInfo->ai_addr, sizeof(struct sockaddr_in6));
		ipv6mr.ipv6mr_multiaddr = toaddr6.sin6_addr;	
		int iScopeID = getIPv6ScopeID( strLclIp.c_str() );
		ipv6mr.ipv6mr_interface = iScopeID; //if_nametoindex

		if( 0 != setsockopt( m_Socket, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&iScopeID, sizeof( iScopeID )) )
		{
			setLastSktError( VxGetLastError() );
            LogModule( eLogMulticast, LOG_ERROR, "VxSktBase::joinGroup set mulicast if error %d\n", getLastSktError() );
			return getLastSktError();
		}

		if( 0 != setsockopt( m_Socket, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&ipv6mr, sizeof(ipv6mr) ) )
		{
			setLastSktError( VxGetLastError() );
            LogModule( eLogMulticast, LOG_ERROR, "VxSktBase::joinGroup join group error %d\n", getLastSktError() );
			return getLastSktError();
		}
	}
	else 
	{
		struct ip_mreq ipmr;
		struct sockaddr_in toaddr, ifaddr;
		memcpy(&toaddr, mcastAddrInfo->ai_addr, sizeof(struct sockaddr_in));
		memcpy(&ifaddr, lclAddrInfo->ai_addr, sizeof(struct sockaddr_in));
		memcpy(&ipmr.imr_multiaddr.s_addr, &toaddr.sin_addr, sizeof(struct in_addr));
		memcpy(&ipmr.imr_interface.s_addr, &ifaddr.sin_addr, sizeof(struct in_addr));

		if( 0 != setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipmr, sizeof(ipmr) ) )
		{
			setLastSktError( VxGetLastError() );
            LogModule( eLogMulticast, LOG_ERROR, "VxSktBase::joinGroup set add membership error %d", getLastSktError() );
		}
		else
		{
			setLastSktError( 0 );
		}

        LogModule( eLogMulticast, LOG_INFO, "joinMulticastGroup: local ip %s group %s error if any %d",
				strLclIp.c_str(), 
				mcastAddr,
				getLastSktError()
				);
	}
	*/

	setTTL( 4 );
	setAllowLoopback( true );
	//freeaddrinfo(mcastAddrInfo);
	//freeaddrinfo(lclAddrInfo);
	return getLastSktError();
}

//============================================================================
void VxSktBase::setTTL( uint8_t ttl )
{
	if( 0 != setsockopt( m_Socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof( ttl )) )
	{
		setLastSktError( VxGetLastError() );
		LogMsg( LOG_ERROR, "VxSktBase::setTTL error %d\n", getLastSktError() ); 
	}
}

//============================================================================
void VxSktBase::setAllowLoopback( bool allowLoopback )
{
	int32_t loopbackOption = allowLoopback;

	if( 0 != setsockopt( m_Socket, 0, 11, (char *)&loopbackOption, sizeof( loopbackOption )) )
	{
		setLastSktError( VxGetLastError() );
		LogMsg( LOG_ERROR, "VxSktBase::setAllowLoopback error %d\n", getLastSktError() ); 
	}
}

//============================================================================
void VxSktBase::setAllowBroadcast( bool allowBroadcast )
{
	int32_t broadcastOption = allowBroadcast;

	if( 0 != setsockopt( m_Socket, 0xffff, 32, (char *)&broadcastOption, sizeof( broadcastOption )) )
	{
		setLastSktError( VxGetLastError() );
		LogMsg( LOG_ERROR, "VxSktBase::setAllowBroadcast error %d\n", getLastSktError() ); 
	}
}

//============================================================================
void VxSktBase::setReceiveCallback( VX_SKT_CALLBACK pfnReceive, void * pvRxCallbackUserData )
{
	m_pfnReceive = pfnReceive;
	m_pvRxCallbackUserData = pvRxCallbackUserData;
}

//============================================================================
//! Set Transmit Callback ( optional for transmit statistics )
void VxSktBase::setTransmitCallback( VX_SKT_CALLBACK pfnTransmit, void * pvTxCallbackUserData )
{
	m_pfnTransmit = pfnTransmit;
	m_pvTxCallbackUserData = pvTxCallbackUserData;
}

//============================================================================
//! set socket to blocking or not
RCODE VxSktBase::setSktBlocking( bool bBlock )
{
	RCODE rc = ::VxSetSktBlocking( m_Socket, bBlock );
	if ( rc )
	{
		setLastSktError( rc );
		LogMsg( LOG_ERROR, "VxSktBase::setSktBlocking skt %d ioctlsocket error %s\n", m_iSktId, VxDescribeSktError( getLastSktError() ) );
	}

	return rc;
}

//============================================================================
void VxSktBase::updateLastActiveTime( void )					
{ 
	setLastActiveTimeMs( GetGmtTimeMs() ); 
}

//============================================================================
RCODE VxSktBase::connectTo(	InetAddress&	oLclIp,
							const char *	pIpUrlOrIp,				// remote ip 
							uint16_t		u16Port,				// port to connect to
							int				iTimeoutMilliSeconds)	// milli seconds before connect attempt times out
{
	m_LclIp = oLclIp;
	m_strLclIp = m_LclIp.toStdString();
	if( isConnected() )
	{
		LogMsg( LOG_ERROR, "VxSktBase::connectTo: skt %d connect attempt on already connected socket\n", m_iSktId );
		vx_assert( false );
		return -1;
	}

	m_bIsConnected = false;
	// kill previous thread if any
	m_SktRxThread.killThread();

	LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::connectTo: skt %d ip %s port %d\n", this->m_iSktId, pIpUrlOrIp, u16Port );
	//resolve url
	std::string strUrlFile;
	uint16_t u16ReturnedPort;
	bool bResolved = VxResolveHostToIp(	pIpUrlOrIp,				//web name to resolve
										m_strRmtIp,
										u16ReturnedPort );		
	if( false == bResolved )
	{
		m_rcLastSktError = -1;
		m_eSktCallbackReason = eSktCallbackReasonConnectError;
        LogModule( eLogConnect, LOG_INFO, "VxSktBase::connectTo: skt %d could not resolve url %s\n", m_iSktId, pIpUrlOrIp );
		// cannot do callback except in thread because may cause mutex deadlock
		//m_pfnReceive( this );
		return getLastSktError();
	}

	m_RmtIp.setIp( m_strRmtIp.c_str() );
	m_RmtIp.setPort( u16Port );
	m_eSktCallbackReason	= eSktCallbackReasonConnecting;
	m_iConnectTimeout		= iTimeoutMilliSeconds;

	RCODE rc = doConnectTo();
	if( rc )
	{
		//LogMsg( LOG_INFO, "doConnectTo returned error %d\n", rc );
		return rc;
	}

	// make a useful thread name
	std::string strVxThreadName;
	StdStringFormat( strVxThreadName, "VxSktBaseTCPa_%d", m_iSktId );
	startReceiveThread( strVxThreadName.c_str() );
    LogModule( eLogConnect, LOG_VERBOSE,  "skt %d connected to %s:%d\n", m_iSktId, pIpUrlOrIp, u16Port );

	return 0;
}

//============================================================================
void VxSktBase::createConnectionUsingSocket( SOCKET skt, const char * rmtIp, uint16_t port )
{
	m_bIsConnected = false;
	m_SktRxThread.killThread();

	m_RmtIp.setIp( rmtIp );
	m_strRmtIp = rmtIp;
	m_eSktCallbackReason	= eSktCallbackReasonConnecting;
	m_iConnectTimeout		= 3000;
	m_bIsConnected			= true;
	m_Socket				= skt;

	std::string strVxThreadName;
	StdStringFormat( strVxThreadName, "VxSktBaseTCPb_%d", m_iSktId );
	startReceiveThread( strVxThreadName.c_str() );
    LogModule( eLogConnect, LOG_VERBOSE,  "createConnectionUsingSocket id %d connected to %s:%d\n", m_iSktId, rmtIp, port );
}

//============================================================================
//! Do connect to from thread
RCODE VxSktBase::doConnectTo( void )
{
	uint16_t u16Port = m_RmtIp.getPort();

	m_Socket = ::VxConnectTo( m_LclIp, m_RmtIp, u16Port, m_iConnectTimeout );

	if( INVALID_SOCKET != m_Socket )
	{
		if( m_rcLastSktError )
		{
			//LogMsg( LOG_INFO, "VxSktBase::doConnectTo: skt %d handle %d connect to %s get remote ip error %s\n",
			//	m_iSktId,
			//	m_Socket,
			//	m_strRmtIp.c_str(),
			//	VxDescribeSktError( m_rcLastError ) );
			// we connected.. dont error out just cause getpeername failed
			m_rcLastSktError = 0;
		}

        m_strLclIp = m_LclIp.toStdString();
        m_strRmtIp = m_RmtIp.toStdString();

        LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::doConnectTo: SUCCESS %s", describeSktConnection().c_str() );
		m_bIsConnected = true;
		return 0;
	}
	else
	{
		m_bIsConnected = false;
        LogModule( eLogConnect, LOG_ERROR, "VxSktBase::doConnectTo: FAILED INVALID_SKT skt %d connect to %s port %d\n",
			m_iSktId,
			m_strRmtIp.c_str(),
			m_RmtIp.getPort() );
		return -1;
	}
}

//============================================================================
std::string	 VxSktBase::describeSktConnection( void )
{
    std::string sktDesc;
    StdStringFormat( sktDesc, "%sid %d handle %d %s:%d%s%s:%d", DescribeSktType( getSktType() ), m_iSktId, m_Socket,
        m_strLclIp.c_str(), m_LclIp.getPort(), describeSktDirection().c_str(), m_strRmtIp.c_str(), m_RmtIp.getPort() );
    return sktDesc;
}

//============================================================================
void VxSktBase::closeSkt( ESktCloseReason closeReason, bool bFlushThenClose )
{
    if( m_SktCloseReason == eSktCloseReasonUnknown )
    {
        m_SktCloseReason = closeReason;
    }

    LogModule( eLogConnect, LOG_VERBOSE, "%s %s", DescribeSktCloseReason( closeReason ), describeSktConnection().c_str() );

	if( m_bClosingFromRxThread || m_bClosingFromDestructor )
	{
		m_SktRxThread.abortThreadRun( true );
		m_SktTxThread.abortThreadRun( true );
		m_SktTxSemaphore.signal();
		m_bIsConnected = false;
		if( INVALID_SOCKET != m_Socket )
		{
			if( !isUdpSocket() 
				&& ( 0 != getLastActiveTimeMs() )
				&& isRxCryptoKeySet()
				&& ( 0 != getLastSktError() ) )
			{
                LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::closeSkt: reason %s %s thread %d err %d %s\n", DescribeSktCloseReason( closeReason ), describeSktType().c_str(), VxGetCurrentThreadId(), getLastSktError(), describeSktError( getLastSktError() ) );
			}

			doCloseThisSocketHandle( bFlushThenClose );

			//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d handle %d close done\n", m_iSktId, oSocket );
		}

		// if thread tries to suicide then problems occurs because thread cannot
		// exit while attempting to kill itself
		if( (false == m_bClosingFromRxThread ) &&
			( m_SktRxThread.getThreadTid() != VxGetCurrentThreadTid() ) )
		{
			//LogMsg( LOG_INFO, "VxSktBase::closeSkt: Skt %d killing thread tid %d\n", m_iSktId, m_SktRxThread.getThreadTid() );
			m_SktRxThread.killThread();
		}
	}
	else
	{
		if( ( INVALID_SOCKET != m_Socket ) 
			&& !isUdpSocket() 
			&& ( 0 != getLastActiveTimeMs() )
			&& isRxCryptoKeySet()
			&& ( 0 != getLastSktError() ) )
		{
            LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::closeSkt: reason %s %s err %d %s\n", DescribeSktCloseReason( closeReason ), describeSktType().c_str(), getLastSktError(), describeSktError( getLastSktError() ) );
		}

		doCloseThisSocketHandle( bFlushThenClose );
	}
}

//============================================================================
void VxSktBase::doCloseThisSocketHandle( bool bFlushThenClose )
{
	// NOTE: if VxFlushThenCloseSkt is called instead of VxCloseSktNow then thread will not be released if waiting on
	// blocking socket operation
	if( INVALID_SOCKET != m_Socket )
	{	
		SOCKET oSocket = m_Socket; 
		m_Socket = INVALID_SOCKET;
		if( bFlushThenClose )
		{
			VxFlushThenCloseSkt( oSocket );
		}
		else
		{
			VxCloseSktNow( oSocket );
		}
	}
}

//============================================================================
//! send data without encrypting
RCODE VxSktBase::sendData(	const char *	pData,					// data to send
							int				iDataLen,				// length of data	
							bool			bDisconnectAfterSend )	// if true disconnect after data is sent
{
	if( m_Socket <= 0 )
	{
        LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::sendData: Attempted send on disconnected skt %s\n", this->describeSktType().c_str() );
		vx_assert( m_Socket > 0 );
	}


	if( false == isConnected() )
	{
        LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::sendData: Attempted send on disconnected skt %s\n", this->describeSktType().c_str() );
		return -1;
	}

    LogModule( eLogConnect, LOG_VERBOSE, "skt %d sendData length %d to %s:%d\n", m_iSktId, iDataLen, m_strRmtIp.c_str(), m_RmtIp.getPort() );
	if( INVALID_SOCKET != m_Socket )
	{
		int iSentLen;
		while( true )
		{
			iSentLen = send( m_Socket, (const char *)pData, iDataLen, 0);
			if( 0 > iSentLen )
			{
				setLastSktError( VxGetLastError() );
				if( 0 == m_rcLastSktError )
				{
					m_rcLastSktError = iSentLen;
				}

                LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase::sendData: Skt %d Handle %d Error %s\n",
									m_iSktId, 
									m_Socket, 
									VxDescribeSktError( m_rcLastSktError ) );
				if( bDisconnectAfterSend )
				{
					closeSkt( eSktCloseDisconnectAfterSend, true );
				}

				return getLastSktError();
			}

			pData = pData + iSentLen;
			iDataLen -= iSentLen;
			TxedPkt( iSentLen );
			m_iLastTxLen = iSentLen;
			if( m_pfnTransmit )
			{
				m_pfnTransmit( this, getTxCallbackUserData() );
			}

			if( 0 >= iDataLen )
			{
				// all done
				if( bDisconnectAfterSend )
				{
					closeSkt( eSktCloseSendComplete, true );
				}

				return 0;
			}
			// sleep and try again
			VxSleep( 20 );
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "INVALID SKT skt %d sendData length %d to %s:%d\n", m_iSktId, iDataLen, m_strRmtIp.c_str(), m_RmtIp.getPort() );
	}

	if( bDisconnectAfterSend )
	{
		closeSkt( eSktCloseDisconnectAfterSend, true );
	}

	return -1;
}

//============================================================================
//! encrypt then send data using session crypto
RCODE VxSktBase::txEncrypted(	const char *	pDataIn, 		// data to send
								int				iDataLen,		// length of data
								bool			bDisconnect )	// if true disconnect after send
{
    if( !pDataIn )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted null data");
        if( bDisconnect )
        {
            closeSkt( eSktCloseCryptoNullData );
        }

        vx_assert( pDataIn );
        return -1;
    }

    if( !iDataLen )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted invalid data len %d", iDataLen);
        if( bDisconnect )
        {
            closeSkt( eSktCloseCryptoInvalidLength );
        }

        vx_assert( pDataIn );
        return -2;
    }

	if( 0 != (iDataLen & 0x0f) )
	{
		LogMsg( LOG_ERROR, "VxSktBase::txEncrypted invalid pkt len %d (pkt type %d)", iDataLen, ((VxPktHdr *)pDataIn)->getPktType() );
        if( bDisconnect )
        {
            closeSkt(eSktClosePktLengthInvalid);
        }

        vx_assert( 0 == (iDataLen & 0x0f) );
        return -3;
	}

    if( !m_TxCrypto.isKeyValid() )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted invalid crypto key");
        vx_assert( m_TxCrypto.isKeyValid() );
        if( bDisconnect )
        {
            closeSkt(eSktCloseCryptoInvalidKey);
        }

        return -4;
    }

	// make copy of data so data is not destroyed
	unsigned char * pu8Data = new unsigned char[ iDataLen ];
	memcpy( pu8Data, pDataIn, iDataLen );

    // protect in case we are sending from thread other than rx thread
    m_TxMutex.lock();
	// encrypt
	RCODE rc =	m_TxCrypto.encrypt( pu8Data, iDataLen );
	if( rc )
	{
		LogMsg( LOG_ERROR, "VxSktBase::txEncrypted: crypto error %d", rc );
		vx_assert( 0 == rc );
	}
	else
	{
		// send
		rc = this->sendData( (char *)pu8Data, iDataLen );
	}

	delete[] pu8Data;
    setLastSktError( rc );
    m_TxMutex.unlock();
    if( rc )
    {
        LogModule( eLogTcpData, LOG_ERROR, "VxSktBase::txEncrypted: sendData error %d", rc );
    }

	if( bDisconnect )
	{
		closeSkt(eSktCloseDisconnectAfterSend, true);
	}

	return rc;
}

//============================================================================
//! encrypt with given key then send.. does not affect session crypto
RCODE VxSktBase::txEncrypted(	VxKey *			poKey,			// key to encrypt with
								const char *	pDataIn,		// data to send
								int				iDataLen,		// length of data
								bool			bDisconnect )	// if true disconnect after send
{
    if( !pDataIn )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted2 null data");
        if( bDisconnect )
        {
            closeSkt( eSktCloseCryptoNullData );
        }

        vx_assert( pDataIn );
        return -1;
    }

    if( !iDataLen )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted2 invalid data len %d", iDataLen);
        if( bDisconnect )
        {
            closeSkt( eSktCloseCryptoInvalidLength );
        }

        vx_assert( pDataIn );
        return -2;
    }

    if( 0 != (iDataLen & 0x0f) )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted2 invalid pkt len %d (pkt type %d)", iDataLen, ((VxPktHdr *)pDataIn)->getPktType() );
        if( bDisconnect )
        {
            closeSkt( eSktClosePktLengthInvalid );
        }

        vx_assert( 0 == (iDataLen & 0x0f) );
        return -3;
    }

    if( !poKey || !poKey->isKeySet() )
    {
        LogMsg( LOG_ERROR, "VxSktBase::txEncrypted2 invalid crypto key");
        vx_assert( m_TxCrypto.isKeyValid() );
        if( bDisconnect )
        {
            closeSkt(eSktCloseCryptoInvalidKey);
        }

        return -4;
    }

	// make copy of data so data is not destroyed
	char * pData = new char[ iDataLen ];
	memcpy( pData, pDataIn, iDataLen );

    // protect in case we are sending from thread other than rx thread
    m_TxMutex.lock();
	// encrypt
	VxSymEncrypt( poKey, (char *)pData, iDataLen );
	// send
	RCODE rc = this->sendData( (char *)pData, iDataLen );
	delete[] pData;
    m_TxMutex.unlock();

	if( bDisconnect )
	{
		closeSkt(eSktCloseDisconnectAfterSend, true);
	}

	if( rc )
	{
        LogModule( eLogTcpData, LOG_ERROR, "VxSktBase::txEncrypted: error %d\n", rc );
	}

	return rc;
}

//============================================================================
RCODE VxSktBase::txPacket(	VxGUID				destOnlineId,
							VxPktHdr *			pktHdr, 		// packet to send
							bool				bDisconnect )	// if true disconnect after send
{
	pktHdr->setDestOnlineId( destOnlineId );
	return txPacketWithDestId( pktHdr, bDisconnect );
}

//============================================================================
RCODE VxSktBase::txPacketWithDestId(	VxPktHdr *			pktHdr, 		// packet to send
										bool				bDisconnect )	// if true disconnect after send
{
	m_u8TxSeqNum = (uint8_t)rand();
	pktHdr->setPktSeqNum( m_u8TxSeqNum );
	vx_assert( pktHdr->getDestOnlineId().isVxGUIDValid() );
	return txEncrypted( (const char *)pktHdr, pktHdr->getPktLength(), bDisconnect );
}

//============================================================================
//! decrypt as much as possible in receive buffer
RCODE VxSktBase::decryptReceiveData( void )
{
	if( false == isRxEncryptionKeySet() )
	{
		// no key to decrypt with
		//LogMsg( LOG_INFO, "No Rx Crypto Key Set %s\n", describeSktType().c_str() );
		return -1;
	}

	lockCryptoAccess();
    uint32_t u32Datalen = getSktBufDataLen();
	// truncate to 16 byte boundary
	u32Datalen = u32Datalen & 0xfffffff0;
	if( u32Datalen )
	{
		vx_assert( u32Datalen >= m_u32RxDecryptedLen );
		int32_t u32LenToDecrypt = u32Datalen - m_u32RxDecryptedLen;
		if( u32LenToDecrypt )
		{
			m_RxCrypto.decrypt( &m_pau8SktBuf[ m_u32RxDecryptedLen ], u32LenToDecrypt );
			m_u32RxDecryptedLen += u32LenToDecrypt;
		}
	}

	unlockCryptoAccess();
	return 0;
}

//============================================================================
//! return true if is connected
bool VxSktBase::isConnected( void )
{
	if( INVALID_SOCKET == m_Socket )
	{
		m_bIsConnected = false;
	}

	return m_bIsConnected;
}

//============================================================================
//! get the sockets peer connection ip address as net order int32_t
RCODE VxSktBase::getRemoteIp(	InetAddress &u32RetIp,		// return ip
								uint16_t &u16RetPort )	// return port
{
	u32RetIp = m_RmtIp;
	u16RetPort = m_RmtIp.getPort();
	return 0;
}

//============================================================================
std::string VxSktBase::getRemoteIp( void )
{
	return m_strRmtIp.c_str();
}

//============================================================================
std::string VxSktBase::getLocalIp( void )
{
	return m_strLclIp.c_str();
}

//============================================================================
void VxSktBase::setRmtAddress( struct sockaddr_storage& oSktAddr )
{
	m_RmtIp.setIpAndPort( oSktAddr );
	m_strRmtIp = m_RmtIp.toStdString();
}

//============================================================================
void VxSktBase::setRmtAddress( struct sockaddr& oSktAddr )
{
	m_RmtIp.setIpAndPort( oSktAddr );
	m_strRmtIp = m_RmtIp.toStdString();
}

//============================================================================
void VxSktBase::setRmtAddress( struct sockaddr_in& oSktAddrIn )
{
	struct sockaddr_storage oSktAddr;
	memset( &oSktAddr, 0, sizeof( oSktAddr ) );
	memcpy( &oSktAddr, &oSktAddrIn, sizeof( oSktAddrIn ) );

	setRmtAddress( oSktAddr );
}

//============================================================================
void VxSktBase::setLclAddress( struct sockaddr_storage& oSktAddr )
{
	m_LclIp.setIpAndPort( oSktAddr );
	m_strLclIp = m_LclIp.toStdString();
}

//============================================================================
void VxSktBase::startReceiveThread( const char * pVxThreadName )
{
	m_SktRxThread.killThread();
    setInUseByRxThread( true );
	m_SktRxThread.startThread( (VX_THREAD_FUNCTION_T)VxSktBaseReceiveVxThreadFunc, this, pVxThreadName );
}

//============================================================================
void VxSktBase::setLastSktError( RCODE rc )						
{ 
	m_rcLastSktError = rc; 
	if( ( 0 != rc ) 
		&& ( -1 != rc ) 
		&& ( 11 != rc ) )
	{
		if( !isUdpSocket() && ( 0 != getLastActiveTimeMs() ) )
		{
			LogMsg( LOG_INFO, "VxSktBase::setLastSktError: %s %d %s\n", describeSktType().c_str(), m_rcLastSktError, describeSktError( m_rcLastSktError ) );
		}
	}
}

//============================================================================
void * VxSktBaseReceiveVxThreadFunc( void * pvContext )
{
	VxThread * poVxThread = (VxThread *)pvContext;
    VxSktBase * sktBase = nullptr;
    if( !poVxThread->isAborted() )
    {
        poVxThread->setIsThreadRunning( true );
        sktBase = (VxSktBase *)poVxThread->getThreadUserParam();
        if( sktBase )
        {
            LogModule( eLogConnect, LOG_VERBOSE, "VxSktBase rx thread 0x%x started for skt %d skt id %d ", VxGetCurrentThreadId(), sktBase->getSktHandle(), sktBase->getSktId() );

            char as8Buf[ 0x8000 ];
            int iDataLen = 0;
            //int iBufferAlmostFull = sktBase->getSktBufSize() - sktBase->getSktBufSize() / 10;
            struct sockaddr_storage oAddr;
            bool bIsUdpSkt = true;

            if( eSktTypeTcpConnect == sktBase->getSktType() ||
                eSktTypeTcpAccept == sktBase->getSktType() )
            {
                bIsUdpSkt = false;
                sktBase->setLastImAliveTimeMs(  GetGmtTimeMs() ); // so we don't get closed if takes awhile for everything to get going
                if( eSktTypeTcpConnect == sktBase->getSktType()  )
                {
                    // we couldn't do callbacks in connect function ( mutex issues ) so
                    // do the callback now
                    sktBase->setCallbackReason( eSktCallbackReasonConnecting );
                    sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
                }
            }

            if(	( poVxThread->isAborted() ) ||
                ( INVALID_SOCKET == sktBase->m_Socket ) )
            {
                // something has already happened to the connection
                //! VxThread calls this just before exit
                poVxThread->threadAboutToExit();
                sktBase->setInUseByRxThread( false );
                return nullptr;
            }

            sktBase->setLastSktError( 0 );
            sktBase->setIsConnected( true );
            sktBase->setCallbackReason( eSktCallbackReasonConnected );
            sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
            sktBase->setCallbackReason( eSktCallbackReasonData );

            //LogMsg( LOG_SKT, "VxSktBaseReceiveVxThreadFunc: set blocking true\n" );
            sktBase->setSktBlocking( true );

            while(	( false == poVxThread->isAborted() ) &&
                    ( INVALID_SOCKET != sktBase->m_Socket ) &&
                    ( eSktCallbackReasonData == sktBase->getCallbackReason() ) )
            {
                if( !sktBase->isConnected() )
                {
                    if( !sktBase->isUdpSocket() && ( 0 != sktBase->getLastActiveTimeMs() ) )
                    {
                        LogModule( eLogConnect, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: %s skt %d skt id %d no longer connected thread 0x%x",
                                   sktBase->describeSktType().c_str(), sktBase->getSktHandle(), sktBase->getSktId(), VxGetCurrentThreadId() );
                    }

                    break;
                }

                int iAttemptLen = sktBase->getSktBufFreeSpace();
                if( iAttemptLen <= 0 )
                {
                    LogMsg( LOG_VERBOSE, "skt %d handle %d buffer is full ", sktBase->getSktId(), sktBase->getSktHandle() );
                }

                vx_assert( iAttemptLen >= 0 );
                if( iAttemptLen >= (int)sizeof( as8Buf ) )
                {
                    iAttemptLen = (int)sizeof( as8Buf ) - 16;
                }

                if( bIsUdpSkt )
                {
					LogModule( eLogUdpData, LOG_VERBOSE, "udp wait for rx attempt len %d on skt %d skt id %d lcl ip %s thread 0x%x",
						iAttemptLen, sktBase->getSktHandle(), sktBase->getSktId(), sktBase->m_strLclIp.c_str(), VxGetCurrentThreadId() );

					if( INVALID_SOCKET == sktBase->m_Socket )
					{
						LogModule( eLogUdpData, LOG_VERBOSE, "udp closing with invalid socket" );
						// has been closed by outside thread
						sktBase->setCallbackReason( eSktCallbackReasonClosing );
						goto closed_skt_exit;
					}

					if( sktBase->getIsMulticastSkt() )
					{
						socklen_t sktAddrLen = sktBase->getMulticastRxAddrLen();

						iDataLen = recvfrom( sktBase->m_Socket,	// socket
							as8Buf,				// buffer to read into
							iAttemptLen,		// length of buffer space
							0,					// flags
							( struct sockaddr* )&sktBase->getMulticastRxAddr(), // source address
							&sktAddrLen );		// size of address structure
					}
					else
					{
						memset( &oAddr, 0, sizeof( struct sockaddr_storage ) );
						socklen_t iSktAddrLen = sizeof( struct sockaddr_storage );
						if( sktBase->m_LclIp.isIPv4() )
						{
							struct sockaddr_in addr;
							memset( &addr, 0, sizeof( addr ) );
							addr.sin_family = AF_INET;
							addr.sin_addr.s_addr = htonl( INADDR_ANY ); // differs from sender
							addr.sin_port = htons( sktBase->getMulticastPort() );

							iSktAddrLen = sizeof( addr );

							iDataLen = recvfrom( sktBase->m_Socket,	// socket
								as8Buf,				// buffer to read into
								iAttemptLen,		// length of buffer space
								0,					// flags
								( struct sockaddr* )&addr, // source address
								&iSktAddrLen );		// size of address structure
						}
						else
						{
							LogModule( eLogUdpData, LOG_INFO, "udp recvfrom IPV6" );
							iSktAddrLen = sizeof( struct sockaddr_in6 );
							( ( struct sockaddr_in6* )&oAddr )->sin6_family = AF_INET6;
							iDataLen = recvfrom( sktBase->m_Socket,	// socket
								as8Buf,				// buffer to read into
								iAttemptLen,		// length of buffer space
								0,					// flags
								( struct sockaddr* )&oAddr, // source address
								&iSktAddrLen );		// size of address structure
						}
					}

					if( poVxThread->isAborted() || !sktBase->isConnected() )
					{
						LogModule( eLogUdpData, LOG_VERBOSE, "udp recvfrom skt %d skt id close from thread",
								   sktBase->getSktHandle(), sktBase->getSktId() );
						goto closed_skt_exit;
					}

					if( iDataLen < 0 )
					{
						RCODE rc = VxGetLastError();
						LogModule( eLogUdpData, LOG_VERBOSE, "udp recvfrom skt %d skt id %d port %d error %d",
							sktBase->getSktHandle(), sktBase->getSktId(), sktBase->m_RmtIp.getPort(), rc );
						VxSleep( 500 );
						continue;
					}
                    else if( iDataLen > 0 )
                    {
						if( sktBase->getIsMulticastSkt() )
						{
							// dont decode.. multicast handler will do that
							sktBase->RxedPkt( iDataLen );
							// call back user with the good news.. we have data
							LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: multicast skt %d skt Id %d receiving len %d thread 0x%x",
								sktBase->getSktHandle(), sktBase->getSktId(), iDataLen, VxGetCurrentThreadId() );

							sktBase->m_iLastRxLen = iDataLen;
							memcpy( sktBase->getSktWriteBuf(), as8Buf, iDataLen );
							sktBase->sktBufAmountWrote( iDataLen );
							sktBase->updateLastActiveTime();
							sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
							sktBase->setLastSktError( 0 );
							if( poVxThread->isAborted() || !sktBase->isConnected() )
							{
								LogModule( eLogUdpData, LOG_VERBOSE, "udp recvfrom skt %d skt id close from thread",
									sktBase->getSktHandle(), sktBase->getSktId() );
								sktBase->setCallbackReason( eSktCallbackReasonClosing );
								goto closed_skt_exit;
							}

							continue;
						}
						else
						{
							sktBase->m_RmtIp.m_u16Port = sktBase->m_RmtIp.setIp( oAddr );
							sktBase->m_strRmtIp = sktBase->m_RmtIp.toStdString();

							LogModule( eLogUdpData, LOG_VERBOSE, "udp recvfrom skt %d skt id %d len %d port %d thread 0x%x",
								sktBase->getSktHandle(), sktBase->getSktId(), iDataLen, sktBase->m_RmtIp.getPort(), VxGetCurrentThreadId() );
							if( sktBase->m_RxKey.isKeySet() )
							{
								if( false == sktBase->m_RxKey.isValidDataLen( iDataLen ) )
								{
									// throw away the data because not valid length
									LogMsg( LOG_INFO, "udp recvfrom not valid data length\n" );
									continue;
								}
								// set encryption context
								sktBase->m_RxCrypto.importKey( &sktBase->m_RxKey );
							}
						}
                    }
                    else
                    {
                        LogModule( eLogUdpData, LOG_INFO, "VxSktBaseReceiveVxThreadFunc: udp recvfrom skt %d skt id %d len %d thread 0x%x", sktBase->getSktHandle(), sktBase->getSktId(), iDataLen, VxGetCurrentThreadId() );
                    }
                }
                else
                {
                    // TCP data
                    //LogMsg( LOG_SKT, "VxSktBaseReceiveVxThreadFunc: recv skt handle %d attempt len %d\n", sktBase->m_Socket, iAttemptLen );

                    iDataLen = recv(		sktBase->m_Socket,	// socket
                                            as8Buf,				// buffer to read into
                                            iAttemptLen,		// length of buffer space
                                            0 );				// flags
                    if( INVALID_SOCKET == sktBase->m_Socket )
                    {
                        // has been closed by outside thread
                        sktBase->setCallbackReason( eSktCallbackReasonClosing );
                        goto closed_skt_exit;
                    }

                    LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: tcp recv skt %d skt id %d rxed len %d attempt len %d thread 0x%x",
                               sktBase->getSktHandle(), sktBase->getSktId(), iDataLen, iAttemptLen, VxGetCurrentThreadId() );
                }

                if( poVxThread->isAborted()
                    || ( eSktCallbackReasonData != sktBase->getCallbackReason() )
                    || ( INVALID_SOCKET == sktBase->m_Socket )
                    || ( iDataLen <= 0 ) )
                {
                    if( poVxThread->isAborted() || ( INVALID_SOCKET == sktBase->m_Socket ) )
                    {
                        // normal close or shutdown
                        //if( !sktBase->isUdpSocket() && ( 0 != sktBase->getLastActiveTime() ) )
                        //{
                        //	LogMsg( LOG_SKT, "VxSktBaseReceiveVxThreadFunc: skt %d 0x%x closed or aborted\n", sktBase->m_Socket, sktBase );
                        //}

                        sktBase->setLastSktError( 0 );
                        break;
                    }

                    // socket error occurred
                    sktBase->setLastSktError(  VxGetLastError() );
                    if( 0 == sktBase->getLastSktError() )
                    {
                        sktBase->setLastSktError( -1 );
                    }

        #ifdef TARGET_OS_WINDOWS
                    if ((iDataLen < 0)
                        && ((11 == sktBase->getLastSktError()) || (WSAEWOULDBLOCK == sktBase->getLastSktError())))
        #else
                    if ((iDataLen < 0)
                        && ((11 == sktBase->getLastSktError()) || (EINPROGRESS == sktBase->getLastSktError())))
        #endif // TARGET_OS_WINDOWS
                    {
                        // try again
                        LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: skt %d skt Id %d ip %s trying again thread 0x%x",
                                   sktBase->getSktHandle(), sktBase->getSktId(), sktBase->m_strLclIp.c_str(), VxGetCurrentThreadId() );
                        if( sktBase->checkForImAliveTimeout( true ) )
                        {
                            LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: skt %d skt Id %d ip %s I Am Alive Timeout thread 0x%x",
                                       sktBase->getSktHandle(), sktBase->getSktId(), sktBase->m_strLclIp.c_str(), VxGetCurrentThreadId() );
                            sktBase->setLastSktError( EIM_ALIVE_TIMEDOUT );
                            break;
                        }

                        VxSleep( SKT_RX_RETRY_SLEEP_TIME_MS );
                        continue;
                    }

                    break;
                }

                sktBase->setLastSktError( 0 );
                if( iDataLen > 0 )
                {
                    //if( sktBase->isTcpSocket() )
                    //{
                    //	LogMsg( LOG_SKT,  "skt %d Received length %d from %s:%d\n",
                    //		sktBase->m_iSktId,
                    //		iDataLen,
                    //		sktBase->m_strRmtIp.c_str(),
                    //		sktBase->m_RmtIp.getPort() );
                    //}

                    sktBase->m_iLastRxLen = iDataLen;

                    memcpy( sktBase->getSktWriteBuf(), as8Buf, iDataLen );
                    sktBase->sktBufAmountWrote( iDataLen );
                    if( false == sktBase->isUdpSocket() )
                    {
                        // decrypt as much as possible
                        sktBase->decryptReceiveData();
                    }

                    sktBase->RxedPkt( iDataLen );
                    // call back user with the good news.. we have data
                    LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: skt %d skt Id %d receiving len %d thread 0x%x",
                               sktBase->getSktHandle(), sktBase->getSktId(), iDataLen, VxGetCurrentThreadId() );
                    sktBase->updateLastActiveTime();
                    sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
                }
            }

        closed_skt_exit:
            sktBase->setIsConnected( false );
            if( eSktCallbackReasonNewMgr != sktBase->getCallbackReason() )
            {
                if( 0 != sktBase->getLastSktError() )
                {
                    if( !sktBase->isUdpSocket() && ( 0 != sktBase->getLastActiveTimeMs() ) )
                    {
                        LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: %s skt %d skt Id %d exit with error %d %s thread 0x%x",
                                    sktBase->describeSktType().c_str(),
                                    sktBase->getSktHandle(), sktBase->getSktId(),
                                    sktBase->getLastSktError(),
                                    sktBase->describeSktError( sktBase->getLastSktError() ),
                                    VxGetCurrentThreadId()
                                 );
                    }

                    // we had a error
                    sktBase->setCallbackReason( eSktCallbackReasonError );
                    sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
                }

                if( false == poVxThread->isAborted() )
                {
                    // we are closing due to error .. not because user called close
                    sktBase->m_bClosingFromRxThread = true;
                }

                sktBase->setCallbackReason( eSktCallbackReasonClosing );
                sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
                sktBase->setCallbackReason( eSktCallbackReasonClosed );
                sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
            }

            LogModule( eLogTcpData, LOG_VERBOSE, "VxSktBaseReceiveVxThreadFunc: skt %d skt Id %d  exiting thread 0x%x", sktBase->getSktHandle(), sktBase->getSktId(), VxGetCurrentThreadId() );

                //if( !sktBase->isUdpSocket() && ( 0 != sktBase->getLastActiveTime() ) )
                //{
                //	LogMsg( LOG_INFO, "VxSktBaseReceiveVxThreadFunc: exiting %s\n", sktBase->describeSktType().c_str() );
                //}

            if( INVALID_SOCKET != sktBase->m_Socket )
            {
                poVxThread->abortThreadRun( true );
                sktBase->m_bClosingFromRxThread = true;
                if( sktBase->getLastSktError() )
                {
                    sktBase->closeSkt( eSktCloseSktWithError );
                }
                else
                {
                    sktBase->closeSkt( eSktCloseNotNeeded );
                }
            }

            if( sktBase->m_SktMgr )
            {
                sktBase->m_SktMgr->handleSktCloseEvent( sktBase );
            }
        }
    }

	if( sktBase && sktBase->isUdpSocket() )
	{
		LogModule( eLogUdpData, LOG_VERBOSE, "udp skt id %d handle %d exit thread last error %d", sktBase->getSktId(), sktBase->getSktHandle(), sktBase->getLastSktError() );;
	}

    poVxThread->abortThreadRun( true );
    if( sktBase )
    {
        sktBase->setInUseByRxThread( false );
    }

	poVxThread->threadAboutToExit();
    return nullptr;
}

//============================================================================
void VxSktBase::setTxCryptoPassword( const char * data, int len )
{
	m_TxKey.m_bIsSet = true;
	m_TxCrypto.setPassword( data, len );
}

//============================================================================
void VxSktBase::setRxCryptoPassword( const char * data, int len )
{
	m_RxKey.m_bIsSet = true;
	m_RxCrypto.setPassword( data, len );
}

//============================================================================
bool VxSktBase::isTxCryptoKeySet( void )
{
	return m_TxKey.m_bIsSet;
}

//============================================================================
bool VxSktBase::isRxCryptoKeySet( void )
{
	return m_RxKey.m_bIsSet;
}

//============================================================================
const char * VxSktBase::describeSktError( RCODE rc )
{
	return VxDescribeSktError( rc );
}

//============================================================================
const char * VxSktBase::describeSktCallbackReason( ESktCallbackReason reason )
{
    switch( reason )
    {
    case eSktCallbackReasonUnknown:
        return "SktCallbackReasonUnknown";
    case eSktCallbackReasonConnecting:
        return "eSktCallbackReasonConnecting";
    case eSktCallbackReasonConnectError:
        return "eSktCallbackReasonConnectError";
    case eSktCallbackReasonConnected:
        return "eSktCallbackReasonConnected";
    case eSktCallbackReasonData:
        return "eSktCallbackReasonData";
    case eSktCallbackReasonClosing:
        return "eSktCallbackReasonClosing";
    case eSktCallbackReasonClosed:
        return "eSktCallbackReasonClosed";
    case eSktCallbackReasonError:
        return "eSktCallbackReasonError";
    case eSktCallbackReasonNewMgr:
        return "eSktCallbackReasonNewMgr";
    default:
        return "SktCallbackReasonInvalid";
    }
}

//============================================================================
std::string VxSktBase::describeSktType( void )
{
    std::string typeDesc;
    StdStringFormat( typeDesc, "skt id %d skt %d  %s %s last active %s ", 
                     m_iSktId, 
                     m_Socket,
					 m_strRmtIp.c_str(), 
					 isAcceptSocket() ? "accept" : (isUdpSocket() ? "udp" : "connect" ),
					 ( 0 == getLastActiveTimeMs() ) ? "never" : VxTimeUtil::formatTimeStampIntoHoursAndMinutesAndSeconds( GmtTimeMsToLocalTimeMs( getLastActiveTimeMs() ) ).c_str() );
    return typeDesc;
}

//============================================================================
bool VxSktBase::setPeerPktAnn( PktAnnounce &peerAnn )
{
    m_PeerAnnMutex.lock();
    bool isSameSize = peerAnn.getPktLength() && peerAnn.getPktLength() == m_PeerPktAnn.getPktLength();
    if( isSameSize )
    {
        memcpy( &m_PeerPktAnn, &peerAnn, m_PeerPktAnn.getPktLength() );
        m_PeerOnlineId = m_PeerPktAnn.getMyOnlineId();
        setIsPeerPktAnnSet( isSameSize );
    }

    m_PeerAnnMutex.unlock();
    return isSameSize;
}

//============================================================================
bool VxSktBase::getPeerPktAnnCopy( PktAnnounce& peerAnn )
{
    m_PeerAnnMutex.lock();
    bool copyResult = getIsPeerPktAnnSet() && peerAnn.getPktLength() && peerAnn.getPktLength() == m_PeerPktAnn.getPktLength();
    if( copyResult )
    {
        memcpy( &peerAnn, &m_PeerPktAnn, m_PeerPktAnn.getPktLength() );
    }

    m_PeerAnnMutex.unlock();
    return copyResult;
}

//============================================================================
void VxSktBase::dumpSocketStats( const char* reason, bool fullDump )
{
    std::string reasonMsg = reason ? reason : "";
    LogModule( eLogConnect, LOG_VERBOSE, "Dump %s connected ? %d last active %s", describeSktConnection().c_str(), isConnected(),
        ( 0 == getLastActiveTimeMs() ) ? "never" : VxTimeUtil::formatTimeStampIntoHoursAndMinutesAndSeconds( GmtTimeMsToLocalTimeMs( getLastActiveTimeMs() ), true ).c_str() );
}

//============================================================================
const std::string& VxSktBase::describeSktDirection( void )
{
    switch( m_eSktType )
    {
    case eSktTypeTcpConnect: return m_SktDirConnect;
    case eSktTypeTcpAccept: return m_SktDirAccept;
    case eSktTypeUdp: return m_SktDirUdp;
    case eSktTypeUdpBroadcast: return m_SktDirBroadcast;
    case eSktTypeLoopback: return m_SktDirLoopback;
    case eSktTypeNone: return m_SktDirUnknown;
    default: return m_SktDirUnknown;
    }
}

