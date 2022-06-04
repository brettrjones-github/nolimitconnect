//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include "IgnoredHostInfo.h"

IgnoredHostInfo::IgnoredHostInfo( VxGUID& onlineId, VxGUID& thumbId, std::string hostUrl, std::string hostTitle, std::string hostDescription, int64_t timestampMs )
    : m_OnlineId( onlineId )
    , m_ThumbId( thumbId )
    , m_HostUrl( hostUrl )
    , m_HostTitle( hostTitle )
    , m_HostDescription( hostDescription )
    , m_TimestampMs( timestampMs )
{
}

IgnoredHostInfo::IgnoredHostInfo( const IgnoredHostInfo& rhs )
    : m_OnlineId( rhs.m_OnlineId )
    , m_ThumbId( rhs.m_ThumbId )
    , m_HostUrl( rhs.m_HostUrl )
    , m_HostTitle( rhs.m_HostTitle )
    , m_HostDescription( rhs.m_HostDescription )
    , m_TimestampMs( rhs.m_TimestampMs )
{
}

IgnoredHostInfo& IgnoredHostInfo::operator=( const IgnoredHostInfo & rhs )
{
    m_OnlineId = rhs.m_OnlineId;
    m_ThumbId = rhs.m_ThumbId;
    m_HostUrl = rhs.m_HostUrl;
    m_HostTitle = rhs.m_HostTitle;
    m_HostDescription = rhs.m_HostDescription;
    m_TimestampMs = rhs.m_TimestampMs;
    return *this;
}

