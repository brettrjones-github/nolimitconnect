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
// http://www.nolimitconnect.org
//============================================================================

#include "SslWrapper.h"
#include "SslMgrCmdSendText.h"

#include <NetLib/VxSktUtil.h>

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxFileUtil.h>
#include <CoreLib/VxParse.h>
#include <CoreLib/StdOutRedirect.h>
#include <CoreLib/VxGlobals.h>
#include <assert.h>

#ifndef WIN32
    #include <unistd.h>
    #include <fcntl.h>
#endif

#if defined(_MSC_VER)
#pragma comment(lib, "crypt32.lib") // for windows CertOpenStore etc
#endif // defined(_MSC_VER)

#if defined(_MSC_VER)
# pragma warning( disable: 4355 ) //'this' : used in base member initializer list
#endif // _MSC_VER


#ifdef __cplusplus
extern "C" {
#endif

	extern int doOpenSslMain(int Argc, char *ARGV[]);

#ifdef __cplusplus
}
#endif

namespace
{
	const char * DEFAULT_CERT_FILE_NAME			= "server.pem";
	const char * DEFAULT_KEY_FILE_NAME			= "server.key.pem";
	const char * DEFAULT_SSLMGR_SERVER_IP		= "100.0.108.123";
	const uint16_t	DEFAULT_SSLMGR_SERVER_PORT		= 8001;

	const int	CONNECT_RETRY_INTERVAL_MS		= 5000;


    void * SslWrapperThreadFunc( void * pvContext )
	{
		VxThread * poVxThread = (VxThread *)pvContext;
		poVxThread->setIsThreadRunning( true );
		SslWrapper * sslWrapper = (SslWrapper *)poVxThread->getThreadUserParam();
		int result = sslWrapper->doThreadFunction();
		if( result )
		{
			//LogMsg( LOG_ERROR, "SslWrapperThreadFunc Error %d\n", result );
		}

		//LogMsg( LOG_ERROR, "SslWrapperThreadFunc: About To Exit\n" );
		poVxThread->threadAboutToExit();
        return nullptr;
	}
    
   
    void * SslWrapperMonitorFunc( void * pvContext )
	{
		VxThread * poVxThread = (VxThread *)pvContext;
		poVxThread->setIsThreadRunning( true );
		SslWrapper * sslWrapper = (SslWrapper *)poVxThread->getThreadUserParam();
        if( sslWrapper )
        {
            sslWrapper->sslMonitorThreadFunction();
        }

		poVxThread->threadAboutToExit();
        return nullptr;
	}
}

//============================================================================
SslWrapper::SslWrapper( ISslCallback& sslCallback, VxFileUtil& fileUtil )
: m_SslCallback( sslCallback )
, m_FileUtil( fileUtil )
, m_strCertPemFileName( DEFAULT_CERT_FILE_NAME )
, m_strKeyPemFileName( DEFAULT_KEY_FILE_NAME )
, m_strSslMgrServerIp( DEFAULT_SSLMGR_SERVER_IP )
, m_u16SslMgrServerPort( DEFAULT_SSLMGR_SERVER_PORT )
, m_ConnectRetryIntervalMs( CONNECT_RETRY_INTERVAL_MS )
, m_SslMgrCmdMgr( *this )
{
}

//============================================================================
void SslWrapper::initSslLib( void )
{
	if( false == m_bSslInitialized )
	{
		m_bSslInitialized = true;

		/*----------------------------------------------------------*/      
		/* Load encryption & hashing algorithms for the SSL program */  
		BIO_sock_init();
		SSL_library_init();				/* Load the error strings for SSL & CRYPTO APIs */      
		SSL_load_error_strings();       /* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
        
        m_SslMonitorThread.startThread( (VX_THREAD_FUNCTION_T)SslWrapperMonitorFunc, this, "SslMonitorThread" );
	}

}

//============================================================================
void SslWrapper::setSslMgrCertFileName( const char * certFileName )
{
	m_strCertPemFileName = certFileName;
}

//============================================================================
void SslWrapper::setSslMgrKeyFileName( const char * keyFileName )
{
	m_strKeyPemFileName = keyFileName;
}

//============================================================================
void SslWrapper::setSslMgrServerIpAddr( const char * ipAddr )
{
	m_strSslMgrServerIp = ipAddr;
}

//============================================================================
void SslWrapper::setSslMgrServerPort( uint16_t port )
{
	m_u16SslMgrServerPort = port;
}

//============================================================================
void SslWrapper::setSslMgrConnectRetryIntervalMs( int intervalMs )
{
	m_ConnectRetryIntervalMs = intervalMs;
}

//============================================================================
void SslWrapper::connectToSslMgrServer( void )
{
	m_SslThread.killThread();
    
    initSslLib();
    
	if( false == m_SslThread.isThreadRunning() )
	{
		// clear out any old commands
		getCmdMgr().endSslMgrCmds();
		m_SslThread.startThread( (VX_THREAD_FUNCTION_T)SslWrapperThreadFunc, this, "SslWrapperThread" );
	}
}

//============================================================================
void SslWrapper::disconnectFromSslMgrServer( void )
{
	if( m_SslThread.isThreadRunning() )
	{
		LogMsg( LOG_ERROR, "SslWrapper: aborting thread\n" );
		m_SslThread.abortThreadRun( true );
	}
	else
	{
		LogMsg( LOG_ERROR, "SslWrapper: Thread is NOT running\n" );
	}
}

//============================================================================
int SslWrapper::doThreadFunction( void )
{
	assert( true == m_bSslInitialized );

	if( false == setupSslContext() )
	{
		return -1;
	}

	if( checkForAbort() )
	{
		cleanupSslContext();
		return -2;
	}

	if( false == verifyCertificatesToSend() )
	{
		cleanupSslContext();
		return false;
	}

	if( checkForAbort() )
	{
		cleanupSslContext();
		return -2;
	}

	if( false == connectToServer() )
	{
		cleanupSslContext();
		return -3;
	}

	if( false == verifyServerCertificate() )
	{
		// do nothing
		//cleanupSslContext();
		//return -4;
	}

	int retVal = runSslMgrCommands();
    LogMsg( LOG_INFO, "runSslMgrCommands: returned error %d\n", retVal );
	if( retVal )
	{
		//LogMsg( LOG_INFO, "runSslMgrCommands: returned error %d\n", retVal );         
	}

	getCmdMgr().endSslMgrCmds();

	cleanupSslContext();

	getSslCallback().disconnectedFromSslMgrServer();

	return retVal;
}

//============================================================================
bool SslWrapper::setupSslContext( void )
{
	m_bIsConnectedToServer = false;
	m_Ssl					= NULL;
	SSL_METHOD *	meth	= (SSL_METHOD *)SSLv3_method();  /* Create an SSL_CTX structure */ 
	m_SslCtx				= SSL_CTX_new(meth);
	if( NULL == m_SslCtx )
	{
		LogMsg( LOG_ERROR, "setupSslContext: NULL m_SslCtx\n" );
		return false;
	}

	return true;
}

//============================================================================
void SslWrapper::cleanupSslContext( void )
{
	//LogMsg( LOG_ERROR, "cleanupSslContext: Closing SSL\n" );

	if( m_Ssl )
	{
		/* Free the SSL structure */   
		SSL_free( m_Ssl );  
		m_Ssl = NULL;
	}

	if( m_SslCtx )
	{
		/* Free the SSL_CTX structure */        
		SSL_CTX_free( m_SslCtx );
		m_SslCtx = NULL;
	}
}

//============================================================================
bool SslWrapper::verifyCertificatesToSend( void )
{
	/* Load the client certificate into the SSL_CTX structure */                
	if( 0 >= SSL_CTX_use_certificate_file( m_SslCtx, m_strCertPemFileName.c_str(), SSL_FILETYPE_PEM ) ) 
	{                   
		ERR_print_errors_fp(stderr);                        
		return false;            
	} 

	/* Load the private-key corresponding to the client certificate */          
	if( 0 >= SSL_CTX_use_PrivateKey_file( m_SslCtx, m_strKeyPemFileName.c_str(), SSL_FILETYPE_PEM ) ) 
	{                     
		ERR_print_errors_fp(stderr);                        
		return false;            
	}    

	/* Check if the client certificate and private-key matches */               
	if (!SSL_CTX_check_private_key( m_SslCtx ) ) 
	{                      
		LogMsg( LOG_ERROR, "Private key does not match the certificate public key\n" );                    
		return false;            
	}   

	return true;
}

//============================================================================
bool SslWrapper::checkForAbort( void )
{
	return m_SslThread.isAborted();
}

//============================================================================
bool SslWrapper::connectToServer( void )
{
	struct sockaddr_in server_addr;     

	while( false == checkForAbort() )
	{
		m_SslSocket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
		if( 0 >= m_SslSocket )
		{
			LogMsg( LOG_ERROR, "connectToSslMgrServer: sock %d.. did you initialize sockets?\n", m_SslSocket );
			return false;
		}

		memset (&server_addr, '\0', sizeof(server_addr));       
		server_addr.sin_family      = AF_INET;  
		server_addr.sin_port        = htons( m_u16SslMgrServerPort );       
		/* Server Port number */     
		server_addr.sin_addr.s_addr = inet_addr( m_strSslMgrServerIp.c_str() ); 
		/* Server IP */      
		/* Establish a TCP/IP connection to the SSL client */           
		int err = connect( m_SslSocket, (struct sockaddr*) &server_addr, sizeof(server_addr) );
		if( 0 == err )
		{
			m_MonitorTimer.startTimer( );
            m_NegotiationSSL = true;
            
            LogMsg( LOG_INFO, "connectToSslMgrServer: TCP Connected to %s:%d.. attempting SSL Handshake\n", m_strSslMgrServerIp.c_str(), m_u16SslMgrServerPort );
			// NOTE SSL has to be created after connection is made.. what crap
			m_Ssl = SSL_new( m_SslCtx );
			/* Assign the socket into the SSL structure (SSL and socket without BIO) */         
			SSL_set_fd( m_Ssl, m_SslSocket );  /* Perform SSL Handshake on the SSL client */
			err = SSL_connect( m_Ssl );
            
            m_NegotiationSSL = false;

			if( 1 == err )
			{
				verifyServerCertificate();

				m_bIsConnectedToServer = true;
				LogMsg( LOG_INFO, "connectToSslMgrServer: SUCCESS SSL connection using %s\n", SSL_get_cipher ( m_Ssl ));     
				return true;
			}
			else
			{
				LogMsg( LOG_ERROR, "connectToSslMgrServer: SSL_connect error %d \n", err );
			}
		}
		else
		{
            LogMsg( LOG_ERROR, "connectToSslMgrServer: connect error %d\n", err );
		}


		if( false == willRetryConnectionToServer() )
		{
			// don't retry
			getSslCallback().disconnectedFromSslMgrServer();
			return false;
		}

		LogMsg( LOG_ERROR, "connectToSslMgrServer: will retry connect in %d millisec to ip %s\n", m_ConnectRetryIntervalMs, m_strSslMgrServerIp.c_str() );
		int iLoopCnt = m_ConnectRetryIntervalMs / 300;
		for( int i = 0; i < iLoopCnt; i++ )
		{
			if( checkForAbort() )
			{
				return false;
			}
			VxSleep( 300 );
		}
	}
	return false;
}

//============================================================================
bool SslWrapper::verifyServerCertificate( void )
{
	X509 *			server_cert;  
	char  *			str;       

	/* Get the server's certificate (optional) */       
	server_cert = SSL_get_peer_certificate( m_Ssl );    
	if (server_cert != NULL)        
	{               
		str = X509_NAME_oneline(X509_get_subject_name(server_cert),0,0); 
		if( NULL == str )
		{
			LogMsg( LOG_INFO, "Server certificate: NULL Subject\n");
			X509_free (server_cert);
			return false;
		}

		//LogMsg( LOG_INFO, "Server certificate: %s\n", str ); 
		printf ("\t subject: %s\n", str);           
		free (str);   

		str = X509_NAME_oneline(X509_get_issuer_name(server_cert),0,0);             
		if( NULL == str )
		{
			LogMsg( LOG_INFO, "Server certificate: NULL Issuer Name\n");
			X509_free (server_cert);
			return false;
		}

		//LogMsg( LOG_INFO, "Server certificate: Issuer %s\n", str ); 
		free(str);              
		X509_free (server_cert);
	}        
	else
	{
		LogMsg( LOG_ERROR, "The SSL server does not have certificate.\n");
		return false;
	}

	return true;
}

//============================================================================
void SslWrapper::forceSSLDisconnect(void)
{
    SSL_shutdown(m_Ssl);
}


//============================================================================
bool SslWrapper::isConnectedToSslMgrServer( void )
{
	if( false == m_bIsConnectedToServer )
	{
		LogMsg( LOG_INFO, "SslWrapper: not connected to server\n" ); 
	}

	return m_bIsConnectedToServer;
}

//============================================================================
bool SslWrapper::willRetryConnectionToServer( void )
{
	return m_ConnectRetryIntervalMs ? true : false;
}

//============================================================================
int SslWrapper::runSslMgrCommands( void )
{
#define SSL_BUF_SIZE 4096
	bool	readOccurred;
	int		width;
	int		r;
	int		read_blocked_on_write	= 0;
	int		write_blocked_on_read	= 0;
	int		read_blocked			= 0;
	fd_set	readfds;
	fd_set	writefds;
	int		shutdown_wait			= 0;

	int		sslWriteLen				= 0;
	int		sslWriteOffs			= 0;
	char	sslWriteBuf[ SSL_BUF_SIZE ];

	char	sslReadBuf[ SSL_BUF_SIZE ];

	// set socket to non blocking
#ifdef WIN32
	u_long s32NonBlock = true; 
	int iResult = ioctlsocket( m_SslSocket, FIONBIO, &s32NonBlock );
	if( iResult != 0 )
	{
		RCODE rc = VxGetLastError();
		LogMsg( 0, "setSktBlocking ioctlsocket error %s\n", VxDescribeSktError( rc ) );
	}
#else
	int iFlags = fcntl(m_SslSocket, F_GETFL, NULL);
	if( 0 >= iFlags )
	{
		LogMsg( 0, "setSktBlocking fcntl error\n" );
	}
	else
	{
		iFlags |= O_NONBLOCK;
		//LogMsg( 0, "setSktBlocking non block flags %d\n", iFlags );
		fcntl( m_SslSocket, F_SETFL, iFlags);
	}
#endif

	width = m_SslSocket + 1;

	getCmdMgr().beginSslMgrCmds();

	while( false == checkForAbort() )
	{
		getCmdMgr().updateSslMgrCmds();
		FD_ZERO( &readfds );
		FD_ZERO( &writefds );

		FD_SET( m_SslSocket, &readfds );

		/* If we're waiting for a read on the socket don't
		try to write to the server */
		if( !write_blocked_on_read )
		{
			/* If we have data in the write queue don't try to
			read from stdin */
			if( sslWriteLen || read_blocked_on_write)
			{
				FD_SET( m_SslSocket, &writefds );
			}
			else
			{
#ifdef WIN32
				FD_SET( _fileno(stdin), &readfds );
#else
				FD_SET(  fileno(stdin), &readfds );
#endif
			}
		}

		r = select( width, &readfds, &writefds, 0, 0 );
		if( r == 0 )
		{
			// nothing to read or write
			VxSleep(100);
            continue;
		}

		readOccurred = false;

		/* Now check if there's data to read */
		if((FD_ISSET( m_SslSocket,&readfds ) && !write_blocked_on_read) ||
			(read_blocked_on_write && FD_ISSET( m_SslSocket, &writefds )))
		{
			do 
			{
				read_blocked_on_write=0;
				read_blocked=0;

				r = SSL_read( m_Ssl, sslReadBuf, SSL_BUF_SIZE - 1 );
				readOccurred = true;

				switch(SSL_get_error( m_Ssl,r ) )
				{
				case SSL_ERROR_NONE:
					/* Note: this call could block, which blocks the
					entire application. It's arguable this is the
					right behavior since this is essentially a terminal
					client. However, in some other applications you
					would have to prevent this condition */
					sslReadBuf[ r ] = 0;
					getCmdMgr().handleDataFromSslMgrServer( sslReadBuf, r );
					break;

				case SSL_ERROR_ZERO_RETURN:
					/* End of data */
					if( !shutdown_wait )
					{
						SSL_shutdown( m_Ssl );
					}
					LogMsg( LOG_ERROR, "runSslMgrCommands: SSL_ERROR_ZERO_RETURN\n" );
					return -1;
					break;

				case SSL_ERROR_WANT_READ:
					read_blocked=1;
					break;

					/* We get a WANT_WRITE if we're
					trying to rehandshake and we block on
					a write during that rehandshake.

					We need to wait on the socket to be 
					writeable but reinitiate the read
					when it is */
				case SSL_ERROR_WANT_WRITE:
					read_blocked_on_write=1;
					break;

				default:
					LogMsg( LOG_ERROR, "Unknown SSL read problem %d \n", r );
					return -1;
				}

					/* We need a check for read_blocked here because
					SSL_pending() doesn't work properly during the
					handshake. This check prevents a busy-wait
					loop around SSL_read() */
			} 
			while( SSL_pending( m_Ssl ) && !read_blocked && ( false == checkForAbort() ));
		}

		if( false == readOccurred )
		{
			getCmdMgr().handleDelays();
		}

		// see if anything to write
		if( 0 == sslWriteLen )
		{

			sslWriteLen = getCmdMgr().getDataToWriteToSslMgrServer( sslWriteBuf, SSL_BUF_SIZE );
			sslWriteOffs = 0;
		}

		// If the socket is writeable... 
		if( ( FD_ISSET( m_SslSocket, &writefds ) && sslWriteLen) ||
			( write_blocked_on_read && FD_ISSET( m_SslSocket, &readfds ) ) ) 
		{
			write_blocked_on_read=0;

			// Try to write 
			r = SSL_write( m_Ssl, sslWriteBuf + sslWriteOffs, sslWriteLen );

			switch(SSL_get_error( m_Ssl, r ) )
			{
				/* We wrote something*/
			  case SSL_ERROR_NONE:
				  sslWriteLen -= r;
				  sslWriteOffs += r;
				  break;

				  /* We would have blocked */
			  case SSL_ERROR_WANT_WRITE:
				  break;

				  /* We get a WANT_READ if we're
				  trying to rehandshake and we block on
				  write during the current connection.

				  We need to wait on the socket to be readable
				  but reinitiate our write when it is */
			  case SSL_ERROR_WANT_READ:
				  write_blocked_on_read=1;
				  break;

				  /* Some other error */
			  default:	      
				  LogMsg( LOG_ERROR, "runSslMgrCommands: Unknown SSL write problem %d\n", r );
				  return -1;
			}
		}
		else
		{
			getCmdMgr().handleDelays();
		}
	}
	return 0;
}


void SslWrapper::sslMonitorThreadFunction(void)
{
    while (false == m_SslMonitorThread.isAborted() && false == VxIsAppShuttingDown()) {
        if (m_NegotiationSSL) {
            if (10 < m_MonitorTimer.elapsedSec())
            {
				SOCKET skt = m_SslSocket;
                VxCloseSkt( skt );
                m_NegotiationSSL = false;
            }
        }
        VxSleep(300);
    }
}

