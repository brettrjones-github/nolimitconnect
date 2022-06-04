#pragma once
//============================================================================
// Copyright (C) 2022 Brett R. Jones
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

#include <GuiInterface/IDefs.h>

#include <CoreLib/DbBase.h>
#include <map>

class IgnoredHostInfo;
class IgnoreListMgr;
class P2PEngine;
class VxGUID;

class IgnoredHostsDb : public DbBase
{
public:
    IgnoredHostsDb() = delete;
    IgnoredHostsDb( P2PEngine& engine, IgnoreListMgr& mgr, const char* dbname );
    virtual ~IgnoredHostsDb() = default;

    bool                        saveToDatabase( IgnoredHostInfo& hostInfo );
    void                        removeFromDatabase( VxGUID& onlineId );

    bool                        restoreFromDatabase( std::map<VxGUID, IgnoredHostInfo>& ignoredHostList );

protected:
    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );

    P2PEngine&					m_Engine;
    IgnoreListMgr&				m_IgnoreListMgr;
};

