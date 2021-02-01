//============================================================================
// Copyright (C) 2020 Brett R. Jones
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
void HostSearchEntry::updateLastRxTime( void )
{
    m_LastRxTime = GetTimeStampMs();
}

//============================================================================
HostSearchEntry::HostSearchEntry( const HostSearchEntry& rhs )
: m_LastRxTime( rhs.m_LastRxTime )
, m_PluginSetting( rhs.m_PluginSetting )
, m_Url( rhs.m_Url )
, m_SearchStrings( rhs.m_SearchStrings )
{
    memcpy( &m_Ident, &rhs.m_Ident, sizeof( VxNetIdent ) );
    m_PktHostAnn.setPktLength( 0 );
    if( rhs.m_PktHostAnn.getPktLength() )
    {
        memcpy( &m_PktHostAnn, &rhs.m_PktHostAnn, rhs.m_PktHostAnn.getPktLength() );
    }
}

//============================================================================
HostSearchEntry& HostSearchEntry::operator=( const HostSearchEntry& rhs )
{
    if( this != &rhs )
    {
        m_LastRxTime = rhs.m_LastRxTime;
        memcpy( &m_Ident, &rhs.m_Ident, sizeof( VxNetIdent ) );
        m_PktHostAnn.setPktLength( 0 );
        if( rhs.m_PktHostAnn.getPktLength() )
        {
            memcpy( &m_PktHostAnn, &rhs.m_PktHostAnn, rhs.m_PktHostAnn.getPktLength() );
        }

        m_PluginSetting = rhs.m_PluginSetting;
        m_Url = rhs.m_Url;
        m_SearchStrings = rhs.m_SearchStrings;
    }

    return *this;
}

//============================================================================
HostServerSearchMgr::HostServerSearchMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase )
: HostBaseMgr(engine, pluginMgr, myIdent, pluginBase)
{
}

//============================================================================
void HostServerSearchMgr::updateHostSearchList( EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent )
{
    if( haveHostList( hostType ) && netIdent->getMyOnlineId().isVxGUIDValid() )
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
bool HostServerSearchMgr::haveHostList( EHostType hostType )
{
    return hostType == eHostTypeChatRoom || hostType == eHostTypeGroup || hostType == eHostTypeRandomConnect;
}

//============================================================================
std::map<VxGUID, HostSearchEntry>& HostServerSearchMgr::getSearchList( EHostType hostType )
{
    switch( hostType )
    {
    case eHostTypeChatRoom:
        return m_ChatHostList;
    case eHostTypeGroup:
        return m_GroupHostList;
    case eHostTypeRandomConnect:
        return m_RandConnectList;
    default:
        return m_NullList;
    }
}