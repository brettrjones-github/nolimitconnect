#pragma once
//============================================================================
// Copyright (C) 2014 Brett R. Jones
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
class VxOnlineStatusFlags
{
public:
	VxOnlineStatusFlags() = default;
    VxOnlineStatusFlags( const VxOnlineStatusFlags &rhs );
    VxOnlineStatusFlags&        operator =( const VxOnlineStatusFlags &rhs );
    bool                        addToBlob( PktBlobEntry& blob );
    bool                        extractFromBlob( PktBlobEntry& blob );

	void						setIsOnline( bool bIsOnline );
	bool						isOnline( void );
	void						setIsNearby( bool bIsNearby );
	bool						isNearby( void );
	void						setHasTextOffers( bool hasOffers );
	bool						getHasTextOffers( void );

	void						setIsFromSearchPkt( bool bIsFromSearch );
	bool						isFromSearchPkt( void );

	//=== vars ===//
    uint8_t						m_u8OnlineStatusFlags{ 0 };
};

#pragma pack(pop)


