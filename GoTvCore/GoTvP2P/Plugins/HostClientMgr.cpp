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

#include <CoreLib/VxGUIDList.h>

#include <PktLib/PktsHostJoin.h>

//============================================================================
HostClientMgr::HostClientMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
    : HostBaseMgr( engine, pluginMgr, myIdent, pluginBase )
{
}

//============================================================================
void HostClientMgr::onPktHostJoinReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostJoinReply* hostReply = ( PktHostJoinReply* )pktHdr;
    if( ePluginAccessOk == hostReply->getAccessState() )
    {
        m_Engine.getToGui().toGuiHostJoinStatus( eHostTypeChatRoom, eHostJoinSuccess );
        m_JoinedHostList.addGuid( netIdent->getMyOnlineId() );
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
