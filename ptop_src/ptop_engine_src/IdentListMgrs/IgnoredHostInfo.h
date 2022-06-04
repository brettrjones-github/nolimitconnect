#pragma once
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

#include <CoreLib/VxGUID.h>

class IgnoredHostInfo
{
public:
    IgnoredHostInfo() = default;
    IgnoredHostInfo( VxGUID& onlineId, VxGUID& thumbId, std::string hostUrl, std::string hostTitle, std::string hostDescription, int64_t timestampMs );

    IgnoredHostInfo( const IgnoredHostInfo& rhs );

    virtual ~IgnoredHostInfo() = default;

    IgnoredHostInfo& operator=( const IgnoredHostInfo& rhs );

    void                        setOnlineId( VxGUID& onlineId )         { m_OnlineId = onlineId; }
    VxGUID&                     getOnlineId()                           { return m_OnlineId; }
    void                        setThumbId( VxGUID& thumbId )           { m_ThumbId = thumbId; }
    VxGUID&                     getThumbId()                            { return m_ThumbId; }
    void                        setHostUrl( std::string& hostUrl )      { m_HostUrl = hostUrl; }
    std::string&                getHostUrl()                            { return m_HostUrl; }
    void                        setHostTitle( std::string& hosTitle )   { m_HostTitle = hosTitle; }
    std::string&                getHostTitle()                          { return m_HostTitle; }
    void                        setHostDescription( std::string& hosDescription ) { m_HostDescription = hosDescription; }
    std::string&                getHostDescription()                    { return m_HostDescription; }
    void                        setTimestampMs( int64_t timestampMs )   { m_TimestampMs = timestampMs; }
    int64_t                     getTimestampMs()                        { return m_TimestampMs; }

protected:
    VxGUID                      m_OnlineId;
    VxGUID                      m_ThumbId;
    std::string                 m_HostUrl{ "" };
    std::string                 m_HostTitle{ "" };
    std::string                 m_HostDescription{ "" };
    int64_t                     m_TimestampMs{ 0 };
};