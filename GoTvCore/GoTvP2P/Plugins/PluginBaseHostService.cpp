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

#include "PluginBaseHostService.h"

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

//============================================================================
PluginBaseHostService::PluginBaseHostService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent )
    : PluginNetServices( engine, pluginMgr, myIdent )
    , m_HostServerMgr(engine, pluginMgr, myIdent, *this)
{
}

//============================================================================
void PluginBaseHostService::buildHostAnnounce( PluginSetting& pluginSetting )
{
    m_AnnMutex.lock();
    m_Engine.lockAnnouncePktAccess();
    m_PktHostAnnounce.setPktAnn( m_Engine.getMyPktAnnounce() );
    m_Engine.unlockAnnouncePktAccess();
    m_PluginSetting = pluginSetting;
    BinaryBlob binarySetting;
    m_PluginSetting.toBinary( binarySetting );
    m_PktHostAnnounce.setPluginSettingBinary( binarySetting );
    m_HostAnnounceBuilt = true;
    m_AnnMutex.unlock();
}

//============================================================================
void PluginBaseHostService::sendHostAnnounce( void )
{
    if( !m_HostAnnounceBuilt && 
        ( m_Engine.getEngineSettings().getFirewallTestSetting() == FirewallSettings::eFirewallTestAssumeNoFirewall || // assume no firewall means extern ip should be set
            m_Engine.getNetStatusAccum().isDirectConnectTested() ) ) // isDirectConnectTested means my url should be valid
    {
        PluginSetting pluginSetting;
        if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
        {
            buildHostAnnounce( pluginSetting );
        }
    }

    if( m_HostAnnounceBuilt && isPluginEnabled() && m_Engine.getNetStatusAccum().getNetAvailStatus() != eNetAvailNoInternet )
    {
        VxGUID::generateNewVxGUID( m_AnnounceSessionId );
        m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostAnnounce, getHostAnnounceConnectReason() );
    }
}

//============================================================================
void PluginBaseHostService::replaceConnection( VxNetIdent * netIdent, VxSktBase * poOldSkt, VxSktBase * poNewSkt )
{
//    m_PluginSessionMgr.replaceConnection( netIdent, poOldSkt, poNewSkt );
}

//============================================================================
void PluginBaseHostService::onContactWentOffline( VxNetIdent * netIdent, VxSktBase * sktBase )
{
//    m_PluginSessionMgr.onContactWentOffline( netIdent, sktBase );
//    m_FriendGuidList.removeGuid( netIdent->getMyOnlineId() );
}

//============================================================================
void PluginBaseHostService::onConnectionLost( VxSktBase * sktBase )
{
//    m_PluginSessionMgr.onConnectionLost( sktBase );
}

//============================================================================
void PluginBaseHostService::onPktHostJoinReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join request" );
    PktHostJoinReq * joinReq = (PktHostJoinReq *)pktHdr;
    PktHostJoinReply joinReply;
    if( joinReq->isValidPkt() )
    {
        joinReply.setSessionId( joinReq->getSessionId() );
        joinReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
        if( ePluginAccessOk == joinReply.getAccessState() )
        {

        }

        txPacket( netIdent, sktBase, &joinReply );
    }
    else
    {
        LogMsg( LOG_DEBUG, "PluginBaseHostService onPktHostJoinReq Invalid Packet" );
        joinReply.setCommError( eCommErrInvalidPkt );
        onInvalidRxedPacket( sktBase, pktHdr, netIdent );   
    }
}

//============================================================================
void PluginBaseHostService::onPktHostSearchReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginBaseHostService onPktHostSearchReq" );
    PktHostSearchReply searchReply;
    searchReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
    if( ePluginAccessOk == searchReply.getAccessState() )
    {
        PktHostSearchReq* searchReq = (PktHostSearchReq*)pktHdr;
        if( searchReq->isValidPkt() )
        {
            SearchParams searchParams;
            searchParams.extractFromBlob( searchReq->getBlobEntry() );
            searchReply.setHostType( searchParams.getHostType() );
            searchReply.setSearchSessionId( searchParams.getSearchSessionId() );
            
            std::string searchText = searchParams.getSearchText();
            if( searchText.size() < MIN_SEARCH_TEXT_LEN )
            {
                searchReply.setCommError( eCommErrSearchTextToShort );
            }
            else
            {
                ECommErr searchErr = m_HostServerMgr.searchRequest( searchParams, searchReply, searchText, sktBase, netIdent );
                searchReply.setCommError( searchErr );
            }
        }
        else
        {
            searchReply.setCommError( eCommErrInvalidPkt );
            onInvalidRxedPacket( sktBase, pktHdr, netIdent );     
        }
    }

    txPacket( netIdent, sktBase, &searchReply );
}

//============================================================================
void PluginBaseHostService::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join offer request" );
    PktHostOfferReply offerReply;
    offerReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
    txPacket( netIdent, sktBase, &offerReply );
}

//============================================================================
void PluginBaseHostService::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join offer reply" );
}
