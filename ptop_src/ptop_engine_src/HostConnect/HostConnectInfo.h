#pragma once
//============================================================================
// Copyright (C) 2020 Brett R. Jones 
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

//ePluginTypeHostGroup = 13,	//!< group host
//ePluginTypeHostNetwork = 14,	//!< master network host
//ePluginTypeServiceConnectTest = 15,	//!< Connection Test Service

//ePluginTypeClientConnectTest = 16,	//!< Connection Test Client
//ePluginTypeHostGroupListing = 17,	//!< group list host
//ePluginTypeRandomConnect = 18,	//!< Random connect to another person ( Using Phone Shake )
//ePluginTypeRandomConnectRelay = 19,	//!< Random connect to another person relay service
//ePluginTypeRelay = 20,	//!< Relay services plugin

#include <GuiInterface/IDefs.h>
#include <ptop_src/ptop_engine_src/HostConnect/HostConnectInterface.h>

#include <CoreLib/VxGUID.h>
#include <vector>

class RcConnectInfo;

class HostConnectInfo
{
public:
    HostConnectInfo() = default;
    virtual ~HostConnectInfo() = default;

    void                        setHostInfo( EPluginType pluginType, std::string ipAddr, uint16_t port, VxGUID onlineId = VxGUID::nullVxGUID(), const char * url = nullptr );
    RcConnectInfo*              getConnectInfo( void )        { return m_RcConnectInfo; }
 
    bool                        hasHostService( EPluginType pluginType );
    bool                        hasHostService( EHostConnectType connectType );
    void                        addHostService( EPluginType pluginType );
    void                        removeHostService( EPluginType pluginType );

    virtual void                onEngineContactConnected( RcConnectInfo * poInfo, bool connectionListLocked );
    virtual void                onEngineContactDisconnected( RcConnectInfo * poInfo, bool connectionListLocked );

    bool                        isConnected( EHostConnectType connectType );
    bool                        isConnected( EPluginType pluginType );


    void                        setHostOnlineId( VxGUID& onlineId )     { m_OnlineId = onlineId; }
    VxGUID&                     getHostOnlineId( void )                 { return m_OnlineId; }
    void                        setHostIpAddr( std::string& ipAddr )    { m_IpAddr = ipAddr; }
    std::string&                getHostIpAddr( void )                   { return m_IpAddr; }
    void                        setHostPort( uint16_t port )            { m_Port = port; }
    uint16_t                    getHostPort( void )                     { return m_Port; }
    void                        setHostUrl( const char * url )          { m_HostUrl = url ? url : ""; }
    std::string&                getHostUrl( void )                      { return m_HostUrl; }

    static EPluginType          connectTypeToPluginType( EHostConnectType connectType );

protected:
    VxGUID                      m_OnlineId;
    std::string                 m_IpAddr;
    uint16_t                    m_Port{ 0 };
    std::string                 m_HostUrl{ "" };
    std::vector<EPluginType>    m_PluginServiceList;
    RcConnectInfo*              m_RcConnectInfo{ nullptr };

};