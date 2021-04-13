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
#include "GuiUserHostSession.h"
#include "GuiHelpers.h"
#include "GuiParams.h"

#include <PktLib/VxSearchDefs.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxFileInfo.h>

#include <QObject>

//============================================================================
GuiUserHostSession::GuiUserHostSession( QWidget* parent )
    : QWidget( parent )
{
}

//============================================================================
GuiUserHostSession::GuiUserHostSession( GuiUserHost* hostIdent, QWidget* parent )
    : QWidget( parent )
    , m_UserHost( hostIdent )
{
    setUserHost( m_UserHost );
}

//============================================================================
GuiUserHostSession::GuiUserHostSession( EHostType hostType, VxGUID& sessionId, GuiUserHost* hostIdent, QWidget* parent )
    : QWidget( parent )
    , m_HostType( hostType )
    , m_SessionId( sessionId )
    , m_UserHost( hostIdent )
{
    setUserHost( m_UserHost );
}

//============================================================================
GuiUserHostSession::GuiUserHostSession( const GuiUserHostSession &rhs )
    : QWidget()
    , m_HostType( rhs.m_HostType )
    , m_SessionId( rhs.m_SessionId )
    , m_UserHost( rhs.m_UserHost )
    , m_OnlineId( rhs.m_OnlineId )
{
}

//============================================================================
GuiUserHostSession& GuiUserHostSession::operator =( const GuiUserHostSession &rhs )
{
	if( this != &rhs )   
	{
        m_HostType		        = rhs.m_HostType;
        m_SessionId				= rhs.m_SessionId;
        m_UserHost		        = rhs.m_UserHost;
        m_OnlineId		        = rhs.m_OnlineId;
	}

	return *this;
}

//============================================================================
void GuiUserHostSession::setUserHost( GuiUserHost* user )
{ 
    m_UserHost = user; 
    if( m_UserHost )
    {
        m_OnlineId = m_UserHost->getMyOnlineId();
    }
}
