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
// http://www.nolimitconnect.com
//============================================================================
#include <NetLib/VxSktBase.h> 

class RcWebServer;
class PluginBaseWebServer;
class VxNetIdent;

// type of request being made
enum EHttpReqType 
{
	eHttpReqUnknown = 0,
	eHttpReqGet		= 1,
	eHttpReqPost	= 2,
};

enum EWbMyFiles
{
	eWbMyFilesNone			= 0,
	eWbMyFilesImages		= 1,
	eWbMyFilesMusic			= 2,
	eWbMyFilesVideos		= 3,
	eWbMyFilesOther			= 4,
	eMaxNormalWbMyFiles		= 5,

	eWbMyFilesError			= 6,
	eWbMyFilesPersonal		= 7,
	eWbMyFilesArchive		= 8,
	eWbMyFilesDocuments		= 9,
	eWbMyFilesIsos			= 10,
	eWbMyFilesRes1			= 11,
	eWbMyFilesRes2			= 12,

	eMaxAllWbMyFiles		// must be last
};

class WbQueEntry
{
public:
	WbQueEntry()
	: m_FileLen( 0 )
	, m_eFileType( eWbMyFilesNone )
	, m_eHttpReqType( eHttpReqUnknown )
	{
	}

	WbQueEntry(const WbQueEntry& rhs)
	{
		*this = rhs;
	}

	WbQueEntry& operator=( const WbQueEntry& rhs )
	{
		if( this != &rhs )
		{
			m_strFileName		= rhs.m_strFileName;
			m_FileLen			= rhs.m_FileLen;
			m_strPwd			= rhs.m_strPwd;
			m_eFileType			= rhs.m_eFileType;
			m_eHttpReqType		= rhs.m_eHttpReqType;
		}

		return *this;
	}


	//=== vars ===//
	std::string			m_strFileName;
	uint64_t					m_FileLen;
	std::string			m_strPwd;
	EWbMyFiles			m_eFileType;
	EHttpReqType		m_eHttpReqType;

};

class RcWebSkt : public VxSktBase
{
public:
	RcWebSkt();
	virtual ~RcWebSkt();

	void						setWebServerPlugin( PluginBaseWebServer * poPlugin );
    PluginBaseWebServer *		getWebServerPlugin( void );

	void						setIdentity( VxNetIdent * netIdent )		{ m_Ident = netIdent; }
	VxNetIdent *				getIdentity( void )							{ return m_Ident; }

	RCODE						queWebsiteFile( const char *	pFileName,		// file name or password
												uint64_t				fileLen,
												EWbMyFiles		eFileType,		// file type if alias for shared file
												EHttpReqType	eHttpReqType,	// http request type
												const char *	pPwd );			// password
	RCODE						doSendFile( const char * pFileName );

	//=== vars ===//
	RcWebServer *				m_WebMgr;
	VxNetIdent *				m_Ident;

	FILE *						m_File;
    uint64_t					m_u64FileLen;			// total file len
    uint64_t					m_u64LenSent;			// how much of file has been sent
	std::string					m_strWebFileName;
	std::string					m_strRealFileName;

	std::vector<WbQueEntry>		m_aoQuedFiles;	// files and prompts qued to be sent

private:
    PluginBaseWebServer *		m_WebServerPlugin;
};

void * RcSktWebReceiveThreadFunc(  void * pvContext );
void * RcSktWebTransmitThreadFunc(  void * pvContext );
