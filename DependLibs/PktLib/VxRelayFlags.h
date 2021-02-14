#pragma once
//============================================================================
// Copyright (C) 2013 Brett R. Jones
// Issued to MIT style license by Brett R. Jones in 2017
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

#include <inttypes.h>

#pragma pack(push) 
#pragma pack(1)

class PktBlobEntry;
//! 1 byte in size
class VxRelayFlags
{
public:
	VxRelayFlags() = default;
    VxRelayFlags( const VxRelayFlags& rhs );
    VxRelayFlags&               operator =( const VxRelayFlags& rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	void						setRelayFlags( uint8_t u8RelayFlags );
	uint8_t						getRelayFlags( void );

	void						setHasRelay( bool bHasRelay );
	bool						hasRelay( void );

	void						setRequiresRelay( bool bRequiresRelay );
	bool						requiresRelay( void );
	void						setIsMyPreferedRelay( bool bRequiresRelay );
	bool						isMyPreferedRelay( void );

	//=== vars ===//
    uint8_t						m_u8RelayFlags{ 0 };
};

#pragma pack(pop)