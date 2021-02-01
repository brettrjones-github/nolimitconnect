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

#include "PluginGroupClient.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PConnectList.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginGroupClient::PluginGroupClient( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBaseHostClient( engine, pluginMgr, myIdent )
// , m_PluginSessionMgr( *this, pluginMgr )
{
    setPluginType( ePluginTypeGroupClient );
}

//============================================================================
void PluginGroupClient::pluginStartup( void )
{
    PluginBase::pluginStartup();
}

//============================================================================
bool PluginGroupClient::setPluginSetting( PluginSetting& pluginSetting )
{
    bool result = PluginBaseHostClient::setPluginSetting( pluginSetting );
    buildHostAnnounce( pluginSetting );
    sendHostAnnounce();
    return result;
}

//============================================================================
void PluginGroupClient::onThreadOncePer15Minutes( void )
{
    sendHostAnnounce();
}

//============================================================================
void PluginGroupClient::onPluginSettingChange( PluginSetting& pluginSetting )
{
    m_SendAnnounceEnabled = pluginSetting.getAnnounceToHost();
    buildHostAnnounce( pluginSetting );
}

/*
//============================================================================
bool PluginGroupClient::onContactConnected( EConnectReason hostConnectType, VxSktBase* sktBase )
{
    if( m_SendAnnounceEnabled && m_HostAnnounceBuilt && isPluginEnabled() )
    {
        if( eConnectReasonGroupAnnounce == hostConnectType )
        {
            m_AnnMutex.lock();
            if( m_Engine.lockSkt( sktBase ) )
            {

                if( sktBase && sktBase->getPeerPktAnn().getMyOnlineId().isVxGUIDValid() )
                {
                    sktBase->txPacket( sktBase->getPeerPktAnn().getMyOnlineId(), &m_PktHostAnnounce );
                }

                m_Engine.unlockSkt( sktBase );
            }

            m_AnnMutex.unlock();
        }
    }

    m_Engine.getConnectionMgr().requestHostConnection( eHostTypeGroup, getPluginType(), eConnectReasonGroupAnnounce, this );

    return false;
}

//============================================================================
void PluginGroupClient::onContactDisconnected( EConnectReason hostConnectType, VxSktBase* sktBase )
{
    if( eConnectReasonGroupAnnounce == hostConnectType )
    {
        // no action needed. we connect and send our group listing then disconnect
    }
}
*/