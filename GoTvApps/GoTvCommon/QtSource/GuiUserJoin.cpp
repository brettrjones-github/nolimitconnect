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
#include "GuiUserJoinMgr.h"
#include "GuiParams.h"

//============================================================================
GuiUserJoin::GuiUserJoin( AppCommon& app )
    : GuiUserBase( app )
    , m_UserJoinMgr( app.getUserJoinMgr() )
{
}

//============================================================================
GuiUserJoin::GuiUserJoin( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online )
    : GuiUserBase( app, netIdent, sessionId, online )
    , m_UserJoinMgr( app.getUserJoinMgr() )
{
}

//============================================================================
GuiUserJoin::GuiUserJoin( const GuiUserJoin& rhs )
    : GuiUserBase( rhs )
    , m_UserJoinMgr( rhs.m_UserJoinMgr )
    , m_JoinStateList( rhs.m_JoinStateList )
{
}

//============================================================================
bool GuiUserJoin::setOnlineStatus( bool isOnline )
{
    if( GuiUserBase::setOnlineStatus( isOnline ) )
    {
        m_UserJoinMgr.onUserOnlineStatusChange( this, m_IsOnline );
        return true;
    }

    return false;
}

//============================================================================
EJoinState GuiUserJoin::getJoinState( EHostType hostType )
{
    EJoinState joinState = eJoinStateNone;
    for( auto& statePair : m_JoinStateList )
    {
        if( statePair.first == hostType )
        {
            joinState = statePair.second;
            break;
        }
    }

    return joinState;
}

//============================================================================
bool GuiUserJoin::setJoinState( EHostType hostType, EJoinState joinState )
{
    bool stateChanged = false;
    bool foundState = false;
    for( auto& statePair : m_JoinStateList )
    {
        if( statePair.first == hostType )
        {
            foundState = true;
            if( statePair.second != joinState )
            {
                statePair.second = joinState;
                stateChanged = true;
            }

            break;
        }
    }

    if( !foundState )
    {
        m_JoinStateList.push_back( std::make_pair( hostType, joinState ) );
        stateChanged = true;
    }

    return stateChanged;
}

