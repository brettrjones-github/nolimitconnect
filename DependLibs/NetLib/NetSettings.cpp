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

#include "NetSettings.h"

namespace
{
	const uint16_t		NET_DEFAULT_TCP_PORT					= 45123;
	const uint16_t		NET_DEFAULT_UDP_PORT					= 45124;
}

//============================================================================
NetSettings::NetSettings()
: m_u16MyTcpInPort( NET_DEFAULT_TCP_PORT )
, m_u16MyMulticastPort( NET_DEFAULT_UDP_PORT )
, m_bMulticastEnable( true )
, m_bAttemptPortForward( true )
{
}

//============================================================================
NetSettings& NetSettings::operator =( const NetSettings& rhs )
{
	if( this != &rhs )
	{
		m_NetworkName				= rhs.m_NetworkName;
        m_NetConnectTestUrl         = rhs.m_NetConnectTestUrl;
        m_u16MyMulticastPort        = rhs.m_u16MyMulticastPort;
		m_u16MyTcpInPort			= rhs.m_u16MyTcpInPort;
        m_bAttemptPortForward       = rhs.m_bAttemptPortForward;
		m_bMulticastEnable			= rhs.m_bMulticastEnable;
        m_AllowMulticastBroadcast   = rhs.m_AllowMulticastBroadcast;
		m_bAttemptPortForward		= rhs.m_bAttemptPortForward;
        m_UserRelayPermissionCount = rhs.m_UserRelayPermissionCount;
        m_SystemRelayPermissionCount = rhs.m_SystemRelayPermissionCount;
        m_AllowUserLocation         = rhs.m_AllowUserLocation;
	}

	return *this;
}
