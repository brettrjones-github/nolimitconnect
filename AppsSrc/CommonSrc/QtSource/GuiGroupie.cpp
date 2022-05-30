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
// http://www.nolimitconnect.org
//============================================================================
#include <QWidget> // must be declared first or Qt 6.2.4 will error in qmetatype.h 2167:23: array subscript value ‘53’ is outside the bounds

#include "GuiGroupie.h"
#include "AppCommon.h"
#include "GuiGroupieListMgr.h"
#include "GuiParams.h"
#include <ptop_src/ptop_engine_src/GroupieListMgr/GroupieInfo.h>

#include <CoreLib/VxGlobals.h>

//============================================================================
GuiGroupie::GuiGroupie( AppCommon& app )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GroupieListMgr( app.getGroupieListMgr() )
{
}

//============================================================================
GuiGroupie::GuiGroupie( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GroupieListMgr( app.getGroupieListMgr() )
    , m_GuiUser( guiUser )
    , m_SessionId( sessionId )
{
}
//============================================================================
GuiGroupie::GuiGroupie( AppCommon& app, GuiUser* guiUser, GroupieId& groupieId, VxGUID& sessionId )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GroupieListMgr( app.getGroupieListMgr() )
    , m_GuiUser( guiUser )
    , m_GroupieId( groupieId )
    , m_SessionId( sessionId )
{
    setGroupieInfoTimestamp( GetGmtTimeMs() );
    setGroupieUrl( guiUser->getNetIdent().getMyOnlineUrl() );
    setGroupieTitle( guiUser->getOnlineName() );
    setGroupieDescription( guiUser->getOnlineDescription() );
}

//============================================================================
GuiGroupie::GuiGroupie( AppCommon& app, GuiUser* guiUser, VxGUID& sessionId, GroupieInfo& groupieInfo )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GroupieListMgr( app.getGroupieListMgr() )
    , m_GuiUser( guiUser )
    , m_GroupieId( groupieInfo.getGroupieId() )
    , m_SessionId( sessionId )
    , m_IsFavorite( groupieInfo.getIsFavorite() )
    , m_GroupieInfoTimestampMs( groupieInfo.getGroupieInfoTimestamp() )
    , m_GroupieUrl( groupieInfo.getGroupieUrl() )
    , m_GroupieTitle( groupieInfo.getGroupieTitle() )
    , m_GroupieDesc( groupieInfo.getGroupieDescription() )
{
}

//============================================================================
GuiGroupie::GuiGroupie( const GuiGroupie& rhs )
    : QWidget( &rhs.m_MyApp )
    , m_MyApp( rhs.m_MyApp )
    , m_GroupieListMgr( rhs.m_GroupieListMgr )
    , m_GuiUser( rhs.m_GuiUser )
    , m_GroupieId( rhs.m_GroupieId )
    , m_SessionId( rhs.m_SessionId )
    , m_IsFavorite( rhs.m_IsFavorite )
    , m_GroupieInfoTimestampMs( rhs.m_GroupieInfoTimestampMs )
    , m_GroupieUrl( rhs.m_GroupieUrl )
    , m_GroupieTitle( rhs.m_GroupieTitle )
    , m_GroupieDesc( rhs.m_GroupieDesc )
{
}
