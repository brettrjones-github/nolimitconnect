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

#include "RcWebServer.h"
#include <ptop_src/ptop_engine_src/Plugins/PluginBaseWebServer.h>
#include <NetLib/VxSktUtil.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxLinuxOnly.h>

#include <stdio.h>
#include <string.h>

#ifdef TARGET_OS_WINDOWS
	#define stricmp _stricmp
#endif // TARGET_OS_WINDOWS

//============================================================================
RcWebSkt::RcWebSkt()
: m_WebMgr(0)
, m_Ident(0)
, m_File(0)
, m_u64FileLen(0)
, m_u64LenSent(0)
, m_WebServerPlugin(0)
{
	setIsWebSkt( true );
}

//============================================================================
RcWebSkt::~RcWebSkt()
{
    if( IsLogEnabled( eLogSkt ) )
	    LogMsg( LOG_DEBUG,  "RcWebSkt::~RcWebSkt %d 0x%x %s skt handle %d destructing\n", this->m_iSktId, this, this->m_strRmtIp.c_str(), m_Socket );
	m_bIsConnected = false;
	m_bClosingFromDestructor = true;
	m_SktRxThread.abortThreadRun( true );
	m_SktTxThread.abortThreadRun( true );
	closeSkt( eSktCloseHttpSktDestroy );
	m_SktTxSemaphore.signal();
	m_SktRxThread.killThread();
	m_SktTxThread.killThread();
}

//============================================================================
void RcWebSkt::setWebServerPlugin( PluginBaseWebServer * poPlugin )
{
	m_WebServerPlugin = poPlugin;
}

//============================================================================
PluginBaseWebServer *	RcWebSkt::getWebServerPlugin( void )
{
	return m_WebServerPlugin;
}

//============================================================================
//! que up a file to be sent
RCODE RcWebSkt::queWebsiteFile( const char *	pFileName,		// file name or password
								uint64_t				fileLen,
								EWbMyFiles		eFileType,		// file type if alias for shared file
								EHttpReqType	eHttpReqType,	// http request type
								const char *	pPwd )			// password
{
	WbQueEntry oEntry;
	oEntry.m_eFileType		= eFileType;
	oEntry.m_eHttpReqType	= eHttpReqType;
	oEntry.m_strFileName	= pFileName;
	oEntry.m_FileLen		= fileLen;
	oEntry.m_strPwd			= pPwd;

	m_Mutex.lock( __FILE__, __LINE__ );
	m_aoQuedFiles.push_back( oEntry );
	m_Mutex.unlock( __FILE__, __LINE__ );
	m_SktTxSemaphore.signal();
	return 0;
}

//============================================================================
//! transmit file to browser
RCODE RcWebSkt::doSendFile( const char * pFileName )
{
	RCODE rc = 0;
	int iFileChunkSize = 1024;
	char as8SendBuf[ 1024 ];
	m_u64FileLen = VxFileUtil::getFileLen( pFileName );
	m_u64LenSent = 0;
	if( 0 == m_u64FileLen )
	{
		rc =  VxGetLastError();
		LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: error %d getting file len %s\n", rc, pFileName );
		m_WebServerPlugin->sendHttpErr( true, this->getIdentity(), this, HTTP_STATUS_NOTFOUND );
		return rc;
	}

	m_File = fopen( pFileName, "rb" );
	if( NULL == m_File )
	{
		rc =  VxGetLastError();
		LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: error %d opening file %s\n", rc, pFileName );
		m_WebServerPlugin->sendHttpErr( true, this->getIdentity(), this, HTTP_STATUS_NOTFOUND );
		return rc;
	}

	// send http header 
	bool			bBinary			= true;
	std::string		strExt;
	std::string		strFullPath		= pFileName;
	std::string		strJustPath;
	std::string		strJustFileName;
	VxFileUtil::seperatePathAndFile(	strFullPath,		// path and file name			
										strJustPath,		// return path to file
										strJustFileName );	// return file name
	VxFileUtil::getFileExtension( strJustFileName, strExt );
	if( (0 == stricmp( strExt.c_str(), "jpg" ) )	||
		(0 == stricmp( strExt.c_str(), "jpeg" ) )	||
		(0 == stricmp( strExt.c_str(), "png" ) )	||
		(0 == stricmp( strExt.c_str(), "gif" ) )	||
		(0 == stricmp( strExt.c_str(), "bmp" ) )	||
		(0 == stricmp( strExt.c_str(), "htm" ) )	||
		(0 == stricmp( strExt.c_str(), "html" ) )	)
	{
		// browser knows how to handle these
		bBinary = false;
        sprintf( as8SendBuf, "HTTP/1.1 200 OK.\r\nContent-Length: " PRId64 "\r\nContent-Type:\r\nConnection: Keep-Alive\r\n\r\n", m_u64FileLen );
	}
	else
	{
		bBinary = true;
		sprintf( as8SendBuf, "HTTP/1.1 200 OK.\r\nContent-Length: %lld\r\nContent-disposition: attachment; filename=%s\r\nConnection: Keep-Alive\r\n\r\n",
						m_u64FileLen,
						strJustFileName.c_str() );
	}

	rc = this->sendData( as8SendBuf,  (int)strlen( as8SendBuf ) );
	if( rc )
	{
		LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: skt error %d %s sending header for file %s\n", rc,VxDescribeSktError( rc ), pFileName );
		fclose( m_File );
		return rc;
	}

	while( m_u64LenSent < m_u64FileLen )
	{
        unsigned int iLenToSend = iFileChunkSize;
		uint64_t u64LenLeft = m_u64FileLen - m_u64LenSent;
		if( u64LenLeft < iLenToSend )
		{
            iLenToSend = (unsigned int)u64LenLeft;
		}

		if( iLenToSend != fread( as8SendBuf, 1, iLenToSend, m_File ) )
		{
			rc =  VxGetLastError();
			LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: error %d reading file %s\n", rc, pFileName );
			fclose( m_File );
			return rc;
		}

		rc = this->sendData( as8SendBuf,  iLenToSend );
		if( rc )
		{
			LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: skt error %d %s sending file %s\n", rc,VxDescribeSktError( rc ), pFileName );
			fclose( m_File );
			return rc;
		}

		m_u64LenSent += iLenToSend;
	}

	LogMsg( LOG_INFO, "RcWebSkt::DoSendFile: success file %s\n", pFileName );
	return 0;
}

#define WEB_PWD_PROMPT	"<html><head><title>Web Files Prompt</title></head><body bgcolor=\"#B1CBEB\">\
<FORM ACTION=\"%s\" METHOD=POST>\
password: <INPUT TYPE=PASSWORD NAME=\"tc_pwd\">\
<P><INPUT TYPE=SUBMIT VALUE=\"submit\"></FORM></body></html>"

//============================================================================
//! thread function to transmit data socket
void * RcSktWebTransmitThreadFunc(  void * pvContext )
{
	VxThread * poThread = (VxThread *)pvContext;
	poThread->setIsThreadRunning( true );
    RcWebSkt * sktBase		= (RcWebSkt *)poThread->getThreadUserParam();
    if( sktBase && false == poThread->isAborted() )
    {
        if( IsLogEnabled( eLogSkt ) )
            LogMsg( LOG_DEBUG,  "skt %d %s RcSktWebTransmitThreadFunc start\n", sktBase->m_iSktId, sktBase->m_strRmtIp.c_str() );

        do
        {
            sktBase->m_SktTxSemaphore.wait();
            while( sktBase->m_aoQuedFiles.size() )
            {
                if( poThread->isAborted() ||
                    ( false == sktBase->isConnected() ) )
                {
                    break;
                }

                std::string		strFileName;
                std::string		strPwd;
                EWbMyFiles		eFileType;
                EHttpReqType	eHttpReqType;

                sktBase->lockSkt();
                //if( sktBase->m_aoQuedFiles.size() )
                //{
                    WbQueEntry * poEntry = &sktBase->m_aoQuedFiles[0];
                    strFileName		= poEntry->m_strFileName.c_str();
                    eFileType		= poEntry->m_eFileType;
                    eHttpReqType	= poEntry->m_eHttpReqType;
                    strPwd			= poEntry->m_strPwd;
                    sktBase->m_aoQuedFiles.erase( sktBase->m_aoQuedFiles.begin() );
                //}

                sktBase->unlockSkt();
    #ifdef USE_WEB_FILESHARE
                if( eFileType )
                {
                    if( eHttpReqGet == eHttpReqType )
                    {
                        // check to see if needs password prompt
                        if( false == poWebMgr->IsPasswordOk( sktBase->m_RmtIp, eFileType ) )
                        {
                            // need to prompt for file share password
                            char as8Buf[ 1024 ];

                            // truncate root of website from file name.. we dont want to send where the real file is
                            const char * pTemp = strFileName.c_str();
                            if( 0 == strncmp( pTemp, poWebMgr->GetWebPageDir(), strlen( poWebMgr->GetWebPageDir() ) ) )
                            {
                                strFileName = &pTemp[ strlen( poWebMgr->GetWebPageDir()) - 1 ];
                            }
                            else if( 0 == strncmp( pTemp, poWebMgr->GetMyFilesDir(eFileType), strlen( poWebMgr->GetMyFilesDir(eFileType) ) ) )
                            {
                                std::string strPostFile = "/";
                                strPostFile = strPostFile + poWebMgr->GetMyFilesAlias(eFileType);
                                strPostFile = strPostFile + &pTemp[ strlen( poWebMgr->GetMyFilesDir(eFileType) ) ];
                                strFileName = strPostFile;

                            }
                            else
                            {
                                vx_assert( false );
                            }

                            sprintf( as8Buf, WEB_PWD_PROMPT, strFileName.c_str(), strFileName.c_str() );

                            char as8SendBuf[ 1024 ];
                            // send http header
                            sprintf( as8SendBuf, "HTTP/1.1 200 OK.\r\nContent-Length: %ld\r\nConnection: Keep-Alive\r\n\r\n", strlen( as8Buf ) );
                            rc = sktBase->sendData( as8SendBuf,  strlen( as8SendBuf ) );
                            if( rc )
                            {
                                LogMsg( LOG_INFO, "RcWebSkt::PwdPrompt: skt error %d %s sending header for pwd prompt\n", rc,VxDescribeSktError( rc ) );
                                continue;
                            }
                            rc = sktBase->sendData( as8Buf, strlen( as8Buf ) );
                            if( rc )
                            {
                                LogMsg( LOG_INFO, "RcWebSkt::PwdPrompt: skt error %d %s sending content for pwd prompt\n", rc,VxDescribeSktError( rc ) );
                                continue;
                            }
                            continue;
                        }
                    }
                    else if( eHttpReqPost == eHttpReqType )
                    {
                        // user posted password
                        if( false == poWebMgr->VerifyPwd( sktBase->m_RmtIp, eFileType, strPwd.c_str() ) )
                        {
                            SendHttpErr( sktBase, HTTP_STATUS_UNAUTHORIZED );
                            continue;
                        }
                    }
                }
    #else
                // don't allow posting if not serving Personal Shared files
                if( eHttpReqPost == eHttpReqType )
                {

                    sktBase->getWebServerPlugin()->sendHttpErr( false, sktBase->getIdentity(), sktBase, HTTP_STATUS_UNAUTHORIZED );
                    continue;
                }
    #endif // USE_WEB_FILESHARE

                if( strFileName.size() )
                {
                    sktBase->doSendFile( strFileName.c_str() );
                }
            }
            if( poThread->isAborted() ||
                ( false == sktBase->isConnected() ) )
            {
                break;
            }
        }while( (false == poThread->isAborted() ) &&
                sktBase->isConnected() );

        if( IsLogEnabled( eLogSkt ) )
            LogMsg( LOG_DEBUG,  "skt %d 0x%x %s RcSktWebTransmitThreadFunc exit\n", sktBase->m_iSktId, sktBase, sktBase->m_strRmtIp.c_str() );
    }

	//! Thread calls this just before exit
	poThread->threadAboutToExit();
    return nullptr;
}
