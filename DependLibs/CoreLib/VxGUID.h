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
// http://www.nolimitconnect.org
//============================================================================

#include "VxDefs.h"
#include <string>

#pragma pack(push)
#pragma pack(1)

class PktBlobEntry;

//! implements unique identifier
//! 16 bytes in size

class VxGUID
{
public:
	VxGUID() = default;
	VxGUID( const VxGUID& rhs );
	VxGUID( uint64_t u64HiPart, uint64_t u64LoPart );
    //bool                        addToBlob( PktBlobEntry& blob );
    //bool                        extractFromBlob( PktBlobEntry& blob );

	VxGUID&						operator = ( const VxGUID& rhs );
	bool						operator == ( const VxGUID& rhs ) const;
	bool						operator != ( const VxGUID& rhs ) const;
	bool						operator < ( const VxGUID& rhs ) const;
	bool						operator <= ( const VxGUID& rhs ) const;
	bool						operator > ( const VxGUID& rhs ) const;
	bool						operator >= ( const VxGUID& rhs ) const;

	static void					generateNewVxGUID( VxGUID& retNewGUID );
	static VxGUID&				nullVxGUID( void );
    static VxGUID&				anybodyVxGUID( void );

	void						initializeWithNewVxGUID( void );
	void						clearVxGUID( void );

	bool						isVxGUIDValid()	const;
	std::string					toGUIDStandardFormatedString( void );

	bool						toHexString( std::string& strRetId ) const;
    void                        toHexString( char * retBuf ) const; // buffer must be at least 33 characters in length
	std::string					toHexString( void ) const;

    //! set VxGUID by converting hex string into VxGUID
    bool						fromVxGUIDHexString( const char* hexString );
    static bool					isVxGUIDHexStringValid( const char* hexString );

    bool						toOnlineIdString( std::string& strRetId ) const;  
    void                        toOnlineIdString( char * retBuf ) const;  // buffer must be at least 35 characters in length
    std::string					toOnlineIdString( void ) const;

    //! set VxGUID by converting online id string into VxGUID
    bool						fromOnlineIdString( const char* onlineIdString );
    static bool					isOnlineIdStringValid( const char* onlineIdString );

	//! get low part of online id
	uint64_t					getVxGUIDLoPart() const;
	//! get high part of online id
	uint64_t					getVxGUIDHiPart() const;
	//! return users online id
	VxGUID&						getVxGUID( void );
	bool 						getVxGUID( std::string& strRetId );
	void						setVxGUID( const char* pId );
	void						setVxGUID( VxGUID& oId );
	void						setVxGUID( uint64_t& u64HiPart, uint64_t& u64LoPart );
	void						getVxGUID( uint64_t& u64HiPart, uint64_t& u64LoPart );
	// returns 0 if equal else -1 if less or 1 if greater
	int							compareTo( VxGUID& guid );
	// returns true if guids are same value
	bool						isEqualTo( const VxGUID& guid );

	std::string					describeVxGUID( bool asOnlineId = true );

    // set bytes to network order
    void                        setToNetOrder( void );
    // set bytes to host cpu endianess
    void                        setToHostOrder( void );

    // buffer must be at least 17 characters in length
    static void                 uint64ToHexAscii( char * retBuf, const uint64_t& val );
    static char                 nibbleToHex( uint8_t val );

protected:
    uint64_t					m_u64HiPart{ 0 };
	uint64_t					m_u64LoPart{ 0 };
};

#pragma pack(pop)

//! comparison of VxGUID ids for std::map
struct cmp_vxguid 
{
	bool operator()(const VxGUID& a, const VxGUID& b) const
	{
		//! return true if a < b
		return a < b;
	}
};

