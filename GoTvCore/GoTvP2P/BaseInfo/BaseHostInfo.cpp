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

#include <config_gotvcore.h>
#include "BaseHostInfo.h"

//============================================================================
BaseHostInfo::BaseHostInfo()
    : BaseInfo()
{ 
}

//============================================================================
BaseHostInfo::BaseHostInfo( const BaseHostInfo& rhs )
    : BaseInfo( rhs )
    , m_HostType( rhs.m_HostType )
    , m_ConnectUrl( rhs.m_ConnectUrl )
{
}

//============================================================================
BaseHostInfo& BaseHostInfo::operator=( const BaseHostInfo& rhs )
{
    if( this != &rhs )
    {
        BaseInfo::operator=( rhs );
        m_HostType = rhs.m_HostType;
        m_ConnectUrl = rhs.m_ConnectUrl;
    }

    return *this;
}

//============================================================================
bool BaseHostInfo::isHostMatch( EHostType hostType, VxGUID& onlineId )
{
    return  hostType == m_HostType && onlineId == m_OnlineId;
}