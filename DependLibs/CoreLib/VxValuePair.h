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
// http://www.nolimitconnect.org
//============================================================================

#include <CoreLib/config_corelib.h>

#include <string>

class VxValuePair
{
public:
	VxValuePair( const char* lineStr);
	VxValuePair( const char* name, const char* value );
	VxValuePair( VxValuePair * header );

	~VxValuePair();

	void						setName( const char* name );
	const char*				getName( void );
	bool						hasName( void );
	void						setValue( const char*value );
	const char*				getValue( void );

private:
	//=== vars ===//
	std::string					m_strName;
	std::string					m_strValue;
};



