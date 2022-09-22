//============================================================================
// Copyright (C) 2003 Brett R. Jones 
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

#include "RcWebServer.h"

#include <ptop_src/ptop_engine_src/Plugins/PluginBaseWebServer.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxFileIsTypeFunctions.h>
#include <CoreLib/VxLinuxOnly.h>

#include <string.h>
#include <memory.h>
#include <stdlib.h>
#ifdef TARGET_OS_WINDOWS
	#define stricmp _stricmp
	#define strnicmp _strnicmp
#endif // TARGET_OS_WINDOWS

#define GETDWORD(ptrData) (*(uint32_t*)(ptrData))
#define SETWORD(ptrData,data) (*(uint16_t*)(ptrData)=data)
#define DEFDWORD(char1,char2,char3,char4) (char1+(char2<<8)+(char3<<16)+(char4<<24))
#define HTTP_HEADEREND DEFDWORD('\r','\n','\r','\n')

//============================================================================
int ParseToken(char *pchToken, char chDelimiter, char *pchBuffer, int iMaxTokenSize)
{
	char *p=pchToken;
	int iCharCopied=0;
	while (*p && *p!=chDelimiter && iCharCopied<iMaxTokenSize) 
	{
		*(pchBuffer++)=*(p++);
		iCharCopied++;
	}

	*pchBuffer = '\0'; // null terminate
	return (*p==chDelimiter) ? iCharCopied : 0;
}

//============================================================================
//! get real file name from website alias ( if has alias )
RCODE RcWebServer::GetWebSiteRealFileName(	std::string&	strWebFileName, 
											uint64_t&			retFileLen,
											EWbMyFiles&		peRetMyFileType )
{
	peRetMyFileType = eWbMyFilesNone;
#ifdef USE_WEB_FILESHARE
	char as8Buf[ VX_MAX_PATH ];
	const char * pTemp = strWebFileName.c_str();
	bool bAliasFound = false;
	// strip off leading slash
	if( '/' == pTemp[0] )
	{
		strcpy( as8Buf, &pTemp[1] );
	}
	else
	{
		strcpy( as8Buf, pTemp );
	}
	if( 0 == strncmp( as8Buf, WEB_MY_TC_PICTURE_ALIAS, strlen( WEB_MY_TC_PICTURE_ALIAS ) ) )
	{
		// its my picture.. get real file
		strWebFileName = GetMyPictureRealFile();
		bAliasFound = true;
	}
	else
	{
		for( int i = 1; i < eWbMyFilesError; i++ )
		{
			if( false == IsMyFilesEnabled( (EWbMyFiles)i ) )
			{
				// not enabled
				continue;
			}
			char * pAlias = GetMyFilesAlias( (EWbMyFiles)i );
			int iAliasLen = (int)strlen( pAlias );
			if( 0 >= iAliasLen )
			{
				LogMsg( LOG_INFO, "RcWebServer::GetRealFileName: zero alias of type %d\n", i );
				vx_assert( false );
				continue;
			}
			if( 0 == strncmp( as8Buf, pAlias, iAliasLen ) )
			{
				// found alias
				if( (int)strlen( as8Buf ) > iAliasLen )
				{
					std::string strAliasPath;
					std::string strAliasFile;
					seperatePathAndFile(	as8Buf,			// path and file name			
											strAliasPath,	// return path to file
											strAliasFile );	// return file name
					if( 0 == strcmp( strAliasFile.c_str(), "tc_index.htm" ) )
					{
						// is a index of shared files in website directory
						strWebFileName = GetWebSiteDir() + as8Buf;
					}
					else
					{
						// is a shared file
						std::string strAliasDir = GetMyFilesDir( (EWbMyFiles)i );
						strWebFileName = strAliasDir + &as8Buf[ iAliasLen ];
					}
					bAliasFound = true;
					* peRetMyFileType = (EWbMyFiles)i;
					break;
				}
			}
		}
	}

	if( false == bAliasFound )
	{
		// no alias so use root of website directory
		strWebFileName = GetWebSiteDir() + as8Buf;
	}

	if( fileExists( strWebFileName.c_str() ) )
	{
		return 0;
	}

	return -1;
#else // no fileshare
	// no alias so use root of website directory
	strWebFileName	= getWebSiteDir() + strWebFileName;
	retFileLen		= VxFileUtil::fileExists( strWebFileName.c_str() );

	if( ( false == VxIsExecutableFile( strWebFileName ) )
		&& ( false == VxIsShortcutFile( strWebFileName ) )  
		&& ( 0 != retFileLen ) )
	{
		return 0;
	}

	return -1;
#endif// USE_WEB_FILESHARE
}

//============================================================================
//! handle incoming data from socket
RCODE RcWebServer::HandleWebData( RcWebSkt * sktBase )
{
	if( false == isWebServerAvailable() )
	{
		sktBase->closeSkt(eSktCloseHttpNoWebSrv);
		return -1;
	}

	if( eSktCallbackReasonData == sktBase->getCallbackReason() )
	{
		return m_WebPlugin->handleDataFromLocalBrowser( sktBase );
	}

	return 0;
}

//============================================================================
bool RcWebServer::parseWebRxData(	char *			pDataBuf, 
									int				iDataLen, 
									int&			iRetDataUsed, 
									int&			iHttpError,
									std::string&	strRetFileName,
									uint64_t&		retFileLen,
									int&			iFileOffset,
									bool&			bCloseConnection,
									EWbMyFiles *	eRetFileType, 
									EHttpReqType *	eRetHttpReqType, 
									std::string *	strRetPwd )
{
	iRetDataUsed				= 0;
	std::string	 strPwd;
	EHttpReqType eHttpReqType	= eHttpReqUnknown;
	bCloseConnection			= false;
	retFileLen					= 0;
	iFileOffset					= 0;

	if( 0 == strstr( pDataBuf, "\r\n\r\n" ) )
	{
		// no terminator of command
		return 0;
	}
	char			szSeps[]		= " \n";
	char *			cpToken			= 0;
	int				iTokenLen		= 0;
	std::string		csFileName;
	RCODE			rc				= 0;
	char			as8Buf[512];

	char * pCurParse = pDataBuf;
	char * pNextParse;
	if( NULL != ( pNextParse = strstr( pCurParse, "\r\n\r\n" ) ) )
	{
		pNextParse += 4;

		int iParseableLen = (int)(pNextParse - pCurParse);

		// Don't let requests include ".." characters
		// in requests
		if (strstr( pCurParse, "..") != NULL)
		{
			// Send "bad request" error
			iHttpError = HTTP_STATUS_BADREQUEST;
			goto error_exit;
		}

		// analyze header
		char * pHeader = pCurParse;
		for(;;) 
		{
			while(*pHeader && *pHeader != '\r' ) 
				pHeader++;

			if( !*pHeader || GETDWORD(pHeader) == HTTP_HEADEREND) 
				break;
			pHeader += 2;	//skip "\r\n"
			switch (*(pHeader++)) 
			{
			case 'C':
				if( 0 == memcmp(pHeader,"onnection: ", 11 ) ) 
				{
					pHeader+=11;
					if( ! memcmp( pHeader, "close", 5 ) ) 
					{
						bCloseConnection = true;
						pHeader+=5;
					}
				} 

				break;

			case 'R':
				if( 0 == memcmp(pHeader, "eferer: ", 8 ) ) 
				{
					pHeader += 8;
					// could log referer
				} 
				else if( 0 == memcmp( pHeader, "ange: bytes=", 12 ) ) 
				{
					pHeader += 12;
					int iLen = ParseToken( pHeader, '-', as8Buf, sizeof(as8Buf) );
					if( 0 == iLen ) 
					{
						continue;
					}
					pHeader += iLen;
					iFileOffset = atoi( as8Buf );
					iLen = ParseToken( pHeader, '/', as8Buf, sizeof( as8Buf ));
					if( 0 == iLen ) 
					{
						continue;
					}
					pHeader += iLen;
				}

				break;

			case 'H':
				if( 0 == memcmp( pHeader, "ost: ", 5 ) ) 
				{
					pHeader += 5;
					// could log host
				}

				break;
			}
		}

		// Determine request method
		cpToken = strtok( pCurParse, szSeps);
		if( 0 == stricmp( cpToken, "GET" ) )
		{
			// getting file.
			eHttpReqType = eHttpReqGet;
		}
		else if( 0 == stricmp( cpToken, "POST" ) )
		{
			// posting password
			eHttpReqType = eHttpReqPost;
		}
		else
		{
			// Send "not implemented" error
			iHttpError = HTTP_STATUS_NOTIMPLEMENTED;
			goto error_exit;
		}

		// Get the online id
		char * pOnlineId = strtok( NULL, szSeps );
		if( NULL == pOnlineId )
		{
			// Send "bad request" error
			iHttpError = HTTP_STATUS_BADREQUEST;
			goto error_exit;
		}

		pOnlineId++;
		char * pEndOfId = strchr( pOnlineId, '/' );
		if( NULL == pEndOfId )
		{
			// no online id.. may be request for faviocon.ico though
			if( 0 == strnicmp( pOnlineId, "favicon.ico", 10 ) )
			{
				csFileName =  "favicon.ico";
				goto file_found_exit;
			}

			iHttpError = HTTP_STATUS_BADREQUEST;
			goto error_exit;
		}

		*pEndOfId = 0;
		if( 32 != ( pEndOfId - pOnlineId ) )
		{
			iHttpError = HTTP_STATUS_BADREQUEST;
			goto error_exit;
		}

		// parse up to 
		cpToken = pEndOfId +1;
		if( NULL == cpToken )
		{
			// Send "bad request" error
			iHttpError = HTTP_STATUS_BADREQUEST;
			goto error_exit;
		}

		iTokenLen = (int)strlen( cpToken );
        if( ( 1 < iTokenLen ) && ( (int)sizeof( as8Buf ) > iTokenLen ) )
		{
			int iFileNameIdx = 0;
			as8Buf[ iFileNameIdx ] = 0;
			int iIdx = 0;
			while( 0 != cpToken[ iIdx ] )
			{
				if( cpToken[ iIdx ] == '%' )
				{
					if( (cpToken[ iIdx + 1 ] == '5' ) &&
						(cpToken[ iIdx + 2 ] == 'C' ) )
					{
						csFileName = as8Buf;
						as8Buf[ iFileNameIdx ] = '/';
						iFileNameIdx++;
						as8Buf[ iFileNameIdx ] = 0;
						iIdx += 3;
						continue;
					}
					if( (cpToken[ iIdx + 1 ] == '2' ) &&
						(cpToken[ iIdx + 2 ] == '0' ) )
					{
						as8Buf[ iFileNameIdx ] = ' ';
						iFileNameIdx++;
						as8Buf[ iFileNameIdx ] = 0;
						iIdx += 3;
						continue;
					}
				}

				as8Buf[ iFileNameIdx ] = cpToken[ iIdx ];
				iFileNameIdx++;
				as8Buf[ iFileNameIdx ] = 0;
				iIdx++;
			}

			csFileName = as8Buf;
		}
		else
		{
			std::string csIdxFile = getWebSiteDir() + "index.htm";
			if( VxFileUtil::fileExists( csIdxFile.c_str() ) )
			{
				csFileName = "index.htm";
			}
			else
			{
				csIdxFile = getWebSiteDir() + "index.html";
				if( VxFileUtil::fileExists( csIdxFile.c_str() ) )
				{
					csFileName = "index.html";
				}
				else
				{
					csFileName = "rc_main_index.htm";
				}
			}
		}

		if( eHttpReqPost == eHttpReqType )
		{
			// we need to parse the pwd also
			bool bPwdOk = true;
			char * pTemp = strstr( pNextParse, "tc_pwd" );
			if( NULL == pTemp )
			{
				bPwdOk = false;
				LogMsg( LOG_INFO, "RcWebServer::HandleWebData: could not parse pwd\n" );
				iHttpError = HTTP_STATUS_NOTFOUND;
				goto error_exit;
			}

			if( bPwdOk )
			{
				pTemp = pTemp + strlen( "tc_pwd" );
				if( '=' != pTemp[0] )
				{
					bPwdOk = false;
				}
				else
				{
					pTemp++;
					strPwd = pTemp;
					if( ( 0 == strPwd.size() ) ||
						( strPwd.size() > 63 ) )
					{
						bPwdOk = false;
					}
				}
			}

			if( false == bPwdOk )
			{
				LogMsg( LOG_ERROR, "RcWebServer::HandleWebData: invalid pwd %s\n", strPwd.c_str() );
				iHttpError = HTTP_STATUS_UNAUTHORIZED;
				goto error_exit;
			}
		}

file_found_exit:
		EWbMyFiles eFileType;	// My shared file type
		rc = GetWebSiteRealFileName( csFileName, retFileLen, eFileType );
		if( rc )
		{
			// could not get real file name
			LogMsg( LOG_ERROR, "RcWebServer::HandleWebData: could not get real file name for %s\n", csFileName.c_str() );
			iHttpError = HTTP_STATUS_NOTFOUND;
			goto error_exit;
		}

		strRetFileName = csFileName;
		if( eRetFileType )
		{
			*eRetFileType = eFileType;
		}

		if( eRetHttpReqType )
		{
			*eRetHttpReqType = eHttpReqType;
		}

		if( strRetPwd )
		{
			*strRetPwd = strPwd;
		}

		strPwd = "";
		pCurParse = pNextParse; // done with this chunk
	}

error_exit:
	iRetDataUsed = iDataLen;
	return 0;
}
