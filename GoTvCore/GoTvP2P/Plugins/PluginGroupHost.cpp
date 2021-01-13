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

#include "PluginGroupHost.h"
#include "PluginMgr.h"

#include "RelayServerSession.h"
#include "RelayClientSession.h"
#include "Relay.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <PktLib/PktsRelay.h>
#include <PktLib/PktAnnounce.h>
#include <PktLib/PktsRelay.h>

#include <NetLib/VxSktBase.h>

#include <string.h>

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
    buildHostGroupAnnounce( pluginSetting );
    sendHostGroupAnnounce();
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
    m_Engine.unlockAnnouncePktAccess();
    m_PluginSetting = pluginSetting;
    BinaryBlob binarySetting;
    m_PluginSetting.toBinary( binarySetting );
    m_PktHostAnnounce.setSettingBinary( binarySetting );
    m_HostAnnounceBuilt = true;
    m_AnnMutex.unlock();
}

//============================================================================
void PluginGroupHost::sendHostGroupAnnounce( void )
{
    if( m_SendAnnounceEnabled && m_HostAnnounceBuilt && isPluginEnabled() )
    {
        m_Engine.getConnectionMgr().requestHostConnection( eHostTypeChatRoom, getPluginType(), eConnectRequestChatRoomAnnounce, this );
    }
}

//============================================================================
void PluginGroupHost::onPluginSettingChange( PluginSetting& pluginSetting )
{
    m_SendAnnounceEnabled = pluginSetting.getAnnounceToHost();
    buildHostGroupAnnounce( pluginSetting );
}

//============================================================================
bool PluginGroupHost::onContactConnected( EConnectRequestType hostConnectType, VxSktBase* sktBase )
{
    if( m_SendAnnounceEnabled && m_HostAnnounceBuilt && isPluginEnabled() )
    {
        if( eConnectRequestChatRoomAnnounce == hostConnectType )
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

    m_Engine.getConnectionMgr().requestHostConnection( eHostTypeChatRoom, getPluginType(), eConnectRequestChatRoomAnnounce, this );

    return false;
}

//============================================================================
void PluginGroupHost::onContactDisconnected( EConnectRequestType hostConnectType, VxSktBase* sktBase )
{
    if( eConnectRequestChatRoomAnnounce == hostConnectType )
    {
        // no action needed. we connect and send our group listing then disconnect
    }
}
