//============================================================================
// Copyright (C) 2017 Brett R. Jones 
// Issued to MIT style license by Brett R. Jones in 2017
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software 
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// brett.jones@engineer.com
// http://www.gotvptop.net
//============================================================================

#include "VxUrl.h"
#include "VxSktUtil.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>

#include <stdio.h>
#include <stdlib.h>
namespace
{
	const char * PROTOCOL_DELIM = "://";
	const char * USER_DELIM = "@";
	const char * COLON_DELIM = ":";
	const char * SLASH_DELIM = "/";
	const char * POUND_SIGN_DELIM = "#";
	const char * QUESTION_DELIM = "?";
    //const char * LEFT_BRACKET_DELIM = "[";
	const char * RIGHT_BRACKET_DELIM = "]";
}

//============================================================================
VxUrl::VxUrl()
: m_Url( "" )
, m_FileName( "" )
, m_FileExtension( "" )
, m_ShareName( "" )
, m_Port( 80 )
{
}

//============================================================================
VxUrl::VxUrl( std::string& url )
: m_Url( url )
, m_FileName( "" )
, m_FileExtension( "" )
, m_ShareName( "" )
, m_Port( 80 )
{
	setUrl( url.c_str() );
}

//============================================================================
VxUrl::VxUrl( const char * pUrl )
{
	setUrl( pUrl );
}
//============================================================================
void VxUrl::setUrl( const char * pUrl )
{
	m_Url = pUrl;

	// protocol
	size_t iReadIdx = m_Url.find( PROTOCOL_DELIM );
    if( iReadIdx != std::string::npos )
	{
		m_strProtocol = m_Url.substr( 0, iReadIdx );
		iReadIdx += strlen( PROTOCOL_DELIM );
		StdStringTrim( m_strProtocol );
	}
	else
	{
		iReadIdx = 0;
	}

	// user and password
	size_t iAtSymbolIdx = m_Url.find( USER_DELIM, iReadIdx );
    if( iAtSymbolIdx != std::string::npos )
	{
		std::string m_strUserPassStr = m_Url.substr( iReadIdx, iAtSymbolIdx - iReadIdx );
		size_t iColonIdx = m_strUserPassStr.find( COLON_DELIM );
        if( iColonIdx != std::string::npos )
		{
			m_strUser = m_strUserPassStr.substr(0, iColonIdx);
			m_strPassword = m_strUserPassStr.substr(iColonIdx + 1, m_strUserPassStr.length() - iColonIdx -1);
		}
		else
		{
			m_strUser = m_strUserPassStr;
		}
		iReadIdx = iAtSymbolIdx + 1;
	}
	// host
	size_t iSlashIdx = m_Url.find( SLASH_DELIM, iReadIdx );
    if( iSlashIdx != std::string::npos )
	{
		m_strHost = m_Url.substr(iReadIdx, iSlashIdx - iReadIdx);
	}
	else
	{
		m_strHost = m_Url.substr(iReadIdx, m_Url.length() - iReadIdx);
	}

	// handle IPv6 and port
	m_Port = 80;

	size_t iColonIdx = m_strHost.rfind( COLON_DELIM );
	if( iColonIdx > 0 )
	{
		if( strchr( m_strHost.c_str(), ']' ) )
		{
			// ipv6
			size_t iLeftBracketIdx = m_strHost.rfind( RIGHT_BRACKET_DELIM );
            if (iColonIdx != std::string::npos && iLeftBracketIdx < iColonIdx)
			{
				std::string strHost = m_strHost;
				m_strHost = strHost.substr( 0, iColonIdx );
				if (0 < m_strHost.length()) 
				{
					if( ( m_strHost.at(0) == '[' ) &&
						( m_strHost.at( m_strHost.length() - 1 ) == ']') )
					{
						m_strHost = m_strHost.substr( 1, iColonIdx - 2 );
					}
				}
				std::string m_PortStr = strHost.substr(iColonIdx + 1, strHost.length() - iColonIdx -1);
				m_Port = atoi(m_PortStr.c_str());
			}
		}
		else
		{
			std::string strHost = m_strHost;
			std::string m_PortStr = m_strHost.substr(iColonIdx + 1, m_strHost.length() - iColonIdx -1);
			m_Port = atoi(m_PortStr.c_str());
			m_strHost = strHost.substr( 0, iColonIdx );
		}

	}

    if( iSlashIdx == std::string::npos )
	{
		return;
	}

	iReadIdx = iSlashIdx;

	// path and fragment
	m_strPath = m_Url.substr(iReadIdx, m_Url.length() - iReadIdx);
	size_t sharpIdx = m_strPath.find( POUND_SIGN_DELIM );
    if (sharpIdx != std::string::npos)
	{
		std::string m_strPathStr = m_strPath;
		m_strPath = m_strPathStr.substr(0, sharpIdx);
		m_strFragment = m_strPathStr.substr(sharpIdx + 1, m_strPathStr.length() - sharpIdx -1);
	}
	StdStringTrim( m_strPath );

	size_t questionIdx = m_strPath.find( QUESTION_DELIM );
    if (questionIdx != std::string::npos)
	{
		std::string m_strPathStr = m_strPath;
		m_strPath = m_strPathStr.substr(0, questionIdx);
		m_strQuery = m_strPathStr.substr(questionIdx + 1, m_strPathStr.length() - questionIdx -1);
	}
}

//============================================================================
const char * VxUrl::getProtocol()
{
	return m_strProtocol.c_str();
}

//============================================================================
const char * VxUrl::getUser()
{
	return m_strUser.c_str();
}

//============================================================================
const char * VxUrl::getPassword()
{
	return m_strPassword.c_str();
}

//============================================================================
const char * VxUrl::getHost()
{
	return m_strHost.c_str();
}

//============================================================================
uint16_t VxUrl::getPort()
{
	return m_Port;
}

//============================================================================
const char * VxUrl::getPath()
{
	return m_strPath.c_str();
}

//============================================================================
void VxUrl::setPath( const char * pPathAndFileName )
{
	if( strstr(  pPathAndFileName, "http://" ) )
	{
		setUrl( pPathAndFileName );
	}
	else
	{
		m_strPath = pPathAndFileName;
	}
}

//============================================================================
const char * VxUrl::getQuery()
{
	return m_strQuery.c_str();
}

//============================================================================
const char * VxUrl::getFragment()
{
	return m_strFragment.c_str();
}

//============================================================================
bool VxUrl::isAbsoluteUrl()
{
	return (0 < m_strProtocol.length());
}

//============================================================================
const char * VxUrl::getRelativeURL( std::string& strRetUrl, bool bWithParam )
{
	if( false == isAbsoluteUrl() ) 
	{
		if( (0 < m_Url.length()) && 
			( '/' != m_Url.at(0) ) )
		{
			strRetUrl = "/";
			strRetUrl += m_Url;
		}
	}
	else 
	{
		strRetUrl = getPath();
		if( true == bWithParam ) 
		{
			std::string strQuery = getQuery();
			if( 0 < strQuery.length() )
			{
				strRetUrl += "?";
				strRetUrl += strQuery;
			}
		}
		if( StdStringEndsWith( m_Url, "/") ) 
		{
			strRetUrl = m_Url.substr( 0, m_Url.length() - 1 );
		}
	}
	return strRetUrl.c_str();
}

//============================================================================
const char * VxUrl::getAbsoluteURL( std::string& strRetAbsoluteUrl )
{
	std::string strRelativeUrl;
	std::string strPort;
	strRetAbsoluteUrl = "";
	strRetAbsoluteUrl += getProtocol();
	strRetAbsoluteUrl += PROTOCOL_DELIM;
	strRetAbsoluteUrl += getHost();
	strRetAbsoluteUrl += COLON_DELIM;
	StdStringFormat( strPort, "%d", getPort() );
	strRetAbsoluteUrl += strPort;
	strRetAbsoluteUrl += getRelativeURL(strRelativeUrl);
	return strRetAbsoluteUrl.c_str();
}

//============================================================================
const char * VxUrl::getHostURL( const char * host, int port, const char * uri, std::string& buf )
{
	std::string hostStr = host;
	if( true == VxIsIPv6Address( host ) ) 
	{
		VxStripIPv6ScopeID(host, hostStr);
		hostStr = "[";
		hostStr += hostStr.c_str();
		hostStr += "]";
	}
	std::string portStr;
	buf = "http://";
	buf += hostStr;
	buf += ":";
	buf += StdStringFromInteger( portStr, port );
	buf += uri;
	return buf.c_str();
}

//============================================================================
// return true if has :// in path
bool VxUrl::isURL( std::string& strFile )
{
	return ( strFile.find( "://" ) != std::string::npos );
}

//============================================================================
void VxUrl::setFileName( const std::string& fileName )
{
	m_FileName = fileName;
	VxFileUtil::makeForwardSlashPath( m_FileName );

	size_t slash = m_FileName.find_last_of( "/" );
	size_t period = m_FileName.find_last_of( '.' );
	if ( period != std::string::npos && (slash == std::string::npos || period > slash) )
	{
		m_FileExtension = m_FileName.substr( period + 1 );
		StdStringTrim( m_FileExtension );
		StdStringMakeLower( m_FileExtension );
	}
	else
	{
		m_FileExtension = "";
	}

	slash = m_FileName.find_first_of( "/" );
	if( slash == std::string::npos )
	{
		m_ShareName = m_FileName;
	}
	else
	{
		m_ShareName = m_FileName.substr( 0, slash );
	}
};


