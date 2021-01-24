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

#include "HostClientMgr.h"

#include <GoTvInterface/IToGui.h>
#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

//============================================================================
HostClientMgr::HostClientMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
    : HostClientSearchMgr( engine, pluginMgr, myIdent, pluginBase )
{
}

//============================================================================
void HostClientMgr::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostJoinReply* hostReply = ( PktHostJoinReply* )pktHdr;
    if( ePluginAccessOk == hostReply->getAccessState() )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSuccess );
        onHostJoined( sktBase, netIdent );
       
    }
    else if( ePluginAccessOk == hostReply->getAccessState() )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinFailPermission );
        m_Engine.getConnectionMgr().doneWithConnection( netIdent->getMyOnlineId(), this, eConnectReasonChatRoomJoin );
    }
    else
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinFail, DescribePluginAccess2( hostReply->getAccessState() ) );
        m_Engine.getConnectionMgr().doneWithConnection( netIdent->getMyOnlineId(), this, eConnectReasonChatRoomJoin );
    }
}

//============================================================================
void HostClientMgr::onPktHostSearchReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostSearchReply* hostReply = ( PktHostSearchReply* )pktHdr;

}

//============================================================================
void HostClientMgr::onHostJoined( VxSktBase * sktBase, VxNetIdent * netIdent )
{
    m_ServerList.addGuidIfDoesntExist( netIdent->getMyOnlineId() );
    //addContact(sktBase, netIdent );
}

//============================================================================
void HostClientMgr::onContactDisconnected( VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    m_ServerList.removeGuid( onlineId );
    removeContact( onlineId );
}
