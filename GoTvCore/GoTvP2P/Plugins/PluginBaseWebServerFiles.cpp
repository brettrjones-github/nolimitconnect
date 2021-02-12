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
// http://www.nolimitconnect.com
//============================================================================

#include "PluginBaseWebServer.h"

#include <PktLib/PktsWebServer.h>
#include <PktLib/PktsScan.h>
#include <NetLib/VxSktBase.h>
#include <NetLib/VxSktUtil.h>
#include <CoreLib/VxTimeUtil.h>

#include <sys/stat.h>
#include <ctype.h> // for toupper
#include <time.h>
#include <string.h>
#include <stdio.h>

namespace
{
	#define GETDWORD(ptrData) (*(uint32_t*)(ptrData))
	#define SETDWORD(ptrData,data) (*(uint32_t*)(ptrData)=data)
	#define GETWORD(ptrData) (*(uint16_t*)(ptrData))
	#define DEFDWORD(char1,char2,char3,char4) (char1+(char2<<8)+(char3<<16)+(char4<<24))

	#define MAX_REQUESTS_PER_CONNECTION 1
	#define HTTP_KEEPALIVE_TIME 15

	#define HTTP_FILE_TYPE_HTML "text/html"
	#define HTTP_FILE_TYPE_XML "text/xml"
	#define HTTP_FILE_TYPE_TEXT "text/plain"
	#define HTTP_FILE_TYPE_GIF "image/gif"
	#define HTTP_FILE_TYPE_JPEG "image/jpeg"
	#define HTTP_FILE_TYPE_PNG "image/png"
	#define HTTP_FILE_TYPE_JS "application/x-javascript"
	#define HTTP_FILE_TYPE_CSS "text/css"
	#define HTTP_FILE_TYPE_SWF "application/x-shockwave-flash"
	#define HTTP_FILE_TYPE_MPA "audio/mpeg"
	#define HTTP_FILE_TYPE_MPEG "video/mpeg"
	#define HTTP_FILE_TYPE_AVI "video/x-msvideo"
	#define HTTP_FILE_TYPE_QUICKTIME "video/quicktime"
	#define HTTP_FILE_TYPE_OCTET "application/octet-stream"
	#define HTTP_FILE_TYPE_STREAM "application/x-datastream"
	#define HTTP_CONTENT_LENGTH "Content-Length:"

	#define FILEEXT_HTM DEFDWORD('H','T','M',0)
	#define FILEEXT_HTML DEFDWORD('H','T','M','L')
	#define FILEEXT_CSS DEFDWORD('C','S','S',0)

	#define FILEEXT_XML DEFDWORD('X','M','L',0)
	#define FILEEXT_TEXT DEFDWORD('T','X','T',0)

	#define FILEEXT_GIF DEFDWORD('G','I','F',0)
	#define FILEEXT_JPG DEFDWORD('J','P','G',0)
	#define FILEEXT_JPEG DEFDWORD('J','P','E','G')
	#define FILEEXT_PNG DEFDWORD('P','N','G',0)

	#define FILEEXT_MPG DEFDWORD('M','P','G',0)
	#define FILEEXT_MPEG DEFDWORD('M','P','E','G')
	#define FILEEXT_MPA DEFDWORD('M','P',('3' - 32),0)
	#define FILEEXT_AVI DEFDWORD('A','V','I',0)
	#define FILEEXT_MP4 DEFDWORD('M','P',('4' - 32),0)
	#define FILEEXT_MOV DEFDWORD('M','O','V',0)

	#define FILEEXT_JS DEFDWORD('J','S',0,0)
	#define FILEEXT_SWF DEFDWORD('S','W','F',0)

//#ifdef APP_MYP2PWEB
	#define HTTP200_HEADER "HTTP/1.1 %s\r\nServer: NoLimitConnectWeb\r\nCache-control: no-cache\r\nPragma: no-cache\r\nAccept-Ranges: bytes\r\nKeep-Alive: timeout=%d, max=%d\r\nConnection: %s\r\nLast-Modified: "
//#else
//	#define HTTP200_HEADER "HTTP/1.1 %s\r\nServer: TestApp\r\nCache-control: no-cache\r\nPragma: no-cache\r\nAccept-Ranges: bytes\r\nKeep-Alive: timeout=%d, max=%d\r\nConnection: %s\r\nLast-Modified: "
//#endif // APP_MYP2PWEB

	const int					HTTP_FILE_CHUNK_SIZE = 1514;
}

//============================================================================
const char * RcWebServerGetContentType(const char *pFileExtensionIn)
{
	// make extension upper case
	char pFileExtension[6];
	strcpy( pFileExtension, &pFileExtensionIn[1] );
	int iExtLen = (int)strlen(pFileExtension);
	for( int i = 0; i < iExtLen; i++ )
	{
		pFileExtension[i] = toupper(pFileExtension[i]);
	}

	if( 1 >= iExtLen )
	{
		// extension has only one char
		return HTTP_FILE_TYPE_OCTET;
	}
	else if( 2 == iExtLen )
	{
		// extension has two chars
		switch( GETDWORD(pFileExtension) ) 
		{
		case FILEEXT_JS: return HTTP_FILE_TYPE_JS;
		}
	} 
	else if( 3 == iExtLen ) 
	{
		//identify 3-char file extensions
		switch( GETDWORD(pFileExtension) ) 
		{
		case FILEEXT_HTM:	return HTTP_FILE_TYPE_HTML;
		case FILEEXT_XML:	return HTTP_FILE_TYPE_XML;
		case FILEEXT_TEXT:	return HTTP_FILE_TYPE_TEXT;
		case FILEEXT_CSS:	return HTTP_FILE_TYPE_CSS;
		case FILEEXT_PNG:	return HTTP_FILE_TYPE_PNG;
		case FILEEXT_JPG:	return HTTP_FILE_TYPE_JPEG;
		case FILEEXT_GIF:	return HTTP_FILE_TYPE_GIF;
		case FILEEXT_SWF:	return HTTP_FILE_TYPE_SWF;
		case FILEEXT_MPA:	return HTTP_FILE_TYPE_MPA;
		case FILEEXT_MPG:	return HTTP_FILE_TYPE_MPEG;
		case FILEEXT_AVI:	return HTTP_FILE_TYPE_AVI;
		case FILEEXT_MP4:	return HTTP_FILE_TYPE_STREAM;
		case FILEEXT_MOV:	return HTTP_FILE_TYPE_STREAM;
		}
	}
	if( 4 == iExtLen ) 
	{
		switch( GETDWORD(pFileExtension) ) 
		{
		case FILEEXT_HTML:	return HTTP_FILE_TYPE_HTML;
		case FILEEXT_MPEG:	return HTTP_FILE_TYPE_MPEG;
		case FILEEXT_JPEG:	return HTTP_FILE_TYPE_JPEG;
		}
	}

	LogMsg( LOG_ERROR, "RcWebServerGetContentType UNKNOWN TYPE\n" );
	return HTTP_FILE_TYPE_OCTET;
}

//============================================================================
static bool BuildFileSendHeader(	const char * pFileName, 
									uint64_t& u64FileLen, 
									int iFileOffset,
									char * pSendBuf,
									int& iRetHeaderTextLength,
									bool bCloseAfterSend )
{
	if( iFileOffset < 0 )
	{
		LogMsg( LOG_INFO, "BuildFileSendHeader: invalid offset\n", iFileOffset );
		return false;
	}

	#ifdef TARGET_OS_WINDOWS
		struct __stat64 gStat;
		int result = _stat64( pFileName, &gStat );
	#else
		struct stat64 gStat;
		// Get data associated with the file
		int result = stat64( pFileName, &gStat );
	#endif //TARGET_OS_WINDOWS

	if( result < 0 )
	{
		// file/dir not found
		LogMsg( LOG_INFO, "BuildFileSendHeader: file %s not found\n", pFileName );
		return false;
	}

	u64FileLen = gStat.st_size;

	u64FileLen -= iFileOffset;

	const char * pExtension = strrchr( pFileName, '.' );
	if( NULL == pExtension )
	{
		pExtension = "";
	}

	char * pTemp = pSendBuf;
	pTemp += sprintf(	pTemp, 
						HTTP200_HEADER,
						(iFileOffset) ? "206 Partial content" : "200 OK",
						HTTP_KEEPALIVE_TIME,
						MAX_REQUESTS_PER_CONNECTION,
						bCloseAfterSend ? "close" : "Keep-Alive" );
	pTemp += VxTimeUtil::getHttpDateTime( gStat.st_mtime, pTemp );
	SETDWORD( pTemp, DEFDWORD('\r','\n',0,0 ) );
	pTemp += 2;
	pTemp += sprintf(	pTemp,
						"Content-Type: %s\r\n",
						RcWebServerGetContentType(pExtension));

	pTemp += sprintf( pTemp, "Content-Length: %lld\r\n\r\n", u64FileLen );
	iRetHeaderTextLength = (int)(pTemp - pSendBuf);
	return true;
}

//============================================================================
RCODE PluginBaseWebServer::doSendFile(	VxNetIdent *	netIdent,
									VxSktBase *		sktBase,	
									const char *	pFileName,
									uint64_t				fileLen,
									int				iFileOffset,
									bool			bCloseAfterSend )
{
	RCODE rc = 0;
	char as8SendBuf[ HTTP_FILE_CHUNK_SIZE + 32 ];
	int iHeaderTextLength = 0;

	if( false == BuildFileSendHeader( pFileName, fileLen, iFileOffset, as8SendBuf, iHeaderTextLength, bCloseAfterSend ) )
	{
		m_WebPlugin->sendHttpErr( false, netIdent, sktBase, HTTP_STATUS_NOTFOUND );
		return -1;
	}

	vx_assert( HTTP_FILE_CHUNK_SIZE > iHeaderTextLength );
	m_PluginMutex.lock();

	m_File = fopen( pFileName, "rb" );
	if( NULL == m_File )
	{
		m_PluginMutex.unlock();

		rc =  VxGetLastError();
		LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: error %d opening file %s\n", rc, pFileName );
		m_WebPlugin->sendHttpErr( false, netIdent, sktBase, HTTP_STATUS_NOTFOUND );
		return rc;
	}

	rc = this->sendWebServerData( false, netIdent, sktBase, as8SendBuf, iHeaderTextLength, false );
	if( rc )
	{
		LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: skt error %d %s sending header for file %s\n", rc,VxDescribeSktError( rc ), pFileName );
		fclose( m_File );
		m_File = NULL;
		m_PluginMutex.unlock();
		return rc;
	}

	uint64_t u64LenSent = 0;
	uint64_t u64LenLeft = fileLen;
    unsigned int lenToSend;
	while( u64LenSent < fileLen )
	{
        lenToSend = HTTP_FILE_CHUNK_SIZE;
		u64LenLeft = fileLen - u64LenSent;
        if( u64LenLeft < lenToSend )
		{
            lenToSend = (unsigned int)u64LenLeft;
		}

        int iLenRead = (int)fread( as8SendBuf, 1, lenToSend, m_File );
        if( iLenRead != lenToSend )
		{
			rc =  VxGetLastError();
			LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: error %d reading file %s\n", rc, pFileName );
			fclose( m_File );
			m_File = NULL;
			m_PluginMutex.unlock();
			return rc;
		}

        rc = this->sendWebServerData( false, netIdent, sktBase, as8SendBuf, lenToSend, false );
		if( rc )
		{
			LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: skt error %d %s sending file %s\n", rc,VxDescribeSktError( rc ), pFileName );
			fclose( m_File );
			m_File = NULL;
			m_PluginMutex.unlock();
			return rc;
		}

        u64LenSent += lenToSend;
	}

	fclose( m_File );
	m_File = NULL;
	m_PluginMutex.unlock();
	LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: success file %s\n", pFileName );
	return 0;
}

