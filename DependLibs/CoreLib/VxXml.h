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

#include "VxDebug.h"

#include <string>

enum EXmlParseType
{
	eXmlParseError,
	eXmlStartTag,
	eXmlEndTag,
	eXmlEmptyTag,
	eXmlDeclarationTag,
	eXmlStringTag,
	eXmlAttibuteTag,
	eXmlCommentTag,
	eXmlUnknownContentTag,
	eXmlDocEnd
};

// callback example void XmlCallback( void * pvUserData, EXmlParseType eXmlType, const char* pName, const char* pValue );

typedef void 	(*VX_XML_CALLBACK_T)( void *, EXmlParseType,  char const *, char const * );

bool XmlIsSpace(char c);
char XmlTolower(char c);
bool XmlStringNoCaseBeginsWith(char const* s1, char const* s2);
void XmlCopyToLower( std::string& strDest, char const* pSrc );
bool XmlStringNoCaseEqual(char const* s1, char const* s2);
void VxXmlParse( void * pvUserData, char* pStart, char* pEnd, VX_XML_CALLBACK_T callback );
