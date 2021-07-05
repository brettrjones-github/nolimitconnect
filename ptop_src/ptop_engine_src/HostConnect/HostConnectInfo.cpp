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

#include "HostConnectInfo.h"

//============================================================================
void HostConnectInfo::setHostInfo( EPluginType pluginType, std::string ipAddr, uint16_t port, VxGUID onlineId, const char * url )
{
    addHostService( pluginType );
    setHostIpAddr( ipAddr );
    setHostPort( port );
    setHostUrl( url );
}

//============================================================================
bool HostConnectInfo::hasHostService( EPluginType pluginType )
{
    bool found = false;
    for( auto iter = m_PluginServiceList.begin(); iter != m_PluginServiceList.end(); ++iter )
    {
        if( *iter == pluginType )
        {
            found = true;
            break;
        }
    }

    return found;
 }

//============================================================================
bool HostConnectInfo::isConnected( EHostConnectType connectType )
{
    EPluginType pluginType = connectTypeToPluginType( connectType );
    if( pluginType == ePluginTypeInvalid )
    {
        return false;
    }

    return hasHostService( pluginType );

}

//============================================================================
bool HostConnectInfo::isConnected( EPluginType pluginType )
{
    bool found = false;
    for( auto iter = m_PluginServiceList.begin(); iter != m_PluginServiceList.end(); ++iter )
    {
        if( *iter == pluginType )
        {
            found = true;
            break;
        }
    }

    return found;
}

//============================================================================
EPluginType HostConnectInfo::connectTypeToPluginType( EHostConnectType connectType )
{
    EPluginType pluginType = ePluginTypeInvalid;

    switch( connectType )
    {
    case eHostConnectRelayFind:
    case eHostConnectRelayJoin:
        pluginType = ePluginTypeRelay;
        break;
    case eHostConnectGroupAnnounce:
    case eHostConnectGroupFind:
        pluginType = ePluginTypeHostGroupListing;
        break;
    case eHostConnectGroupJoin:
        pluginType = ePluginTypeHostGroup;
        break;
    case eHostConnectChatRoomAnnounce:
    case eHostConnectChatRoomFind:
        pluginType = ePluginTypeHostGroupListing;
        break;
    case eHostConnectChatRoomJoin:
        pluginType = ePluginTypeChatRoom;
        break;
    }

    return pluginType;
}

//============================================================================
bool HostConnectInfo::hasHostService( EHostConnectType connectType )
{
    EPluginType pluginType = connectTypeToPluginType( connectType );
    if( pluginType == ePluginTypeInvalid )
    {
        return false;
    }

    return hasHostService( pluginType );
}

//============================================================================
void HostConnectInfo::addHostService( EPluginType pluginType )
{
    if( !hasHostService( pluginType ) )
    {
        m_PluginServiceList.push_back( pluginType );
    }
}

//============================================================================
void HostConnectInfo::removeHostService( EPluginType pluginType )
{
    for( auto iter = m_PluginServiceList.begin(); iter != m_PluginServiceList.end(); ++iter )
    {
        if( *iter == pluginType )
        {
            m_PluginServiceList.erase( iter );
            break;
        }
    }
}

//============================================================================
void HostConnectInfo::onEngineContactConnected( RcConnectInfo * poInfo, bool connectionListLocked )
{

}

//============================================================================
void HostConnectInfo::onEngineContactDisconnected( RcConnectInfo * poInfo, bool connectionListLocked )
{

}
