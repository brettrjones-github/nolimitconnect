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

#include "VxHttpParse.h"

#include <CoreLib/VxDefs.h>
#include <CoreLib/VxValuePair.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxDebug.h>
#include <CoreLib/VxLinuxOnly.h>

#include <ctype.h>
#include <algorithm> // for std::transform
#include <string.h>
#include <stdlib.h>

#ifdef _MSC_VER
# pragma warning( disable: 4996 ) //'stricmp': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _stricmp. See online help for details.
#endif // _MSC_VER

namespace
{
    //const char * HTTP_REQEST_LINE_DELIM = " ";
    //const char * HTTP_HEADER_LINE_DELIM = " :";
    //const char * HTTP_STATUS_LINE_DELIM = " ";
    //const char * HTTP_POST = "POST";
    //const char * HTTP_GET = "GET";
    //const char * HTTP_HEAD = "HEAD";
	const char * HTTP_LOCATION = "Location";
	const char * HTTP_HOST = "Host";
	const char * HTTP_SERVER = "Server";
    //const char * HTTP_DATE = "Date";
	const char * HTTP_CACHE_CONTROL = "Cache-Control";
    //const char * HTTP_NO_CACHE = "no-cache";
    //const char * HTTP_MAX_AGE = "max-age";
    //const char * HTTP_CONNECTION = "Connection";
    //const char * HTTP_CLOSE = "close";
    //const char * HTTP_KEEP_ALIVE = "Keep-Alive";
    //const char * HTTP_CONTENT_TYPE = "Content-Type";
	const char * HTTP_CONTENT_LENGTH = "Content-Length";
    //const char * HTTP_CONTENT_RANGE = "Content-Range";
    //const char * HTTP_CONTENT_RANGE_BYTES = "bytes";
    //const char * HTTP_RANGE = "Range";
    //const char * HTTP_TRANSFER_ENCODING = "Transfer-Encoding";
    //const char * HTTP_CHUNKED = "Chunked";

	char to_lower(char c)
	{
		return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
	}

}

//============================================================================
VxHttpParse::VxHttpParse()
{
}

//============================================================================
VxHttpParse::~VxHttpParse()
{
}

//============================================================================
VxBuffer& VxHttpParse::getContentsBuffer( void )
{
	return m_ContentsBuffer;
}

//============================================================================
bool VxHttpParse::parseHttpHeaderFromContents()
{
	char * pDataRxed = m_ContentsBuffer.getDataPtr();
	if( NULL == pDataRxed )
	{
		return false;
	}
	char * pCrLfCrLf = strstr( pDataRxed, "\r\n\r\n");
	if( NULL == pCrLfCrLf )
	{
		return false;
	}
	pCrLfCrLf[2] = 0;
	setStreamData( pDataRxed );
	uint32_t dataUsed = (uint32_t)((uint64_t)(pCrLfCrLf) - (uint64_t)(pDataRxed)) + 4;
	m_ContentsBuffer.removeData( dataUsed );
	return true;
}

//============================================================================
int VxHttpParse::getHttpContentLen()
{
	std::string strContentsLen;
	getHttpValue( HTTP_CONTENT_LENGTH, strContentsLen );
	return StdStringToInteger( strContentsLen );
}

//============================================================================
const char * VxHttpParse::getHttpValue( const char * name, std::string& strRetValue )
{
	strRetValue = "";
	if( NULL == name )
	{
		return NULL;
	}
	resetStreamReadPosition();
	std::string lineStr;
	while( readStreamLine(lineStr) ) 
	{
		VxValuePair valuePair( lineStr.c_str() );
		if( false == valuePair.hasName() ) 
		{
			continue;
		}

		if( 0 != stricmp( valuePair.getName(), name ) )
		{
			continue;
		}

		strRetValue = valuePair.getValue();
		break;
	}
	return strRetValue.c_str();
}

//============================================================================
const char * VxHttpParse::getHost( std::string& strRetValue )
{
	return getHttpValue( HTTP_HOST, strRetValue );
}

//============================================================================
const char * VxHttpParse::getServer( std::string& strRetValue )
{
	return getHttpValue( HTTP_SERVER, strRetValue );
}

//============================================================================
const char * VxHttpParse::getLocation( std::string& strRetValue )
{
	return getHttpValue( HTTP_LOCATION, strRetValue );
}

//============================================================================
const char * VxHttpParse::getCacheControl( std::string& strRetValue )
{
	return getHttpValue( HTTP_CACHE_CONTROL, strRetValue );
}

//============================================================================
int VxHttpParse::getHttpCodeNumber( void )
{
	resetStreamReadPosition();
	std::string lineStr;
	while( readStreamLine(lineStr) ) 
	{
		const char * pHTTP = strstr( lineStr.c_str(), "HTTP/" );
		if( pHTTP )
		{
			pHTTP += 5;
			if( ( isdigit( pHTTP[0] ) ) &&
				( '.' ==  pHTTP[1] ) &&
				( isdigit( pHTTP[2] ) ) &&
				( ' ' == pHTTP[3] ) )
			{
				return atoi( &pHTTP[4] );
			}
		}
	}
	return 0;
}

//============================================================================
bool VxHttpParse::isHttp( void )
{
	if( getRawData().substr(0, 5) == "HTTP/" )
	{
		return true;
	}
	return false;
}

//============================================================================
std::string VxHttpParse::getMethod( void )
{
	std::string method = "";
	if( getRawData().length() && 
		( ! isHttp() ) )
	{
		char * pTemp = (char *)getRawData().c_str();
		char * pTemp2 = strstr( pTemp, " ");
		if( pTemp2 )
		{
			*pTemp2 = 0;
			method = pTemp;
			*pTemp2 = ' ';
			std::transform( method.begin(), method.end(), method.begin(), &to_lower );

		}
	}

	return method;
}

//============================================================================
std::string VxHttpParse::readUntil( char const*& str, char delim, char const* end )
{
	vx_assert( str <= end );

	std::string ret;
	while (str != end && *str != delim)
	{
		ret += *str;
		++str;
	}
	// skip the delimiter as well
	while (str != end && *str == delim) ++str;
	return ret;
}

