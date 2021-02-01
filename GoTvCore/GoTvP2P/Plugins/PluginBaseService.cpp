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

#include "PluginBaseService.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>

//============================================================================
PluginBaseService::PluginBaseService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBase( engine, pluginMgr, myIdent )
{
}

//============================================================================
EConnectReason PluginBaseService::getHostAnnounceConnectReason( void )
{
    EConnectReason connectReason = eConnectReasonUnknown;
    switch( getPluginType() )
    {
    case ePluginTypeChatRoomClient:
    case ePluginTypeChatRoomHost:
        connectReason = eConnectReasonChatRoomAnnounce;
        break;
    case ePluginTypeGroupClient:
    case ePluginTypeGroupHost:
        connectReason = eConnectReasonGroupAnnounce;
        break;
    case ePluginTypeRandomConnectHost:
    case ePluginTypeRandomConnectClient:
        connectReason = eConnectReasonRandomConnectAnnounce;
        break;
    default:
        break;
    }

    return connectReason;
}

//============================================================================
EConnectReason PluginBaseService::getHostJoinConnectReason( void )
{
    EConnectReason connectReason = eConnectReasonUnknown;
    switch( getPluginType() )
    {
    case ePluginTypeChatRoomClient:
    case ePluginTypeChatRoomHost:
        connectReason = eConnectReasonChatRoomJoin;
        break;
    case ePluginTypeGroupClient:
    case ePluginTypeGroupHost:
        connectReason = eConnectReasonGroupJoin;
        break;
    case ePluginTypeRandomConnectHost:
    case ePluginTypeRandomConnectClient:
        connectReason = eConnectReasonRandomConnectJoin;
        break;
    default:
        break;
    }

    return connectReason;
}

//============================================================================
EConnectReason PluginBaseService::getHostSearchConnectReason( void )
{
    EConnectReason connectReason = eConnectReasonUnknown;
    switch( getPluginType() )
    {
    case ePluginTypeChatRoomClient:
    case ePluginTypeChatRoomHost:
        connectReason = eConnectReasonChatRoomSearch;
        break;
    case ePluginTypeGroupClient:
    case ePluginTypeGroupHost:
        connectReason = eConnectReasonGroupSearch;
        break;
    case ePluginTypeRandomConnectHost:
    case ePluginTypeRandomConnectClient:
        connectReason = eConnectReasonRandomConnectSearch;
        break;
    default:
        break;
    }

    return connectReason;
}