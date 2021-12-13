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

#include <CoreLib/VxGUID.h>

#include <QWidget>

#include <vector>

class AppCommon;
class GuiUser;
class GuiUserJoinMgr;

class GuiUserJoin : public QWidget
{
public:
    GuiUserJoin() = delete;
    GuiUserJoin( AppCommon& app );
    GuiUserJoin( AppCommon& app, GuiUser* netIdent, VxGUID& sessionId, bool online = false );
    GuiUserJoin( const GuiUserJoin& rhs );
    virtual ~GuiUserJoin() = default;

    GuiUserJoinMgr&             getUserJoinMgr( void ) { return m_UserJoinMgr; }

    void                        setUser( GuiUser* user )    { m_GuiUser = user; }
    GuiUser*                    getUser( void )             { return m_GuiUser; }

    virtual bool                setOnlineStatus( bool isOnline );
    bool                        isOnline( void ) { return m_IsOnline; }
    bool                        isInSession( void ) { return m_IsOnline && m_SessionId.isVxGUIDValid(); }

    EJoinState                  getJoinState( EHostType hostType );
    bool                        setJoinState( EHostType hostType, EJoinState joinState ); // return true if state changed

protected:
    AppCommon&                  m_MyApp;
    GuiUserJoinMgr&             m_UserJoinMgr;

    GuiUser*                    m_GuiUser{ nullptr };
    VxGUID                      m_SessionId;
    bool                        m_IsOnline{ false };
    std::vector<std::pair<EHostType, EJoinState>>  m_JoinStateList;
};
