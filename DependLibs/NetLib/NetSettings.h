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
// http://www.nolimitconnect.org
//============================================================================

#include "NetHostSetting.h"

class NetSettings : public NetHostSetting
{
public:


	NetSettings() = default;
	virtual ~NetSettings() = default;

	NetSettings&				operator =( const NetSettings& rhs );

	void						setMyMulticastPort( uint16_t port )							{ m_u16MyMulticastPort = port; }
	uint16_t					getMyMulticastPort( void )									{ return m_u16MyMulticastPort; }

	void						setUserRelayPermissionCount( int count )					{ m_UserRelayPermissionCount = count; }
	uint16_t					getUserRelayPermissionCount( void )							{ return m_UserRelayPermissionCount; }
	void						setSystemRelayPermissionCount( int count )					{ m_SystemRelayPermissionCount = count; }
	uint16_t					getSystemRelayPermissionCount( void )						{ return m_SystemRelayPermissionCount; }

	void						setAllowUserLocation( bool enable )							{ m_AllowUserLocation = enable; }
	bool						getAllowUserLocation( void )								{ return m_AllowUserLocation; }
	void						setMulticastEnable( bool enable )							{ m_bMulticastEnable = enable; }
	bool						getMulticastEnable( void )									{ return m_bMulticastEnable; }
	void						setAllowMulticastBroadcast( bool enable )					{ m_AllowMulticastBroadcast = enable; }
	bool						getAllowMulticastBroadcast( void )							{ return m_AllowMulticastBroadcast; }

protected:
	//=== vars ===//
    uint16_t					m_u16MyMulticastPort{ NET_DEFAULT_UDP_PORT };
	bool						m_bMulticastEnable{ false };
	int							m_UserRelayPermissionCount{ 0 };
	int							m_SystemRelayPermissionCount{ 0 };
	bool						m_AllowUserLocation{ false };
	bool						m_AllowMulticastBroadcast{ false };
};


