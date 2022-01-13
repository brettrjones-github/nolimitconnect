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

#include <CoreLib/VxPtopUrl.h>
#include <PktLib/PktsGroupie.h>

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
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostOnlineId() == hostOnlineId && iter->getHostType() == hostType )
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
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostOnlineId() == hostOnlineId && iter->getHostType() == hostType )
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
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostOnlineId() == hostOnlineId && iter->getHostType() == hostType )
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
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostOnlineId() == hostOnlineId && iter->getHostType() == hostType )
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
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostOnlineId() == hostOnlineId && iter->getHostType() == hostType )
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
    for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); )
    {
        if( iter->getGroupieOnlineId() == groupieOnlineId && iter->getHostOnlineId() == hostOnlineId && iter->getHostType() == hostType )
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
void GroupieListMgr::onPktGroupieInfoReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent )
{
    bool result{ false };
    PktGroupieInfoReply* pktReply = (PktGroupieInfoReply *)pktHdr;

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
                    result = true;
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
        for( auto iter = m_GroupieInfoList.begin(); iter != m_GroupieInfoList.end(); ++iter )
        {
            if( joinedTimestamp )
            {

            }
            else if( connectedTimestamp )
            {
                // skip those with joined timestamp and put those with connected timestamp in connected time order
                if( iter->getJoinedTimestamp() )
                {
                    continue;
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
void GroupieListMgr::hostSearchResult( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, GroupieInfo& groupieInfo )
{
    GroupieInfo resultInfo;
    if( updateGroupieInfo( hostType, groupieInfo, netIdent, sktBase, &resultInfo ) )
    {
        announceGroupieInfoSearchResult( &resultInfo, searchSessionId );
    }
}

//============================================================================
void GroupieListMgr::hostSearchCompleted( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, ECommErr commErr )
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
        if( iter->getHostType() == hostType && iter->getGroupieOnlineId() == groupieInfo.getGroupieOnlineId() && iter->getHostOnlineId() == groupieInfo.getHostOnlineId() )
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
                    m_GroupieInfoListDb.updateGroupieUrl( iter->getGroupieOnlineId(), iter->getHostOnlineId(), iter->getHostType(), groupieInfo.getGroupieUrl() );
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
                        m_GroupieInfoListDb.updateGroupieTitleAndDescription( iter->getGroupieOnlineId(), iter->getHostOnlineId(), iter->getHostType(), groupieInfo.getGroupieTitle(), groupieInfo.getGroupieDescription(), iter->getGroupieInfoTimestamp() );
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