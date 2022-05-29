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
// http://www.nolimitconnect.org
//============================================================================

#include "config_corelib.h"

#include "VxCrypto.h"
#include "VxDebug.h"

#include <memory.h>
#include <string>

#ifndef _MSC_VER
#include <sys/time.h>
#include <cstdlib>
#endif // _MSC_VER

#include "md5.h"

#include "VxParse.h"
#include <PktLib/VxCommon.h>


//=== cheezy crypto with known weeknesses ===//
// issued to public domain year 2006
//===========================================//


//============================================================================
//============================================================================
VxKey::VxKey()
	: m_bIsSet( 0 )
{
}

//============================================================================
VxKey::~VxKey()
{
	memset( m_au32Key, 0xa5, sizeof( m_au32Key ) );
}

//============================================================================
//! set key from data..
RCODE VxKey::importKey( unsigned char * pu8Data, int iLen )
{
	//vx_assert( iLen == sizeof( m_au8Key ) );
	if ( iLen == sizeof( m_au32Key ) )
	{
		memcpy( m_au32Key, pu8Data, iLen );
		m_bIsSet = true;
		return 0;
	}
	return -1;
}

//============================================================================
//! set encryption key from another
void VxKey::importKey( VxKey * poKey )
{
	memcpy( m_au32Key, poKey->m_au32Key, sizeof( m_au32Key ) );
	m_bIsSet = poKey->m_bIsSet;
}

//============================================================================
//! return key into data buffer
RCODE VxKey::exportKey( unsigned char * pu8RetKeyData, int iBufLen )
{
	if ( iBufLen >= ( int )sizeof( m_au32Key ) )
	{
		memcpy( pu8RetKeyData, m_au32Key, sizeof( m_au32Key ) );
		return 0;
	}

	return -1;
}

//============================================================================
//! export encryption key to another another
void VxKey::exportKey( VxKey * poKey )
{
	memcpy( poKey->m_au32Key, m_au32Key, sizeof( m_au32Key ) );
	poKey->m_bIsSet = m_bIsSet;
}

//============================================================================
void VxKey::exportToAsciiString( std::string& exportedKey )
{
#define myToHex(N) (unsigned char)(((N>9) ? ((N)-10+'A'):((N)+'0')))
	char buf[ CHEEZY_SYM_KEY_LEN * 2 + 1 ];
	int bufIdx = 0;
	unsigned char * keyAsBytes = (unsigned char *)m_au32Key;
	for ( int i = 0; i < CHEEZY_SYM_KEY_LEN; i++ )
	{
		unsigned char keyByte = keyAsBytes[ i ];
		buf[ bufIdx ] = myToHex( ( ( keyByte >> 4 ) & 0x0f ) );
		bufIdx++;
		buf[ bufIdx ] = myToHex( ( keyByte & 0x0f ) );
		bufIdx++;
	}

	buf[ bufIdx ] = 0;
	exportedKey = buf;
}

//============================================================================
//! make encryption key from user name and password
RCODE VxKey::setKeyFromPassword( const char *	pUserName,			// user name
									const char *	pPassword,			// password
									const char *	pSalt )				// salt
{
	std::string strPwd = pUserName;
	strPwd += pPassword;

	return setKeyFromPassword( strPwd.c_str(), (int)strPwd.size(), pSalt );
}

//============================================================================
//! make encryption key from password
RCODE VxKey::setKeyFromPassword( const char *	pPassword,			// password
	int				iPasswordLen,		// length of password	int iPasswordLen )
	const char *	pSalt )	// salt
{
	//vx_assert( pPassword );
	//vx_assert( (iPasswordLen > 0) && (iPasswordLen < 256) );
	struct MD5Context   md5c;

	if ( ( pPassword == NULL ) || ( 0 == iPasswordLen ) )
	{
		return -1;
	}
	MD5Init( &md5c );
	MD5Update( &md5c, (unsigned char *)pPassword, (unsigned int)iPasswordLen );
	MD5Final( (unsigned char *)m_au32Key, &md5c );
	m_bIsSet = true;
	return 0;
}

//============================================================================
//============================================================================
//============================================================================
VxCrypto::VxCrypto()
	: m_bIsKeyValid( 0 )
{
}


//============================================================================
//! generate key from password and set encryption key in one function call
//! NOTE: Max password len 255
RCODE VxCrypto::setPassword( const char * pPassword, int iPasswordLen )
{
	struct MD5Context   md5c;
	unsigned char       bfvec[ CHEEZY_SYM_KEY_LEN ];

	if ( pPassword == NULL )
	{
		return -1;
	}
	MD5Init( &md5c );
	MD5Update( &md5c, (unsigned char *)pPassword, (unsigned int)iPasswordLen );
	MD5Final( bfvec, &md5c );
	BlowsetKey( &m_BlowCtx, bfvec, CHEEZY_SYM_KEY_LEN );
	m_bIsKeyValid = true;
	return 0;
}
//============================================================================
//! Generate encryption key from password
RCODE VxCrypto::generateKey( const char * pPassword, int iPasswordLen, VxKey * poRetKey )
{
	struct MD5Context   md5c;
	unsigned char       bfvec[ CHEEZY_SYM_KEY_LEN ];

	if ( pPassword == NULL )
	{
		return -1;
	}

	MD5Init( &md5c );
	MD5Update( &md5c, (unsigned char *)pPassword, (unsigned int)iPasswordLen );
	MD5Final( bfvec, &md5c );
	memcpy( poRetKey->m_au32Key, bfvec, CHEEZY_SYM_KEY_LEN );
	importKey( poRetKey );
	return 0;
}
//============================================================================
//! set key used for encryption and decryption
RCODE VxCrypto::importKey( VxKey * poKey )
{
	memcpy( &m_Key, poKey, sizeof( VxKey ) );
	BlowsetKey( &m_BlowCtx, (unsigned char *)poKey->m_au32Key, CHEEZY_SYM_KEY_LEN );
	m_bIsKeyValid = true;
	return 0;
}

//============================================================================
//! encrypt some data
//! NOTE: iDataLen must be a multiple of 16
RCODE VxCrypto::encrypt( unsigned char * pu8Data, int iDataLen )
{
	if ( ( false == m_bIsKeyValid ) || ( iDataLen & 0x0f ) )
	{
		return -1;
	}

	BlowEncrypt( &m_BlowCtx, pu8Data, iDataLen );
	return 0;
}

//============================================================================
//! decrypt some data
//! NOTE: iDataLen must be a multiple of 16
RCODE VxCrypto::decrypt( unsigned char * pu8Data, int iDataLen )
{
	if ( ( false == m_bIsKeyValid ) || ( iDataLen & 0x0f ) )
	{
		return -1;
	}
	BlowDecrypt( &m_BlowCtx, pu8Data, iDataLen );
	return 0;
}

//============================================================================
//! encrypt known string
RCODE VxCrypto::EncryptKnownString( unsigned char * pu8RetData, int iDataLen )
{
	if ( ( false == m_bIsKeyValid ) || ( iDataLen != CHEEZY_SYM_KEY_LEN ) )
	{
		return -1;
	}
	memcpy( pu8RetData, CHEEZY_KNOWN_TEXT_STR, CHEEZY_SYM_KEY_LEN );
	BlowEncrypt( &m_BlowCtx, pu8RetData, iDataLen );
	return 0;
}

//============================================================================
//! verify data is known text string
RCODE VxCrypto::VerifyKnownString( unsigned char * pu8Data, int iDataLen )
{
	if ( ( false == m_bIsKeyValid ) || ( iDataLen != CHEEZY_SYM_KEY_LEN ) )
	{
		return -1;
	}
	unsigned char au8Buf[ CHEEZY_SYM_KEY_LEN ];
	memcpy( au8Buf, pu8Data, CHEEZY_SYM_KEY_LEN );
	BlowDecrypt( &m_BlowCtx, au8Buf, CHEEZY_SYM_KEY_LEN );
	return memcmp( au8Buf, CHEEZY_KNOWN_TEXT_STR, CHEEZY_SYM_KEY_LEN );
}

//============================================================================
//! fill with random data
//! NOTE: this is not truly random enough for strong encryption uses
void CheezyFillRandom( void * pvData, int iLen )
{
	unsigned long u32Seed = 0;
#ifdef _MSC_VER
	LARGE_INTEGER u64Timer;
#else
	timeval tim;
#endif

	int i;
	char * pData = (char *)pvData;
	while ( 1 )
	{
#ifdef _MSC_VER
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );
		bool bHaveMultipleCPUS = ( sysinfo.dwNumberOfProcessors > 1 );
		DWORD_PTR affinityMask;
		if ( bHaveMultipleCPUS )
		{
			affinityMask = SetThreadAffinityMask( GetCurrentThread(), 1 );
		}
		QueryPerformanceCounter( &u64Timer );
		// Restore the true affinity.
		if ( bHaveMultipleCPUS )
		{
			(void)SetThreadAffinityMask( GetCurrentThread(), affinityMask );
		}
		u32Seed = u64Timer.LowPart;
#else
		gettimeofday( &tim, NULL );
		u32Seed = tim.tv_usec;
#endif

		unsigned long u32RandCnt = u32Seed & 0x03;
		srand( u32Seed );
		rand();
		if ( iLen >= 2 )
		{
			if ( u32RandCnt )
			{
				for ( i = 0; i < (int)u32RandCnt; i++ )
				{
					*( (short *)pData ) = rand();
					while ( 0 == *( (short *)pData ) )
					{
						*( (short *)pData ) = rand();
					}
				}
				iLen -= 2;
				pData += 2;
			}
		}
		else
		{
			*pData = (char)rand();

			iLen--;
			pData++;
		}

		if ( 0 == iLen )
		{
			return;
		}

		VxSleep( 2 * u32RandCnt );
	}
}

//============================================================================
//! encrypt data with VxCryptoo
RCODE VxSymEncrypt( VxKey *		poKey,			// Symmetric key must be 16 bytes long
	char *			pDataIn,		// buffer to encrypt
	int				iDataLen,		// data length ( must be multiple of key length )
	char *			pRetBuf )		// if null then encrypted data put in pData
{
	vx_assert( poKey );
	vx_assert( pDataIn );
	vx_assert( iDataLen );
	vx_assert( 0 == ( iDataLen & 0x0f ) ); // must be multiple of 16 bytes
	char * pDataOut = pRetBuf;
	if ( 0 == pDataOut )
	{
		pDataOut = pDataIn;
	}

	VxCrypto oCrypto;
	oCrypto.importKey( poKey );
	oCrypto.encrypt( (uint8_t *)pDataOut, iDataLen );
	return 0;
}
//============================================================================
//! decrypt data with VxCryptoo
RCODE VxSymDecrypt( VxKey *			poKey,			// Symmetric key must be 16 bytes long
	char *			pDataIn,		// buffer to decrypt
	int				iDataLen,		// data length ( must be multiple of key length )
	char *			pRetBuf )		// if null then encrypted data put in pData
{
	vx_assert( poKey );
	vx_assert( pDataIn );
	vx_assert( iDataLen );
	vx_assert( 0 == ( iDataLen & 0x0f ) ); // must be multiple of 16 bytes
	char * pDataOut = pRetBuf;
	if ( 0 == pDataOut )
	{
		pDataOut = pDataIn;
	}

	VxCrypto oCrypto;
	oCrypto.importKey( poKey );
	oCrypto.decrypt( (uint8_t *)pDataOut, iDataLen );
	return 0;
}

