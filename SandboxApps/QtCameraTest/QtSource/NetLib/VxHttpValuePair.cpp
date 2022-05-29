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

#include "VxHttpValuePair.h"
#include <CoreLib/VxParse.h>

//namespace
//{
//	const char LF = '\n';
//	const char CR = '\r';
//}

//============================================================================
VxHttpValuePair::VxHttpValuePair( const char * pStr )
{
	if( NULL == pStr )
	{
		return;
	}

	int iColonIdx = -1;
	int iSearchIdx = 0;
	while( 0 != pStr[iSearchIdx] )
	{
		if( ':' == pStr[iSearchIdx] )
		{
			iColonIdx = iSearchIdx;
			break;
		}
		++iSearchIdx;
	}
	if( 0 > iColonIdx )
	{
		return;
	}

	std::string strLine = pStr;
	m_strName = strLine.substr(0, iColonIdx).c_str();
	StdStringTrim( m_strName );
	m_strValue = strLine.substr( iColonIdx + 1, strLine.length() - iColonIdx - 1 ).c_str();
	StdStringTrim( m_strValue );
}

//============================================================================
VxHttpValuePair::VxHttpValuePair( const char * name, const char * value )
{
	setName(name);
	setValue(value);
}

//============================================================================
VxHttpValuePair::VxHttpValuePair( VxHttpValuePair * header )
{
	setName( header->getName() );
	setValue( header->getValue() );
}

//============================================================================
VxHttpValuePair::~VxHttpValuePair()
{

}

//============================================================================
void VxHttpValuePair::setName(const char *name)
{
	m_strName = name;
}

//============================================================================
const char * VxHttpValuePair::getName( void )
{
	return m_strName.c_str();
}

//============================================================================
bool VxHttpValuePair::hasName( void )
{
	return( 0 < m_strName.length() );
}

//============================================================================
void VxHttpValuePair::setValue(const char *value)
{
	m_strValue = value;
}

//============================================================================
const char * VxHttpValuePair::getValue( void )
{
	return m_strValue.c_str();
}
