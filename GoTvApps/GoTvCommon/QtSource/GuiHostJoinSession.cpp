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

#include <app_precompiled_hdr.h>
#include "GuiHostJoinSession.h"
#include "GuiHostJoin.h"

//============================================================================
GuiHostJoinSession::GuiHostJoinSession( QWidget* parentWidget )
    : QWidget( parentWidget )
{
}

//============================================================================
GuiHostJoinSession::GuiHostJoinSession( EHostType hostType, GuiHostJoin* hostJoinIdent, QWidget* parentWidget )
    : QWidget( parentWidget )
    , m_HostType( hostType )
    , m_OnlineId( hostJoinIdent->getMyOnlineId() )
    , m_HostIdent( hostJoinIdent )
{
}

//============================================================================
GuiHostJoinSession::GuiHostJoinSession( const GuiHostJoinSession &rhs )
    : QWidget()
    , m_HostType( rhs.m_HostType )
    , m_OnlineId( rhs.m_OnlineId )
    , m_HostIdent( rhs.m_HostIdent )
{
}

//============================================================================
GuiHostJoinSession& GuiHostJoinSession::operator =( const GuiHostJoinSession &rhs )
{
	if( this != &rhs )   
	{
        m_HostType		        = rhs.m_HostType;
        m_HostIdent		        = rhs.m_HostIdent;
        m_OnlineId			    = rhs.m_OnlineId;
	}

	return *this;
}

//============================================================================
std::string GuiHostJoinSession::getHostUrl( void )
{
    return m_HostIdent->getMyOnlineUrl();
}

//============================================================================
std::string GuiHostJoinSession::getHostDescription( void )
{
    if( !m_HostIdent->getOnlineDescription().empty() )
    {
        return m_HostIdent->getOnlineDescription();
    }
    else
    {
        return getHostUrl();
    }
}

//============================================================================
VxGUID GuiHostJoinSession::getHostThumbId( void )
{
    return m_HostIdent->getAvatarThumbId();
}