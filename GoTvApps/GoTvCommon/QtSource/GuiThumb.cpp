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
#include "GuiThumb.h"
#include "GuiThumbMgr.h"
#include "GuiParams.h"

#include <GoTvCore/GoTvP2P/ThumbMgr/ThumbInfo.h>

//============================================================================
GuiThumb::GuiThumb( AppCommon& app )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GuiThumbMgr( m_MyApp.getThumbMgr() )
{
}

//============================================================================
GuiThumb::GuiThumb( AppCommon& app, ThumbInfo* thumbInfo, VxGUID& sessionId )
    : QWidget( &app )
    , m_MyApp( app )
    , m_GuiThumbMgr( m_MyApp.getThumbMgr() )
    , m_ThumbInfo( thumbInfo )
    , m_SessionId( sessionId )
{
    updateThumbInfoIds();
}

//============================================================================
GuiThumb::GuiThumb( const GuiThumb& rhs )
    : m_MyApp( rhs.m_MyApp )
    , m_GuiThumbMgr( m_MyApp.getThumbMgr() )
    , m_ThumbInfo( rhs.m_ThumbInfo )
    , m_SessionId( rhs.m_SessionId )
    , m_ThumbId( rhs.m_ThumbId )
    , m_CreatorId( rhs.m_CreatorId )
{
}

//============================================================================
void GuiThumb::setThumbInfo( ThumbInfo* thumbInfo )
{
    m_ThumbInfo = thumbInfo;
    updateThumbInfoIds();
}

//============================================================================
void GuiThumb::updateThumbInfoIds( void )
{
    if( m_ThumbInfo )
    {
        m_CreatorId = m_ThumbInfo->getAssetUniqueId();
        m_CreatorId = m_ThumbInfo->getCreatorId();
    }
}
