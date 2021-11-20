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

#include "RcMulticastBase.h"
#include "NetworkMgr.h"

#include <string.h>

namespace
{
	const char *	RC_DEFAULT_MULTICAST_ADDR	= "224.255.255.38";
	uint16_t		RC_DEFAULT_MULTICAST_PORT	= 45124;
}

//============================================================================
RcMulticastBase::RcMulticastBase( NetworkMgr& networkMgr )
: m_NetworkMgr( networkMgr )
, m_Engine( networkMgr.getEngine() )
, m_SktUdp()
, m_LclIp()
, m_u16MulticastPort( RC_DEFAULT_MULTICAST_PORT )
, m_strMulticastIp( RC_DEFAULT_MULTICAST_ADDR )
, m_MulticastMutex()
{
}

//============================================================================
void RcMulticastBase::setLocalIp( InetAddress& newLocalIp )
{
	m_LclIp = newLocalIp;
}

//============================================================================
void RcMulticastBase::setMulticastPort( uint16_t u16Port )
{
	m_u16MulticastPort = u16Port;
}

//============================================================================
void RcMulticastBase::setMulticastIp( const char * multicastIpAddr )
{
	m_strMulticastIp = multicastIpAddr;
}

//============================================================================
void RcMulticastBase::setMulticastKey( const char * networkName )
{
	m_SktUdp.m_TxCrypto.setPassword( networkName, ( int )strlen(networkName ));
	m_SktUdp.m_RxCrypto.setPassword( networkName, ( int )strlen(networkName ));
}
