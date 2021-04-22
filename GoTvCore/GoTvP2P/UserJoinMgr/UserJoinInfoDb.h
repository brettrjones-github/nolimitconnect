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
#include <CoreLib/VxGUID.h>

class P2PEngine;
class UserJoinInfo;
class UserJoinMgr;
class VxGUID;

class UserJoinInfoDb : public DbBase
{
public:
    UserJoinInfoDb( P2PEngine& engine, UserJoinMgr& mgr, const char *dbName );
    virtual ~UserJoinInfoDb() = default;

    void						lockUserJoinInfoDb( void ) { m_UserJoinInfoDbMutex.lock(); }
    void						unlockUserJoinInfoDb( void ) { m_UserJoinInfoDbMutex.unlock(); }

    bool						addUserJoin( UserJoinInfo * hostInfo );
    void						removeUserJoin( VxGUID& onlineId, EHostType hostType );

    void						getAllUserJoins( std::vector<UserJoinInfo*>& userHostList );
    void						purgeAllUserJoins( void ); 

protected:

    bool						addUserJoin(    VxGUID&			onlineId, 
                                                VxGUID&			thumbId,
                                                uint64_t		infoModTime,
                                                EPluginType     pluginType,
                                                EHostType       hostType,
                                                EJoinState      joinState,
                                                uint64_t		lastConnectMs,
                                                uint64_t		lastJoinMs,
                                                uint32_t        hostFlags,
                                                std::string     hostUrl
                                            );

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );
    void						insertUserJoinInTimeOrder( UserJoinInfo * hostInfo, std::vector<UserJoinInfo*>& assetList );

    P2PEngine&					m_Engine;
    UserJoinMgr&				m_UserJoinMgr;
    VxMutex						m_UserJoinInfoDbMutex;
};

