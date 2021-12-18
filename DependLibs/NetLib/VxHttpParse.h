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

#include <AppDependLibrariesConfig.h>

#include <CoreLib/VxTextStreamReader.h>
#include <CoreLib/VxBuffer.h>

class VxHttpParse : public VxTextStreamReader 
{
public:
	VxHttpParse();
	virtual ~VxHttpParse();

	VxBuffer&					getContentsBuffer( void );

	virtual bool				parseHttpHeaderFromContents();

	virtual const char *		getHttpValue( const char *name, std::string& strRetValue );

	virtual const char *		getHost( std::string& strRetValue );
	virtual const char *		getServer( std::string& strRetValue );
	virtual const char *		getLocation( std::string& strRetValue );
	virtual const char *		getCacheControl( std::string& strRetValue );
	virtual int					getHttpContentLen();
	virtual int					getHttpCodeNumber( void );
	std::string					getMethod( void );
	bool						isHttp( void );
	std::string					readUntil( char const*& str, char delim, char const* end );

	//=== vars ===//
	VxBuffer					m_ContentsBuffer;
};

