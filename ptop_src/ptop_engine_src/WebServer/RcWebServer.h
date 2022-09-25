#pragma once
//============================================================================
// Copyright (C) 2009 Brett R. Jones
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

#include "RcWebPageSettings.h"

#include <ptop_src/ptop_engine_src/WebServer/RcWebSkt.h>

#include <NetLib/VxSktBaseMgr.h>
#include <CoreLib/VxFileLists.h>

class PluginBaseWebServer;

//HTTP ERRORS
//
// These numbers relate to their
// sequence in an array in
//
#define HTTP_STATUS_OK				0
#define HTTP_STATUS_CREATED			1
#define HTTP_STATUS_ACCEPTED		2
#define HTTP_STATUS_NOCONTENT		3
#define HTTP_STATUS_MOVEDPERM		4
#define HTTP_STATUS_MOVEDTEMP		5
#define HTTP_STATUS_NOTMODIFIED		6
#define HTTP_STATUS_BADREQUEST		7
#define HTTP_STATUS_UNAUTHORIZED	8
#define HTTP_STATUS_FORBIDDEN		9
#define HTTP_STATUS_NOTFOUND		10
#define HTTP_STATUS_SERVERERROR		11
#define HTTP_STATUS_NOTIMPLEMENTED	12
#define HTTP_STATUS_BADGATEWAY		13
#define HTTP_STATUS_UNAVAILABLE		14

////============================================================================
////! send standard http error to browser
//RCODE SendHttpErr( RcWebSkt * sktBase, int iErrIdx );
////============================================================================

class VxWbSharedFiles
{
public:
	VxWbSharedFiles(){};

	//=== vars ===//
	std::vector<VxFileInfo>	m_aoFiles;
};

class VxWbAllFiles
{
public:
	VxWbAllFiles(){};

	//=== vars ===//
	VxWbSharedFiles			m_aoSharedFiles[ eMaxAllWbMyFiles ];

};

class VxPwdEntry
{
public:
	VxPwdEntry()
		: m_iPwdAttempts(0)
	{
	}

	//=== vars ===//
	int							m_iPwdAttempts;			// failed pwd attempts count
	std::string					m_strPwd;				// if pwd success.. this is the pwd
};

class VxSharedFilesAuth
{
public:
    VxSharedFilesAuth() = default;

	//=== vars ===//
	uint32_t					m_u32Ip = 0;							// ip address of remote user				
	int64_t						m_LastAttemptTimeMs = 0;				// time last attempt was made ( successfull or not )
	VxPwdEntry					m_aoPwds[ eMaxNormalWbMyFiles ];	// password attempts etc
};

class RcWebServer : public VxSktBaseMgr, public VxWbAllFiles, public RcWebPageSettings
{
public:
	RcWebServer();
	virtual ~RcWebServer();

	//=== helpers ===//
	void						setPluginBaseWebServer( PluginBaseWebServer * poPlugin );

	void						setWebSiteDir( const char * pWebSiteDir );
	std::string&				getWebSiteDir(  void );

	//! set enable
	void						setWebServerEnable( bool bEnable );

	//=== overrides ===//
	//! handle callbacks from sockets
	void						doReceiveCallback( VxSktBase* sktBase );

	//=== methods ===//
	// shutdown web server
	void						webServerShutdown( void );

	//! return true if web server is available
	bool						isWebServerAvailable( void );

	//! accept connection from skt manager
	RCODE						acceptRawLocalHttpConnection( VxSktBase* sktBase, VxNetIdent* netIdent, RcWebSkt ** ppoRetWebSkt );

	//! start skt threads for accepted web skt
	void						startWebSktThreads( RcWebSkt * poWebSkt );

	//! handle incoming data from socket
	RCODE						HandleWebData( RcWebSkt * sktBase );

	//! return true if more to parse
	bool						parseWebRxData( char *			pDataBuf, 
												int				iDataLen, 
												int&			iRetDataUsed, 
												int&			iHttpError,
												std::string&	strRetFileName,
												uint64_t&			retFileLen,
												int&			iFileOffset,
												bool&			bCloseConnection,
												EWbMyFiles *	eFileType		= NULL, 
												EHttpReqType *	eHttpReqType	= NULL, 
												std::string *	strRetPwd		= NULL );

	//! get real file name from website alias ( if has alias )
	RCODE						GetWebSiteRealFileName(	std::string&	strWebFileName,	// web file name
														uint64_t&			retFileLen,
														EWbMyFiles&		peRetFileType );	// My shared file type
#ifdef USE_WEB_FILESHARE
	//! get build website progress
	virtual int					GetBuildProgress( void ) = 0;

	//! return true if password has been set or no password required
	bool						IsPasswordOk( uint32_t u32IpAddr, EWbMyFiles eFileType );

	//! verify the password is correct for the given file share type
	bool						VerifyPwd( uint32_t u32IpAddr, EWbMyFiles eFileType, const char * pPwd );

	//! called once per second for authorization timeouts
	void						WebServerOncePerSecond( void );
#endif // USE_WEB_FILESHARE

	RCODE						sentWebServerData(	RcWebSkt *		sktBase,					// web socket
													const char *	pData,					// data to send
													int				iDataLen,				// length of data	
													bool			bDisconnectAfterSend,	// if true disconnect after data is sent
													int				iSendTimeoutSeconds );	// timeout attempt to send

	bool						isExecuteableFile( std::string& strWebFileName );

	//=== vars ===//
	PluginBaseWebServer *			m_WebPlugin;
	bool						m_bWebServerEnabled;
	bool						m_bShuttingDown;
#ifdef USE_WEB_FILESHARE
	std::vector<VxSharedFilesAuth>	m_aoAuthorizations;
#endif // USE_WEB_FILESHARE

};
