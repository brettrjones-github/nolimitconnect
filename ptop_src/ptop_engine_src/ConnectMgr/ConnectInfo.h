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
// http://www.nolimitconnect.org
//============================================================================

#include <ptop_src/ptop_engine_src/BaseInfo/BaseHostInfo.h>

#include <CoreLib/VxPtopUrl.h>

#define HOST_FLAG_DEFAULT_HOST			0x0001
#define HOST_FLAG_IS_TEMP			    0x0002

class ConnectInfo : public BaseHostInfo
{
public:
	ConnectInfo();
	ConnectInfo( const ConnectInfo& rhs );

	ConnectInfo&				operator=( const ConnectInfo& rhs ); 

    bool                        isValid( void ) { return true;  }

    virtual void			    setHostFlags( uint32_t hostFlags )                  { m_HostFlags = hostFlags; }
    virtual uint32_t			getHostFlags( void )                                { return m_HostFlags; }

    virtual void				setOfferId( VxGUID& offerId )                       { m_OfferId = offerId; }
    virtual void				setOfferId( const char * guid )                     { m_OfferId.fromVxGUIDHexString( guid ); }
    virtual VxGUID&				getOfferId( void )                                  { return m_OfferId; }
    virtual void			    setOfferState( enum EOfferState offerState )        { m_OfferState = offerState; }
    virtual EOfferState	        getOfferState( void )                               { return m_OfferState; }
    virtual void			    setOfferModifiedTime( uint64_t timeMs )             { m_OfferModMs = timeMs; }
    virtual uint64_t			getOfferModifiedTime( void )                        { return m_OfferModMs; }

    virtual void			    setHostUrl( std::string hostUrl )                   { m_HostUrl.setUrl(hostUrl ); }
    virtual void			    setHostUrl( VxPtopUrl& hostUrl )                    { m_HostUrl = hostUrl; }
    virtual VxPtopUrl&	        getHostUrl( void )                                  { return m_HostUrl; }

    void						setIsDefaultHost( bool isDefault )	                { if( isDefault ) m_HostFlags |= HOST_FLAG_DEFAULT_HOST; else m_HostFlags &= ~HOST_FLAG_DEFAULT_HOST; }
    bool						isDefaultHost( void )				                { return m_HostFlags & HOST_FLAG_DEFAULT_HOST ? true : false; }
    void						setIsTemp( bool isTemp )	                        { if( isTemp ) m_HostFlags |= HOST_FLAG_DEFAULT_HOST; else m_HostFlags &= ~HOST_FLAG_DEFAULT_HOST; }
    bool						isTemp( void )				                        { return m_HostFlags & HOST_FLAG_DEFAULT_HOST ? true : false; }

    // temporaries
    virtual void				setConnectionId( VxGUID& connectionId )             { m_ConnectionId = connectionId; }
    virtual VxGUID&				getConnectionId( void )                             { return m_ConnectionId; }

    virtual void			    setOnlineState( enum EOnlineState onlineState )     { m_OnlineState = onlineState; }
    virtual EOnlineState	    getOnlineState( void )                              { return m_OnlineState; }

protected:
	//=== vars ===//
    EHostType                   m_HostType;
    VxGUID                      m_HostOnlineId;
    uint64_t			        m_HostModMs{ 0 };
    uint32_t                    m_HostFlags{ 0 };
    VxGUID                      m_ThumbId;
    uint64_t			        m_ThumbModMs{ 0 };
    VxGUID                      m_OfferId;
    EOfferState                 m_OfferState{ eOfferStateNone };
    uint64_t			        m_OfferModMs{ 0 };
    VxPtopUrl                   m_HostUrl;

    // temporaries
    VxGUID                      m_ConnectionId;
    EOnlineState                m_OnlineState{ eOnlineStateUnknown };

};
