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

#include <CoreLib/VxDefs.h>

#include <string>

#define NET_DEFAULT_NETWORK_NAME			"NoLimitNet"
#define NET_DEFAULT_NET_HOST_URL			"ptop://www.nolimitconnect.net:45124"
#define NET_DEFAULT_NETSERVICE_URL			"ptop://www.nolimitconnect.net:45124" // for connection test.. for now same as Network Host but could be seperated to spread network load
#define NET_DEFAULT_NETSERVICE_PORT			45124

class NetHostSetting
{
public:
	NetHostSetting();
	virtual ~NetHostSetting() = default;

	NetHostSetting&				operator =( const NetHostSetting& rhs );
    bool						operator == ( const NetHostSetting& rhs ) const;
    bool						operator != ( const NetHostSetting& rhs ) const;

	void						setNetHostSettingName( const char * settingName )			{ m_NetHostSettingName = settingName; }
	std::string&				getNetHostSettingName( void )								{ return m_NetHostSettingName; }

	void						setNetworkKey( const char * networkName )					{ m_NetworkName = networkName; }
	std::string&				getNetworkKey( void )										{ return m_NetworkName; }

	void						setNetworkHostUrl( const char * anchorUrl )				{ m_NetConnectTestUrl = anchorUrl; }
	std::string&				getNetworkHostUrl( void )								{ return m_NetConnectTestUrl; }
    void						setConnectTestUrl( const char * netServiceUrl )		{ m_NetworkHostUrl = netServiceUrl; }
    std::string&				getConnectTestUrl( void )								{ return m_NetworkHostUrl; }
    
	void						setIsThisNodeAnNetHostOld( bool isHost )						{ m_ThisNodeIsHost = isHost; }// TODO remove
	bool						getIsThisNodeAnNetHostOld( void )								{ return m_ThisNodeIsHost; }// TODO remove

	void						setExcludeMeFromNetHostList( bool excludeMe )				{ m_ExcludeMeFromHostList = excludeMe; }
	bool						getExcludeMeFromNetHostList( void )							{ return m_ExcludeMeFromHostList; }

protected:
	//=== vars ===//
	std::string					m_NetHostSettingName;
	std::string					m_NetworkName;

	std::string					m_NetworkHostUrl;
	std::string					m_NetConnectTestUrl;
    bool						m_ThisNodeIsHost{ false };  // TODO remove
    bool						m_ExcludeMeFromHostList{ false };
};
