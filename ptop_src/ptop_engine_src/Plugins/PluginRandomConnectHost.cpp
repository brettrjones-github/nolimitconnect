//============================================================================
// Copyright (C) 2010 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include "PluginRandomConnectHost.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginRandomConnectHost::PluginRandomConnectHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBaseHostService( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeHostRandomConnect );
    setHostType( eHostTypeRandomConnect );
}

//============================================================================
void PluginRandomConnectHost::pluginStartup( void )
{
    PluginBaseHostService::pluginStartup();
}

//============================================================================
bool PluginRandomConnectHost::setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    bool result = PluginBaseHostService::setPluginSetting( pluginSetting, modifiedTimeMs );
    buildHostAnnounce( pluginSetting );
    sendHostAnnounce();
    return result;
}


//============================================================================
void PluginRandomConnectHost::onThreadOncePer15Minutes( void )
{
    sendHostRandomConnectAnnounce();
}

//============================================================================
void PluginRandomConnectHost::buildHostRandomConnectAnnounce( PluginSetting& pluginSetting )
{
    updateHostInvite( pluginSetting );
}

//============================================================================
void PluginRandomConnectHost::sendHostRandomConnectAnnounce( void )
{
    if( m_Engine.isDirectConnectReady() )
    {
        LogModule( eLogHosts, LOG_DEBUG, "%s sendHostChatRoomAnnounce built %d ", DescribeHostType( getHostType() ), m_PktHostInviteIsValid );
        if( !m_PktHostInviteIsValid || m_Engine.getPktAnnLastModTime() != m_PktAnnLastModTime )
        {
            PluginSetting pluginSetting;
            if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
            {
                buildHostRandomConnectAnnounce( pluginSetting );
            }
        }
    }
    else
    {
        LogModule( eLogHosts, LOG_DEBUG, "%s sendHostRandomConnectAnnounce requires direct connect ", DescribeHostType( getHostType() ) );
    }

    if( m_PktHostInviteIsValid && isPluginEnabled() && m_Engine.isDirectConnectReady() )
    {
        if( m_Engine.isNetworkHostEnabled() )
        {
            // if we are also network host then send to ourself also
            PluginBase* netHostPlugin = m_PluginMgr.getPlugin( ePluginTypeHostNetwork );
            if( netHostPlugin )
            {
                m_AnnMutex.lock();
                netHostPlugin->updateHostSearchList( m_PktHostInviteAnnounceReq.getHostType(), &m_PktHostInviteAnnounceReq, m_MyIdent );
                m_AnnMutex.unlock();
            }
        }
        else
        {
            VxGUID::generateNewVxGUID( m_AnnounceSessionId );
            m_AnnMutex.lock();
            m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostInviteAnnounceReq, eConnectReasonRandomConnectAnnounce );
            m_AnnMutex.unlock();
        }
    }
}

//============================================================================
void PluginRandomConnectHost::onPluginSettingChange( PluginSetting& pluginSetting, int64_t lastModifiedTime )
{
    updateHostInvite( pluginSetting );
    buildHostRandomConnectAnnounce( pluginSetting );
    onPluginSettingsChanged( pluginSetting.getLastUpdateTimestamp() );
}
