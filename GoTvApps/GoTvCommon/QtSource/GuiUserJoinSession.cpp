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

#include "GuiUserJoinSession.h"
#include "GuiUserJoin.h"

//============================================================================
GuiUserJoinSession::GuiUserJoinSession( QWidget* parent )
    : QWidget( parent )
{
}

//============================================================================
GuiUserJoinSession::GuiUserJoinSession( GuiUserJoin* hostIdent, QWidget* parent )
    : QWidget( parent )
    , m_UserJoin( hostIdent )
{
    setUserJoin( m_UserJoin );
}

//============================================================================
GuiUserJoinSession::GuiUserJoinSession( EHostType hostType, VxGUID& sessionId, GuiUserJoin* hostIdent, QWidget* parent )
    : QWidget( parent )
    , m_HostType( hostType )
    , m_UserJoin( hostIdent )
    , m_SessionId( sessionId )
{
    setUserJoin( m_UserJoin );
}

//============================================================================
GuiUserJoinSession::GuiUserJoinSession( const GuiUserJoinSession &rhs )
    : QWidget()
    , m_HostType( rhs.m_HostType )
    , m_UserJoin( rhs.m_UserJoin )
    , m_SessionId( rhs.m_SessionId )
    , m_OnlineId( rhs.m_OnlineId )
{
}

//============================================================================
GuiUserJoinSession& GuiUserJoinSession::operator =( const GuiUserJoinSession &rhs )
{
	if( this != &rhs )   
	{
        m_HostType		        = rhs.m_HostType;
        m_SessionId				= rhs.m_SessionId;
        m_UserJoin		        = rhs.m_UserJoin;
        m_OnlineId		        = rhs.m_OnlineId;
	}

	return *this;
}

//============================================================================
std::string GuiUserJoinSession::getOnlineName( void )
{ 
    return m_UserJoin ? m_UserJoin->getUser()->getOnlineName() : ""; 
}

//============================================================================
bool GuiUserJoinSession::setIsOnline( bool isOnline )
{
    if( m_UserJoin && m_UserJoin->isOnline() != isOnline )
    {
        m_UserJoin->setOnlineStatus( isOnline );
        return true;
    }

    return false;
}

//============================================================================
void GuiUserJoinSession::setUserJoin( GuiUserJoin* user )
{ 
    m_UserJoin = user; 
    if( m_UserJoin )
    {
        m_OnlineId = m_UserJoin->getUser()->getMyOnlineId();
    }
}
