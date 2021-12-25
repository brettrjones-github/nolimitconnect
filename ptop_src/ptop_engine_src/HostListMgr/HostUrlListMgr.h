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

#include <GuiInterface/IDefs.h>

#include "HostUrlInfo.h"
#include "HostUrlListDb.h"

#include <CoreLib/VxMutex.h>

class P2PEngine;
class VxGUID;

class HostUrlListMgr
{
    const int HOST_URL_LIST_DB_VERSION = 1;
public:
    HostUrlListMgr() = delete;
    HostUrlListMgr( P2PEngine& engine );
    virtual ~HostUrlListMgr() = default;

    RCODE                       hostUrlListMgrStartup( std::string& dbFileName );
    RCODE                       hostUrlListMgrShutdown( void );

    void                        lockList( void )    { m_HostUrlsMutex.lock(); }
    void                        unlockList( void )  { m_HostUrlsMutex.unlock(); }

    void                        updateHostUrl( EHostType hostType, VxGUID& hostGuid, std::string& hostUrl, int64_t timestampMs = 0 );
    bool                        getHostUrls( EHostType hostType, std::vector<HostUrlInfo>& retHostUrls );

protected:
    P2PEngine&                  m_Engine;
    VxMutex                     m_HostUrlsMutex;
    HostUrlListDb               m_HostUrlListDb;

    std::vector<HostUrlInfo>    m_HostUrlsList;
};

