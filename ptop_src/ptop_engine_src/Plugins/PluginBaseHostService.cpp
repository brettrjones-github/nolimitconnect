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
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>

//============================================================================
PluginBaseHostService::PluginBaseHostService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
    : PluginNetServices( engine, pluginMgr, myIdent, pluginType )
    , m_HostServerMgr(engine, pluginMgr, myIdent, *this)
{
}

//============================================================================
void PluginBaseHostService::buildHostAnnounce( PluginSetting& pluginSetting )
{
    m_AnnMutex.lock();
    m_Engine.lockAnnouncePktAccess();
    m_PktHostAnnounce.setPktAnn( m_Engine.getMyPktAnnounce() );
    m_PktAnnLastModTime = m_Engine.getPktAnnLastModTime();
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
        ( m_Engine.getEngineSettings().getFirewallTestSetting() == eFirewallTestAssumeNoFirewall || // assume no firewall means extern ip should be set
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
    LogMsg( LOG_DEBUG, "PluginBaseHostService %s got join request", DescribeHostType( getHostType() ) );
    PktHostJoinReq * joinReq = (PktHostJoinReq *)pktHdr;
    PktHostJoinReply joinReply;
    if( joinReq->isValidPkt() )
    {
        joinReply.setHostType( joinReq->getHostType() );
        joinReply.setPluginType( getPluginType() );
        joinReply.setSessionId( joinReq->getSessionId() );
        joinReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
        if( ePluginAccessOk == joinReply.getAccessState() )
        {
            m_HostServerMgr.onUserJoined( sktBase, netIdent, joinReq->getSessionId(), joinReq->getHostType() );
        }
        else if( ePluginAccessLocked == joinReply.getAccessState() )
        {
            if( !netIdent->isIgnored() )
            {
                if( m_HostServerMgr.getJoinState( netIdent, joinReq->getHostType() ) == eJoinStateJoinAccepted )
                {
                    // even though friendship not high enough if admin has accepted then send accepted
                    joinReply.setAccessState( ePluginAccessOk );
                }
                else
                {
                    // add to join request list
                    m_HostServerMgr.onJoinRequested( sktBase, netIdent, joinReq->getSessionId(), joinReq->getHostType() );
                }
            }
            else
            {
                // TODO .. should we drop the connection?
            }
        }
        else if( ePluginAccessDisabled == joinReply.getAccessState() )
        {
            // join request sent to disabled plugin.. this should not happen
            LogMsg( LOG_ERROR, "PluginBaseHostService %s got join request to disabled plugin from %s", DescribeHostType( getHostType() ), netIdent->getMyOnlineUrl().c_str() );
        }
        else if( ePluginAccessIgnored == joinReply.getAccessState() )
        {
            // TODO .. should we drop the connection of ignored person?
            LogMsg( LOG_ERROR, "PluginBaseHostService %s got join request from ignored person %s", DescribeHostType( getHostType() ), netIdent->getMyOnlineUrl().c_str() );
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
    PktHostSearchReq* searchReq = (PktHostSearchReq*)pktHdr;
    if( searchReq->isValidPkt() )
    {
        EPluginAccess pluginAccess = m_HostServerMgr.getPluginAccessState( netIdent );
        searchReply.setAccessState( pluginAccess );
        if( ePluginAccessOk == pluginAccess )
        {
            PktBlobEntry& blobEntry = searchReq->getBlobEntry();
            blobEntry.resetRead();

            SearchParams searchParams;
            searchParams.extractFromBlob( blobEntry );
            searchReply.setHostType( searchParams.getHostType() );
            searchReply.setSearchSessionId( searchParams.getSearchSessionId() );
            
            std::string searchText = searchParams.getSearchText();
            if( !searchParams.getSearchListAll() && searchText.size() < MIN_SEARCH_TEXT_LEN )
            {
                LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseHostService search text too short" );
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
            LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseHostService host service not enabled" );
            searchReply.setCommError( eCommErrPluginNotEnabled );   
        }
    }
    else
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseHostService invalid search packet" );
        searchReply.setCommError( eCommErrInvalidPkt );
    }
    
    searchReply.setHostType( searchReq->getHostType() );
    searchReply.setSearchSessionId( searchReq->getSearchSessionId() );
    EPluginType overridePlugin = searchReq->getPluginType();

    // BRJ temp for debug
    // searchReply.setIsLoopback( true );
    if( !txPacket( netIdent->getMyOnlineId(), sktBase, &searchReply, false, overridePlugin ) )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseHostService failed send search reply" );
    }
}

//============================================================================
void PluginBaseHostService::onPktPluginSettingReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginBaseHostService onPktPluginSettingReq" );
    PktPluginSettingReply settingReply;
    PktPluginSettingReq* settingReq = (PktPluginSettingReq*)pktHdr;
    if( settingReq->isValidPkt() )
    {
        PluginId pluginId = settingReq->getPluginId();

        settingReply.setHostType( settingReq->getHostType() );
        settingReply.setSessionId( settingReq->getSessionId() );
        settingReply.setPluginId( pluginId );
        settingReply.setPluginType( settingReq->getPluginType() );

        // for now we only handle a single entry.. we could add more for efficiency
        
        ECommErr searchErr = m_HostServerMgr.settingsRequest( pluginId, settingReply, sktBase, netIdent );
        settingReply.setCommError( searchErr );
    }
    else
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseHostService invalid search packet" );
        settingReply.setCommError( eCommErrInvalidPkt );
    }

    EPluginType overridePlugin = settingReq->getPluginType();

    // BRJ temp for debug
    // settingReply.setIsLoopback( true );
    if( !txPacket( netIdent->getMyOnlineId(), sktBase, &settingReply, false, overridePlugin ) )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "PluginBaseHostService failed send search reply" );
    }
}

//============================================================================
void PluginBaseHostService::onPktHostOfferReq( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join offer request" );
    PktHostOfferReply offerReply;
    offerReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
    if( !txPacket( netIdent, sktBase, &offerReply ) )
    {
        LogMsg( LOG_DEBUG, "PluginBaseHostService failed send onPktHostOfferReq" );
    }
}

//============================================================================
void PluginBaseHostService::onPktHostOfferReply( VxSktBase * sktBase, VxPktHdr * pktHdr, VxNetIdent * netIdent )
{
    LogMsg( LOG_DEBUG, "PluginChatRoomHost got join offer reply" );
}

//============================================================================
bool PluginBaseHostService::fromGuiRequestPluginThumb( VxNetIdent* netIdent, VxGUID& thumbId )
{
    if( netIdent && thumbId.isVxGUIDValid() )
    {
        VxSktBase* sktBase = 0;
        m_PluginMgr.pluginApiSktConnectTo( getPluginType(), netIdent, 0, &sktBase );
        if( sktBase && sktBase->isConnected() )
        {
            // the netIdent from gui is not the same one as in big list
            BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( netIdent->getMyOnlineId() );
            if( bigListInfo )
            {
                return ptopEngineRequestPluginThumb( sktBase, bigListInfo->getVxNetIdent(), thumbId );
            }
        }
    }

    return false;
}

//============================================================================
bool PluginBaseHostService::ptopEngineRequestPluginThumb( VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& thumbId )
{
    return m_ThumbXferMgr.requestPluginThumb( sktBase, netIdent, thumbId );
}