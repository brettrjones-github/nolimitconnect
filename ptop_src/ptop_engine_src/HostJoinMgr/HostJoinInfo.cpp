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

#include <config_appcorelibs.h>
#include "HostJoinInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxPtopUrl.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
HostJoinInfo::HostJoinInfo()
    : BaseJoinInfo()
{ 
}

//============================================================================
HostJoinInfo::HostJoinInfo( const HostJoinInfo& rhs )
    : BaseJoinInfo( rhs )
    , m_NetIdent( rhs.m_NetIdent )
    , m_FriendState( rhs.m_FriendState )
    , m_HostFlags( rhs.m_HostFlags )
    , m_GroupieId( rhs.m_GroupieId )
    , m_UserUrl( rhs.m_UserUrl )
    , m_ConnectionId( rhs.m_ConnectionId )
    , m_SessionId( rhs.m_SessionId )    
{
}

//============================================================================
HostJoinInfo& HostJoinInfo::operator=( const HostJoinInfo& rhs ) 
{	
	if( this != &rhs )
	{
        BaseJoinInfo::operator = ( rhs );
        m_NetIdent = rhs.m_NetIdent;
        m_FriendState = rhs.m_FriendState;
        m_HostFlags = rhs.m_HostFlags;
        m_GroupieId = rhs.m_GroupieId;
        m_UserUrl = rhs.m_UserUrl;
        m_ConnectionId = rhs.m_ConnectionId;
        m_SessionId = rhs.m_SessionId;
    }

	return *this;
}

//============================================================================
bool HostJoinInfo::isUrlValid( void )
{
    VxPtopUrl ptopUrl( m_UserUrl );
    return ptopUrl.isValid() && ptopUrl.getOnlineId() == getOnlineId();
}
