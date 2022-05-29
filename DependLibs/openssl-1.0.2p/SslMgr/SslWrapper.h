#ifndef SSL_WRAPPER_H
#define SSL_WRAPPER_H

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


#include "SslMgrCmdMgr.h"

#include <CoreLib/VxDebug.h>
#include <CoreLib/VxThread.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#include <string>
#include <vector>

class VxFileUtil;
class SslMgrCmdSendText;

class ISslCallback
{
public:
	virtual void				LogMsg( unsigned long u32LogMsgType, const char * logMsg ) = 0;

	virtual void				connectedToSslMgrServer( void ) = 0;
	virtual void				disconnectedFromSslMgrServer( void ) = 0;

	virtual void				sslMgrCmdComplete( ESslMgrCmdType cmdType ) = 0;
	virtual void				sslMgrCmdError( ESslMgrCmdType cmdType, int errCode, const char * errorText ) = 0;
	virtual void				sslMgrWentIdle( void ) = 0;

	virtual void				sslMgrListFile( const char * fileName ) = 0;

	virtual void				sslMgrFileGetBegin( const char * fileName ) = 0;
	virtual void				sslMgrFileGetProgress( const char * fileName, int percentComplete ) = 0;
	virtual void				sslMgrFileGetComplete( const char * fileName ) = 0;

	virtual void				sslMgrFileSendBegin( const char * fileName ) = 0;
	virtual void				sslMgrFileSendProgress( const char * fileName, int percentComplete ) = 0;
	virtual void				sslMgrFileSendComplete( const char * fileName ) = 0;
};

class SslWrapper
{
public:
	SslWrapper( ISslCallback& sslCallback, VxFileUtil& fileUtil );
    virtual ~SslWrapper() = default;

	ISslCallback&				getSslCallback( void )								{ return m_SslCallback; }
	SslMgrCmdMgr&				getCmdMgr( void )									{ return m_SslMgrCmdMgr; }
	VxFileUtil&					getFileUtil( void )									{ return m_FileUtil; }

	virtual void				setSslMgrCertFileName( const char * certFileName );
	virtual void				setSslMgrKeyFileName( const char * keyFileName );
	virtual void				setSslMgrServerIpAddr( const char * ipAddr );
	virtual void				setSslMgrServerPort( uint16_t port );
	virtual void				setSslMgrConnectRetryIntervalMs( int intervalMs );

	virtual void				connectToSslMgrServer( void );
	virtual void				disconnectFromSslMgrServer( void );
	virtual bool				isConnectedToSslMgrServer( void );

	int							doThreadFunction( void );
    void                        sslMonitorThreadFunction(void);
    
    void                        forceSSLDisconnect(void);

protected:
	void						initSslLib( void );
	bool						setupSslContext( void );
	void						cleanupSslContext( void );
	bool						verifyCertificatesToSend( void );
	bool						checkForAbort( void );
	bool						connectToServer( void );
	bool						verifyServerCertificate( void );
	int							runSslMgrCommands( void );
	int							executeSendTextCmd( SslMgrCmdSendText * sendTextCmd, char * buf, int bufSize );

	bool						willRetryConnectionToServer( void );
    
    


	ISslCallback&				m_SslCallback;
	VxFileUtil&					m_FileUtil;
    bool						m_bSslInitialized{false};

	std::string					m_strCertPemFileName;
	std::string					m_strKeyPemFileName;
	std::string					m_strSslMgrServerIp;
    uint16_t					m_u16SslMgrServerPort{0};
	VxThread					m_SslThread;
    
    VxThread                    m_SslMonitorThread;

    SSL *						m_Ssl{nullptr};
    SSL_CTX *					m_SslCtx{nullptr};
	int							m_ConnectRetryIntervalMs;
    bool						m_bIsConnectedToServer{false};
    int							m_SslSocket{0};
    
    bool                        m_NegotiationSSL{false};
    
	SslMgrCmdMgr				m_SslMgrCmdMgr;
    
    VxTimer                     m_MonitorTimer;
};

#endif //SSL_WRAPPER_H
