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

#include "GuiUser.h"

#include <QWidget>

class P2PEngine;
class VxGUID;
class GuiUserJoin;

class GuiUserJoinSession : public QWidget
{
public:
	GuiUserJoinSession( QWidget* parent = nullptr );
    GuiUserJoinSession( GuiUserJoin* hostIdent, QWidget* parent = nullptr );
	GuiUserJoinSession(	VxGUID& sessionId, GuiUserJoin* hostIdent, QWidget* parent = nullptr );
    GuiUserJoinSession(	EHostType userType, VxGUID& sessionId, GuiUserJoin * userIdent, QWidget* parent = nullptr );
	GuiUserJoinSession( const GuiUserJoinSession &rhs );

	GuiUserJoinSession&			operator =( const GuiUserJoinSession &rhs );

    GuiUser *                   getUserIdent( void ) { return nullptr; }

    void				        setUserJoin( GuiUserJoin* user );
    GuiUserJoin*				getUserJoin( void )                         { return m_UserJoin; }

    EHostType                   getHostType( void )                         { return m_HostType; }

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&					    getSessionId( void )                        { return m_SessionId; }

    VxGUID&					    getMyOnlineId( void )                       { return m_OnlineId; }
    std::string                 getOnlineName( void );

    bool                        setIsOnline( bool isOnline );

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    GuiUserJoin*			    m_UserJoin;
    VxGUID					    m_SessionId;
    VxGUID                      m_OnlineId;
};

