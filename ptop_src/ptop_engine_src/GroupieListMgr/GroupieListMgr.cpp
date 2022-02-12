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

#include "GroupieListMgr.h"
#include "GroupieListCallbackInterface.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/BigListLib/BigListInfo.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginMgr.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBaseHostService.h>

#include <CoreLib/VxPtopUrl.h>
#include <PktLib/PktsGroupie.h>
#include <PktLib/GroupieId.h>

//============================================================================
GroupieListMgr::GroupieListMgr( P2PEngine& engine )
    : m_Engine( engine )
{
}

//============================================================================
RCODE GroupieListMgr::groupieListMgrStartup( std::string& dbFileName )
{
    RCODE rc = m_GroupieInfoListDb.groupieListDbStartup( HOSTED_LIST_DB_VERSION, dbFileName.c_str() );
    m_GroupieInfoList.clear();
    m_GroupieInfoListDb.getAllGroupies( m_GroupieInfoList );
    return rc;
}

//============================================================================
RCODE GroupieListMgr::groupieListMgrShutdown( void )
{
    return m_GroupieInfoListDb.groupieListDbShutdown();
}

//============================================================================
void GroupieListMgr::updateGroupie( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& hosted, int64_t timestampMs )
{
    if( !groupieOnlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "GroupieListMgr::updateDirectConnectIdent invalid groupie id" );
        return;
    }

    if( !hostOnlineId.isVxGUIDValid() )
    {
        LogMsg( LOG_ERROR, "GroupieListMgr::updateDirectConnectIdent invalid host id" );
        return;
    }

    /*
    bool wasUpdated = false;
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType && iter->getOnlineId() == onlineId )
        {
            iter->setGroupie( hosted );
            if( timestampMs )
            {
                iter->setTimestamp( timestampMs );
                m_GroupieInfoListDb.saveGroupie( *iter );
            }
            
            wasUpdated = true;
            break;
        }
    }

    if( !wasUpdated )
    {
        GroupieInfo groupieInfo( hostType, onlineId, hosted, timestampMs );
        m_GroupieInfoList.push_back( groupieInfo );
        if( timestampMs )
        {
            m_GroupieInfoListDb.saveGroupie( groupieInfo );
        }
    }

    unlockList();
    */
}

//============================================================================
void GroupieListMgr::updateGroupieList( VxNetIdent* netIdent, VxSktBase* sktBase )
{
    if( !netIdent || !sktBase )
    {
        LogMsg( LOG_ERROR, "GroupieListMgr::updateGroupies null netIdent or sktBase" );
        return;
    }

    /*
    if( netIdent->requiresRelay() )
    {
        removeClosedPortIdent( netIdent->getMyOnlineId() );
    }
    else
    {     
        VxGUID onlineId = netIdent->getMyOnlineId();
        if( !onlineId.isVxGUIDValid() )
        {
            LogMsg( LOG_ERROR, "GroupieListMgr::updateGroupieList invalid id" );
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
    */
}

//============================================================================
void GroupieListMgr::removeGroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
    bool wasRemoved{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostedOnlineId() == hostOnlineId && iter->getHostType() == hostType )
        {
            m_GroupieInfoList.erase( iter );
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
        m_GroupieInfoListDb.removeGroupieInfo( groupieOnlineId, hostOnlineId, hostType );
        announceGroupieInfoRemoved( groupieOnlineId, hostOnlineId, hostType );
    }
}

//============================================================================
void GroupieListMgr::clearGroupieInfoList( void )
{
    m_GroupieInfoList.clear();
}

//============================================================================
void GroupieListMgr::addGroupieListMgrClient( GroupieListCallbackInterface* client, bool enable )
{
    lockClientList();
    for( auto iter = m_GroupieInfoListClients.begin(); iter != m_GroupieInfoListClients.end(); ++iter )
    {
        if( *iter == client )
        {
            m_GroupieInfoListClients.erase( iter );
            break;
        }
    }

    if( enable )
    {
        m_GroupieInfoListClients.push_back( client );
    }

    unlockClientList();
}

//============================================================================
void GroupieListMgr::announceGroupieInfoUpdated( GroupieInfo* groupieInfo )
{
    if( groupieInfo )
    {
        lockClientList();
        std::vector<GroupieListCallbackInterface*>::iterator iter;
        for( iter = m_GroupieInfoListClients.begin(); iter != m_GroupieInfoListClients.end(); ++iter )
        {
            GroupieListCallbackInterface* client = *iter;
            client->callbackGroupieInfoListUpdated( groupieInfo );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceGroupieInfoUpdated invalid param" );
    }
}

//============================================================================
void GroupieListMgr::announceGroupieInfoRemoved( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType )
{
    // removeFromDatabase( hostOnlineId, hostType, false );
    lockClientList();
    std::vector<GroupieListCallbackInterface*>::iterator iter;
    for( iter = m_GroupieInfoListClients.begin(); iter != m_GroupieInfoListClients.end(); ++iter )
    {
        GroupieListCallbackInterface* client = *iter;
        client->callbackGroupieInfoListRemoved( groupieOnlineId, hostOnlineId, hostType );
    }

    unlockClientList();
}

//============================================================================
void GroupieListMgr::announceGroupieInfoSearchResult( GroupieInfo* groupieInfo, VxGUID& sessionId )
{
    if( groupieInfo )
    {
        lockClientList();
        std::vector<GroupieListCallbackInterface*>::iterator iter;
        for( iter = m_GroupieInfoListClients.begin(); iter != m_GroupieInfoListClients.end(); ++iter )
        {
            GroupieListCallbackInterface* client = *iter;
            client->callbackGroupieInfoListSearchResult( groupieInfo, sessionId );
        }

        unlockClientList();
    }
    else
    {
        LogMsg( LOG_ERROR, "HostJoinMgr::announceGroupieInfoSearchResult invalid param" );
    }
}

//============================================================================
void GroupieListMgr::updateAndRequestInfoIfNeeded( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& nodeUrl, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    bool requiresSendGroupieInfoRequest{ false };
    bool requiresAnnounceUpdate{ false };
    bool wasFound{ false };
    bool urlChanged{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostedOnlineId() == hostOnlineId && iter->getHostType() == hostType )
        {
            wasFound = true;
            iter->setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
            if( iter->getGroupieInfoTimestamp() < netIdent->getHostOrThumbModifiedTime( hostType ) )
            {
                requiresSendGroupieInfoRequest = true;
            }

            if( nodeUrl != iter->getGroupieUrl() )
            {
                urlChanged = true;
                iter->setGroupieUrl( nodeUrl );
                if( !requiresSendGroupieInfoRequest && iter->isValidForGui() )
                {
                    requiresAnnounceUpdate = true;
                }
            }       

            if( iter->shouldSaveToDb() )
            {
                if( urlChanged )
                {
                    m_GroupieInfoListDb.saveGroupie( *iter );
                }
                else
                {
                    m_GroupieInfoListDb.updateLastConnected( groupieOnlineId, hostOnlineId, hostType, iter->getConnectedTimestamp() );
                }
            }

            if( requiresAnnounceUpdate )
            {
                announceGroupieInfoUpdated( &( *iter ) );
            }
        }
    }

    if( !wasFound )
    {
        requiresSendGroupieInfoRequest = true;
        GroupieInfo groupieInfo( groupieOnlineId, hostOnlineId, hostType, nodeUrl );
        groupieInfo.setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
        m_GroupieInfoList.push_back( groupieInfo );
    }

    unlockList();

    if( requiresSendGroupieInfoRequest )
    {
        requestGroupieInfo( groupieOnlineId, hostOnlineId, hostType, netIdent, sktBase );
    }
}

//============================================================================
bool GroupieListMgr::updateLastConnected( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastConnectedTime )
{
    bool result{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostedOnlineId() == hostOnlineId && iter->getHostType() == hostType )
        {
            iter->setConnectedTimestamp( lastConnectedTime );
            result = true;

            if( iter->shouldSaveToDb() )
            {
                m_GroupieInfoListDb.updateLastConnected( groupieOnlineId, hostOnlineId, hostType, iter->getConnectedTimestamp() );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool GroupieListMgr::updateLastJoined( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, int64_t lastJoinedTime )
{
    bool result{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostedOnlineId() == hostOnlineId && iter->getHostType() == hostType )
        {
            int64_t oldJoinedTime = iter->getJoinedTimestamp();
            iter->setJoinedTimestamp( lastJoinedTime );
            result = true;

            if( iter->shouldSaveToDb() )
            {
                m_GroupieInfoListDb.updateLastConnected( groupieOnlineId, hostOnlineId, hostType, lastJoinedTime );
            }
            else if( oldJoinedTime )
            {
                m_GroupieInfoListDb.removeGroupieInfo( groupieOnlineId, hostOnlineId, hostType );
            }

            if( iter->isValidForGui() )
            {
                announceGroupieInfoUpdated( &( *iter ) );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool GroupieListMgr::updateIsFavorite( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, bool isFavorite )
{
    bool result{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostedOnlineId() == hostOnlineId && iter->getHostType() == hostType )
        {
            bool wasFavorite = iter->getIsFavorite();
            iter->setIsFavorite( isFavorite );
            result = true;

            if( iter->shouldSaveToDb() )
            {
                m_GroupieInfoListDb.updateIsFavorite( groupieOnlineId, hostOnlineId, hostType, isFavorite );
            }
            else if( wasFavorite )
            {
                m_GroupieInfoListDb.removeGroupieInfo( groupieOnlineId, hostOnlineId, hostType );
            }

            if( iter->isValidForGui() )
            {
                announceGroupieInfoUpdated( &( *iter ) );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool GroupieListMgr::updateGroupieUrlAndTitleAndDescription( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, std::string& title, std::string& description, int64_t lastDescUpdateTime, VxNetIdent* netIdent )
{
    bool result{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostedOnlineId() == hostOnlineId && iter->getHostType() == hostType )
        {
            result = true;
            iter->setGroupieTitle( title );
            iter->setGroupieDescription( description );
            iter->setGroupieInfoTimestamp( lastDescUpdateTime );

            if( iter->shouldSaveToDb() )
            {
                m_GroupieInfoListDb.updateGroupieTitleAndDescription( groupieOnlineId, hostOnlineId, hostType, title, description, lastDescUpdateTime );
            }

            if( iter->isValidForGui() )
            {
                announceGroupieInfoUpdated( &( *iter ) );
            }
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool GroupieListMgr::requestGroupieInfo( VxGUID& groupieOnlineId, VxGUID& hostOnlineId, EHostType hostType, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    bool result{ false };
    // only hosts that announce to network respond to Host Info requests
    if( HostShouldAnnounceToNetwork( hostType ) )
    {
        PktGroupieInfoReq pktReq;
        pktReq.setHostType( hostType );
        pktReq.getSessionId().initializeWithNewVxGUID();

        result = sktBase->txPacket( netIdent->getMyOnlineId(), &pktReq);
    }

    return result;
}

//============================================================================
bool GroupieListMgr::fromGuiQueryMyGroupieInfo( EHostType hostType, std::vector<GroupieInfo>& groupieInfoList )
{
    groupieInfoList.clear();
    PluginBase* pluginBase = m_Engine.getPluginMgr().getPlugin( HostTypeToHostPlugin( hostType ) );
    if( pluginBase && pluginBase->isPluginEnabled() )
    {
        GroupieInfo groupieInfo;
        if( pluginBase->getGroupieInfo( groupieInfo ) )
        {
            groupieInfoList.push_back( groupieInfo );
        }
    }

    return !groupieInfoList.empty();
}

//============================================================================
bool GroupieListMgr::fromGuiQueryGroupieInfoList( EHostType hostType, std::vector<GroupieInfo>& groupieInfoList, VxGUID& hostIdIfNullThenAll )
{
    bool result{ false };
    groupieInfoList.clear();
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getHostType() == hostType )
        {
            addToListInJoinedTimestampOrder( groupieInfoList, *iter );
            groupieInfoList.push_back( *iter );
        }
    }

    unlockList();

    return !groupieInfoList.empty();
}

//============================================================================
bool GroupieListMgr::onContactConnected( VxGUID& sessionId, VxSktBase* sktBase, VxGUID& onlineId, EConnectReason connectReason )
{
    if( eConnectReasonNetworkHostListSearch == connectReason )
    {
        PktGroupieSearchReq pktReq;
        pktReq.setPluginNum( ( uint8_t )ePluginTypeHostNetwork );
        pktReq.setSrcOnlineId( m_Engine.getMyOnlineId() );
        pktReq.setSearchSessionId( sessionId );
        pktReq.setHostType( m_SearchHostType );
        pktReq.setSpecificOnlineId( m_SearchSpecificOnlineId );
        return 0 == sktBase->txPacket( onlineId, &pktReq );
    }
    else if( eConnectReasonGroupGroupieUserListSearch == connectReason || eConnectReasonChatRoomGroupieUserListSearch == connectReason || eConnectReasonRandomConnectGroupieUserListSearch == connectReason )
    {
        EHostType hostType{ eHostTypeUnknown };
        EPluginType pluginType{ ePluginTypeInvalid };

        switch( connectReason )
        {
        case eConnectReasonGroupGroupieUserListSearch:
            hostType = eHostTypeGroup;
            pluginType = ePluginTypeHostGroup;
            break;
        case eConnectReasonChatRoomGroupieUserListSearch:
            hostType = eHostTypeChatRoom;
            pluginType = ePluginTypeHostChatRoom;
            break;
        case eConnectReasonRandomConnectGroupieUserListSearch:
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
            LogMsg( LOG_ERROR, "GroupieListMgr::onContactConnected invalid param" );
        }
    }

    return false;
}

//============================================================================
void GroupieListMgr::addToListInJoinedTimestampOrder( std::vector<GroupieInfo>& groupieInfoList, GroupieInfo& groupieInfo )
{
    bool wasInserted{ false };
    int64_t joinedTimestamp = groupieInfo.getJoinedTimestamp();
    int64_t connectedTimestamp = groupieInfo.getConnectedTimestamp();
    if( !groupieInfoList.empty() && (joinedTimestamp || connectedTimestamp) )
    {
        for( auto iter = groupieInfoList.begin(); iter != groupieInfoList.end(); ++iter )
        {
            if( joinedTimestamp )
            {
                if( joinedTimestamp > iter->getJoinedTimestamp() )
                {
                    groupieInfoList.insert( iter, groupieInfo );
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
                    groupieInfoList.insert( iter, groupieInfo );
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
        groupieInfoList.push_back( groupieInfo );
    }
}

//============================================================================
void GroupieListMgr::groupieSearchResult( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, GroupieInfo& groupieInfo )
{
    GroupieInfo resultInfo;
    if( updateGroupieInfo( hostType, groupieInfo, netIdent, sktBase, &resultInfo ) )
    {
        announceGroupieInfoSearchResult( &resultInfo, searchSessionId );
    }
}

//============================================================================
void GroupieListMgr::groupieSearchCompleted( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr )
{
    if( commErr )
    {
        LogMsg( LOG_ERROR, "GroupieListMgr::hostSearchCompleted with error %s from %s", DescribeCommError( commErr ), sktBase->describeSktConnection().c_str() );
    }
    else
    {
        LogMsg( LOG_VERBOSE, "GroupieListMgr::hostSearchCompleted with no errors" );
    }

    m_Engine.getConnectionMgr().doneWithConnection( searchSessionId, netIdent->getMyOnlineId(), this, eConnectReasonNetworkHostListSearch );
}

//============================================================================
void GroupieListMgr::onGroupieAnnounceAdded( EHostType hostType, GroupieInfo& groupieInfo, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    LogMsg( LOG_VERBOSE, "GroupieListMgr::onGroupieAnnounceAdded %s from %s ", DescribeHostType( hostType), netIdent->getOnlineName() );
    updateGroupieInfo( hostType, groupieInfo, netIdent, sktBase );
}

//============================================================================
void GroupieListMgr::onGroupieAnnounceUpdated( EHostType hostType, GroupieInfo& groupieInfo, VxNetIdent* netIdent, VxSktBase* sktBase )
{
    LogMsg( LOG_VERBOSE, "GroupieListMgr::onGroupieAnnounceUpdated %s from %s ", DescribeHostType( hostType ), netIdent->getOnlineName() );
    updateGroupieInfo( hostType, groupieInfo, netIdent, sktBase );
}

//============================================================================
// returns true if retGroupieInfo was filled
bool GroupieListMgr::updateGroupieInfo( EHostType hostType, GroupieInfo& groupieInfo, VxNetIdent* netIdent, VxSktBase* sktBase, GroupieInfo* retResultInfo )
{
    VxPtopUrl ptopUrl( groupieInfo.getGroupieUrl() );
    if( !ptopUrl.isValid() )
    {
        LogMsg( LOG_ERROR, "GroupieListMgr::hostSearchResult INVALID url" );
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
            m_Engine.getHostUrlListMgr().requestIdentity( groupieInfo.getGroupieUrl() );
        }
    }

    LogMsg( LOG_VERBOSE, "GroupieListMgr::hostSearchResult title %s desc %s time %lld host url %s", 
        groupieInfo.getGroupieTitle().c_str(), groupieInfo.getGroupieDescription().c_str(), groupieInfo.getGroupieInfoTimestamp(), groupieInfo.getGroupieUrl().c_str() );

    bool alreadyExisted{ false };
    bool groupieInfoUpdated{ false };
    GroupieInfo updatedGroupieInfo;

    lockList();
    // if exists see if needs update
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->getGroupieId() == groupieInfo.getGroupieId() )
        {
            alreadyExisted = true;
            if( sktBase )
            {
                iter->setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
            }

            if( iter->getGroupieUrl() != groupieInfo.getGroupieUrl() )
            {
                // url has changed. just update
                iter->setGroupieUrl( groupieInfo.getGroupieUrl() );
                // update our url list also
                //m_Engine.getHostUrlListMgr().updateHostUrl( hostType, groupieInfo.getOnlineId(), groupieInfo.getGroupieUrl() );
                if( iter->shouldSaveToDb() )
                {
                    m_GroupieInfoListDb.updateGroupieUrl( iter->getGroupieOnlineId(), iter->getHostedOnlineId(), iter->getHostType(), groupieInfo.getGroupieUrl() );
                }
                // TODO do we need to update if just url changed ?
            }

            if( iter->getGroupieInfoTimestamp() > groupieInfo.getGroupieInfoTimestamp() )
            {
                iter->setGroupieInfoTimestamp( groupieInfo.getGroupieInfoTimestamp() );
                iter->setGroupieTitle( groupieInfo.getGroupieTitle() );
                iter->setGroupieDescription( groupieInfo.getGroupieDescription() );
                updatedGroupieInfo = *iter;
                groupieInfoUpdated = true;
                if( iter->shouldSaveToDb() )
                {
                    m_GroupieInfoListDb.saveGroupie( *iter );
                }
            }
            else
            {
                // in theory they should be same if we are up to date.. check anyway
                if( iter->getGroupieTitle() != groupieInfo.getGroupieTitle() || iter->getGroupieDescription() != groupieInfo.getGroupieDescription() )
                {
                    LogMsg( LOG_ERROR, "GroupieListMgr::hostSearchResult title or description is different" );
                    iter->setGroupieTitle( groupieInfo.getGroupieTitle() );
                    iter->setGroupieDescription( groupieInfo.getGroupieDescription() );
                    updatedGroupieInfo = *iter;
                    groupieInfoUpdated = true;
                    if( iter->shouldSaveToDb() )
                    {
                        m_GroupieInfoListDb.updateGroupieTitleAndDescription( iter->getGroupieId(), groupieInfo.getGroupieTitle(), groupieInfo.getGroupieDescription(), iter->getGroupieInfoTimestamp() );
                    }
                }
            }

            if( !needsIdentityReq && netIdent && !netIdent->canRequestJoin( hostType ) )
            {
                // clear groupieInfoUpdated.. if cannot possibly join dont announce it
                groupieInfoUpdated = false;
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
            groupieInfo.setConnectedTimestamp( sktBase->getLastActiveTimeMs() );
        }

        if( retResultInfo )
        {
            *retResultInfo = groupieInfo;
            filledResultInfo = true;
        }

        m_GroupieInfoList.push_back( groupieInfo );
        updatedGroupieInfo = groupieInfo;
        groupieInfoUpdated = true;
    }

    unlockList();

    if( groupieInfoUpdated )
    {
        announceGroupieInfoUpdated( &updatedGroupieInfo );
    }

    return filledResultInfo;
}


//============================================================================
void GroupieListMgr::onHostJoinedByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    GroupieId groupieId( netIdent->getMyOnlineId(), m_Engine.getMyOnlineId(), sessionInfo.getHostType() );

    std::string groupieUrl = netIdent->getMyOnlineUrl();
    std::string groupieTitle = netIdent->getOnlineName();
    std::string groupieDesc = netIdent->getOnlineDescription();

    GroupieInfo groupieInfo( groupieId, groupieUrl, groupieTitle, groupieDesc, GetGmtTimeMs() );
    GroupieInfo retResultInfo;
    updateGroupieInfo( sessionInfo.getHostType(), groupieInfo, netIdent, sktBase, &retResultInfo );
}

//============================================================================
void GroupieListMgr::onHostLeftByUser( VxSktBase* sktBase, VxNetIdent* netIdent, BaseSessionInfo& sessionInfo )
{
    // probably could be removed. host join state is managed by HostJoinMgr
}

//============================================================================
void GroupieListMgr::onPktGroupieInfoReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin )
{
    LogMsg( LOG_VERBOSE, "PluginBaseHostService got groupie info request" );
    PktGroupieInfoReq* pktReq = ( PktGroupieInfoReq* )pktHdr;
    PktGroupieInfoReply pktReply;
    pktReply.setHostType( pktReq->getHostType() );
    pktReply.setSessionId( pktReq->getSessionId() );
    if( eCommErrNone == commErr )
    {
        std::string groupieUrl;
        std::string groupieTitle;
        std::string groupieDesc;
        int64_t lastModifiedTime;
        VxGUID groupieOnlineId = pktReq->getGroupieOnlineId();
        GroupieId groupieId( groupieOnlineId, m_Engine.getMyOnlineId(), pktReq->getHostType() );
        bool foundGroupie{ false };
        if( groupieId.isValid() && getGroupieUrlAndTitleAndDescription( groupieId, groupieUrl, groupieTitle, groupieDesc, lastModifiedTime ) )
        {
            foundGroupie = pktReply.setGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, lastModifiedTime );
        }

        if( !foundGroupie )
        {
            commErr = eCommErrNotFound;
        }
    }

    pktReply.setCommError( commErr );

    if( !plugin->txPacket( netIdent, sktBase, &pktReply ) )
    {
        LogMsg( LOG_DEBUG, "PluginBaseHostService failed send onPktHostInfoReq" );
    }
}

//============================================================================
void GroupieListMgr::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin )
{
    bool result{ false };
    PktGroupieInfoReply* pktReply = ( PktGroupieInfoReply* )pktHdr;

    VxGUID sessionId = pktReply->getSessionId();
    EHostType hostType = pktReply->getHostType();
    std::string groupieUrl;
    VxGUID groupieOnlineId;
    std::string groupieTitle;
    std::string groupieDesc;
    int64_t lastModifiedTime{ 0 };
    ECommErr commErr = pktReply->getCommError();
    if( eCommErrNone == commErr )
    {
        if( pktReply->getGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, lastModifiedTime ) )
        {
            VxPtopUrl ptopUrl( groupieUrl );
            if( ptopUrl.isValid() )
            {
                groupieOnlineId = ptopUrl.getOnlineId();
                if( !groupieTitle.empty() && !groupieDesc.empty() && lastModifiedTime )
                {
                    GroupieId groupieId( groupieOnlineId, netIdent->getMyOnlineId(), hostType );
                    if( groupieId.isValid() && setGroupieUrlAndTitleAndDescription( groupieId, groupieUrl, groupieTitle, groupieDesc, lastModifiedTime ) )
                    {
                        result = true;
                    }              
                }
                else
                {
                    LogMsg( LOG_ERROR, "GroupieListMgr::onPktGroupieInfoReply INVALID host info %s", netIdent->getOnlineName() );
                }
            }
            else
            {
                LogMsg( LOG_ERROR, "GroupieListMgr::onPktGroupieInfoReply INVALID url%s", netIdent->getOnlineName() );
            }
        }
        else
        {
            LogMsg( LOG_ERROR, "GroupieListMgr::onPktGroupieInfoReply extract host info FAILED %s", netIdent->getOnlineName() );
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GroupieListMgr::onPktGroupieInfoReply error %s %s", DescribeCommError( commErr ), netIdent->getOnlineName() );
    }


    if( result )
    {
        LogMsg( LOG_VERBOSE, "GroupieListMgr::onPktGroupieInfoReply success title %s desc %s", groupieTitle.c_str(), groupieDesc.c_str() );

        updateGroupieUrlAndTitleAndDescription( groupieOnlineId, netIdent->getMyOnlineId(), hostType, groupieTitle, groupieDesc, lastModifiedTime, netIdent );
    }
}

//============================================================================
void GroupieListMgr::onPktGroupieAnnReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin )
{
    PktGroupieAnnounceReq* pktReq = ( PktGroupieAnnounceReq* )pktHdr;
    PktGroupieAnnounceReply pktReply;
    pktReply.setHostType( pktReq->getHostType() );
    pktReply.setCommError( commErr );
    if( eCommErrNone == commErr )
    {
        std::string groupieUrl;
        std::string groupieTitle;
        std::string groupieDesc;
        int64_t timeModified{ 0 };

        if( pktReq->getGroupieInfo( groupieUrl, groupieTitle, groupieDesc, timeModified ) )
        {
            VxPtopUrl ptopUrl( groupieUrl );
            if( ptopUrl.isValid() && !groupieUrl.empty() && !groupieTitle.empty() && !groupieDesc.empty() && timeModified )
            {
                GroupieId groupieId( ptopUrl.getOnlineId(), m_Engine.getMyOnlineId(), pktReq->getHostType() );
                if( groupieId.isValid() )
                {
                    setGroupieUrlAndTitleAndDescription( groupieId, groupieUrl, groupieTitle, groupieDesc, timeModified );
                }
            }
            else
            {
                pktReply.setCommError( eCommErrInvalidPkt );
            }
        }
        else
        {
            pktReply.setCommError( eCommErrInvalidPkt );
        }
    }

    if( !plugin->txPacket( netIdent->getMyOnlineId(), sktBase, &pktReply, false, plugin->getClientPluginType() ) )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "GroupieListMgr::onPktGroupieAnnReq failed send reply" );
    }
}

//============================================================================
void GroupieListMgr::onPktGroupieAnnReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin )
{
    // nothing to do I think
}

//============================================================================
void GroupieListMgr::onPktGroupieSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin )
{
    LogMsg( LOG_DEBUG, "PluginBaseHostService onPktGroupieSearchReq" );
    PktGroupieSearchReq* pktReq = ( PktGroupieSearchReq* )pktHdr;
    EHostType hostType = pktReq->getHostType();
    PktGroupieSearchReply pktReply;
    pktReply.setSearchSessionId( pktReq->getSearchSessionId() );
    pktReply.setHostType( hostType );
    pktReply.setCommError( commErr );
    if( eCommErrNone == commErr && pktReq->isValidPkt() )
    {
        pktReply.getBlobEntry().resetWrite();

        std::string searchText;
        if( pktReq->getSearchText( searchText ) && !searchText.empty() )
        {
            // search by text
            lockList();
            for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
            {
                if( iter->getHostType() != hostType )
                {
                    continue;
                }

                if( iter->isSearchTextMatch( searchText ) )
                {
                    std::string groupieUrl;
                    std::string groupieTitle;
                    std::string groupieDesc;
                    int64_t timeModified{ 0 };

                    if( iter->getGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, timeModified ) )
                    {
                        if( !pktReply.addGroupieInfo( groupieUrl, groupieTitle, groupieDesc, timeModified ) )
                        {
                            LogModule( eLogHostSearch, LOG_DEBUG, "GroupieListMgr::onPktGroupieSearchReq search text full count %d", pktReply.getGroupieCountThisPkt() );
                            // normally would set next online id so client can ask for more but because is a text search we just quit when a single packet is full
                            // this is so client side does not have to remember search params between searches
                            // this might be changed in the future
                            break;
                        }
                    }
                    else
                    {
                        LogModule( eLogHostSearch, LOG_DEBUG, "GroupieListMgr::onPktGroupieSearchReq search text failed extract groupie info" );
                    }
                }
            }

            unlockList();
        }
        else if( pktReq->getSpecificOnlineId().isVxGUIDValid() )
        {
            // search specific user
            GroupieId groupieId( pktReq->getSpecificOnlineId(), m_Engine.getMyOnlineId(), pktReq->getHostType() );
            bool foundUser{ false };
            if( groupieId.isValid() )
            {
                lockList();
                for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
                {
                    if( iter->getHostType() != hostType )
                    {
                        continue;
                    }

                    if( iter->isIdMatch( groupieId ) )
                    {
                        std::string groupieUrl;
                        std::string groupieTitle;
                        std::string groupieDesc;
                        int64_t timeModified{ 0 };

                        foundUser = iter->getGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, timeModified );
                        if( foundUser )
                        {
                            foundUser = pktReply.addGroupieInfo( groupieUrl, groupieTitle, groupieDesc, timeModified );
                        }
                   
                        break;
                    }
                }

                unlockList();
                if( !foundUser )
                {
                    LogModule( eLogHostSearch, LOG_DEBUG, "GroupieListMgr::onPktGroupieSearchReq user not found" );
                    pktReply.setCommError( eCommErrNotFound );
                }
            }
            else
            {
                LogModule( eLogHostSearch, LOG_DEBUG, "GroupieListMgr::onPktGroupieSearchReq invalid groupieId" );
            }
        }
        else
        {
            // all users
            lockList();
            PktBlobEntry& blobEntry = pktReply.getBlobEntry();
            for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
            {
                if( iter->getHostType() != hostType )
                {
                    continue;
                }

                int spaceRequired = iter->getSearchBlobSpaceRequirement();
                if( blobEntry.haveRoom( spaceRequired ) )
                {
                    std::string groupieUrl;
                    std::string groupieTitle;
                    std::string groupieDesc;
                    int64_t timeModified{ 0 };

                    if( iter->getGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, timeModified ) )
                    {
                        pktReply.addGroupieInfo( groupieUrl, groupieTitle, groupieDesc, timeModified );
                    }
                }
                else
                {
                    pktReply.setMoreGroupiesExist( true );
                    pktReply.setNextSearchOnlineId( iter->getGroupieOnlineId() );
                    break;
                }
            }

            unlockList();
        }
    }

    pktReply.calcPktLen();
    if( !plugin->txPacket( netIdent->getMyOnlineId(), sktBase, &pktReply ) )
    {
        LogModule( eLogHostSearch, LOG_DEBUG, "GroupieListMgr::onPktGroupieSearchReq failed send search reply" );
    }
}

//============================================================================
void GroupieListMgr::onPktGroupieSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin )
{
    PktGroupieSearchReply* pktReply = ( PktGroupieSearchReply* )pktHdr;
    if( pktReply->getCommError() )
    {
        logCommError( pktReply->getCommError(), "PktGroupieSearchReply", sktBase, netIdent );
        groupieSearchCompleted( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getCommError() );
    }
    else
    {
        updateFromGroupieSearchBlob( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getBlobEntry(), pktReply->getGroupieCountThisPkt() );
        if( pktReply->getMoreGroupiesExist() )
        {
            if( !requestMoreGroupiesFromHost( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getNextSearchOnlineId(), plugin ) )
            {
                groupieSearchCompleted( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrNone );
            }
        }
        else
        {
            groupieSearchCompleted( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrNone );
        }
    }
}

//============================================================================
void GroupieListMgr::onPktGroupieMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, ECommErr commErr, PluginBaseHostService* plugin )
{
    PktGroupieMoreReq* pktReq = ( PktGroupieMoreReq* )pktHdr;
    PktGroupieMoreReply pktReply;
    EHostType hostType = pktReq->getHostType();
    VxGUID nextSearchOnlineId = pktReq->getNextSearchOnlineId();

    pktReply.setHostType( hostType );
    pktReply.setSearchSessionId( pktReq->getSearchSessionId() );
    pktReply.setCommError( commErr );

    if( eCommErrNone == commErr )
    {
        bool foundNextId{ false };

        lockList();
        for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
        {
            if( iter->getHostType() != hostType )
            {
                continue;
            }

            if( !foundNextId && iter->getGroupieOnlineId() == nextSearchOnlineId )
            {
                foundNextId = true;
            }

            if( foundNextId )
            {
                if( iter->fillSearchBlob( pktReply.getBlobEntry() ) )
                {
                    pktReply.incrementGroupieCount();
                }
                else
                {
                    pktReply.setMoreGroupiesExist( true );
                    pktReply.setNextSearchOnlineId( iter->getGroupieOnlineId() );
                    break;
                }
            }
        }

        unlockList();
    }

    pktReply.calcPktLen();
    plugin->txPacket( netIdent, sktBase, &pktReply );
}

//============================================================================
void GroupieListMgr::onPktGroupieMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent, PluginBase* plugin )
{
    PktGroupieMoreReply* pktReply = ( PktGroupieMoreReply* )pktHdr;
    if( pktReply->getCommError() )
    {
        logCommError( pktReply->getCommError(), "PktHostInviteSearchReply", sktBase, netIdent );
        groupieSearchCompleted( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getCommError() );
    }
    else
    {
        updateFromGroupieSearchBlob( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getBlobEntry(), pktReply->getGroupieCountThisPkt() );
        if( pktReply->getMoreGroupiesExist() )
        {
            if( !requestMoreGroupiesFromHost( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, pktReply->getNextSearchOnlineId(), plugin ) )
            {
                groupieSearchCompleted( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrNone );
            }
        }
        else
        {
            groupieSearchCompleted( pktReply->getHostType(), pktReply->getSearchSessionId(), sktBase, netIdent, eCommErrNone );
        }
    }
}

//============================================================================
bool GroupieListMgr::setGroupieUrlAndTitleAndDescription( GroupieId& groupieId, std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    bool result{ false };
    bool foundGroupie{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->isIdMatch( groupieId ) )
        {
            result = iter->setGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, lastModifiedTime );
            foundGroupie = true;
            break;
        }
    }

    if( !foundGroupie )
    {
        GroupieInfo groupieInfo( groupieId, groupieUrl, groupieTitle, groupieDesc, lastModifiedTime );
        if( groupieInfo.isValidForGui() )
        {
            m_GroupieInfoList.push_back( groupieInfo );
            result = true;
        }
    }

    unlockList();
    return result;
}

//============================================================================
bool GroupieListMgr::getGroupieUrlAndTitleAndDescription( GroupieId& groupieId, std::string& groupieUrl, std::string& groupieTitle, std::string& groupieDesc, int64_t& lastModifiedTime )
{
    bool result{ false };
    lockList();
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
    {
        if( iter->isIdMatch( groupieId ) )
        {
            result = iter->getGroupieUrlAndTitleAndDescription( groupieUrl, groupieTitle, groupieDesc, lastModifiedTime );
            break;
        }
    }

    unlockList();
    return result;
}

//============================================================================
void GroupieListMgr::logCommError( ECommErr commErr, const char* desc, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    LogMsg( LOG_ERROR, "%s %s from %s %s", desc, DescribeCommError( commErr ), netIdent->getOnlineName(), sktBase->describeSktConnection().c_str() );
}

//============================================================================
void GroupieListMgr::updateFromGroupieSearchBlob( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, PktBlobEntry& blobEntry, int hostInfoCount )
{
    blobEntry.resetRead();
    for( int i = 0; i < hostInfoCount; i++ )
    {
        GroupieInfo groupieInfo;
        if( groupieInfo.extractFromSearchBlob( blobEntry ) )
        {
            groupieInfo.setHostType( hostType );

            groupieSearchResult( hostType, searchSessionId, sktBase, netIdent, groupieInfo );
        }
        else
        {
            LogMsg( LOG_ERROR, "Could not extract GroupieListMgr::updateFromGroupieSearchBlob" );
            break;
        }
    }
}

//============================================================================
bool GroupieListMgr::requestMoreGroupiesFromHost( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& nextGroupieOnlineId, PluginBase* plugin )
{
    PktHostInviteMoreReq pktReq;
    pktReq.setHostType( hostType );
    pktReq.setSearchSessionId( searchSessionId );
    pktReq.setNextSearchOnlineId( nextGroupieOnlineId );
    return plugin->txPacket( netIdent, sktBase, &pktReq );
}
