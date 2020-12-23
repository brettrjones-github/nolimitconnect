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
#define NET_DEFAULT_NET_HOST_URL			"ptop://nolimitconnect.net:45124"
#define NET_DEFAULT_CONNECT_TEST_URL		"ptop://gotvptop.net:45124" // for connection test.. for now same as Network Host but could be seperated to spread network load
#define NET_DEFAULT_RANDOM_CONNECT_URL		"ptop://gotvptop.com:45124" // for random connect.. for now same as Network Host but could be seperated to spread network load
#define NET_DEFAULT_GROUP_HOST_URL		    "ptop://myp2pnetwork.com:45124" // for default group host.. for now same as Network Host but could be seperated to spread network load
#define NET_DEFAULT_CHAT_ROOM_HOST_URL		"ptop://myp2pconnect.com:45124" // for default chat room host.. for now same as Network Host but could be seperated to spread network load
#define NET_DEFAULT_NETSERVICE_PORT			45124

class NetHostSetting
{
public:
	NetHostSetting() = default;
	virtual ~NetHostSetting() = default;

	NetHostSetting&				operator =( const NetHostSetting& rhs );
    bool						operator == ( const NetHostSetting& rhs ) const;
    bool						operator != ( const NetHostSetting& rhs ) const;

	void						setNetHostSettingName( const char * settingName )			{ m_NetHostSettingName = settingName; }
	std::string&				getNetHostSettingName( void )								{ return m_NetHostSettingName; }

	void						setNetworkKey( const char * networkName )					{ m_NetworkName = networkName; }
	std::string&				getNetworkKey( void )										{ return m_NetworkName; }

	void						setNetworkHostUrl( const char * anchorUrl )				    { m_NetworkHostUrl = anchorUrl; }
	std::string&				getNetworkHostUrl( void )								    { return m_NetworkHostUrl; }
    void						setConnectTestUrl( const char * netServiceUrl )		        { m_NetConnectTestUrl = netServiceUrl; }
    std::string&				getConnectTestUrl( void )								    { return m_NetConnectTestUrl; }
    void						setRandomConnectUrl( const char * netServiceUrl )		    { m_NetRandomConnectUrl = netServiceUrl; }
    std::string&				getRandomConnectUrl( void )								    { return m_NetRandomConnectUrl; }
    void						setGroupHostUrl( const char * netServiceUrl )		        { m_GroupHostUrl = netServiceUrl; }
    std::string&				getGroupHostUrl( void )								        { return m_GroupHostUrl; }
    void						setChatRoomHostUrl( const char * netServiceUrl )		    { m_ChatRoomHostUrl = netServiceUrl; }
    std::string&				getChatRoomHostUrl( void )								    { return m_ChatRoomHostUrl; }
    void						setExternIpAddr( const char * ipAddr )		                { m_ExternIpAddr = ipAddr; }
    std::string&				getExternIpAddr( void )								        { return m_ExternIpAddr; }

    void                        setConnectTestType( int32_t testType )		                { m_ConnectTestType = testType; }
    int32_t                     getConnectTestType( void )								    { return m_ConnectTestType; }

    void						setUseUpnp( bool excludeMe )				                { m_UseUpnp = excludeMe; }
    bool						getUseUpnp( void )							                { return m_UseUpnp; }

	void						setExcludeMeFromNetHostList( bool excludeMe )				{ m_ExcludeMeFromHostList = excludeMe; }
	bool						getExcludeMeFromNetHostList( void )							{ return m_ExcludeMeFromHostList; }

protected:
	//=== vars ===//
    std::string					m_NetHostSettingName{ "default" };
    std::string					m_NetworkName{ NET_DEFAULT_NETWORK_NAME };

    std::string					m_NetworkHostUrl{ NET_DEFAULT_NET_HOST_URL };
	std::string					m_NetConnectTestUrl{ NET_DEFAULT_CONNECT_TEST_URL };
    std::string					m_NetRandomConnectUrl{ NET_DEFAULT_RANDOM_CONNECT_URL };
    std::string					m_GroupHostUrl{ NET_DEFAULT_GROUP_HOST_URL };
    std::string					m_ChatRoomHostUrl{ NET_DEFAULT_GROUP_HOST_URL };
    std::string					m_ExternIpAddr{ "" };

    int32_t                     m_ConnectTestType{ 0 };
    bool						m_UseUpnp{ false };
    bool						m_ExcludeMeFromHostList{ false };
};
