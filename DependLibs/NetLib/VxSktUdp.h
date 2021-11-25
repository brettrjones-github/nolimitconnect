#pragma once
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

#include "InetAddress.h"
#include "VxSktBase.h"

class VxSktUdp : public VxSktBase
{
public:
	VxSktUdp();	
	virtual ~VxSktUdp() = default;

	//! open a udp port
	virtual long				udpOpen( InetAddress& oLclIp, uint16_t u16Port = 54321, bool enableReceiveThread = true );
	virtual long				udpOpenBroadcast( std::string broadcastIp, uint16_t u16Port, bool enableReceiveThread = true );
	virtual long				udpOpenMulticast( std::string multicastGroupIp, uint16_t u16Port, bool enableReceiveThread = true );
	virtual long				udpOpenUnicast( InetAddress& oLclIp, uint16_t u16Port );

	//! send data to given ip 
	virtual long				sendTo(	const char *	pData,			// data to send
										int				iDataLen,		// data length
										InetAddress&	u32RmpIp,		// destination ip
										uint16_t		u16Port = 0 );	// port to send to ( if 0 then port specified when opened )

	//! send data to given ip 
	virtual long				sendTo(	const char *	pData,			// data to send
										int				iDataLen,		// data length
										const char *	pRmpIp,			// destination ip in dotted format
										uint16_t		u16Port = 0 );	// port to send to ( if 0 then port specified when opened )

	//! send data to given ip 
	virtual long				sendToMulticast(	const char *	pData,				// data to send
													int				iDataLen,			// data length
													const char *	muliticastGroupIp,	// destination multicast group ip in dotted format
													uint16_t		u16Port = 0 );		// port to send to ( if 0 then port specified when opened )
protected:
	long						createSocket( InetAddress& oLclIp, uint16_t u16Port, struct addrinfo ** ppoResultAddr );
	void						startReceive( void );
};



