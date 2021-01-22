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

#include "PluginChatRoomHost.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginChatRoomHost::PluginChatRoomHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
    : PluginBaseHostService( engine, pluginMgr, myIdent )
    , m_HostServerMgr(engine, pluginMgr, myIdent, *this)
{
    setPluginType( ePluginTypeChatRoomHost );
}

//============================================================================
void PluginChatRoomHost::pluginStartup( void )
{
    PluginBaseHostService::pluginStartup();
}

//============================================================================
bool PluginChatRoomHost::setPluginSetting( PluginSetting& pluginSetting )
{
    bool result = PluginBaseHostService::setPluginSetting( pluginSetting );
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
void PluginChatRoomHost::sendHostChatRoomAnnounce( void )
{
    if( !m_HostAnnounceBuilt && m_Engine.getNetStatusAccum().getNetAvailStatus() != eNetAvailNoInternet )
    {
        PluginSetting pluginSetting;
        m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting );
        buildHostChatRoomAnnounce( pluginSetting );
    }

    if( m_HostAnnounceBuilt && isPluginEnabled() )
    {
        m_HostServerMgr.sendHostAnnounceToNetworkHost( m_PktHostAnnounce, eConnectReasonChatRoomAnnounce );
    }
}

//============================================================================
void PluginChatRoomHost::onPluginSettingChange( PluginSetting& pluginSetting )
{
    m_SendAnnounceEnabled = pluginSetting.getAnnounceToHost();
    buildHostChatRoomAnnounce( pluginSetting );
}

//============================================================================
void PluginChatRoomHost::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join request" );
    PktHostJoinReply joinReply;
    joinReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
    txPacket( netIdent, sktBase, &joinReply );
}

//============================================================================
void PluginChatRoomHost::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join reply" );
}

//============================================================================
void PluginChatRoomHost::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join offer request" );
    PktHostOfferReply offerReply;
    offerReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
    txPacket( netIdent, sktBase, &offerReply );
}

//============================================================================
void PluginChatRoomHost::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join offer reply" );
}
