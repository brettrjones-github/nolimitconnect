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

#include <ptop_src/ptop_engine_src/BaseInfo/BaseSessionInfo.h>

#include <CoreLib/VxMutex.h>

#include <vector>

class VxNetIdent;
class VxSktBase;
class P2PEngine;

class User
{
public:
    User();
    User( P2PEngine& engine );
    User( P2PEngine& engine, VxNetIdent* netIdent );
    User( P2PEngine& engine,  VxNetIdent* netIdent, BaseSessionInfo& sessionInfo );
    User(const User& rhs );
	virtual ~User() = default;

    User&				        operator=( const User& rhs ); 

    VxMutex&					getUserMutex( void )					{ return m_UserMutex; }
    void						lockUser( void )						{ m_UserMutex.lock(); }
    void						unlockUser( void )						{ m_UserMutex.unlock(); }

    void                        setNetIdent( VxNetIdent* netIdent );
    VxNetIdent *                getNetIdent( void )                     { return m_NetIdent; }
    VxGUID&                     getMyOnlineId( void )                   { return m_MyOnlineId; }

    bool                        isInSession( void )                     { return 0 != m_SessionList.size(); }

    bool                        addSession( BaseSessionInfo& sessionInfo );
    bool                        removeSession( BaseSessionInfo& sessionInfo );

protected:
    P2PEngine&                  m_Engine;
    VxMutex                     m_UserMutex;

    VxNetIdent *                m_NetIdent{ nullptr };
    VxGUID                      m_MyOnlineId;
    std::vector<BaseSessionInfo> m_SessionList;
};
