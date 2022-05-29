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
#include "UserJoinInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
UserJoinInfo::UserJoinInfo()
    : BaseJoinInfo()
{ 
}

//============================================================================
UserJoinInfo::UserJoinInfo( const UserJoinInfo& rhs )
    : BaseJoinInfo( rhs )
    , m_NetIdent( rhs.m_NetIdent )
    , m_HostFlags( rhs.m_HostFlags )
    , m_GroupieId( rhs.m_GroupieId )
    , m_HostUrl( rhs.m_HostUrl )
    , m_ConnectionId( rhs.m_ConnectionId )
    , m_SessionId( rhs.m_SessionId )
{
}

//============================================================================
UserJoinInfo& UserJoinInfo::operator=( const UserJoinInfo& rhs ) 
{	
	if( this != &rhs )
	{
        BaseJoinInfo::operator = ( rhs );
        m_NetIdent = rhs.m_NetIdent;
        m_HostFlags = rhs.m_HostFlags;
        m_GroupieId = rhs.m_GroupieId;
        m_HostUrl = rhs.m_HostUrl;
        m_ConnectionId = rhs.m_ConnectionId;
        m_SessionId = rhs.m_SessionId;
    }

	return *this;
}
