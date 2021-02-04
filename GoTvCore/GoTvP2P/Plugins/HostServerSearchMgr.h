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

#include <CoreLib/VxGUIDList.h>
#include <CoreLib/VxMutex.h>

#include <map>

#define MIN_HOST_RX_TIME_MS         30000 // must rx a host announce in this time frame or host is considered offline

class ConnectionMgr;
class P2PEngine;
class PluginMgr;
class PluginBase;
class VxPktHdr;
class PktHostSearchReply;

class HostServerSearchMgr : public HostBaseMgr
{
public:
    HostServerSearchMgr( P2PEngine& engine, PluginMgr& pluginMgr, VxNetIdent * myIdent, PluginBase& pluginBase );
	virtual ~HostServerSearchMgr() = default;

    void                        updateHostSearchList( EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent );
    virtual ECommErr            searchRequest( EHostType hostType, PktHostSearchReply& searchReply, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent );

protected:
    std::map<VxGUID, HostSearchEntry>& getSearchList( EHostType hostType );
    bool                        haveHostList( EHostType hostType );
    bool                        fillSearchEntry( HostSearchEntry& searchEntry, EHostType hostType, PktHostAnnounce* hostAnn, VxNetIdent* netIdent, bool forced );
    void                        addOrQueSearchMatch( PktHostSearchReply&searchReply, VxSktBase* sktBase, VxNetIdent* netIdent, const VxGUID& guid, const HostSearchEntry& entry );

    // remove entries does not lock m_SearchMutex
    void                        removeEntries( std::map<VxGUID, HostSearchEntry>& searchMap, VxGUIDList& toRemoveList );

    //=== vars ===//
    VxMutex                     m_SearchMutex;
    std::map<VxGUID, HostSearchEntry>   m_ChatHostList;
    std::map<VxGUID, HostSearchEntry>   m_GroupHostList;
    std::map<VxGUID, HostSearchEntry>   m_RandConnectList;
    std::map<VxGUID, HostSearchEntry>   m_NullList; // empty list and list for network hosts
};

