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
#pragma once

#include "HostListEntry.h"

// MAX_PKT_LEN						16384
#define MAX_HOSTED_BLOB_LEN (16384 - 1024)

class BinaryBlob;

class HostedEntry : public HostListEntry
{
public:
	HostedEntry() = default;
    HostedEntry( const VxConnectIdent& rhs );
	HostedEntry( const HostedEntry& rhs );
	HostedEntry( const HostListEntry& listEntry, int64_t postTime );
	~HostedEntry() = default;

	HostedEntry& operator=( const HostedEntry& rhs );

    void						setConnectIdent( const VxConnectIdent& entry );
	void						setHostListEntry( HostListEntry& entry );
    void						setPluginSettingsBlob( BinaryBlob& blob );

	void						setPostTimeMs( int64_t postTimeMs )			{ m_PostTimeMs = postTimeMs; }
	int64_t						getPostTimeMs( void )					    { return m_PostTimeMs; }
	HostListEntry*			    getHostListEntry( void )				    { return (HostListEntry *)this; }

private:
    int64_t						m_PostTimeMs{ 0 };
    uint16_t                    m_Res1{ 0 };

    uint8_t                     m_BlobVersion{ 0 };
    uint8_t                     m_BlobType{ 0 };
    uint8_t                     m_BlobNetworkOrder{ 0 };
    uint8_t                     m_Res2{ 0 };
    uint16_t                    m_BlobLen{ 0 };
    
    char                        m_BlobData[ MAX_HOSTED_BLOB_LEN + 16 ];

};

