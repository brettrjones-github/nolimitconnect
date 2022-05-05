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
#include "GuiParams.h"

//============================================================================
GuiUser::GuiUser( AppCommon& app )
    : GuiUserBase( app )
    , m_UserMgr( m_MyApp.getUserMgr() )
{
}

//============================================================================
GuiUser::GuiUser( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online )
    : GuiUserBase( app, netIdent, sessionId, online )
    , m_UserMgr( m_MyApp.getUserMgr() )
{
}

//============================================================================
GuiUser::GuiUser( const GuiUser& rhs )
    : GuiUserBase( rhs )
    , m_UserMgr( rhs.m_UserMgr )
{
}

//============================================================================
bool GuiUser::setRelayStatus( bool isRelayed )
{
    if( GuiUserBase::setRelayStatus( isRelayed ) )
    {
        m_UserMgr.onUserRelayStatusChange( this );
        return true;
    }

    return false;
}

//============================================================================
bool GuiUser::setOnlineStatus( bool isOnline )
{
    if( GuiUserBase::setOnlineStatus( isOnline ) )
    {
        m_UserMgr.onUserOnlineStatusChange( this );
        return true;
    }

    return false;
}

//============================================================================
bool GuiUser::canDirectConnectToUser( void )
{
    return m_NetIdent.isValidNetIdent() && (isOnline() || isNearby()) && !isRelayed();
}

//============================================================================
VxGUID& GuiUser::getAvatarThumbGuid( void ) 
{ 
    if( !m_NetIdent.isValidNetIdent() )
    {
        VxGUID guid;
        return guid;
    }

    return m_NetIdent.getAvatarThumbGuid();
}