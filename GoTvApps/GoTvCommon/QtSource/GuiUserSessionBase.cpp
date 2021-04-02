//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include <app_precompiled_hdr.h>
#include "GuiUserSessionBase.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileInfo.h>

#include <QObject>

//============================================================================
GuiUserSessionBase::GuiUserSessionBase( QWidget* parent )
    : QWidget( parent )
{
}

//============================================================================
GuiUserSessionBase::GuiUserSessionBase( GuiUser* hostIdent, QWidget* parent )
    : QWidget( parent )
    , m_Userdent( hostIdent )
{
    setUserIdent( m_Userdent );
}

//============================================================================
GuiUserSessionBase::GuiUserSessionBase( EHostType hostType, VxGUID& sessionId, GuiUser* hostIdent, QWidget* parent )
    : QWidget( parent )
    , m_HostType( hostType )
    , m_SessionId( sessionId )
    , m_Userdent( hostIdent )
{
    setUserIdent( m_Userdent );
}

//============================================================================
GuiUserSessionBase::GuiUserSessionBase( const GuiUserSessionBase &rhs )
    : QWidget()
    , m_HostType( rhs.m_HostType )
    , m_SessionId( rhs.m_SessionId )
    , m_Userdent( rhs.m_Userdent )
    , m_OnlineId( rhs.m_OnlineId )
{
}

//============================================================================
GuiUserSessionBase& GuiUserSessionBase::operator =( const GuiUserSessionBase &rhs )
{
	if( this != &rhs )   
	{
        m_HostType		        = rhs.m_HostType;
        m_SessionId				= rhs.m_SessionId;
        m_Userdent		        = rhs.m_Userdent;
        m_OnlineId		        = rhs.m_OnlineId;
	}

	return *this;
}

//============================================================================
void GuiUserSessionBase::setUserIdent( GuiUser* user )
{ 
    m_Userdent = user; 
    if( m_Userdent )
    {
        m_OnlineId = m_Userdent->getMyOnlineId();
    }
}
