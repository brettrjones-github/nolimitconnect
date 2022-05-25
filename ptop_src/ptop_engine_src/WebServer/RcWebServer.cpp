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

#include <CoreLib/VxGlobals.h>
#include <CoreLib/VxLinuxOnly.h>

#include <time.h>
#include <memory.h>
#ifdef TARGET_OS_WINDOWS
	#define stricmp _stricmp
#endif // TARGET_OS_WINDOWS

//============================================================================
RcWebServer::RcWebServer()
	: m_WebPlugin(0)
	, m_bWebServerEnabled( false )
	, m_bShuttingDown( false )
{
}

//============================================================================
RcWebServer::~RcWebServer()
{
}

//============================================================================
void RcWebServer::setPluginBaseWebServer( PluginBaseWebServer * poPlugin ) 
{ 
	m_WebPlugin = poPlugin; 
}

//============================================================================
//! set root directory of web site
void RcWebServer::setWebSiteDir( const char * pWebSiteDir ) 
{ 
	m_strWebSiteDir = pWebSiteDir; 
} 

//============================================================================
//! get root directory of web site
std::string& RcWebServer::getWebSiteDir(  void ) 
{ 
	return m_strWebSiteDir; 
}

//============================================================================
//=== overrides ===//
//============================================================================
//! handle callbacks from sockets
void RcWebServer::doReceiveCallback( VxSktBase * sktBase )
{
	this->HandleWebData( (RcWebSkt *) sktBase );
}

//============================================================================
//=== methods ===//
//============================================================================
//============================================================================
//! return true if web server is available
bool RcWebServer::isWebServerAvailable( void )
{
	#ifdef USE_WEB_FILESHARE
		return ( (false == VxIsAppShuttingDown() ) && 
			(true == m_bWebServerEnabled ) &&
			(100 == GetBuildProgress() ) );
	#else
		return (	(false == VxIsAppShuttingDown() ) && 
			(true == m_bWebServerEnabled ) );
	#endif //USE_WEB_FILESHARE
}

//============================================================================
// shutdown web server
void RcWebServer::webServerShutdown( void )
{
    if( IsLogEnabled( eLogSkt ) )
	    LogMsg( LOG_DEBUG, "RcWebServer::webServerShutdown\n" );
	setWebServerEnable( false );
    if( IsLogEnabled( eLogSkt ) )
	    LogMsg( LOG_DEBUG, "RcWebServer::webServerShutdown done\n" );
}

//============================================================================
//! set enable
void RcWebServer::setWebServerEnable( bool bEnable )
{
	m_bWebServerEnabled = bEnable;
}

//============================================================================
// accept connection
RCODE RcWebServer::acceptRawLocalHttpConnection( VxSktBase * poSktIn, VxNetIdent * netIdent, RcWebSkt ** ppoRetWebSkt )
{
	* ppoRetWebSkt = NULL;
	if( false == isWebServerAvailable() )
	{
		// we are shutting down or build is in progress
		return -1;
	}

	if( poSktIn )
	{
		RcWebSkt * sktBase		= new RcWebSkt();
		sktBase->setWebServerPlugin( m_WebPlugin );

		sktBase->setIdentity( netIdent );

		sktBase->setSktType( eSktTypeTcpAccept );
		sktBase->m_Socket		= poSktIn->m_Socket;
		poSktIn->m_Socket		= INVALID_SOCKET;

		sktBase->m_strRmtIp		= poSktIn->m_strRmtIp; 
		sktBase->m_RmtIp		= poSktIn->m_RmtIp; 

		sktBase->m_strLclIp		= poSktIn->m_strLclIp; 
		sktBase->m_LclIp		= poSktIn->m_LclIp; 

		// copy any data from the other socket
		int iDataLen = poSktIn->getSktBufDataLen();
		if( iDataLen )
		{
			uint8_t * pTempRead = poSktIn->getSktReadBuf();
			uint8_t * pTempWrite = sktBase->getSktWriteBuf();
			memcpy( pTempWrite, pTempRead, iDataLen );
			sktBase->sktBufAmountWrote( iDataLen );
			poSktIn->sktBufAmountRead( iDataLen );
			sktBase->m_iLastRxLen = iDataLen;
			sktBase->setLastActiveTimeMs(  poSktIn->getLastActiveTimeMs() );
			sktBase->setLastImAliveTimeMs( poSktIn->getLastImAliveTimeMs() );
		}

		sktBase->setReceiveCallback( m_pfnOurReceive, getRxCallbackUserData() );
		sktBase->setTransmitCallback( m_pfnOurTransmit, getTxCallbackUserData() );
		sktBase->setCallbackReason( eSktCallbackReasonConnecting );
		sktBase->m_SktMgr		= this;
		sktBase->m_WebMgr		= this;
		sktBase->setIsConnected( true );

		addSkt( sktBase );

        if( IsLogEnabled( eLogSkt ) )
		    LogMsg( LOG_DEBUG,  "skt %d to %s raw accept from skt %d\n", sktBase->m_SktNumber, sktBase->m_strRmtIp.c_str(), poSktIn->m_SktNumber );

		* ppoRetWebSkt = sktBase;

		return 0;
	}

	return -1;
}

//============================================================================
//! start skt threads for accepted web skt
void RcWebServer::startWebSktThreads( RcWebSkt * poWebSkt )
{
	poWebSkt->m_SktRxThread.startThread(  (VX_THREAD_FUNCTION_T)RcSktWebReceiveThreadFunc, poWebSkt, "RcWebSktRx" );
	poWebSkt->m_SktTxThread.startThread(  (VX_THREAD_FUNCTION_T)RcSktWebTransmitThreadFunc, poWebSkt, "RcWebSktTx" );
}

//============================================================================
//! thread function to receive data from socket
void * RcSktWebReceiveThreadFunc(  void * pvContext )
{
	VxThread * poThread = (VxThread *)pvContext;
	poThread->setIsThreadRunning( true );
	VxSktBase * sktBase = (VxSktBase *)poThread->getThreadUserParam();
    if( sktBase && false == poThread->isAborted() )
    {
        if(	( poThread->isAborted() )
            || ( INVALID_SOCKET == sktBase->m_Socket ) )
        {
            // something has already happened to the connection
            //! Thread calls this just before exit
            poThread->threadAboutToExit();
            return nullptr;
        }

        if( IsLogEnabled( eLogSkt ) )
            LogMsg( LOG_DEBUG,  "skt %d %s SktWebReceiveThreadFunc start\n", sktBase->m_SktNumber, sktBase->m_strRmtIp.c_str() );

        if( eSktTypeTcpAccept != sktBase->getSktType() )
        {
            LogMsg( LOG_ERROR, "ERROR skt %d SktWebReceiveThreadFunc is not an accept socket \n", sktBase->m_SktNumber );
            sktBase->m_bClosingFromRxThread = true;
            sktBase->closeSkt( eSktCloseHttpNotAcceptSkt );
            poThread->threadAboutToExit();
            return nullptr;
        }

        char as8Buf[ 0x8000 ];
        int iDataLen;
        int iBufferAlmostFull = sktBase->getSktBufSize() - ( sktBase->getSktBufSize() / 10 );

        sktBase->setIsConnected( true );
        sktBase->setCallbackReason( eSktCallbackReasonConnected );
        sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
        sktBase->setCallbackReason( eSktCallbackReasonData );
        if( sktBase->getSktBufDataLen() )
        {
            sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
        }

        sktBase->setSktBlocking( true );

        while(	( false == poThread->isAborted() )
                && ( INVALID_SOCKET != sktBase->m_Socket )
                && ( eSktCallbackReasonData == sktBase->getCallbackReason() ) )
        {
            int iAttemptLen = sktBase->getSktBufFreeSpace();
            if( iAttemptLen >= (int)sizeof( as8Buf ) )
            {
                iAttemptLen = (int)sizeof( as8Buf ) - 16;
            }

            if( iAttemptLen < iBufferAlmostFull )
            {
                // socket buffer is almost full.. let app empty some out
                VxSleep( 200 );
                continue;
            }

            if( IsLogEnabled( eLogSkt ) )
                LogMsg( LOG_DEBUG,  "skt %d SktWebReceiveThreadFunc wait for recv\n", sktBase->m_SktNumber );

            iDataLen = recv(		sktBase->m_Socket,	// socket
                                    as8Buf,				// buffer to read into
                                    iAttemptLen,		// length of buffer space
                                    0 );				// flags

            if( IsLogEnabled( eLogSkt ) )
                LogMsg( LOG_DEBUG,  "skt %d SktWebReceiveThreadFunc wait for recv done len %d\n", sktBase->m_SktNumber, iDataLen );

            if( poThread->isAborted()
                || ( eSktCallbackReasonData != sktBase->getCallbackReason() )
                || ( INVALID_SOCKET == sktBase->m_Socket )
                || ( iDataLen <= 0 ) )
            {
                if( ( iDataLen < 0 )
                    && ( false == poThread->isAborted() ) )
                {
                    // socket error occurred
                    sktBase->setLastSktError( VxGetLastError() );
                    if( 0 == sktBase->getLastSktError() )
                    {
                        sktBase->setLastSktError( -1 );
                    }
                }
                else
                {
                    sktBase->setLastSktError( 0 );
                }

                break;
            }

            if( iDataLen > 0 )
            {
                sktBase->updateLastActiveTime();
                sktBase->m_iLastRxLen = iDataLen;

                memcpy( sktBase->getSktWriteBuf(), as8Buf, iDataLen );
                sktBase->sktBufAmountWrote( iDataLen );
                sktBase->RxedPkt( iDataLen );
                // call back user with the good news.. we have data
                sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
            }
        }

        if( 0 != sktBase->getLastSktError() )
        {
            // we had a error
            sktBase->setCallbackReason( eSktCallbackReasonError );
            sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
        }

        if( false == poThread->isAborted() )
        {
            // we are closing due to error .. not because user called close
            sktBase->m_bClosingFromRxThread = true;
        }

    #ifdef DEBUG_SKTS
        LogMsg( LOG_INFO, "VxSktBaseReceiveVxThreadFunc: skt %d closing func 0x%x\n", sktBase->m_SktNumber, sktBase->m_pfnReceive );
    #endif // DEBUG_SKTS
        sktBase->setCallbackReason( eSktCallbackReasonClosing );
        sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
        sktBase->setCallbackReason( eSktCallbackReasonClosed );
        sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );

        #ifdef DEBUG_SKTS
        LogMsg( LOG_INFO, "VxSktBaseReceiveVxThreadFunc: skt %d 0x%x exiting\n", sktBase->m_SktNumber, sktBase );
        #endif // DEBUG_SKTS

        if( INVALID_SOCKET != sktBase->m_Socket )
        {
            sktBase->m_bClosingFromRxThread = true;
            sktBase->closeSkt( eSktCloseHttpCloseNoError );
        }

        sktBase->setCallbackReason( eSktCallbackReasonClosing );
        sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
        if( false == poThread->isAborted() )
        {
            // we are closing due to error .. not because user called close
            sktBase->m_bClosingFromRxThread = true;
            sktBase->closeSkt( eSktCloseHttpCloseWithError );
        }

        sktBase->setCallbackReason( eSktCallbackReasonClosed );
        sktBase->m_pfnReceive( sktBase, sktBase->getRxCallbackUserData() );
        if( sktBase->m_SktMgr )
        {
            sktBase->m_SktMgr->handleSktCloseEvent( sktBase );
        }
    }

	//! Thread calls this just before exit
	poThread->threadAboutToExit();
    return nullptr;
}

//============================================================================
bool RcWebServer::isExecuteableFile( std::string& strWebFileName )
{
	bool bIsExe = false;
	if(strWebFileName.length() >= 4)
	{
		const char * pTemp = (const char *)strWebFileName.c_str();
		const char * pExt = &pTemp[strWebFileName.length() - 4];
		if( ( 0 == stricmp( ".exe", pExt ) ) 
			|| ( 0 == stricmp( ".com", pExt ) ) 
			|| ( 0 == stricmp( ".bat", pExt ) ) )
		{
			bIsExe = true;
		}
	}

	return bIsExe;
}

#ifdef USE_WEB_FILESHARE
//============================================================================
//! return true if password has been set or no password required
bool RcWebServer::IsPasswordOk( uint32_t u32IpAddr, EWbMyFiles eFileType )
{
	if( false == IsMyFilesPwdProtected( eFileType ) )
	{
		return false;
	}
	bool bPwdOk = false;
	std::vector<VxSharedFilesAuth>::iterator iter;

	m_SktMgrMutex.lock(__FILE__,__LINE__);
	for( iter = m_aoAuthorizations.begin(); iter != m_aoAuthorizations.end(); ++iter )
	{
		if( (*iter).m_u32Ip == u32IpAddr )
		{
			bPwdOk =  (*iter).m_aoPwds[ eFileType ].m_strPwd.size()?1:0;
			break;
		}
	}
	m_SktMgrMutex.unlock(__FILE__,__LINE__);
	return bPwdOk;
}

//============================================================================
//! verify the password is correct for the given file share type
bool RcWebServer::VerifyPwd( uint32_t u32IpAddr, EWbMyFiles eFileType, const char * pPwd )
{
	if( false == IsMyFilesPwdProtected( eFileType ) )
	{
		return true;
	}
	bool	bPwdOk		= m_aoMyFiles[ eFileType ].VerifyPwd( pPwd );
	bool	bFoundEntry = false;
    int64_t		curTime	= GetTimeStampMs();

	std::vector<VxSharedFilesAuth>::iterator iter;
	m_SktMgrMutex.lock(__FILE__,__LINE__);
	for( iter = m_aoAuthorizations.begin(); iter != m_aoAuthorizations.end(); ++iter )
	{
		if( (*iter).m_u32Ip == u32IpAddr )
		{
			bFoundEntry = true;
			(*iter).m_u64LastAttemptTime = curTime;
			if( bPwdOk )
			{
				(*iter).m_aoPwds[ eFileType ].m_strPwd = pPwd;
				(*iter).m_aoPwds[ eFileType ].m_iPwdAttempts = 0;
			}
			else
			{
				(*iter).m_aoPwds[ eFileType ].m_iPwdAttempts++;
				if( (*iter).m_aoPwds[ eFileType ].m_iPwdAttempts > 6 )
				{
					VxReportHack( NULL, 2, u32IpAddr, "My Files Password Attempts > 6" );
					(*iter).m_aoPwds[ eFileType ].m_iPwdAttempts = 0;
				}
			}
			break;
		}
	}
	if( false == bFoundEntry )
	{
		// make a entry for next time
		VxSharedFilesAuth oAuth;
		oAuth.m_u32Ip = u32IpAddr;
		oAuth.m_LastAttemptTimeMs = curTime;
		if( bPwdOk )
		{
			oAuth.m_aoPwds[ eFileType ].m_strPwd = pPwd;
		}
		else
		{
			oAuth.m_aoPwds[ eFileType ].m_iPwdAttempts++;
		}

		m_aoAuthorizations.push_back( oAuth );
	}

	m_SktMgrMutex.unlock(__FILE__,__LINE__);

	return bPwdOk;
}

//============================================================================
//! called once per second for authorization timeouts
void RcWebServer::WebServerOncePerSecond( void )
{
	// every hour clean out old authorizations
static int iSecCnt = 0;
	iSecCnt++;
	if( iSecCnt < 3600 )
	{
		return;
	}
	// its been a hour
	iSecCnt = 0;

	std::vector<VxSharedFilesAuth>::iterator iter;
	int64_t curTime = time( NULL );

	m_SktMgrMutex.lock(__FILE__,__LINE__);
	for( iter = m_aoAuthorizations.begin(); iter != m_aoAuthorizations.end(); ++iter )
	{

		if( curTime - (*iter).m_LastAttemptTimeMs > 3600000 )
		{
			m_aoAuthorizations.erase( iter );
		}
	}

	m_SktMgrMutex.unlock(__FILE__,__LINE__);
};

#endif // USE_WEB_FILESHARE
