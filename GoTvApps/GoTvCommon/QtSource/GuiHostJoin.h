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

class GuiHostJoinMgr;

class GuiHostJoin : public GuiUserBase
{
public:
    GuiHostJoin() = delete;
    GuiHostJoin( AppCommon& app );
    GuiHostJoin( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online = false );
    GuiHostJoin( const GuiHostJoin& rhs );
	virtual ~GuiHostJoin() = default;

    GuiHostJoinMgr&             getHostJoineMgr( void ) { return m_HostJoinMgr; }

    virtual bool                setOnlineStatus( bool isOnline ) override;

    EJoinState                  getJoinState( EHostType hostType );
    bool                        setJoinState( EHostType hostType, EJoinState joinState ); // return true if state changed
    int                         getHostRequestCount( void );
    int                         geRequestStateCount( EJoinState joinState );

protected:
    GuiHostJoinMgr&             m_HostJoinMgr;
    std::vector<std::pair<EHostType, EJoinState>>  m_JoinStateList;
};
