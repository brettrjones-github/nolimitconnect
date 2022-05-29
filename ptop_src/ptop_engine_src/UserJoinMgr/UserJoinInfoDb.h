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
#include <PktLib/GroupieId.h>
#include <map>

class P2PEngine;
class UserJoinInfo;
class UserJoinMgr;

class UserJoinInfoDb : public DbBase
{
public:
    UserJoinInfoDb( P2PEngine& engine, UserJoinMgr& mgr, const char *dbName );
    virtual ~UserJoinInfoDb() = default;

    bool						userJoinInfoDbStartup( int dbVersion, const char* dbFileName );
    bool						userJoinInfoDbShutdown( void );

    bool						addUserJoin( UserJoinInfo * hostInfo );
    void						removeUserJoin( GroupieId& groupieId );

    void						getAllUserJoins( std::map<GroupieId, UserJoinInfo*>& userHostList );
    void						purgeAllUserJoins( void ); 

protected:

    bool						addUserJoin(    GroupieId&      groupieId,
                                                VxGUID&			thumbId,
                                                uint64_t		infoModTime,                                    
                                                EJoinState      joinState,
                                                uint64_t		lastConnectMs,
                                                uint64_t		lastJoinMs,
                                                EFriendState    friendState,
                                                uint32_t        hostFlags,
                                                std::string     hostUrl
                                            );

    virtual RCODE				onCreateTables( int iDbVersion );
    virtual RCODE				onDeleteTables( int iOldVersion );
    void						insertUserJoinInTimeOrder( UserJoinInfo * hostInfo, std::map<GroupieId, UserJoinInfo*>& assetList );

    P2PEngine&					m_Engine;
    UserJoinMgr&				m_UserJoinMgr;
};

