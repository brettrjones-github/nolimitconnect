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

#include "VxPtopUrl.h"

#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/VxGUID.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace
{
	const char * PROTOCOL_DELIM = "://";
	const char * COLON_DELIM = ":";
	const char * SLASH_DELIM = "/";
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
VxPtopUrl::VxPtopUrl( std::string& url )
{
	setUrl( url );
}

//============================================================================
VxPtopUrl::VxPtopUrl( const char* urlIn )
{
    setUrl( urlIn );
}

//============================================================================
VxPtopUrl::VxPtopUrl( const VxPtopUrl& rhs )
{
    *this = rhs;
}

//============================================================================
VxPtopUrl& VxPtopUrl::operator = ( const VxPtopUrl& rhs )
{
    if( this != &rhs )
    {
        m_Url = rhs.m_Url;
    }

    return *this;
}

//============================================================================
bool VxPtopUrl::operator == ( const VxPtopUrl& rhs ) const
{
    return m_Url == rhs.m_Url;
}

//============================================================================
bool VxPtopUrl::isValid( void )
{
    return m_Port && m_OnlineId.isVxGUIDValid() && !m_Protocol.empty() && !m_Host.empty();
}

//============================================================================
void VxPtopUrl::setUrl( const char* urlIn )
{
    if( urlIn )
    {
        std::string url = urlIn;
        setUrl( url );
    }
}

//============================================================================
void VxPtopUrl::setUrl( std::string& url )
{
    m_Url = url;

    // protocol
    size_t iReadIdx = m_Url.find( PROTOCOL_DELIM );
    if( iReadIdx != std::string::npos )
    {
        m_Protocol = m_Url.substr( 0, iReadIdx );
        iReadIdx += strlen( PROTOCOL_DELIM );
        StdStringTrim( m_Protocol );
    }
    else
    {
        iReadIdx = 0;
    }

    // host
    size_t iSlashIdx = m_Url.find( SLASH_DELIM, iReadIdx );
    if( iSlashIdx != std::string::npos )
    {
        m_Host = m_Url.substr( iReadIdx, iSlashIdx - iReadIdx );
    }
    else
    {
        m_Host = m_Url.substr( iReadIdx, m_Url.length() - iReadIdx );
    }

    m_Port = 0;
    size_t iPeriodIdx = m_Host.rfind( "." );
    if( iPeriodIdx != std::string::npos )
    {
        // host name or ipv4
        // check if has :PortNumber
        size_t iColonIdx = m_Host.rfind( COLON_DELIM );
        if( iColonIdx != std::string::npos )
        {
            std::string strHost = m_Host;
            std::string portStr = m_Host.substr( iColonIdx + 1, m_Host.length() - iColonIdx - 1 );
            m_Port = atoi( portStr.c_str() );
            if( 0 == m_Port )
            {
                m_Port = 80;
            }

            m_Host = strHost.substr( 0, iColonIdx );
        }
    }
    else
    {
        // handle IPv6 and port
        size_t iColonIdx = m_Host.rfind( COLON_DELIM );
        if( iColonIdx > 0 )
        {
            if( strchr( m_Host.c_str(), ']' ) )
            {
                // ipv6
                size_t iLeftBracketIdx = m_Host.rfind( ']' );
                if( iColonIdx != std::string::npos && iLeftBracketIdx < iColonIdx )
                {
                    std::string strHost = m_Host;
                    m_Host = strHost.substr( 0, iColonIdx );
                    if( 0 < m_Host.length() )
                    {
                        if( ( m_Host.at( 0 ) == '[' ) &&
                            ( m_Host.at( m_Host.length() - 1 ) == ']' ) )
                        {
                            m_Host = m_Host.substr( 1, iColonIdx - 2 );
                        }
                    }

                    std::string m_PortStr = strHost.substr( iColonIdx + 1, strHost.length() - iColonIdx - 1 );
                    m_Port = atoi( m_PortStr.c_str() );
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
    std::string strOnlineId = m_Url.substr( iReadIdx + 1, m_Url.length() - (iReadIdx + 1) );
    if( VxGUID::isOnlineIdStringValid( strOnlineId.c_str() ) )
    {
        if( strOnlineId.length() == 34 )
        {
            // there is nothing past the online id but it looks valid
            m_OnlineId.fromOnlineIdString( strOnlineId.c_str() );
        }

        iReadIdx += strOnlineId.length();
        iSlashIdx = m_Url.find( SLASH_DELIM, iReadIdx );
        if( iSlashIdx == std::string::npos )
        {
            return;
        }     
    }
}
