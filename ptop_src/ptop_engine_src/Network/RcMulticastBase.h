#pragma once
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

#include <NetLib/VxSktUdp.h>

class NetworkMgr;
class P2PEngine;

class RcMulticastBase
{
public:
	RcMulticastBase( NetworkMgr& networkMgr );

	VxSktUdp&					getUdpSkt( void )							{ return m_SktUdp; }
	void						setLocalIp( InetAddress& newLocalIp );
	void						setMulticastPort( uint16_t u16Port );
	void 						setMulticastIp( const char * multicastIpAddr );

	void						setMulticastKey( const char * networkName );

protected:

	NetworkMgr&					m_NetworkMgr;
	P2PEngine&					m_Engine;
	VxSktUdp					m_SktUdp;
	InetAddress					m_LclIp;
	uint16_t					m_u16MulticastPort;
	std::string					m_strMulticastIp;
	VxMutex						m_MulticastMutex;
};


