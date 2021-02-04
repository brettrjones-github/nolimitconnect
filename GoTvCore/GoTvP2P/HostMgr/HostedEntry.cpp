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

#include "HostedEntry.h"

#include <CoreLib/BinaryBlob.h>
#include <CoreLib/VxDebug.h>

#include <memory.h>

//============================================================================
HostedEntry::HostedEntry( const VxConnectIdent& rhs )
{
    *((VxConnectIdent *)this) = rhs;
}

//============================================================================
HostedEntry::HostedEntry( const HostedEntry& rhs )
{
	*this = rhs;
}

//============================================================================
HostedEntry::HostedEntry( const HostListEntry& listEntry, int64_t postTime )
{
	memcpy( this, &listEntry, sizeof( HostListEntry ) );
	m_PostTimeMs = postTime;
}

//============================================================================
HostedEntry& HostedEntry::operator=( const HostedEntry& rhs )
{
	if( this != &rhs )
	{
		memcpy( this, &rhs, sizeof( HostedEntry ) );
	}

	return *this;
}

//============================================================================
void HostedEntry::setHostListEntry( HostListEntry& entry )
{
	memcpy( this, &entry, sizeof( HostListEntry ) );
}

//============================================================================
void HostedEntry::setConnectIdent( const VxConnectIdent& entry )
{
    memcpy( this, &entry, sizeof( VxConnectIdent ) );
}

//============================================================================
void HostedEntry::setPluginSettingsBlob( BinaryBlob& blob )
{
    if( blob.getBlobLen() > 0 && blob.getBlobLen() < MAX_HOSTED_BLOB_LEN )
    {

    }
    else
    {
        LogMsg( LOG_ERROR, "HostedEntry::setPluginSettingsBlob invalid blob" );
    }

}