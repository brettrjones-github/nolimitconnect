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

#include <CoreLib/DbBase.h>
#include <PktLib/GroupieId.h>
#include <map>

class P2PEngine;
class HostJoinInfo;
class HostJoinMgr;
class VxGUID;

class HostJoinInfoDb : public DbBase
{
public:
    HostJoinInfoDb( P2PEngine& engine, HostJoinMgr& mgr, const char *dbName );
    virtual ~HostJoinInfoDb() = default;

    void						lockHostJoinInfoDb( void ) { m_HostJoinInfoDbMutex.lock(); }
    void						unlockHostJoinInfoDb( void ) { m_HostJoinInfoDbMutex.unlock(); }

    bool						addHostJoin( HostJoinInfo * hostInfo );
    void						removeHostJoin( GroupieId& groupieId );

    void						getAllHostJoins( std::map<GroupieId, HostJoinInfo*>& userHostList );
    void						purgeAllHostJoins( void ); 

protected:

    bool						addHostJoin( GroupieId& groupieId,
                                                VxGUID&			thumbId,
                                                uint64_t		infoModTime,                             
                                                enum EJoinState      joinState,
                                                uint64_t		lastConnectMs,
                                                uint64_t		lastJoinMs,
                                                enum EFriendState    friendState,
                                                uint32_t        hostFlags,
                                                std::string     hostUrl
                                            );

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );
    void						insertHostJoinInTimeOrder( HostJoinInfo * hostInfo, std::map<GroupieId, HostJoinInfo*>& assetList );

    P2PEngine&					m_Engine;
    HostJoinMgr&				m_HostJoinMgr;
    VxMutex						m_HostJoinInfoDbMutex;
};

