#pragma once
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
// bjones.engineer@gmail.com
// http://www.nolimitconnect.com
//============================================================================

#include <CoreLib/config_corelib.h>

#include <string>

class VxUrl
{
public:
	VxUrl();
	VxUrl( const char * pUrl );
	VxUrl( std::string& url );
    VxUrl( const VxUrl& rhs );

    VxUrl&                      operator = ( const VxUrl& rhs );
    bool                        operator == ( const VxUrl& rhs ) const;

	// return true if has :// in path
	static bool					isURL( std::string& strFile );

	void						setUrl( const char * pUrl );
	void						setUrl( std::string& url )				{ setUrl( url.c_str() ); }
	std::string&				getUrl( void )							{ return m_Url; }

	void						setFileName( const std::string& fileName );
	std::string&				getFileName( void )						{ return m_FileName; }
	const char *				getProtocol( void );

	const char *				getUser( void );
	const char *				getPassword( void );

	const char *				getHost( void );
    std::string                 getHostString( void );
	uint16_t					getPort( void );
	void						setPath( const char * pFilePathAndName );
	const char *				getPath( void );
	const char *				getQuery( void );
	const char *				getFragment( void );
    std::string				    getOnlineId( void ) { return m_strOnlineId.empty() ? "" : m_strOnlineId.c_str(); }
    bool                        hasValidOnlineId( void );
    bool                        validateUrl( bool onlineIdMustBeValid );

	bool						isAbsoluteUrl();
	const char *				getRelativeURL( std::string& strRetRelativeUrl, bool bWithParam = false );
	const char *				getAbsoluteURL( std::string& strRetAbsoluteUrl );
	const char *				getHostURL( const char * host, int port, const char * uri, std::string& buf );

protected:
	std::string					m_Url;
	std::string					m_FileName;
	std::string					m_FileExtension;
	std::string					m_ShareName;
	std::string					m_strProtocol;
	std::string					m_strHost;
	uint16_t					m_Port;
	std::string					m_strPath;
	std::string					m_strUser;
	std::string					m_strPassword;

	std::string					m_strQuery;
	std::string					m_strFragment;
    std::string					m_strOnlineId;
};


