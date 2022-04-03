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

#include <QWidget>

#include <PktLib/GroupieId.h>

#include <vector>

class AppCommon;
class GuiUser;
class GuiUserJoinMgr;
class UserJoinInfo;

class GuiUserJoin : public QWidget
{
public:
    GuiUserJoin() = delete;
    GuiUserJoin( AppCommon& app );
    GuiUserJoin( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, bool online = false );
    GuiUserJoin( AppCommon& app, GuiUser* guiUser, UserJoinInfo* userJoinInfo );
    GuiUserJoin( const GuiUserJoin& rhs );
    virtual ~GuiUserJoin() = default;

    GuiUserJoinMgr&             getUserJoinMgr( void )                  { return m_UserJoinMgr; }

    void                        setUser( GuiUser* user )                { m_GuiUser = user; }
    GuiUser*                    getUser( void )                         { return m_GuiUser; }

    void                        setGroupieId( GroupieId& groupieId )    { m_GroupieId = groupieId; }
    GroupieId&                  getGroupieId( void )                    { return m_GroupieId; }

    bool                        setJoinState( EJoinState joinState );
    EJoinState                  getJoinState( void )                    { return m_JoinState; }

    bool                        isOnline( void );
    bool                        isInSession( void )                     { return isOnline() && m_SessionId.isVxGUIDValid(); }

protected:
    AppCommon&                  m_MyApp;
    GuiUserJoinMgr&             m_UserJoinMgr;

    GuiUser*                    m_GuiUser{ nullptr };
    GroupieId                   m_GroupieId;
    EJoinState                  m_JoinState{ eJoinStateNone };
    VxGUID                      m_SessionId;
};
