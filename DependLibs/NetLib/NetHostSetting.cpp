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

#include "NetHostSetting.h"

//============================================================================
NetHostSetting::NetHostSetting()
: m_NetHostSettingName("")
, m_NetworkName( NET_DEFAULT_NETWORK_NAME )
, m_NetServiceWebsiteUrl( NET_DEFAULT_NETSERVICE_URL )
, m_NetHostWebsiteUrl( NET_DEFAULT_NET_HOST_URL )
{
}

//============================================================================
NetHostSetting& NetHostSetting::operator =( const NetHostSetting& rhs )
{
	if( this != &rhs )
	{
		m_NetHostSettingName		= rhs.m_NetHostSettingName;
		m_NetworkName				= rhs.m_NetworkName;
		m_NetServiceWebsiteUrl		= rhs.m_NetServiceWebsiteUrl;
        m_NetHostWebsiteUrl         = rhs.m_NetHostWebsiteUrl;
		m_ThisNodeIsHost			= rhs.m_ThisNodeIsHost;
		m_ExcludeMeFromHostList	    = rhs.m_ExcludeMeFromHostList;
	}

	return *this;
}

//============================================================================
bool NetHostSetting::operator == ( const NetHostSetting& rhs ) const
{
    return 	m_NetHostSettingName == rhs.m_NetHostSettingName &&
        m_NetworkName == rhs.m_NetworkName &&
        m_NetServiceWebsiteUrl == rhs.m_NetServiceWebsiteUrl &&
        m_NetHostWebsiteUrl == rhs.m_NetHostWebsiteUrl &&
        m_ThisNodeIsHost == rhs.m_ThisNodeIsHost &&
        m_ExcludeMeFromHostList == rhs.m_ExcludeMeFromHostList;
}

//============================================================================
bool NetHostSetting::operator != ( const NetHostSetting& rhs ) const
{
    return !( *this == rhs );
}