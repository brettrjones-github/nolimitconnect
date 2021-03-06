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

#include <ptop_src/ptop_engine_src/BaseInfo/BaseJoinInfo.h>
#include <GuiInterface/IDefs.h>

#define HOST_FLAG_DEFAULT_HOST			0x0001
#define HOST_FLAG_IS_TEMP			    0x0002

class HostJoinInfo : public BaseJoinInfo
{
public:
	HostJoinInfo();
	HostJoinInfo( const HostJoinInfo& rhs );

	HostJoinInfo&				operator=( const HostJoinInfo& rhs ); 

    bool                        isValid( void ) { return true;  }

    virtual void			    setHostFlags( uint32_t hostFlags )                  { m_HostFlags = hostFlags; }
    virtual uint32_t			getHostFlags( void )                                { return m_HostFlags; }

    void						setIsDefaultHost( bool isDefault )	                { if( isDefault ) m_HostFlags |= HOST_FLAG_DEFAULT_HOST; else m_HostFlags &= ~HOST_FLAG_DEFAULT_HOST; }
    bool						isDefaultHost( void )				                { return m_HostFlags & HOST_FLAG_DEFAULT_HOST ? true : false; }
    void						setIsTemp( bool isTemp )	                        { if( isTemp ) m_HostFlags |= HOST_FLAG_IS_TEMP; else m_HostFlags &= ~HOST_FLAG_IS_TEMP; }
    bool						isTemp( void )				                        { return m_HostFlags & HOST_FLAG_IS_TEMP ? true : false; }

    virtual void			    setUserUrl( std::string userUrl )                   { m_UserUrl = userUrl; }
    virtual std::string&	    getUserUrl( void )                                  { return m_UserUrl; }
    virtual void			    setFriendState( EFriendState friendshipToHim )      { m_FriendState = friendshipToHim; }
    virtual EFriendState	    getFriendState( void )                              { return m_FriendState; }

    // temporaries
    virtual void				setConnectionId( VxGUID& connectionId )             { m_ConnectionId = connectionId; }
    virtual VxGUID&				getConnectionId( void )                             { return m_ConnectionId; }
    virtual void				setSessionId( VxGUID& sessionId )                   { m_SessionId = sessionId; }
    virtual VxGUID&				getSessionId( void )                                { return m_SessionId; }

protected:
	//=== vars ===//
    EFriendState                m_FriendState{ eFriendStateIgnore };
    uint32_t                    m_HostFlags{ 0 };
    std::string                 m_UserUrl{ "" };

    // temporaries
    VxGUID                      m_ConnectionId;
    VxGUID                      m_SessionId;
};
