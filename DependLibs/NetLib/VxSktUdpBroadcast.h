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
// http://www.nolimitconnect.org
//============================================================================

#include "VxSktUdp.h"

class VxSktUdpBroadcast : public VxSktUdp
{
public:
	
	VxSktUdpBroadcast();
	
	virtual ~VxSktUdpBroadcast();

	//! startup broadcast socket.. if pBroadcastIp is null
	//! then will broadcast to 255.255.255.255 
	virtual RCODE udpBroadcastOpen(	InetAddress& oIpAddr,
									uint16_t u16Port = 12345,			// port to listen on
									const char * pBroadcastIp = "255.255.255.255" ); // broadcast to address
	//virtual RCODE udpBroadcastOpen(	InetAddress& oIpAddr,
	//								uint16_t u16Port = 12345,			// port to listen on
	//								const char * pBroadcastIp = "192.168.60.21"  ); // broadcast to address

	//! broadcast data 
	virtual RCODE broadcastData(	const char *	pData,			// data to send
									int				iDataLen,		// data length
									uint16_t				u16Port = 0 );	// port to send to ( if 0 then port specified when opened )

	
	std::string			m_strBroadcastIp;	// ip to broadcast to in dotted form
    InetAddress			m_BroadcastIp;     // ip to broadcast to in host ordered binary form
};



