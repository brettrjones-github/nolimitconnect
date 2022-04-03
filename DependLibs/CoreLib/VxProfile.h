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

#include "VxFileUtil.h"
#include <string>

#ifndef ENUM_RADIX
#define ENUM_RADIX 1
enum EnumRadix
{
	BASE10=10,
	BASE16=16
};
#endif //ENUM_RADIX

// ini entry example
// [ section ]
// key = value

class VxProfile : public VxFileUtil
{
public:

	/// read a binary value from an entry in a ini file
	long getProfileLong(		const char *	pFileName,			// name of file to access
								const char *	pSection,			// which section to read from
								const char *	pKey,				// the key in the specified section
								long			lDef,				// default value to return if error or entry doesn't exist
                                enum EnumRadix	eRadix = BASE10 );	// BASE10=read as decimal, BASE16=read as hexadecimal

	/// write a binary value to an entry in a ini file
	RCODE setProfileLong(	const char *	pFileName,			// name of file to access
							const char *	pSection,			// which section to write to
							const char *	pKey,				// which key to write
							const long		lValue,				// value to write
                            enum EnumRadix	eRadix = BASE10 );	// BASE10=write as decimal, BASE16=write as hexadecimal

	/// read a floating decimal point value from an entry in a ini file
	double getProfileDouble(	const char *	pFileName,	// name of file to access
								const char *	pSection,	// which section to read from
								const char *	pKey, 		// which key to read
								double			dDef );		// default value to return if error or entry doesn't exist

	/// write an floating decimal point value to a entry in a ini file
	RCODE setProfileDouble( const char *	pFileName,	// name of file to access
							const char *	pSection,	// which section to write to
							const char *	pKey,		// which key to write
							double			dValue );	// value to write

	/// read a string from a entry in a ini file
	int getProfileString(	const char *pFileName,		// name of file to access
							const char *pSection,		// which section to read from
							const char *pKey,			// which key to read
							const char *pDef,			// default value to return if error or entry doesn't exist
							char *		pRetBuf,		// buffer to write string into
							int			iRetBufLen );	// maximum bytes that can be written into the buffer

	/// read a string from a entry in a ini file
	int getProfileString(	const char *	pFileName,		// name of file to access
							const char *	pSection,		// which section to read from
							const char *	pKey,			// which key to read
							const char *	pDef,			// default value to return if error or entry doesn't exist
							std::string &	csRetValue );	// value read from ini

	/// write a string from a entry in a ini file
	RCODE setProfileString( const char *pFileName,	// name of file to access
							const char *pSection,	// which section to write to
							const char *pKey,		// which key to write
							const char *pStr, ... );// string to write ( can be formatted like printf )

};
