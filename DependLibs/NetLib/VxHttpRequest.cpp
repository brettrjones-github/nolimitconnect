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

#include "VxHttpRequest.h"
#include <CoreLib/VxUrl.h>
#include <stdio.h>
#include <string.h>

const char* VxHttpRequest::HTTP_GET = "GET";
const char* VxHttpRequest::HTTP_POST = "POST";
const char* VxHttpRequest::HTTP_HEAD = "HEAD";
const char* VxHttpRequest::HTTP_SUBSCRIBE = "SUBSCRIBE";
const char* VxHttpRequest::HTTP_UNSUBSCRIBE = "UNSUBSCRIBE";
const char* VxHttpRequest::HTTP_NOTIFY = "NOTIFY";
const char* VxHttpRequest::HTTP_CRLF = "\r\n";

//============================================================================
VxHttpRequest::VxHttpRequest()
: m_bKeepConnectionAlive(false)
{
	initDefaults();
}

//============================================================================
VxHttpRequest::VxHttpRequest( VxUrl& oUrl )
: m_bKeepConnectionAlive(false)
{
	initDefaults();
	setUrl( oUrl );
}

//============================================================================
void VxHttpRequest::initDefaults( void )
{
	setMethod( HTTP_GET );
	setHttpVersion1_1();
}

//============================================================================
void VxHttpRequest::setUrl( VxUrl& oUrl )
{
	setRequestHost( oUrl.getHost() );
	setRequestPort( oUrl.getPort() );
	setURI( oUrl.getPath() );
}

//============================================================================
void VxHttpRequest::setMethod( const char* pValue )
{
	m_strMethod = pValue;
}

//============================================================================
const char* VxHttpRequest::getMethod( void )
{
	return m_strMethod.c_str();
}

//============================================================================
const char* VxHttpRequest::getRequestHost( void )
{
	return m_strRequestingHost.c_str();
}

//============================================================================
void VxHttpRequest::setRequestHost( const char* pHost )
{
	m_strRequestingHost = pHost;
}

//============================================================================
void VxHttpRequest::setURI( const char* pValue, bool useRelativeUrl )
{
	m_strUri = pValue;
}

//============================================================================
const char* VxHttpRequest::getURI( void )
{
	return m_strUri.c_str();
}

//============================================================================
void VxHttpRequest::setRequestPort( int iPort )
{
	m_iPort = iPort;
}

//============================================================================
int VxHttpRequest::getRequestPort()
{
	return m_iPort;
}

//============================================================================
void VxHttpRequest::setHttpVersion1_0( void )
{
	m_strHttpVersion = "HTTP/1.0";
}

//============================================================================
void VxHttpRequest::setHttpVersion1_1( void )
{
	m_strHttpVersion = "HTTP/1.1";
}

//============================================================================
const char* VxHttpRequest::getHttpVersion( void )
{
	return m_strHttpVersion.c_str();
}

//============================================================================
void VxHttpRequest::setKeepConnectionAlive( bool bKeepAlive )
{
	m_bKeepConnectionAlive = bKeepAlive;
}

//============================================================================
const char* VxHttpRequest::buildGetHeader( std::string &headerBuf )
{
	headerBuf = "";
	headerBuf += getMethod();
	headerBuf += " ";
	headerBuf += getURI();
	headerBuf += " ";
	headerBuf += getHttpVersion();
	headerBuf += HTTP_CRLF;

	buildHost( headerBuf );
	headerBuf += "\r\n";


	//headerBuf += "Cache-Control: no-cache\r\n";
	headerBuf += "Accept: text/xml, application/xml\r\n";
	headerBuf += "Connection: ";
	headerBuf += m_bKeepConnectionAlive ? "Keep-Alive" : "close";
	headerBuf += HTTP_CRLF;

	//headerBuf += "Pragma: no-cache\r\n";

	return headerBuf.c_str();
}

//============================================================================
const char* VxHttpRequest::buildHost( std::string &headerBuf )
{
	headerBuf += "HOST: ";
	headerBuf += getRequestHost();
	if( ( 0 != m_iPort ) && ( 80 != m_iPort ) )
	{
		char as8Port[ 32 ];
		sprintf( as8Port, ":%d", m_iPort );
		headerBuf += as8Port;
	}

	return headerBuf.c_str();
}

//============================================================================
const char* VxHttpRequest::buildUpnpSoapPost(	std::string &headerBuf, 
												const char* pSoapAction, 
												unsigned int contentLen )
{
	setMethod( HTTP_POST );
	headerBuf = "";
	headerBuf += getMethod();
	headerBuf += " ";
	headerBuf += getURI();
	headerBuf += " ";
	headerBuf += getHttpVersion();
	headerBuf += HTTP_CRLF;

	headerBuf += "Connection: close\r\n";
	headerBuf += "Content-Type: text/xml; charset=\"utf-8\"\r\n";
	headerBuf += "SOAPAction: \"";
	headerBuf += pSoapAction;
	headerBuf += "\"\r\n";
	headerBuf += "Content-Length: ";
	char as8Len[ 32 ];
	sprintf( as8Len, "%d", contentLen );
	headerBuf += as8Len;
	headerBuf += HTTP_CRLF;

	buildHost( headerBuf );
	headerBuf += HTTP_CRLF;

	return headerBuf.c_str();
}

//============================================================================
bool VxHttpRequest::isMethod( const char* pValue )
{
	return ( 0 == strcmp( pValue, getMethod() ) );
}

//============================================================================
bool VxHttpRequest::isGetRequest( void )
{
	return isMethod( HTTP_GET );
}

//============================================================================
bool VxHttpRequest::isPostRequest( void )
{
	return isMethod( HTTP_POST );
}

//============================================================================
bool VxHttpRequest::isHeadRequest( void )
{
	return isMethod(HTTP_HEAD);
}

//============================================================================
bool VxHttpRequest::isSubscribeRequest( void )
{
	return isMethod( HTTP_SUBSCRIBE );
}

//============================================================================
bool VxHttpRequest::isUnsubscribeRequest( void )
{
	return isMethod( HTTP_UNSUBSCRIBE );
}

//============================================================================
bool VxHttpRequest::isNotifyRequest( void )
{
	return isMethod(HTTP_NOTIFY);
}

//============================================================================
bool VxHttpRequest::isKeepAlive()
{
	return m_bKeepConnectionAlive;
}

//============================================================================
std::vector<VxValuePair> * VxHttpRequest::getParameterList( std::vector<VxValuePair>& paramList )
{
	std::string uri = m_strUri;
	if( 0 >= uri.length() )
	{
		return &paramList;
	}
	std::string::size_type paramIdx = uri.find('?');
	if (paramIdx == std::string::npos)
	{
		return &paramList;
	}
	while (paramIdx != std::string::npos) 
	{
		std::string::size_type eqIdx = uri.find('=', (paramIdx+1));
		std::string name = uri.substr(paramIdx+1, eqIdx-(paramIdx+1));
		std::string::size_type nextParamIdx = uri.find('&', (eqIdx+1));
		std::string value = uri.substr(eqIdx+1, ((nextParamIdx != std::string::npos) ? nextParamIdx : uri.length()) - (eqIdx+1));
		paramList.push_back( VxValuePair( name.c_str(), value.c_str() ) );
		paramIdx = nextParamIdx;
	}
	return &paramList;

}

//============================================================================
const char* VxHttpRequest::getParameterValue( const char*name, std::string & strRetParamValue )
{
	/*
	strRetParamValue = "";
	std::vector<VxHttpValuePair> paramList;
	getParameterList( paramList );
	std::vector<VxHttpValuePair>::iterator iter;
	for( iter = paramList.begin(); iter != paramList.end() ++iter )
	{
		if( 0 == stricmp( name, iter->getName() ) )
		{
			strRetParamValue = iter->getValue();
			break;
		}
	}
	return strRetParamValue.c_str();
	*/
	return NULL;
}

