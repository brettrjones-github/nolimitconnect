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

#include "GuiHostedListSession.h"
#include "GuiUser.h"

//============================================================================
GuiHostedListSession::GuiHostedListSession( QWidget* parent )
    : QWidget( parent )
{
}

//============================================================================
GuiHostedListSession::GuiHostedListSession( HostedId& hostedId, GuiHosted* guiHosted, QWidget* parent )
    : QWidget( parent )
    , m_HostedId( hostedId )
    , m_GuiHosted( guiHosted )
{
}

//============================================================================
GuiHostedListSession::GuiHostedListSession( const GuiHostedListSession &rhs )
    : QWidget()
    , m_HostedId( rhs.m_HostedId )
    , m_GuiHosted( rhs.m_GuiHosted )
    , m_SessionId( rhs.m_SessionId )
{
}

//============================================================================
GuiHostedListSession& GuiHostedListSession::operator =( const GuiHostedListSession &rhs )
{
	if( this != &rhs )   
	{
        m_HostedId              = rhs.m_HostedId;
        m_SessionId				= rhs.m_SessionId;
        m_GuiHosted             = rhs.m_GuiHosted;
	}

	return *this;
}

//============================================================================
std::string GuiHostedListSession::getHostUrl( void )
{
    if( m_GuiHosted )
    {
        return m_GuiHosted->getHostInviteUrl();
    }

    return "";
}

//============================================================================
std::string GuiHostedListSession::getHostTitle( void )
{
    if( m_GuiHosted )
    {
        return m_GuiHosted->getHostTitle();
    }

    return "";
}

//============================================================================
std::string GuiHostedListSession::getHostDescription( void )
{
    if( m_GuiHosted )
    {
        return m_GuiHosted->getHostDescription();
    }

    return "";
}

//============================================================================
VxGUID& GuiHostedListSession::getSessionId( void )
{
    if( !m_SessionId.isVxGUIDValid() )
    {
        m_SessionId.initializeWithNewVxGUID();
    }

    return m_SessionId; 
}

//============================================================================
VxGUID GuiHostedListSession::getHostThumbId( void )
{
    if( m_GuiHosted && m_GuiHosted->getUser() )
    {
        return m_GuiHosted->getUser()->getHostThumbId( m_HostedId.getHostType(), true );
    }

    VxGUID emptyThumbId;
    return emptyThumbId;
}