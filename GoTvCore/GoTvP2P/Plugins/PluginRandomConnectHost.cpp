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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginRandomConnectHost::PluginRandomConnectHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBaseHostService( engine, pluginMgr, myIdent )
{
    setPluginType( ePluginTypeRandomConnectHost );
}

//============================================================================
void PluginRandomConnectHost::pluginStartup( void )
{
    PluginBaseHostService::pluginStartup();
}

//============================================================================
bool PluginRandomConnectHost::setPluginSetting( PluginSetting& pluginSetting )
{
    bool result = PluginBaseHostService::setPluginSetting( pluginSetting );
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
    m_AnnMutex.lock();
    m_Engine.lockAnnouncePktAccess();
    m_PktHostAnnounce.setPktAnn( m_Engine.getMyPktAnnounce() );
    pluginSetting.setPluginUrl( m_Engine.getMyPktAnnounce().getMyOnlineUrl() );
    m_Engine.unlockAnnouncePktAccess();
    m_PluginSetting = pluginSetting;
    m_PluginSetting.setUpdateTimestampToNow();
    BinaryBlob binarySetting;
    m_PluginSetting.toBinary( binarySetting );
    m_PktHostAnnounce.setHostType( eHostTypeRandomConnect );
    m_PktHostAnnounce.setPluginSettingBinary( binarySetting );
    // m_PktHostAnnounce.setIsLoopback( true ); // BRJ temp for testing
    m_HostAnnounceBuilt = true;
    m_AnnMutex.unlock();
}

//============================================================================
void PluginRandomConnectHost::sendHostRandomConnectAnnounce( void )
{
    if( !m_HostAnnounceBuilt && m_Engine.isDirectConnectReady() )
    {
        PluginSetting pluginSetting;
        if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
        {
            buildHostRandomConnectAnnounce( pluginSetting );
        }
    }

    if( m_HostAnnounceBuilt && isPluginEnabled() && m_Engine.isDirectConnectReady() )
    {
        if( m_Engine.isNetworkHostEnabled() )
        {
            // if we are also network host then send to ourself also
            PluginBase* netHostPlugin = m_PluginMgr.getPlugin( ePluginTypeNetworkHost );
            if( netHostPlugin )
            {
                m_AnnMutex.lock();
                netHostPlugin->updateHostSearchList( m_PktHostAnnounce.getHostType(), &m_PktHostAnnounce, m_MyIdent );
                m_AnnMutex.unlock();
            }
        }

        VxGUID::generateNewVxGUID( m_AnnounceSessionId );
        m_AnnMutex.lock();
        m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostAnnounce, eConnectReasonRandomConnectAnnounce );
        m_AnnMutex.unlock();
    }
}

//============================================================================
void PluginRandomConnectHost::onPluginSettingChange( PluginSetting& pluginSetting )
{
    m_SendAnnounceEnabled = pluginSetting.getAnnounceToHost();
    buildHostRandomConnectAnnounce( pluginSetting );
}
