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

#include "VxXml.h"
#include <memory.h>

//============================================================================
bool XmlIsSpace( char c )
{
	const static char* pStrChars = " \t\n\r\f\v";
	return 0 != strchr( pStrChars, c );
}

//============================================================================
char XmlTolower( char c )
{
	return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

//============================================================================
bool XmlStringNoCaseBeginsWith( char const* pStr1, char const* pStr2 )
{
	while (*pStr1 != 0)
	{
		if( XmlTolower( *pStr1 ) != XmlTolower( *pStr2 ) )
		{
			return false;
		}

		++pStr1;
		++pStr2;
	}

	return true;
}

//============================================================================
void XmlCopyToLower( std::string& strDest, char const* pSrc )
{
	strDest.clear();
	while ( *pSrc ) 
		strDest.push_back(XmlTolower( *pSrc++ ) );
}

//============================================================================
bool XmlStringNoCaseEqual( char const* pStr1, char const* pStr2 )
{
	while( XmlTolower( *pStr1 ) == XmlTolower( *pStr2 ) )
	{
		if( *pStr1 == 0 ) 
			return true;
		++pStr1;
		++pStr2;
	}

	return false;
}

//============================================================================
void VxXmlParse( void * pvUserData, char* pStart, char* pEnd, VX_XML_CALLBACK_T callback )
{
	for( ; pStart != pEnd; ++pStart )
	{
		char const* startPrev = pStart;
		char const* val_start = 0;
		EXmlParseType token;

		for( ; pStart != pEnd && *pStart != '<'; ++pStart );

		if( pStart != startPrev )
		{
			if( pStart != pEnd )
			{
				vx_assert( *pStart == '<' );
				*pStart = 0;
			}

			token = eXmlStringTag;
			callback( pvUserData, token, startPrev, val_start );
			if (pStart != pEnd) 
				*pStart = '<';
		}

		if (pStart == pEnd) 
			break;

		++pStart;
		if (pStart != pEnd && pStart+8 < pEnd && XmlStringNoCaseBeginsWith("![CDATA[", pStart))
		{
			pStart += 8;
			startPrev = pStart;
			while (pStart != pEnd && !XmlStringNoCaseBeginsWith("]]>", pStart-2)) 
				++pStart;

			if (pStart == pEnd)
			{
				token = eXmlParseError;
				startPrev = "unexpected pEnd of file";
				callback( pvUserData, token, startPrev, val_start );
				break;
			}

			token = eXmlStringTag;
			char tmp = pStart[-2];
			pStart[-2] = 0;
			callback( pvUserData, token, startPrev, val_start );
			pStart[-2] = tmp;
			continue;
		}

		for (startPrev = pStart; pStart != pEnd && *pStart != '>' && !XmlIsSpace(*pStart); ++pStart);

		char* tag_name_end = pStart;

		for (; pStart != pEnd && *pStart != '>'; ++pStart);

		if (pStart == pEnd)
		{
			token = eXmlParseError;
			startPrev = "unexpected pEnd of file";
			callback(pvUserData, token, startPrev, val_start );
			break;
		}

		vx_assert(*pStart == '>');
		char save = *tag_name_end;
		*tag_name_end = 0;

		char* tag_end = pStart;
		if (*startPrev == '/')
		{
			++startPrev;
			token = eXmlEndTag;
			callback( pvUserData, token, startPrev, val_start );
		}
		else if (*(pStart-1) == '/')
		{
			*(pStart-1) = 0;
			token = eXmlEmptyTag;
			callback( pvUserData, token, startPrev, val_start );
			*(pStart-1) = '/';
			tag_end = pStart - 1;
		}
		else if (*startPrev == '?' && *(pStart-1) == '?')
		{
			*(pStart-1) = 0;
			++startPrev;
			token = eXmlDeclarationTag;
			callback( pvUserData, token, startPrev, val_start );
			*(pStart-1) = '?';
			tag_end = pStart - 1;
		}
		else if (startPrev + 5 < pStart && memcmp(startPrev, "!--", 3) == 0 && memcmp(pStart-2, "--", 2) == 0)
		{
			startPrev += 3;
			*(pStart-2) = 0;
			token = eXmlCommentTag;
			callback( pvUserData, token, startPrev, val_start );
			*(pStart-2) = '-';
			tag_end = pStart - 2;
		}
		else
		{
			token = eXmlStartTag;
			callback( pvUserData, token, startPrev, val_start );
		}

		*tag_name_end = save;

		for (char* i = tag_name_end; i < tag_end; ++i)
		{
			for (; i != tag_end && XmlIsSpace(*i); ++i);
			if (i == tag_end) break;
			startPrev = i;
			for (; i != tag_end && *i != '=' && !XmlIsSpace(*i); ++i);
			char* name_end = i;

			for (; i != tag_end && *i != '='; ++i);

			if (i == tag_end)
			{
				char cTemp = *i;
				*i = 0;
				token = eXmlUnknownContentTag;
				val_start = 0;
				callback( pvUserData, token, startPrev, val_start );
				*i = cTemp;
				break;
			}

			++i;
			for (; i != tag_end && XmlIsSpace(*i); ++i);
			if (i == tag_end || (*i != '\'' && *i != '\"'))
			{
				token = eXmlParseError;
				val_start = 0;
				startPrev = "unquoted attribute value";
				callback( pvUserData, token, startPrev, val_start );
				break;
			}

			char quote = *i;
			++i;
			val_start = i;
			for (; i != tag_end && *i != quote; ++i);
			if (i == tag_end)
			{
				token = eXmlParseError;
				val_start = 0;
				startPrev = "missing pEnd quote on attribute";
				callback( pvUserData, token, startPrev, val_start );
				break;
			}

			save = *i;
			*i = 0;
			*name_end = 0;
			token = eXmlAttibuteTag;
			callback( pvUserData, token, startPrev, val_start );
			*name_end = '=';
			*i = save;
		}
	}

	callback( pvUserData, eXmlDocEnd, "", "" );
}
