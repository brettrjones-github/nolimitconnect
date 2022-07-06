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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include <GuiInterface/IDefs.h>

#include "GuiUser.h"

#include <QWidget>

class P2PEngine;
class VxGUID;
class PluginSetting;

class GuiUserSessionBase : public QWidget
{
    Q_OBJECT
public:
	GuiUserSessionBase( QWidget* parent = nullptr );
    GuiUserSessionBase( GuiUser* hostIdent, QWidget* parent = nullptr );
	GuiUserSessionBase(	EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent, QWidget* parent = nullptr );
	GuiUserSessionBase( const GuiUserSessionBase &rhs );
    virtual ~GuiUserSessionBase() {};

	GuiUserSessionBase&			operator =( const GuiUserSessionBase &rhs );

    void				        setUserIdent( GuiUser* user );
    GuiUser*				    getUserIdent( void )                        { return m_Userdent; }

    EHostType                   getHostType( void )                         { return m_HostType; }

    void						setSessionId( VxGUID& sessionId )           { m_SessionId = sessionId; }
    VxGUID&					    getSessionId( void )                        { return m_SessionId; }

    VxGUID&					    getMyOnlineId( void )                       { return m_OnlineId; }
    std::string                 getOnlineName( void )                       { return m_Userdent ? m_Userdent->getOnlineName() : ""; }

protected:
	//=== vars ===//
    EHostType                   m_HostType{ eHostTypeUnknown };
    GuiUser*			    	m_Userdent;
    VxGUID					    m_SessionId;
    VxGUID                      m_OnlineId;
};

