//============================================================================
// Copyright (C) 201 Brett R. Jones 
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

#include <config_gotvcore.h>
#include "BaseInfo.h"

//============================================================================
BaseInfo::BaseInfo()
{ 
}

//============================================================================
BaseInfo::BaseInfo( const BaseInfo& rhs )
    : m_OnlineId( rhs.m_OnlineId )
    , m_ThumbId( rhs.m_ThumbId )
    , m_InfoModifiedTime( rhs.m_InfoModifiedTime )
{
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
