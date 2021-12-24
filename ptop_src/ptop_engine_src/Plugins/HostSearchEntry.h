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

#include <ptop_src/ptop_engine_src/PluginSettings/PluginSetting.h>

#include <PktLib/PktHostAnnounce.h>

#include <string>
#include <vector>

class HostedEntry;

class HostSearchEntry
{
public:
    HostSearchEntry() = default;
    ~HostSearchEntry() = default;
    HostSearchEntry( const HostSearchEntry& rhs );

    HostSearchEntry&			operator=( const HostSearchEntry& rhs );

    void                        updateLastRxTime( void );
    bool                        searchMatch( SearchParams& searchParams, std::string& searchStr );
    //void                        toHostedEntry( HostedEntry& entry );
    void                        fillSearchReplyBlob( BinaryBlob& blob );

    bool                        addToBlob( PktBlobEntry& entryBlob );

    uint64_t                    m_LastRxTime{ 0 }; // time last recieved announce
    VxNetIdent                  m_Ident;
    PktHostAnnounce             m_PktHostAnn;
    PluginSetting               m_PluginSetting;
    std::string                 m_Url;
    std::vector<std::string>    m_SearchStrings;  
};
