#pragma once
//============================================================================
// Copyright (C) 2015 Brett R. Jones
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

#include "VxDefs.h"

#ifdef __cplusplus

#pragma pack(push) 
#pragma pack(1)
class VxThread;
//! 20 bytes in size
class VxSha1Hash
{
public:
	VxSha1Hash();
	VxSha1Hash( uint8_t * data ) { setHashData( data ); }
	VxSha1Hash( const VxSha1Hash &rhs );
	VxSha1Hash&					operator =( const VxSha1Hash &a );
	bool						operator == ( const VxSha1Hash &a ) const;
	bool						operator != ( const VxSha1Hash &a ) const;

	bool						generateHashFromFile( const char * fileName, VxThread * workThread = 0 );
	bool						isHashValid( void )	const;
	bool						isEqualTo(  const uint8_t * hashData ) const;

	void						setHashData( const uint8_t * hashData );
	uint8_t *					getHashData( void )					{ return m_HashId; }


protected:
	uint8_t						m_HashId[20];
};
#pragma pack(pop)

#endif // __cplusplus
