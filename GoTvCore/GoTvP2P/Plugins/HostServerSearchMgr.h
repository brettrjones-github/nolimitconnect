#pragma once
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

#include "HostBaseMgr.h"

#include <GoTvCore/GoTvP2P/PluginSettings/PluginSetting.h>

#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxMutex.h>

#include <PktLib/PktHostAnnounce.h>

#include <map>
#include <string>
#include <vector>

class ConnectionMgr;
class P2PEngine;
class PluginMgr;
class VxNetIdent;
class PluginBase;
class VxPktHdr;
class PktHostAnnounce;

class HostSearchEntry
{
public:
    HostSearchEntry() = default;
    ~HostSearchEntry() = default;
    HostSearchEntry( const HostSearchEntry& rhs );

    HostSearchEntry&			operator=( const HostSearchEntry& rhs );

    void                        updateLastRxTime( void );

    uint64_t                    m_LastRxTime{ 0 }; // time last recieved announce
    VxNetIdent                  m_Ident;
    PktHostAnnounce             m_PktHostAnn;
    PluginSetting               m_PluginSetting;
    std::string                 m_Url;
    std::vector<std::string>    m_SearchStrings;
    
};

class HostServerSearchMgr : public HostBaseMgr
{
public:
    HostServerSearchMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostServerSearchMgr() = default;

    void                        updateHostSearchList( EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent );

protected:
    std::map<VxGUID, HostSearchEntry>& getSearchList( EHostType hostType );
    bool                        haveHostList( EHostType hostType );
    bool                        fillSearchEntry( HostSearchEntry& searchEntry, EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent, bool forced );

    //=== vars ===//
    VxMutex                     m_SearchMutex;
    std::map<VxGUID, HostSearchEntry>   m_ChatHostList;
    std::map<VxGUID, HostSearchEntry>   m_GroupHostList;
    std::map<VxGUID, HostSearchEntry>   m_RandConnectList;
    std::map<VxGUID, HostSearchEntry>   m_NullList; // empty list and list for network hosts
};

