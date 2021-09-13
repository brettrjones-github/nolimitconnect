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

#include "HostServerSearchMgr.h"
#include "PluginBase.h"
#include "PluginMgr.h"

#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>
#include <ptop_src/ptop_engine_src/Plugins/PluginBase.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostSearch.h>
#include <PktLib/PluginIdList.h>
#include <CoreLib/VxTime.h>

#include <memory.h>

#ifdef _MSC_VER
# pragma warning(disable: 4355) //'this' : used in base member initializer list
#endif //_MSC_VER

//============================================================================
HostServerSearchMgr::HostServerSearchMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
: HostBaseMgr(engine, pluginMgr, myIdent, pluginBase)
{
}

//============================================================================
void HostServerSearchMgr::updateHostSearchList( EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent )
{
    if( haveBlob( hostType ) && netIdent->getMyOnlineId().isVxGUIDValid() )
    {
        EPluginType pluginType = getSearchPluginType( hostType );
        m_SearchMutex.lock();
        std::map<PluginId, HostSearchEntry>& searchMap = getSearchList( hostType );
        PluginId pluginId( netIdent->getMyOnlineId(), pluginType );
        auto iter = searchMap.find( pluginId );
        if( iter != searchMap.end() )
        {
            fillSearchEntry( iter->second, hostType, hostAnn, netIdent, false );
        }
        else
        {
            HostSearchEntry searchEntry;
            fillSearchEntry( searchEntry, hostType, hostAnn, netIdent, true );
            searchMap[pluginId] = searchEntry;
        }

        m_SearchMutex.unlock();
        LogModule( eLogHostConnect, LOG_DEBUG, "HostServerSearchMgr host ann plugin %s updated ", pluginId.describePluginId().c_str() );
    }
}

//============================================================================
ECommErr HostServerSearchMgr::searchRequest( SearchParams& searchParams, PktHostSearchReply& searchReply, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    EHostType hostType = searchParams.getHostType();
    ECommErr searchErr = haveBlob(hostType) ? eCommErrNone : eCommErrInvalidHostType;
    if( eCommErrNone == searchErr )
    {
        unsigned int matchCnt = 0;
        PluginIdList toRemoveList;
        PluginIdList matchList;
        uint64_t timeNow = GetGmtTimeMs();
        searchReply.setIsGuidPluginTypePairs( true );
        
        m_SearchMutex.lock();
        std::map<PluginId, HostSearchEntry>& searchMap = getSearchList( hostType );
        for( std::map<PluginId, HostSearchEntry>::iterator iter = searchMap.begin(); iter != searchMap.end(); ++iter )
        {
            if( timeNow - iter->second.m_LastRxTime > MIN_HOST_RX_UPDATE_TIME_MS )
            {
                toRemoveList.addPluginId( iter->first );
            }
            else if( iter->second.searchMatch( searchParams, searchStr ) )
            {
                const PluginId& pluginId = iter->first;
                searchReply.addPluginId( pluginId );
                matchCnt++;
                LogModule( eLogHostConnect, LOG_DEBUG, "HostServerSearchMgr match %d plugin %s ", matchCnt, pluginId.describePluginId().c_str() );
            }
        }

        removeEntries( searchMap, toRemoveList );
        m_SearchMutex.unlock();
        searchReply.calcPktLen();
    }

    return searchErr;
}

//============================================================================
ECommErr HostServerSearchMgr::settingsRequest( PluginId& pluginId, PktPluginSettingReply& settingReply, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    EHostType hostType = settingReply.getHostType();
    ECommErr searchErr = haveBlob(hostType) ? eCommErrNone : eCommErrInvalidHostType;
    if( eCommErrNone == searchErr )
    {
        unsigned int matchCnt = 0;
        PluginIdList toRemoveList;
        PluginIdList matchList;
        uint64_t timeNow = GetGmtTimeMs();
        PktBlobEntry& blobEntry = settingReply.getBlobEntry();
        blobEntry.resetWrite();
        
        m_SearchMutex.lock();
        std::map<PluginId, HostSearchEntry>& searchMap = getSearchList( hostType );
        for( std::map<PluginId, HostSearchEntry>::iterator iter = searchMap.begin(); iter != searchMap.end(); ++iter )
        {
            if( timeNow - iter->second.m_LastRxTime > MIN_HOST_RX_UPDATE_TIME_MS )
            {
                toRemoveList.addPluginId( iter->first );
            }
            else if( iter->first == pluginId )
            {
                if( iter->second.m_Ident.addToBlob( blobEntry ) )
                {
                    if( iter->second.m_PluginSetting.addToBlob( blobEntry ) )
                    {
                        settingReply.setTotalMatches( settingReply.getTotalMatches() + 1 );
                        LogModule( eLogHostConnect, LOG_DEBUG, "HostServerSearchMgr match %d plugin %s ", matchCnt, pluginId.describePluginId().c_str() );
                    }
                }    
            }
        }

        removeEntries( searchMap, toRemoveList );
        m_SearchMutex.unlock();
        settingReply.calcPktLen();
    }

    return searchErr;
}

//============================================================================
void HostServerSearchMgr::removeEntries( std::map<PluginId, HostSearchEntry>& searchMap, PluginIdList& toRemoveList )
{
    for( auto iter = toRemoveList.getPluginIdList().begin(); iter != toRemoveList.getPluginIdList().end(); ++iter )
    {
        auto iter2 = searchMap.find( *iter );
        if( iter2 != searchMap.end() )
        {
            searchMap.erase( iter2 );
        }
    }
}

//============================================================================
bool HostServerSearchMgr::fillSearchEntry( HostSearchEntry& searchEntry, EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent, bool forced )
{
    bool result = false;

    searchEntry.updateLastRxTime();
    memcpy( &searchEntry.m_Ident, netIdent, sizeof( VxNetIdent ) );
    searchEntry.m_PktHostAnn.setPktLength( 0 );
    hostAnn->fillPktHostAnn( searchEntry.m_PktHostAnn );
    BinaryBlob binarySettings;
    if( hostAnn->getPluginSettingBinary( binarySettings ) )
    {
        if( searchEntry.m_PluginSetting.fromBinary( binarySettings ) )
        {
            searchEntry.m_SearchStrings.clear();
            if( searchEntry.m_PluginSetting.fillSearchStrings( searchEntry.m_SearchStrings ) )
            {
                result = true;
            }
        }
    }

    searchEntry.m_Url = netIdent->getMyOnlineUrl();

    return result;
}

//============================================================================
bool HostServerSearchMgr::haveBlob( EHostType hostType )
{
    return hostType == eHostTypeChatRoom || hostType == eHostTypeGroup || hostType == eHostTypeRandomConnect;
}

//============================================================================
std::map<PluginId, HostSearchEntry>& HostServerSearchMgr::getSearchList( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeChatRoom:
        return m_ChatBlob;
    case eHostTypeGroup:
        return m_GroupBlob;
    case eHostTypeRandomConnect:
        return m_RandConnectList;
    default:
        return m_NullList;
    }
}

//============================================================================
EPluginType HostServerSearchMgr::getSearchPluginType( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeChatRoom:
        return ePluginTypeHostChatRoom;
    case eHostTypeGroup:
        return ePluginTypeHostGroup;
    case eHostTypeRandomConnect:
        return ePluginTypeHostRandomConnect;
    default:
        return ePluginTypeInvalid;
    }
}


//============================================================================
void HostServerSearchMgr::fromGuiSendAnnouncedList( EHostType hostType )
{
    LogMsg( LOG_DEBUG, "HostServerMgr fromGuiSendAnnouncedList" );

    uint64_t timeNow = GetGmtTimeMs();
    m_SearchMutex.lock();
    std::map<PluginId, HostSearchEntry>& searchMap = getSearchList( hostType );
    for( std::map<PluginId, HostSearchEntry>::iterator iter = searchMap.begin(); iter != searchMap.end(); ++iter )
    {
        // if currently active
        if( timeNow - iter->second.m_LastRxTime <= MIN_HOST_RX_UPDATE_TIME_MS )
        {
            m_Engine.getToGui().toGuiHostSearchResult( hostType, iter->second.m_Ident.getMyOnlineId(), iter->second.m_Ident, iter->second.m_PluginSetting );
        }
    }

    m_SearchMutex.unlock();
}
