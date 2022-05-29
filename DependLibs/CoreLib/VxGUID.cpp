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
// http://www.nolimitconnect.org
//============================================================================
#include "config_corelib.h"

#include "VxGUID.h"

#include <CoreLib/IsBigEndianCpu.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>

#include <PktLib/PktBlobEntry.h>

#include <libcrossguid/guid.h>

#ifdef TARGET_OS_ANDROID
# include "VxJava.h"
#endif //TARGET_OS_ANDROID

#include <string.h>
#include <stdio.h>

//============================================================================
static uint8_t charToHex( char cVal )
{
    if( cVal >= '0' && cVal <= '9' )
    {
        return cVal & 0x0f;
    }
    else if( cVal >= 'A' && cVal <= 'F' )
    {
        return cVal - 55;
    }
    else if( cVal >= 'a' && cVal <= 'f' )
    {
        return cVal - 87;
    }
    else
    {
        LogMsg( LOG_ERROR, "VxGUID::charToHex invalid char 0x%2.2x\n", cVal );
        return 0;
    }
}

//============================================================================
VxGUID::VxGUID( const VxGUID &rhs )
{
    *this = rhs;
}

//============================================================================
VxGUID::VxGUID( uint64_t u64HiPart, uint64_t u64LoPart )
: m_u64HiPart(u64HiPart)
, m_u64LoPart(u64LoPart)
{
}

/* commented out because of linux order of dependency linking issue
//============================================================================
bool VxGUID::addToBlob( PktBlobEntry& blob )
{
    bool result = blob.setValue( m_u64HiPart );
    result &= blob.setValue( m_u64LoPart );
    return result;
}

//============================================================================
bool VxGUID::extractFromBlob( PktBlobEntry& blob )
{
    bool result = blob.getValue( m_u64HiPart );
    result &= blob.getValue( m_u64LoPart );
    return result;
}
*/

//============================================================================
VxGUID & VxGUID::operator =( const VxGUID &rhs )
{
	if( this != &rhs )   
	{
		m_u64HiPart = rhs.m_u64HiPart;
		m_u64LoPart = rhs.m_u64LoPart;
	}

	return *this;
}

//============================================================================
bool VxGUID::operator == ( const VxGUID &a ) const
{
	return ((m_u64LoPart == a.m_u64LoPart ) && (m_u64HiPart == a.m_u64HiPart ));
}

//============================================================================
bool VxGUID::operator != ( const VxGUID &a ) const
{
	return !(*this == a);
}

//============================================================================
bool VxGUID::operator < ( const VxGUID &rhs ) const
{
    return m_u64HiPart < rhs.m_u64HiPart || ( m_u64HiPart == rhs.m_u64HiPart && m_u64LoPart < rhs.m_u64LoPart );
}

//============================================================================
bool VxGUID::operator <= ( const VxGUID &rhs ) const
{
	if( *this == rhs )
	{
		return true;
	}

    if( *this < rhs )
    {
        return true;
    }

	return false;
}

//============================================================================
bool VxGUID::operator > ( const VxGUID & rhs ) const
{
    return m_u64HiPart > rhs.m_u64HiPart || ( m_u64HiPart == rhs.m_u64HiPart && m_u64LoPart > rhs.m_u64LoPart );
}

//============================================================================
bool VxGUID::operator >= ( const VxGUID & rhs ) const
{
	if( *this == rhs )
	{
		return true;
	}

    if( *this > rhs )
    {
        return true;
    }

	return false;
}

//============================================================================
// returns 0 if equal else -1 if less or 1 if greater
int VxGUID::compareTo( VxGUID& rhs )
{
	if( *this < rhs )
		return -1;
	if( *this > rhs )
		return 1;

	return 0;
}

//============================================================================
// returns true if guids are same value
bool VxGUID::isEqualTo( const VxGUID& guid )
{
	return ( ( m_u64LoPart == guid.m_u64LoPart ) && ( m_u64HiPart == guid.m_u64HiPart ) );
}

#include <libcrossguid/guid.h>

//============================================================================
void VxGUID::generateNewVxGUID( VxGUID& retNewGUID )
{
#if 1
    // use library cross guid
#ifdef TARGET_OS_ANDROID
    GuidGenerator guidGen( VxJava::getJavaEnv() );
#else
    GuidGenerator guidGen;
#endif //TARGET_OS_ANDROID

    Guid crossGuid = guidGen.newGuid();
    if( (int)(sizeof( uint64_t ) * 2) <= crossGuid.guidByteCount() )
    {
        std::vector<unsigned char>& guidBytes = crossGuid.getGuidBytes();
        unsigned char * guidData = &guidBytes[0];
        memcpy( &retNewGUID.m_u64HiPart, guidData, sizeof( uint64_t ) );
        guidData +=  sizeof( uint64_t );
        memcpy( &retNewGUID.m_u64LoPart, guidData, sizeof( uint64_t ) );
    }
    else
    {
        LogMsg( LOG_ERROR, "VxGUID::generateNewVxGUID Failed to generate" );
    }
#else
    // custom.. have to link os specific libraries
# ifdef TARGET_OS_WINDOWS
	UuidCreate( (UUID *)&retNewGUID );
# else
	#ifdef TARGET_OS_ANDROID
		// android has no uuid.h.. get from java
		JavaToGuiGenerateGuid( retNewGUID );
	#else
		uuid_t * pUuid = (uuid_t *)&retNewGUID;
        uuid_generate_random( *pUuid );
    #endif
# endif
#endif
}

//============================================================================
VxGUID& VxGUID::nullVxGUID( void )
{
static VxGUID nullGuid;
	return nullGuid;
}

//============================================================================
VxGUID& VxGUID::anybodyVxGUID( void )
{
    const uint64_t  anybodyPart1 = 0x1F2E3D4C5B6A7986;
    const uint64_t  anybodyPart2 = 0x79865B6A3D4C1F2E;
    static VxGUID anybodyGuid( anybodyPart1, anybodyPart2 );
    return anybodyGuid;
}

//============================================================================
std::string VxGUID::toGUIDStandardFormatedString( void )
{
#ifdef TARGET_OS_WINDOWS
	unsigned char * str;
	UuidToStringA ( (UUID *)this, &str );
	std::string strFormatedUuid( ( char* )str );
	RpcStringFreeA ( &str );
	return strFormatedUuid;
#else
    #if defined(TARGET_OS_ANDROID) || defined(TARGET_OS_LINUX)
		// format string into something like 79ECEFE6-D91C-457B-B17D-3FEDD63F3F73),
		char formatedStrBuf[37];
		sprintf( formatedStrBuf, "%2.2X%2.2X%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X-%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X",
			(unsigned int)((m_u64HiPart >> 56) & 0xff),
			(unsigned int)((m_u64HiPart >> 48) & 0xff),
			(unsigned int)((m_u64HiPart >> 32) & 0xff),
			(unsigned int)((m_u64HiPart >> 24) & 0xff),
			(unsigned int)((m_u64HiPart >> 16) & 0xff),
			(unsigned int)((m_u64HiPart >> 8) & 0xff),
			(unsigned int)((m_u64HiPart ) & 0xff),
			(unsigned int)((m_u64LoPart >> 56) & 0xff),
			(unsigned int)((m_u64LoPart >> 48) & 0xff),
			(unsigned int)((m_u64LoPart >> 32) & 0xff),
			(unsigned int)((m_u64LoPart >> 24) & 0xff),
			(unsigned int)((m_u64LoPart >> 16) & 0xff),
			(unsigned int)((m_u64LoPart >> 8) & 0xff),
			(unsigned int)((m_u64LoPart ) & 0xff) );
		return formatedStrBuf;
	#else
		uuid_t * pUuid = (uuid_t *)this;
		char formatedStrBuf[37];
		uuid_unparse ( *pUuid, formatedStrBuf );
		return formatedStrBuf;
	#endif // TARGET_OS_ANDROID
#endif
}

//============================================================================
void VxGUID::initializeWithNewVxGUID( void )
{
	generateNewVxGUID( *this );
}

//============================================================================
bool VxGUID::isVxGUIDValid() const 
{ 
	return ((m_u64LoPart != 0 ) || (m_u64HiPart != 0 ));
}

//============================================================================
std::string VxGUID::toHexString( void )	
{ 
    char lclBuf[ 33 ];
    toHexString( lclBuf );
    return lclBuf; 
} 

//============================================================================
bool VxGUID::toHexString( std::string& strRetId )	
{ 
	char lclBuf[ 33 ];
    toHexString( lclBuf );
	strRetId = lclBuf; 
    return isVxGUIDValid() && !strRetId.empty();
} 

//============================================================================
// buffer must be at least 33 characters in length
void VxGUID::toHexString( char * retBuf )
{
    uint64ToHexAscii( &retBuf[ 0 ], m_u64HiPart );
    uint64ToHexAscii( &retBuf[ 16 ], m_u64LoPart );
    retBuf[ 32 ] = 0;
}

//============================================================================
//! set online id by converting hex string into U128
bool VxGUID::fromVxGUIDHexString( const char * pHexString )
{
    if( false == isVxGUIDHexStringValid( pHexString ) )
    {
        return false;
    }

    uint8_t u8Byte;
    uint64_t u64Part = 0;
    for( int i = 0; i < 8; i++ )
    {
        u64Part = u64Part << 8;
        u8Byte = (charToHex(*pHexString))<<4;
        pHexString++;
        u8Byte |= charToHex(*pHexString);
        pHexString++;
        u64Part |= u8Byte;
    }

    m_u64HiPart = u64Part;
    u64Part = 0;
    for( int i = 0; i < 8; i++ )
    {
        u64Part = u64Part << 8;
        u8Byte = (charToHex(*pHexString))<<4;
        pHexString++;
        u8Byte |= charToHex(*pHexString);
        pHexString++;
        u64Part |= u8Byte;
    }

    m_u64LoPart = u64Part;

    return true;
}

//============================================================================
bool VxGUID::isVxGUIDHexStringValid( const char * pId )
{
    if( NULL == pId )
    {
        return false;
    }

    for( int i = 0; i < 32; i++ )
    {
        char ch = pId[ i ];
        if( false == ( (('0' <= ch) && ('9' >= ch)) 
            || (('a' <= ch) && ('f' >= ch)) 
            || (('A' <= ch) && ('F' >= ch)) ) )
        {
            return false;
        }
    }

    return true;
}

//============================================================================
std::string VxGUID::toOnlineIdString( void ) const	
{ 
    char lclBuf[ 35 ];
    toOnlineIdString( lclBuf );
    return lclBuf; 
} 

//============================================================================
bool VxGUID::toOnlineIdString( std::string& strRetId ) const	
{ 
    char lclBuf[ 35 ];
    toOnlineIdString( lclBuf );
    strRetId = lclBuf; 
    return isOnlineIdStringValid( lclBuf ) && !strRetId.empty();
} 

//============================================================================
// buffer must be at least 35 characters in length
void VxGUID::toOnlineIdString( char * retBuf ) const
{
    retBuf[ 0 ] = '!';
    uint64ToHexAscii( &retBuf[ 1 ], m_u64HiPart );
    uint64ToHexAscii( &retBuf[ 17 ], m_u64LoPart );
    retBuf[ 33 ] = '!';
    retBuf[ 34 ] = 0;
}

//============================================================================
//! set online id by converting hex string into U128
bool VxGUID::fromOnlineIdString( const char * pHexString )
{
    if( false == isOnlineIdStringValid( pHexString ) )
    {
        return false;
    }

    pHexString++;
    uint8_t u8Byte;
    uint64_t u64Part = 0;
    for( int i = 0; i < 8; i++ )
    {
        u64Part = u64Part << 8;
        u8Byte = (charToHex(*pHexString))<<4;
        pHexString++;
        u8Byte |= charToHex(*pHexString);
        pHexString++;
        u64Part |= u8Byte;
    }

    m_u64HiPart = u64Part;
    u64Part = 0;
    for( int i = 0; i < 8; i++ )
    {
        u64Part = u64Part << 8;
        u8Byte = (charToHex(*pHexString))<<4;
        pHexString++;
        u8Byte |= charToHex(*pHexString);
        pHexString++;
        u64Part |= u8Byte;
    }

    m_u64LoPart = u64Part;

    return true;
}

//============================================================================
bool VxGUID::isOnlineIdStringValid( const char * pId )
{
    if( nullptr == pId )
    {
        return false;
    }

    if( '!' != pId[0] || '!' != pId[33] )
    {
        return false;
    }

    for( int i = 1; i < 33; i++ )
    {
        char ch = pId[ i ];
        if( false == ( (('0' <= ch) && ('9' >= ch)) 
            || (('a' <= ch) && ('f' >= ch)) 
            || (('A' <= ch) && ('F' >= ch)) ) )
        {
            return false;
        }
    }

    return true;
}

//============================================================================
// buffer must be at least 17 characters in length
void VxGUID::uint64ToHexAscii( char * retBuf, const uint64_t& val )
{
    uint64_t netOrderedVal = htonU64( val );
    uint8_t * byteArray = (uint8_t *)&netOrderedVal;
    int charIdx = 0;
    for( int byteIdx = 0; byteIdx < 8; byteIdx++ )
    {
        uint8_t bytePart = byteArray[ byteIdx ];
        retBuf[ charIdx++ ] = nibbleToHex( ( bytePart >> 4 ) & 0x0f );
        retBuf[ charIdx++ ] = nibbleToHex( bytePart & 0x0f );
    }

    retBuf[ 16 ] = 0;
}

//============================================================================
char VxGUID::nibbleToHex( uint8_t val )
{
    return val > 9 ? ( val - 10 ) + 'A' : val + 0x30;
}

//============================================================================
//! get low part of online id
uint64_t		VxGUID::getVxGUIDLoPart() const								{ return m_u64LoPart; }

//============================================================================
//! get high part of online id
uint64_t		VxGUID::getVxGUIDHiPart() const						{ return m_u64HiPart; }

//============================================================================
//! return users online id
VxGUID&	VxGUID::getVxGUID( void )						            { return *this; }

//============================================================================
//! return users online id as string
bool VxGUID::getVxGUID( std::string& strRetId )			            { return this->toHexString( strRetId ); }

//============================================================================
//! set users online id
void VxGUID::setVxGUID( VxGUID& oId )					            { *this = oId; }

//============================================================================
//! set users online id
void VxGUID::setVxGUID( uint64_t& u64HiPart, uint64_t& u64LoPart )	{ m_u64HiPart = u64HiPart; m_u64LoPart = u64LoPart; }

//============================================================================
//! get users online as 2 parts
void VxGUID::getVxGUID( uint64_t& u64HiPart, uint64_t& u64LoPart )	{  u64HiPart = m_u64HiPart; u64LoPart = m_u64LoPart; }

//============================================================================
void VxGUID::setVxGUID( const char * pId )					        { this->fromVxGUIDHexString( pId ); }

//============================================================================
// set bytes to network order
void VxGUID::setToNetOrder()
{
    m_u64HiPart = htonU64( m_u64HiPart );
    m_u64LoPart = htonU64( m_u64LoPart );
}

//============================================================================
// set bytes to host cpu endianess
void VxGUID::setToHostOrder()
{
    m_u64HiPart = ntohU64( m_u64HiPart );
    m_u64LoPart = ntohU64( m_u64LoPart );
}

//============================================================================
void VxGUID::clearVxGUID( void )
{
	m_u64LoPart = 0;
	m_u64HiPart = 0;
}

//============================================================================
std::string	VxGUID::describeVxGUID( void )
{
	std::string strId;
    char bufHiPart[ 33 ];
    char bufLoPart[ 33 ];

    uint64ToHexAscii( bufHiPart, m_u64HiPart );
    uint64ToHexAscii( bufLoPart, m_u64LoPart );
    strId = " 0x";
    strId += bufHiPart;
    strId += " 0x";
    strId += bufLoPart;
    strId += " ";

	return strId;
}
