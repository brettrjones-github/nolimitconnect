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

class HostedInfo
{
public:
	HostedInfo() = default;
	HostedInfo( const HostedInfo& rhs );
    HostedInfo( EHostType hostType, VxGUID& onlineId, std::string& hostUrl, int64_t timestamp = 0 );

	HostedInfo&				operator=( const HostedInfo& rhs ); 

    virtual void				setOnlineId( VxGUID& onlineId )                     { m_OnlineId = onlineId; }
    virtual VxGUID&             getOnlineId( void )                                 { return m_OnlineId; }

    virtual void			    setHostType( EHostType friendshipToHim )            { m_HostType = friendshipToHim; }
    virtual EHostType	        getHostType( void )                                 { return m_HostType; }

    virtual void			    setHosted( std::string hostUrl )                   { m_Hosted = hostUrl; }
    virtual std::string&	    getHosted( void )                                  { return m_Hosted; }

    virtual void			    setTimestamp( int64_t timestampMs )                 { m_TimestampMs = timestampMs; }
    virtual int64_t             getTimestamp( void )                                { return m_TimestampMs; }

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    VxGUID                      m_OnlineId;
    std::string                 m_Hosted{ "" };
    int64_t                     m_TimestampMs{ 0 };
};
