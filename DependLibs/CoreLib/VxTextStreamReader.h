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

class VxTextStreamReader
{

public:
	VxTextStreamReader();
	virtual ~VxTextStreamReader() {};

	void				setStreamData( const char* pData );
	const char*		getStreamData( void );
	bool 				readStreamLine( std::string& retStrValue );
	int					readStreamData( std::string& retStrValue, int lenToRead );
	int					skipStreamData( int iSkipLen );
	void				resetStreamReadPosition( void );
	void				unwindStreamReadPosition( int iLen );
	std::string&		getRawData( void ) { return m_strData; }

private:
	//=== vars ===//
	std::string			m_strData;
	int					m_iReadIdx;

};
