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
// http://www.nolimitconnect.org
//============================================================================

#include "PluginBaseHostService.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <CoreLib/Invite.h>
#include <CoreLib/VxPtopUrl.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
#include <PktLib/PktsHostSearch.h>
#include <PktLib/PktsHostInfo.h>
#include <PktLib/PktsGroupie.h>

//============================================================================
PluginBaseHostService::PluginBaseHostService( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, EPluginType pluginType )
    : PluginNetServices( engine, pluginMgr, myIdent, pluginType )
    , m_HostServerMgr(engine, pluginMgr, myIdent, *this)
{
}

//============================================================================
EPluginType PluginBaseHostService::getClientPluginType( void )
{
    return HostPluginToClientPluginType( getPluginType() );
}

//============================================================================
bool PluginBaseHostService::getHostedInfo( HostedInfo& hostedInfo )
{
    bool result = false;
    if( !m_HostInviteUrl.empty() && !m_HostTitle.empty() && !m_HostDescription.empty() && m_HostInfoModifiedTime )
    {
        m_AnnMutex.lock();
        hostedInfo.setHostInfoTimestamp( m_HostInfoModifiedTime );
        hostedInfo.setHostInviteUrl( m_HostInviteUrl );
        hostedInfo.setHostTitle( m_HostTitle );
        hostedInfo.setHostDescription( m_HostDescription );
        hostedInfo.setHostedId( getHostedId() );

        m_AnnMutex.unlock();
        result = hostedInfo.isValidForGui();
    }

    return result;
}

//============================================================================
bool PluginBaseHostService::setPluginSetting( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    bool result = PluginNetServices::setPluginSetting( pluginSetting, modifiedTimeMs );
    if( isPluginEnabled() && PluginShouldAnnounceToNetwork( getPluginType() ) )
    {
        buildHostAnnounce( pluginSetting );
        sendHostAnnounce();
    }

    return result;
}

//============================================================================
void PluginBaseHostService::onPluginSettingChange( PluginSetting& pluginSetting, int64_t modifiedTimeMs )
{
    if( isPluginEnabled() && PluginShouldAnnounceToNetwork( getPluginType() ) )
    {
        updateHostInvite( pluginSetting );
    }

    onPluginSettingsChanged( pluginSetting.getLastUpdateTimestamp() );
}

//============================================================================
void PluginBaseHostService::onThreadOncePer15Minutes( void )
{
    if( isPluginEnabled() && PluginShouldAnnounceToNetwork( getPluginType() ) )
    {
        sendHostAnnounce();
    }
}

//============================================================================
void PluginBaseHostService::onMyOnlineUrlIsValid( bool isValidUrl )
{
    if( isValidUrl )
    {
        sendHostAnnounce();
    }
    else
    {
        m_PktHostInviteIsValid = false;
    }
}

//============================================================================
void PluginBaseHostService::buildHostAnnounce( PluginSetting& pluginSetting )
{
    updateHostInvite( pluginSetting );
}

//============================================================================
void PluginBaseHostService::sendHostAnnounce( void )
{
    if( !isPluginEnabled() )
    {
        return;
    }

    if( m_Engine.getMyPktAnnounce().requiresRelay() )
    {
        VxGUID sessionId;
        m_Engine.getToGui().toGuiHostAnnounceStatus( getHostType(), sessionId, eHostAnnounceFailRequiresOpenPort, "Announce Host Requires An Open Port" );
        return;
    }

    if( !m_PktHostInviteIsValid && 
        ( m_Engine.getEngineSettings().getFirewallTestSetting() == eFirewallTestAssumeNoFirewall || // assume no firewall means extern ip should be set
            m_Engine.getNetStatusAccum().isDirectConnectTested() ) ) // isDirectConnectTested means my url should be valid
    {
        PluginSetting pluginSetting;
        if( m_Engine.getPluginSettingMgr().getPluginSetting( getPluginType(), pluginSetting ) )
        {
            buildHostAnnounce( pluginSetting );
        }
    }

    if( !m_PktHostInviteIsValid )
    {
        return;
    }

    bool sentToOurself = false;
    if( m_Engine.isNetworkHostEnabled() )
    {
        std::string netHostUrlStr = m_Engine.fromGuiQueryDefaultUrl( eHostTypeNetwork );
        VxPtopUrl netHostUrl( netHostUrlStr );
        if( netHostUrl.isValid() && netHostUrl.getOnlineId() == m_Engine.getMyOnlineId() )
        {
            // if we are also network host then send to ourself also
            PluginBase* netHostPlugin = m_PluginMgr.getPlugin( ePluginTypeHostNetwork );
            if( netHostPlugin )
            {
                m_AnnMutex.lock();
                netHostPlugin->updateHostSearchList( m_PktHostInviteAnnounceReq.getHostType(), &m_PktHostInviteAnnounceReq, m_MyIdent );
                m_AnnMutex.unlock();
                sentToOurself = true;
            }
        }
    }
    
    if( !sentToOurself )
    {
        if( m_PktHostInviteIsValid && m_Engine.getNetStatusAccum().getNetAvailStatus() != eNetAvailNoInternet )
        {
            VxGUID::generateNewVxGUID( m_AnnounceSessionId );
            m_HostServerMgr.sendHostAnnounceToNetworkHost( m_AnnounceSessionId, m_PktHostInviteAnnounceReq, getHostAnnounceConnectReason() );
        }
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
    LogModule( eLogHosts, LOG_VERBOSE, "PluginBaseHostService PktHostJoinReq %s got PktHostJoinReq from %s", DescribeHostType( getHostType() ), netIdent->getOnlineName() );
    PktHostJoinReq * joinReq = (PktHostJoinReq *)pktHdr;
    PktHostJoinReply joinReply;
    if( joinReq->isValidPkt() )
    {
        GroupieId rxedGroupieId = joinReq->getGroupieId();
        GroupieId groupieId( netIdent->getMyOnlineId(), m_Engine.getMyOnlineId(), getHostType() );
        if( rxedGroupieId != groupieId )
        {
            LogModule( eLogHosts, LOG_DEBUG, "PluginBaseHostService %s from %s groupie %s does not match rxed %s", DescribePluginType( getPluginType() ), 
                netIdent->getOnlineName(), groupieId.describeGroupieId().c_str(), rxedGroupieId.describeGroupieId().c_str() );
        }

        joinReply.setGroupieId( groupieId );
        joinReply.setPluginType( getPluginType() );
        joinReply.setSessionId( joinReq->getSessionId() );
        joinReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );

        VxGUID sktConnectionId( sktBase->getSocketId() );

        bool broadcastPkt = false;
        bool sendPkt = false;
        if( ePluginAccessOk == joinReply.getAccessState() )
        {
            broadcastPkt = true;
        }
        else if( ePluginAccessLocked == joinReply.getAccessState() )
        {
            if( !netIdent->isIgnored() )
            {
                if( m_HostServerMgr.getJoinState( netIdent, joinReq->getHostType() ) == eJoinStateJoinWasGranted )
                {
                    broadcastPkt = true;

                    // even though friendship not high enough if admin has accepted then send accepted
                    joinReply.setAccessState( ePluginAccessOk );
                    LogModule( eLogHosts, LOG_VERBOSE, "PluginBaseHostService from %s host %s join granted", netIdent->getOnlineName(), DescribeHostType( getHostType() ) );
                }
                else
                {
                    sendPkt = true;
                    LogModule( eLogHosts, LOG_VERBOSE, "PluginBaseHostService from %s host %s join requested", netIdent->getOnlineName(), DescribeHostType( getHostType() ) );
                }
            }
            else
            {
                // TODO .. should we drop the connection or just ignore ?
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

        if( broadcastPkt )
        {
            m_Engine.getConnectIdListMgr().addConnection( sktConnectionId, groupieId, false );
            m_HostServerMgr.onUserJoined( sktBase, netIdent, joinReply.getSessionId(), joinReply.getHostType() );
            broadcastToClients( &joinReply, netIdent->getMyOnlineId(), sktBase );       
        }
        else if( sendPkt )
        {
            m_Engine.getConnectIdListMgr().addConnection( sktConnectionId, groupieId, false );
            m_HostServerMgr.onJoinRequested( sktBase, netIdent, joinReq->getSessionId(), joinReq->getHostType() );
            txPacket( netIdent->getMyOnlineId(), sktBase, &joinReply );
        }
    }
    else
    {
        LogMsg( LOG_DEBUG, "PluginBaseHostService onPktHostJoinReq Invalid Packet" );
        joinReply.setCommError( eCommErrInvalidPkt );
        onInvalidRxedPacket( sktBase, pktHdr, netIdent );   
    }
}

//============================================================================
void PluginBaseHostService::onPktHostLeaveReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    LogModule( eLogHosts, LOG_DEBUG, "PluginBaseHostService  %s got PktHostLeaveReq from %s", DescribeHostType( getHostType() ), netIdent->getOnlineName() );
    PktHostLeaveReq* pktReq = ( PktHostLeaveReq* )pktHdr;
    PktHostLeaveReply pktReply;
    if( pktReq->isValidPkt() )
    {
        GroupieId rxedGroupieId = pktReq->getGroupieId();
        GroupieId groupieId( netIdent->getMyOnlineId(), m_Engine.getMyOnlineId(), getHostType() );
        if( rxedGroupieId != groupieId )
        {
            LogModule( eLogHosts, LOG_DEBUG, "PluginBaseHostService %s from %s groupie %s does not match rxed %s", DescribePluginType( getPluginType() ),
                netIdent->getOnlineName(), groupieId.describeGroupieId().c_str(), rxedGroupieId.describeGroupieId().c_str() );
        }

        pktReply.setGroupieId( groupieId );
        pktReply.setPluginType( getPluginType() );
        pktReply.setSessionId( pktReq->getSessionId() );
        pktReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );
        
        VxGUID sktConnectionId( sktBase->getSocketId() );

        bool broadcastPkt = false;
        if( ePluginAccessOk == pktReply.getAccessState() )
        {
            bool broadcastPkt = true;
        }
        else if( ePluginAccessLocked == pktReply.getAccessState() )
        {
            if( !netIdent->isIgnored() )
            {
                if( m_HostServerMgr.getJoinState( netIdent, pktReq->getHostType() ) == eJoinStateJoinWasGranted )
                {
                    // even though friendship not high enough if admin has accepted then send accepted
                    pktReply.setAccessState( ePluginAccessOk );
                    bool broadcastPkt = true;
                }
            }
            else
            {
                // TODO .. should we drop the connection?
            }
        }
        else if( ePluginAccessDisabled == pktReply.getAccessState() )
        {
            // join request sent to disabled plugin.. this should not happen
            LogMsg( LOG_ERROR, "PluginBaseHostService %s got leave request to disabled plugin from %s", DescribeHostType( getHostType() ), netIdent->getMyOnlineUrl().c_str() );
        }
        else if( ePluginAccessIgnored == pktReply.getAccessState() )
        {
            // TODO .. should we drop the connection of ignored person?
            LogMsg( LOG_ERROR, "PluginBaseHostService %s got leave request from ignored person %s", DescribeHostType( getHostType() ), netIdent->getMyOnlineUrl().c_str() );
        }

        if( broadcastPkt )
        {
            broadcastToClients( &pktReply, netIdent->getMyOnlineId(), sktBase );
        }
        
        m_HostServerMgr.onUserLeftHost( sktBase, netIdent, pktReply.getSessionId(), pktReply.getHostType() );
        m_Engine.getConnectIdListMgr().removeConnection( sktConnectionId, groupieId );
    }
    else
    {
        LogMsg( LOG_DEBUG, "PluginBaseHostService onPktHostLeaveReq Invalid Packet" );
        pktReply.setCommError( eCommErrInvalidPkt );
        onInvalidRxedPacket( sktBase, pktHdr, netIdent );
    }
}

//============================================================================
void PluginBaseHostService::onPktHostUnJoinReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    LogModule( eLogHosts, LOG_DEBUG, "PluginBaseHostService %s got unjoin request from %s", DescribeHostType( getHostType() ), netIdent->getOnlineName() );
    PktHostUnJoinReq* joinReq = ( PktHostUnJoinReq* )pktHdr;
    PktHostUnJoinReply joinReply;
    if( joinReq->isValidPkt() )
    {
        GroupieId rxedGroupieId = joinReq->getGroupieId();
        GroupieId groupieId( netIdent->getMyOnlineId(), m_Engine.getMyOnlineId(), getHostType() );
        if( rxedGroupieId != groupieId )
        {
            LogModule( eLogHosts, LOG_DEBUG, "PluginBaseHostService %s unjoin request from %s groupie %s does not match rxed %s", DescribePluginType( getPluginType() ),
                netIdent->getOnlineName(), groupieId.describeGroupieId().c_str(), rxedGroupieId.describeGroupieId().c_str() );
        }

        joinReply.setGroupieId( groupieId );
        joinReply.setPluginType( getPluginType() );
        joinReply.setSessionId( joinReq->getSessionId() );
        joinReply.setAccessState( m_HostServerMgr.getPluginAccessState( netIdent ) );

        VxGUID sktConnectionId( sktBase->getSocketId() );

        bool broadcastPkt = false;
        if( ePluginAccessOk == joinReply.getAccessState() )
        {
            broadcastPkt = true;
        }
        else if( ePluginAccessLocked == joinReply.getAccessState() )
        {
            if( !netIdent->isIgnored() )
            {
                if( m_HostServerMgr.getJoinState( netIdent, joinReq->getHostType() ) == eJoinStateJoinWasGranted )
                {
                    // even though friendship not high enough if admin has accepted then send accepted
                    joinReply.setAccessState( ePluginAccessOk );
                    broadcastPkt = true;
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

        if( broadcastPkt )
        {
            broadcastToClients( &joinReply, netIdent->getMyOnlineId(), sktBase );
        }

        m_HostServerMgr.onUserUnJoined( sktBase, netIdent, joinReply.getSessionId(), joinReply.getHostType() );
        m_Engine.getConnectIdListMgr().removeConnection( sktConnectionId, groupieId );
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

    if( !txPacket( netIdent->getMyOnlineId(), sktBase, &searchReply, false, overridePlugin ) )
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
    LogMsg( LOG_DEBUG, "PluginBaseHostService got host offer reply" );
}

//============================================================================
void PluginBaseHostService::onPktHostInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    LogMsg( LOG_DEBUG, "PluginBaseHostService got host info request" );
    PktHostInfoReq* pktReq = ( PktHostInfoReq* )pktHdr;
    PktHostInfoReply pktReply;
    pktReply.setHostType( pktReq->getHostType() );
    pktReply.setSessionId( pktReq->getSessionId() );

    std::string hostTitle;
    std::string hostDesc;
    int64_t lastModifiedTime;
    if( m_PktHostInviteIsValid && isPluginEnabled() && m_Engine.getNetStatusAccum().getNetAvailStatus() != eNetAvailNoInternet && getHostTitleAndDescription( hostTitle,  hostDesc, lastModifiedTime ) )
    {
        pktReply.setHostTitleAndDescription( hostTitle, hostDesc, lastModifiedTime );
    }
    else
    {
        pktReply.setCommError( eCommErrPluginNotEnabled );
    }

    if( !txPacket( netIdent, sktBase, &pktReply ) )
    {
        LogMsg( LOG_DEBUG, "PluginBaseHostService failed send onPktHostInfoReq" );
    }
}

//============================================================================
void PluginBaseHostService::onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getHostedListMgr().onPktHostInfoReply( sktBase, pktHdr, netIdent, this );
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

//============================================================================
bool PluginBaseHostService::getHostTitleAndDescription( std::string& hostTitle, std::string& hostDesc, int64_t& lastModifiedTime )
{
    return m_PluginSetting.getHostTitleAndDescription( hostTitle, hostDesc, lastModifiedTime );
}

//============================================================================
void PluginBaseHostService::onPluginSettingsChanged( int64_t modifiedTimeMs )
{
    /*
    int64_t timeNow = GetGmtTimeMs();
    m_Engine.setPktAnnLastModTime( timeNow );

    m_Engine.lockAnnouncePktAccess();
    //m_Engine.getMyPktAnnounce().setHostOrThumbModifiedTime( getPluginType(), modifiedTimeMs );
    m_Engine.setPktAnnLastModTime( timeNow );
    // just time changes so other users will update when next connect.. no need to reannounce 
    m_Engine.getToGui().toGuiSaveMyIdent(m_Engine.getMyPktAnnounce().getVxNetIdent() );
    m_Engine.unlockAnnouncePktAccess();
    */
}

//============================================================================
void PluginBaseHostService::updateHostInvite( PluginSetting& pluginSetting )
{
    pluginSetting.getHostTitleAndDescription( m_HostTitle, m_HostDescription, m_HostInfoModifiedTime );

    updateHostInviteUrl();
}

//============================================================================
void PluginBaseHostService::updateHostInviteUrl( void )
{
    if( !PluginShouldAnnounceToNetwork( getPluginType() ) )
    {
        return;
    }

    if( eHostTypeUnknown == getHostType() )
    {
        LogMsg( LOG_VERBOSE, "PluginBaseHostService::updateHostInviteUrl unknown host type" );
        return;
    }

    if( m_HostInfoModifiedTime && m_Engine.getNetStatusAccum().isDirectConnectTested() && !m_Engine.getMyPktAnnounce().requiresRelay() )
    {
        std::string myOnlineUrl;

        m_Engine.lockAnnouncePktAccess();
        PktAnnounce& pktAnn = m_Engine.getMyPktAnnounce();
        myOnlineUrl = pktAnn.getMyOnlineUrl();
        pktAnn.setHostOrThumbModifiedTime( getPluginType(), m_HostInfoModifiedTime );
        m_Engine.getToGui().toGuiSaveMyIdent( pktAnn.getVxNetIdent() );
        m_Engine.unlockAnnouncePktAccess();

        m_Engine.setPktAnnLastModTime( GetGmtTimeMs() );

        std::string inviteUrl = Invite::makeInviteUrl( getHostType(), myOnlineUrl );
        if( !inviteUrl.empty() )
        {
            VxGUID sessionId;
            sessionId.initializeWithNewVxGUID();

            m_AnnMutex.lock();
            m_HostInviteUrl = inviteUrl;
            m_PktHostInviteAnnounceReq.setHostType( getHostType() );
            m_PktHostInviteAnnounceReq.setSessionId( sessionId );
            VxGUID thumbId = pktAnn.getHostThumbId( getHostType(), true );
            bool result = m_PktHostInviteAnnounceReq.setHostInviteInfo( m_HostInviteUrl, m_HostTitle, m_HostDescription, m_HostInfoModifiedTime, thumbId );
            m_AnnMutex.unlock();

            if( result )
            {
                m_PktHostInviteIsValid = true;
            }
        }
    }
    else
    {
        m_PktHostInviteIsValid = false;
    }
}

//============================================================================
ECommErr PluginBaseHostService::getCommAccessState( VxNetIdent* netIdent )
{
    ECommErr commErr{ eCommErrNone };
    if( !isPluginEnabled() )
    {
        commErr = eCommErrPluginNotEnabled;
    }
    else
    {
        EPluginAccess pluginAccess = m_HostServerMgr.getPluginAccessState( netIdent );
        if( ePluginAccessOk != pluginAccess )
        {
            commErr = eCommErrPluginPermission;
        }
    }

    return commErr;
}

//============================================================================
void PluginBaseHostService::onPktGroupieInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieInfoReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieInfoReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieAnnReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieAnnReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieSearchReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieSearchReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieMoreReq( sktBase, pktHdr, netIdent, getCommAccessState( netIdent ), this );
}

//============================================================================
void PluginBaseHostService::onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    m_Engine.getGroupieListMgr().onPktGroupieMoreReply( sktBase, pktHdr, netIdent, this );
}

//============================================================================
void PluginBaseHostService::onUserJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    m_HostServerMgr.onUserJoinedHost( groupieId, sktBase, netIdent );
}

//============================================================================
void PluginBaseHostService::onUserLeftHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    m_HostServerMgr.onUserLeftHost( groupieId, sktBase, netIdent );
}

//============================================================================
void PluginBaseHostService::onUserUnJoinedHost( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    m_HostServerMgr.onUserUnJoinedHost( groupieId, sktBase, netIdent );
}

//============================================================================
void PluginBaseHostService::onGroupDirectUserAnnounce( GroupieId& groupieId, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    m_HostServerMgr.onGroupDirectUserAnnounce( groupieId, sktBase, netIdent );
}