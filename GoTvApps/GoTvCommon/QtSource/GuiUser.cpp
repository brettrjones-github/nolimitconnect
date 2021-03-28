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

#include "AppCommon.h"
#include "GuiUser.h"
#include "GuiUserMgr.h"

//============================================================================
GuiUser::GuiUser( AppCommon& app )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GuiUserMgr( m_MyApp.getUserMgr() )
{
}

//============================================================================
GuiUser::GuiUser( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GuiUserMgr( m_MyApp.getUserMgr() )
    , m_NetIdent( netIdent )
    , m_OnlineId( netIdent->getMyOnlineId() )
    , m_SessionId( sessionId )
{
    setOnlineStatus( online );
}

//============================================================================
GuiUser::GuiUser( const GuiUser& rhs )
    : m_MyApp( rhs.m_MyApp )
    , m_GuiUserMgr( m_MyApp.getUserMgr() )
    , m_NetIdent( rhs.m_NetIdent )
    , m_OnlineId( rhs.m_OnlineId )
    , m_SessionId( rhs.m_SessionId )
    , m_IsOnline( rhs.m_IsOnline )
    , m_HostSet( rhs.m_HostSet )
{
}

//============================================================================
void GuiUser::setNetIdent( VxNetIdent* netIdent )
{ 
    if( netIdent )
    {
        if( netIdent != m_NetIdent )
        {
            m_NetIdent = netIdent; 
            m_OnlineId = m_NetIdent->getMyOnlineId();
        }
    }
    else
    {
        LogMsg( LOG_ERROR, "GuiUser::setNetIdent null ident" );
    }
}

//============================================================================
void GuiUser::setOnlineStatus( bool isOnline )
{
    if( isOnline != m_IsOnline )
    {
        m_IsOnline = isOnline;
        m_GuiUserMgr.onUserOnlineStatusChange( this, m_IsOnline );
        if( !m_IsOnline )
        {
            m_SessionId.clearVxGUID();
        }
    }
}

//============================================================================
bool GuiUser::isInSession( void )
{
    return m_IsOnline && m_SessionId.isVxGUIDValid();
}

//============================================================================
void GuiUser::addHostType( EHostType hostType )
{
    if( hostType > eHostTypeUnknown && hostType < eMaxHostType )
    {
        m_HostSet.insert( hostType );
    }
}