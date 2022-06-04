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
// http://www.nolimitconnect.org
//============================================================================

#include "IdentListMgrBase.h"
#include "IgnoredHostInfo.h"
#include "IgnoredHostsDb.h"

#include <vector>

// maintains a list of ignored sorted by timestamp for faster lookup

class IgnoreListMgr : public IdentListMgrBase
{
public:
    IgnoreListMgr() = delete;
    IgnoreListMgr( P2PEngine& engine );
    virtual ~IgnoreListMgr() = default;

    bool                        isIgnored( VxGUID& onlineId );
    virtual void                updateIdent( VxGUID& onlineId, int64_t timestamp ) override;
    virtual void                removeIdent( VxGUID& onlineId ) override;

    bool                        isHostIgnored( VxGUID& onlineId );
    void                        addHostIgnore( VxGUID& onlineId, std::string hostUrl, std::string hostTitle, VxGUID& thumbId, std::string hostDescription );
    void                        removeHostIgnore( VxGUID& onlineId );

    std::vector<std::pair<VxGUID, int64_t>>& getIdentList() { return m_IgnoreIdentList; };

protected:
    void                        initializeIgnoredHostsIfNeeded( void );

    IgnoredHostsDb              m_IgnoredHostsDb;
    VxMutex                     m_IgnoredHostsMutex;
    bool                        m_IgnoredHostsDbInitialized{ false };

    std::vector<std::pair<VxGUID, int64_t>> m_IgnoreIdentList;
    std::map<VxGUID, IgnoredHostInfo> m_IgnoredHostList;
};

