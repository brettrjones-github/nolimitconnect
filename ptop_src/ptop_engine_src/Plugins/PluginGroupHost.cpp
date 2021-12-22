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

#include "PluginGroupHost.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif
 

//============================================================================
PluginGroupHost::PluginGroupHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
    : PluginBaseHostService( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeHostGroup );
    setHostType( eHostTypeGroup );
}

//============================================================================
void PluginGroupHost::pluginStartup( void )
{
    PluginBaseHostService::pluginStartup();
}

//============================================================================
bool PluginGroupHost::setPluginSetting( PluginSetting& pluginSetting )
{
    bool result = PluginBaseHostService::setPluginSetting( pluginSetting );
    buildHostAnnounce( pluginSetting );
    sendHostAnnounce();
    return result;
}


//============================================================================
void PluginGroupHost::onThreadOncePer15Minutes( void )
{
    sendHostGroupAnnounce();
}

//============================================================================
void PluginGroupHost::buildHostGroupAnnounce( PluginSetting& pluginSetting )
{
    m_AnnMutex.lock();
    m_Engine.lockAnnouncePktAccess();
    m_PktHostAnnounce.setPktAnn( m_Engine.getMyPktAnnounce() );
    pluginSetting.setPluginUrl( m_Engine.getMyPktAnnounce().getMyOnlineUrl() );
    m_PktAnnLastModTime = m_Engine.getPktAnnLastModTime();
    m_Engine.unlockAnnouncePktAccess();
    m_PluginSetting = pluginSetting;
    m_PluginSetting.setUpdateTimestampToNow();
    BinaryBlob binarySetting;
    m_PluginSetting.toBinary( binarySetting );
    m_PktHostAnnounce.setHostType( getHostType() );
    m_PktHostAnnounce.setPluginSettingBinary( binarySetting );
    m_HostAnnounceBuilt = true;
    m_AnnMutex.unlock();
}

//============================================================================
void PluginGroupHost::sendHostGroupAnnounce( void )
{
    if( m_Engine.isDirectConnectReady() )
    {
        LogModule( eLogHosts, LOG_DEBUG, "%s sendHostGroupAnnounce built %d ", DescribeHostType( getHostType() ), m_HostAnnounceBuilt );
        if( !m_HostAnnounceBuilt || m_Engine.getPktAnnLastModTime() != m_PktAnnLastModTime )
        {
            PluginSetting pluginSetting;
            if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
            {
                buildHostGroupAnnounce( pluginSetting );
            }
        }
    }
    else
    {
        LogModule( eLogHosts, LOG_DEBUG, "%s sendHostGroupAnnounce requires direct connect ", DescribeHostType( getHostType() ) );
    }

    if( m_HostAnnounceBuilt && isPluginEnabled() && m_Engine.isDirectConnectReady() )
    {
        if( m_Engine.isNetworkHostEnabled() )
        {
            // if we are also network host then send to ourself also
            PluginBase* netHostPlugin = m_PluginMgr.getPlugin( ePluginTypeHostNetwork );
            if( netHostPlugin )
            {
                m_AnnMutex.lock();
                netHostPlugin->updateHostSearchList( m_PktHostAnnounce.getHostType(), &m_PktHostAnnounce, m_MyIdent );
                m_AnnMutex.unlock();
            }
        }
        else
        {
            VxGUID::generateNewVxGUID( m_AnnounceSessionId );
            m_AnnMutex.lock();
            m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostAnnounce, eConnectReasonGroupAnnounce );
            m_AnnMutex.unlock();
        }
    }
}

//============================================================================
void PluginGroupHost::onPluginSettingChange( PluginSetting& pluginSetting )
{
    m_SendAnnounceEnabled = pluginSetting.getAnnounceToHost();
    buildHostGroupAnnounce( pluginSetting );
}

//============================================================================
EMembershipState PluginGroupHost::getMembershipState( VxNetIdent* netIdent )
{
    if( eFriendStateIgnore == getPluginPermission() )
    {
        return eMembershipStateJoinDenied;
    }

    return m_HostServerMgr.getMembershipState( netIdent, getHostType() );
}
