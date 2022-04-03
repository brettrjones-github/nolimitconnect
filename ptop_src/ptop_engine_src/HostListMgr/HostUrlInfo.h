#pragma once
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

#include <GuiInterface/IDefs.h>

#include <CoreLib/VxGUID.h>

#include <string>

class HostUrlInfo
{
public:
	HostUrlInfo() = default;
	HostUrlInfo( const HostUrlInfo& rhs );
    HostUrlInfo( EHostType hostType, VxGUID& onlineId, std::string& hostUrl, int64_t timestamp = 0 );

	HostUrlInfo&				operator=( const HostUrlInfo& rhs ); 

    virtual void				setOnlineId( VxGUID& onlineId )                     { m_OnlineId = onlineId; }
    virtual VxGUID&             getOnlineId( void )                                 { return m_OnlineId; }

    virtual void			    setHostType( enum EHostType friendshipToHim )       { m_HostType = friendshipToHim; }
    virtual EHostType	        getHostType( void )                                 { return m_HostType; }

    virtual void			    setHostUrl( std::string hostUrl )                   { m_HostUrl = hostUrl; }
    virtual std::string&	    getHostUrl( void )                                  { return m_HostUrl; }

    virtual void			    setTimestamp( int64_t timestampMs )                 { m_TimestampMs = timestampMs; }
    virtual int64_t             getTimestamp( void )                                { return m_TimestampMs; }

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    VxGUID                      m_OnlineId;
    std::string                 m_HostUrl{ "" };
    int64_t                     m_TimestampMs{ 0 };
};
