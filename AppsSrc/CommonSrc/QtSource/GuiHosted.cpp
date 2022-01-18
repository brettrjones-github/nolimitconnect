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
GuiHosted::GuiHosted( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId )
    : QWidget( &app )
    , m_MyApp( app )
    , m_HostedListMgr( app.getHostedListMgr() )
    , m_GuiUser( guiUser )
    , m_SessionId( sessionId )
{
}

//============================================================================
GuiHosted::GuiHosted( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, HostedInfo& hostedInfo )
    : QWidget( &app )
    , m_MyApp( app )
    , m_HostedListMgr( app.getHostedListMgr() )
    , m_GuiUser( guiUser )
    , m_SessionId( sessionId )
    , m_HostType( hostedInfo.getHostType() )
    , m_OnlineId( hostedInfo.getOnlineId() )
    , m_IsFavorite( hostedInfo.getIsFavorite() )
    , m_ConnectedTimestampMs( hostedInfo.getConnectedTimestamp() )
    , m_JoinedTimestampMs( hostedInfo.getJoinedTimestamp() )
    , m_HostInfoTimestampMs( hostedInfo.getHostInfoTimestamp() )
    , m_HostInviteUrl( hostedInfo.getHostInviteUrl() )
    , m_HostTitle( hostedInfo.getHostTitle() )
    , m_HostDesc( hostedInfo.getHostDescription() )
{
}

//============================================================================
GuiHosted::GuiHosted( const GuiHosted& rhs )
    : QWidget( &rhs.m_MyApp )
    , m_MyApp( rhs.m_MyApp )
    , m_HostedListMgr( rhs.m_HostedListMgr )
    , m_GuiUser( rhs.m_GuiUser )
    , m_SessionId( rhs.m_SessionId )
    , m_IsFavorite( rhs.m_IsFavorite )
    , m_ConnectedTimestampMs( rhs.m_ConnectedTimestampMs )
    , m_JoinedTimestampMs( rhs.m_JoinedTimestampMs )
    , m_HostInfoTimestampMs( rhs.m_HostInfoTimestampMs )
    , m_HostInviteUrl( rhs.m_HostInviteUrl )
    , m_HostTitle( rhs.m_HostTitle )
    , m_HostDesc( rhs.m_HostDesc )
{
}
