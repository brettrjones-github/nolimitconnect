//============================================================================
// Copyright (C) 2019 Brett R. Jones 
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
#pragma once

#include "HostListEntry.h"

class RandomConnectEntry : public HostListEntry
{
public:
	RandomConnectEntry() = default;
	RandomConnectEntry( const RandomConnectEntry& rhs );
	RandomConnectEntry( const HostListEntry& anchorEntry, int64_t postTime );
	~RandomConnectEntry() = default;

	RandomConnectEntry& operator=( const RandomConnectEntry& rhs );

	void						setHostListEntry( HostListEntry& entry );
	void						setPostTimeMs( int64_t postTimeMs )			{ m_PostTimeMs = postTimeMs; }
	int64_t						getPostTimeMs( void )						{ return m_PostTimeMs; }
	HostListEntry *			    getHostListEntry( void )					{ return (HostListEntry *)this; }

private:
	int64_t						m_PostTimeMs = 0;
};

