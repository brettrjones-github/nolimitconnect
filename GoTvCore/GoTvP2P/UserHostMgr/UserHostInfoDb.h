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

    void						addUserHost(    EHostType       hostType,
                                                VxGUID&			hostOnlineId,  
                                                uint64_t		hostModTime,
                                                uint32_t        hostFlags,
                                                VxGUID&			thumbId,
                                                uint64_t		thumbModTime,
                                                VxGUID&			offerId,
                                                EOfferState		offerState,
                                                uint64_t		offerModTime,
                                                std::string     hostUrl
                                                );

    void						addUserHost( UserHostInfo * hostInfo );
    void						removeUserHost( VxGUID& hostOnlineId );

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

