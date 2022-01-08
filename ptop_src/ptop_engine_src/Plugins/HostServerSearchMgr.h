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

#define MIN_HOST_RX_UPDATE_TIME_MS         (30 * 60 * 1000) // must rx a host announce in this time frame or host is considered offline

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

    void                        updateHostSearchList( EHostType hostType, PktHostInviteAnnounceReq* hostAnn, VxNetIdent* netIdent, VxSktBase* sktBase = nullptr );

    virtual ECommErr            searchRequest( SearchParams& searchParams, PktHostSearchReply& searchReply, std::string& searchStr, VxSktBase* sktBase, VxNetIdent* netIdent );
    virtual ECommErr            settingsRequest( PluginId& pluginId, PktPluginSettingReply& searchReply, VxSktBase* sktBase, VxNetIdent* netIdent );

    virtual void				fromGuiSendAnnouncedList( EHostType hostType );
    virtual void				fromGuiListAction( EListAction listAction );

    virtual bool				onPktHostInviteSearchReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktHostInviteSearchReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual bool				onPktHostInviteMoreReq( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );
    virtual void				onPktHostInviteMoreReply( VxSktBase* sktBase, VxPktHdr* pktHdr, VxNetIdent* netIdent );

protected:
    std::map<PluginId, HostSearchEntry>& getHostAnnList( EHostType hostType );
    bool                        haveHostAnnList( EHostType hostType );
    bool                        fillSearchEntry( HostSearchEntry& searchEntry, EHostType hostType, PktHostInviteAnnounceReq* hostAnn, VxNetIdent* netIdent, bool forced );

    void                        onHostInviteAnnounceAdded( EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase );
    void                        onHostInviteAnnounceUpdated( EHostType hostType, HostedInfo& hostedInfo, VxNetIdent* netIdent, VxSktBase* sktBase );

    // remove entries does not lock m_SearchMutex
    void                        removeEntries( std::map<PluginId, HostSearchEntry>& searchMap, PluginIdList& toRemoveList );
    EPluginType                 getSearchPluginType( EHostType hostType );
    void                        doFromGuiListAction( EListAction listAction, EPluginType pluginType, std::map<PluginId, HostSearchEntry>& hostedList );
    bool                        fillSearchPktBlob( PktBlobEntry& blobEntry, HostedInfo& hostedInfo );
    bool                        extractSearchBlobToHostedInfo( PktBlobEntry& blobEntry, HostedInfo& hostedInfo );

    void                        logCommError( ECommErr commErr, const char* desc, VxSktBase* sktBase, VxNetIdent* netIdent );
    void                        updateFromHostInviteSearchBlob( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, PktBlobEntry& blobEntry, int hostInfoCount );
    bool                        requestMoreHostInvitesFromNetworkHost( EHostType hostType, VxGUID& searchSessionId, VxSktBase* sktBase, VxNetIdent* netIdent, VxGUID& nextHostOnlineId );

    //=== vars ===//
    VxMutex                     m_SearchMutex;
    std::map<PluginId, HostSearchEntry>   m_ChatRoomHostAnnList;
    std::map<PluginId, HostSearchEntry>   m_GroupHostAnnList;
    std::map<PluginId, HostSearchEntry>   m_RandConnectHostAnnList;
    std::map<PluginId, HostSearchEntry>   m_NullList; // empty list and list for network hosts
};

