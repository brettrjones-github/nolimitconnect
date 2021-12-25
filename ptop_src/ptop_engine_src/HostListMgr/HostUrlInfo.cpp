//============================================================================
// Copyright (C) 2021 Brett R. Jones 
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

#include "HostUrlInfo.h"

//============================================================================
HostUrlInfo::HostUrlInfo( EHostType hostType, VxGUID& onlineId, std::string& hostUrl, int64_t timestamp )
    : m_HostType( hostType )
    , m_OnlineId( onlineId )
    , m_HostUrl( hostUrl )
    , m_TimestampMs( timestamp )
{
}

//============================================================================
HostUrlInfo::HostUrlInfo( const HostUrlInfo& rhs )
    : m_HostType( rhs.m_HostType )
    , m_OnlineId( rhs.m_OnlineId )
    , m_HostUrl( rhs.m_HostUrl )
    , m_TimestampMs( rhs.m_TimestampMs )
{
}

//============================================================================
HostUrlInfo& HostUrlInfo::operator=( const HostUrlInfo& rhs ) 
{	
	if( this != &rhs )
	{
        m_HostType = rhs.m_HostType;
        m_OnlineId = rhs.m_OnlineId;
        m_HostUrl = rhs.m_HostUrl;
        m_TimestampMs = rhs.m_TimestampMs;
    }

	return *this;
}
