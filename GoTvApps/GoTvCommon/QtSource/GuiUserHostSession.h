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
class GuiUserHost;

class GuiUserHostSession : public QWidget
{
public:
	GuiUserHostSession( QWidget* parent = nullptr );
    GuiUserHostSession( GuiUserHost* hostIdent, QWidget* parent = nullptr );
	GuiUserHostSession(	VxGUID& sessionId, GuiUserHost* hostIdent, QWidget* parent = nullptr );
    GuiUserHostSession(	EHostType userType, VxGUID& sessionId, GuiUserHost * userIdent, QWidget* parent = nullptr );
	GuiUserHostSession( const GuiUserHostSession &rhs );

	GuiUserHostSession&			operator =( const GuiUserHostSession &rhs );

    GuiUser *                   getUserIdent( void ) { return nullptr; }

    void				        setUserHost( GuiUserHost* user );
    GuiUserHost*				getUserHost( void )                         { return m_UserHost; }

    EHostType                   getHostType( void )                         { return m_HostType; }

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&					    getSessionId( void )                        { return m_SessionId; }

    VxGUID&					    getMyOnlineId( void )                       { return m_OnlineId; }
    const char *                getOnlineName( void )                       { return m_UserHost ? m_UserHost->getOnlineName() : ""; }

    bool                        setIsOnline( bool isOnline )
    {
        if( m_UserHost && m_UserHost->isOnline() != isOnline )
        {
            m_UserHost->setOnlineStatus( isOnline );
            return true;
        }

        return false;
    }

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    GuiUserHost*			    m_UserHost;
    VxGUID					    m_SessionId;
    VxGUID                      m_OnlineId;
};

