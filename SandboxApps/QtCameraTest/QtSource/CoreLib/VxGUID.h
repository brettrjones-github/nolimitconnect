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


#include <string>

#pragma pack(1)
//! implements unique identifier
//! 16 bytes in size

class VxGUID
{
public:
	VxGUID();
	VxGUID( const VxGUID &rhs );
	VxGUID( uint64_t u64HiPart, uint64_t u64LoPart );

	VxGUID&						operator =( const VxGUID &a );
	bool						operator == ( const VxGUID &a ) const;
	bool						operator != ( const VxGUID &a ) const;
	bool						operator < ( const VxGUID &b ) const;
	bool						operator <= ( const VxGUID &b ) const;
	bool						operator > ( const VxGUID &b ) const;
	bool						operator >= ( const VxGUID &b ) const;

	static void					generateNewVxGUID( VxGUID& retNewGUID );
	static VxGUID&				nullVxGUID( void );
    static VxGUID&				anybodyVxGUID( void );

	void						initializeWithNewVxGUID( void );
	void						clearVxGUID( void );

	bool						isVxGUIDValid()	const;
	std::string					toGUIDStandardFormatedString( void );
	void						toHexString( std::string& strRetId );
    // buffer must be at least 33 characters in length
    void                        toHexString( char * retBuf );
	std::string					toHexString( void );
	//! set VxGUID by converting hex string into VxGUID
	bool						fromVxGUIDHexString( const char * pHexString );
	//! get low part of online id
	uint64_t					getVxGUIDLoPart() const;
	//! get high part of online id
	uint64_t					getVxGUIDHiPart() const;
	//! return users online id
	VxGUID&						getVxGUID( void );
	void						getVxGUID( std::string& strRetId );
	void						setVxGUID( const char * pId );
	void						setVxGUID( VxGUID& oId );
	void						setVxGUID( uint64_t& u64HiPart, uint64_t& u64LoPart );
	void						getVxGUID( uint64_t& u64HiPart, uint64_t& u64LoPart );
	// returns 0 if equal else -1 if less or 1 if greater
	int							compareTo( VxGUID& guid );
	// returns true if guids are same value
	bool						isEqualTo( VxGUID& guid );


	std::string					describeVxGUID( void );
	static bool					isVxGUIDHexStringValid( const char * pId );

    // set bytes to network order
    void                       setToNetOrder();
    // set bytes to host cpu endianess
    void                       setToHostOrder();

protected:
    // buffer must be at least 17 characters in length
    void                        uint64ToHexAscii( char * retBuf, uint64_t& val );
    char                        nibbleToHex( uint8_t val );

	uint64_t					m_u64HiPart;
	uint64_t					m_u64LoPart;
};
#pragma pack()

//! comparison of VxGUID ids for std::map
struct cmp_vxguid 
{
	bool operator()(const VxGUID& a, const VxGUID& b) const
	{
		//! return true if a < b
		return a < b;
	}
};

