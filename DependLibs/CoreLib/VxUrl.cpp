//============================================================================
// Copyright (C) 2017 Brett R. Jones 
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

#include "VxUrl.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGUID.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    const char * ONLINE_ID_DELIM = "!";


    //============================================================================
    bool VxIsIPv6Address( const char *addr )
    {
        if ( NULL == addr )
        {
            return false;
        }
        std::string addrStr = addr;
        if ( addrStr.find( ":" ) != std::string::npos )
        {
            return true;
        }
        return false;
    }

    //============================================================================
    const char * VxStripIPv6ScopeID( const char *addr, std::string &buf )
    {
        std::string addrStr = addr;
        if ( true == VxIsIPv6Address( addr ) )
        {
            unsigned int pos = (int)addrStr.find( "%" );
            if ( pos != std::string::npos )
            {
                addrStr = addrStr.substr( 0, pos );
            }
        }

        buf = addrStr;
        return buf.c_str();
    }
}

//============================================================================
VxUrl::VxUrl()
{
}

//============================================================================
VxUrl::VxUrl( std::string& url )
: m_Url( url )
{
	setUrl( url.c_str() );
}

//============================================================================
VxUrl::VxUrl( const char * pUrl )
{
    setUrl( pUrl );
}

//============================================================================
VxUrl::VxUrl( const VxUrl& rhs )
{
    *this = rhs;
}

//============================================================================
VxUrl& VxUrl::operator = ( const VxUrl& rhs )
{
    if( this != &rhs )
    {
        m_Url = rhs.m_Url;
        m_FileName = rhs.m_FileName;
        m_FileExtension = rhs.m_FileExtension;
        m_ShareName = rhs.m_ShareName;
        m_strProtocol = rhs.m_strProtocol;
        m_strHost = rhs.m_strHost;
        m_Port = rhs.m_Port;
        m_strPath = rhs.m_strPath;
        m_strUser = rhs.m_strUser;
        m_strPassword = rhs.m_strPassword;
        m_strQuery = rhs.m_strQuery;
        m_strFragment = rhs.m_strFragment;
        m_strOnlineId = rhs.m_strOnlineId;
        m_OnlineId = rhs.m_OnlineId;
    }

    return *this;
}

//============================================================================
bool VxUrl::operator == ( const VxUrl& rhs ) const
{
    return m_Url == rhs.m_Url &&
        m_FileName == rhs.m_FileName &&
        m_FileExtension == rhs.m_FileExtension &&
        m_ShareName == rhs.m_ShareName &&
        m_strProtocol == rhs.m_strProtocol &&
        m_strHost == rhs.m_strHost &&
        m_Port == rhs.m_Port &&
        m_strPath == rhs.m_strPath &&
        m_strUser == rhs.m_strUser &&
        m_strPassword == rhs.m_strPassword &&
        m_strQuery == rhs.m_strQuery &&
        m_strFragment == rhs.m_strFragment &&
        m_strOnlineId == rhs.m_strOnlineId &&
        m_OnlineId == rhs.m_OnlineId;
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
            m_strUser = m_strUserPassStr.substr( 0, iColonIdx );
            m_strPassword = m_strUserPassStr.substr( iColonIdx + 1, m_strUserPassStr.length() - iColonIdx - 1 );
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
        m_strHost = m_Url.substr( iReadIdx, iSlashIdx - iReadIdx );
    }
    else
    {
        m_strHost = m_Url.substr( iReadIdx, m_Url.length() - iReadIdx );
    }

    m_Port = 80;
    size_t iPeriodIdx = m_strHost.rfind( "." );
    if( iPeriodIdx != std::string::npos )
    {
        // host name or ipv4
        // check if has :PortNumber
        size_t iColonIdx = m_strHost.rfind( COLON_DELIM );
        if( iColonIdx != std::string::npos )
        {
            std::string strHost = m_strHost;
            std::string portStr = m_strHost.substr( iColonIdx + 1, m_strHost.length() - iColonIdx - 1 );
            m_Port = atoi( portStr.c_str() );
            if( 0 == m_Port )
            {
                m_Port = 80;
            }

            m_strHost = strHost.substr( 0, iColonIdx );
        }
    }
    else
    {
        // handle IPv6 and port
        size_t iColonIdx = m_strHost.rfind( COLON_DELIM );
        if( iColonIdx > 0 )
        {
            if( strchr( m_strHost.c_str(), ']' ) )
            {
                // ipv6
                size_t iLeftBracketIdx = m_strHost.rfind( RIGHT_BRACKET_DELIM );
                if( iColonIdx != std::string::npos && iLeftBracketIdx < iColonIdx )
                {
                    std::string strHost = m_strHost;
                    m_strHost = strHost.substr( 0, iColonIdx );
                    if( 0 < m_strHost.length() )
                    {
                        if( ( m_strHost.at( 0 ) == '[' ) &&
                            ( m_strHost.at( m_strHost.length() - 1 ) == ']' ) )
                        {
                            m_strHost = m_strHost.substr( 1, iColonIdx - 2 );
                        }
                    }
                    std::string m_PortStr = strHost.substr( iColonIdx + 1, strHost.length() - iColonIdx - 1 );
                    m_Port = atoi( m_PortStr.c_str() );
                    if( 0 == m_Port )
                    {
                        m_Port = 80;
                    }
                }
            }
        }
    }

    if( iSlashIdx == std::string::npos )
    {
        return;
    }

    iReadIdx = iSlashIdx;

    // read online id if exists
    m_strOnlineId = m_Url.substr( iReadIdx + 1, m_Url.length() - (iReadIdx + 1) );
    if( VxGUID::isOnlineIdStringValid( m_strOnlineId.c_str() ) )
    {
        if( m_strOnlineId.length() == 34 )
        {
            // there is nothing past the online id but it looks valid
            m_OnlineId.fromOnlineIdString( m_strOnlineId.c_str() );
        }

        iReadIdx += m_strOnlineId.length();
        iSlashIdx = m_Url.find( SLASH_DELIM, iReadIdx );
        if( iSlashIdx == std::string::npos )
        {
            return;
        }     
    }

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
std::string VxUrl::getHostString()
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
bool VxUrl::validateUrl( bool onlineIdMustBeValid )
{
    bool isValid = !getHostString().empty() && getPort() > 0;
    if( onlineIdMustBeValid && !hasValidOnlineId() )
    {
        isValid = false;
    }

    return isValid;
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


