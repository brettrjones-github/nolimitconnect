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

#include <GuiInterface/IDefs.h>

#include <CoreLib/DbBase.h>

class P2PEngine;
class HostJoinMgr;
class VxGUID;

class HostJoinedLastDb : public DbBase
{
public:
    HostJoinedLastDb( P2PEngine& engine, HostJoinMgr& mgr, const char*dbName );
    virtual ~HostJoinedLastDb() = default;

    void						lockHostJoinedLastDb( void ) { m_HostJoinedLastDbMutex.lock(); }
    void						unlockHostJoinedLastDb( void ) { m_HostJoinedLastDbMutex.unlock(); }

    bool						setJoinedLast( enum EPluginType pluginType, VxGUID& onlineId, int64_t lastJoinMs, std::string hostUrl );
    bool						getJoinedLast( enum EPluginType pluginType, VxGUID& onlineId, int64_t& lastJoinMs, std::string& hostUrl );

    void						removeJoinedLast( enum EPluginType pluginType );
    void						purgeAllHJoinedLast( void );

protected:

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );

    P2PEngine&					m_Engine;
    HostJoinMgr&				m_HostJoinMgr;
    VxMutex						m_HostJoinedLastDbMutex;
};

