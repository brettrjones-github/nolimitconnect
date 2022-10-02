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
class UserJoinMgr;
class VxGUID;

class UserJoinedLastDb : public DbBase
{
public:
    UserJoinedLastDb( P2PEngine& engine, UserJoinMgr& mgr, const char*dbName );
    virtual ~UserJoinedLastDb() = default;

    void						lockDb( void )    { m_DbMutex.lock(); }
    void						unlockDb( void )  { m_DbMutex.unlock(); }

    bool						setJoinedLastHostType( EHostType hostType );
    bool						getJoinedLastHostType( EHostType& hostType );

    bool						setJoinedLast( EHostType hostType, VxGUID& onlineId, int64_t lastJoinMs, std::string hostUrl );
    bool						getJoinedLast( EHostType pluginType, VxGUID& onlineId, int64_t& lastJoinMs, std::string& hostUrl );

    void						removeJoinedLast( EHostType hostType );
    void						purgeAllJoinedLast( void );

protected:

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );

    P2PEngine&					m_Engine;
    UserJoinMgr&				m_UserJoinMgr;
};

