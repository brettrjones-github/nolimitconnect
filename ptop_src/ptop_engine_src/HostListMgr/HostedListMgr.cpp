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

#include "HostedListMgr.h"
#include "HostedListCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinMgr.h>
#include <ptop_src/ptop_engine_src/UserJoinMgr/UserJoinMgr.h>

#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBaseHostService.h>

#include <CoreLib/VxPtopUrl.h>
#include <PktLib/PktsHostInvite.h>
#include <PktLib/PktsHostInfo.h>
#include <PktLib/PktsGroupie.h>

//============================================================================
HostedListMgr::HostedListMgr( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
RCODE HostedListMgr::hostedListMgrStartup( std::string& dbFileName )
{
    RCODE rc = m_HostedInfoListDb.hostedListDbStartup( HOSTED_LIST_DB_VERSION, dbFileName.c_str() );
    m_HostedInfoList.clear();
    m_HostedInfoListDb.getAllHosteds( m_HostedInfoList );
    return rc;
}

//============================================================================
RCODE HostedListMgr::hostedListMgrShutdown( void )
{
    return m_HostedInfoListDb.hostedListDbShutdown();
}

//============================================================================
void HostedListMgr::updateHosted( EHostType hostType, VxGUID& onlineId, std::string& hosted, int64_t timestampMs )
{
    if( !onlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "HostedListMgr::updateDirectConnectIdent invalid id" );
        return;
    }

    /*
    bool wasUpdated = false;
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType && iter->getOnlineId() == onlineId )
        {
            iter->setHosted( hosted );
            if( timestampMs )
            {
                iter->setTimestamp( timestampMs );
                m_HostedInfoListDb.saveHosted( *iter );
            }
            
            wasUpdated = true;
            break;
        }
    }

    if( !wasUpdated )
    {
        HostedInfo hostedInfo( hostType, onlineId, hosted, timestampMs );
        m_HostedInfoList.push_back( hostedInfo );
        if( timestampMs )
        {
            m_HostedInfoListDb.saveHosted( hostedInfo );
        }
    }

    unlockList();
    */
}

//============================================================================
void HostedListMgr::updateHostedList( VxNetIdent* netIdent, VxSktBase* sktBase )
{
    if( !netIdent || !sktBase )
    {
        LogMsg( LOG_ERROR, "HostedListMgr::updateHosteds null netIdent or sktBase" );
        return;
    }

    if( netIdent->requiresRelay() )
    {
        removeClosedPortIdent( netIdent->getMyOnlineId() );
    }
    else
    {     
        VxGUID onlineId = netIdent->getMyOnlineId();
        if( !onlineId.isVxGUIDValid() )
        {
            LogMsg( LOG_ERROR, "HostedListMgr::updateHostedList invalid id" );
            return;
        }

        std::string nodeUrl = netIdent->getMyOnlineUrl();
        for( int i = eHostTypeUnknown + 1; i < eMaxHostType; ++i )
        {
            EHostType hostType = ( EHostType )i;
            if( netIdent->canRequestJoin( hostType ) )
            {
                updateAndRequestInfoIfNeeded( hostType, onlineId, nodeUrl, netIdent, sktBase );
            }
        }
    }
}

//============================================================================
void HostedListMgr::removeClosedPortIdent( VxGUID& onlineId )
{
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); )
    {
        if( iter->getOnlineId() == onlineId )
        {
            iter = m_HostedInfoList.erase( iter );
        }
        else
        {
            ++iter;
        }
    }

    unlockList();
    m_HostedInfoListDb.removeClosedPortIdent( onlineId );
}

//============================================================================
void HostedListMgr::removeHostedInfo( EHostType hostType, VxGUID& onlineId )
{
    bool wasRemoved{ false };
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); )
    {
        if( iter->getOnlineId() == onlineId && iter->getHostType() == hostType )
        {
            m_HostedInfoList.erase( iter );
            wasRemoved = true;
            break;
        }
        else
        {
            ++iter;
        }
    }

    unlockList();
    if( wasRemoved )
    {
        m_HostedInfoListDb.removeHostedInfo( hostType, onlineId );
        announceHostInfoRemoved( hostType, onlineId );
    }
}

//============================================================================
void HostedListMgr::clearHostedInfoList( void )
{
    m_HostedInfoList.clear();
}

//============================================================================
void HostedListMgr::addHostedListMgrClient( HostedListCallbackInterface* client, bool enable )
{
    lockClientList();
    for( auto iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
    {
        if( *iter == client )
        {
            iter = m_HostedInfoListClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_HostedInfoListClients.push_back( client );
    }

    unlockClientList();
}

//============================================================================
void HostedListMgr::announceHostInfoUpdated( HostedInfo* hostedInfo )
{
    if( hostedInfo )
    {
        lockClientList();
        std::vector<HostedListCallbackInterface*>::iterator iter;
        for( iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
        {
            HostedListCallbackInterface* client = *iter;
            client->callbackHostedInfoListUpdated( hostedInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostInfoUpdated invalid param" );
    }
}

//============================================================================
void HostedListMgr::announceHostInfoRemoved( EHostType hostType, VxGUID& hostOnlineId )
{
    // removeFromDatabase( hostOnlineId, hostType, false );
    lockClientList();
    std::vector<HostedListCallbackInterface*>::iterator iter;
    for( iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
    {
        HostedListCallbackInterface* client = *iter;
        client->callbackHostedInfoListRemoved( hostOnlineId, hostType );
    }

    unlockClientList();
}

//============================================================================
void HostedListMgr::announceHostInfoSearchResult( HostedInfo* hostedInfo, VxGUID& sessionId )
{
    if( hostedInfo )
    {
        lockClientList();
        std::vector<HostedListCallbackInterface*>::iterator iter;
        for( iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
        {
            HostedListCallbackInterface* client = *iter;
            client->callbackHostedInfoListSearchResult( hostedInfo, sessionId );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostInfoSearchResult invalid param" );
    }
}


//============================================================================
void HostedListMgr::announceHostInfoSearchComplete( EHostType hostType, VxGUID& sessionId )
{
    if( hostType != eHostTypeUnknown )
    {
        lockClientList();
        std::vector<HostedListCallbackInterface*>::iterator iter;
        for( iter = m_HostedInfoListClients.begin(); iter != m_HostedInfoListClients.end(); ++iter )
        {
            HostedListCallbackInterface* client = *iter;
            client->callbackHostedInfoListSearchComplete( hostType, sessionId );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceHostInfoSearchResult invalid param" );
    }
}

//============================================================================
void HostedListMgr::updateAndRequestInfoIfNeeded( EHostType hostType, VxGUID& onlineId, std::string& nodeUrl, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    bool requiresSendHostInfoRequest{ false };
    bool requiresAnnounceUpdate{ false };
    bool wasFound{ false };
    bool urlChanged{ false };
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter)
    {
        if( iter->getOnlineId() == onlineId && iter->getHostType() == hostType )
        {
            wasFound = true;
            iter->setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
            if( iter->getHostInfoTimestamp() < netIdent->getHostOrThumbModifiedTime( hostType ) )
            {
                requiresSendHostInfoRequest = true;
            }

            if( nodeUrl != iter->getHostInviteUrl() )
            {
                urlChanged = true;
                iter->setHostInviteUrl( nodeUrl );
                if( !requiresSendHostInfoRequest && iter->isValidForGui() )
                {
                    requiresAnnounceUpdate = true;
                }
            }       

            if( iter->shouldSaveToDb() )
            {
                if( urlChanged )
                {
                    m_HostedInfoListDb.saveHosted( *iter );
                }
                else
                {
                    m_HostedInfoListDb.updateLastConnected( hostType, onlineId, iter->getConnectedTimestamp() );
                }
            }

            if( requiresAnnounceUpdate )
            {
                announceHostInfoUpdated( &( *iter ) );
            }
        }
    }

    if( !wasFound )
    {
        requiresSendHostInfoRequest = true;
        HostedInfo hostedInfo( hostType, onlineId, nodeUrl );
        hostedInfo.setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
        m_HostedInfoList.push_back( hostedInfo );
    }

    unlockList();

    if( requiresSendHostInfoRequest )
    {
        requestHostedInfo( hostType, onlineId, netIdent, sktBase );
    }
}

//============================================================================
bool HostedListMgr::updateLastConnected( EHostType hostType, VxGUID& onlineId, int64_t lastConnectedTime )
{
    bool result{ false };
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter)
    {
        if( iter->getOnlineId() == onlineId && iter->getHostType() == hostType )
        {
            iter->setConnectedTimestamp( lastConnectedTime );
            result = true;

            if( iter->shouldSaveToDb() )
            {
                m_HostedInfoListDb.updateLastConnected( hostType, onlineId, iter->getConnectedTimestamp() );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool HostedListMgr::updateLastJoined( EHostType hostType, VxGUID& onlineId, int64_t lastJoinedTime )
{
    bool result{ false };
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter)
    {
        if( iter->getOnlineId() == onlineId && iter->getHostType() == hostType )
        {
            int64_t oldJoinedTime = iter->getJoinedTimestamp();
            iter->setJoinedTimestamp( lastJoinedTime );
            result = true;

            if( iter->shouldSaveToDb() )
            {
                m_HostedInfoListDb.updateLastConnected( hostType, onlineId, lastJoinedTime );
            }
            else if( oldJoinedTime )
            {
                m_HostedInfoListDb.removeHostedInfo( hostType, onlineId );
            }

            if( iter->isValidForGui() )
            {
                announceHostInfoUpdated( &( *iter ) );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool HostedListMgr::updateIsFavorite( EHostType hostType, VxGUID& onlineId, bool isFavorite )
{
    bool result{ false };
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter)
    {
        if( iter->getOnlineId() == onlineId && iter->getHostType() == hostType )
        {
            bool wasFavorite = iter->getIsFavorite();
            iter->setIsFavorite( isFavorite );
            result = true;

            if( iter->shouldSaveToDb() )
            {
                m_HostedInfoListDb.updateIsFavorite( hostType, onlineId, isFavorite );
            }
            else if( wasFavorite )
            {
                m_HostedInfoListDb.removeHostedInfo( hostType, onlineId );
            }

            if( iter->isValidForGui() )
            {
                announceHostInfoUpdated( &( *iter ) );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool HostedListMgr::updateHostTitleAndDescription( EHostType hostType, VxGUID& onlineId, std::string& title, std::string& description, int64_t lastDescUpdateTime, VxNetIdent* netIdent )
{
    bool result{ false };
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter)
    {
        if( iter->getOnlineId() == onlineId && iter->getHostType() == hostType )
        {
            result = true;
            iter->setHostTitle( title );
            iter->setHostDescription( description );
            iter->setHostInfoTimestamp( lastDescUpdateTime );

            if( iter->shouldSaveToDb() )
            {
                m_HostedInfoListDb.updateHostTitleAndDescription( hostType, onlineId, title, description, lastDescUpdateTime );
            }

            if( iter->isValidForGui() )
            {
                announceHostInfoUpdated( &( *iter ) );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool HostedListMgr::requestHostedInfo( EHostType hostType, VxGUID& onlineId, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    bool result{ false };
    // only hosts that announce to network respond to Host Info requests
    if( HostShouldAnnounceToNetwork( hostType ) )
    {
        PktHostInfoReq pktReq;
        pktReq.setPluginNum( ( uint8_t )HostTypeToHostPlugin( hostType ) );
        pktReq.getSessionId().initializeWithNewVxGUID();

        result = sktBase->txPacket( netIdent->getMyOnlineId(), &pktReq);
    }

    return result;
}

//============================================================================
void HostedListMgr::onPktHostInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin )
{
    bool result{ false };
    PktHostInfoReply* pktReply = (PktHostInfoReply *)pktHdr;

    VxGUID sessionId = pktReply->getSessionId();
    EHostType hostType = pktReply->getHostType();
    std::string hostTitle;
    std::string hostDesc;
    int64_t lastModifiedTime{ 0 };
    ECommErr commErr = pktReply->getCommError();
    if( eCommErrNone == commErr )
    {
        if( pktReply->getHostTitleAndDescription( hostTitle, hostDesc, lastModifiedTime ) )
        {
            if( !hostTitle.empty() && !hostDesc.empty() && lastModifiedTime )
            {
                result = true;
            }
            else
            {
                LogMsg( LOG_ERROR, "HostedListMgr::onPktHostInfoReply INVALID host info %s", netIdent->getOnlineName() );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "HostedListMgr::onPktHostInfoReply extract host info FAILED %s", netIdent->getOnlineName() );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "HostedListMgr::onPktHostInfoReply error %s %s", DescribeCommError( commErr ), netIdent->getOnlineName() );
    }


    if( result )
    {
        LogMsg( LOG_VERBOSE, "HostedListMgr::onPktHostInfoReply success title %s desc %s", hostTitle.c_str(), hostDesc.c_str() );
        updateHostTitleAndDescription( hostType, netIdent->getMyOnlineId(), hostTitle, hostDesc, lastModifiedTime, netIdent );
    }
}

//============================================================================
bool HostedListMgr::fromGuiQueryMyHostedInfo( EHostType hostType, std::vector<HostedInfo>& hostedInfoList )
{
    hostedInfoList.clear();
    PluginBase* pluginBase = m_Engine.getPluginMgr().getPlugin( HostTypeToHostPlugin( hostType ) );
    if( pluginBase && pluginBase->isPluginEnabled() )
    {
        HostedInfo hostedInfo;
        if( pluginBase->getHostedInfo( hostedInfo ) )
        {
            hostedInfoList.push_back( hostedInfo );
        }
    }

    return !hostedInfoList.empty();
}

//============================================================================
bool HostedListMgr::fromGuiQueryHostedInfoList( EHostType hostType, std::vector<HostedInfo>& hostedInfoList, VxGUID& hostIdIfNullThenAll )
{
    bool result{ false };
    hostedInfoList.clear();
    lockList();
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType )
        {
            addToListInJoinedTimestampOrder( hostedInfoList, *iter );
            hostedInfoList.push_back( *iter );
        }
    }

    unlockList();

    return !hostedInfoList.empty();
}

//============================================================================
bool HostedListMgr::fromGuiQueryHostListFromNetworkHost( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& hostIdIfNullThenAll )
{
    if( netHostUrl.isValid() )
    {
        m_SearchHostType = hostType;
        m_SearchSpecificOnlineId = hostIdIfNullThenAll;
        m_SearchSessionId.initializeWithNewVxGUID();

        // add ourself to host list if we have enabled hosting of the given host type
        EPluginType pluginType = HostTypeToHostPlugin( hostType );
        if( m_Engine.getMyPktAnnounce().isPluginEnabled( pluginType ) )
        {
            PluginBaseHostService* plugin = dynamic_cast< PluginBaseHostService*>( m_Engine.getPluginMgr().findPlugin( pluginType ));
            HostedInfo myHostedInfo;
            if( plugin && plugin->getHostedInfo( myHostedInfo ) )
            {
                // so we can work with join our own host always start out as needing request join
                GroupieId groupieId( m_Engine.getMyOnlineId(), m_Engine.getMyOnlineId(), hostType );
                m_Engine.getHostJoinMgr().changeJoinState( groupieId, eJoinStateNone );
                m_Engine.getUserJoinMgr().changeJoinState( groupieId, eJoinStateNone );
                hostSearchResult( hostType, m_SearchSessionId, m_Engine.getSktLoopback(), m_Engine.getMyPktAnnounce().getVxNetIdent(), myHostedInfo );
            }
        }

        VxSktBase* sktBase{ nullptr };
        m_Engine.getConnectionMgr().requestConnection( m_SearchSessionId, netHostUrl.getUrl(), netHostUrl.getOnlineId(), this, sktBase, eConnectReasonNetworkHostListSearch );
        return true;
    }

    return false;
}

//============================================================================
bool HostedListMgr::fromGuiQueryGroupiesFromHosted( VxPtopUrl& netHostUrl, EHostType hostType, VxGUID& onlineIdIfNullThenAll )
{
    if( netHostUrl.isValid() )
    {
        m_SearchHostType = hostType;
        m_SearchSpecificOnlineId = onlineIdIfNullThenAll;
        m_SearchSessionId.initializeWithNewVxGUID();
        VxSktBase* sktBase{ nullptr };
        EConnectReason connectReason{ eConnectReasonUnknown };
        switch( hostType )
        {
        case eHostTypeGroup:
            connectReason = eConnectReasonGroupHostedUserListSearch;
            break;
        case eHostTypeChatRoom:
            connectReason = eConnectReasonChatRoomHostedUserListSearch;
            break;
        case eHostTypeRandomConnect:
            connectReason = eConnectReasonRandomConnectHostedUserListSearch;
            break;
        default:
            break;
        }

        if( connectReason != eConnectReasonUnknown )
        {
            m_Engine.getConnectionMgr().requestConnection( m_SearchSessionId, netHostUrl.getUrl(), netHostUrl.getOnlineId(), this, sktBase, eConnectReasonGroupHostedUserListSearch );
            return true;
        }
        else
        {
            LogMsg( LOG_ERROR, "HostedListMgr::fromGuiQueryGroupiesFromHosted invalid host type" );
        }
    }

    return false;
}

//============================================================================
bool HostedListMgr::onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    if( eConnectReasonNetworkHostListSearch == connectReason )
    {
        PktHostInviteSearchReq pktReq;
        pktReq.setPluginNum( ( uint8_t )ePluginTypeHostNetwork );
        pktReq.setSrcOnlineId( m_Engine.getMyOnlineId() );
        pktReq.setSearchSessionId( sessionId );
        pktReq.setHostType( m_SearchHostType );
        pktReq.setSpecificOnlineId( m_SearchSpecificOnlineId );
        return 0 == sktBase->txPacket( onlineId, &pktReq );
    }
    else if( eConnectReasonGroupHostedUserListSearch == connectReason || eConnectReasonChatRoomHostedUserListSearch == connectReason || eConnectReasonRandomConnectHostedUserListSearch == connectReason )
    {
        EHostType hostType{ eHostTypeUnknown };
        EPluginType pluginType{ ePluginTypeInvalid };

        switch( connectReason )
        {
        case eConnectReasonGroupHostedUserListSearch:
            hostType = eHostTypeGroup;
            pluginType = ePluginTypeHostGroup;
            break;
        case eConnectReasonChatRoomHostedUserListSearch:
            hostType = eHostTypeChatRoom;
            pluginType = ePluginTypeHostChatRoom;
            break;
        case eConnectReasonRandomConnectHostedUserListSearch:
            hostType = eHostTypeRandomConnect;
            pluginType = ePluginTypeHostRandomConnect;
            break;
        default:
            break;
        }

        if( hostType != eHostTypeUnknown && pluginType != ePluginTypeInvalid )
        {
            PktGroupieSearchReq pktReq;

            pktReq.setPluginNum( ( uint8_t )pluginType );
            pktReq.setSrcOnlineId( m_Engine.getMyOnlineId() );
            pktReq.setSearchSessionId( sessionId );
            pktReq.setHostType( hostType );
            pktReq.setSpecificOnlineId( m_SearchSpecificOnlineId );
            return 0 == sktBase->txPacket( onlineId, &pktReq );
        }
        else
        {
            LogMsg( LOG_ERROR, "HostedListMgr::onContactConnected invalid param" );
        }
    }

    return false;
}

//============================================================================
void HostedListMgr::addToListInJoinedTimestampOrder( std::vector<HostedInfo>& hostedInfoList, HostedInfo& hostedInfo )
{
    bool wasInserted{ false };
    int64_t joinedTimestamp = hostedInfo.getJoinedTimestamp();
    int64_t connectedTimestamp = hostedInfo.getConnectedTimestamp();
    if( !hostedInfoList.empty() && (joinedTimestamp || connectedTimestamp) )
    {
        for( auto iter = hostedInfoList.begin(); iter != hostedInfoList.end(); ++iter )
        {
            if( joinedTimestamp )
            {
                if( joinedTimestamp > iter->getJoinedTimestamp() )
                {
                    hostedInfoList.insert( iter, hostedInfo );
                    wasInserted = true;
                }
            }
            else if( connectedTimestamp )
            {
                // skip those with joined timestamp and put those with connected timestamp in connected time order
                if( iter->getJoinedTimestamp() )
                {
                    continue;
                }
                else if( connectedTimestamp > iter->getConnectedTimestamp() )
                {
                    hostedInfoList.insert( iter, hostedInfo );
                    wasInserted = true;
                }
            }

            if( wasInserted )
            {
                break;
            }
        }
    }

    if( !wasInserted )
    {
        hostedInfoList.push_back( hostedInfo );
    }
}

//============================================================================
void HostedListMgr::hostSearchResult( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, HostedInfo& hostedInfo )
{
    HostedInfo resultInfo;
    if( updateHostInfo( hostType, hostedInfo, netIdent, sktBase, &resultInfo ) )
    {
        announceHostInfoSearchResult( &resultInfo, searchSessionId );
    }
}

//============================================================================
void HostedListMgr::hostSearchCompleted( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr )
{
    if( commErr )
    {
        LogMsg( LOG_ERROR, "HostedListMgr::hostSearchCompleted with error %s from %s", DescribeCommError( commErr ), sktBase->describeSktConnection().c_str() );
    }
    else
    {
        LogMsg( LOG_VERBOSE, "HostedListMgr::hostSearchCompleted with no errors" );
    }

    m_Engine.getConnectionMgr().doneWithConnection( searchSessionId, netIdent->getMyOnlineId(), this, eConnectReasonNetworkHostListSearch );
    announceHostInfoSearchComplete( hostType, searchSessionId );
}

//============================================================================
void HostedListMgr::onHostInviteAnnounceAdded( EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    LogMsg( LOG_VERBOSE, "HostedListMgr::onHostInviteAnnounceAdded %s from %s ", DescribeHostType( hostType), netIdent->getOnlineName() );
    updateHostInfo( hostType, hostedInfo, netIdent, sktBase );
}

//============================================================================
void HostedListMgr::onHostInviteAnnounceUpdated( EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    LogMsg( LOG_VERBOSE, "HostedListMgr::onHostInviteAnnounceUpdated %s from %s ", DescribeHostType( hostType ), netIdent->getOnlineName() );
    updateHostInfo( hostType, hostedInfo, netIdent, sktBase );
}

//============================================================================
// returns true if retHostedInfo was filled
bool HostedListMgr::updateHostInfo( EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase, HostedInfo* retResultInfo )
{
    VxPtopUrl ptopUrl( hostedInfo.getHostInviteUrl() );
    if( !ptopUrl.isValid() )
    {
        LogMsg( LOG_ERROR, "HostedListMgr::hostSearchResult INVALID url" );
        return false;
    }

    bool filledResultInfo = false;
    bool needsIdentityReq = false;
    if( !netIdent )
    {
        VxNetIdent hostIdent;
        BigListInfo* bigListInfo = m_Engine.getBigListMgr().findBigListInfo( ptopUrl.getOnlineId() );
        if( bigListInfo )
        {
            netIdent = bigListInfo->getVxNetIdent();
        }
        else
        {
            needsIdentityReq = true;
            m_Engine.getHostUrlListMgr().requestIdentity( hostedInfo.getHostInviteUrl() );
        }
    }

    LogMsg( LOG_VERBOSE, "HostedListMgr::hostSearchResult title %s desc %s time %lld host url %s", 
        hostedInfo.getHostTitle().c_str(), hostedInfo.getHostDescription().c_str(), hostedInfo.getHostInfoTimestamp(), hostedInfo.getHostInviteUrl().c_str() );

    bool alreadyExisted{ false };
    bool hostedInfoUpdated{ false };
    HostedInfo updatedHostedInfo;

    lockList();
    // if exists see if needs update
    for( auto iter = m_HostedInfoList.begin(); iter != m_HostedInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType && iter->getOnlineId() == hostedInfo.getOnlineId() )
        {
            alreadyExisted = true;
            if( sktBase )
            {
                iter->setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
            }

            if( iter->getHostInviteUrl() != hostedInfo.getHostInviteUrl() )
            {
                // url has changed. just update
                iter->setHostInviteUrl( hostedInfo.getHostInviteUrl() );
                // update our url list also
                m_Engine.getHostUrlListMgr().updateHostUrl( hostType, hostedInfo.getOnlineId(), hostedInfo.getHostInviteUrl() );
                if( iter->shouldSaveToDb() )
                {
                    m_HostedInfoListDb.updateHostUrl( iter->getHostType(), iter->getOnlineId(), hostedInfo.getHostInviteUrl() );
                }
                // TODO do we need to update if just url changed ?
            }

            if( hostedInfo.getHostInfoTimestamp() > iter->getHostInfoTimestamp() )
            {
                iter->setHostInfoTimestamp( hostedInfo.getHostInfoTimestamp() );
                iter->setHostTitle( hostedInfo.getHostTitle() );
                iter->setHostDescription( hostedInfo.getHostDescription() );
                updatedHostedInfo = *iter;
                hostedInfoUpdated = true;
                if( iter->shouldSaveToDb() )
                {
                    m_HostedInfoListDb.saveHosted( *iter );
                }
            }
            else
            {
                // in theory they should be same if we are up to date.. check anyway
                if( iter->getHostTitle() != hostedInfo.getHostTitle() || iter->getHostDescription() != hostedInfo.getHostDescription() )
                {
                    LogMsg( LOG_ERROR, "HostedListMgr::hostSearchResult title or description is different" );
                    iter->setHostTitle( hostedInfo.getHostTitle() );
                    iter->setHostDescription( hostedInfo.getHostDescription() );
                    updatedHostedInfo = *iter;
                    hostedInfoUpdated = true;
                    if( iter->shouldSaveToDb() )
                    {
                        m_HostedInfoListDb.updateHostTitleAndDescription( iter->getHostType(), iter->getOnlineId(), hostedInfo.getHostTitle(), hostedInfo.getHostDescription(), iter->getHostInfoTimestamp() );
                    }
                }
            }

            if( !needsIdentityReq && netIdent && !netIdent->canRequestJoin( hostType ) )
            {
                // clear hostedInfoUpdated.. if cannot possibly join dont announce it
                hostedInfoUpdated = false;
            }

            if( retResultInfo )
            {
                *retResultInfo = *iter;
                filledResultInfo = true;
            }

            break;
        }
    }

    if( !alreadyExisted )
    {
        if( sktBase )
        {
            hostedInfo.setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
        }

        if( retResultInfo )
        {
            *retResultInfo = hostedInfo;
            filledResultInfo = true;
        }

        m_HostedInfoList.push_back( hostedInfo );
        updatedHostedInfo = hostedInfo;
        hostedInfoUpdated = true;
    }

    unlockList();

    if( hostedInfoUpdated )
    {
        announceHostInfoUpdated( &updatedHostedInfo );
    }

    return filledResultInfo;
}