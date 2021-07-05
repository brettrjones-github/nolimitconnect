//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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
#pragma once

#include <PktLib/VxCommon.h>

enum EHostConnectionType
{
    eHostConnectionTypeInvalid = 0,
    eHostConnectionTypeService,
    eHostConnectionTypeClient,
    eHostConnectionTypePeer,

    eMaxHostConnectionType
};

class HostSettingBase
{
public:
    HostSettingBase() = default;
    virtual ~HostSettingBase() = default;

    void                        setPluginType( EPluginType pluginType )                 { m_PluginType = pluginType; }
    EPluginType                 getPluginType( void )                                   { return m_PluginType; }
    void                        setConnectionType( EHostConnectionType connectType )    { m_ConnectType = connectType; }
    EHostConnectionType         getConnectionType( void )                               { return m_ConnectType; }
    void                        setServiceName( std::string& serviceName )              { m_ServiceName = serviceName; }
    std::string&                getServiceName( void )                                  { return m_ServiceName; }
    void                        setServiceUrl( std::string& serviceUrl )                { m_ServiceUrl = serviceUrl; }
    std::string&                getServiceUrl( void )                                   { return m_ServiceUrl; }
    void                        setServiceDesc( std::string& serviceDesc )              { m_ServiceDesc = serviceDesc; }
    std::string&                getServiceDesc( void )                                  { return m_ServiceDesc; }
    void                        setThumbId( VxGUID& id )                                { m_ThumbAssetId = id; }
    VxGUID&                     getThumbId( void )                                      { return m_ThumbAssetId; }
    void                        setThumbTimestamp( int64_t timeStamp )                  { m_LastThumbUpdateTimestamp = timeStamp; }
    int64_t                     getThumbTimestamp( void )                               { return m_LastThumbUpdateTimestamp; }
    void                        setConnectedTimestamp( int64_t timeStamp )              { m_LastConnectTimestamp = timeStamp; }
    int64_t                     getConnectedTimestamp( void )                           { return m_LastConnectTimestamp; }
    void                        setLastChangedTimestamp( int64_t timeStamp )            { m_LastChangeTimestamp = timeStamp; }
    int64_t                     getLastChangedTimestamp( void )                         { return m_LastChangeTimestamp; }
    void                        setPermissionLevel( EFriendState permLevel )            { m_PermissionLevel = permLevel; }
    EFriendState                getPermissionLevel( void )                              { return m_PermissionLevel; }



protected:
    EHostConnectionType         m_ConnectType = eHostConnectionTypeInvalid;
    EPluginType                 m_PluginType = ePluginTypeInvalid;
    std::string                 m_ServiceName;
    std::string                 m_ServiceUrl;
    std::string                 m_ServiceDesc;
    VxGUID                      m_ThumbAssetId;
    int64_t                     m_LastThumbUpdateTimestamp = 0;
    int64_t                     m_LastConnectTimestamp = 0;
    int64_t                     m_LastChangeTimestamp = 0;
    EFriendState                m_PermissionLevel = eFriendStateIgnore;

};

class HostSettingService : public HostSettingBase
{
public:
    HostSettingService() = default;
    virtual ~HostSettingService() = default;


protected:
    uint32_t                    m_MaxStoreAndForwardPerUser = 0;
    uint32_t                    m_MaxInConnections = 0;
    uint32_t                    m_MaxOutConnections = 0;
    uint32_t                    m_MaxBandwidth = 0;
};