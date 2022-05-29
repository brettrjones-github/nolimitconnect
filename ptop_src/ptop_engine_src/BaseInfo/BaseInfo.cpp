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

#include <config_appcorelibs.h>
#include "BaseInfo.h"

#include <CoreLib/VxDebug.h>
#include <PktLib/VxCommon.h>
#include <GuiInterface/IGoTv.h>
#include <ptop_src/ptop_engine_src/P2PEngine/P2PEngine.h>

//============================================================================
BaseInfo::BaseInfo()
{ 
}

//============================================================================
BaseInfo::BaseInfo( VxGUID& onlineId, int64_t modifiedTime )
    : m_OnlineId( onlineId )
    , m_InfoModifiedTime( modifiedTime )
{
    assureHasCreatorId();
}

//============================================================================
BaseInfo::BaseInfo( VxGUID& creatorId, VxGUID& assetId, int64_t modifiedTime )
    : m_OnlineId( creatorId )
    , m_ThumbId( assetId )
    , m_InfoModifiedTime( modifiedTime )
{
    assureHasCreatorId();
}

//============================================================================
BaseInfo::BaseInfo( const BaseInfo& rhs )
    : m_OnlineId( rhs.m_OnlineId )
    , m_ThumbId( rhs.m_ThumbId )
    , m_InfoModifiedTime( rhs.m_InfoModifiedTime )
{
    assureHasCreatorId();
}

//============================================================================
BaseInfo::~BaseInfo()
{
    // LogMsg( LOG_DEBUG, "~BaseInfo %p %s", this, m_ThumbId.toHexString().c_str() );
}

//============================================================================
BaseInfo& BaseInfo::operator=( const BaseInfo& rhs )
{
    if( this != &rhs )
    {
        m_OnlineId = rhs.m_OnlineId;
        m_ThumbId = rhs.m_ThumbId;
        m_InfoModifiedTime = rhs.m_InfoModifiedTime;
    }

    return *this;
}

//============================================================================
void BaseInfo::fillBaseInfo( VxNetIdent* netIdent, EHostType hostType )
{
    vx_assert( netIdent );
    m_OnlineId = netIdent->getMyOnlineId();
    m_ThumbId = netIdent->getThumbId( hostType );
    m_InfoModifiedTime = GetTimeStampMs();
}

//============================================================================
void BaseInfo::assureHasCreatorId( void )
{
    if( !m_OnlineId.isVxGUIDValid() )
    {
        m_OnlineId = GetPtoPEngine().getMyOnlineId();
    }
}
