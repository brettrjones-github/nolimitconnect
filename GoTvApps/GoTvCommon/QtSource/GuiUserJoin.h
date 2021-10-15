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

#include "GuiUserBase.h"

class GuiUserJoinMgr;

class GuiUserJoin : public GuiUserBase
{
public:
    GuiUserJoin() = delete;
    GuiUserJoin( AppCommon& app );
    GuiUserJoin( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online = false );
    GuiUserJoin( const GuiUserJoin& rhs );
	virtual ~GuiUserJoin() = default;

    GuiUserJoinMgr&             getUserJoinMgr( void )                  { return m_UserJoinMgr; }

    virtual bool                setOnlineStatus( bool isOnline ) override;

    EJoinState                  getJoinState( EHostType hostType );
    bool                        setJoinState( EHostType hostType, EJoinState joinState ); // return true if state changed

protected:
    GuiUserJoinMgr&             m_UserJoinMgr;
    std::vector<std::pair<EHostType, EJoinState>>  m_JoinStateList;
};
