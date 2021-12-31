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

#include "GuiHosted.h"
#include "AppCommon.h"
#include "GuiHostedListMgr.h"
#include "GuiParams.h"
#include <ptop_src/ptop_engine_src/HostListMgr/HostedInfo.h>

//============================================================================
GuiHosted::GuiHosted( AppCommon& app )
    : QWidget( &app )
    , m_MyApp( app )
    , m_HostedListMgr( app.getHostedListMgr() )
{
}

//============================================================================
GuiHosted::GuiHosted( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, bool online )
    : QWidget( &app )
    , m_MyApp( app )
    , m_HostedListMgr( app.getHostedListMgr() )
    , m_GuiUser( guiUser )
    , m_SessionId( sessionId )
    , m_IsOnline( online )
{
}

//============================================================================
GuiHosted::GuiHosted( const GuiHosted& rhs )
    : QWidget( &rhs.m_MyApp )
    , m_MyApp( rhs.m_MyApp )
    , m_HostedListMgr( rhs.m_HostedListMgr )
    , m_GuiUser( rhs.m_GuiUser )
    , m_SessionId( rhs.m_SessionId )
    , m_IsOnline( rhs.m_IsOnline )
    , m_JoinStateList( rhs.m_JoinStateList )
{
}

//============================================================================
bool GuiHosted::setOnlineStatus( bool isOnline )
{
    if( m_IsOnline != isOnline )
    {
        m_IsOnline = isOnline;
        if( m_GuiUser && m_GuiUser->setOnlineStatus( isOnline ) )
        {
            m_HostedListMgr.onUserOnlineStatusChange( this, m_IsOnline );
            return true;
        }
    }

    return false;
}

//============================================================================
EJoinState GuiHosted::getJoinState( EHostType hostType )
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
bool GuiHosted::setJoinState( EHostType hostType, EJoinState joinState )
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

//============================================================================
int GuiHosted::getHostRequestCount( void )
{
    return getRequestStateCount( eJoinStateJoinRequested );
}

//============================================================================
int GuiHosted::getRequestStateCount( EJoinState joinState )
{
    int stateCnt = 0;
    for( auto& joinPair : m_JoinStateList )
    {
        if( joinPair.second == joinState )
        {
            stateCnt++;
        }
    }

    return stateCnt;
}


//============================================================================
void GuiHosted::getRequestStateHosts( EJoinState joinState, std::vector<EHostType>& hostRequests )
{
    hostRequests.clear();
    for( auto& joinPair : m_JoinStateList )
    {
        if( joinPair.second == joinState )
        {
            hostRequests.push_back( joinPair.first );
        }
    }
}