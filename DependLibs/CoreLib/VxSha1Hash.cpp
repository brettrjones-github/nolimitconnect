
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
// http://www.nolimitconnect.org
//============================================================================
#include "config_corelib.h"

#include "VxSha1Hash.h"
#include "SHA1.h"

//============================================================================
VxSha1Hash::VxSha1Hash()
{
	memset( m_HashId, 0, sizeof( m_HashId ) );
}

//============================================================================
VxSha1Hash::VxSha1Hash( const VxSha1Hash &rhs )
{
	*this = rhs;
}

//============================================================================
VxSha1Hash& VxSha1Hash::operator =( const VxSha1Hash &rhs )
{
	if( this != &rhs )   
	{
		memcpy( m_HashId, rhs.m_HashId, sizeof( m_HashId ) );
	}

	return *this;
}

//============================================================================
bool VxSha1Hash::operator == ( const VxSha1Hash &a ) const
{
	return ( 0 == memcmp( m_HashId, a.m_HashId, sizeof( m_HashId ) ) ) ? true : false;
}

//============================================================================
bool VxSha1Hash::operator != ( const VxSha1Hash &a ) const
{
	return ! (*this == a);
}

//============================================================================
bool VxSha1Hash::isHashValid()	const
{
	uint32_t * longPtr = (uint32_t *)m_HashId;
	for( unsigned int i = 0; i < 4; i++ )
	{
		if( 0 != longPtr[0] )
		{
			return true;
		}
	}

	return false;
}

//============================================================================
bool VxSha1Hash::isEqualTo(  const uint8_t * hashData ) const
{
	bool isEqual = false;
	if( hashData )
	{
		isEqual = (0 == memcmp( hashData, m_HashId, sizeof( m_HashId ) ) );
	}

	return isEqual;
}


//============================================================================
void VxSha1Hash::setHashData( const uint8_t * data )			
{ 
	if( data )
	{
		memcpy( m_HashId, data, sizeof( m_HashId ) ); 
	}
	else
	{
		memset( m_HashId, 0, sizeof( m_HashId ) ); 
	}
}

//============================================================================
bool VxSha1Hash::generateHashFromFile( const char * fileName, VxThread * workThread )
{
	CSHA1 shaInstance;
	memset( m_HashId, 0, sizeof( m_HashId ) );
	bool result = shaInstance.HashFile( fileName, workThread );
	if( result )
	{
		shaInstance.Final();
		shaInstance.GetHash( m_HashId );
	}

	return result;
}
