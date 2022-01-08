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

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

#include <CoreLib/VxFileUtil.h>
#include <PktLib/PktsHostInvite.h>

//============================================================================
PluginNetworkHost::PluginNetworkHost( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
: PluginBaseHostService( engine, pluginMgr, myIdent, pluginType )
{
    setPluginType( ePluginTypeHostNetwork );
    setHostType( eHostTypeNetwork );
}

//============================================================================
void PluginNetworkHost::onPktHostInviteAnnReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    PktHostInviteAnnounceReq* hostAnn = ( PktHostInviteAnnounceReq*)pktHdr;
    if( eHostTypeChatRoom == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got chat room announce" );
        updateHostSearchList( eHostTypeChatRoom, hostAnn, netIdent, sktBase );
    }
    else if( eHostTypeGroup == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got group announce" );
        updateHostSearchList( eHostTypeGroup, hostAnn, netIdent, sktBase );
    }
    else if( eHostTypeRandomConnect == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got random connect announce" );
       updateHostSearchList( eHostTypeRandomConnect, hostAnn, netIdent, sktBase );
    }
    else if( eHostTypeNetwork == hostAnn->getHostType() )
    {
        // for now we are the only network host so ignore
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got network announce.. ignored" );
    }
    else if( eHostTypeConnectTest == hostAnn->getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost got connect test announce" );
        updateHostSearchList( eHostTypeConnectTest, hostAnn, netIdent, sktBase );
    }
    else
    {
        LogMsg( LOG_VERBOSE, "PluginNetworkHost unknown announce %d", hostAnn->getHostType() );
    }
}

//============================================================================
void PluginNetworkHost::updateHostSearchList( EHostType hostType, PktHostInviteAnnounceReq* hostAnn, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    m_HostServerMgr.updateHostSearchList( hostType, hostAnn, netIdent );
}

//============================================================================
void PluginNetworkHost::fromGuiSendAnnouncedList( EHostType hostType )
{
    m_HostServerMgr.fromGuiSendAnnouncedList( hostType );
}

//============================================================================
void PluginNetworkHost::fromGuiListAction( EListAction listAction )
{
    m_HostServerMgr.fromGuiListAction( listAction );
}

//============================================================================
void PluginNetworkHost::onPktHostInviteSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_HostServerMgr.onPktHostInviteSearchReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginNetworkHost::onPktHostInviteSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_HostServerMgr.onPktHostInviteSearchReply( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginNetworkHost::onPktHostInviteMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_HostServerMgr.onPktHostInviteMoreReq( sktBase, pktHdr, netIdent );
}

//============================================================================
void PluginNetworkHost::onPktHostInviteMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_HostServerMgr.onPktHostInviteMoreReply( sktBase, pktHdr, netIdent );
}
