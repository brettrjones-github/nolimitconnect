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

#include "VxSktUdp.h"
#include "VxSktUtil.h"

#include <CoreLib/VxParse.h>

#include <time.h>
#include <memory.h>
#include <stdio.h>

//#define DEBUG_VXSKT_UDP 1

//============================================================================
VxSktUdp::VxSktUdp()
: VxSktBase()
{
	m_eSktType = eSktTypeUdp;
}

//============================================================================
RCODE VxSktUdp::udpOpen( InetAddress& oLclIp, uint16_t u16Port, bool enableReceive )	
{
	struct addrinfo * poResultAddr = 0;
#ifdef DEBUG_VXSKT_UDP
	LogMsg( LOG_INFO, "udpOpen port %d", u16Port );
#endif // DEBUG_VXSKT_UDP
	RCODE rc = createSocket( oLclIp, u16Port, &poResultAddr );
	if( 0 == rc )
	{
		// for udp always allow reuse
		int reusePort = 1;
		if( 0 != setsockopt( m_Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reusePort, sizeof( int ) ) )
		{
			LogMsg( LOG_ERROR, "VxSktUdp::udpOpen setReuseSocket error %d", VxGetLastError() );
		}

		if( 0 != bind( m_Socket, ( sockaddr* )&poResultAddr, sizeof( struct addrinfo ) ) )
		{
			LogMsg( LOG_ERROR, "VxSktUdp::udpOpen bind error %d", VxGetLastError() );
		}

#if USE_BIND_LOCAL_IP
		if( false == VxBindSkt( m_Socket, oLclIp, u16Port ) )
		{
			m_rcLastSktError = VxGetLastError();
			if( 0 == m_rcLastSktError )
			{
				m_rcLastSktError = -1;
			}
			rc = m_rcLastSktError;
		}
#endif // #if USE_BIND_LOCAL_IP
	}

	if( ( 0 == rc ) && enableReceive )
	{
		startReceive();
	}

	if( rc )
	{
		LogMsg( LOG_ERROR, "ERROR udpOpen error %d", rc );
	}
	else if( false == enableReceive )
	{
		m_bIsConnected = true;
	}

	return rc;
}

//============================================================================
RCODE VxSktUdp::udpOpenUnicast( InetAddress& oLclIp, uint16_t u16Port )
{
	struct addrinfo * poResultAddr = 0;
	RCODE rc = createSocket( oLclIp, u16Port, &poResultAddr );
#if USE_BIND_LOCAL_IP
	if( 0 == rc )
	{
		if( false == VxBindSkt( m_Socket, oLclIp, u16Port ) )
		{
			rc = -1;
		}
	}
#endif // USE_BIND_LOCAL_IP

	if( 0 == rc )
	{
		startReceive();
	}

	if( rc )
	{
		LogMsg( LOG_ERROR, "udpOpenUnicast error %d", rc );
	}

	return rc;
}

//============================================================================
RCODE VxSktUdp::udpOpenMulticast( std::string multicastGroupIp, uint16_t u16Port, bool enableReceiveThread )
{
	m_rcLastSktError = 0;
	if( isConnected() )
	{
		closeSkt( eSktCloseUdpCreate );
	}

	// only ipV4 supported
	m_LclIp;
	m_strLclIp = m_LclIp.toStdString();
	setMulticastGroupIp( multicastGroupIp );
	setMulticastPort( u16Port );

	m_LclIp.setPort( u16Port );
	m_RmtIp.setPort( u16Port );
	m_strRmtIp = "";
	m_eSktCallbackReason = eSktCallbackReasonConnecting;
	if( m_pfnReceive )
	{
        // m_pfnReceive( this, getRxCallbackUserData() );
	}

	m_Socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if( INVALID_SOCKET == m_Socket )
	{
		// create socket error
		m_rcLastSktError = VxGetLastError();
		LogMsg( LOG_ERROR, "VxSktUdp::udpOpenMulticast: socket create error %s", VxDescribeSktError( m_rcLastSktError ) );

		m_eSktCallbackReason = eSktCallbackReasonConnectError;
		if( m_pfnReceive )
		{
            // m_pfnReceive( this, getRxCallbackUserData() );
		}

		return m_rcLastSktError;
	}

	// allow multiple sockets to use the same PORT number
	u_int yes = 1;
	if( 0 > setsockopt( m_Socket, SOL_SOCKET, SO_REUSEADDR, ( char* )&yes, sizeof( yes ) ) )
	{
		LogMsg( LOG_ERROR, "VxSktUdp::udpOpenMulticast: Reusing ADDR failed error %s", VxDescribeSktError( m_rcLastSktError ) );
	}
	
	// set up destination address
	memset( &m_MulticastRxAddr, 0, sizeof( m_MulticastRxAddr ) );
	m_MulticastRxAddr.sin_family = AF_INET;
	m_MulticastRxAddr.sin_addr.s_addr = htonl( INADDR_ANY ); // differs from sender
	m_MulticastRxAddr.sin_port = htons( u16Port );

	// bind to receive address
	if( bind( m_Socket, ( struct sockaddr* )&m_MulticastRxAddr, sizeof( m_MulticastRxAddr ) ) < 0 ) 
	{
		m_rcLastSktError = VxGetLastError();
		LogMsg( LOG_ERROR, "VxSktUdp::udpOpenMulticast: socket bind error %s", VxDescribeSktError( m_rcLastSktError ) );

		m_eSktCallbackReason = eSktCallbackReasonConnectError;
		if( m_pfnReceive )
		{
			m_pfnReceive( this, getRxCallbackUserData() );
		}

		closeSkt( eSktCloseUdpCreate );
		return m_rcLastSktError;
	}

	// use setsockopt to request that the kernel join a multicast group
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr( m_MulticastGroupIp.c_str() );
	mreq.imr_interface.s_addr = htonl( INADDR_ANY );
	if( 0 > setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, ( char* )&mreq, sizeof( mreq ) ) )
	{
		m_rcLastSktError = VxGetLastError();
		LogMsg( LOG_ERROR, "VxSktUdp::udpOpenMulticast: join group %s error %s", m_MulticastGroupIp.c_str(), VxDescribeSktError( m_rcLastSktError ) );

		m_eSktCallbackReason = eSktCallbackReasonConnectError;
		if( m_pfnReceive )
		{
			m_pfnReceive( this, getRxCallbackUserData() );
		}

		closeSkt( eSktCloseUdpCreate );
		return m_rcLastSktError;
	}

	m_rcLastSktError = VxGetLastError();
	setAllowLoopback( true );
	m_IsMulticastSkt = true;
	if( 0 == m_rcLastSktError )
	{
		if( enableReceiveThread )
		{
			startReceive();
		}
	}
	else
	{
		LogMsg( LOG_ERROR, "VxSktUdp::udpOpenMulticast: join %s completed with error %s", m_MulticastGroupIp.c_str(), VxDescribeSktError( m_rcLastSktError ) );
	}

	return m_rcLastSktError;
}

//============================================================================
RCODE VxSktUdp::createSocket( InetAddress& oLclIp, uint16_t u16Port, struct addrinfo ** ppoResultAddr )	
{
	m_rcLastSktError = 0;
	if( isConnected() )
	{
		closeSkt(eSktCloseUdpCreate);
	}

	m_LclIp = oLclIp;
	m_strLclIp = m_LclIp.toStdString();

	m_LclIp.setPort( u16Port );
	m_RmtIp.setPort( u16Port );
	m_strRmtIp = "";
	m_eSktCallbackReason = eSktCallbackReasonConnecting;

	struct addrinfo * poResultAddr;
	struct addrinfo oHints;
	VxFillHints( oHints, true );

	char as8Port[16];
	sprintf( as8Port, "%d", u16Port);

	int err;
	if( 0 != ( err = getaddrinfo( NULL, as8Port, &oHints, &poResultAddr ) ) )
	{
		LogMsg( LOG_ERROR, "VxSktUdp::createSocket ERROR %d\n", err );
		return err;
	}

	* ppoResultAddr = poResultAddr;
	m_Socket = socket(poResultAddr->ai_family, poResultAddr->ai_socktype, poResultAddr->ai_protocol);
	if( INVALID_SOCKET == m_Socket )
	{
		// create socket error
		m_eSktCallbackReason = eSktCallbackReasonConnectError;
		m_rcLastSktError = VxGetLastError();
		LogMsg( LOG_ERROR, "VxSktBase::udpOpen: socket create error %s", VxDescribeSktError( m_rcLastSktError ) );
		if( m_pfnReceive )
		{
			m_pfnReceive( this, getRxCallbackUserData() );
		}

		return m_rcLastSktError;
	}
	else
	{
		u_int yes = 1;
		if( 0 > setsockopt( m_Socket, SOL_SOCKET, SO_REUSEADDR, ( char* )&yes, sizeof( yes ) ) )
		{
			LogMsg( LOG_ERROR, "VxSktBase::udpOpen: Reusing ADDR failed error %s", VxDescribeSktError( m_rcLastSktError ) );
		}
	}

#ifdef DEBUG_VXSKT_UDP
	LogMsg( LOG_INFO, "VxSktUdp::createSocket Success port %d skt handle %d ip %s\n", u16Port, m_Socket, m_strLclIp.c_str() );
#endif // DEBUG_VXSKT_UDP

	return m_rcLastSktError;
}

//============================================================================
void VxSktUdp::startReceive( void )	
{
	m_bIsConnected = true;
	if( m_pfnReceive )
	{
		// tell user we connected
		//vx_assert( m_pfnReceive );
		m_pfnReceive( this, getRxCallbackUserData() );

		// make a useful thread name
		std::string strThreadName;
		StdStringFormat( strThreadName, "VxSktBaseUDP%d", m_iSktId );
		startReceiveThread( strThreadName.c_str() );
	}
}

//============================================================================
//! send data to given ip 
RCODE  VxSktUdp::sendTo(	const char *	pData,		// data to send
							int				iDataLen,	// data len
							const char *	pRmtIp, 	// destination ip in dotted format
							uint16_t		u16Port )	// port to send to ( if 0 then port specified when opened )
{
	InetAddress oAddr( pRmtIp );
	return sendTo( pData, iDataLen, oAddr, u16Port );
}

//============================================================================
//! send data to given ip 
RCODE  VxSktUdp::sendToMulticast(	const char *	pData,				// data to send
									int				iDataLen,			// data len
									const char *	muliticastGroupIp, 	// destination multicast group ip in dotted format
									uint16_t		u16Port )			// port to send to ( if 0 then port specified when opened )
{
	InetAddress oAddr( muliticastGroupIp );
	// it does not seem necessary to join a group to send to it.. just the send has to be to the group address
	/*
	struct ip_mreq mreq = {};
	mreq.imr_multiaddr.s_addr = inet_addr( muliticastGroupIp );
	mreq.imr_interface.s_addr = htonl( INADDR_ANY );
	if( setsockopt( m_Socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,(char*)&mreq, sizeof(mreq)) < 0)
	{
		m_rcLastSktError = VxGetLastError();
		LogModule( eLogMulticast, LOG_ERROR, "VxSktUdp::sendToMulticast setsockopt mreq failed %s", VxDescribeSktError( m_rcLastSktError ) );
	}
	*/

	return sendTo( pData, iDataLen, oAddr, u16Port );
}

//============================================================================
//! send data to given ip 
RCODE VxSktUdp::sendTo(		const char *	pData,		// data to send
							int				iDataLen,	// data len
							InetAddress&	oRmtIp, 	// destination ip in host ordered u32
							uint16_t		u16Port )	// port to send to ( if 0 then port specified when opened )
{
	if(	( false == isConnected()) 
		|| ( m_pfnReceive && ( false == m_SktRxThread.isThreadCreated()) ) )
	{
		LogMsg( LOG_ERROR, "VxSktUdp::sendTo: NOT CONNECTED OR INVALID\n" );
		return -1;
	}

	if( 0 == u16Port )
	{
		u16Port = m_LclIp.getPort();
	}

	struct sockaddr_storage oSktAddr;
    oRmtIp.fillAddress( oSktAddr, u16Port );
	std::string ip = oRmtIp.toStdString();
#ifdef DEBUG_UDP
	LogMsg( LOG_INFO, "VxSktUdp::sendTo: ip %s port %d \n", ip.c_str(), u16Port );
#endif // DEBUG_UDP
	int structLen = oRmtIp.isIPv4() ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_storage);
	int iDataSentLen =  sendto( m_Socket, pData, iDataLen, 0, (struct sockaddr *)&oSktAddr, structLen );
	if( iDataSentLen !=  iDataLen )
	{
		m_rcLastSktError = VxGetLastError();
		if( 0 == m_rcLastSktError )
		{
			m_rcLastSktError = iDataSentLen;
			LogMsg( LOG_ERROR, "VxSktUdp::sendTo: Error 0.. assigning data sent as error %d\n", iDataSentLen );
		}
		else
		{
			LogMsg( LOG_ERROR, "VxSktUdp::sendTo: Error %s\n", VxDescribeSktError( m_rcLastSktError ) );
		}

		return m_rcLastSktError;
	}

	return 0;
}




