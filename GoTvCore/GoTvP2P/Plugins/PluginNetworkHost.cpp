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

#include "PluginNetworkHost.h"
#include "PluginMgr.h"
#include "P2PSession.h"
#include "RxSession.h"
#include "TxSession.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>
#include <PktLib/PktHostAnnounce.h>

//============================================================================
PluginNetworkHost::PluginNetworkHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
: PluginBaseHostService( engine, pluginMgr, myIdent )
{
    setPluginType( ePluginTypeNetworkHost );
}

//============================================================================
void PluginNetworkHost::onPktHostAnnounce( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostAnnounce* hostAnn = (PktHostAnnounce*)pktHdr;
    if( eHostTypeChatRoom == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got chat room announce" );
        m_HostServerMgr.updateHostSearchList( eHostTypeChatRoom, hostAnn, netIdent );
    }
    else if( eHostTypeConnectTest == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got connect test announce" );
        m_HostServerMgr.updateHostSearchList( eHostTypeConnectTest, hostAnn, netIdent );
    }
    else if( eHostTypeGroup == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got group announce" );
        m_HostServerMgr.updateHostSearchList( eHostTypeGroup, hostAnn, netIdent );
    }
    else if( eHostTypeNetwork == hostAnn->getHostType() )
    {
        // for now we are the only network host so ignore
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got network announce" );
    }
    else if( eHostTypeRandomConnect == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got random connect announce" );
        m_HostServerMgr.updateHostSearchList( eHostTypeRandomConnect, hostAnn, netIdent );
    }
    else
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost unknown announce %d", hostAnn->getHostType() );
    }
}
