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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif
 

//============================================================================
PluginGroupHost::PluginGroupHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
    : PluginBaseHostService( engine, pluginMgr, myIdent )
{
    setPluginType( ePluginTypeGroupHost );
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
    m_Engine.unlockAnnouncePktAccess();
    m_PluginSetting = pluginSetting;
    m_PluginSetting.setUpdateTimestampToNow();
    BinaryBlob binarySetting;
    m_PluginSetting.toBinary( binarySetting );
    m_PktHostAnnounce.setHostType( eHostTypeGroup );
    m_PktHostAnnounce.setPluginSettingBinary( binarySetting );
    // m_PktHostAnnounce.setIsLoopback( true ); // BRJ temp for testing
    m_HostAnnounceBuilt = true;
    m_AnnMutex.unlock();
}

//============================================================================
void PluginGroupHost::sendHostGroupAnnounce( void )
{
    if( !m_HostAnnounceBuilt && 
        ( m_Engine.getEngineSettings().getFirewallTestSetting() == FirewallSettings::eFirewallTestAssumeNoFirewall || // assume no firewall means extern ip should be set
            m_Engine.getNetStatusAccum().isDirectConnectTested() ) ) // isDirectConnectTested means my url should be valid
    {
        PluginSetting pluginSetting;
        if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
        {
            buildHostGroupAnnounce( pluginSetting );
        }
    }

    if( m_HostAnnounceBuilt && isPluginEnabled() && m_Engine.getNetStatusAccum().getNetAvailStatus() != eNetAvailNoInternet )
    {
        VxGUID::generateNewVxGUID( m_AnnounceSessionId );
        m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostAnnounce, eConnectReasonGroupAnnounce );
    }
}

//============================================================================
void PluginGroupHost::onPluginSettingChange( PluginSetting& pluginSetting )
{
    m_SendAnnounceEnabled = pluginSetting.getAnnounceToHost();
    buildHostGroupAnnounce( pluginSetting );
}
