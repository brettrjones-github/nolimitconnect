#include "RandomConnectEntry.h"

#include <memory.h>

//============================================================================
RandomConnectEntry::RandomConnectEntry( const RandomConnectEntry& rhs )
{
	*this = rhs;
}

//============================================================================
RandomConnectEntry::RandomConnectEntry( const HostListEntry& anchorEntry, int64_t postTime )
{
	memcpy( this, &anchorEntry, sizeof( HostListEntry ) );
	m_PostTimeMs = postTime;
}

//============================================================================
RandomConnectEntry& RandomConnectEntry::operator=( const RandomConnectEntry& rhs )
{
	if( this != &rhs )
	{
		memcpy( this, &rhs, sizeof( RandomConnectEntry ) );
	}

	return *this;
}

//============================================================================
void RandomConnectEntry::setHostListEntry( HostListEntry& entry )
{
	memcpy( this, &entry, sizeof( HostListEntry ) );
}
