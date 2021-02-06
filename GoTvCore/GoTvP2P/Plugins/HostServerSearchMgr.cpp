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

#include <GoTvCore/GoTvP2P/P2PEngine/P2PEngine.h>
#include <GoTvCore/GoTvP2P/Plugins/PluginBase.h>

#include <NetLib/VxSktBase.h>
#include <PktLib/PktsHostJoin.h>
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
        m_SearchMutex.lock();
        std::map<VxGUID, HostSearchEntry>& searchMap = getSearchList( hostType );
        auto& iter = searchMap.find( netIdent->getMyOnlineId() );
        if( iter != searchMap.end() )
        {
            fillSearchEntry( iter->second, hostType, hostAnn, netIdent, false );
        }
        else
        {
            HostSearchEntry searchEntry;
            fillSearchEntry( searchEntry, hostType, hostAnn, netIdent, true );
            searchMap.insert_or_assign( netIdent->getMyOnlineId(), searchEntry );
        }

        m_SearchMutex.unlock();
    }
}

//============================================================================
ECommErr HostServerSearchMgr::searchRequest( EHostType hostType, PktHostSearchReply& searchReply, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent )
{
    ECommErr searchErr = haveBlob(hostType) ? eCommErrNone : eCommErrInvalidHostType;
    if( eCommErrNone == searchErr )
    {
        unsigned int matchCnt = 0;
        VxGUIDList toRemoveList;
        VxGUIDList matchList;
        uint64_t timeNow = GetGmtTimeMs();
        m_SearchMutex.lock();
        std::map<VxGUID, HostSearchEntry>& searchMap = getSearchList( hostType );
        for( std::map<VxGUID, HostSearchEntry>::iterator iter = searchMap.begin(); iter != searchMap.end(); ++iter )
        {
            if( timeNow - iter->second.m_LastRxTime > MIN_HOST_RX_TIME_MS )
            {
                toRemoveList.addGuid( iter->first );
            }
            else if( iter->second.searchMatch( searchStr ) )
            {
                matchList.addGuid( iter->first );
                addOrQueSearchMatch( searchReply, sktBase, netIdent, iter->first, iter->second );
            }
        }

        removeEntries( searchMap, toRemoveList );
        m_SearchMutex.unlock();
    }

    return searchErr;
}

//============================================================================
void HostServerSearchMgr::addOrQueSearchMatch( PktHostSearchReply& searchReply, VxSktBase* sktBase, VxNetIdent* netIdent, const VxGUID& guid, const HostSearchEntry& entry )
{
    // asdkfgs todo
}

//============================================================================
void HostServerSearchMgr::removeEntries( std::map<VxGUID, HostSearchEntry>& searchMap, VxGUIDList& toRemoveList )
{
    for( auto& iter = toRemoveList.getGuidList().begin(); iter != toRemoveList.getGuidList().end(); ++iter )
    {
        auto& iter2 = searchMap.find( *iter );
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

    searchEntry.m_Url = netIdent->getMyPtopUrl();

    return result;
}

//============================================================================
bool HostServerSearchMgr::haveBlob( EHostType hostType )
{
    return hostType == eHostTypeChatRoom || hostType == eHostTypeGroup || hostType == eHostTypeRandomConnect;
}

//============================================================================
std::map<VxGUID, HostSearchEntry>& HostServerSearchMgr::getSearchList( EHostType hostType )
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