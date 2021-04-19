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

#include <GoTvCore/GoTvP2P/BaseInfo/BaseInfo.h>
#include <GuiInterface/IDefs.h>

#define HOST_FLAG_DEFAULT_HOST			0x0001
#define HOST_FLAG_IS_TEMP			    0x0002

class HostJoinInfo : public BaseInfo
{
public:
	HostJoinInfo();
	HostJoinInfo( const HostJoinInfo& rhs );

	HostJoinInfo&				operator=( const HostJoinInfo& rhs ); 

    bool                        isValid( void ) { return true;  }

    virtual void			    setPluginType( EPluginType pluginType )             { m_PluginType = pluginType; }
    virtual EPluginType			getPluginType( void )                               { return m_PluginType; }

    virtual void			    setHostType( EHostType hostType )                   { m_HostType = hostType; }
    virtual EHostType			getHostType( void )                                 { return m_HostType; }

    virtual void			    setHostFlags( uint32_t hostFlags )                  { m_HostFlags = hostFlags; }
    virtual uint32_t			getHostFlags( void )                                { return m_HostFlags; }

    void						setIsDefaultHost( bool isDefault )	                { if( isDefault ) m_HostFlags |= HOST_FLAG_DEFAULT_HOST; else m_HostFlags &= ~HOST_FLAG_DEFAULT_HOST; }
    bool						isDefaultHost( void )				                { return m_HostFlags & HOST_FLAG_DEFAULT_HOST ? true : false; }
    void						setIsTemp( bool isTemp )	                        { if( isTemp ) m_HostFlags |= HOST_FLAG_IS_TEMP; else m_HostFlags &= ~HOST_FLAG_IS_TEMP; }
    bool						isTemp( void )				                        { return m_HostFlags & HOST_FLAG_IS_TEMP ? true : false; }

    virtual void			    setHostUrl( std::string hostUrl )                   { m_HostUrl = hostUrl; }
    virtual std::string&	    getHostUrl( void )                                  { return m_HostUrl; }

    virtual void				setLastConnectTime( uint64_t timestamp )            { m_LastConnectMs = timestamp; }
    virtual uint64_t			getLastConnectTime( void )                          { return m_LastConnectMs; }

    virtual void				setLastJoinTime( uint64_t timestamp )               { m_LastJoinMs = timestamp; }
    virtual uint64_t			getLastJoinTime( void )                             { return m_LastJoinMs; }

    // temporaries
    virtual void				setConnectionId( VxGUID& connectionId )             { m_ConnectionId = connectionId; }
    virtual VxGUID&				getConnectionId( void )                             { return m_ConnectionId; }

protected:
	//=== vars ===//
    EPluginType                 m_PluginType{ ePluginTypeInvalid };
    EHostType                   m_HostType{ eHostTypeUnknown };
    uint32_t                    m_HostFlags{ 0 };
    std::string                 m_HostUrl{ "" };
    int64_t						m_LastConnectMs{ 0 };
    int64_t						m_LastJoinMs{ 0 };

    // temporaries
    VxGUID                      m_ConnectionId;
};
