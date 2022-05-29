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
// http://www.nolimitconnect.org
//============================================================================

#include "NetSettings.h"

//============================================================================
NetSettings& NetSettings::operator =( const NetSettings& rhs )
{
	if( this != &rhs )
	{
        *( (NetHostSetting*)this ) = rhs;
        m_u16MyMulticastPort        = rhs.m_u16MyMulticastPort;
		m_bMulticastEnable			= rhs.m_bMulticastEnable;
        m_AllowMulticastBroadcast   = rhs.m_AllowMulticastBroadcast;
        m_UserRelayPermissionCount = rhs.m_UserRelayPermissionCount;
        m_SystemRelayPermissionCount = rhs.m_SystemRelayPermissionCount;
        m_AllowUserLocation         = rhs.m_AllowUserLocation;
	}

	return *this;
}
