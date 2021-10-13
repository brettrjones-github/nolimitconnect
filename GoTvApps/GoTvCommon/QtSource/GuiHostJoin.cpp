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
#include "GuiHostJoin.h"
#include "GuiUserMgr.h"
#include "GuiParams.h"
#include <ptop_src/ptop_engine_src/HostJoinMgr/HostJoinInfo.h>

//============================================================================
GuiHostJoin::GuiHostJoin( AppCommon& app )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GuiUserMgr( app.getUserMgr() )
{
}

//============================================================================
GuiHostJoin::GuiHostJoin( AppCommon& app, VxNetIdent* netIdent, VxGUID& sessionId, bool online )
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
GuiHostJoin::GuiHostJoin( const GuiHostJoin& rhs )
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
bool GuiHostJoin::isMyself( void )
{
    return getMyOnlineId() == m_GuiUserMgr.getMyOnlineId();
}

//============================================================================
QString GuiHostJoin::describeMyFriendshipToHim( void )
{
    return GuiParams::describeFriendship( m_NetIdent ? m_NetIdent->getMyFriendshipToHim() : eFriendStateIgnore );
}

//============================================================================
QString GuiHostJoin::describeHisFriendshipToMe( void )
{
    return GuiParams::describeFriendship( m_NetIdent ? m_NetIdent->getHisFriendshipToMe() : eFriendStateIgnore );
}

//============================================================================
void GuiHostJoin::setNetIdent( VxNetIdent* netIdent )
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
        LogMsg( LOG_ERROR, "GuiHostJoin::setNetIdent null ident" );
    }
}

//============================================================================
void GuiHostJoin::setOnlineStatus( bool isOnline )
{
    if( isOnline != m_IsOnline )
    {
        m_IsOnline = isOnline;
    }
}

//============================================================================
bool GuiHostJoin::isInSession( void )
{
    return m_IsOnline && m_SessionId.isVxGUIDValid();
}

//============================================================================
void GuiHostJoin::addHostType( EHostType hostType )
{
    if( hostType > eHostTypeUnknown && hostType < eMaxHostType )
    {
        m_HostSet.insert( hostType );
    }
}

//============================================================================
EJoinState GuiHostJoin::getJoinState( EHostType hostType )
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
bool GuiHostJoin::setJoinState( EHostType hostType, EJoinState joinState )
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
        m_JoinStateList.push_back( std::make_pair(hostType, joinState ) );
        stateChanged = true;
    }

    return stateChanged;
}

//============================================================================
int GuiHostJoin::getHostRequestCount( void )
{
    return geRequestStateCount( eJoinStateJoinRequested );
}

//============================================================================
int GuiHostJoin::geRequestStateCount( EJoinState joinState )
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