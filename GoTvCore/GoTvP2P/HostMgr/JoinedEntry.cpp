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

#include "JoinedEntry.h"

#include <memory.h>

//============================================================================
JoinedEntry::JoinedEntry( const JoinedEntry& rhs )
{
    *((VxConnectIdent *)this) = rhs;
}

//============================================================================
JoinedEntry::JoinedEntry( const VxConnectIdent& rhs )
{
    *this = rhs;
}

//============================================================================
JoinedEntry::JoinedEntry( const HostListEntry& listEntry, int64_t postTime )
{
	memcpy( this, &listEntry, sizeof( HostListEntry ) );
	m_PostTimeMs = postTime;
}

//============================================================================
JoinedEntry& JoinedEntry::operator=( const JoinedEntry& rhs )
{
	if( this != &rhs )
	{
		memcpy( this, &rhs, sizeof( JoinedEntry ) );
	}

	return *this;
}

//============================================================================
void JoinedEntry::setHostListEntry( HostListEntry& entry )
{
	memcpy( this, &entry, sizeof( HostListEntry ) );
}
