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
    : QWidget( &app )
    , m_MyApp( app )
    , m_UserJoinMgr( app.getUserJoinMgr() )
{
}

//============================================================================
GuiUserJoin::GuiUserJoin( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, bool online )
    : QWidget( &app )
    , m_MyApp( app )
    , m_UserJoinMgr( app.getUserJoinMgr() )
    , m_GuiUser( guiUser )
    , m_SessionId( sessionId )
    , m_IsOnline( online )
{
}

//============================================================================
GuiUserJoin::GuiUserJoin( const GuiUserJoin& rhs )
    : QWidget( &rhs.m_MyApp )
    , m_MyApp( rhs.m_MyApp )
    , m_UserJoinMgr( rhs.m_UserJoinMgr )
    , m_GuiUser( rhs.m_GuiUser )
    , m_SessionId( rhs.m_SessionId )
    , m_IsOnline( rhs.m_IsOnline )
    , m_JoinStateList( rhs.m_JoinStateList )
{
}

//============================================================================
bool GuiUserJoin::setOnlineStatus( bool isOnline )
{
    if( m_IsOnline != isOnline )
    {
        m_IsOnline = isOnline;
        if( m_GuiUser && m_GuiUser->setOnlineStatus( isOnline ) )
        {
            m_UserJoinMgr.onUserOnlineStatusChange( this, m_IsOnline );
            return true;
        }
    }

    return m_IsOnline;
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

