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

#include <QWidget> // must be declared first or linux Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiUserBase.h"

class GuiUserMgr;

class GuiUser : public GuiUserBase
{
public:
    GuiUser() = delete;
    GuiUser( AppCommon& app );
    GuiUser( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online = false );
    GuiUser( const GuiUser& rhs );
	virtual ~GuiUser() override = default;

    GuiUserMgr&                 getUserMgr( void )                          { return m_UserMgr; }

    virtual bool                setOnlineStatus( bool isOnline ) override;

protected:
    GuiUserMgr&                 m_UserMgr;
};
