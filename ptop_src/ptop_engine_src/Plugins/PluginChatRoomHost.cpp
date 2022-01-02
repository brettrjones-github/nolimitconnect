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

#include "PluginChatRoomHost.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

//============================================================================
PluginChatRoomHost::PluginChatRoomHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
    : PluginBaseHostService( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeHostChatRoom );
    setHostType( eHostTypeChatRoom );
}

//============================================================================
void PluginChatRoomHost::pluginStartup( void )
{
    PluginBaseHostService::pluginStartup();
}

//============================================================================
bool PluginChatRoomHost::setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    bool result = PluginBaseHostService::setPluginSetting( pluginSetting, modifiedTimeMs );
    buildHostChatRoomAnnounce( pluginSetting );
    sendHostChatRoomAnnounce();
    return result;
}

//============================================================================
void PluginChatRoomHost::onThreadOncePer15Minutes( void )
{
    sendHostChatRoomAnnounce();
}

//============================================================================
void PluginChatRoomHost::buildHostChatRoomAnnounce( PluginSetting& pluginSetting )
{
    updateHostInvite( pluginSetting );

    /*
    m_AnnMutex.lock();

    m_Engine.lockAnnouncePktAccess();
    std::string myOnlineUrl = m_Engine.getMyPktAnnounce().getMyOnlineUrl();

    m_PktHostAnnounce.setPktAnn( m_Engine.getMyPktAnnounce() );
    pluginSetting.setPluginUrl( myOnlineUrl );
    m_PktAnnLastModTime = m_Engine.getPktAnnLastModTime();
    m_Engine.unlockAnnouncePktAccess();

    m_PluginSetting = pluginSetting;
    m_PluginSetting.setUpdateTimestampToNow();
    BinaryBlob binarySetting;
    m_PluginSetting.toBinary( binarySetting );
    m_PktHostAnnounce.setHostType( getHostType() );
    m_PktHostAnnounce.setPluginSettingBinary( binarySetting );

    m_PktHostInviteIsValid = true;
    m_AnnMutex.unlock();
    */
}

//============================================================================
void PluginChatRoomHost::sendHostChatRoomAnnounce( void )
{
    if( m_Engine.isDirectConnectReady() )
    {
        LogModule( eLogHosts, LOG_DEBUG, "%s sendHostChatRoomAnnounce built %d ", DescribeHostType( getHostType() ), m_PktHostInviteIsValid );
        if( !m_PktHostInviteIsValid || m_Engine.getPktAnnLastModTime() != m_PktAnnLastModTime )
        {
            PluginSetting pluginSetting;
            if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
            {
                buildHostChatRoomAnnounce( pluginSetting );
            }
        }
    }
    else
    {
        LogModule( eLogHosts, LOG_DEBUG, "%s sendHostChatRoomAnnounce requires direct connect ", DescribeHostType( getHostType() ) );
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
            m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostInviteAnnounceReq, eConnectReasonChatRoomAnnounce );
            m_AnnMutex.unlock();
        }
    }
}

//============================================================================
void PluginChatRoomHost::onPluginSettingChange( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    updateHostInvite( pluginSetting );

    onPluginSettingsChanged( pluginSetting.getLastUpdateTimestamp() );
}

//============================================================================
EMembershipState PluginChatRoomHost::getMembershipState( VxNetIdent* netIdent )
{
    if( eFriendStateIgnore == getPluginPermission() )
    {
        return eMembershipStateJoinDenied;
    }

    return m_HostServerMgr.getMembershipState( netIdent, getHostType() );
}
