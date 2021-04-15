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
#include "ConnectInfo.h"

#include <PktLib/VxSearchDefs.h>

#include <CoreLib/VxFileLists.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxGlobals.h>

#include <sys/types.h>
#include <sys/stat.h>

//============================================================================
ConnectInfo::ConnectInfo()
{ 
}

//============================================================================
ConnectInfo::ConnectInfo( const ConnectInfo& rhs )
    : m_HostOnlineId( rhs.m_HostOnlineId )
{
}

//============================================================================
ConnectInfo& ConnectInfo::operator=( const ConnectInfo& rhs ) 
{	
	if( this != &rhs )
	{
        m_HostOnlineId = rhs.m_HostOnlineId;
	}

	return *this;
}
