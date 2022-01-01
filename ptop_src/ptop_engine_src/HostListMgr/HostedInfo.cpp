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

#include "HostedInfo.h"

//============================================================================
HostedInfo::HostedInfo( EHostType hostType, VxGUID& onlineId, std::string& hostUrl )
    : m_HostType( hostType )
    , m_OnlineId( onlineId )
    , m_HostUrl( hostUrl )
{
}

//============================================================================
HostedInfo::HostedInfo( const HostedInfo& rhs )
    : m_HostType( rhs.m_HostType )
    , m_OnlineId( rhs.m_OnlineId )
    , m_ConnectedTimestampMs( rhs.m_ConnectedTimestampMs )
    , m_JoinedTimestampMs( rhs.m_JoinedTimestampMs )
    , m_HostInfoTimestampMs( rhs.m_HostInfoTimestampMs )
    , m_HostUrl( rhs.m_HostUrl )
    , m_HostTitle( rhs.m_HostTitle )
    , m_HostDesc( rhs.m_HostDesc )
{
}

//============================================================================
HostedInfo& HostedInfo::operator=( const HostedInfo& rhs ) 
{	
	if( this != &rhs )
	{
        m_HostType = rhs.m_HostType;
        m_OnlineId = rhs.m_OnlineId;
        m_ConnectedTimestampMs = rhs.m_ConnectedTimestampMs;
        m_JoinedTimestampMs = rhs.m_JoinedTimestampMs;
        m_HostInfoTimestampMs = rhs.m_HostInfoTimestampMs;
        m_HostUrl = rhs.m_HostUrl;
        m_HostTitle = rhs.m_HostTitle;
        m_HostDesc = rhs.m_HostDesc;
    }

	return *this;
}
