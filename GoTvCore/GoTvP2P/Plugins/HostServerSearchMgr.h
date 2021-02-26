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
#include "HostSearchEntry.h"

#include <PktLib/PluginId.h>

#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxMutex.h>

#include <map>

#define MIN_HOST_RX_UPDATE_TIME_MS         30000 // must rx a host announce in this time frame or host is considered offline

class ConnectionMgr;
class P2PEngine;
class PluginBase;
class PluginId;
class PluginIdList;
class PluginMgr;
class VxPktHdr;
class PktHostSearchReply;
class PktPluginSettingReply;

class HostServerSearchMgr : public HostBaseMgr
{
public:
    HostServerSearchMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostServerSearchMgr() = default;

    void                        updateHostSearchList( EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent );
    virtual ECommErr            searchRequest( SearchParams& searchParams, PktHostSearchReply& searchReply, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual ECommErr            settingsRequest( PluginId& pluginId, PktPluginSettingReply& searchReply, VxSktBase* sktBase, VxNetIdent* netIdent );

protected:
    std::map<PluginId, HostSearchEntry>& getSearchList( EHostType hostType );
    bool                        haveBlob( EHostType hostType );
    bool                        fillSearchEntry( HostSearchEntry& searchEntry, EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent, bool forced );

    // remove entries does not lock m_SearchMutex
    void                        removeEntries( std::map<PluginId, HostSearchEntry>& searchMap, PluginIdList& toRemoveList );
    EPluginType                 getSearchPluginType( EHostType hostType );

    //=== vars ===//
    VxMutex                     m_SearchMutex;
    std::map<PluginId, HostSearchEntry>   m_ChatBlob;
    std::map<PluginId, HostSearchEntry>   m_GroupBlob;
    std::map<PluginId, HostSearchEntry>   m_RandConnectList;
    std::map<PluginId, HostSearchEntry>   m_NullList; // empty list and list for network hosts
};

