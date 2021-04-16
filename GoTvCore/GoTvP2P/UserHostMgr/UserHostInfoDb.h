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

#include <GoTvInterface/IDefs.h>

#include <CoreLib/DbBase.h>
#include <CoreLib/VxGUID.h>

class P2PEngine;
class UserHostInfo;
class UserHostMgr;
class VxGUID;

class UserHostInfoDb : public DbBase
{
public:
    UserHostInfoDb( P2PEngine& engine, UserHostMgr& mgr, const char *dbName );
    virtual ~UserHostInfoDb() = default;

    void						lockUserHostInfoDb( void ) { m_UserHostInfoDbMutex.lock(); }
    void						unlockUserHostInfoDb( void ) { m_UserHostInfoDbMutex.unlock(); }

    void						addUserHost(    VxGUID&			onlineId, 
                                                VxGUID&			thumbId,
                                                uint64_t		infoModTime,
                                                EHostType       hostType,
                                                uint32_t        hostFlags,
                                                std::string     hostUrl,
                                                uint64_t		lastConnectMs,
                                                uint64_t		lastJoinMs
                                                );

    void						addUserHost( UserHostInfo * hostInfo );
    void						removeUserHost( VxGUID& onlineId, EHostType hostType );

    void						getAllUserHosts( std::vector<UserHostInfo*>& userHostList );
    void						purgeAllUserHosts( void ); 

protected:

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );
    void						insertUserHostInTimeOrder( UserHostInfo * hostInfo, std::vector<UserHostInfo*>& assetList );

    P2PEngine&					m_Engine;
    UserHostMgr&				m_UserHostMgr;
    VxMutex						m_UserHostInfoDbMutex;
};

